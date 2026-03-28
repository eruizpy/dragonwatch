# TASKS 001 - DragonWatch MVP

Estado global: `IN_PROGRESS (validación de build pendiente por ausencia de PlatformIO CLI en entorno actual)`

## Tareas
1. **Bootstrap del proyecto PlatformIO**
- Dependencias: ninguna
- Aceptación: `platformio.ini` y estructura base creados
- Estado: `DONE`

2. **Especificación técnica MVP**
- Dependencias: 1
- Aceptación: spec completa en `docs/specs/001-dragonwatch-mvp.md`
- Estado: `DONE`

3. **Definir contratos y modelos de dominio**
- Dependencias: 2
- Aceptación: headers/modelos puros para PR, commit, estado app, scheduler
- Estado: `DONE`

4. **Tests unitarios (TDD) de lógica pura**
- Dependencias: 3
- Aceptación: suites nativas para parser, selección, rotación, SHA, estado, animación, layout, backoff
- Estado: `DONE (creados; ejecución local bloqueada por falta de CLI)`

5. **Implementar servicios de dominio para pasar tests**
- Dependencias: 4
- Aceptación: tests nativos en verde
- Estado: `DONE (pendiente confirmar en CI/local con PlatformIO instalado)`

6. **Implementar capa app (scheduler + controlador)**
- Dependencias: 5
- Aceptación: loop no bloqueante con ticks de refresh/rotate/render
- Estado: `DONE`

7. **Implementar infraestructura (WiFi, GitHub API, parser)**
- Dependencias: 6
- Aceptación: cliente encapsulado + parseo mínimo + manejo de errores/rate limit
- Estado: `DONE`

8. **Implementar UI + animación desacoplada**
- Dependencias: 6
- Aceptación: render de datos + dragón + commits flotantes + estados visuales
- Estado: `DONE`

9. **Integración en `main.cpp` y soporte board**
- Dependencias: 7,8
- Aceptación: build embebido exitoso
- Estado: `IN_PROGRESS (integrado, falta validar compilación con `pio run`)`

10. **Documentación final + ADRs claves**
- Dependencias: 9
- Aceptación: README setup y ADR de decisiones críticas
- Estado: `IN_PROGRESS`
