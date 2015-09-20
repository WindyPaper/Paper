#include "util/PrecompileHead.h"
#include "Stream.h"

Stream::Stream() :
	mIsOpen(false)
	//mIsMapped(false)
{

}

Stream::~Stream()
{

}

void Stream::open()
{
	mIsOpen = true;
}

bool Stream::isOpen() const
{
	return mIsOpen;
}

bool Stream::copy(const char *from, char *to, const int numBytes)
{
	return false;
}

