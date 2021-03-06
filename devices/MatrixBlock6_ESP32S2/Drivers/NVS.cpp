#include "Device.h"
#include "framework/Hash.h"

namespace Device::NVS
{
    nvs_handle_t nvs_handle;
    void Init()
    {
        esp_err_t err = nvs_flash_init();
        if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
            // NVS partition was truncated and needs to be erased
            // Retry nvs_flash_init
            ESP_ERROR_CHECK(nvs_flash_erase());
            err = nvs_flash_init();
        }
        ESP_ERROR_CHECK( err );
        nvs_open("matrix_os", NVS_READWRITE, &nvs_handle);
    }

    vector<char> Read(string name)
    {
        size_t length;

        if (nvs_get_blob(nvs_handle, name.c_str(), NULL, &length) != ESP_OK)
        {
            return vector<char>(0);
        }
        vector<char> value(length);
        if (nvs_get_blob(nvs_handle, name.c_str(), value.data(), &length) != ESP_OK)
        {
            return vector<char>(0);
        }
        return value;
    }

    bool Write(string name, void* pointer, uint16_t length)
    {

        bool success = nvs_set_blob(nvs_handle, name.c_str(), pointer, length) == ESP_OK;
        nvs_commit(nvs_handle);
        return success;
    }

    bool Delete(string name)
    {
        bool success = nvs_erase_key(nvs_handle, name.c_str()) == ESP_OK;
        nvs_commit(nvs_handle);
        return success;
    }

    void Clear()
    {
        nvs_flash_erase_partition("matrix_os");
        nvs_commit(nvs_handle);
    }
}