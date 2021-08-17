#include <platform_opts.h>
#include <platform_stdlib.h>

#if defined(CONFIG_EXAMPLE_MATTER) && CONFIG_EXAMPLE_MATTER

extern void ChipTest(void);

static void example_matter_task_thread(void *pvParameters)
{
	ChipTest();

    vTaskDelete(NULL);
    return;
}

void example_matter_task(void)
{
    if(xTaskCreate(example_matter_task_thread, ((const char*)"example_matter_task_thread"), 8192, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS)
        printf("\n\r%s xTaskCreate(example_matter_task_thread) failed", __FUNCTION__);
}

#endif // #if (CONFIG_EXAMPLE_MATTER)
