# DragonWatch (ESP32 + TFT)

DragonWatch muestra Pull Requests abiertos de GitHub y commits en una TFT 320x240 con una escena animada 2D de pombero esquivando ataques.

Versión actual: `MVP 0.1`

## Stack
- PlatformIO
- ESP32 Arduino framework
- TFT_eSPI
- ArduinoJson
- Unity tests (native + embedded)

## Estructura
- `include/` configuración de board/app y secrets ejemplo
- `src/domain/` lógica pura testeable
- `src/app/` orquestación
- `src/infrastructure/` WiFi + GitHub
- `src/ui/` render y animación
- `src/platform/` adaptadores de hardware
- `test/test_native_logic/` unit tests lógicos
- `test/test_embedded_smoke/` smoke tests de entorno
- `docs/specs/` spec y tasks
- `docs/decisions/` ADRs

## Configuración
1. Copiar `include/secrets.example.h` a `include/secrets.h`.
2. Completar WiFi, token GitHub y repos (`owner/name`).
3. Ajustar pines/board en `include/board_config.h` si tu lote CYD difiere.

## Comandos
- `pio run`
- `pio test -e native`
- `pio test -e embedded`
- `pio run -t upload`
- `pio device monitor`

## Notas de hardware
- Perfil inicial: `esp32dev` con pinout conservador de ESP32-2432S028/CYD.
- Touch no implementado en MVP.
- Configuración de pantalla centralizada en `board_config.h` y `platformio.ini`.
- Parámetros release recomendados: refresh cada 15s, rotación cada 6s, render a 20 FPS.

## Seguridad
- `include/secrets.h` está ignorado por git.
- No commits de credenciales reales.
