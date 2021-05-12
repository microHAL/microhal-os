#include "FreeRTOS.h"
#include "diagnostic/diagnostic.h"
#include "microhalConfig.h"
#include "task.h"

using namespace microhal::diagnostic;

extern "C" __attribute__((weak)) void vApplicationStackOverflowHook(xTaskHandle pxTask, signed char* pcTaskName) {
#if defined(MICROHAL_FREERTOS_ENABLE_STACK_OVERFLOW_BREAKPOINT) || defined(MICROHAL_FREERTOS_ENABLE_DEBUG_BREAKPOINTS) || \
    defined(MICROHAL_DEBUG_BREAKPOINTS)
    __asm volatile("BKPT #01");
#endif

    [[maybe_unused]] volatile xTaskHandle task = pxTask;
    diagChannel << MICROHAL_EMERGENCY << "Stack overflow detected.";
    if (pcTaskName) {
        diagChannel << Emergency << " Task name: " << pcTaskName;
    }

    /* Run time stack overflow checking is performed if
     configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
     function is called if a stack overflow is detected. */
    // taskDISABLE_INTERRUPTS();
    for (;;)
        ;
}

extern "C" __attribute__((weak)) void vApplicationMallocFailedHook(void) {
#if defined(MICROHAL_FREERTOS_ENABLE_MALLOCK_FAILED_BREAKPOINT) || defined(MICROHAL_FREERTOS_ENABLE_DEBUG_BREAKPOINTS) || \
    defined(MICROHAL_DEBUG_BREAKPOINTS)
    __asm volatile("BKPT #01");
#endif
    diagChannel << MICROHAL_EMERGENCY << "FreeRTOS malloc failed.";
}

extern "C" __attribute__((weak)) void vApplicationIdleHook(void) {
    while (1) {
    }
}

extern "C" void vApplicationTickHook(void) {
    static volatile uint32_t counter = 0;
    counter++;
}

extern "C" void FreeRTOS_ConfigAssert(const char* fileName, uint32_t lineNo) {
#if defined(MICROHAL_FREERTOS_ENABLE_CONFIG_ASSERT_BREAKPOINT) || defined(MICROHAL_FREERTOS_ENABLE_DEBUG_BREAKPOINTS) || \
    defined(MICROHAL_DEBUG_BREAKPOINTS)
    __asm volatile("BKPT #01");
#endif
    using namespace microhal;
    diagChannel << Emergency << "FreeRTOS configAssert failed at: " << fileName << ":" << lineNo << endl;
}
