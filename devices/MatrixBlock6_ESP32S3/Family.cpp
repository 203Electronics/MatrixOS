#include "Device.h"

namespace MatrixOS::SYS
{
    void ExecuteAPP(string author, string app_name);
}

namespace MatrixOS::UIInterface
{
void TextScroll(string ascii, Color color, uint16_t speed = 10, bool loop = false);
}

namespace Device
{
    void DeviceInit()
    {
        // esp_timer_early_init();
        LoadDeviceInfo();
        USB::Init();
        LED::Init();
        KeyPad::Init();
        NVS::Init();

        BLEMIDI::Init(name);
        if(bluetooth)
        {
            BLEMIDI::Start();
        }

        // WIFI::Init();
        // ESPNOW::Init();
        // ESPNOW::BroadcastMac();

        // MatrixOS::Logging::LogDebug("Matrix", "Device Inited - Device Code: %s  Reversion %s Production Time %02d-%02d", deviceInfo.DeviceCode, deviceInfo.Revision, deviceInfo.ProductionYear, deviceInfo.ProductionMonth);
    
    }

    void LoadDeviceInfo()
    {
        #ifndef FACTORY_CONFIG
        esp_efuse_read_field_blob(ESP_EFUSE_USER_DATA, &deviceInfo, sizeof(deviceInfo) * 8);
        #endif
        LoadVarientInfo();
    }

    void PostBootTask()
    {   
        KeyPad::Scan();
        #ifdef FACTORY_CONFIG
        if(esp_efuse_block_is_empty(EFUSE_BLK3))
        {
            MatrixOS::UIInterface::TextScroll("Factory Test", Color(0xFF00FF));
            MatrixOS::SYS::ExecuteAPP("203 Electronics", "Matrix Factory Menu");
        }
        #endif
        if(KeyPad::GetKey(KeyPad::XY2ID(Point(0, 0)))->velocity && KeyPad::GetKey(KeyPad::XY2ID(Point(1, 1)))->velocity)
        {
            MatrixOS::SYS::ExecuteAPP("203 Electronics", "Matrix Factory Menu");
        }
    }

    // bool wdt_subscribed = false;
    void DeviceTask()
    {   
        // if(wdt_subscribed == false)
        // {
        //     esp_task_wdt_add(NULL);
        //     esp_task_wdt_status(NULL);
        // }

        // ESP_LOGI("Device", "Task Watchdog Reset1");
        // esp_task_wdt_reset();
        // ESP_LOGI("Device", "Task Watchdog Reset2");
    }

    void Bootloader()
    {
        // Check out esp_reset_reason_t for other Espressif pre-defined values
        #define APP_REQUEST_UF2_RESET_HINT (esp_reset_reason_t)0x11F2

        // call esp_reset_reason() is required for idf.py to properly links esp_reset_reason_set_hint()
        (void) esp_reset_reason();
        esp_reset_reason_set_hint(APP_REQUEST_UF2_RESET_HINT);
        esp_restart();
    }

    void Reboot()
    {
        esp_restart();
    }

    void Delay(uint32_t interval)
    {
        vTaskDelay(pdMS_TO_TICKS(interval));
    }

    uint32_t Millis()
    {
        return ((((uint64_t) xTaskGetTickCount()) * 1000) / configTICK_RATE_HZ );
        // return 0;
    }

    void Log(string format, va_list valst)
    {
        // ESP_LOG_LEVEL((esp_log_level_t)level, tag.c_str(), format.c_str(), valst);
        // esp_log_writev(ESP_LOG_INFO, format.c_str(), valst);
        
        vprintf(format.c_str(), valst);
    }

    string GetSerial()
    {
        uint8_t uuid[16];
        esp_efuse_read_field_blob(ESP_EFUSE_OPTIONAL_UNIQUE_ID, (void*)uuid, 128);
        string uuid_str;
        uuid_str.reserve(33);
        const char char_table[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
        for(uint8_t i = 0; i < 16; i++)
        {
            uuid_str += char_table[uuid[i] >> 4];
            uuid_str += char_table[uuid[i] & 0x0F];
        }
        return uuid_str; //TODO
    }

    
    void ErrorHandler()
    {
        
    }

    namespace MIDI
    {
        uint32_t Available()
        {   
            uint32_t packets = 0;
            if(BLEMIDI::started)
            {
                packets += BLEMIDI::MidiAvailable();
            }
            if(ESPNOW::started)
            {
                packets += ESPNOW::MidiAvailable();
            }
            packets += ESPNOW::MidiAvailable();
            return packets;
        }

        MidiPacket Get()
        {
            if(BLEMIDI::started && BLEMIDI::MidiAvailable())
            {
                return BLEMIDI::GetMidi();
            }
            if(ESPNOW::started && ESPNOW::MidiAvailable())
            {
                return ESPNOW::GetMidi();
            }
            // ESP_LOGI("Midi Get", "WTF?");
            return MidiPacket(0, None);
        }

        bool Send(MidiPacket packet)
        {
            if(BLEMIDI::started)
            {
                BLEMIDI::SendMidi(packet.data);
            }
            if(ESPNOW::started)
            {
                ESPNOW::SendMidi(packet.data);
            }
            return true; //idk what bool should mean in a multi port situation. Leave it be for now
        }
    } 
}

namespace MatrixOS::SYS
{
    void ErrorHandler(char const* error);
}

extern "C"
{
    int main();
    void app_main(void)
    {
        main();
    }
}
