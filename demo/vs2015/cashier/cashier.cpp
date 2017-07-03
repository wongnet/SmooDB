// cashier.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Database.h"
#include  "Disposition.h"
using namespace std;

void process(unsigned int &tmpID)
{
	
	Disposition dis(tmpID);
	tmpID = dis.nowID;
	string tmp;
	TableGoods goods;
	TableMember member;
	control c;
	unsigned int key;
	string help = "admin:���̹���\nvip:��Ա����\nbuy:������Ʒ\nbuybyid:���ݻ��������Ʒ\npay:֧��\nclear:����\nanalysis:������å\nlog��ʷ�˵�\nbye:�˳�\n>>";
	cout << "��ѡ������Ҫ������ָ��" << endl;
	cout << "����-help����" << endl;
	cout << "�ǻ�Ա��?[y/n]" << endl;
	cin >> tmp;
	if (tmp != "n")
	{
		cout << "�������Ա����:";
		cin >> key;
		if (dis.UpdateStatus(key) == -1)
			cout << "���Ǹ�JJ��Ա!!" << endl;
		else
			cout << "��ӭ:" << dis.member.name << endl;
	}
	else
		dis.discount = 1;
	cout << "--------��----��----��----��--------------\n" << help << endl;

	system("color a");//�ı���ɫ
	while ((cin >> tmp))
	{

		if (tmp == "help")
		{

			cout << help << endl;
		}
		if (tmp == "buy")
		{

			unsigned int id, count;
			cout << "��������ƷID ��������:" << endl;
			if (cin >> id >> count)
			{
				if (dis.shoppingByLocation(id, count) == -1)
					cout << "����ʧ��!" << dis.getGoods(id).name << "ʣ��:" << dis.getGoods(id).left << endl;
				//else
				//cout << "����ɹ�!" << endl;
			}
			else
			{
				cout << "��ʽ����!" << endl;
				cin.clear();
			}
			cout << "admin@buy>>";
		}

		if (tmp == "buybyid")
		{
			cout << "admin@buybyid>>";
			unsigned int id, count;
			cout << "��������ƷID ��������:" << endl;
			if (cin >> id >> count)
			{
				if (dis.shopping(id, count) == -1)
					try
				{
					cout << "����ʧ��!" << dis.getGoods(id).name << "ʣ��:" << dis.getGoods(id).left << endl;
				}
				catch (char*)
				{
					cout << "����ʧ��!" << endl;
				}
				//else
				//cout << "����ɹ�!" << endl;
			}
			else
			{
				cout << "��ʽ����!" << endl;
				cin.clear();
			}

		}
		if (tmp == "clear")
			system("cls");
		if (tmp == "admin")
		{

			cout << "\n1.updateg:��Ʒ��Ϣ�޸�\n2.selectg:��Ʒ��Ϣ��ѯ\n3.updatem:�˿���Ϣ�޸�\n4.selectm:�˿���Ϣ��ѯ\n5.delete -goods id\n6.delete -member id\n7.append -goods\n8.append -member\n9.exit:�˳�" << endl;
			while ((cin >> tmp))
			{

				if (tmp == "1" || tmp == "updateg")
				{
					cout << ">>>��Ʒ��Ϣ�޸�:" << endl;
					cout << ">>>�޸ĸ�ʽ:\n>>>��¼���\t��Ʒ��\t��Ʒ��\t����\tʣ������" << endl;
					if (cin >> key >> goods.ID >> goods.name >> goods.price >> goods.left)
					{
						cout << "���޸ĵ�" << key << "����¼Ϊ:" << goods.ID << " " << goods.name << " " << goods.price << " " << goods.left << endl;
						c.update(key, goods);
					}
					else
					{
						cout << "�޸�ʧ��!" << endl;
						cin.clear();
					}

				}
				else if (tmp == "2" || tmp == "selectg")
				{
					unsigned int from, end;
					cout << "�����¼���λ��:\n��ʼλ��\t����λ��" << endl;
					cin >> from >> end;
					cout << "��¼��� ��Ʒ��\t��Ʒ��\t��Ʒ����\tʣ������" << endl;
					for (unsigned int i = from; i <= end; i++)
					{
						try
						{
							goods = c.selectGoods(i);
							cout << i << "\t" << goods.ID << "\t" << goods.name << "\t" << goods.price << "\t" << goods.left << endl;
						}
						catch (char*) {};
					}
					//��Ʒ��Ϣ��ѯ
				}
				else if (tmp == "3" || tmp == "updatem")
				{
					cout << ">>>�˿���Ϣ�޸�:" << endl;
					cout << ">>>�޸ĸ�ʽ:\n>>>��¼���\t�˿�ID\t����\t�绰\t\t����\t�ȼ�\t����ID" << endl;

					if (cin >> key >> member.ID >> member.name >> member.tel >> member.point >> member.level >> member.SencondaryID)
					{
						cout << "���޸ĵ�" << key << "����¼Ϊ:" << member.ID << " " << member.name << " " << member.tel << " " << member.point << " " << member.level << " " << member.SencondaryID << endl;
						c.update(key, member);
					}
					else
					{
						cout << "�޸�ʧ��!" << endl;
						cin.clear();
					}



					//�˿���Ϣ�޸�
				}
				else if (tmp == "4" || tmp == "selectm")
				{
					//�˿���Ϣ��ѯ
					unsigned int from, end;
					cout << "�����¼���λ��:\n��ʼλ��\t����λ��" << endl;
					cin >> from >> end;
					cout << "��¼��� �˿�ID\t����\t�绰\t\t����\t�ȼ�\t����ID" << endl;
					for (unsigned int i = from; i <= end; i++)
					{
						try
						{
							member = c.selectMember(i);
							cout << i << "\t" << member.ID << "\t" << member.name << "\t" << member.tel << "\t" << member.point << "\t" << member.level << "\t" << member.SencondaryID << endl;
						}
						catch (char*) {};
					}

				}
				else if (tmp == "5" || tmp == "delete -goods")
				{
					//delete -goods id
					if (cin >> key)
					{
						if (c.del(key, true) == -1)
							cout << "ɾ��ʧ��!" << endl;
						else
							cout << "ɾ���ɹ�!" << endl;
					}
					else
					{
						cout << "ɾ��ʧ��" << endl;
						cin.clear();
					}
				}
				else if (tmp == "6" || tmp == "delete -member")
				{
					if (cin >> key)
					{
						if (c.del(key, false) == -1)
							cout << "ɾ��ʧ��!" << endl;
						else
							cout << "ɾ���ɹ�!" << endl;
					}
					else
					{
						cout << "ɾ��ʧ��" << endl;
						cin.clear();
					}
					//delete -member id
				}
				else if (tmp == "7" || tmp == "append -goods")
				{
					cout << ">>>��Ʒ��Ϣ����:" << endl;
					cout << ">>>�޸ĸ�ʽ:\n>>>��Ʒ��\t��Ʒ��\t����\tʣ������" << endl;
					if (cin >> goods.ID >> goods.name >> goods.price >> goods.left)
					{
						cout << "������¼:" << goods.ID << " " << goods.name << " " << goods.price << " " << goods.left << endl;
						c.append(goods);
					}
					else
					{
						cout << "�޸�ʧ��!" << endl;
						cin.clear();
					}
					//append -goods
				}
				else if (tmp == "8" || tmp == "append -member")
				{
					cout << ">>>�����û���¼:" << endl;
					cout << ">>>�޸ĸ�ʽ:\n>>>�˿�ID\t����\t�绰\t\t����\t�ȼ�\t����ID" << endl;

					if (cin >> member.ID >> member.name >> member.tel >> member.point >> member.level >> member.SencondaryID)
					{
						cout << "������¼Ϊ:" << member.ID << " " << member.name << " " << member.tel << " " << member.point << " " << member.level << " " << member.SencondaryID << endl;
						c.append(member);
					}
					else
					{
						cout << "�޸�ʧ��!" << endl;
						cin.clear();
					}
					//append -member
				}
				else if (tmp == "9" || tmp == "exit")
				{
					break;
				}
				cout << "admin@admin>>";
			}
		}
		if (tmp == "bye")
			return;
		if (tmp == "pay")
		{
			cout << dis.PrintBill() << endl;
			cout << "΢��֧��?֧����?�����ֽ�?���ǲ�����?[n]" << endl;
			if (cin >> tmp)
			{
				if (tmp != "n")
				{
					dis.AppendRecord();//��Ʒ��������,����
					if (dis.updateStatus() != "")
						return;
					
				}
				else
				{
					cout << "лл�ݹ�!" << endl;
					return;
					
				}
			}
			else
			{
				cout << "֧��ʧ��!" << endl;
				return;
			}
			
		}


		if (tmp == "vip")
		{

			cout << "�������Ա����:";
			cin >> key;
			if (dis.UpdateStatus(key) == -1)
				cout << "���Ǹ�JJ��Ա!!" << endl;
			else
				cout << "��ӭ:" << dis.member.name << endl;

		}
		cout << "admin@root>>";
	}

	//dis.shopping();
}
int _tmain(int argc, _TCHAR* argv[])
{
	unsigned int tmpID = 0;

	system("title �������й������ϵͳ ver 1.0   Designed by TK.Wong");
	system("mode con cols=100 lines=80");//�ı���
	ifstream logo = ifstream("name.txt"); char buf[200];
	while (logo.getline(buf, 200))
	{
		cout << "\t\t" << buf << endl;
	}
	logo.close();
	string user,pass;
	//for (int i = 0; i < 2; i++)
		cout << "\n" << endl;
	cout << "\t\t\t\t\t���������Ա�˺�:";
	cin >> user;
	cout << "\t\t\t\t\t����������:" ;
	cin >> pass;
	if (user != "admin"||pass != "admin")
	{
		cout << "\t\t\t�������!"<<endl;
		system("pause");
		exit(1);
	}

	system("cls");
	string isContinue;
	while (1)
	{
		process(tmpID);
		cout << "������?[y]:" ;
		cin >> isContinue;
		if (isContinue == "y")
			break;
		system("cls");

	}
	
	cout << "������ر�ϵͳ!" << endl;
	system("pause>>nul");
	return 0;
}

