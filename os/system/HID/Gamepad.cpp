#include "MatrixOS.h"

typedef struct {
	// 32 Buttons, 6 Axis, 2 D-Pads
    int8_t	xAxis;
    int8_t	yAxis;
    int8_t	zAxis;

    int8_t	rzAxis;
    int8_t	rxAxis;
    int8_t	ryAxis;

    uint8_t	dPad;

    uint32_t buttons;
} HID_GamepadReport_Data_t;

namespace MatrixOS::HID::Gamepad
{   
    HID_GamepadReport_Data_t _report;

    void Send(void){ 
        // tud_hid_n_report(0, REPORT_ID_GAMEPAD, &_report, sizeof(_report));
        // MLOGD("Gamepad", "%d %d %d %d %d %d %d %d", _report.xAxis, _report.yAxis, _report.zAxis, _report.rzAxis, _report.rxAxis, _report.ryAxis, _report.dPad, _report.buttons);
        
        tud_hid_n_gamepad_report(0, REPORT_ID_GAMEPAD, _report.xAxis, _report.yAxis, _report.zAxis, _report.rzAxis, _report.rxAxis, _report.ryAxis, _report.dPad, _report.buttons);
    }

    void Press(GamepadKeycode b)
    {
        _report.buttons |= (uint32_t)1 << b; 
        Send();
    }

    void Release(GamepadKeycode b)
    {
        _report.buttons &= ~((uint32_t)1 << b); 
        Send();
    }

    void ReleaseAll(void)
    {
        _report.buttons = 0;
        Send();
    }

    void Buttons(uint32_t b)
    {
        _report.buttons = b; 
        Send();
    }

    void XAxis(int8_t a){ 
        _report.xAxis = a; 
        Send();
    }


    void YAxis(int8_t a){ 
        _report.yAxis = a; 
        Send();
    }


    void ZAxis(int8_t a){ 
        _report.zAxis = a; 
        Send();
    }


    void RXAxis(int8_t a){ 
        _report.rxAxis = a; 
        Send();
    }


    void RYAxis(int8_t a){ 
        _report.ryAxis = a; 
        Send();
    }


    void RZAxis(int8_t a){ 
        _report.rzAxis = a; 
        Send();
    }


    void DPad(GamepadDPadDirection d){ 
        _report.dPad = d; 
        Send();
    }
}