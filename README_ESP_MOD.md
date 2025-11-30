# Free Fire ESP Mod - v1.118.1

Módulo Magisk con ESP holográfico para Free Fire versión 1.118.1

## 🎯 Características

- ✅ ESP holográfico (color cian brillante)
- ✅ Visibilidad a través de paredes
- ✅ Cajas alrededor de jugadores
- ✅ Líneas desde tu posición
- ✅ Distancia a jugadores
- ✅ Menú ImGui para configurar

## 📋 Requisitos

- Android con root
- Magisk v24 o superior
- Zygisk activado en Magisk
- Free Fire v1.118.1

## 🔧 Compilación (GitHub Actions - SIN PC)

### Opción 1: Usar GitHub Actions (RECOMENDADO)

1. **Crear cuenta en GitHub** (si no tienes): https://github.com/signup

2. **Hacer Fork de este repositorio**:
   - Click en "Fork" arriba a la derecha
   - Espera a que se copie el repositorio a tu cuenta

3. **Ir a Actions**:
   - En TU repositorio (el fork), ve a la pestaña "Actions"
   - Click en "I understand my workflows, go ahead and enable them"

4. **Ejecutar Build**:
   - En el menú izquierdo, click en "Build"
   - Click en "Run workflow" (botón azul a la derecha)
   - En "Game package name" escribe: `com.dts.freefireth`
   - Click en "Run workflow" (verde)

5. **Descargar el módulo**:
   - Espera 5-10 minutos a que compile
   - Cuando termine, verás un ✅ verde
   - Click en el workflow completado
   - Baja hasta "Artifacts"
   - Descarga el ZIP del módulo

6. **Instalar**:
   - Copia el ZIP a tu teléfono
   - Abre Magisk → Módulos
   - Instalar desde almacenamiento
   - Selecciona el ZIP
   - Reinicia

### Opción 2: Compilar Localmente (CON PC)

Si tienes PC con Android Studio:

```bash
git clone https://github.com/TU_USUARIO/Zygisk-Il2CppDumper.git
cd Zygisk-Il2CppDumper
./gradlew :module:assembleRelease
```

El módulo se generará en `out/`

## 📱 Uso

1. **Instala el módulo** en Magisk
2. **Reinicia** el teléfono
3. **Abre Free Fire**
4. El ESP se activará automáticamente
5. Verás jugadores con holograma cian a través de paredes

### Configuración del Menú

- El menú ImGui aparece automáticamente
- Puedes activar/desactivar:
  - Cajas ESP
  - Líneas
  - Distancia
  - Nombres
- Cambiar colores y grosor de líneas

## ⚠️ Advertencias

- **RIESGO DE BANEO**: Usar mods viola los términos de servicio
- **USA CUENTA SECUNDARIA**: NUNCA uses tu cuenta principal
- **ACTUALIZACIONES**: Cada update de Free Fire rompe el mod
- **DETECCIÓN**: El anti-cheat puede detectar el mod

## 🔍 Offsets Incluidos

Este mod incluye los offsets extraídos del dump.cs de v1.118.1:

```cpp
Camera_WorldToScreenPoint = 0x82dc3ec
Camera_get_main = 0x82dca04
Component_get_gameObject = 0x82ded7c
GameObject_get_name = 0x826d9a4
GameObject_get_tag = 0x82df060
GameObject_get_transform = 0x82ded10
Transform_get_position = 0x7ede9cc
```

## 📝 Créditos

- Basado en [Zygisk-Il2CppDumper](https://github.com/Perfare/Zygisk-Il2CppDumper)
- Offsets extraídos con Il2CppDumper
- Creado para Free Fire v1.118.1

## 🛠️ Solución de Problemas

### El módulo no aparece en Magisk
- Verifica que Zygisk esté activado
- Reinstala el módulo
- Revisa los logs de Magisk

### El ESP no funciona
- Verifica que sea Free Fire v1.118.1
- Reinicia el teléfono después de instalar
- Revisa que el módulo esté activado en Magisk

### El juego crashea
- Desactiva el módulo
- Verifica la versión del juego
- Puede que haya actualización y los offsets cambien

## 📞 Soporte

Si tienes problemas:
1. Verifica que sea la versión correcta (1.118.1)
2. Revisa los logs de Magisk
3. Asegúrate de que Zygisk esté activado

---

**DISCLAIMER**: Este mod es solo para fines educativos. El uso de mods en juegos online puede resultar en baneo permanente. Úsalo bajo tu propio riesgo.
