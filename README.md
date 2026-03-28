# DragonWatch (ESP32 + TFT)

DragonWatch muestra Pull Requests abiertos de GitHub y commits en una TFT 320x240 con una escena 2D animada (pombero + cuchillos/PRs).

Versión actual: `MVP 0.1`

## Tecnologías
- PlatformIO
- ESP32 Arduino framework (`espressif32`)
- C++17
- TFT_eSPI
- ArduinoJson
- Unity (`native` + `embedded`)

## Hardware objetivo para pruebas
- ESP32-2432S028 / CYD (320x240, ILI9341)
- Conexión USB serial al host
- WiFi 2.4GHz

Pinout base actual (centralizado):
- `TFT_MISO=12`
- `TFT_MOSI=13`
- `TFT_SCLK=14`
- `TFT_CS=15`
- `TFT_DC=2`
- `TFT_RST=-1`
- `TFT_BL=21`

Si tu lote CYD usa otros pines, ajustar:
- `include/board_config.h`
- `platformio.ini` (`build_flags` de TFT_eSPI)

## Estructura del proyecto
- `include/` config de app/board y secretos
- `src/domain/` lógica pura testeable
- `src/app/` orquestación y scheduler
- `src/infrastructure/` WiFi + GitHub API
- `src/ui/` renderer, animación y layout
- `src/platform/` adaptadores de hardware
- `test/test_native_logic/` unit tests lógicos
- `test/test_embedded_smoke/` smoke tests en embedded
- `docs/specs/` especificación y plan de tareas
- `docs/decisions/` ADRs
- `docs/releases/` notas de release

## Paso a paso detallado
1. Clonar el repositorio.
2. Entrar a la carpeta del proyecto:
```bash
cd dragonwatch
```
3. Instalar PlatformIO si no está disponible:
```bash
pip install platformio
```
4. Crear archivo de secretos:
```bash
cp include/secrets.example.h include/secrets.h
```
5. Editar `include/secrets.h` con tus datos:
- `DW_WIFI_SSID`
- `DW_WIFI_PASSWORD`
- `DW_GITHUB_TOKEN`
- `DW_REPO_1..3` (formato `owner/name`)
6. Compilar firmware:
```bash
pio run -e esp32dev
```
7. Ejecutar tests unitarios nativos:
```bash
pio test -e native
```
8. Ejecutar smoke tests embedded (opcional):
```bash
pio test -e embedded
```
9. Conectar la placa por USB y detectar puerto:
```bash
pio device list
```
10. Flashear firmware:
```bash
pio run -e esp32dev -t upload --upload-port <PUERTO>
```
11. Abrir monitor serial:
```bash
pio device monitor -b 115200 -p <PUERTO>
```

## Comandos de uso diario
- Build: `pio run -e esp32dev`
- Test lógica: `pio test -e native`
- Test embedded: `pio test -e embedded`
- Upload: `pio run -e esp32dev -t upload --upload-port <PUERTO>`
- Monitor: `pio device monitor -b 115200 -p <PUERTO>`

## Perfil MVP 0.1
- `refreshIntervalMs = 15000`
- `rotateIntervalMs = 6000`
- `renderIntervalMs = 50` (20 FPS)

## Alcance MVP
- Conexión WiFi y reconexión básica
- Consulta de PRs abiertos a GitHub
- Rotación automática de PR activo
- Estados visuales de carga/error/sin PRs/rate limit
- Animación desacoplada de la fuente de datos

## Limitaciones conocidas
- Fetch HTTP síncrono en loop principal (puede existir micro-stutter durante refresh).
- TLS con `setInsecure()` en esta iteración.
- Touch no implementado en MVP.

## Troubleshooting rápido
- Pantalla en negro:
  - Validar pinout TFT para tu lote.
  - Revisar alimentación USB.
- No aparecen PRs:
  - Verificar token y repos.
  - Confirmar que el repo tenga PRs abiertos.
- Errores de upload:
  - Confirmar puerto correcto en `pio device list`.
  - Probar mantener `BOOT` al iniciar upload en algunos boards.

## Seguridad
- `include/secrets.h` no se versiona.
- Nunca commitear tokens ni claves reales.
- Compartir solo `include/secrets.example.h` como plantilla.
