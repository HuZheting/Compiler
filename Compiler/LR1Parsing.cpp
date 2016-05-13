#include "LR1Parsing.h"
#pragma warning(disable : 4996)

LR1Parsing::LR1Parsing()
{
	produceSet = new ProductionSet();
	s = new Scanner();
	s->OpenFile("e:\\test.txt");
	s->scanner();
}

LR1Parsing::~LR1Parsing()
{
	if (ifs.is_open()){
		ifs.close();
	}
	if (ofs.is_open()){
		ofs.close();
	}
	delete produceSet;

	//����Ҫɾ����map���½������е�����
}

void LR1Parsing::Production()
{
	ifstream proSetFile;
	proSetFile.open("pro.txt");
	if (proSetFile.is_open()){
		string buffer;
		while (!proSetFile.eof()){
			getline(proSetFile, buffer);
			int head = (buffer[0] - '0') * 10 + (buffer[1] - '0');

			int i, j;
			char* numStr = new char[20];
			for (i = 3, j = 0; i <= buffer.size() && j < 20; ++i, ++j){

				if (isdigit(buffer[i]) && i < buffer.size()){
					numStr[j] = buffer[i];
				}
				else{
					numStr[j] = '\0';
					char* newStr = new char[j + 1]();
					strcpy(newStr, numStr);
					j = -1;
					produceSet->insert(head, atoll(newStr));
					delete newStr;
				}
			}
			delete numStr;
		}
	}
	else{
		cerr << "error in opening ProSetFile.txt" << endl;
	}
	//produceSet->printProList();
}

void LR1Parsing::OpenFile()
{
	ifs.open("a.txt");
	ofs.open("b.txt");
	if (!ifs.is_open()){
		cerr << "error in LL1Parsing open file" << endl;
	}
	if (!ofs.is_open()){
		cerr << "error in LL1Parsing open file" << endl;
	}
}

int LR1Parsing::GetLength(long long int code)
{
	int a = log10l(code);
	return ((int)log10l(code) + 1);
}

int LR1Parsing::Get2Code(long long int code, int n)
{
	int len = GetLength(code);
	if (len % 2 == 0){
		return (long long int)(code / (long long int)pow(10, len - 2 * n)) % 100;
	}
	else {
		return (long long int)(code / pow(10, len - 2 * n + 1)) % 100;
	}

}

enum tokenType LR1Parsing::GetNewToken()
{
	string buffer;
	char* str = new char[1024]();
	size_t i;
	if (!ifs.eof()){
		getline(ifs, buffer);
		for (i = 0; i < buffer.size(); ++i){
			if (buffer[i] == ' '){
				break;
			}
			else{
				str[i] = buffer[i];
			}
		}
		str[i] = '\0';
	}
	else{
		delete str;
		return EMPTY;
	}

	char* str1 = new char[i + 1]();
	strcpy(str1, str);

	enum tokenType ret = enum tokenType(atoi(str1));

	delete str, str1;
	return ret;
}

void LR1Parsing::FisrtInit()
{
	for (size_t i = 0; i < produceSet->proList.size(); ++i){
		shared_ptr<struct variable> head = produceSet->proList[i];
		shared_ptr<struct terminal> ptr = head->next;
		while (ptr){
			if (ptr->code < 10){
				head->first[ptr->code] = true;
				//cout << "1 " << ptr->code << endl;
			}
			else{
				int code = Get2Code(ptr->code, 1);
				//cout <<"2 " << code << endl;
				if (code < t_len){
					head->first[code] = true;
				}
			}
			ptr = ptr->next;
		}
	}
}

void LR1Parsing::GetVariFst()
{
	bool flag = true;
	size_t size = produceSet->proList.size();

	while (flag){
		flag = false;
		for (size_t i = 0; i < size; ++i){
			shared_ptr<struct variable> head = produceSet->proList[i];
			shared_ptr<struct terminal> ptr = produceSet->proList[i]->next;
			while (ptr){
				for (size_t j = 1; j < (int)((GetLength(ptr->code) + 1) / 2) + 1; ++j){
					int code = 0;
					if (ptr->code < 10){
						head->first[ptr->code] = true;
						break;
					}
					else if ((code = Get2Code(ptr->code, j)) < t_len){
						if (!head->first[code]){
							flag = true;
							head->first[code] = true;
						}
						break;
					}
					else if (code > 49){
						shared_ptr<struct variable> fst = produceSet->proList[code - 50];
						for (size_t k = 0; k < t_len; ++k){
							if (fst->first[k] && !head->first[k]){
								flag = true;
								head->first[k] = true;
							}
						}
						if (!fst->first[0]){
							break;
						}
					}
				}
				ptr = ptr->next;
			}
		}
	}
}

void LR1Parsing::GetClosureSet()
{
	queue<shared_ptr<vector<shared_ptr<struct LR1ItemNode>>>> que;

	shared_ptr<vector<shared_ptr<struct LR1ItemNode>>> startItem = make_shared<vector<shared_ptr<struct LR1ItemNode>>>();
	shared_ptr<struct terminal> codePtr = produceSet->proList[0]->next;

	while (codePtr){
		shared_ptr<struct LR1ItemNode> startNode = make_shared<struct LR1ItemNode>();

		startNode->advancedCode[0] = true;
		startNode->headCode = variSrt;
		startNode->proCode = codePtr->code;
		startNode->point = 0;
		startItem->push_back(startNode);
		codePtr = codePtr->next;
	}
	itemSet.push_back(startItem);

	GetClosure(startItem);

	que.push(startItem);

	while (que.size()){
		shared_ptr<vector<shared_ptr<struct LR1ItemNode>>> item = que.front();
		que.pop();

		//���ڴ���item ���ÿһ������ʽ����һ���µ�ת��
		for (vector<shared_ptr<struct LR1ItemNode>>::iterator iter = item->begin(); iter != item->end(); ++iter){
			if ((*iter)->proCode < 10 && (*iter)->point == 0){
				//��ǰ����ʽ��û�е���Լ��ʱ��
				(*iter)->advanced = (*iter)->proCode;

			}
			else if ((*iter)->proCode > 9 && (*iter)->point < (GetLength((*iter)->proCode) + 1) / 2){
				//��ǰ����ʽ��û�е���Լ��ʱ��

				(*iter)->advanced = Get2Code((*iter)->proCode, (*iter)->point + 1);

			}
			else{
				continue;
			}
			shared_ptr<vector<shared_ptr<struct LR1ItemNode>>> newItem = make_shared<vector<shared_ptr<struct LR1ItemNode>>>();
			shared_ptr<struct LR1ItemNode> newNode = make_shared<struct LR1ItemNode>();
			newNode->headCode = (*iter)->headCode;
			newNode->proCode = (*iter)->proCode;
			newNode->point = (*iter)->point + 1;
			for (size_t k = 0; k < t_len; ++k){
				if ((*iter)->advancedCode[k]){
					newNode->advancedCode[k] = true;
				}
			}


			newItem->push_back(newNode);
			GetClosure(newItem);
			bool tempFlag = false;
			vector<shared_ptr<vector<shared_ptr<struct LR1ItemNode>>>>::iterator iter1;
			for (iter1 = itemSet.begin(); iter1 != itemSet.end(); ++iter1){
				if (Compare2Item((*iter1), newItem)){
					tempFlag = true;
					(*iter)->gotoPtr = *iter1;
					break;
				}
			}
			if (!tempFlag){
				que.push(newItem);
				itemSet.push_back(newItem);
				(*iter)->gotoPtr = newItem;
			}

		}
	}
}

void LR1Parsing::GetClosure(shared_ptr<vector<shared_ptr<struct LR1ItemNode>>> item)
{
	//չʾ�������ظ�������
	vector<shared_ptr<struct LR1ItemNode>> tempSet;
	bool flag = true;


	while (flag){
		flag = false;
		for (vector<shared_ptr<struct LR1ItemNode>>::iterator iter = item->begin(); iter != item->end(); ++iter){
			
			bool advancedList[t_len];
			for (size_t i = 0; i < t_len; ++i){
				advancedList[i] = false;
			}
			if (!(*iter)->isVisited){
				(*iter)->isVisited = true;
				long long int code = (*iter)->proCode;//��ǰ����
				//�õ���ǰ��������ԣ���ǰ�հ���״̬�Ĳ���ʽ
				if (code == 0){
					(*iter)->point = 1;
				}
				if (code > 9 && (*iter)->point + 2 < (GetLength(code) + 1) / 2 + 1 && Get2Code(code, (*iter)->point + 1) >= variSrt){
					//�������һ���ַ�

					int nowCode = Get2Code(code, (*iter)->point + 1);//����ź���һ���ַ�
					int count = (*iter)->point + 2;
					while (count < (GetLength(code) + 1) / 2 + 1){
						int co = 0;
						if ((co = Get2Code(code, count)) >= variSrt){
							for (size_t k = 1; k < t_len; ++k){
								if (produceSet->proList[co - variSrt]->first[k]){
									advancedList[k] = true;
								}
							}
							if (!produceSet->proList[co - variSrt]->first[0]){
								break;
							}
						}
						else{
							advancedList[co] = true;
							break;
						}
						++count;
					}
					if (count >= (GetLength(code) + 1) / 2 + 1){
						//˵������ʽ�ĺ󲿿����Ƶ����ղ���ʽ
						//����Ҫ�����Ƶ����������ʽ����ǰ������
						for (size_t k = 0; k < t_len; ++k){
							if ((*iter)->advancedCode[k]){
								advancedList[k] = true;
							}
						}
					}
				}
				else if (code > 9 && (*iter)->point + 1 < (GetLength(code) + 1) / 2 + 1 && Get2Code(code, (*iter)->point + 1) >= variSrt){
					//��������һ���ַ���̳�

					for (size_t k = 0; k < t_len; ++k){
						if ((*iter)->advancedCode[k]){
							advancedList[k] = true;
						}
					}
				}

				if (code > 9 && (*iter)->point + 1 < (GetLength(code) + 1) / 2 + 1 && Get2Code(code, (*iter)->point + 1) >= variSrt){
					//��������ս��������
					//�������Ƴ��Ĳ���ʽ���뵱ǰ�ļ���
					shared_ptr<struct variable> head = produceSet->proList[Get2Code(code, (*iter)->point + 1) - variSrt];
					shared_ptr<struct terminal> body = head->next;

					while (body){
						shared_ptr<struct LR1ItemNode> newNode = make_shared<struct LR1ItemNode>();
						newNode->point = 0;
						newNode->headCode = head->code;
						newNode->isVisited = false;
						newNode->proCode = body->code;
						body = body->next;
						for (size_t k = 0; k < t_len; ++k){
							if (advancedList[k]){
								newNode->advancedCode[k] = true;
							}
						}
						tempSet.push_back(newNode);
					}
				}
			}
		}

		//����Ѹո�ѹ����ʱջ��itemNodeѹ��հ�ջ
		if (tempSet.size()){
			for (vector<shared_ptr<struct LR1ItemNode>>::iterator iter = tempSet.begin(); iter != tempSet.end(); ++iter){
				item->push_back((*iter));
			}
			tempSet.clear();
			flag = true;
		}
	}
}

void LR1Parsing::GetLR1ParsingTable()
{
	size_t i = 0;
	for (vector<shared_ptr<vector<shared_ptr<struct LR1ItemNode>>>>::iterator iter = itemSet.begin(); iter != itemSet.end(); ++iter, ++i){

		itemTable[(*iter)] = new struct itemSetNode[90]();//ǰ��t_len����ת�ƻ��߹�Լ��variSrt�Ժ���GOTO��
		if (i == 0){
			itemTable[(*iter)][50].flag = 4;
		}
	}
	cout << "һ����" << i << "����Ŀ��" << endl;
	for (vector<shared_ptr<vector<shared_ptr<struct LR1ItemNode>>>>::iterator iter = itemSet.begin(); iter != itemSet.end(); ++iter){
		//����ÿһ����
		shared_ptr<vector<shared_ptr<struct LR1ItemNode>>> item = (*iter);
		
		for (vector<shared_ptr<struct LR1ItemNode>>::iterator itemIter = item->begin(); itemIter != item->end(); ++itemIter){
			//��������ÿһ������ʽ
			shared_ptr<struct LR1ItemNode> itemNode = (*itemIter);
			
			if (itemNode->gotoPtr){
				//˵��״̬�㲻�ڲ���ʽ�����
				int gotoCode = itemNode->advanced;
				if (gotoCode < t_len){
					//ת�Ƶķ������ս��
					itemTable[item][gotoCode].flag = 1;
					itemTable[item][gotoCode].gotoPtr = itemNode->gotoPtr;
				}
				else{
					//ת�Ƶķ��Ų����ս�� Ӧ������GOTO����
					itemTable[item][gotoCode].flag = 3;
					itemTable[item][gotoCode].gotoPtr = itemNode->gotoPtr;
				}
			}

			else{
				//����ʽ�Ĺ�Լ
				for (size_t k = 0; k < t_len; ++k){
					if (itemNode->advancedCode[k]){

						itemTable[item][k].head = itemNode->headCode;
						itemTable[item][k].code = itemNode->proCode;
						itemTable[item][k].flag = 2;
					}
				}
			}
		}
	}
}

bool LR1Parsing::Compare2Item(shared_ptr<vector<shared_ptr<struct LR1ItemNode>>> vector1,
	shared_ptr<vector<shared_ptr<struct LR1ItemNode>>> vector2)
{
	bool flag = true;
	if (vector1->size() == vector2->size()){
		vector<shared_ptr<struct LR1ItemNode>> vec1(*vector1);
		vector<shared_ptr<struct LR1ItemNode>> vec2(*vector2);

		for (vector<shared_ptr<struct LR1ItemNode>>::iterator iter1 = vec1.begin(); iter1 != vec1.end(); ++iter1){

			for (vector<shared_ptr<struct LR1ItemNode>>::iterator iter2 = vec2.begin(); iter2 != vec2.end(); ++iter2){

				if ((*iter1)->headCode == (*iter2)->headCode
					&& (*iter1)->proCode == (*iter2)->proCode
					&& (*iter1)->point == (*iter2)->point){

					for (size_t k = 0; k < t_len; ++k){
						if ((*iter1)->advancedCode[k] != (*iter2)->advancedCode[k]){
							flag = false;
							return flag;
						}
					}
					vec2.erase(iter2);
					break;
				}
				else if (iter2 + 1 == vec2.end()){
					flag = false;
					return flag;
				}
			}
		}
	}
	else{
		flag = false;
	}
	return flag;
}

void LR1Parsing::LR1()
{
	OpenFile();
	Production();
	FisrtInit();
	GetVariFst();
	GetClosureSet();
	GetLR1ParsingTable();


	shared_ptr<vector<shared_ptr<struct LR1ItemNode>>> item = itemSet.front();
	int symbol = GetCode(GetNewToken());
	
	itemStack.push(item);
	symbolStack.push_back(0);
	shared_ptr<struct variNode> head = make_shared<struct variNode>();
	head->offset = 0;
	//variStack.push_back(head);

	while (true)
	{
		struct itemSetNode itemNode = itemTable[itemStack.top()][symbol];
		if (itemNode.flag == 1){//���ǹ�Լ
			itemStack.push(itemNode.gotoPtr);
			symbolStack.push_back(symbol);
			ofs << "push:" << symbol << " " << PrintID(symbol) << endl;
			ofs << "Stack:";
			for (size_t i = 0; i < itemStack.size(); ++i){
				ofs << symbolStack[i] << " ";
			}
			ofs << endl;
			symbol = GetCode(GetNewToken());
		}
		else if (itemNode.flag == 2){//��Լ

			ofs << "��Լ:" << PrintID(itemNode.head)<< itemNode.head << "-->" << itemNode.code << endl;
			int length = (GetLength(itemNode.code) + 1) / 2;
			while (length > 0){

				itemStack.pop();
				symbolStack.pop_back();
				--length;
			}

			symbolStack.push_back(itemNode.head);
			//cout << "push:" << itemNode.head << endl;
			
			if (itemTable[itemStack.top()][itemNode.head].flag == 4 && symbol == 0){
				
				ofs << "Accept!" << endl;
				ofs << "finish!" << endl;
				return;
			}
			itemStack.push(itemTable[itemStack.top()][itemNode.head].gotoPtr);
			ofs << "Stack:";
			for (size_t i = 0; i < itemStack.size(); ++i){
				ofs << symbolStack[i] << " ";
			}
			ofs << endl;
		}
		else{
			cout << "error" << endl;
			return;
		}
	}
}

int LR1Parsing::GetCode(enum tokenType token)
{
	if (token == NOT)
		return 1;
	if (token == AND)
		return 2;
	if (token == OR)
		return 3;
	if (token == ID)
		return 4;
	if (token == ARRAY)
		return 5;
	if (token == FUNC)
		return 6;
	if (token == STRING)
		return 7;
	if (token == INTEGER)
		return 8;
	if (token == REAL)
		return 9;
	if (token == LR_BRAC)
		return 10;
	if (token == RR_BRAC)
		return 11;
	if (token == CHAR)
		return 12;
	if (token == INT)
		return 13;
	if (token == FLOAT)
		return 14;
	if (token == DOUBLE)
		return 15;
	if (token == COMMA)
		return 16;
	if (token == LS_BRAC)
		return 17;
	if (token == RS_BRAC)
		return 18;
	if (token == SEMIC)
		return 19;
	if (token == ASSIGN)
		return 20;
	if (token == LB_BRAC)
		return 21;
	if (token == RB_BRAC)
		return 22;
	if (token == IF)
		return 23;
	if (token == ELSE)
		return 24;
	if (token == WHILE)
		return 25;
	if (token == FOR)
		return 26;
	if (token == CONTINUE)
		return 27;
	if (token == BREAK)
		return 28;
	if (token == RETURN)
		return 29;
	if (token == QUES_MARK)
		return 30;
	if (token == COLON)
		return 31;
	if (token == GT)
		return 32;
	if (token == LT)
		return 33;
	if (token == LE)
		return 34;
	if (token == GE)
		return 35;
	if (token == NE)
		return 36;
	if (token == EQ)
		return 37;
	if (token == PLUS)
		return 38;
	if (token == MINUS)
		return 39;
	if (token == RDIV)
		return 40;
	if (token == MULTI)
		return 41;
	if (token == MOD)
		return 42;
	if (token == CH)
		return 43;
	return 0;
}

string LR1Parsing::PrintID(int token)
{
	if (token == 1)
		return "NOT";
	if (token == 2)
		return "AND";
	if (token == 3)
		return "OR";
	if (token == 4)
		return "ID";
	if (token == 5)
		return "ARRAY";
	if (token == 6)
		return "FUNC";
	if (token == 7)
		return "STRING";
	if (token == 8)
		return "INTEGER";
	if (token == 9)
		return "REAL";
	if (token == 10)
		return "(";
	if (token == 11)
		return ")";
	if (token == 12)
		return "CHAR";
	if (token == 13)
		return "INT";
	if (token == 14)
		return "FLOAT";
	if (token == 15)
		return "COUBLE";
	if (token == 16)
		return "COMMA";
	if (token == 17)
		return "[";
	if (token == 18)
		return "]";
	if (token == 19)
		return ";";
	if (token == 20)
		return "-";
	if (token == 21)
		return "{";
	if (token == 22)
		return "}";
	if (token == 23)
		return "IF";
	if (token == 24)
		return "ELSE";
	if (token == 25)
		return "WHILE";
	if (token == 26)
		return "FOR";
	if (token == 27)
		return "CONTINUE";
	if (token == 28)
		return "BREAK";
	if (token == 29)
		return "RETURN";
	if (token == 30)
		return "QUES_MARK";
	if (token == 31)
		return "COLON";
	if (token == 32)
		return ">";
	if (token == 33)
		return ">";
	if (token == 34)
		return "<=";
	if (token == 35)
		return ">=";
	if (token == 36)
		return "<>";
	if (token == 37)
		return "=";
	if (token == 38)
		return "+";
	if (token == 39)
		return "-";
	if (token == 40)
		return "*";
	if (token == 41)
		return "/";
	if (token == 42)
		return "%";
	if (token == 43)
		return "CH";
	if (token == 50)
		return "����";
	if (token == 51)
		return "��չ����";
	if (token == 52)
		return "��������";
	if (token == 53)
		return "���Ͷ���";
	if (token == 54)
		return "�β��б�";
	if (token == 55)
		return "�β�����";
	if (token == 56)
		return "��������";
	if (token == 57)
		return "��ʼ������";
	if (token == 58)
		return "ֱ��������";
	if (token == 59)
		return "��ʼ������";
	if (token == 60)
		return "��ʼ������";
	if (token == 61)
		return "��ʼ������1";
	if (token == 62)
		return "�������";
	if (token == 63)
		return "���������";
	if (token == 64)
		return "�������";
	if (token == 65)
		return "���";
	if (token == 66)
		return "������";
	if (token == 67)
		return "��֧���";
	if (token == 68)
		return "ѭ�����";
	if (token == 69)
		return "��ת���";
	if (token == 70)
		return "���ʽ";
	if (token == 71)
		return "�������ʽ";
	if (token == 72)
		return "��Ŀ����ʽ";
	if (token == 73)
		return "�߼����ʽ";
	if (token == 74)
		return "�߼����ʽ1";
	if (token == 75)
		return "�߼���";
	if (token == 76)
		return "��ϵ���ʽ";
	if (token == 77)
		return "��ϵ���ʽ1";
	if (token == 78)
		return "��ϵ������";
	if (token == 79)
		return "��ʽ";
	if (token == 80)
		return "��ʽ1";
	if (token == 81)
		return "��";
	if (token == 82)
		return "��׺���ʽ";
	if (token == 83)
		return "�������ʽ";
	if (token == 84)
		return "ʵ���б�";
	if (token == 85)
		return "�Ӽ�������";
	if (token == 86)
		return "�˳�������";
	if (token == 87)
		return "if���";
	if (token == 88)
		return "else���";
	if (token == 89)
		return "��1";
	return "empty";
}

shared_ptr<struct variNode> LR1Parsing::genCode(int head, long long int production)
{
	shared_ptr<struct variNode> newNode = make_shared<struct variNode>();

	switch (head){
	case 50:
		break;
	case 51:
		break;
	case 52:
		break;
	case 53:
		switch (production){
		case 12:
			//char
			newNode->type = CHAR;
			newNode->width = 1;
			break;
		case 13:
			//int
			newNode->type = INT;
			newNode->width = 1;
			break;
		case 14:
			//float
			newNode->type = FLOAT;
			newNode->width = 1;
			break;
		case 15:
			//double
			newNode->type = DOUBLE;
			newNode->width = 1;
			break;
		default:
			break;
		}
		break;
	case 58:
		switch (production){
		case 4:
			newNode->name = (*variStack.end())->name;
			newNode->token_type = ID;
			newNode->offset = 0;
			break;
		case 5178318:
			newNode->name = (*variStack.end())->name;
			newNode->token_type = ARRAY;
			//����Ҳ�п�����value
			newNode->offset = (*(variStack.end()-2))->offset;
			break;
		}
		break;
	case 83:
		switch (production){
		case 4:
			//��ʶ��
			genCode();
		case 8:
			//��������
		case 9:
			//С������
		case 7:
			//�ַ�������
		case 43:
			//�ַ�

		default:
			break;
		}
	default:
		;
	}

	return newNode;
}

void LR1Parsing::genCode()
{
	//���һ���м����ʲôҲ����
}