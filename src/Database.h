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
	unsigned int BlockPosition;//�ļ���λ��
	//unsigned int size;
	//�䳤��С�����ܿ��п���
	char extra;//������Ϣ��־
			   //�ļ������ƣ�Ԥ��
	unsigned int last;
	unsigned int next;//˫������
}HashTable;

template <class T>
class Database
{
public:
	Database(string table);//table��path·���ͱ�����ƣ�����T�Ǳ�Ľṹ
	Database(string table, string path);
	~Database(void);
	//int bind(char* table,T title);
	int append(T);//˳����룬�ɹ�����0��ʧ�ܷ���-1
	//unsigned int ������64bit��32bit���Ĳ�ͬ���п��ܵ������ݵĶ�ȡ����
	int insert(unsigned int key, T);
	int update(unsigned int key,T);//key�����޸ĵ���ֵ��T�����޸ĵļ�¼
	int del(unsigned int key);
	unsigned int GetFileEnd();
	T select(unsigned int key);//��ֵѡȡ����
	T select(string keywords);//�ַ���������ѯ��332
	T select(string keywords, unsigned int , unsigned int );//�ַ���������ѯ��332
	unsigned int GetEndKey();
	class Trash //����������
	{
	public:
		Trash();
		~Trash();
		long allocate(unsigned int size);
		void free();//�ͷŻ��ռ�¼
		void sort();//��Ƭ����
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
	HashTable GetLogicHash(unsigned int key);//��ϣ���ļ�����߼�λ�ã�keyֵ���߼���ֵ,�����߼�λ�ñ���
	unsigned int map(unsigned int key, bool logic2phisics);//��ϣ�������λ�ú��߼�λ�õ��໥ת��
	HashTable GetHash(unsigned int key);//��ϣ���ļ���ľ���λ��
	
	unsigned int PutHash(HashTable hash,bool instert);//����д���ϣ��ľ���λ��
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
int Database<T>::insert(unsigned int key, T t)//Key�Ǹ��ڵ�Key���ڵ�ĺ���,���ǽ���Key���ڵ��滻��������λ����Key+1
{
	char *data = new char[length];
	memcpy(data, &t, length);
	//������data��
	//Ȼ��д��FileBlock��,д�뷽ʽ��append��Ȼ��Hashָ��ָ��ȥ
	writefile(FilePath, fend, data, length, true);//�Ƚ�����д���ļ�����
	fend++;//�ļ���������
	HashTable hash[3];
	unsigned int phisic[3];
	hash[0] = GetLogicHash(key);//lash node,ǰһ���ڵ�
	phisic[0] = map(key, true);
	hash[2] = GetLogicHash(key+1);//next node����һ���ڵ�
	phisic[2] = map(key + 1,true);
	phisic[1] = fend  - 1;//�±��0��ʼ
	hash[1].BlockPosition = (fend)*length;//�������λ��
	hash[1].extra = 0;
	//�޸�˫������ָ��,key��ֵ�����ó�
	hash[1].last = hash[2].last;
	hash[1].next = hash[0].next;//����key��key+1�м䣬���һ���µ�key+1��ԭ�ȵ�key+`1˳�ӱ���߼��ϵ�key+2
	PutHash(hash[1],true);//����hash�±�
	hash[0].next = phisic[1];//����Ϊ����Ľڵ�
	hash[2].last = phisic[1];
	AlterHash(phisic[0], hash[0]);//Ӧ�ø��ĵ��߼�λ���ϣ���Ҫ�����߼�λ�ú�����ӳ��Ĺ�ϣת��
	AlterHash(phisic[2], hash[2]);
	delete[] data;
	return 0;
}

template<class T>
int Database<T>::update(unsigned int key,T t)//�޸�ָ����¼������
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
	//������data��
	//Ȼ��д��FileBlock��,д�뷽ʽ��append��Ȼ��Hashָ��ָ��ȥ
	writefile(FilePath, fend, data, length, true);//�Ƚ�����д���ļ�����
	HashTable hash[3];
	hash[0] = GetLogicHash(key - 1);//lash node,ǰһ���ڵ�
	hash[1] = GetLogicHash(key);//this node����ɾ���ڵ�
	hash[2] = GetLogicHash(key + 1);//next node,��һ���ڵ�
	//hash[0].BlockPosition = (fend + 1)*length;//�������λ��
	//hash[0].extra = 0;
	//�޸�˫������ָ��,key��ֵ�����ó�
	//hash[0].last = hash[2].last;
	//hash[0].next = hash[1].next;//����key��key+1�м䣬���һ���µ�key+1��ԭ�ȵ�key+`1˳�ӱ���߼��ϵ�key+2
	//unsigned int pos = PutHash(hash[0], true) - 1;//�±��0��ʼ�����ص��Ǹ�������һ
	hash[0].next = hash[1].next;//����Ϊ����Ľڵ�
	hash[2].last = hash[1].last;
	AlterHash(map(key - 1, true), hash[0]);//Ӧ�ø��ĵ��߼�λ���ϣ���Ҫ�����߼�λ�ú�����ӳ��Ĺ�ϣת��
	AlterHash(map(key, true), hash[2]);
	CountKey--;//fend���ü���fend���ļ���ĩβλ��
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
HashTable Database<T>::GetHash(unsigned int key)//��ϣ���ļ���ľ���λ�ã�keyֵ�Ǿ��Ե�ַ,��������λ�ñ���
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
HashTable Database<T>::GetLogicHash(unsigned int key)//��ϣ���ļ�����߼�λ�ã�keyֵ���߼���ֵ,�����߼�λ�ñ���
{
	HashTable t;
	t.next = 0;//��ͷ��ʼ
	for (unsigned int i = 0; i < key+1; i++)
	{
		//���������key��λ��
		t = GetHash(t.next);
		//cout <<i<< "   GETHASH:::::" << t.next << endl;
		if ((t.next == 0) ) //�������β����,�˴�-1��ʵ��0xFFFFFFFF,�������޷������ε��������
		{
			//cout << "!!!!!!!!jinlaila" << endl;
			//t.extra = -1;//extra �����ó��ˣ�-1��ʾû�ҵ�
			return t;
		}
	}
	return t;
}


template<class T>
unsigned int Database<T>::PutHash(HashTable hash, bool inst)//���ӹ�ϣ����
{
	char *data = new char[sizeof(HashTable)];
	memcpy(data, &hash, sizeof(HashTable));
	writefile(FilePath + ".HASH", fend, data, sizeof(HashTable),true);
	
	if (fend > 0 && !inst)//���������޷����������������fend - 1 4byte �� 0xFF FF FF FF,�ܴ�
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
void Database<T>::AlterHash(unsigned int key ,HashTable hash)//�޸�ָ��λ�õ�HASH����ڵ�
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
	t.next = 0;//��ͷ��ʼ
	char *data = new char[sizeof(HashTable)];
	ifstream input(FilePath + ".HASH", ios::binary | ios::in);
	unsigned int ret;
	if (logic2phisics)//�߼�ת����
	{
		
		
		for (unsigned int i = 0; i < key + 1; i++)//
		{
			//˳�����������key��λ��,ȡ�ô˴�λ�õ�����λ�ã���Ϊ����ֵ
			//t = GetHash(t.next);
			///////////////////////
			
			//int error = readfile(FilePath + ".HASH", t.next, data, sizeof(HashTable));
			/////////////////////////////////////
			
			if ((fend == 0))
			{
				ret = 0;
				break;
			}
			else if ((key > fend))//fend��������󳤶�,�Ѿ�Խ���ˣ�
			{
				ret = fend;
				break;
			}
			input.seekg(sizeof(HashTable)*(t.next));//�ƶ��ļ��α꣬�������λ�ñ���
			input.read(data,sizeof(HashTable));
			
			memcpy(&t, data, sizeof(HashTable));
			ret = input.tellg()/sizeof(HashTable) - 1;
			//cout << input.tellg() << endl;
			if ((t.next == 0)) //�������β����
			{
				//return ;
				break;
			}
		}
	}
	else//����ת�߼�
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
	t.next = 0;//��ͷ��ʼ
	for (unsigned int i = 0; ; i++)
	{
		t = GetHash(t.next);
		if ((t.next == 0)) //�������β����
		{
			return i;
		}
	}
	return 0;
}
