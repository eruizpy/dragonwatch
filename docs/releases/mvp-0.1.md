# DragonWatch MVP 0.1

## Estado
Release candidate funcional para CYD/ESP32-2432S028.

## Incluye
- Arquitectura desacoplada por capas (domain/app/infrastructure/ui/platform).
- Conexión WiFi y consumo de PRs abiertos de GitHub.
- Escena 2D con pombero y cuchillos representando PRs abiertos.
- Rotación automática de PR activo.
- Manejo visual de estados (`Loading`, `Ready`, `No PRs`, `Network/API/Rate limit`).
- Tests unitarios nativos de lógica pura.

## Perfil release
- `refreshIntervalMs = 15000`
- `rotateIntervalMs = 6000`
- `renderIntervalMs = 50`

## Limitaciones conocidas
- Fetch HTTP todavía síncrono en loop principal (micro-stutter residual posible durante refresh).
- TLS en modo `setInsecure()` (pendiente hardening en próxima iteración).
