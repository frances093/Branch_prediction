#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

void printstate(int history) 
{
	switch (history)
	{
	case 0:
		cout << "SN";
		break;
	case 1:
		cout << "WN";
		break;
	case 2:
		cout << "WT";
		break;
	case 3:
		cout << "ST";
		break;
	}
}

bool printout(int *mode, int *p, int predict, bool outcome, bool miss) 
{
	cout << "(" << mode[0] << mode[1] << ",";
	printstate(*p);
	cout << ",";
	printstate(*(p+1));
	cout << ",";
	printstate(*(p+2));
	cout << ",";
	printstate(*(p+3));
	cout << ")	";
	
	if ((predict - 2) < 0) { cout << "N	"; }
	else cout << "T	";

	if (outcome) { cout << "T		";}
	else cout << "N		";
	
	if (miss) { cout << "miss!"; }
	
	cout << endl;

	return miss;
}

bool predictor(int predict, int outcome)
{
	if (predict < 2 && outcome == 0) { return false; }
	else if (predict > 1 && outcome == 1 ){return false; }
	else { return true; }
}

int fixhistory(int history) 
{
	switch (history)
	{
	case 4:
		history = 3;
		break;
	case -1:
		history = 0;
		break;
	default:
		break;
	}
	return history;
}

string readfile() 
{
	ifstream file;
	string line;
	file.open("pre_basic.txt", ios::in);
	if (file.is_open())
	{
		getline(file, line);
		file.close();
	}
	else
	{
		cout << "Unable to open the file!" << endl;
	}
	return line;
}

int main(int argc, char **argv) 
{
	// initial predicter
	int mode[2] = { 0, 0 };
	int now = 0;
	int history[4] = { 0, 0, 0, 0 };
	
	//initial  counter
	int predictime = 0;
	int miss = 0;

	string NTonly = readfile();

	cout << "State			" << "pred.	" << "outcome		" << "miss?" << endl;

	for (int i = 0; i < NTonly.size() ; i++)
	{
		now = mode[0] * 2 + mode[1];
		predictime++;
		// predict
		if (NTonly[i] == 78) 		// N
		{
			if (printout(mode, history, history[now], 0, predictor(history[now], 0))) { miss++; }
			history[now]--;
			history[now] = fixhistory(history[now]);
			mode[0] = mode[1];
			mode[1] = 0;
		}
		else if(NTonly[i] == 84)
		{
			if (printout(mode, history, history[now], 1, predictor(history[now], 1))) { miss++; }
			history[now]++;
			history[now] = fixhistory(history[now]);
			mode[0] = mode[1];
			mode[1] = 1;
		}
		else
		{
			cout << "Including unvalid input!" << endl;
			break;
		}

	}

	cout << endl;
	cout << "Total predict time: " << predictime << endl;
	cout << "Total miss time: " << miss << endl;

	char output[] = "pre_output_basic.txt";
	fstream fp;
	fp.open(output, ios::out);
	if (!fp) {
		cout << "Fail to open file: " << output << endl;
	}
	fp << "Total predict time: " << predictime << endl;
	fp << "Total miss time: " << miss << endl;

	fp.close();
	system("pause");
}
