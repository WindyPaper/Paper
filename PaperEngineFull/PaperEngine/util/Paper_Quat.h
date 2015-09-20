#ifndef _PAPER_QUAT_H_
#define _PAPER_QUAT_H_

#include "XNAMath/xnamath.h"

namespace math
{

	struct Vector3f;
	class Matrix4f;

	struct Quaternion
	{
		//float x, y, z, w;
		XMVECTOR vec4;

		Quaternion(float _x, float _y, float _z, float _w);
		Quaternion();

		void Normalize();

		Quaternion Conjugate() const;

		void fromAngleAxis(const math::Vector3f &axis, const float angle);

		Matrix4f toMatrix() const;
	};


	Quaternion operator*(const math::Quaternion& l, const math::Quaternion& r);

	Vector3f operator*(const math::Quaternion& q, const math::Vector3f& v);
}




#endif