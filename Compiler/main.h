#pragma once
#include <cmath>
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <fstream>
#include <stack>

#define t_len 44
using namespace std;
enum tokenType {
	EMPTY,
	ID,
	INTEGER,
	REAL,
	STRING,
	CH,
	ARRAY,
	FUNC,
	NOT,
	AND,
	OR,
	INT,
	FLOAT,
	DOUBLE,
	CHAR,
	WHILE,
	FOR,
	IF,
	ELSE,
	SWITCH,
	CASE,
	DEFAULT,
	TRUE,
	FALSE,
	DEFINE,
	RETURN,
	BREAK,
	CONTINUE,
	CONST,
	UNDERLINE,
	PLUS,
	MINUS,
	MULTI,
	RDIV,
	MOD,
	ASSIGN,
	EQ,
	LT,
	GT,
	NE,
	LE,
	GE,
	LR_BRAC,
	RR_BRAC,
	LS_BRAC,
	RS_BRAC,
	LB_BRAC,
	RB_BRAC,
	QUES_MARK,
	COLON,
	SEMIC,
	Q_MARK,
	DOUBLE_QUOTE,
	COMMA,
	HASHTAG,
	BACKSLASH,
};

struct variable
{
	size_t code;
	bool first[t_len];
	bool follow[t_len];
	shared_ptr<struct terminal> next;

	struct variable(){
		next = nullptr;
		for (size_t i = 0; i < t_len; ++i){
			first[i] = false;
			follow[i] = false;
		}
	}
};

struct terminal
{
	long long int code;
	bool first[t_len];
	shared_ptr<struct terminal> next;

	struct terminal(){
		next = nullptr;
		for (size_t i = 0; i < t_len; ++i){
			first[i] = false;
		}
	}
};

struct tokenRecord
{
	enum tokenType token;		//��������
	shared_ptr<struct tokenRecord> next;	//ָ����һ�����ŵ�ָ��
	shared_ptr<struct nameItam> name_item;
	//�����Ժ�Ҫ���һ��ָ����ű��ָ��
	tokenRecord()
	{
		next = nullptr;
	}
};

struct nameItem
{
	string name;//ÿһ��token������
	//enum tokenType token_type;//token������
	enum tokenType type;//ʵ�ʵ�����
	int* extendPtr;//��չ����ָ��
	
	shared_ptr<struct nameItem> next;

	union {
		char ch;
		int intrger;
		double rear;
		char* str;
		int* address;//������ڴ��ַ
	} value;
	struct nameItem(){
		name = nullptr;
		extendPtr = nullptr;
	}
};

struct nameTableHead
{
	stack<shared_ptr<struct nameTableHead>> otherNameTable;
	shared_ptr<struct nameItem> name_item;

	struct nameTableHead(){
		name_item = nullptr;
	}
};