#include "stdafx.h"
#include "Disposition.h"

Disposition::Disposition(unsigned int RunningNum)
{
	d_goods = new Database<TableGoods>("goods", ".\\DATA");
	d_member = new Database<TableMember>("member", ".\\DATA");
	if (RunningNum == 0 && d_member->GetFileEnd() != 0)
		nowID = d_member->GetFileEnd() ;
	else
		nowID = RunningNum+1;
	cout << "��ӭ��ˮ��Ϊ:" << nowID << "�Ĺ˿Ϳ�������֮��!" << endl;

}

Disposition::~Disposition()
{
	delete d_goods;
	delete d_member;

}

void Disposition::payment()
{

}

string Disposition::PrintBill()
{
	string ret="";
	vector<TableBill>::iterator it;
	 char str[25];
	for (it = bill.begin(); it != bill.end(); it++)
	{
		//���������
		
		snprintf(str, 25, "%d", (*it).goodsID);
		ret += str;
		ret += "\t";
		snprintf(str, 25, "%s", (*it).goods);
		ret += str;
		ret += "\t";
		snprintf(str, 25, "%.2f", (*it).price);
		//cout << (*it).price <<"->price"<< endl;
		ret += str;
		ret += "\t";
		snprintf(str, 25, "%.2f", (*it).discount);
		ret += str;
		ret += "\t";
		snprintf(str, 25, "%d", (*it).count);
		ret += str;
		ret += "\t";
		snprintf(str, 25, "%.2f", (*it).count*(*it).price*(*it).discount);
		TotalCost += (*it).count*(*it).price*(*it).discount;
		ret += str;
		ret += "\t";
		snprintf(str, 25, "%s", (*it).time);
		ret += str;
		//cout << ret << endl;
		ret += "\n";
	}
	snprintf(str, 25, "%.2f", TotalCost);
	ret += "\t\t\tTotal cost:";
	ret += str;
	
	return ret;
}

void Disposition::AppendRecord()
{
	vector<unsigned int*>::iterator p;
	TableGoods goods;
	for (p = queue.begin(); p != queue.end(); p++)
	{
		
		goods = d_goods->select(**p);
		goods.left = *(*p + 1);
		d_goods->update(**p, goods);
		cout << "goods key:" << **p << "left:" << goods.left << endl;
		delete *p;
		
	}
	//�޸�ʣ����Ʒ����
	vector<TableBill>::iterator it; char buf[15];
	//member.
	
	snprintf(buf, 15, "%d", member.ID);//Cת��������ô�Ѿ�
	d_bill = new Database<TableBill>(buf, ".\\DATA\\BILL");
	for (it = bill.begin(); it != bill.end(); it++)
	{
		d_bill->append(*it);
	}
	delete d_bill;
}

int Disposition::shopping(unsigned int id,unsigned int count)
{
	int ret = -1;
	try
	{
		TableGoods goods;
		for (unsigned int i = 0; i < d_goods->GetEndKey(); i++)
		{
			goods = d_goods->select(i);
			cout << "goods id:" << goods.ID << endl;
			if (goods.ID == id)
			{
				if (count <= goods.left)
				{
					ret = 1;//����ɹ�
					goods.left -= count;
					//d_goods->update(i, goods);
					unsigned int *tmp = new unsigned int[2];
					tmp[0] = i;
					tmp[1] = goods.left;
					queue.push_back(tmp);
					TableBill tmpbill;
					tmpbill.count = count;
					tmpbill.discount = discount;
					//cout << discount << "->discount" << endl;
					try
					{
						tmpbill.price = getGoods(i).price;
						time_t t = time(NULL);
						strcpy(tmpbill.time, ctime(&t));
						cout << tmpbill.time << endl;
						tmpbill.goodsID = getGoods(i).ID;
						strcpy(tmpbill.goods, getGoods(i).name);
						bill.push_back(tmpbill);
					}
					catch (char*)
					{
						return -1;//����ʧ��
					}
				}
				break;
			}
		}
	}
	catch (char*)
	{
		return -1;//����ʧ��
	}
	return ret;
}

int Disposition::shoppingByLocation(unsigned int key, unsigned int count)
{
	int ret = -1;
	try
	{
				TableGoods goods;
				goods = d_goods->select(key);
				if (count <= goods.left)
				{
					ret = 1;//����ɹ�
					goods.left -= count;
					//d_goods->update(key, goods);
					unsigned int *tmp = new unsigned int[2];
					tmp[0] = key;
					tmp[1] = goods.left;
					queue.push_back(tmp);
					TableBill tmpbill;
					tmpbill.count = count;
					tmpbill.discount = discount;
					//cout << discount << "->discount" << endl;
					try
					{
						tmpbill.price = getGoods(key).price;
						time_t t = time(NULL);
						strcpy(tmpbill.time, ctime(&t));
						cout << "����ɹ�!ʱ��:"<<tmpbill.time << endl;
						tmpbill.goodsID = getGoods(key).ID;
						strcpy(tmpbill.goods, getGoods(key).name);
						bill.push_back(tmpbill);
					}
					catch (char*)
					{
						delete tmp;
						return -1;//����ʧ��
					}
				}
	}
	catch (char*)
	{
		//delete tmp;
		return -1;//����ʧ��
	}

	return ret;
}

TableGoods Disposition::getGoods(unsigned int key)
{
	//����Ĳ������ڹ�ϣ��-�����е�λ��
	try 
	{
		return d_goods->select(key);
	}
	catch(char *a)
	{ throw a; }
}

char Disposition::UpdateStatus(unsigned int id)
{
	char ret = -1; unsigned int i = 0;
	try
	{
		//TableMember member;
		for (; i < d_member->GetEndKey(); i++)
		{
			//cout << d_member->GetEndKey() << endl;
			member = d_member->select(i);
			if (member.ID == id)
			{
				nowID = id;
				ret = member.level;//-1 û�м�¼��1 ��ͨvip��2 �߼�vip
				break;
			}
		}
		
	}
	catch(char* a)
	{
		
		discount = 1;
		return -1;
	}
	//cout << ret <<"->ret"<< endl;
	//printf("ret:::%c %d", ret, ret);
	switch (ret)
	{

	case '1':
		discount = 0.98f;
		SID = i;
		break;
	case '2':
		discount = 0.95f;
		SID = i;
		break;
	default:
		SID = 0;
		discount = 1;
		break;
	}
	return ret;
}

string Disposition::updateStatus()
{
	if (TotalCost >= 1000 && discount == 1)
		return "�������ͨ��Ա�ʸ����ڹ�Ա��ϵ����������Ϊ����";
	if (TotalCost >= 2000 && discount == 1)
		return "����ø߼���Ա�ʸ����ڹ�Ա��ϵ����������Ϊ����";
	if (TotalCost >= 1000 && discount == 0.98f)
	{
		TableMember m;
		m = d_member->select(SID);
		m.level = 2;
		d_member->update(SID, m);
		return "��������Ϊ�߼���Ա!";
	}
	return "";
}
