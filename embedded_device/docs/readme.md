# DOCUMENTATION

- [FreeRTOS](#freertos)
    - [Task priority](#task-priority)
    - [Hardware interrupt priority](#hardware-interrupt-priority)
    - [Kernel interrupt priority](#kernel-interrupt-priority)
    - [Task priority vs hardware priority](#task-priority-vs-hardware-priority)



## FREERTOS


### TASK PRIORITY

The higher priority value, the higher urgency of a task, i.e. task with
priority value 2 has higher urgency than a task with priority value 1.

The idle task has priority value 0.


### HARDWARE INTERRUPT PRIORITY

The lower priority value, the higher urgency of a hardware interrupt, i.e.
hardware interrupt with priority value 1 has higher urgency than a hardware
interrupt with priority value 2.

The default hardware interrupt priority is 0 (the highest possible).


### KERNEL INTERRUPT PRIORITY

`configKERNEL_INTERRUPT_PRIORITY` decides the priority level for the kernel
related interrupts like `SysTick`, `PendSV` and `SVC`. It is set to the lowest
possible hardware interrupt priority.

For example, in ARM Cortex-M4 NVIC priority bits are set to 4 (4 higher bits in
a byte). It means that priorities range from 0x00 (the highest priority) to
0xF0 (the lowest priority).


### TASK PRIORITY VS HARDWARE PRIORITY

FreeRTOS APIs that end with `FromISR` are interrupt safe, and should be used
from interrupt service routines, which have priority value above the priority
defined by `configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY`.

Therefore, any interrupt service routine that uses FreeRTOS API function must
have its priority value set to a value that is equal to or greater than
`configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY` setting.

For example, when `configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY` equals 0x50,
then priority of a hardware interrupt which uses `FromISR` FreeRTOS APIs in its
interrupt service routine should be at least 0x50 or higher (lesser in terms of
urgency).

ARM Cortex-M interrupts have a priority value set to 0 by default. 0 is the
highest possible priority (urgency) value.
Therefore, **never leave the priority of an interrupt that uses FreeRTOS API at
its default value**.

