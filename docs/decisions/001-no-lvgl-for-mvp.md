# ADR 001 - No usar LVGL en MVP

## Estado
Aceptada

## Contexto
El hardware objetivo (ESP32 + TFT 320x240) tiene recursos limitados y el MVP requiere una UI simple con animaciones ligeras.

## Decisión
No usar LVGL en el MVP. Se implementa render directo con `TFT_eSPI` detrás de un `DisplayAdapter`.

## Consecuencias
- Menor consumo de RAM/flash inicial.
- Menor complejidad de integración.
- El renderer queda simple pero suficiente para estados y animación base.
