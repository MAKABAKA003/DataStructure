#ifndef QUEUESYSTEM_H
#define QUEUESYSTEM_H
#include"Event.h"
#include"ServiceWindows.h"
#include<deque>
using namespace std;

class QueueSystem
{
public:
	QueueSystem(int total_service_time, int total_customer_num,int windows_num,int max_service_time) 
				:total_service_time(total_service_time), total_customer_num(total_customer_num),windows_num(windows_num),max_service_time(max_service_time)
	{
		init();
	}
	~QueueSystem()
	{
		delete[] windows;
	}
	void simulate()
	{
		run();
	}

	// �ṩ�ⲿ�ӿ����ڻ�ȡ���
	double get_avr_wait_time()
	{
		return avr_wait_time;
	}
	int get_max_wait_num()
	{
		return max_wait_num;
	}
	int get_no_service_num()
	{
		return no_service_num;
	}

private:
	void run()
	{	
		this->init();
		while (current_event)
		{
			if (current_event->event_type == -1)
				customerArrived();
			if (current_event->event_type >= 0)
				customerDeparture();
			*current_event = event_list.front();
			event_list.pop_front();
		}
		this->makeEmpty();
	}
	// ��ʼ��
	void init()
	{	
		this->windows = new ServiceWindows[windows_num];
		Event* event=new Event(exprand(0, total_service_time));
		current_event = event;
	}
	// ������ж��в��������д���Ϊ����״̬
	void makeEmpty()
	{
		for (int i = 0; i < windows_num; ++i)
			windows[i].setIdle();
		while (!customer_list.empty()) customer_list.pop_front();
		while (!event_list.empty()) event_list.pop_front();
	}
	// ��ȡ���д�������
	int getIdleWindow()
	{
		for (int i = 0; i < windows_num; ++i)
		{
			if (windows[i].isIdle())
				return i;
		}
		return -1;
	}
	// ����˿͵����¼�
	void customerArrived()
	{	
		// ������һ���˿͵ĵ����¼�
		Event* tmpEvent = new Event(exprand(current_event->occur_time, total_service_time));
		event_list.push_back(*tmpEvent);

		// ���˿ͼ���˿Ͷ�����
		Customer* customer = new Customer(exprand(1,max_service_time),current_event->occur_time);
		if (!customer) exit(-1);
		customer_list.push_back(*customer);

		// ����п��д��ھͽ��˿��͵�����
		int Idlewindow = getIdleWindow();
		if (Idlewindow >= 0)
		{	
			windows[Idlewindow].ServiceCustomer(customer_list.front());
			customer_list.pop_front();
			//event_list.pop();
			windows[Idlewindow].setBusy();

			// ���ù˿͵��뿪�¼������¼�������,�뿪ʱ��Ϊ����ʱ��ӷ���ʱ��
			Event* departureEvent = new Event(current_event->occur_time + customer->service_time, Idlewindow);
			event_list.push_back(*departureEvent);
			event_list=order(event_list);
		}
		delete customer;
	}
	// ����˿��뿪�¼�
	void customerDeparture()
	{
		if (current_event->occur_time < total_service_time)
		{
			// �ȴ�ʱ��=�뿪�¼��ķ���ʱ��-�˿͵���ʱ��-�˿ͷ���ʱ��
			avr_wait_time += current_event->occur_time - windows[current_event->event_type].getCustomerArriveTime() - windows[current_event->event_type].getCustomerServiceTime();
			// ����˿Ͷ����л�����,����ŵ�������
			if (!customer_list.empty())
			{
				Customer* customer=new Customer();
				*customer= customer_list.front();
				windows[current_event->event_type].ServiceCustomer(*customer);
				Event tmpEvent(current_event->occur_time + customer->service_time, current_event->event_type);
				event_list.push_back(tmpEvent);
				event_list = order(event_list);
				delete customer;
			}
			// ����˿Ͷ�����û��,�򽫴˴�����Ϊ����
			else
				windows[current_event->event_type].setIdle();
		}
	}
	// ���¼����н�������,�ȷ�������ǰ
	deque<Event> order(deque<Event> q)
	{
		for (int i = 1; i < q.size(); ++i)
		{
			if (q[i].occur_time < q[i-1].occur_time)
			{
				swap(q[i], q[i - 1]);
			}
		}
		return q;
	}

	int windows_num;		// ��������K
	int total_service_time; // �ܷ���ʱ��T
	int total_customer_num; // �ܹ˿���C
	int max_service_time;   //�����ʱ��L

	ServiceWindows* windows;
	deque<Customer> customer_list;
	deque<Event> event_list;
	Event* current_event;

	int serviced_customer_num;	// �Ѿ�������Ĺ˿���
	double avr_wait_time;		// ƽ���ȴ�ʱ�� 
	int max_wait_num;			// ���ȴ�����
	int no_service_num;			// ��������ر�ʱ��δ�����������
};
#endif // !QUEUESYSTEM_H
