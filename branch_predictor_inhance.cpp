#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <sstream>
#include <vector>
#include <math.h>

using namespace std;

struct Predictor
{
	bool first;		// true if is first access
	int pmode[2];
	int phistory[4];
};

string Label_name[8];
int Label_position[8];

vector<Predictor*>entry;		// store entries
vector<string*> code;			// code read by file
vector<int*> numcode;			// code translate from code
vector<string> realpc;			// store instruction name from file

int pc = 0, num_predictor = 0;

//initial register
int regi[8] = {0,0,0,0,0,0,0,0};

//===================== read file ==========================

int findnum(string a)
{
	int temp = 0;
	if(a == "add"){return 0;	}
	else if(a == "sub"){return 1;	}
	else if(a == "mul"){return 2;	}
	else if(a == "addi"){return 4;	}
	else if(a == "subi"){return 5;	}
	else if(a == "muli"){return 6;	}
	else if(a == "beq"){return 8;	}
	else if(a == "bne"){return 9;	}
	else if(a == "andi"){return 10;	}
	else if(a == "J"){return 12;	}
	else if(a == "R0"){return 0;	}
	else if(a == "R1"){return 1;	}
	else if(a == "R2"){return 2;	}
	else if(a == "R3"){return 3;	}
	else if(a == "R4"){return 4;	}
	else if(a == "R5"){return 5;	}
	else if(a == "R6"){return 6;	}
	else if(a == "R7"){return 7;	}
	else if(a == "0"){return 0;	}
	else if(a == "1"){return 1;	}
	else if(a == "2"){return 2;	}
	else if(a == "3"){return 3;	}
	else if(a == "4"){return 4;	}
	else if(a == "5"){return 5;	}
	else if(a == "6"){return 6;	}
	else if(a == "7"){return 7;	}
	else if(a == "8"){return 8;	}
	else if(a == "9"){return 9;	}
	else
	{
		for(int i = 0; i < 8;i++)
		{
			if(Label_name[i] == a)
			{
				return Label_position[i];
			}
		}
		cout << "Including unvalid input!" << endl;	
	}
}

int getnum(string a)
{
	int num = 0;
	string temp;
	for(int k = 0; k < a.size() ;k++)
	{
		if(int(a[k]) > 57 || a[k] < 48)
		{
			return findnum(a);
		}
		else
		{
			temp = a[k];
			num = num + findnum(temp) * pow(10, a.size()-1-k);
		}
	}
	return num;
}

void readfile() 
{
	ifstream file;
	string line, temp;
	string *ptr;
	int pcc = 0, Label = 0, count = 0;
	bool gene = true;	// if this line is number of predictor 
	
	file.open("pre_inhance.txt", ios::in);
	if (file.is_open())
	{
		while(getline(file, line))
		{
			if(gene)
			{
				num_predictor = getnum(line);
				gene = false;
			}
			
			else if(line.find(":")!= std::string::npos)				//find Label 
			{
				for(int j = 0; j < line.size();j++)
				{
					switch(line[j])
					{
					case '\t':
						temp.clear();
						break;
					case ':':
						Label_name[Label] = temp;
						temp.clear();
						Label_position[Label] = pcc;
						Label++;
						break;
					default:
						temp = temp + line[j];			
						break;
					}
				}
				continue;
			}
			else		// instructions
			{
				temp.clear();
				count = 0;
				ptr = new string[4];
				
				for(int i = 0; i < line.size();i++)
				{
					switch(line[i])
					{
					case '\t':
						realpc.push_back(temp);
						temp.clear();
						break;
					case ' ':
						*(ptr) = temp;
						temp.clear();
						break;
					case ',':
						*(ptr + 1 + count) = temp;
						temp.clear();
						count++;
						break;
					default:
						temp = temp + line[i];			
						break;
					}
					
					if(i == line.size() - 1)
					{
						*(ptr + 1 + count) = temp;
						temp.clear();
					}	
				}
				
				code.push_back(ptr);
				pcc++;
			}
		}
		file.close();
	}
	else
	{
		cout << "Unable to open the file!" << endl;
	}
}

void transinst()
{
	int *ptrn;
	for (int i = 0; i < code.size(); i++)
	{
		if(*(code[i]) == "J")
		{
			*(ptrn) = findnum(*(code[i]));
			*(ptrn + 1) = getnum(*(code[i] + 1));	
		}
		else
		{
			ptrn = new int[4];
			*(ptrn) = findnum(*(code[i]));
			*(ptrn + 1) = findnum(*(code[i] + 1));
			*(ptrn + 2) = findnum(*(code[i] + 2));
			*(ptrn + 3) = getnum(*(code[i] + 3));
		}
		numcode.push_back(ptrn);
	}
}

void generatepre()
{
	Predictor *ptrp;
	for(int i = 0; i < num_predictor;i++)
	{
		ptrp = new Predictor;
		ptrp->first = true;
		ptrp->pmode[0] = 0;
		ptrp->pmode[1] = 0;
		ptrp->phistory[0] = 0;
		ptrp->phistory[1] = 0;
		ptrp->phistory[2] = 0;
		ptrp->phistory[3] = 0;
		entry.push_back(ptrp);
	}
	cout << "Total entry: " << entry.size() <<endl << endl;
}

string getstate(int history) 
{
	switch (history)
	{
	case 0:
		return "SN";
	case 1:
		return "WN";
	case 2:
		return "WT";
	case 3:
		return "ST";
	}
}

//===================== output file =========================
char getchar(int a)
{
	switch(a)
	{
	case 0:
		return '0';
	case 1:
		return '1';
	case 2:
		return '2';
	case 3:
		return '3';
	case 4:
		return '4';
	case 5:
		return '5';
	case 6:
		return '6';
	case 7:
		return '7';
	case 8:
		return '8';
	case 9:
		return '9';	
	}
}

void outputmisscount(int predictime, int miss)
{
	char output[] = "pre_outcome_inhance.txt";
	fstream fp;
	fp.open(output, ios::out);
	if (!fp) 
	{
		cout << "Fail to open file: " << output << endl;
	}
	else
	{
		fp << "Total entry: " << num_predictor << endl;
		fp << "Total predict time: " << predictime << endl;
		fp << "Total miss time: " << miss << endl;
	}
	fp.close();
}

void outputthisstate(int now_pred, int now_mode, int pred, int outcome, int miss, Predictor *ptrp, int oldpc)
{
	int temp;
	int count;
	char output[] = "entry";
	char *ctemp;
	char tail[] = ".txt";
	vector<int> vtemp;
	
	while(now_pred >= 0)
	{
		temp = now_pred % 10;
		vtemp.push_back(temp);
		now_pred = now_pred / 10;
		if(now_pred == 0)
		{
			break;
		}
	}
	for(int i = vtemp.size() - 1; i > -1; i--)
	{
		*ctemp = getchar(vtemp[i]);
		strncat(output, ctemp, 1);
	}
	strcat(output, tail);
	
	fstream fp;
	if(ptrp->first)	{fp.open(output, ios::out);}
	else{fp.open(output, ios::app);}
	if (!fp) { cout << "Fail to open file: " << output << endl;}
	else
	{
		if(ptrp->first)
		{
			fp << "	" <<  "State			" << "pred.	" << "outcome	" << "miss?	" << endl;
			ptrp->first = false;
		}
		fp << realpc[oldpc] << "	" << "(" << ptrp->pmode[0] << ptrp->pmode[1] << ",";
		fp << getstate(ptrp->phistory[0]) << "," << getstate(ptrp->phistory[1]) << "," << getstate(ptrp->phistory[3]) << "," << getstate(ptrp->phistory[3]) << ")	";
		if(pred){fp << "T	";}
		else{fp << "N	";}
		if(outcome){fp << "T	";}
		else{fp << "N	";}
		if(miss){fp << "miss!" << endl;}
		else{fp << endl;}	
	}
	fp.close();
}

//===================== get outcome =========================
bool operate()
{		
	switch(*(numcode[pc]))
	{
	case 0:		// add
		regi[*(numcode[pc] + 1)] = regi[*(numcode[pc] + 2)] + regi[*(numcode[pc] + 3)];
		break;
	case 1:		// sub
		regi[*(numcode[pc] + 1)] = regi[*(numcode[pc] + 2)] - regi[*(numcode[pc] + 3)];
		break;
	case 2:		// mul
		regi[*(numcode[pc] + 1)] = regi[*(numcode[pc] + 2)] * regi[*(numcode[pc] + 3)];
		break;
	case 4:		// addi
		regi[*(numcode[pc] + 1)] = regi[*(numcode[pc] + 2)] + *(numcode[pc] + 3);
		break;
	case 5:		// subi
		regi[*(numcode[pc] + 1)] = regi[*(numcode[pc] + 2)] - *(numcode[pc] + 3);
		break;
	case 6:		// muli
		regi[*(numcode[pc] + 1)] = regi[*(numcode[pc] + 2)] * *(numcode[pc] + 3);
		break;
	case 8:		// beq
		if(regi[*(numcode[pc] + 1)] == regi[*(numcode[pc] + 2)])		// taken
		{
			pc = *(numcode[pc] + 3);
			return true;
		}
		break;		// not taken
	case 9:		// bne
		if(regi[*(numcode[pc] + 1)] != regi[*(numcode[pc] + 2)])		// taken
		{
			pc = *(numcode[pc] + 3);
			return true;
		}
		break;		// not taken
	case 10:		// andi1
		regi[*(numcode[pc] + 1)] = regi[*(numcode[pc] + 2)] % 2;
		break;
	case 11:		// andi0
		regi[*(numcode[pc] + 1)] = 0;
		break;
	case 12:
		pc = *(numcode[pc] + 1);
		return true;
	}

	pc++;
	return false;
}

void registerstate()		// print out register state
{
	for (int i = 0; i< 8;i++)
	{
		cout << regi[i] << "  ";
	}
	cout << endl;
}

//====================== predict ===========================

bool predict(Predictor *ptrp,int now)
{
	if(ptrp->phistory[now] < 2)
	{
		return false; 
	}
	else
	{
		return true;
	}
}

bool is_miss(bool pre, bool out)
{	
	if (pre) { cout << "T		"; }
	else cout << "N		";

	if (out) { cout << "T		";}
	else cout << "N		";
	
	if (pre != out) 
	{ 
		cout << "miss!	";
		return true; 
	}
	else
	{	
		cout << '	';
		return false;
	}
}

void fixhistory(int *history) 
{
	switch (*(history))
	{
	case 4:
		*(history) = 3;
		break;
	case -1:
		*(history) = 0;
		break;
	default:
		break;
	}
}

int set2bitcount(Predictor *ptrp, int now_pred)
{
	int now = ptrp->pmode[0] * 2 + ptrp->pmode[1];
		
	cout << realpc[pc] << '\t';
	cout << now_pred << "\t\t";
	cout << "(" << ptrp->pmode[0] << ptrp->pmode[1] << ",";
	cout << getstate(ptrp->phistory[0]) << ",";
	cout << getstate(ptrp->phistory[1]) << ",";
	cout << getstate(ptrp->phistory[2]) << ",";
	cout << getstate(ptrp->phistory[3]) << ")	";
	return now;
}

void fixpredictor(bool outcome, int now_mode, Predictor *ptrp)
{
	if(outcome)
	{
		ptrp->phistory[now_mode]++;
		fixhistory(&ptrp->phistory[now_mode]);
		ptrp->pmode[0] = ptrp->pmode[1];
		ptrp->pmode[1] = 1;
	}
	else
	{
		ptrp->phistory[now_mode]--;
		fixhistory(&ptrp->phistory[now_mode]);
		ptrp->pmode[0] = ptrp->pmode[1];
		ptrp->pmode[1] = 0;
	}
} 

//============================================================

int main(int argc, char **argv) 
{
	//initial  counter
	int predictime = 0;
	int misscount = 0;
	bool miss, pred, outcome;	// true = miss/taken
	int now_mode, now_pred, oldpc;

	readfile();
	transinst();
	generatepre();
	
	cout << "	" << "Predictor	" <<  "State			" << "predict		" << "outcome		" << "miss?	";
	cout << "R0 " << "R1 "<< "R2 "<< "R3 "<< "R4 "<< "R5 "<< "R6 "<< "R7 "<< endl;
	
	while(pc < numcode.size())	//pc < numcode.size()
	{		
		oldpc = pc;
		// choose predictor
		now_pred = pc % num_predictor;
		
		// predict
		now_mode = set2bitcount(entry[now_pred], now_pred);
		pred = predict(entry[now_pred], now_mode);
		predictime++;
		
		// operate
		outcome = operate();
		
		// check miss
		miss = false;
		miss = is_miss(pred, outcome);
		if(miss){misscount++;}
		
		// update this cycle state
		registerstate();
		outputthisstate(now_pred, now_mode, pred, outcome, miss, entry[now_pred], oldpc);
		
		// fix predictor
		fixpredictor(outcome, now_mode, entry[now_pred]);
	}
	cout << "Predict time: " << predictime << endl;
	cout << "Miss time: " << misscount <<endl;
	// output file
	outputmisscount(predictime, misscount);
	
	system("pause");
}
