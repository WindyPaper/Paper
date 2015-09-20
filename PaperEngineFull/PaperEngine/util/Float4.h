#ifndef _FLOAT4_H_
#define _FLOAT4_H_

namespace math
{

	class Float4
	{
	public:
		Float4() :
			x(0),
			y(0),
			z(0),
			w(0)
		{

		}

		Float4(const float x, const float y, const float z, const float w) :
			x(x),
			y(y),
			z(z),
			w(w)
		{

		}

		float getX() const { return x; }
		float getY() const { return y; }
		float getZ() const { return z; }
		float getW() const { return w; }

	private:
		float x, y, z, w;
	};

}

#endif