#ifndef __INPUT_DEVICE_H__
#define __INPUT_DEVICE_H__

#include <boost/circular_buffer.hpp>
#include "types.h"
#include "input_event_types.h"

class InputDevice;


class SimpleButton
{
public: 
	SimpleButton();
	
	bool is_down() const; 
	bool is_up() const;
	int get_down_count() const; 
	int get_up_count() const; 
	void reset_frame(); 
	void update_frame(bool is_pressed); 
	bool was_pressed() const; 
	bool was_released() const; 
	
	void set_value(unsigned short val); 
	unsigned short get_value() const; 
	
private:

	bool was_down() const; 
	bool was_up() const; 
	
	static const unsigned short IS_DOWN_MASK	= 0x8000; 
	static const unsigned short WAS_DOWN_MASK	= 0x4000; 
	static const unsigned short DOWN_COUNT_MASK = 0x3FFF; 

	unsigned short m_state; 
	unsigned short m_value; 
};



class TouchPad
{
public:
	static const size_t NUM_TOUCHES			= 10; //like 10 fingers
	static const size_t TOUCH_MAX_LENGTH	= 60; //1 second @ 60 hz

	struct Touch
	{ 
		Touch();

		static const uint UNDEFINED_TOUCH_ID = (uint)~0; 
		
		unsigned int m_touch_id;
		Point<short> m_first;
		boost::circular_buffer<Point<short> > m_points; 
	};
	
	TouchPad();
	
	int get_touch_count() const; 
	const Touch& get_touch_by_idx(int idx) const; 
	const Touch& get_touch_by_id(uint id) const; 
	
	void raise_touch_event(
		InputDevice* device, 
		int touch_pad_idx,
		InputEventTypes event,
		uint touch_id, 
		float x, float y); 

	bool update(InputEventTypes evt_type, unsigned int touch_id, short x, short y); 

private:
	
	int m_active_touches_idx[NUM_TOUCHES]; 
	int m_num_active_touches; 
	Touch m_touches[NUM_TOUCHES]; 
}; 


class Cursor
{
public: 
	Cursor();
	
	void update_frame(bool is_visible, short x, short y); 
	
	bool has_moved() const; 
	
	const Point<short>& get_pos() const { return m_pos; }

	const Point<short>& get_last_pos() const { return m_last_pos; }
	
	Point<short> get_delta() const { return m_pos - m_last_pos; }
	
	void reset_frame() 
	{
		m_last_pos = m_pos;
	} 

private: 
	Point<short> m_last_pos; 
	Point<short> m_pos; 

};

////////////////////////////////////////////////////////////////////////// 

class InputDevice
{
public: 
	InputDevice();
	virtual ~InputDevice();
	
	virtual bool presend_event(CoreEvent& event) { return true; }
	virtual void reset_frame(); 
	void set_updated(); 

	void update_button(unsigned int idx, bool is_down); 
	void update_cursor(uint idx, int x, int y); 

protected: 
	typedef std::vector<SimpleButton> ButtonsArray;
	typedef std::vector<Cursor> CursorsArray;

	ButtonsArray m_btns; 
	CursorsArray m_cursors; 
	TouchPad m_touchpad; 
};

////////////////////////////////////////////////////////////////////////// 

class Mouse : public InputDevice 
{
public: 
	
	enum Btn {
		BTN_NONE = -1,
		BTN_LEFT, BTN_RIGHT, BTN_MIDDLE,
		BTN_WHEELUP, BTN_WHEELDOWN,
		BTN_COUNT
	};

	Mouse(); 

	virtual bool presend_event(CoreEvent& event);
	virtual void reset_frame(); 

	void raise_move_event(float x, float y); 
	void raise_btn_event(int idx, bool is_down);

	const Point<short>& get_pos() const 
	{
		return m_cursor.get_pos(); 
	}
	
	bool has_moved() const 
	{
		return m_cursor.has_moved(); 
	}
	
	bool is_down(Btn btn) const 
	{
		return m_btns[btn].is_down(); 
	}
	
	bool is_up(Btn btn) const
	{
		return m_btns[btn].is_up(); 
	}

	int get_down_count(Btn btn) const 
	{
		return m_btns[btn].get_down_count();
	}

	int get_up_count(Btn btn) const 
	{
		return m_btns[btn].get_up_count(); 
	} 

	bool was_pressed(Btn btn) const
	{
		return m_btns[btn].was_pressed();
	}

	bool was_released(Btn btn) const 
	{
		return m_btns[btn].was_released(); 
	}

	void update_wheel(float wheel);

	
private:
	
	SimpleButton m_mouse_btns[BTN_COUNT]; 
	Cursor m_cursor; 
	float m_wheel; 
};


class GamePad : public InputDevice
{
public: 
	virtual bool presend_event(CoreEvent& evt); 

	void raise_button_event(int idx, bool is_down); 
	void raise_touch_event(InputEventTypes evt_type, 
							uint touch_id, 
							float x, float y);

};


class AndroidController : public GamePad
{
public:
	static const int AndroidCursorsNum = 4;
	
	enum Joy
	{
		BTN_NONE = -1,
		BTN_VOLUP, BTN_VOLDOWN, BTN_CLOSE, BTN_LOCK, 
		BTN_COUNT
	};
	
	AndroidController();
	
private: 
	
	CursorsArray m_ctrl_cursors; 
	ButtonsArray m_ctrl_buttons;
	TouchPad m_tpad;
};


#endif 