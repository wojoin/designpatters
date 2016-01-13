#include <iostream>
using namespace std;

#include "observer.h"


int _tmain(int argc, _TCHAR* argv[])
{
	executeObserver();	
	

	char tempbuf[128];
	_strtime(tempbuf);
	cout<<tempbuf<<endl;

	cout <<"666"<<endl;
	cin.get();
	return 0;
}
