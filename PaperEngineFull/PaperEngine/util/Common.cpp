#include "util/PrecompileHead.h"
#include "util/Common.h"


NameGenerator::NameGenerator(const std::string &prefix) :
mNext(0)
{
	mPrefix = prefix;
}

const std::string NameGenerator::generate()
{
	std::ostringstream outStr;
	outStr << mPrefix << mNext++;
	return outStr.str();
}
