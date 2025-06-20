# ALFOBI

```
 █████╗ ██╗     ███████╗ ██████╗ ██████╗ ██╗
██╔══██╗██║     ██╔════╝██╔═══██╗██╔══██╗██║
███████║██║     █████╗  ██║   ██║██████╔╝██║
██╔══██║██║     ██╔══╝  ██║   ██║██╔══██╗██║
██║  ██║███████╗██║     ╚██████╔╝██████╔╝██║
╚═╝  ╚═╝╚══════╝╚═╝      ╚═════╝ ╚═════╝ ╚═╝
```

~~Alfombra~~ Teclado de baile interactivo.

Proyecto de fin de curso de Sistemas Embebidos para Tiempo Real.

**Autores:**
- Fabricio Correa.
- Ricardo Llorente.
- Gennaro Monetti.

**Tutor:** Josefina Lema.

## Setup

### Compilación (Code Composer Studio)

La siguiente configuración fue testeada en Code Composer Studio v20.1.

Se **debe** configurar el dialecto de C99:

1. Ir a propiedades del proyecto.
2. En la ventana de propiedades ir a: Build > Tools > MSP430 Compiler > Advanced Options > Language Options.
3. Cambiar la opción `C Dialect` a `Compile program in C99 mode`.

Se **recomienda** activar las optimizaciones del compilador:

1. Ir a propiedades del proyecto.
2. En la ventana de propiedades ir a: Build > Tools > MSP430 Compiler > Optimization.
3. Cambiar `Optimization level` a `4 - Whole Program Optimizations`.
4. Cambiar `Speed vs. size trade-offs` a `5 (speed)`.

### Teclado

Los cables van de izquierda a derecha, en los siguientes puertos: P2.0, P2.3, P2.4, P2.5, P1.1, P1.2, P1.3, P1.4.

### Jumpers

Sacar los jumpers de P2.1, P2.3, P2.5 y P1.6. El jumper de P1.0 debe quedar puesto.

### Flash

El juego permite guardar la tabla de resultados en memoria FLASH a demanda (presionando la tecla `1` mientras no se está en la pantalla de juego). Para esto, es necesario modificar el archivo lnk_msp430g2553.cmd como se explica más abajo. Notar que se puede omitir este paso para ejecutar el juego, pero en caso de no hacerlo, presionar la tecla `1` generará comportamientos indefinidos, potencialmente borrando parte de la sección de código en la memoria FLASH.

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

## Troubleshooting

### Se prende el LED de P1.0 y se tranca el display.

Posible solución: desconectar el display de la fuente y volver a conectar. Luego presionar el botón de reset de la placa.

Explicación: cuando la ejecución se corta durante una transmisión I2C, queda la flag `UCBBUSY` encendida y el display queda en un estado inválido.
