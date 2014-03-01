#ifndef __TYPES_H__
#define __TYPES_H__


typedef unsigned long	ulong;
typedef unsigned int	uint;
typedef unsigned short	ushort;
typedef unsigned char	uchar;

typedef unsigned int		uint32;
typedef unsigned short		uint16;
typedef unsigned char		uint8;

typedef signed int			int32;
typedef signed short		int16;
typedef signed char			int8;

typedef float				f32;
typedef double				f64; 


template <typename T>
struct point 
{
	T x,y;
};

template<typename T>
struct size
{
	T w,h;
};

template<typename T>
struct Point : public point<T>
{
	// create an uninitialized point
	Point<T>();
	// create from two integers
	Point<T>(T x, T y);
	// create from another point
	Point<T>(const point<T>& p);
	// create from a size
	Point<T>(const size<T>& s);


	// Operations

	// translate the point
	void Offset(T xOffset, T yOffset);
	void Offset(const point<T>& point);
	void Offset(const size<T>& size);
	void Set(T X, T Y);
	void Set(const Point<T>& p);

	bool operator==(const point<T>& point) const;
	bool operator!=(const point<T>& point) const;
	void operator+=(const size<T>& size);
	void operator-=(const size<T>& size);
	void operator+=(const point<T>& point);
	void operator-=(const point<T>& point);

	// Operators returning Point values
	Point<T> operator+(const size<T>& size) const;
	Point<T> operator-(const size<T>& size) const;
	Point<T> operator-() const;
	Point<T> operator+(const point<T>& point) const; 
	Point<T> operator-(const point<T>& pt) const; 

	// Cast operators
	operator T *();
	operator const T *() const;
};


template<typename T>
inline Point<T>::Point()
{ 
	// random filled 
}

template<typename T>
inline Point<T>::Point(T initX, T initY)
{
	this->x = initX;
	this->y = initY;
}

template<typename T>
inline Point<T>::Point(const point<T>& initPt)
{
	*(Point<T>*)this = initPt;
}

template<typename T>
inline Point<T>::Point(const size<T>& initSize)
{ 
	this->x = initSize.w;
	this->y = initSize.h;
	//*(size*)this = initSize;
}

template<typename T>
inline void Point<T>::Offset(T xOffset, T yOffset)
{
	this->x += xOffset;
	this->y += yOffset;
}

template<typename T>
inline void Point<T>::Offset(const point<T>& point)
{
	this->x += point.x;
	this->y += point.y;
}

template<typename T>
inline void Point<T>::Offset(const size<T>& size)
{
	this->x += size.w;
	this->y += size.h;
}

template<typename T>
inline void Point<T>::Set(T X, T Y)
{
	this->x = X;
	this->y = Y;
}

template<typename T>
void Point<T>::Set(const Point<T>& p) {
	this->x = p.x;
	this->y = p.y;
}

template<typename T>
inline bool Point<T>::operator==(const point<T>& point) const
{
	return (this->x == point.x && this->y == point.y);
}

template<typename T>
inline bool Point<T>::operator!=(const point<T>& point) const
{
	return (this->x != point.x || this->y != point.y);
}

template<typename T>
inline void Point<T>::operator+=(const size<T>& size)
{
	this->x += size.w;
	this->y += size.h;
}

template<typename T>
inline void Point<T>::operator-=(const size<T>& size)
{
	this->x -= size.w;
	this->y -= size.h;
}

template<typename T>
inline void Point<T>::operator+=(const point<T>& point)
{
	this->x += point.x;
	this->y += point.y;
}

template<typename T>
inline void Point<T>::operator-=(const point<T>& point)
{
	this->x -= point.x;
	this->y -= point.y;
}

template<typename T>
inline Point<T> Point<T>::operator+(const size<T>& size) const
{
	return Point<T>(this->x + size.w, this->y + size.h);
}

template<typename T>
inline Point<T> Point<T>::operator-(const size<T>& size) const
{
	return Point<T>(this->x - size.w, this->y - size.h);
}

template<typename T>
inline Point<T> Point<T>::operator-() const
{
	return Point<T>(-this->x, -this->y);
}

template<typename T>
inline Point<T> Point<T>::operator+(const point<T>& point) const
{
	return Point<T>(this->x + point.x, this->y + point.y);
}

template<typename T>
inline Point<T> Point<T>::operator-(const point<T>& pt) const
{
	return Point<T>(this->x + pt.x, this->y + pt.y);
}
	
template<typename T>
inline Point<T>::operator T *()
{
	return &this->x;
}

template<typename T>
inline Point<T>::operator const T *() const
{
	return &this->x;
}





#endif 