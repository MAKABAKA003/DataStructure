#ifndef QUEUESYSTEM_H
#define QUEUESYSTEM_H
#include"Event.h"
#include"ServiceWindows.h"
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
		while (current_event->occur_time<=total_service_time && current_event->occur_time>=0)
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
		this->windows = new ServiceWindows[windows_num];
		for (int i = 0; i < total_customer_num; ++i)
		{
			Event* event = new Event(unifrand(0, total_service_time));
			event_list.push_back(*event);
		}
		event_list=Eorder(event_list);
		current_event = new Event(exprand(0, total_service_time));
		*current_event = event_list.front();
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
		Customer* customer = new Customer(exprand(1, max_service_time), current_event->occur_time);
		if (!customer) exit(-1);
		customer_list.push_back(*customer);
		Corder(customer_list);

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
			Event* departureEvent = new Event(current_event->occur_time + customer->service_time, Idlewindow);
			event_list.push_back(*departureEvent);
			event_list=Eorder(event_list);
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
				event_list=Eorder(event_list);
				delete customer;
			}
			// ����˿Ͷ�����û��,�򽫴˴�����Ϊ����
			else
				windows[current_event->event_type].setIdle();
		}
	}
	// ���¼����н�������,�ȷ�������ǰ
	deque<Event> Eorder(deque<Event> q)
	{	
		for (int i = 0; i <= q.size(); i++)
		{
			for (int j = i + 1; j <= q.size() - 1; j++)
			{
				if (q[i].occur_time > q[j].occur_time)
				{
					swap(q[i], q[j]);
				}

			}
		}
		return q;
	}
	// �Թ˿Ͷ��н�������,���ȼ��ߵ���ǰ
	deque<Customer> Corder(deque<Customer> q)
	{
		for (int i = 0; i <= q.size(); i++)
		{
			for (int j = i + 1; j <= q.size() - 1; j++)
			{
				if (q[i].vip > q[j].vip)
				{
					swap(q[i], q[j]);
				}
				if (q[i].vip == q[j].vip)
				{
					if (q[i].arrive_time < q[j].arrive_time)
						swap(q[i], q[j]);
				}
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

	int serviced_customer_num=0;	// �Ѿ�������Ĺ˿���
	double avr_wait_time;		// ƽ���ȴ�ʱ�� 
	int max_wait_num = 0;		// ���ȴ�����
	int no_service_num;			// ��������ر�ʱ��δ�����������
};
#endif // !QUEUESYSTEM_H
