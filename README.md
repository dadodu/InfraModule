#InfraModule#
-----------

Open Source project on stm32l151 - USB, Flash, RTC and OLED management.
If you are interested : [click here!](http://la-bluefrog.myinstapage.com/)


Common requirements
-------------------

- To compile :
    arm-none-eabi-gcc
    arm-none-eabi-ld
    arm-none-eabi-objcopy
    arm-none-eabi-objdump
    arm-none-eabi-size
    arm-none-eabi-gdb (optional)

- To run :
    st-flash from [stlink](https://github.com/texane/stlink)

Common libraries used
---------------------

- STM32L1xx_StdPeriph_Driver_v1.2.0
- STM32_USB-FS-Device_Driver_v4.0.0



Connection to the board
-----------------------

-> UART_RX
-> UART_TX
-> GND
-> SWD_CLK
-> VCC
-> SWD_IO
-> RST
