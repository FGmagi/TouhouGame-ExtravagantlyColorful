//============================================================
//	Input 输入
//============================================================
#pragma once
#include <Base.h>
struct Input1;
class Input {
public:
	//确定按键情况
	static void clear();
	//设置监听链表
	static void set(const char*);
	//设置是否监听
	static void active(bool);
	//是否按下（不论是否拦截）
	static bool input(char c);
	//是否按住
	static bool press(char);
	//按下或长按
	static bool repeat(char);
	static bool repeat2(char c);
	//是否长按
	static bool hold(char);
	//是否弹起
	static bool bonunce(char);
	//执行拦截
	static void block(char c);
	friend Base;
private:
	Input();
	~Input();
};
//============================================================
//	宏定义
//============================================================
#define mouse_BUTTON_L 0x01        
#define mouse_BUTTON_R 0x02        
#define mouse_BUTTON_M 0x04        
#define mouse_BUTTON_4 0x05        
#define mouse_BUTTON_5 0x06        
//-----------------------------------------------------------
#define R_BACK 0x08        
#define R_TAB 0x09        
#define R_ENTER 0x0D        
#define R_SHIFT 0x10        
#define R_CTLR 0x11        
#define R_ALT 0x12        
#define R_PAUSE 0x13        
#define R_CAPITAL 0x14        
#define R_ESCAPE 0x1B        
#define R_SPACE 0x20        
#define R_PRIOR 0x21        
#define R_NEXT 0x22        
#define R_END 0x23        
#define R_HOME 0x24        
#define R_LEFT 0x25        
#define R_UP 0x26        
#define R_RIGHT 0x27        
#define R_DOWN 0x28        
#define R_SELECT 0x29        
#define R_PRINT 0x2A        
#define R_SNAPSHOT 0x2C        
#define R_INSERT 0x2D        
#define R_DELETE 0x2E        
//-----------------------------------------------------------
#define R_0 0x30        
#define R_1 0x31        
#define R_2 0x32        
#define R_3 0x33        
#define R_4 0x34        
#define R_5 0x35        
#define R_6 0x36        
#define R_7 0x37        
#define R_8 0x38        
#define R_9 0x39        
//-----------------------------------------------------------
#define R_A 0x41        
#define R_B 0x42        
#define R_C 0x43        
#define R_D 0x44        
#define R_E 0x45        
#define R_F 0x46        
#define R_G 0x47        
#define R_H 0x48        
#define R_I 0x49        
#define R_J 0x4A        
#define R_K 0x4B        
#define R_L 0x4C        
#define R_M 0x4D        
#define R_N 0x4E        
#define R_O 0x4F        
#define R_P 0x50        
#define R_Q 0x51        
#define R_R 0x52        
#define R_S 0x53        
#define R_T 0x54        
#define R_U 0x55        
#define R_V 0x56        
#define R_W 0x57        
#define R_X 0x58        
#define R_Y 0x59        
#define R_Z 0x5A        
//-----------------------------------------------------------
#define R_LWIN 0x5B        
#define R_RWIN 0x5C        
#define R_APPS 0x5D        
//-----------------------------------------------------------
#define R_NUMPAD0 0x60        
#define R_NUMPAD1 0x61        
#define R_NUMPAD2 0x62        
#define R_NUMPAD3 0x63        
#define R_NUMPAD4 0x64        
#define R_NUMPAD5 0x65        
#define R_NUMPAD6 0x66        
#define R_NUMPAD7 0x67        
#define R_NUMPAD8 0x68        
#define R_NUMPAD9 0x69        
#define R_MULTIPLY 0x6A        
#define R_ADD 0x6B        
#define R_SEPARATOR 0x6C        
#define R_SUBTRACT 0x6D        
#define R_DECIMAL 0x6E        
#define R_DIVIDE 0x6F        
//-----------------------------------------------------------
#define R_F1 0x70        
#define R_F2 0x71        
#define R_F3 0x72        
#define R_F4 0x73        
#define R_F5 0x74        
#define R_F6 0x75        
#define R_F7 0x76        
#define R_F8 0x77        
#define R_F9 0x78        
#define R_F10 0x79        
#define R_F11 0x7A        
#define R_F12 0x7B        
//-----------------------------------------------------------
#define R_NUMLOCK 0x90        
#define R_SCROLL 0x91        
//-----------------------------------------------------------
#define R_LSHIFT 0xA0        
#define R_RSHIFT 0xA1        
#define R_LCONTROL 0xA2        
#define R_RCONTROL 0xA3        
#define R_L_ALT 0xA4        
#define R_R_ALT 0xA5        
//-----------------------------------------------------------
#define R_SEP 0xBC        
#define R_DASH 0xBD        
#define R_DOTT 0xBE     

#define PRESS_C (Input::press(R_SPACE) || Input::press(R_ENTER) || Input::press(R_Z))
#define INPUT_C (Input::input(R_SPACE) || Input::input(R_ENTER) || Input::input(R_Z))
#define INPUT2_C (Input::input(R_SPACE) || Input::input(R_ENTER) || Input::input(R_Z))
#define PRESS_B (Input::press(R_ESCAPE) || Input::press(R_X))
#define INPUT_B (Input::input(R_ESCAPE) || Input::input(R_X))
