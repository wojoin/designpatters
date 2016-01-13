/*
例子使用Observer模式使时间不停地更新
Subject:ClockTimer--记录当前的时间
Observer:DigitalClock,AnalogClock为两种不同的观察者,显示时间的方式不同。观察者根据通知的时间来更新自己显示的时间
*/

#include <vector>
#include <time.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <string.h>
#include <windows.h>


class Observer;

class Subject
{
public:
	Subject() {};
	virtual ~Subject() 
	{
		int count = observers.size();
		for (int i = 0;i < count;i++)
		{
			observers[i] = NULL;
			delete observers[i];
		}
	};
public:
	virtual void Attach(Observer* o)
	{
		observers.push_back(o);
	}

	virtual void Detach(Observer* o)
	{
		int count = observers.size();
		int i;
		for(i = 0;i < count;i++)
		{
			if (o == observers[i])
				break;
		}
		if (i<count)
		{
			observers.erase(observers.begin() + i);
		}

	}
	virtual void notify() = 0;
protected:
	vector<Observer*> observers;
};

class Observer
{
public:
	virtual void Update(Subject* changedSubject) = 0;
};


// 时间Subject
class ClockTimer : public Subject
{
public:
	ClockTimer()
	{
		_strtime(buffer);
	}
	virtual ~ClockTimer()
	{

	}
public:
	int GetHour()
	{
		char timebuf[128];
		strncpy(timebuf,buffer,2);
		timebuf[2] = NULL;
		m_nHour = atoi(timebuf);
		return m_nHour;
	}
	int GetMinite()
	{
		char timebuf[128];
		strncpy(timebuf,buffer+3,2);
		timebuf[2] = NULL;
		m_nMinite = atoi(timebuf);
		return m_nMinite;
	}
	int GetSecond()
	{
		char timebuf[128];
		strncpy(timebuf,buffer+6,2);
		timebuf[2] = NULL;
		m_nSecond = atoi(timebuf);
		return m_nSecond;
	}
	void RefrushTime()
	{
		_strtime(buffer);
		GetHour();
		GetMinite();
		GetSecond();
	}
public:
	virtual void notify()
	{
		int count = observers.size();
		for (int i = 0;i < count;i++)
		{
			observers[i]->Update(this);
		}
	}

	void Tick()
	{
		_tzset();
		Sleep(500);
		RefrushTime();
		notify();
	}
private:
	char buffer[128];
	int m_nHour;
	int m_nMinite;
	int m_nSecond;
};



// 数字时钟--First Observer
class DigitalClock : public Observer
{
public:
	DigitalClock(ClockTimer* timeSubject)
	{
		this->subject = timeSubject;
		this->subject->Attach(this);// 注册该Observer
	}
	~DigitalClock()
	{
		subject->Detach(this);
	}
public:
	void Update(Subject* changedSubject)
	{
		// 如果显示的值与通知来的值不一样是更新
		// 如果参数传来的Subject与Observer维护的当前Subject不一致时进行更新
		ClockTimer* pSubject = dynamic_cast<ClockTimer*>(changedSubject);
		if (pSubject != NULL)
		{
			if (pSubject->GetSecond() != m_DisplaySecond)
			{
				subject = pSubject;
				Draw();
			}
		}
		/*if (changedSubject == subject)
		{
			Draw();
		}*/
		
	}
	void Draw()
	{
		m_DisplayHour = subject->GetHour();
		m_DisplayMinite = subject->GetMinite();
		m_DisplaySecond = subject->GetSecond();

		cout<<"Digital time is:\t"<< m_DisplayHour<<":"<<m_DisplayMinite
			<< ":"<<m_DisplaySecond<<endl;
	}
private:
	ClockTimer* subject; // 存放具体的时间数据Subject
	int m_DisplayHour;
	int m_DisplayMinite;
	int m_DisplaySecond;
};

// 模拟时钟-Second Observer
class AnalogClock : public Observer
{
public:
	AnalogClock(ClockTimer* timeSubject)
	{
		this->subject = timeSubject;
		this->subject->Attach(this);
	}
	~AnalogClock()
	{
		subject->Detach(this);
	}
public:
	void Update(Subject* changedSubject)
	{
		ClockTimer* pSubject = dynamic_cast<ClockTimer*>(changedSubject);
		if (pSubject != NULL)
		{
			if (pSubject->GetSecond() != m_DisplaySecond)
			{
				subject = pSubject;
				Draw();
			}
		}
	}
	void Draw()
	{
		m_DisplayHour = subject->GetHour();
		m_DisplayMinite = subject->GetMinite();
		m_DisplaySecond = subject->GetSecond();

		cout<<"Analog  time is:\t"<< m_DisplayHour<<":"<<m_DisplayMinite
			<< ":"<<m_DisplaySecond<<endl;
	}
private:
	ClockTimer* subject;
	int m_DisplayHour;
	int m_DisplayMinite;
	int m_DisplaySecond;
};

void executeObserver()
{
	// Subject
	ClockTimer* timer = new ClockTimer();

	// Observer
	DigitalClock* digitalClock = new DigitalClock(timer);
	AnalogClock* analogClock = new AnalogClock(timer);

	// notify
	timer->Tick();

	cout<<"notificiations are called persecond.The summary is 10."<<endl;

	// 一下为一秒钟通知一次
	int index = 0;
	while (index < 10)
	{
		timer->Tick();
		index++;
	}	
}
