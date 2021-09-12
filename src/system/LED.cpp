#include "MatrixOS.h"

namespace MatrixOS::LED
{
    uint8_t currentLayer = 0;
    Color* frameBuffer;
    void Init()
    {
        uint8_t size = sizeof(Color);
        // frameBuffer = (Color*)malloc(Device::numsOfLED * sizeof(Color) * LED_LAYERS );
        frameBuffer = (Color*)calloc(Device::numsOfLED * LED_LAYERS, sizeof(Color));
        for(uint32_t i = 0; i < Device::numsOfLED * LED_LAYERS; i++)
        {
            frameBuffer[i] = Color();
        }
        Update();
    }

    void SetColor(Point xy, Color color, uint8_t layer)
    {
        if(layer >= LED_LAYERS)
        {
            MatrixOS::SYS::ErrorHandler("LED Layer Unavailable");
            return;
        }
        uint16_t index = Device::LED::XY2Index(xy);
        if(index == UINT16_MAX) return;
        frameBuffer[index + Device::numsOfLED * layer] = color;
    }

    void SetColor(uint16_t ID, Color color, uint8_t layer)
    {
        if(layer >= LED_LAYERS)
        {
            MatrixOS::SYS::ErrorHandler("LED Layer Unavailable");
            return;
        }
        uint16_t index = Device::LED::ID2Index(ID);
        if(index == UINT16_MAX) return;
        uint16_t bufferIndex = index + Device::numsOfLED * layer;
        frameBuffer[bufferIndex] = color;

    }

    void Fill(Color color, uint8_t layer)
    {
        if(layer >= LED_LAYERS)
        {
            MatrixOS::SYS::ErrorHandler("LED Layer Unavailable");
            return;
        }
        for(uint16_t index = 0; index < Device::numsOfLED; index++)
        {
            frameBuffer[index + Device::numsOfLED * layer] = color;
        }
    }

    void Update(int8_t layer)
    {
        Device::LED::Update(&frameBuffer[Device::numsOfLED * layer], 64); //TODO: Get brightness
    }

    void SwitchLayer(uint8_t layer)
    {
        if(layer >= LED_LAYERS)
        {
            MatrixOS::SYS::ErrorHandler("LED Layer Unavailable");
            return;
        }
        currentLayer = layer;
    }
}