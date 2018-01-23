#pragma once
#include <string>
#include <fstream>
#ifdef _M_IX86  
#define MAXVAL 0xFFFFFFFF
#else
#define MAXVAL 0xFFFFFFFFFFFFFFFF
#endif // _M_IX86

using namespace std;

typedef struct
{
	unsigned int BlockPosition;//文件块位置
	//unsigned int size;
	//变长大小，功能可有可无
	char extra;//额外信息标志
			   //文件块名称，预留
	unsigned int last;
	unsigned int next;//双向链表
}HashTable;

template <class T>
class Database
{
public:
	Database(string table);//table是path路径和表的名称，泛型T是表的结构
	Database(string table, string path);
	~Database(void);
	//int bind(char* table,T title);
	int append(T);//顺序插入，成功返回0，失败返回-1
	//unsigned int 长度在64bit和32bit机的不同，有可能导致数据的读取问题
	int insert(unsigned int key, T);
	int update(unsigned int key,T);//key代表修改的主值，T代表修改的记录
	int del(unsigned int key);
	unsigned int GetFileEnd();
	T select(unsigned int key);//主值选取数据
	T select(string keywords);//字符串搜索查询，332
	T select(string keywords, unsigned int , unsigned int );//字符串搜索查询，332
	unsigned int GetEndKey();
	class Trash //垃圾回收类
	{
	public:
		Trash();
		~Trash();
		long allocate(unsigned int size);
		void free();//释放回收记录
		void sort();//碎片整理
	};

private:
	//T title;
	unsigned int fend;
	unsigned int length,member;
	unsigned int CountKey;
	inline int readfile(string path,unsigned int position,char *data, unsigned int len);
	inline int ReadBlock(string path, unsigned int position, char *data);
	inline void writefile(string path, unsigned int position, char *data, unsigned int len, bool append);
	unsigned int  getEnd(string path, unsigned int len);
	HashTable GetLogicHash(unsigned int key);//哈希出文件块的逻辑位置，key值是逻辑键值,即按逻辑位置遍历
	unsigned int map(unsigned int key, bool logic2phisics);//哈希表的物理位置和逻辑位置的相互转换
	HashTable GetHash(unsigned int key);//哈希出文件块的具体位置
	
	unsigned int PutHash(HashTable hash,bool instert);//返回写入哈希表的具体位置
	void AlterHash(unsigned int key, HashTable hash);
	string table;
	string FilePath;

};

template<class T>
Database<T>::Database(string table)
{
	this->length = sizeof(T);
	this->table = table;
	this->FilePath = table;
	this->fend = getEnd(this->FilePath+".HASH",sizeof(HashTable));
	//this->fend = getEnd(this->FilePath,this->length);
	//cout << count() << endl;
	CountKey = GetEndKey();
	
}
template<class T>
Database<T>::Database(string table,string path)
{
	this->length = sizeof(T);
	this->table = table;
	this->FilePath = path+"\\"+table;//windows
	//this->fend = getEnd(this->FilePath,this->length);
	this->fend = getEnd(this->FilePath + ".HASH", sizeof(HashTable));
	//cout << count() << endl;
	CountKey = GetEndKey();
}

template<class T>
Database<T>::~Database(void)
{

}
/*
template<class T>
T Database<T>::select(unsigned int key)
{
	char *data = new char[length];
	int error = readfile(FilePath,key,data,length);
	if (error == -1)
	{
		throw "Key is over the boundary!";
	}
	T ret;
	memcpy(&ret,data,length);
	delete(data);
	return ret;
}
*/

template<class T>
T Database<T>::select(unsigned int key)
{

	char *data = new char[length];
	HashTable hash = GetLogicHash(key);
	int error = ReadBlock(FilePath, hash.BlockPosition, data);
	if (error == -1 || (key>CountKey))
	{
		
		throw "Key is over the boundary!";
	}
	T ret;
	memcpy(&ret, data, length);
	delete(data);
	return ret;
}

template<class T>
T Database<T>::select(string keywords)
{

}

template<class T>
T Database<T>::select(string keywords, unsigned int begin, unsigned int end)
{

}

/*
template<class T>
int Database<T>::append(T t)
{
	char *data = new char[length];
	memcpy(data,&t,length);
	writefile(FilePath,fend,data,length);
	delete(data);
	cout << fend << endl;
	return 0;
}
*/

template<class T>
int Database<T>::append(T t)
{
	char *data = new char[length];
	memcpy(data, &t, length);
	HashTable h = GetHash(fend-1);
	writefile(FilePath, h.BlockPosition, data,length,true);
	HashTable h2;
	h2.BlockPosition = h.BlockPosition + length;
	h2.extra = 0;
	h2.last = fend-1;
	h2.next = 0;
	PutHash(h2,false);
	delete(data);
	//cout <<"Hash Block:"<<h.BlockPosition << endl;
	return 0;
}

template<class T>
int Database<T>::insert(unsigned int key, T t)//Key是跟在第Key个节点的后面,不是将第Key个节点替换掉，插入位置是Key+1
{
	char *data = new char[length];
	memcpy(data, &t, length);
	//拷贝到data中
	//然后写入FileBlock中,写入方式是append，然后Hash指针指过去
	writefile(FilePath, fend, data, length, true);//先将数据写入文件块中
	fend++;//文件长度自增
	HashTable hash[3];
	unsigned int phisic[3];
	hash[0] = GetLogicHash(key);//lash node,前一个节点
	phisic[0] = map(key, true);
	hash[2] = GetLogicHash(key+1);//next node，后一个节点
	phisic[2] = map(key + 1,true);
	phisic[1] = fend  - 1;//下标从0开始
	hash[1].BlockPosition = (fend)*length;//新申请的位置
	hash[1].extra = 0;
	//修改双向链表指针,key键值排上用场
	hash[1].last = hash[2].last;
	hash[1].next = hash[0].next;//插在key和key+1中间，变成一个新的key+1，原先的key+`1顺延变成逻辑上的key+2
	PutHash(hash[1],true);//插入hash新表
	hash[0].next = phisic[1];//更改为插入的节点
	hash[2].last = phisic[1];
	AlterHash(phisic[0], hash[0]);//应该更改到逻辑位置上，需要进行逻辑位置和物理映射的哈希转换
	AlterHash(phisic[2], hash[2]);
	delete[] data;
	return 0;
}

template<class T>
int Database<T>::update(unsigned int key,T t)//修改指定记录的内容
{
	if (key > this->CountKey)
		return -1;
	HashTable hash;
	hash = GetLogicHash(key);
	//cout << "HASH POS:" << hash.BlockPosition << " HASH last:" << hash.last << " HASH next" << hash.next << endl;
	char *data = new char[length];
	memcpy(data, &t, length);
	writefile(FilePath, (hash.BlockPosition - length)/length, data , length, false);
	delete data;
	return 0;

}

template<class T>
int Database<T>::del(unsigned int key)
{
	if (key > this->CountKey)
		return -1;
	char *data = new char[length];
	//memcpy(data, &t, length);
	//拷贝到data中
	//然后写入FileBlock中,写入方式是append，然后Hash指针指过去
	writefile(FilePath, fend, data, length, true);//先将数据写入文件块中
	HashTable hash[3];
	hash[0] = GetLogicHash(key - 1);//lash node,前一个节点
	hash[1] = GetLogicHash(key);//this node，被删除节点
	hash[2] = GetLogicHash(key + 1);//next node,后一个节点
	//hash[0].BlockPosition = (fend + 1)*length;//新申请的位置
	//hash[0].extra = 0;
	//修改双向链表指针,key键值排上用场
	//hash[0].last = hash[2].last;
	//hash[0].next = hash[1].next;//插在key和key+1中间，变成一个新的key+1，原先的key+`1顺延变成逻辑上的key+2
	//unsigned int pos = PutHash(hash[0], true) - 1;//下标从0开始，返回的是个数，减一
	hash[0].next = hash[1].next;//更改为插入的节点
	hash[2].last = hash[1].last;
	AlterHash(map(key - 1, true), hash[0]);//应该更改到逻辑位置上，需要进行逻辑位置和物理映射的哈希转换
	AlterHash(map(key, true), hash[2]);
	CountKey--;//fend不用减，fend是文件的末尾位置
	delete[] data;
	return 0;
}

template<class T>
inline unsigned int Database<T>::GetFileEnd()
{
	return fend;
}

template<class T>
inline void Database<T>::writefile(string path, unsigned int position, char *data,unsigned int len,bool append)
{
	int method;
	if (append)
		method = ios::binary | ios::app;
	else
		method = ios::binary | ios::in;
	ofstream output(path, method);
	output.seekp(position*len);
	output.write(data, len);
	output.close();
}

template<class T>
inline int Database<T>::readfile(string path, unsigned int position, char *data,unsigned int len)
{
	ifstream input(path, ios::binary|ios::in);
	//char *buffer = new char[length];
	if ((fend == 0))
		return -2;
	else if ((position > fend))
	{
		//cout << "position"<<position<<"fend:"<<fend<<"return -1" << endl;
		return -1;
	}
	input.seekg(len*position);
	input.read(data,len);
	input.close();
	return 0;
}

template<class T>
inline int Database<T>::ReadBlock(string path, unsigned int position, char *data)
{
	ifstream input(path,ios::binary);
	//cout << "position:" << position << " fend:" << fend << endl;
	if (position > (fend)*length)
		return -1;
	input.seekg(position - length);
	input.read(data, length);
	input.close();
	return 0;
}

template<class T>
unsigned int Database<T>::getEnd(string path, unsigned int len)
{
	ifstream input(path, ios::binary|ios::app);
	//char *buffer = new char[length];
	input.seekg(0,ios::end);
	std::streamoff end = input.tellg();
	unsigned int  ret = (end - end % len)/len;
	//cout << end << endl;
	input.close();
	return ret;
}

template<class T>
HashTable Database<T>::GetHash(unsigned int key)//哈希出文件块的具体位置，key值是绝对地址,即按物理位置遍历
{

	char *data = new char[sizeof(HashTable)];
	int error = readfile(FilePath+".HASH", key, data, sizeof(HashTable));
	HashTable hash;
	//cout <<"error:"<< error << endl;
	if (error == -2)
	{
		hash.BlockPosition = 0;
		hash.extra = 1;//head
		hash.last = 0;
		hash.next = 0;//null
		//PutHash(hash);
		//cout << -2 << endl;
	}
	else
	{
		memcpy(&hash, data, sizeof(HashTable));
		//cout << "Get Real Hash:" << hash.BlockPosition<<"next:"<<hash.next <<"last:"<<hash.last<< endl;
	}
	delete(data);
	return hash;
}

template<class T>
HashTable Database<T>::GetLogicHash(unsigned int key)//哈希出文件块的逻辑位置，key值是逻辑键值,即按逻辑位置遍历
{
	HashTable t;
	t.next = 0;//从头开始
	for (unsigned int i = 0; i < key+1; i++)
	{
		//链表遍历到key的位置
		t = GetHash(t.next);
		//cout <<i<< "   GETHASH:::::" << t.next << endl;
		if ((t.next == 0) ) //到链表的尾部了,此处-1其实是0xFFFFFFFF,利用了无符号整形的溢出操作
		{
			//cout << "!!!!!!!!jinlaila" << endl;
			//t.extra = -1;//extra 排上用场了，-1表示没找到
			return t;
		}
	}
	return t;
}


template<class T>
unsigned int Database<T>::PutHash(HashTable hash, bool inst)//增加哈希链节
{
	char *data = new char[sizeof(HashTable)];
	memcpy(data, &hash, sizeof(HashTable));
	writefile(FilePath + ".HASH", fend, data, sizeof(HashTable),true);
	
	if (fend > 0 && !inst)//否则会造成无符号整型数的溢出，fend - 1 4byte 是 0xFF FF FF FF,很大
	{
		HashTable tmp = GetHash(fend - 1);
		tmp.next = fend;
		AlterHash(fend - 1, tmp);
	}
	fend++;
	CountKey++;
	delete(data);
	return fend;
}

template<class T>
void Database<T>::AlterHash(unsigned int key ,HashTable hash)//修改指定位置的HASH链表节点
{
	char *data = new char[sizeof(HashTable)];
	memcpy(data, &hash, sizeof(HashTable));
	writefile(FilePath + ".HASH",key, data, sizeof(HashTable), false);
	delete(data);
}

template<class T>
unsigned int Database<T>::map(unsigned int key, bool logic2phisics)
{
	HashTable t;
	t.next = 0;//从头开始
	char *data = new char[sizeof(HashTable)];
	ifstream input(FilePath + ".HASH", ios::binary | ios::in);
	unsigned int ret;
	if (logic2phisics)//逻辑转物理
	{
		
		
		for (unsigned int i = 0; i < key + 1; i++)//
		{
			//顺着链表遍历到key的位置,取得此处位置的物理位置，即为返回值
			//t = GetHash(t.next);
			///////////////////////
			
			//int error = readfile(FilePath + ".HASH", t.next, data, sizeof(HashTable));
			/////////////////////////////////////
			
			if ((fend == 0))
			{
				ret = 0;
				break;
			}
			else if ((key > fend))//fend是物理最大长度,已经越界了，
			{
				ret = fend;
				break;
			}
			input.seekg(sizeof(HashTable)*(t.next));//移动文件游标，逐个物理位置遍历
			input.read(data,sizeof(HashTable));
			
			memcpy(&t, data, sizeof(HashTable));
			ret = input.tellg()/sizeof(HashTable) - 1;
			//cout << input.tellg() << endl;
			if ((t.next == 0)) //到链表的尾部了
			{
				//return ;
				break;
			}
		}
	}
	else//物理转逻辑
	{
		cout << "waitting" << endl;
	}
	input.close();
	delete(data);
	return ret;
}

template<class T>
inline unsigned int Database<T>::GetEndKey()
{
	HashTable t;
	t.next = 0;//从头开始
	for (unsigned int i = 0; ; i++)
	{
		t = GetHash(t.next);
		if ((t.next == 0)) //到链表的尾部了
		{
			return i;
		}
	}
	return 0;
}
