#pragma once
#include "main.h"
#include "ProductionSet.h"

/*
�� 00
not 01
and 02
or 03
��ʶ�� 04
������ 05
������ 06
�ַ��� 07
�������� 08
С������ 09
( 10
) 11
char 12
int 13
float 14
double 15
, 16
[ 17
] 18
; 19
= 20
{ 21
} 22
if 23
else 24
while 25
for 26
continue 27
break 28
return 29
? 30
: 31
> 32
< 33
<= 34
>= 35
<> 36
== 37
+ 38
- 39
/ 40
* 41
% 42
CH 43

���� 50
��չ���� 51
�������� 52
���Ͷ��� 53
�β��б� 54
�β����� 55
�������� 56
��ʼ������ 57
ֱ�������� 58
��ʼ������ 59
��ʼ������ 60
��ʼ������1 61
������� 62
��������� 63
������� 64
��� 65
������ 66
��֧��� 67
ѭ����� 68
��ת��� 69
���ʽ 70
�������ʽ 71
��Ŀ����ʽ 72
�߼����ʽ 73
�߼����ʽ1 74
�߼��� 75
��ϵ���ʽ 76
��ϵ���ʽ1 77
��ϵ������ 78
��ʽ 79
��ʽ1 80
�� 81
��׺���ʽ 82
�������ʽ 83
ʵ���б� 84
�Ӽ������� 85
�˳������� 86
*/



class LL1Parsing
{
private:
	ifstream ifs;
	ProductionSet* produceSet;
	
	int GetLength(long long int code);
	int Get2Code(long long int code, int n);
	vector<long long int> faaTable[40][44 + 1];

public: 
	
	LL1Parsing();
	~LL1Parsing();
	void FisrtInit();
	void OpenFile();
	void Production();
	enum tokenType GetNewToken();
	void GetVariFst();
	void GetVariFol();
	void GetProFst();
	void GetFAATable();
	void LL1();
	int GetCode(enum tokenType token);

	void PrintVariFst();
	void PrintVariFol();
	void PrintProFst();
	void PrintFAATable();
};

