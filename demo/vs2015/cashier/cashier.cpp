// cashier.cpp : 定义控制台应用程序的入口点。
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
	string help = "admin:店铺管理\nvip:会员购物\nbuy:购买商品\nbuybyid:根据货物号买商品\npay:支付\nclear:清屏\nanalysis:数据流氓\nlog历史账单\nbye:退出\n>>";
	cout << "请选择您需要操作的指令" << endl;
	cout << "输入-help帮助" << endl;
	cout << "是会员吗?[y/n]" << endl;
	cin >> tmp;
	if (tmp != "n")
	{
		cout << "请输入会员卡号:";
		cin >> key;
		if (dis.UpdateStatus(key) == -1)
			cout << "你是个JJ会员!!" << endl;
		else
			cout << "欢迎:" << dis.member.name << endl;
	}
	else
		dis.discount = 1;
	cout << "--------帮----助----菜----单--------------\n" << help << endl;

	system("color a");//改变颜色
	while ((cin >> tmp))
	{

		if (tmp == "help")
		{

			cout << help << endl;
		}
		if (tmp == "buy")
		{

			unsigned int id, count;
			cout << "请输入商品ID 购买数量:" << endl;
			if (cin >> id >> count)
			{
				if (dis.shoppingByLocation(id, count) == -1)
					cout << "购买失败!" << dis.getGoods(id).name << "剩余:" << dis.getGoods(id).left << endl;
				//else
				//cout << "购买成功!" << endl;
			}
			else
			{
				cout << "格式错误!" << endl;
				cin.clear();
			}
			cout << "admin@buy>>";
		}

		if (tmp == "buybyid")
		{
			cout << "admin@buybyid>>";
			unsigned int id, count;
			cout << "请输入商品ID 购买数量:" << endl;
			if (cin >> id >> count)
			{
				if (dis.shopping(id, count) == -1)
					try
				{
					cout << "购买失败!" << dis.getGoods(id).name << "剩余:" << dis.getGoods(id).left << endl;
				}
				catch (char*)
				{
					cout << "购买失败!" << endl;
				}
				//else
				//cout << "购买成功!" << endl;
			}
			else
			{
				cout << "格式错误!" << endl;
				cin.clear();
			}

		}
		if (tmp == "clear")
			system("cls");
		if (tmp == "admin")
		{

			cout << "\n1.updateg:商品信息修改\n2.selectg:商品信息查询\n3.updatem:顾客信息修改\n4.selectm:顾客信息查询\n5.delete -goods id\n6.delete -member id\n7.append -goods\n8.append -member\n9.exit:退出" << endl;
			while ((cin >> tmp))
			{

				if (tmp == "1" || tmp == "updateg")
				{
					cout << ">>>商品信息修改:" << endl;
					cout << ">>>修改格式:\n>>>记录序号\t商品号\t商品名\t单价\t剩余数量" << endl;
					if (cin >> key >> goods.ID >> goods.name >> goods.price >> goods.left)
					{
						cout << "您修改第" << key << "条记录为:" << goods.ID << " " << goods.name << " " << goods.price << " " << goods.left << endl;
						c.update(key, goods);
					}
					else
					{
						cout << "修改失败!" << endl;
						cin.clear();
					}

				}
				else if (tmp == "2" || tmp == "selectg")
				{
					unsigned int from, end;
					cout << "输入记录序号位置:\n起始位置\t结束位置" << endl;
					cin >> from >> end;
					cout << "记录序号 商品号\t商品名\t商品单价\t剩余数量" << endl;
					for (unsigned int i = from; i <= end; i++)
					{
						try
						{
							goods = c.selectGoods(i);
							cout << i << "\t" << goods.ID << "\t" << goods.name << "\t" << goods.price << "\t" << goods.left << endl;
						}
						catch (char*) {};
					}
					//商品信息查询
				}
				else if (tmp == "3" || tmp == "updatem")
				{
					cout << ">>>顾客信息修改:" << endl;
					cout << ">>>修改格式:\n>>>记录序号\t顾客ID\t姓名\t电话\t\t积分\t等级\t副卡ID" << endl;

					if (cin >> key >> member.ID >> member.name >> member.tel >> member.point >> member.level >> member.SencondaryID)
					{
						cout << "您修改第" << key << "条记录为:" << member.ID << " " << member.name << " " << member.tel << " " << member.point << " " << member.level << " " << member.SencondaryID << endl;
						c.update(key, member);
					}
					else
					{
						cout << "修改失败!" << endl;
						cin.clear();
					}



					//顾客信息修改
				}
				else if (tmp == "4" || tmp == "selectm")
				{
					//顾客信息查询
					unsigned int from, end;
					cout << "输入记录序号位置:\n起始位置\t结束位置" << endl;
					cin >> from >> end;
					cout << "记录序号 顾客ID\t姓名\t电话\t\t积分\t等级\t副卡ID" << endl;
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
							cout << "删除失败!" << endl;
						else
							cout << "删除成功!" << endl;
					}
					else
					{
						cout << "删除失败" << endl;
						cin.clear();
					}
				}
				else if (tmp == "6" || tmp == "delete -member")
				{
					if (cin >> key)
					{
						if (c.del(key, false) == -1)
							cout << "删除失败!" << endl;
						else
							cout << "删除成功!" << endl;
					}
					else
					{
						cout << "删除失败" << endl;
						cin.clear();
					}
					//delete -member id
				}
				else if (tmp == "7" || tmp == "append -goods")
				{
					cout << ">>>商品信息增加:" << endl;
					cout << ">>>修改格式:\n>>>商品号\t商品名\t单价\t剩余数量" << endl;
					if (cin >> goods.ID >> goods.name >> goods.price >> goods.left)
					{
						cout << "新增记录:" << goods.ID << " " << goods.name << " " << goods.price << " " << goods.left << endl;
						c.append(goods);
					}
					else
					{
						cout << "修改失败!" << endl;
						cin.clear();
					}
					//append -goods
				}
				else if (tmp == "8" || tmp == "append -member")
				{
					cout << ">>>增加用户记录:" << endl;
					cout << ">>>修改格式:\n>>>顾客ID\t姓名\t电话\t\t积分\t等级\t副卡ID" << endl;

					if (cin >> member.ID >> member.name >> member.tel >> member.point >> member.level >> member.SencondaryID)
					{
						cout << "新增记录为:" << member.ID << " " << member.name << " " << member.tel << " " << member.point << " " << member.level << " " << member.SencondaryID << endl;
						c.append(member);
					}
					else
					{
						cout << "修改失败!" << endl;
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
			cout << "微信支付?支付宝?还是现金?还是不买了?[n]" << endl;
			if (cin >> tmp)
			{
				if (tmp != "n")
				{
					dis.AppendRecord();//商品数量减少,记账
					if (dis.updateStatus() != "")
						return;
					
				}
				else
				{
					cout << "谢谢惠顾!" << endl;
					return;
					
				}
			}
			else
			{
				cout << "支付失败!" << endl;
				return;
			}
			
		}


		if (tmp == "vip")
		{

			cout << "请输入会员卡号:";
			cin >> key;
			if (dis.UpdateStatus(key) == -1)
				cout << "你是个JJ会员!!" << endl;
			else
				cout << "欢迎:" << dis.member.name << endl;

		}
		cout << "admin@root>>";
	}

	//dis.shopping();
}
int _tmain(int argc, _TCHAR* argv[])
{
	unsigned int tmpID = 0;

	system("title 超级超市购物管理系统 ver 1.0   Designed by TK.Wong");
	system("mode con cols=100 lines=80");//改变宽高
	ifstream logo = ifstream("name.txt"); char buf[200];
	while (logo.getline(buf, 200))
	{
		cout << "\t\t" << buf << endl;
	}
	logo.close();
	string user,pass;
	//for (int i = 0; i < 2; i++)
		cout << "\n" << endl;
	cout << "\t\t\t\t\t请输入管理员账号:";
	cin >> user;
	cout << "\t\t\t\t\t请输入密码:" ;
	cin >> pass;
	if (user != "admin"||pass != "admin")
	{
		cout << "\t\t\t密码错误!"<<endl;
		system("pause");
		exit(1);
	}

	system("cls");
	string isContinue;
	while (1)
	{
		process(tmpID);
		cout << "打烊吗?[y]:" ;
		cin >> isContinue;
		if (isContinue == "y")
			break;
		system("cls");

	}
	
	cout << "任意键关闭系统!" << endl;
	system("pause>>nul");
	return 0;
}

