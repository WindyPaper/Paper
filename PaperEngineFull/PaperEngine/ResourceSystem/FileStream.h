#ifndef _FILE_STREAM_H_
#define _FILE_STREAM_H_

#include "util/Platform.h"
#include "util/Engine_Define.h"
#include "ResourceSystem/Stream.h"
#include <stdio.h>

class ENGINE_DLL FileStream : public Stream
{
public:
	FileStream();
	FileStream(const char *filePath);
	FileStream(const char *filePath, const Stream::AccessMode accessMode);
	virtual ~FileStream();
	
	virtual void seek(const int offset, const SeekOrigin seekOrigin);
	virtual bool isEof();
	virtual size_t read(char *buf);
	virtual size_t read(char *buf, size_t byteSize);
	virtual void write(const char *buf, const int numBytes);
	//virtual const void *map();
	//virtual void unmap();
	void open(const char *filename);
	void open(const char *filename, const Stream::AccessMode accessMode);
	void appendStr(const char *str, const uint size);

	//void readFromFile(const char *filePath);
	size_t getSize();

protected:
	virtual bool openFile(const char *filePath, const Stream::AccessMode accessMode);
	void closeFile();

private:
	FILE *mpFileHandler;
};

#endif