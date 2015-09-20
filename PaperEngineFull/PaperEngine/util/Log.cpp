#include "util/PrecompileHead.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <tchar.h>

#include "util/Log.h"

template <> Log *Singleton<Log>::msInstance = 0;

FileStream Log::mFileStream;

Log *Log::getInstancePtr()
{
	return msInstance;
}

Log &Log::getInstance()
{
	assert(msInstance);
	return (*msInstance);
}

Log::Log()
{
	mFileStream.open("PaperDebug.txt", Stream::ReadWriteAccess);
}

Log::~Log()
{
}

void Log::logMsgStr(std::string msg)
{
	msg.append("\n");

	printf(msg.c_str());
	mFileStream.appendStr(msg.c_str(), msg.size());
}

void Log::logNvFX(const char *str)
{
	printf(str);
	mFileStream.appendStr(str, strlen(str));
}

void Log::logMsg(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	logMsgArg(format, args);
	va_end(args);
}

void Log::logMsgArg(const char *format, va_list args)
{
	char strBuffer[MAX_LOG_CHAR];
	memset(strBuffer, 0, MAX_LOG_CHAR);

	_vstprintf(strBuffer, format, args);

	//std::string outStr = strBuffer;
	//const char *pOutStr = strBuffer;

	logMsgStr(strBuffer);
}
