#ifndef _STREAM_H_
#define _STREAM_H_

#include "util/Platform.h"

class ENGINE_DLL Stream
{
public:
	Stream();
	virtual ~Stream();

	virtual void open();
	virtual bool isOpen() const;

	enum AccessMode
	{
		ReadAccess,
		WriteAccess,
		AppendAccess,
		ReadWriteAccess
	};

	enum SeekOrigin
	{
		BEGIN,
		CURRENT,
		END
	};
	virtual void seek(const int offset, const SeekOrigin seekOrigin) = 0;
	virtual bool isEof() = 0;
	virtual size_t read(char *buf, size_t byteSize) = 0;
	virtual void write(const char *buf, const int numBytes) = 0;
	//virtual const void *map() = 0;
	//virtual void unmap() = 0;

protected:
	virtual bool copy(const char *from, char *to, const int numBytes);

protected:
	bool mIsOpen;
	//bool mIsMapped;
};

#endif