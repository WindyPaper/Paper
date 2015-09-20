#ifndef _LOG_H_
#define _LOG_H_

#include "util/Singleton.h"
#include "ResourceSystem/FileStream.h"
#include <string>

class ENGINE_DLL Log : public Singleton<Log>
{
public:
	Log();
	~Log();

	static Log &getInstance();
	static Log *getInstancePtr();

	static void logMsgStr(std::string msg);
	static void logNvFX(const char *str);
	
	static void logMsg(const char *format, ...);

protected:
	static void logMsgArg(const char *format, va_list args);

private:
	static FileStream mFileStream;
};

#endif