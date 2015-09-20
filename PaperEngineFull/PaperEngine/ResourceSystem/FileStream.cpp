#include "util/PrecompileHead.h"
#include "ResourceSystem/FileStream.h"
#include "util/Log.h"

FileStream::FileStream() :
	mpFileHandler(0)
{
}

FileStream::FileStream(const char *filePath) :
	mpFileHandler(0)
{
	//默认为只读文件
	bool ret = openFile(filePath, Stream::AccessMode::ReadAccess);
}

FileStream::FileStream(const char *filePath, const Stream::AccessMode accessMode)
{
	bool ret = openFile(filePath, accessMode);
}

FileStream::~FileStream()
{
	closeFile();
}

void FileStream::open(const char *filename)
{
	open(filename, Stream::ReadAccess);
}

void FileStream::open(const char *filename, const Stream::AccessMode accessMode)
{
	openFile(filename, accessMode);
}

bool FileStream::openFile(const char *filePath, const Stream::AccessMode accessMode)
{
	closeFile();

	char *pMode = 0;
	if(accessMode == Stream::ReadAccess)
	{
		pMode = "r";
	}
	else if(accessMode == Stream::ReadWriteAccess)
	{
		pMode = "w+";
	}
	else if(accessMode == Stream::AppendAccess)
	{
		pMode = "a+";
	}

	mpFileHandler = fopen(filePath, pMode);

	if(mpFileHandler) //打开文件成功
	{
		return true;
	}

	Log::getInstancePtr()->logMsg("Read File Not Found");
	return false;
}

void FileStream::seek(const int offset, const SeekOrigin seekOrigin)
{
	int ret = 0;
	switch(seekOrigin)
	{
	case Stream::BEGIN:
		ret = fseek(mpFileHandler, offset, SEEK_SET);
		break;
	case Stream::CURRENT:
		ret = fseek(mpFileHandler, offset, SEEK_CUR);
		break;
	case Stream::END:
		ret = fseek(mpFileHandler, offset, SEEK_END);
		break;
	default:
		break;
	}

	if(ret == -1)
	{
		//error
	}
}

size_t FileStream::getSize() 
{
	seek(0, Stream::END);
	size_t byteSize = ftell(mpFileHandler);
	return byteSize;
}

bool FileStream::isEof()
{
	size_t pos = feof(mpFileHandler);
	if(pos != 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

size_t FileStream::read(char *buf)
{
	read(buf, getSize());
	return getSize();
}

size_t FileStream::read(char *buf, size_t byteSize)
{
	this->seek(0, Stream::SeekOrigin::BEGIN);
	size_t ret = fread(buf, byteSize, 1, mpFileHandler);
	return ret;
}

void FileStream::write(const char *buf, const int numBytes)
{
	fwrite(buf, numBytes, 1, mpFileHandler);
}

void FileStream::appendStr(const char *str, const uint size)
{
	//fprintf(mpFileHandler, str);
	fseek(mpFileHandler, 0, SEEK_END);
	fwrite(str, size, 1, mpFileHandler);
	fflush(mpFileHandler);
}

void FileStream::closeFile()
{
	if(mpFileHandler)
	{
		fclose(mpFileHandler);
		mpFileHandler = 0;
	}
}