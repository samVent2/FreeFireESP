//
// Free Fire ESP with ImGui Menu - Main Entry Point
// Version 1.118.1
//

#include <cstring>
#include <jni.h>
#include <pthread.h>
#include <unistd.h>
#include <dlfcn.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>

#include "game.h"
#include "zygisk.hpp"
#include "Includes/Logger.h"
#include "Includes/Utils.h"
#include "Includes/obfuscate.h"
#include "Includes/Dobby/dobby.h"
#include "ImGui/imgui.h"
#include "ImGui/backends/imgui_impl_opengl3.h"
#include "ImGui/backends/imgui_impl_android.h"
#include "Includes/Roboto-Regular.h"
#include "ESP_Menu.h"
#include "FreeFire_ESP.h"
#include "AntiBan.h"

using zygisk::Api;
using zygisk::AppSpecializeArgs;
using zygisk::ServerSpecializeArgs;

// Global variables
static bool g_ModEnabled = false;
static char* g_GameDataDir = nullptr;
static bool g_ImGuiInitialized = false;
static int g_ScreenWidth = 0;
static int g_ScreenHeight = 0;

// Function pointers
using swapbuffers_t = EGLBoolean (*)(EGLDisplay dpy, EGLSurface surf);
swapbuffers_t original_swapBuffers = nullptr;

using inject_event_t = int (*)(JNIEnv *env, jobject thiz, jobject inputEvent);
inject_event_t original_injectEvent = nullptr;

// Check if this is the target game
int IsTargetGame(JNIEnv *env, jstring appDataDir) {
    if (!appDataDir) return 0;
    
    const char *app_data_dir = env->GetStringUTFChars(appDataDir, nullptr);
    static char package_name[256];
    
    if (sscanf(app_data_dir, OBFUSCATE("/data/%*[^/]/%d/%s"), 0, package_name) != 2) {
        if (sscanf(app_data_dir, OBFUSCATE("/data/%*[^/]/%s"), package_name) != 1) {
            package_name[0] = '\0';
            LOGW(OBFUSCATE("Can't parse %s"), app_data_dir);
            env->ReleaseStringUTFChars(appDataDir, app_data_dir);
            return 0;
        }
    }
    
    if (strcmp(package_name, GamePackageName) == 0) {
        LOGI(OBFUSCATE("Target game detected: %s"), package_name);
        g_GameDataDir = new char[strlen(app_data_dir) + 1];
        strcpy(g_GameDataDir, app_data_dir);
        env->ReleaseStringUTFChars(appDataDir, app_data_dir);
        return 1;
    }
    
    env->ReleaseStringUTFChars(appDataDir, app_data_dir);
    return 0;
}

// Input event hook for ImGui
int InjectEvent_Hook(JNIEnv *env, jobject thiz, jobject input_event) {
    if (!g_ImGuiInitialized) {
        return original_injectEvent(env, thiz, input_event);
    }
    
    jclass motion_event = env->FindClass(OBFUSCATE("android/view/MotionEvent"));
    if (motion_event && env->IsInstanceOf(input_event, motion_event)) {
        jmethodID get_action = env->GetMethodID(motion_event, "getActionMasked", "()I");
        jmethodID get_x = env->GetMethodID(motion_event, "getX", "()F");
        jmethodID get_y = env->GetMethodID(motion_event, "getY", "()F");
        jmethodID get_pointer_count = env->GetMethodID(motion_event, "getPointerCount", "()I");
        
        ImGui_ImplAndroid_HandleInputEvent(
            env->CallIntMethod(input_event, get_action),
            env->CallFloatMethod(input_event, get_x),
            env->CallFloatMethod(input_event, get_y),
            env->CallIntMethod(input_event, get_pointer_count)
        );
        
        ImGuiIO &io = ImGui::GetIO();
        if (io.WantCaptureMouse) {
            return true; // Block input to game when interacting with menu
        }
    }
    
    return original_injectEvent(env, thiz, input_event);
}

// Setup ImGui
void SetupImGui() {
    if (g_ImGuiInitialized) return;
    
    LOGI("Setting up ImGui...");
    
    auto ctx = ImGui::CreateContext();
    if (!ctx) {
        LOGE("Failed to create ImGui context");
        return;
    }
    
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float)g_ScreenWidth, (float)g_ScreenHeight);
    io.ConfigWindowsMoveFromTitleBarOnly = true;
    io.IniFilename = nullptr;
    
    // Setup backends
    ImGui_ImplAndroid_Init();
    ImGui_ImplOpenGL3_Init("#version 300 es");
    
    // Load font
    int systemScale = (1.0 / g_ScreenWidth) * g_ScreenWidth;
    ImFontConfig font_cfg;
    font_cfg.SizePixels = systemScale * 22.0f;
    io.Fonts->AddFontFromMemoryTTF(Roboto_Regular, systemScale * 30.0, 40.0f);
    
    // Apply iOS style
    ApplyiOSStyle();
    ImGui::GetStyle().ScaleAllSizes(7);
    
    g_ImGuiInitialized = true;
    LOGI("ImGui initialized successfully");
}

// Render ImGui frame
void RenderImGui() {
    if (!g_ImGuiInitialized) return;
    
    ImGuiIO &io = ImGui::GetIO();
    
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame(g_ScreenWidth, g_ScreenHeight);
    ImGui::NewFrame();
    
    // Draw ESP
    RenderESP();
    
    // Draw Menu
    DrawESPMenu();
    
    // Anti-Ban update
    AntiBan::Update();
    
    ImGui::Render();
    ImGui::EndFrame();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
}

// SwapBuffers hook
EGLBoolean SwapBuffers_Hook(EGLDisplay dpy, EGLSurface surf) {
    // Get screen dimensions
    EGLint w, h;
    eglQuerySurface(dpy, surf, EGL_WIDTH, &w);
    eglQuerySurface(dpy, surf, EGL_HEIGHT, &h);
    g_ScreenWidth = w;
    g_ScreenHeight = h;
    
    // Setup ImGui on first frame
    if (!g_ImGuiInitialized) {
        SetupImGui();
    }
    
    // Render our overlay
    RenderImGui();
    
    return original_swapBuffers(dpy, surf);
}

// Hook initialization
void InitializeHooks() {
    LOGI("Initializing hooks...");
    
    // Wait for libEGL.so to load
    do {
        sleep(1);
    } while (!isLibraryLoaded(OBFUSCATE("libEGL.so")));
    
    LOGI("libEGL.so loaded, hooking eglSwapBuffers...");
    
    // Hook eglSwapBuffers
    auto swapBuffers = (uintptr_t)DobbySymbolResolver(OBFUSCATE("libEGL.so"), 
                                                       OBFUSCATE("eglSwapBuffers"));
    if (swapBuffers) {
        DobbyHook((void*)swapBuffers, (void*)SwapBuffers_Hook, 
                 (void**)&original_swapBuffers);
        LOGI("eglSwapBuffers hooked successfully");
    } else {
        LOGE("Failed to find eglSwapBuffers");
    }
    
    // Hook input events for Unity games
    uintptr_t inject_event = find_pattern_in_module(
        OBFUSCATE("libunity.so"), 
        OBFUSCATE("FF 83 01 D1 F7 5B 03 A9 F5 53 04 A9 F3 7B 05 A9 F5 03 02 AA F9 35 FF 97")
    );
    
    if (inject_event != 0) {
        DobbyHook((void*)inject_event, (void*)InjectEvent_Hook, 
                 (void**)&original_injectEvent);
        LOGI("Input event hooked successfully");
    } else {
        LOGW("Input event hook not found (may not be needed)");
    }
    
    // Wait for libil2cpp.so to load
    do {
        sleep(1);
    } while (!isLibraryLoaded(OBFUSCATE("libil2cpp.so")));
    
    LOGI("libil2cpp.so loaded");
    uintptr_t il2cpp_base = getBaseAddress(OBFUSCATE("libil2cpp.so"));
    
    // Initialize ESP system
    InitializeESP(il2cpp_base, g_ScreenWidth, g_ScreenHeight);
    
    LOGI("All hooks initialized");
}

// Main mod thread
void* ModThread(void*) {
    LOGI("Mod thread started (TID: %d)", gettid());
    
    // Initialize anti-ban system
    AntiBan::Initialize();
    
    // Initialize hooks
    InitializeHooks();
    
    LOGI("Mod initialization complete");
    return nullptr;
}

// Zygisk Module Implementation
class FreeFireESPModule : public zygisk::ModuleBase {
public:
    void onLoad(Api *api, JNIEnv *env) override {
        env_ = env;
        LOGI("Zygisk module loaded");
    }
    
    void preAppSpecialize(AppSpecializeArgs *args) override {
        if (!args || !args->nice_name) {
            return;
        }
        g_ModEnabled = IsTargetGame(env_, args->app_data_dir);
    }
    
    void postAppSpecialize(const AppSpecializeArgs *) override {
        if (g_ModEnabled) {
            LOGI("Starting Free Fire ESP mod...");
            int ret;
            pthread_t thread;
            if ((ret = pthread_create(&thread, nullptr, ModThread, nullptr))) {
                LOGE("Failed to create mod thread: %s", strerror(ret));
            } else {
                pthread_detach(thread);
            }
        }
    }
    
private:
    JNIEnv *env_{};
};

REGISTER_ZYGISK_MODULE(FreeFireESPModule)
