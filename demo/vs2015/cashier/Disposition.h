#pragma once
#include "Database.h"
#include <string>
#include <vector>

using namespace std;
typedef struct
{
	unsigned int ID;
	char name[10];
	double price;
	unsigned int left;
}TableGoods;

typedef struct
{
	unsigned int ID;
	char name[10];
	char tel[15];
	unsigned int point;
	char level;//等级普通会员，高级会员
	unsigned int bill;
	unsigned int SencondaryID;

}TableMember;

typedef struct 
{
	//unsigned int NUM;
	char goods[10];
	unsigned int goodsID;
	float price;
	float discount;
	unsigned int count;
	char time[25];
}TableBill;

class Disposition
{
public:
	Disposition(unsigned int RunningNum);
	~Disposition();
	//unsigned int TotalCost;
	//string search(unsigned int MemberID);
	void payment();
	string PrintBill();
	void AppendRecord();
	unsigned int nowID;
	int shopping(unsigned int,unsigned int);
	int shoppingByLocation(unsigned int, unsigned int);
	TableGoods getGoods(unsigned int);
	char UpdateStatus(unsigned int id);
	float discount;
	unsigned int SID;//记录序号，会员的序号
	string updateStatus();
	TableMember member;
private:
	//TableGoods goods;
	
	vector<TableBill> bill;
	vector<unsigned int*> queue;//商品数目减少容器
	Database<TableGoods> *d_goods;
	Database<TableMember> *d_member;
	Database<TableBill> *d_bill;
	float TotalCost = 0;
	
};


class control 
{
public:
	control();
	~control();
	 void append(TableGoods t);
	 void append( TableMember t);
	 void update(unsigned int key, TableGoods t);
	 void update(unsigned int key, TableMember t);
	 int del(unsigned int key,bool isGoods);
	 TableGoods selectGoods(unsigned int key);
	 TableMember selectMember(unsigned int key);
private:
	 TableGoods goods;
	 TableMember member;
	 TableBill bill;
	 Database<TableGoods> *d_goods;
	 Database<TableMember> *d_member;
	 Database<TableBill> *d_bill;
	
};

inline  control::control()
{
	d_goods = new Database<TableGoods>("goods", ".\\DATA");
	d_member = new Database<TableMember>("member", ".\\DATA");

}

inline control::~control()
{
	delete d_goods;
	delete d_member;
	//delete d_bill;
}

//template<class T>
inline void control::append( TableGoods t)
{
	d_goods->append(t);
}

inline void control::append( TableMember t)
{
	d_member->append(t);
}

inline void control::update(unsigned int key, TableGoods t)
{
	if (d_goods->update(key, t) == -1)
		d_goods->append(t);

}

inline void control::update(unsigned int key, TableMember t)
{

	if (d_member->update(key, t) == -1)
		d_member->append(t);
}

inline int control::del(unsigned int key, bool isGoods)
{
	if (isGoods)
	{
		return d_goods->del(key);
	}
	else
	{
		return d_member->del(key);
	}
}

inline TableGoods control::selectGoods(unsigned int key)
{
	try
	{ return d_goods->select(key);}
	catch(char* a)
	{
		throw a;
	}
	
}

inline TableMember control::selectMember(unsigned int key)
{
	try
	{
		return d_member->select(key);
	}
	catch (char* a)
	{
		throw a;
	}
}

