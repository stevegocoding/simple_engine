#ifndef __APPLICATION_H__
#define __APPLICATION_H__


class AppConfig 
{
	
};


class Application 
{ 
public:
	
	virtual int init(AppConfig& config);
	virtual void destory();
	virtual int run();
	virtual void update();
	
	

};


#endif