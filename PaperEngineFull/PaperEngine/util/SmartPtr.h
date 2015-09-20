#ifndef _SMART_PTR_H_
#define _SMART_PTR_H_

template <typename T>
class SmartPtr
{
public:
	SmartPtr() : 
		mp(0)
	{
	}

	SmartPtr(T *p)
	{
		mp = p;
		if(mp)
		{
			mp->addRef();
		}
	}

	SmartPtr(const SmartPtr<T> &p)
	{
		mp = p.mp;
		if(mp)
		{
			mp->addRef();
		}
	}

	~SmartPtr()
	{
		if(mp)
		{
			mp->release();
			mp = 0;
		}
	}

	T *get() const { return mp; }

	void swap(SmartPtr<T> &other)
	{
		std::swap(mp, other.mp);
	}

	void reset()
	{
		SmartPtr<T> t();
		t.swap(*this);
	}

	//operator type translate eg: Class::operator Type (); Class c; Type t = c;
	operator bool() const
	{
		return mp != 0;
	}

	SmartPtr &operator =(T *p)
	{
		if(p)
		{
			p->addRef();
		}
		if(mp)
		{
			mp->release();
		}

		mp = p;
		return *this;
	}
	SmartPtr &operator =(const SmartPtr<T> &p)
	{
		if(p)
		{
			p->addRef();
		}
		if(mp)
		{
			mp->release();
		}

		mp = p;
		return *this;
	}
	SmartPtr &operator =(T &other) // warning, need test
	{
		other.addRef();
		if(mp)
		{
			mp->release();
		}
		mp = &other;
		return *this;
	}
	operator T*() const { return mp; } 
	operator const T*() const { return mp; }
	operator T&() const { return *mp; }   //warning, need test
	//operator const T&() const { return *mp; }

	T &operator *() const { return *mp; }
	T *operator ->() const { return mp; }
	bool operator !() const { return p == 0; }
	bool operator ==(const T *p) const { return mp == p; }
	bool operator ==(const SmartPtr<T> &rhs) const { return mp == rhs.mp; }
	bool operator !=(const T *p) { return mp != p; }
	bool operator !=(const SmartPtr<T> &rhs) { return mp != rhs.mp; }

	template<class BASET> const SmartPtr<BASET>& upcast() const
	{
		return *reinterpret_cast<const SmartPtr<BASET>*>(this);
	}

	// give up unvalid SmartPtr
	friend void releaseOwnerchip(SmartPtr<T> &ptr)
	{
		T *p = ptr.mp;
		ptr.mp = 0;
		return p;
	}

private:
	T *mp;
};

#endif