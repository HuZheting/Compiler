#pragma once

/*�ʷ�������
����������
������Toke*/
#include "NameTable.h"


class Scanner
{
private:
	size_t row_counter;//�м�����
	size_t col_counter;//�м�����
	ifstream ifs;	//Դ�����ļ�
	ofstream ofs;	//tokenд���ļ�
	string buffer;	//�л�����
	shared_ptr<struct tokenRecord> head;	//token����ͷָ��
	shared_ptr<struct tokenRecord> rear;	//token����βָ��
	NameTable nameTable;

public:
	Scanner();
	~Scanner();

	void OpenFile(string filename);
	bool ReadNewLine();
	void scanner();
	void Sort(char ch);		//���ʷ���
	void Num(char ch);		//��ֵ������
	void Identify(char ch);	//��ʶ��������
	void String(char ch);	//�ַ���������
	void Ch(char ch);		//�ַ�������
	void newToken(enum tokenType token);
	void newToken(enum tokenType token, char ch);
	void newToken(enum tokenType token, char* name);

	void SetRowCounter(size_t value);
	void SetColCounter(size_t value);

	size_t GetRowCounter();
	size_t GetColCounter();
};

