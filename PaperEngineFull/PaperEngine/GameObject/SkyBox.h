#ifndef _SKY_BOX_H_
#define _SKY_BOX_H_

#include "GameObject/GameObject.h"

class SkyBox : public GameObject
{
public:
	SkyBox(const std::string &name);
	virtual ~SkyBox();

	void init();

};

#endif