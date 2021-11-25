#include "MatrixOS.h"
#include "printf.h"

#define DEFAULT_LOGGING_LEVEL VERBOSE //Change this later

namespace MatrixOS::Logging
{

    void LogLevelSet(string tag, ELogLevel level)
    {

    }

    ELogLevel GetLogLevel(string tag)
    {
        return DEFAULT_LOGGING_LEVEL;   
    }

    bool ShouldLog(string tag, ELogLevel target_level)
    {
        return (uint8_t)GetLogLevel(tag) >= (uint8_t)target_level;
    }

    string logLevel[5] = {"E", "W", "I", "D", "V"};
    string logLevelColor[5] = {"31", "33", "32", "36", "37"};
    void Log(ELogLevel level, string tag, string format, ...) //DO NOT USE THIS DIRECTLY. STRING WILL NOT BE REMOVED IF LOG LEVEL ISN'T SET FOR IT TO LOG
    {   
        if(ShouldLog(tag, level))
        {
            #ifdef MATRIXOS_LOG_COLOR
            string msg = string() + "\033[0;" + logLevelColor[level - 1] + "m" + logLevel[level - 1] + " (" + std::to_string(SYS::Millis()) + ") " +  tag + ": " + format + "\033[0m\n";
            #else
            string msg = logLevel[level - 1] + " (" + std::to_string(SYS::Millis()) + ") " +  tag + ": " + format + "\n";
            #endif
            va_list valst;
            #ifdef MATRIXOS_LOG_DEVICE
            Device::Log(msg, valst);
            #endif

            #ifdef MATRIXOS_LOG_USBCDC
            USB::CDC::Printf(msg, valst)
            #endif
        }

    }    


    void LogError (string tag, string format, ...)
    {
        #if MATRIXOS_LOG_LEVEL >= 1
        va_list valst;
        Log(ERROR, tag, format, valst);
        #endif
    }    

    void LogWarning (string tag, string format, ...)
    {
        #if MATRIXOS_LOG_LEVEL >= 2
        va_list valst;
        Log(WARNING, tag, format, valst);
        #endif
    }

    void LogInfo (string tag, string format, ...)
    {
        #if MATRIXOS_LOG_LEVEL >= 3
        va_list valst;
        Log(INFO, tag, format, valst);
        #endif
    }

    void LogDebug (string tag, string format, ...)
    {
        #if MATRIXOS_LOG_LEVEL >= 4
        va_list valst;
        Log(DEBUG, tag, format, valst);
        #endif
    }

    void LogVerbose (string tag, string format, ...)
    {
        #if MATRIXOS_LOG_LEVEL >= 5
        va_list valst;
        Log(VERBOSE, tag, format, valst);
        #endif
    }
}