/*
例子为普通的销售人员和销售经理销售不同的汽车可以有不同的折扣
普通人员销售普通汽车和Benz一样，没有折扣；而销售经理销售Benz有不同的折扣
*/

class Sales;
class SalesManager;

class IVehicle
{
public:
	virtual double Visit(Sales* sales) = 0;
	virtual double Visit(SalesManager* sm) = 0;
};

// // 普通汽车，折扣为0.03
class Vehicle : public IVehicle
{
public:
	virtual double Visit(Sales* sale)
	{
		return 0.03;
	}
	virtual double Visit(SalesManager* sm)
	{
		return 0.06;
	}
};

//  由于是奔驰特销商，因此可以得到更大的折扣
class Benz : public IVehicle
{
public:
	virtual double Visit(Sales* sales)
	{
		return 0.03;
	}
	virtual double Visit(SalesManager* sm)
	{
		return 0.066;
	}
};

class ISales
{
	virtual double Accept(IVehicle* vehicle) = 0;
};

// 普通的销售人员，只能按照公司规定的折扣进行销售
class Sales : public ISales
{
public:
	double Accept(IVehicle* vehicle)
	{
		return vehicle->Visit(this);
	}
};

// 销售经理，可以针对某些车型提供额外的优惠
class SalesManager : public ISales
{
public:
	virtual double Accept(IVehicle* vehicle)
	{
		return vehicle->Visit(this);
	}
};

void executeVisitor()
{
	Vehicle* vehicle = new Vehicle();
	Benz* benz = new Benz();
	/*Vehicle vehicle = Vehicle();
	Benz benz = Benz();*/
	
	// 向普通销售询问这两辆车的折扣
	Sales* sales = new Sales();
	cout<<"Sales: The rate for common vehicle is: "<< sales->Accept(vehicle)<<endl;
	cout<<"Sales: The rate for benz is: "<< sales->Accept(benz)<<endl;

	SalesManager* salesManager = new SalesManager();
	cout<<"SalesManager: The rate for common vehicle is: "<< salesManager->Accept(vehicle)<<endl;
	cout<<"SalesManager: The rate for benz is: "<< salesManager->Accept(benz)<<endl;
}
