#include "util/PrecompileHead.h"
#include "util/System.h"
#include "util/Log.h"

System::System() :
pLog(0)
{

}

System::~System()
{
	SAFE_DELETE(pLog);
}

void System::init()
{
	pLog = new Log();
}
