.syntax unified
.cpu cortex-m3
.thumb

.global reset_handler
.global _estack

_estack = 0x20005000

.section .vectors, "a", %progbits
.type vector_table, %object

vector_table:
    .word _estack
    .word reset_handler

.text
.type reset_handler, %function

reset_handler:
    bl main

loop:
    b loop
