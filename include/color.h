#ifndef __COLOR_H__
#define __COLOR_H__


class Color 
{
public:
	
	explicit Color(float red = 1.0f, float green = 1.0f, float blue = 1.0f, float alpha = 1.0f)
		: r(red)
		, g(green)
		, b(blue)
		, a(alpha)
	{}

	float r, g, b, a; 
	
};


#endif