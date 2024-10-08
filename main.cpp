#include "MatrixOS.h"

int main()
{
    MatrixOS::SYS::Begin();

    #ifndef ESP_IDF_VERSION //ESP32s Doesn't need to start scheduler
    vTaskStartScheduler();
    #else
    vTaskDelete(NULL);
    #endif

    return 0;             
}