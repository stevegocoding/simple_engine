#ifndef __EVENT_H__
#define __EVENT_H__

enum EventTypeEnum
{
	E_FRAME_EVENT = 0
};

struct Event 
{
	Event(unsigned short _type)
		: type(_type) {}

	unsigned short type;
};


class EventListener 
{
public:
	virtual void on_event(const Event& e) = 0;
};



class EventManager 
{
	
};


#endif