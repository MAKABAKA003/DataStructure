#ifndef EVENT_H
#define EVENT_H

#include"random.h"

struct Event
{
	int occur_time;// �¼�����ʱ��
	int event_type;// �¼�����,-1��ʾ�����¼�,>=0��ʾ�뿪�¼�
	Event(int occ=0 ,int event_Type=-1):occur_time(occ),event_type(event_Type){}
};
#endif // !EVENT_H