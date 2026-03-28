# SPEC 001 - DragonWatch MVP

## Problema
Los equipos que revisan Pull Requests no tienen una forma visible y pasiva de monitorear actividad en repositorios mientras trabajan en hardware embebido. DragonWatch busca mostrar PRs activos y commits en una pantalla TFT integrada en ESP32 con una visualización animada de bajo costo computacional.

## Objetivo del MVP
Entregar firmware para ESP32 (perfil CYD/ESP32-2432S028) que:
- se conecte a WiFi,
- consulte PRs abiertos de GitHub para repos configurables,
- seleccione y rote PRs para visualización,
- muestre estado de sincronización y errores,
- renderice escena 2D con dragón y commits flotantes,
- mantenga lógica de negocio testeable sin hardware.

## Alcance
### Dentro de alcance
- Configuración por archivos locales (`app_config.h`, `secrets.h` no versionado).
- Cliente GitHub encapsulado.
- Parseo mínimo de JSON para PRs y commits.
- Estados de app: `LOADING`, `READY`, `NO_PRS`, `RECONNECTING`, `ERROR_NETWORK`, `ERROR_API`, `RATE_LIMIT`.
- Animación simple por estado del dragón (`idle`, `working`, `error`).
- Rotación automática de PRs/repositorios con tolerancia a fallos.
- Unit tests de lógica pura en entorno `native`.

### Fuera de alcance (MVP)
- Interacción touch.
- Persistencia histórica de PRs.
- Renderizado avanzado por sprites externos o LVGL.
- Web portal de configuración.
- OTA.

## Restricciones de hardware
- MCU: ESP32 (memoria y CPU limitadas).
- Pantalla 320x240 landscape.
- Ciclo principal no bloqueante (sin `delay` largo).
- Evitar parseos JSON completos cuando no sea necesario.
- Evitar redraw total con flicker.

## Supuestos
1. Se usa panel ILI9341 típico de ESP32-2432S028/CYD.
2. Pinout exacto puede variar por lote; se centraliza en `board_config.h` para ajuste único.
3. GitHub API v3 disponible por HTTPS con token personal clásico/fine-grained con alcance de lectura de PR.
4. El firmware no valida certificado raíz custom en esta iteración (tradeoff operativo vs. simplicidad de MVP).

## Riesgos y mitigaciones
- **Pinout no coincide con board real**: encapsular board support y setup TFT en un solo lugar.
- **Rate limit GitHub**: detectar headers/HTTP 403 y estado `RATE_LIMIT`; usar backoff.
- **Fragmentación por strings/JSON**: parseo selectivo y modelos compactos.
- **Flicker**: estrategia de redraw parcial y frame pacing.
- **Intermitencia WiFi**: estado explícito + reconexión con backoff.

## Arquitectura propuesta
Capas:
- **Domain (puro)**: modelos, reglas de selección/rotación, animación matemática, layout lógico, estado app.
- **Application**: orquestación de ciclos (refresh/rotate/render ticks), coordinación de servicios abstractos.
- **Infrastructure**: WiFi real, cliente HTTP GitHub, parser JSON adaptado.
- **UI**: renderer de pantalla, widgets, animador visual del dragón.
- **Platform**: adaptación TFT_eSPI, soporte board y logger.

Dependencias permitidas: capa superior depende de inferior por interfaces, no por implementación concreta.

## Flujo de datos
1. Scheduler dispara `refresh` por intervalo.
2. AppController consulta estado WiFi.
3. Si WiFi ok, GitHub client obtiene PRs por repo y commits del PR activo.
4. Parser transforma JSON -> modelos internos.
5. Selector + rotador eligen PR mostrado.
6. UI recibe ViewModel inmutable y dibuja dif incremental.
7. Animator actualiza posiciones flotantes en tick periódico.

## Modelo de estados de la app
- `LOADING`: inicio o refresh en curso sin datos previos.
- `READY`: PR activo disponible.
- `NO_PRS`: sin PRs abiertos en repos configurados.
- `RECONNECTING`: WiFi desconectado intentando recuperar.
- `ERROR_NETWORK`: fallo transporte HTTP/WiFi sin llegar API.
- `ERROR_API`: respuesta API inválida/no esperada.
- `RATE_LIMIT`: GitHub limita consultas.

Transiciones se prueban en unidad (sin hardware).

## Estrategia de renderizado
- Fondo y elementos estáticos una vez por cambio de estado mayor.
- Redraw parcial de áreas dinámicas:
  - dragón (bounding box),
  - commits flotantes,
  - barra de estado.
- Animación por `millis()` y delta time.

## Estrategia de recuperación ante fallos
- Backoff exponencial acotado para refresh y reconexión.
- Mantener último dato válido mostrado cuando falla refresh.
- Cambiar visual a estado de error sin limpiar toda la UI.
- Reiniciar ciclo normal al primer refresh exitoso.

## Estrategia de pruebas
- **Native unit tests** para:
  - parseo JSON->modelo,
  - normalización de SHAs,
  - rotador/selector,
  - transiciones de estado,
  - scheduler y backoff,
  - posiciones flotantes,
  - layout lógico.
- **Embedded tests (base)** solo smoke de integración donde aplique.
- No se testea en unit: WiFi real, HTTP real, TFT real.

## Definición de terminado (DoD)
- Spec y plan de tareas presentes.
- Compila `pio run` en entorno embebido.
- Ejecuta `pio test -e native` con tests relevantes.
- Arquitectura desacoplada de hardware.
- UI funcional con dragón + commits flotantes.
- README de setup desde cero.
- Sin secretos en git.

## Backlog de siguientes iteraciones
1. Soporte de touch para cambio manual de PR.
2. Caché local de último snapshot con timestamp.
3. Doble buffer/sprite para reducción extra de flicker.
4. Telemetría serial estructurada.
5. OTA y perfilado de memoria.
