#ifndef QUEUESYSTEM_H
#define QUEUESYSTEM_H
#include"Event.h"
#include"ServiceWindows.h"
#include"order.h"
#include<deque>
#include<algorithm>

using namespace std;

class QueueSystem
{
	
public:
	QueueSystem(int total_service_time, int total_customer_num, int windows_num, int max_service_time)
		:total_service_time(total_service_time), total_customer_num(total_customer_num), windows_num(windows_num), max_service_time(max_service_time)
	{}
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
		return avr_wait_time/serviced_customer_num;
	}
	int get_max_wait_num()
	{
		return max_wait_num;
	}
	int get_no_service_num()
	{
		return no_service_num;
	}
	int get_serviced_customer_num()
	{
		return serviced_customer_num;
	}
private:
	void run()
	{
		this->init();
		while (event_list.size())
		{
			if (current_event->event_type == -1)
				customerArrived();
			if (current_event->event_type >= 0)
				customerDeparture();
			*current_event = event_list.front();
			event_list.pop_front();
		}
		no_service_num = customer_list.size();

		this->makeEmpty();
	}
	// ��ʼ��
	void init()
	{
		double lambda = (double)total_customer_num / (double)total_service_time;
		this->windows = new ServiceWindows[windows_num];
		while (event_list.size() < total_customer_num)
		{
			if (event_list.size() == 0)
			{
				Event* event = new Event(exprand(lambda));
				event_list.push_back(*event);
			}
			Event* event = new Event(event_list.back().occur_time + exprand(lambda));
			event_list.push_back(*event);
		}
		mergesort(event_list);
		current_event = new Event(exprand(lambda));
		*current_event = event_list.front();
		avr_wait_time = 0;
		serviced_customer_num = 0;
		max_wait_num = 0;
		no_service_num = 0;
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
		// ���˿ͼ���˿Ͷ�����
		Customer* customer = new Customer(unifrand(1, max_service_time), current_event->occur_time);
		if (!customer) exit(-1);
		// ����˿͵���ʱ��Ӵ���ʱ������ܷ���ʱ��,��˿�ֱ���뿪
		if (customer->arrive_time + customer->service_time < total_service_time)
		{
			customer_list.push_back(*customer);
			if (customer_list.size() > max_wait_num) max_wait_num++;
			mergesort(customer_list);
			// ����п��д��ھͽ��˿��͵�����
			int Idlewindow = getIdleWindow();
			if (Idlewindow >= 0)
			{
				serviced_customer_num++; // �ѱ�����Ĺ˿���+1
				windows[Idlewindow].ServiceCustomer(customer_list.front());
				customer_list.pop_front();
				//event_list.pop();
				windows[Idlewindow].setBusy();

				// ���ù˿͵��뿪�¼������¼�������,�뿪ʱ��Ϊ����ʱ��ӷ���ʱ��
				Event* DepreEvent = new Event(current_event->occur_time + customer->service_time, Idlewindow);
				event_list.push_back(*DepreEvent);
				mergesort(event_list);
			}
			
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
				serviced_customer_num++;
				Customer* customer=new Customer();
				*customer = customer_list.front();
				customer_list.pop_front();
				windows[current_event->event_type].ServiceCustomer(*customer);
				Event tmpEvent(current_event->occur_time + customer->service_time, current_event->event_type);
				event_list.push_back(tmpEvent);
				mergesort(event_list);
				delete customer;
			}
			// ����˿Ͷ�����û��,�򽫴˴�����Ϊ����
			else
				windows[current_event->event_type].setIdle();
		}
	}
	int windows_num;		// ��������K
	int total_service_time; // �ܷ���ʱ��T
	int total_customer_num; // �ܹ˿���C
	int max_service_time;   //�����ʱ��L

	ServiceWindows* windows;
	deque<Customer> customer_list;
	deque<Event> event_list;
	Event* current_event;

	int serviced_customer_num=0;	// �Ѿ�������Ĺ˿���
	double avr_wait_time;		// ƽ���ȴ�ʱ�� 
	int max_wait_num = 0;		// ���ȴ�����
	int no_service_num;			// ��������ر�ʱ��δ�����������
};
#endif // !QUEUESYSTEM_H