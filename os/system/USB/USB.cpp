#include "tusb.h"
#include "MatrixOS.h"
#include "MIDI.h"

namespace MatrixOS::USB
{
  void usb_device_task(void* param) {
    (void)param;
    // RTOS forever loop
    while (1)
    {
      // tinyusb device task
      tud_task();
    }
  }

// Create a task for tinyusb device stack
#define USBD_STACK_SIZE (3 * configMINIMAL_STACK_SIZE)
  StackType_t usb_device_stack[USBD_STACK_SIZE];
  StaticTask_t usb_device_taskdef;
  void Init() {
    tusb_init();
    (void)xTaskCreateStatic(usb_device_task, "usbd", USBD_STACK_SIZE, NULL, configMAX_PRIORITIES - 1, usb_device_stack,
                            &usb_device_taskdef);
    USB::MIDI::Init();
  }

  bool Inited() {
    return tusb_inited();
  }

  bool Connected() {
    return tud_ready();
  }
}