# ADR 002 - TLS con `setInsecure()` en MVP

## Estado
Aceptada (temporal)

## Contexto
Para HTTPS de GitHub en ESP32 se requiere manejo de certificados. El MVP prioriza entrega funcional y simplicidad de operación.

## Decisión
Usar `WiFiClientSecure::setInsecure()` en MVP.

## Consecuencias
- Simplifica conectividad inicial.
- Reduce seguridad de validación TLS.
- Debe reemplazarse en siguiente iteración por pinning/cert raíz.
