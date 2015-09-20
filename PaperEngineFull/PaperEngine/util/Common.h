#ifndef _COMMON_H_
#define _COMMON_H_

#include <string>

class NameGenerator
{
public:
	NameGenerator(const std::string &prefix);

	const std::string generate();

protected:
	std::string mPrefix;
	unsigned long long int mNext;
};

#endif