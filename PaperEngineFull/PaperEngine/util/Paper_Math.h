#ifndef _PAPER_MATH_H_
#define _PAPER_MATH_H_

#include <math.h>
#include <memory>
#include "util/Platform.h"
#include "util/Paper_Quat.h"
#include "XNAMath/xnamath.h"

#define M_PI 3.1415

#define ToRadian(x) ((x) * M_PI / 180.0f)
#define ToDegree(x) ((x) * 180.0f / M_PI)

namespace math 
{
struct ENGINE_DLL Vector2f
{
	float x;
	float y;

	Vector2f()
	{
		x = 0;
		y = 0;
	}

	Vector2f(const float x, const float y)
	{
		this->x = x;
		this->y = y;
	}

	Vector2f(const Vector2f &other)
	{
		x = other.x;
		y = other.y;
	}
};


struct ENGINE_DLL Vector3f
{
	float x;
	float y;
	float z;

	Vector3f()
	{
		x = 0;
		y = 0;
		z = 0;
	}

	Vector3f(const Vector3f &other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
	}

	Vector3f(const float x, const float y, const float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Vector3f cross(const Vector3f &v) const
	{
		const float _x = y * v.z - z * v.y;
		const float _y = z * v.x - x * v.z;
		const float _z = x * v.y - y * v.x;

		return Vector3f(_x, _y, _z);
	}

	Vector3f& normalize()
	{
		const float length = sqrtf(x * x + y * y + z * z);

		if (length > 0.000001)
		{
			x /= length;
			y /= length;
			z /= length;
		}
		

		return *this;
	}

	Vector3f operator -() const
	{
		return Vector3f(-x, -y, -z);
	}

	Vector3f operator += (const Vector3f &right)
	{
		x += right.x;
		y += right.y;
		z += right.z;

		return *this;
	}

	Vector3f operator -= (const Vector3f &right)
	{
		x -= right.x;
		y -= right.y;
		z -= right.z;

		return *this;
	}

	Vector3f operator *= (const float &num)
	{
		x *= num;
		y *= num;
		z *= num;

		return *this;
	}

	Vector3f operator - (const Vector3f &vec)
	{
		return Vector3f(x - vec.x, y - vec.y, z - vec.z);
	}

	bool operator !=(const Vector3f &vec)
	{
		if(x == vec.x && y == vec.y && z == vec.z)
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	Vector3f mul(const Vector3f &rhs) const
	{
		return Vector3f(x * rhs.x, y * rhs.y, z * rhs.z);
	}

	friend Vector3f operator *(const Vector3f &vec, const float num)
	{
		Vector3f retVec;
		retVec.x = num * vec.x;
		retVec.y = num * vec.y;
		retVec.z = num * vec.z;

		return retVec;
	}

	friend Vector3f operator *(const float num, const Vector3f &vec)
	{
		Vector3f retVec;
		retVec.x = num * vec.x;
		retVec.y = num * vec.y;
		retVec.z = num * vec.z;

		return retVec;
	}

	friend float operator *(const Vector3f &l, const Vector3f &r)
	{
		return l.x * r.x + l.y * r.y + l.z * r.z;
	}

	Vector3f rotate(const float angle, const Vector3f &axe)
	{
		/*const float SinHalfAngle = sinf(ToRadian(angle/2));
		const float CosHalfAngle = cosf(ToRadian(angle/2));

		const float Rx = axe.x * SinHalfAngle;
		const float Ry = axe.y * SinHalfAngle;
		const float Rz = axe.z * SinHalfAngle;
		const float Rw = CosHalfAngle;*/
		//Quaternion RotationQ(Rx, Ry, Rz, Rw);
		Quaternion RotationQ;

		XMVECTOR axis = axe;
		XMVECTOR quat = XMQuaternionRotationAxis(axis, ToRadian(angle));

		/*Quaternion ConjugateQ = RotationQ.Conjugate();
		ConjugateQ.Normalize();
		Quaternion W = RotationQ * (*this) * ConjugateQ;

		x = W.x;
		y = W.y;
		z = W.z;*/
		RotationQ.vec4 = quat;
		rotate(RotationQ);
		return *this;
	}

	Vector3f rotate(const Quaternion &quat)
	{
		XMVECTOR srcVec = *this;
		XMVECTOR vec = XMVector3Rotate(srcVec, quat.vec4);

		x = vec.x;
		y = vec.y;
		z = vec.z;

		return *this;
	}

	operator XMVECTOR() const
	{
		XMVECTOR vec;
		memset(&vec, 0, sizeof(XMVECTOR));
		vec.x = x;
		vec.y = y;
		vec.z = z;

		return vec;
	}
};

const Vector3f VEC3F_UNIT_X(1, 0, 0);
const Vector3f VEC3F_UNIT_Y(0, 1, 0);
const Vector3f VEC3F_UNIT_Z(0, 0, 1);
const Vector3f VEC3F_NEGATIVE_UNIT_X(-1, 0, 0);
const Vector3f VEC3F_NEGATIVE_UNIT_Y(0, -1, 0);
const Vector3f VEC3F_NEGATIVE_UNIT_Z(0, 0, -1);
const Vector3f VEC3F_UNIT_SCALE(1, 1, 1);
const Vector3f VEC3F_ZERO(0, 0, 0);

struct Vector4f
{
	XMVECTOR vec4;

	Vector4f()
	{
		memset(&vec4, 0, sizeof(XMVECTOR));
	}

	Vector4f(float x, float y, float z, float w)
	{
		vec4.x = x;
		vec4.y = y;
		vec4.z = z;
		vec4.w = w;
	}
};

struct ENGINE_DLL Vertex
{
	Vector3f pos;
	Vector2f texCood;
	Vector3f normal;

	Vertex() :
		pos(0.0, 0.0, 0.0),
		texCood(0.0, 0.0),
		normal(0.0, 0.0, 0.0)
	{
	}

	Vertex(Vector3f pos, Vector2f texcood, Vector3f normal) :
		pos(pos),
		texCood(texcood),
		normal(normal)
	{}

};

struct ENGINE_DLL PerspectiveProjParam
{
	float fov;
	float width;
	float height;
	float zNear;
	float zFar;

	PerspectiveProjParam()
	{
		fov = 30;
		width = 960;
		height = 640;
		zNear = 0.1;
		zFar = 1000;
	}
};

class ENGINE_DLL Matrix4f
{
public:
	//float m[4][4];
	XMMATRIX m;

	const XMMATRIX *getData() const
	{
		return &m;
	}

	Matrix4f()
	{
		memset(this, 0, sizeof(Matrix4f));
	}

	Matrix4f(const Matrix4f &other)
	{
		if(this != &other)
			*this = other;
	}

	inline void InitIdentity()
	{
		/*m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
		m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
		m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
		m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;*/
		m = XMMatrixIdentity();
	}

	inline Matrix4f operator*(const Matrix4f& Right) const
	{
		Matrix4f ret;

		/*for (unsigned int i = 0 ; i < 4 ; i++) {
			for (unsigned int j = 0 ; j < 4 ; j++) {
				Ret.m[i][j] = m[i][0] * Right.m[0][j] +
					m[i][1] * Right.m[1][j] +
					m[i][2] * Right.m[2][j] +
					m[i][3] * Right.m[3][j];
			}
		}*/
		ret.m = XMMatrixMultiply(m, Right.m);

		return ret;
	}

	inline Matrix4f &operator =(const Matrix4f &right)
	{
		if(this != &right)
		{
			int size = sizeof(m);
			memcpy(&m, &right.m, size);
		}
		return *this;
	}


	void initTranslateTransform(const Vector3f &translate)
	{
		/*m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = translate.x;
		m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = translate.y;
		m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = translate.z;
		m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;*/
		m = XMMatrixTranslation(translate.x, translate.y, translate.z);
	}

	void initRotateTransform(const Vector3f &rotation)  
	{
		//Matrix4f rotateMX, rotateMY, rotateMZ;

		/*rotateMX.m[0][0] = 1.0f; rotateMX.m[0][1] = 0.0f; rotateMX.m[0][2] = 0.0f; rotateMX.m[0][3] = 0.0f;
		rotateMX.m[1][0] = 0.0f; rotateMX.m[1][1] = cosf(rotation.x); rotateMX.m[1][2] = -sinf(rotation.x); rotateMX.m[1][3] = 0.0f;
		rotateMX.m[2][0] = 0.0f; rotateMX.m[2][1] = sinf(rotation.x); rotateMX.m[2][2] = cosf(rotation.x); rotateMX.m[2][3] = 0.0f;
		rotateMX.m[3][0] = 0.0f; rotateMX.m[3][1] = 0.0f; rotateMX.m[3][2] = 0.0f; rotateMX.m[3][3] = 1.0f;

		rotateMY.m[0][0] = cosf(rotation.y); rotateMY.m[0][1] = 0.0f; rotateMY.m[0][2] = -sinf(rotation.y); rotateMY.m[0][3] = 0.0f;
		rotateMY.m[1][0] = 0.0f; rotateMY.m[1][1] = 1.0f; rotateMY.m[1][2] = 0.0f; rotateMY.m[1][3] = 0.0f;
		rotateMY.m[2][0] = sinf(rotation.y); rotateMY.m[2][1] = 0.0f; rotateMY.m[2][2] = cosf(rotation.y); rotateMY.m[2][3] = 0.0f;
		rotateMY.m[3][0] = 0.0f; rotateMY.m[3][1] = 0.0f; rotateMY.m[3][2] = 0.0f; rotateMY.m[3][3] = 1.0f;

		rotateMZ.m[0][0] = cosf(rotation.z); rotateMZ.m[0][1] = -sinf(rotation.z); rotateMZ.m[0][2] = 0.0f; rotateMZ.m[0][3] = 0.0f;
		rotateMZ.m[1][0] = sinf(rotation.z); rotateMZ.m[1][1] = cosf(rotation.z); rotateMZ.m[1][2] = 0.0f; rotateMZ.m[1][3] = 0.0f;
		rotateMZ.m[2][0] = 0.0f; rotateMZ.m[2][1] = 0.0f; rotateMZ.m[2][2] = 1.0f; rotateMZ.m[2][3] = 0.0f;
		rotateMZ.m[3][0] = 0.0f; rotateMZ.m[3][1] = 0.0f; rotateMZ.m[3][2] = 0.0f; rotateMZ.m[3][3] = 1.0f;

		*this = rotateMZ * rotateMY * rotateMX;*/
		m = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
	}

	void initScaleTransform(const Vector3f &scale)  
	{
		/*m[0][0] = scale.x; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
		m[1][0] = 0.0f; m[1][1] = scale.y; m[1][2] = 0.0f; m[1][3] = 0.0f;
		m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = scale.z; m[2][3] = 0.0f;
		m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;*/
		m = XMMatrixScaling(scale.x, scale.y, scale.z);
	}

	void initFullTransform(const Vector3f &pos, const Vector3f &scale, const Quaternion &quat)
	{
		XMVECTOR posVec;
		posVec = pos;

		XMVECTOR scaleVec;
		scaleVec = scale;

		XMVECTOR rotateOrigin;
		memset(&rotateOrigin, 0, sizeof(XMVECTOR));

		XMVECTOR scaleOrigin;
		memset(&scaleOrigin, 0, sizeof(XMVECTOR));

		XMVECTOR scaleQuat;
		scaleQuat = Vector3f(0, 0, 0);
		scaleQuat.w = 1;

		m = XMMatrixAffineTransformation(scaleVec, rotateOrigin, quat.vec4, posVec);
		//m = XMMatrixTransformation(scaleOrigin, scaleQuat, scaleVec, rotateOrigin, quat.vec4, posVec);
		
	}

	void initPerspectiveProj(const PerspectiveProjParam &param) 
	{
		/*const float ar = param.width / param.height;
		const float zNear = param.zNear;
		const float zFar = param.zFar;
		const float zRange = param.zNear - param.zFar;
		const float tanHalfFov = tanf(ToRadian(param.fov/2.0));*/

		/*float mm[4][4];
		mm[0][0] = 1.0f / (tanHalfFov * ar); mm[0][1] = 0.0f; mm[0][2] = 0.0f; mm[0][3] = 0.0f;
		mm[1][0] = 0.0f; mm[1][1] = 1.0f / tanHalfFov; mm[1][2] = 0.0f; mm[1][3] = 0.0f;
		mm[2][0] = 0.0f; mm[2][1] = 0.0f; mm[2][2] = (-zNear - zFar) / zRange; mm[2][3] = 2.0f * zNear * zFar / zRange;
		mm[3][0] = 0.0f; mm[3][1] = 0.0f; mm[3][2] = 1.0f; mm[3][3] = 0.0f; */

		m = XMMatrixPerspectiveFovRH(ToRadian(param.fov), param.width/param.height, param.zNear, param.zFar);
	}

	void initCameraTransform(const Vector3f &target, const Vector3f &up)
	{
		Vector3f n = target;
		n.normalize();
		Vector3f u = up;
		u.normalize();
		u = u.cross(target);
		Vector3f v = n.cross(u);

		/*m[0][0] = u.x; m[0][1] = u.y; m[0][2] = u.z; m[0][3] = 0.0f;
		m[1][0] = v.x; m[1][1] = v.y; m[1][2] = v.z; m[1][3] = 0.0f;
		m[2][0] = n.x; m[2][1] = n.y; m[2][2] = n.z; m[2][3] = 0.0f;
		m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;*/

		XMVECTOR eyePos;
		XMVECTOR targetVec;
		targetVec = target;
		XMVECTOR upVec;
		upVec = up;
		m = XMMatrixLookToRH(eyePos, targetVec, upVec);
	}
};

//typedef  eg:easy to replace
typedef Matrix4f Matrix44;

}


#endif