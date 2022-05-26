#include "CommonDebug.h"

#include <stdarg.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

static void PlatformConsoleWrite(const char* message, __CDBG_LogLevel color)
{
#ifdef _WIN32
	HANDLE stdoutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	static uint8_t levels[6] = 
	{ 
		BACKGROUND_RED,					 // FATAL
		FOREGROUND_RED,					 // ERROR
		FOREGROUND_RED|FOREGROUND_GREEN, // WARN
		FOREGROUND_GREEN,				 // INFO  
		FOREGROUND_BLUE,                 // DEBUG
		FOREGROUND_INTENSITY             // TRACE
	};
	SetConsoleTextAttribute(stdoutHandle, levels[color]);
	OutputDebugStringA(message);
	size_t length = strlen(message);
	LPDWORD numBytes = 0;
	WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), message, (DWORD)length, numBytes, 0);
#else
	const char* colorCodes[] = { "0;41", "1;31", "1;33", "1;32", "1;34", "1;30" };
	printf("\033[%sm%s\033[0m", colorCodes[color], message);
#endif
}

static void PlatformConsoleWriteError(const char* message, __CDBG_LogLevel color)
{
#ifdef _WIN32
	HANDLE stderrHandle = GetStdHandle(STD_ERROR_HANDLE);
	static uint8_t levels[6] = 
	{ 
		BACKGROUND_RED,					 // FATAL
		FOREGROUND_RED,					 // ERROR
		FOREGROUND_RED|FOREGROUND_GREEN, // WARN
		FOREGROUND_GREEN,				 // INFO  
		FOREGROUND_BLUE,                 // DEBUG
		FOREGROUND_INTENSITY             // TRACE
	};
	SetConsoleTextAttribute(stderrHandle, levels[color]);
	OutputDebugStringA(message);
	size_t length = strlen(message);
	LPDWORD numBytes = 0;
	WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), message, (DWORD)length, numBytes, 0);
#else
	const char* colorCodes[] = { "0;41", "1;31", "1;33", "1;32", "1;34", "1;30" };
	printf("\033[%sm%s\033[0m", colorCodes[color], message);
#endif
}

void LogMessage(__CDBG_LogLevel level, const char* message, ...)
{
	const char* levelTags[6] =
	{
		"[FATAL]: ", "[ERROR]: ", "[WARN]: ", "[INFO]: ", "[DEBUG]: ", "[TRACE]: "
	};

	int isError = level < LOG_LEVEL_WARN;
	// TODO(rohan): support multithreaded logger
	static char outMessage[32768];
	static char copyBuffer[32768];

	va_list argList;
	va_start(argList, message);
	vsnprintf(outMessage, sizeof(outMessage), message, argList);
	va_end(argList);

	sprintf(copyBuffer, "%s%s\n", levelTags[level], outMessage);
	if(isError)
	{
		PlatformConsoleWriteError(copyBuffer, level);
	}
	else
	{
		PlatformConsoleWrite(copyBuffer, level);
	}
}

void __CDBG_LogAssertionFailure(const char* expression, const char* message, const char* file, int line)
{
    LogMessage(LOG_LEVEL_FATAL, "Assertion Failure: %s, Message: '%s', In File: %s, Line: %d\n", expression, message, file, line);
}

int main()
{
    LOG_TRACE("HELLO %d", 2);
    ASSERT_MSG(0, "This is an assert :)");
}
