#pragma once
#include <cmath>
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <fstream>
#define t_len 43
using namespace std;
enum tokenType {
	EMPTY,
	ID,
	INTEGER,
	REAL,
	STRING,
	CH,
	ARRAY,
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
	shared_ptr<struct terminal> next;
	bool first[t_len];
	bool follow[t_len];

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
	shared_ptr<struct terminal> next;
	bool first[t_len];

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

	//�����Ժ�Ҫ���һ��ָ����ű��ָ��
	tokenRecord()
	{
		next = nullptr;
	}
};