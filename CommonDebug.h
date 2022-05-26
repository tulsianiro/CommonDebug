#ifndef COMMON_DEBUG_H
#define COMMON_DEBUG_H

#ifndef CDBG_LOG_WARN_ENABLED
#define CDBG_LOG_WARN_ENABLED	1
#endif

#ifndef CDBG_LOG_INFO_ENABLED
#define CDBG_LOG_INFO_ENABLED	1
#endif 

#ifndef CDBG_LOG_DEBUG_ENABLED
#define CDBG_LOG_DEBUG_ENABLED	1
#endif 

#ifndef CDBG_LOG_TRACE_ENABLED
#define CDBG_LOG_TRACE_ENABLED	1
#endif

#ifndef CDBG_ASSERTIONS_ENABLED
#ifndef _NDEBUG
#define CDBG_ASSERTIONS_ENABLED	1
#else
#define CDBG_ASSERTIONS_ENABLED	0
#endif
#endif

typedef enum __CDBG_LogLevel
{
    LOG_LEVEL_FATAL = 0,
	LOG_LEVEL_ERROR,
	LOG_LEVEL_WARN,
	LOG_LEVEL_INFO,
	LOG_LEVEL_DEBUG,
	LOG_LEVEL_TRACE,
    LOG_LEVEL_COUNT
} __CDBG_LogLevel;

void LogMessage(__CDBG_LogLevel level, const char* message, ...);

#define LOG_FATAL(message, ...) LogMessage(LOG_LEVEL_FATAL, message, ##__VA_ARGS__);
#define LOG_ERROR(message, ...) LogMessage(LOG_LEVEL_ERROR, message, ##__VA_ARGS__);

#if CDBG_LOG_WARN_ENABLED == 1
#define LOG_WARN(message, ...) LogMessage(LOG_LEVEL_WARN, message, ##__VA_ARGS__);
#else
#define LOG_WARN(message, ...) 
#endif

#if CDBG_LOG_INFO_ENABLED == 1
#define LOG_INFO(message, ...) LogMessage(LOG_LEVEL_INFO, message, ##__VA_ARGS__);
#else
#define LOG_INFO(message, ...)
#endif

#if CDBG_LOG_DEBUG_ENABLED == 1
#define LOG_DEBUG(message, ...) LogMessage(LOG_LEVEL_DEBUG, message, ##__VA_ARGS__);
#else
#define LOG_DEBUG(message, ...)
#endif

#if CDBG_LOG_TRACE_ENABLED == 1
#define LOG_TRACE(message, ...) LogMessage(LOG_LEVEL_TRACE, message, ##__VA_ARGS__);
#else
#define LOG_TRACE(message, ...)
#endif

#if CDBG_ASSERTIONS_ENABLED == 1
#if _MSC_VER
#include <intrin.h>
#define debugBreak() __debugbreak()
#else
#define debugBreak() __builtin_trap()
#endif

void __CDBG_LogAssertionFailure(const char* expression, const char* message, const char* file, int line);

#define ASSERT(stmt)                                                    \
    do                                                                  \
    {                                                                   \
        if(stmt){}                                                      \
        else                                                            \
        {                                                               \
            __CDBG_LogAssertionFailure(#stmt, "", __FILE__, __LINE__);   \
            debugBreak();                                               \
        }                                                               \
    } while(0)              


#define ASSERT_MSG(stmt, msg)                                           \
    do                                                                  \
    {                                                                   \
        if(stmt){}                                                      \
        else                                                            \
        {                                                               \
            __CDBG_LogAssertionFailure(#stmt, msg, __FILE__, __LINE__);  \
            debugBreak();                                               \
        }                                                               \
    } while(0)     
#else
#define ASSERT(expr)
#define ASSERT_MSG(expr, msg)  
#endif

#endif