#include "main.h"
#include "Scanner.h"
#include "LL1Parsing.h"

int main()
{
	Scanner *s = new Scanner();
	s->OpenFile("e:\\test.txt");
	s->scanner();
	LL1Parsing l;
	l.OpenFile();
	l.Production();

	l.FisrtInit();		//FIRST���ϳ�ʼ��
	//l.PrintVariFst();

	l.GetVariFst();		//��FIRST��
	//l.PrintVariFst();

	l.GetVariFol();		//��FOLLOW��
	//l.PrintVariFol();

	l.GetProFst();		//�����ʽ��FIRST����
	//l.PrintProFst();

	l.GetFAATable();	//����Ԥ�������
	//l.PrintFAATable();
	l.LL1();			//LL1�﷨����
	getchar();
	return 0;
}