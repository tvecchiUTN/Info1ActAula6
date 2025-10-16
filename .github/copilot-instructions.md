# Instrucciones rápidas para agentes de IA (proyecto Info1ActAula6)

Objetivo breve
- Proyecto en C que implementa tres hilos: entrada de usuario (`inputThread`), cálculo de histograma (`histograma`) y escritura a archivo (`fileThread`). La coordinación se hace por un buffer circular (`param_t` en `fThreads.h`) y un mutex global `myMutex`.

Qué debes saber para ser productivo
- Archivos clave:
  - `main.c`: inicializa `param_t`, crea hilos y gestiona mutex.
  - `fThreads.h`: definición de `param_t`, macros importantes (por ejemplo `CAPACIDAD`, `N`, `FINALIZADOR`) y prototipos.
  - `inputThread.c`, `histoThread.c`, `fileThread.c`: lógica de productor/consumidor y manejo del buffer circular.

- Patrón de concurrencia: un único mutex global (`myMutex`) protege el acceso al buffer y a los flags. Los tres hilos usan este mutex alrededor de operaciones de lectura/escritura del buffer.

- Buffer y flags:
  - `param_t->vecStr` es un array circular de `char*` con capacidad `CAPACIDAD` (32).
  - `contProductor`, `contArc`, `contHisto` son índices para productor (entrada), consumidor-archivo y consumidor-histograma.
  - `flagFull` indica si el buffer está lleno; `flagArc` y `flagHisto` marcan que un consumidor procesó un elemento y permiten liberar memoria.
  - `FINALIZADOR` ("InfoI-2022") es la cadena especial que hace terminar al programa.

Convenciones de código y estilo
- Memoria: los strings se asignan con `malloc(N)` en `inputThread` y se pasan al buffer sin copia; los consumidores (histograma/archivo) llaman a `free` según los flags compartidos. Ten cuidado con la propiedad de la memoria: un string es liberado sólo cuando ambos consumidores han procesado y marcado flags apropiados.
- Mutex: `myMutex` es declarado `extern` en `fThreads.h` y definido en múltiples archivos actualmente — esto puede generar redefiniciones al linkear. Si modificas/añades código, define `myMutex` en un único C (p. ej. `main.c`) y usar `extern` en encabezados.

Comandos de build y ejecución (probados localmente)
- Compilación simple (desde la raíz del repo):
  - gcc -pthread main.c inputThread.c histoThread.c fileThread.c -o info1act
  - Ejecuta: `./info1act` y escribe líneas en stdin. Envía `InfoI-2022` para terminar.

Patrones y ejemplos detectables en el código
- Productor: `inputThread.c` usa `fgets(buffStr, N, stdin)` y luego `addPila(...)` para insertar el puntero en `vecStr`.
- Consumidores:
  - `histoThread.c`: extrae con `extPilaHisto`, llama `calcHisto` y usa `vecHisto[26]` para contar letras (A-Z y a-z tratados igual). Si la cadena es `FINALIZADOR` cambia `flagEnd` a 0 para terminar los hilos.
  - `fileThread.c`: extrae con `extPilaFile` y escribe `N` bytes en `histograma.txt` usando `write()`.

Riesgos y puntos a revisar por un agente
- Definición múltiple de `myMutex`: se declara en más de un .c — al compilar puede provocar errores de linkeo. Un cambio seguro: mantener `extern pthread_mutex_t myMutex;` en `fThreads.h` y sólo una definición en `main.c`.
- Ownership de `char*`: el productor asigna y los consumidores liberan, pero la lógica de flags es frágil; modifica o agrega comentarios si refactorizas.
- Uso de constantes: `N` (64) y `CAPACIDAD` (32) aparecen en varios lugares; respeta estas macros al cambiar tamaños.

Pequeñas tareas útiles para contribuir rápidamente
- Consolidar la definición de `myMutex` en `main.c` y dejar `extern` en `fThreads.h`.
- Añadir comprobaciones nulas tras `fgets` y validación de `write()`/`open()` para robustez.
- Documentar claramente la propiedad de memoria de `vecStr` en `fThreads.h`.

Dónde mirar para más contexto
- Revisa las funciones `addPila`, `extPilaHisto`, `extPilaFile` para entender exactamente cuándo se marca `flagFull`, `flagArc` y `flagHisto`.

Si algo está poco claro
- Dime si prefieres que: (1) unifique la definición de `myMutex` y aplique un patch, o (2) solo documente la recomendación. También puedo añadir tests de compilación automáticos.
