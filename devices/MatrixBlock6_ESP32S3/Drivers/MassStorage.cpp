#include "Device.h"
#include "esp_vfs.h"
#include "esp_vfs_fat.h"
#include "esp_system.h"

namespace Device::MassStorage
{
    static const char *TAG = "MassStorage";

    namespace InternalFlash
    {
        wl_handle_t s_wl_handle = WL_INVALID_HANDLE;
        void Init()
        {
            const esp_vfs_fat_mount_config_t mount_config = {
                .format_if_mount_failed = true,
                .max_files = 8,
                .allocation_unit_size = CONFIG_WL_SECTOR_SIZE
            };

            esp_err_t err = esp_vfs_fat_spiflash_mount("/", "ffat", &mount_config, &s_wl_handle); //"ffat" is from https://github.com/203Electronics/Matrix-Bootloader/blob/master/ports/espressif/partitions-8MB.csv
            if (err != ESP_OK) {
                MatrixOS::Logging::LogWarning(TAG, "Failed to mount FATFS (%s)", esp_err_to_name(err));
                return;
            }

            MatrixOS::Logging::LogInfo(TAG, "Initialized Internal Flash, size=%d KB", wl_size(s_wl_handle) / 1024);
        }
    }

    namespace SDCard
    {

    }

    void Init()
    {
      InternalFlash::Init();
    }
}