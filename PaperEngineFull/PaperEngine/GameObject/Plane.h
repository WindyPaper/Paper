#ifndef _PLANE_H_
#define _PLANE_H_

#include "GameObject/GameObject.h"

struct Vertex_P3UVF2;

class Plane : public GameObject
{
public:
	Plane(const int width, const int height, const int seg);
	Plane(const std::string &name);

	~Plane();

	void setPlaneSize(const int width, const int height, const int seg);

private:
	int mWidth;
	int mHeight;
	int mSeg;

	Vertex_P3UVF2 *mpVerMemData;
	unsigned int *mpIndexMemData;
};

#endif