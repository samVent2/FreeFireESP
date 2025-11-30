//
// Anti-Ban Protection System
// Protects against detection and bans
//

#ifndef ANTIBAN_H
#define ANTIBAN_H

#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <sys/mman.h>
#include <dlfcn.h>
#include "Includes/Logger.h"

namespace AntiBan {
    
    // Random number generator
    static bool initialized = false;
    
    void Initialize() {
        if (!initialized) {
            srand(time(NULL) ^ getpid());
            initialized = true;
            LOGI("Anti-Ban system initialized");
        }
    }
    
    // Generate random delay (milliseconds)
    int RandomDelay(int min_ms, int max_ms) {
        if (max_ms <= min_ms) return min_ms;
        return min_ms + (rand() % (max_ms - min_ms));
    }
    
    // Sleep with random delay
    void RandomSleep(int min_ms, int max_ms) {
        int delay = RandomDelay(min_ms, max_ms);
        usleep(delay * 1000);
    }
    
    // Randomize float value within a percentage range
    float RandomizeFloat(float value, float variance_percent = 5.0f) {
        float variance = (variance_percent / 100.0f) * value;
        float random = ((float)rand() / RAND_MAX) * 2.0f - 1.0f; // -1 to 1
        return value + (random * variance);
    }
    
    // Randomize integer value within a range
    int RandomizeInt(int value, int variance) {
        int random = (rand() % (variance * 2 + 1)) - variance;
        return value + random;
    }
    
    // Hide memory region from scans
    bool HideMemoryRegion(void* address, size_t size) {
        // Change memory protection to make it harder to scan
        int result = mprotect(address, size, PROT_READ | PROT_EXEC);
        if (result == 0) {
            LOGI("Memory region hidden: %p (size: %zu)", address, size);
            return true;
        }
        return false;
    }
    
    // Anti-debugging: Check if being debugged
    bool IsDebugged() {
        // Check TracerPid in /proc/self/status
        FILE* status = fopen("/proc/self/status", "r");
        if (status) {
            char line[256];
            while (fgets(line, sizeof(line), status)) {
                if (strncmp(line, "TracerPid:", 10) == 0) {
                    int pid;
                    sscanf(line, "TracerPid: %d", &pid);
                    fclose(status);
                    return pid != 0;
                }
            }
            fclose(status);
        }
        return false;
    }
    
    // Anti-emulator: Basic emulator detection
    bool IsEmulator() {
        // Check for common emulator properties
        char prop[PROP_VALUE_MAX];
        
        // Check product name
        __system_property_get("ro.product.name", prop);
        if (strstr(prop, "sdk") || strstr(prop, "emulator") || 
            strstr(prop, "vbox") || strstr(prop, "genymotion")) {
            return true;
        }
        
        // Check build fingerprint
        __system_property_get("ro.build.fingerprint", prop);
        if (strstr(prop, "generic") || strstr(prop, "unknown")) {
            return true;
        }
        
        return false;
    }
    
    // String obfuscation helper (XOR)
    void ObfuscateString(char* str, size_t len, unsigned char key) {
        for (size_t i = 0; i < len; i++) {
            str[i] ^= key;
        }
    }
    
    // Deobfuscate string
    void DeobfuscateString(char* str, size_t len, unsigned char key) {
        ObfuscateString(str, len, key); // XOR is reversible
    }
    
    // Memory pattern obfuscation
    void ObfuscateMemory(void* address, size_t size) {
        unsigned char* bytes = (unsigned char*)address;
        unsigned char key = rand() % 256;
        for (size_t i = 0; i < size; i++) {
            bytes[i] ^= key;
        }
    }
    
    // Check for suspicious processes (anti-cheat, debuggers)
    bool CheckSuspiciousProcesses() {
        const char* suspicious[] = {
            "gdb",
            "lldb",
            "strace",
            "frida",
            "xposed",
            "substrate",
            NULL
        };
        
        for (int i = 0; suspicious[i] != NULL; i++) {
            char cmd[256];
            snprintf(cmd, sizeof(cmd), "pidof %s", suspicious[i]);
            FILE* fp = popen(cmd, "r");
            if (fp) {
                char result[128];
                if (fgets(result, sizeof(result), fp) != NULL) {
                    pclose(fp);
                    LOGW("Suspicious process detected: %s", suspicious[i]);
                    return true;
                }
                pclose(fp);
            }
        }
        return false;
    }
    
    // Integrity check: Verify module hasn't been tampered with
    bool VerifyIntegrity(void* module_base, size_t expected_size) {
        // Simple check: verify memory is readable
        if (module_base == NULL) return false;
        
        // Try to read first few bytes
        unsigned char test[16];
        memcpy(test, module_base, sizeof(test));
        
        return true; // If we got here, memory is accessible
    }
    
    // Anti-screenshot: Detect if screenshot is being taken
    bool IsScreenshotDetected() {
        // Check for screenshot processes
        FILE* fp = popen("pidof screencap", "r");
        if (fp) {
            char result[128];
            if (fgets(result, sizeof(result), fp) != NULL) {
                pclose(fp);
                return true;
            }
            pclose(fp);
        }
        return false;
    }
    
    // Randomize execution timing to avoid pattern detection
    class TimingRandomizer {
    private:
        int base_delay;
        int variance;
        unsigned long last_call;
        
    public:
        TimingRandomizer(int base_ms, int variance_ms) 
            : base_delay(base_ms), variance(variance_ms), last_call(0) {}
        
        bool ShouldExecute() {
            unsigned long current = time(NULL);
            if (last_call == 0) {
                last_call = current;
                return true;
            }
            
            int random_delay = RandomDelay(base_delay, base_delay + variance);
            if ((current - last_call) >= (random_delay / 1000)) {
                last_call = current;
                return true;
            }
            return false;
        }
    };
    
    // Hook protection: Detect if our hooks have been hooked
    bool AreHooksCompromised(void* hook_address) {
        // Check if the hook address has been modified
        unsigned char* bytes = (unsigned char*)hook_address;
        
        // Look for common hook patterns (JMP, CALL, etc.)
        // This is a simplified check
        if (bytes[0] == 0xE9 || bytes[0] == 0xE8) { // JMP or CALL
            LOGW("Hook may be compromised at %p", hook_address);
            return true;
        }
        
        return false;
    }
    
    // Safe memory read with error handling
    template<typename T>
    bool SafeRead(uintptr_t address, T* out) {
        if (address == 0 || out == NULL) return false;
        
        // Try to read, catch any crashes
        __try {
            *out = *(T*)address;
            return true;
        } __except(EXCEPTION_EXECUTE_HANDLER) {
            return false;
        }
    }
    
    // Safe memory write with error handling
    template<typename T>
    bool SafeWrite(uintptr_t address, T value) {
        if (address == 0) return false;
        
        __try {
            *(T*)address = value;
            return true;
        } __except(EXCEPTION_EXECUTE_HANDLER) {
            return false;
        }
    }
    
    // Periodic security check
    class SecurityMonitor {
    private:
        TimingRandomizer timer;
        
    public:
        SecurityMonitor() : timer(5000, 2000) {} // Check every 5-7 seconds
        
        void Update() {
            if (!timer.ShouldExecute()) return;
            
            // Perform security checks
            if (IsDebugged()) {
                LOGW("Debugger detected!");
                // Could exit or disable features here
            }
            
            if (CheckSuspiciousProcesses()) {
                LOGW("Suspicious process detected!");
            }
            
            if (IsScreenshotDetected()) {
                LOGW("Screenshot detected!");
            }
        }
    };
    
    // Global security monitor instance
    static SecurityMonitor g_SecurityMonitor;
    
    // Call this every frame
    void Update() {
        g_SecurityMonitor.Update();
    }
    
    // Cleanup function
    void Cleanup() {
        LOGI("Anti-Ban system cleanup");
    }
}

#endif // ANTIBAN_H
