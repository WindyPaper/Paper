#include "util/PrecompileHead.h"
#include "util/Paper_Quat.h"
#include "util/Paper_Math.h"

namespace math
{

	Quaternion::Quaternion(float _x, float _y, float _z, float _w)
	{
		vec4.x = _x;
		vec4.y = _y;
		vec4.z = _z;
		vec4.w = _w;
	}

	Quaternion::Quaternion() 
	{
		memset(&vec4, 0, sizeof(XMVECTOR));
	}

	void Quaternion::Normalize()
	{
		float Length = sqrtf(vec4.x * vec4.x + vec4.y * vec4.y + vec4.z * vec4.z + vec4.w * vec4.w);

		vec4.x /= Length;
		vec4.y /= Length;
		vec4.z /= Length;
		vec4.w /= Length;
	}


	Quaternion Quaternion::Conjugate() const
	{
		Quaternion ret(-vec4.x, -vec4.y, -vec4.z, vec4.w);
		return ret;
	}

	void Quaternion::fromAngleAxis(const math::Vector3f &axis, const float angle)
	{
		vec4 = XMQuaternionRotationAxis(axis, ToRadian(angle));
	}

	Matrix4f Quaternion::toMatrix() const
	{
		Matrix4f mat;
		mat.m = XMMatrixRotationQuaternion(vec4);
		return mat;
	}


	math::Quaternion operator*(const math::Quaternion& l, const math::Quaternion& r)
	{		
		/*const float w = (l.vec4.w * r.vec4.w) - (l.vec4.x * r.vec4.x) - (l.vec4.y * r.vec4.y) - (l.vec4.z * r.vec4.z);
		const float x = (l.vec4.x * r.vec4.w) + (l.vec4.w * r.vec4.x) + (l.vec4.y * r.vec4.z) - (l.vec4.z * r.vec4.y);
		const float y = (l.vec4.y * r.vec4.w) + (l.vec4.w * r.vec4.y) + (l.vec4.z * r.vec4.x) - (l.vec4.x * r.vec4.z);
		const float z = (l.vec4.z * r.vec4.w) + (l.vec4.w * r.vec4.z) + (l.vec4.x * r.vec4.y) - (l.vec4.y * r.vec4.x);*/

		//Returns the product of two quaternions as Q2*Q1.
		XMVECTOR retVec4 = XMQuaternionMultiply(l.vec4, r.vec4);
		math::Quaternion ret(retVec4.x, retVec4.y, retVec4.z, retVec4.w);

		return ret;
	}

	math::Vector3f operator*(const math::Quaternion& q, const math::Vector3f& v)
	{
		/*const float w = -(q.vec4.x * v.x) - (q.vec4.y * v.y) - (q.vec4.z * v.z);
		const float x = (q.vec4.w * v.x) + (q.vec4.y * v.z) - (q.vec4.z * v.y);
		const float y = (q.vec4.w * v.y) + (q.vec4.z * v.x) - (q.vec4.x * v.z);
		const float z = (q.vec4.w * v.z) + (q.vec4.x * v.y) - (q.vec4.y * v.x);

		math::Quaternion ret(x, y, z, w);*/

		Vector3f vn(v);
		vn.normalize();

		Quaternion vecQuat, resQuat;
		vecQuat.vec4.x = vn.x;
		vecQuat.vec4.y = vn.y;
		vecQuat.vec4.z = vn.z;
		vecQuat.vec4.w = 0.0f;

		resQuat = vecQuat * q.Conjugate();
		resQuat = q * resQuat;

		return (Vector3f(resQuat.vec4.x, resQuat.vec4.y, resQuat.vec4.z));

		//return ret;
	}

}
