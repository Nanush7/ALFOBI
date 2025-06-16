# ALFOBI

~~Alfombra~~ Teclado de baile interactivo.

Proyecto de fin de curso de Sistemas Embebidos para Tiempo Real.

**Autores:**
- Fabricio Correa.
- Ricardo Llorente.
- Gennaro Monetti.

**Tutor:** Josefina Lema.

## Setup

### Teclado

Los cables van de izquierda a derecha, en los siguientes puertos: P2.0, P2.3, P2.4, P2.5, P1.1, P1.2, P1.3, P1.4.

### Flash

Para poder persistir la tabla de resultados en memoria Flash, es necesario modificar el archivo lnk_msp430g2553.cmd de la siguiente manera:

1. En la sección donde se define el mapeo de la memoria, comentar o borrar la entrada (mapeo) FLASH.
2. Agregar una nueva entrada donde estaba FLASH, de nombre ALFOBI_SCORES, que empiece en la misma dirección y tenga largo de 512 bytes (0x0200, equivalente a un segmento). Además, debe rellenarse con fill = 0x0000.
3. Agregar otra entrada FLASH abajo de ALFOBI_SCORES, que empiece en el origen de ALFOBI_SCORES más 0x0200, de largo igual al que había en FLASH menos 0x0200.

Por ejemplo, si en el archivo .cmd tenía:
```c
MEMORY
{
    // ...
    FLASH                   : origin = 0xC000, length = 0x3FDE
    // ...
}
```
Debería quedar de la siguiente forma:

```c
MEMORY
{
    // ...
 // FLASH                   : origin = 0xC000, length = 0x3FDE
    ALFOBI_SCORES           : origin = 0xC000, length = 0x0200, fill = 0x0000
    FLASH                   : origin = 0xC200, length = 0x3DDE
    // ...
}
```
