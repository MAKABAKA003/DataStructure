#ifndef EVENT_H
#define EVENT_H

#include"random.h"
#define lamda 0.1
struct Event
{
	int occur_time;// �¼�����ʱ��
	int event_type;// �¼�����,-1��ʾ�����¼�,>=0��ʾ�뿪�¼�
	Event* next;
	Event(int occ ,int event_Type=-1):occur_time(occ),event_type(event_Type),next(nullptr){}
};
#endif // !EVENT_H