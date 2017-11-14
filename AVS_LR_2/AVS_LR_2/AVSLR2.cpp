// AVSLR2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "iostream"
#include <string>
#include <math.h>
#include <fstream>

using namespace std;
double fractional__bin_to_dec(string n)
{
	double res = 0;	
	for (int i = 0; i < n.length(); i++) {
		bool f = (n[i] == '1');
		if (f) { res += (double)(1 / pow(2, (i+1))); }
	}
	return res;
}
double x32_bin_to_dec(char n[]) {//нигде не используется, для проверки корректности функции	inp_in_32
	double res = 0;
	for (int i = 1; i < 16; i++) {
		bool f = (n[i] == '1');
		if (f) { res += pow(2, (16 - i)); }
	}
	for (int i = 17; i < 32; i++) {
		bool f = (n[i] == '1');
		if (f) { res += (double)(1/pow(2, (i-16))); }
	}
	return res;
}
int integer_bin_to_dec(string n) {
	int length = n.length();
	int res = 0; 
	for (int i = 0; i < length; i++) {
		bool f = (n[i]=='1');
		if (f) { res += (int)pow(2 , (length - 1 - i)); }
	}
	return res;
}
string integer_dec_to_bin(int S) {
	if (S != 0)
	{
		bool f = true;
		string l;
		int i = 0;
		char bb[30] = ""; //число соберется в обратном виде
		char chch[30] = ""; //для реверсированной строки
		while (f)
		{
			int a = S / 2;
			int b = S % 2;
			S = a;			
			bb[i] = (b + 48); i++;
			f = (!(a == 0));
		}
		for (int j = 0; j < i; j++) {
			chch[j] = bb[(i - 1) - j];
		}
		l = chch;
		return l;
	}
	else
		return "0";

}
string fractional__dec_to_bin(double S)
{
	string l1;
	if (S != 0.0) {
		while (l1.size()<15) //в нашей разрядной сетке выделено 15 ячеек под дробную часть
		{
			S = S * 2;
			if (S >= 1)
			{
				l1 = l1 + '1';
				S = S - 1; //вычитаем целую часть чтобы дальше работать опять с дробью
			}
			else
				l1 = l1 + '0';
		}
	}
	else
	{
		l1 = l1 + '0';
	}
	return l1;

};
void sum(char A[], char B[],char res[])
{
	bool perenos = false;
	for (int i = 31; i >= 0; i--) {
		if (perenos) {
			if (A[i] == '1') {
				if (B[i] == '1') { res[i] = '1'; perenos = true; }
				else { res[i] = '0'; perenos = true; }
			}
			else {
				if (B[i] == '1') { res[i] = '0'; perenos = true; }
				else { res[i] = '1'; perenos = false; }
			}
		}
		else {
			if (A[i] == '1') {
				if (B[i] == '1') { res[i] = '0'; perenos = true; }
				else { res[i] = '1'; perenos = false; }
			}
			else {
				if (B[i] == '1') { res[i] = '1'; perenos = false; }
				else { res[i] = '0'; perenos = false; }
			}
		}
	}
	//return res;
}
void dop_code(char B[], char res[]) {
	char ress[32];
	for (int i = 0; i < 32; i++) { if (B[i] == '1') ress[i] = '0';
	else ress[i] = '1';
	}
	char e[32]= { '0','0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1' };
	sum(ress, e, res);
}
void sdvig_left(char A[]) {
	for (int i = 1; i < 31; i++) { A[i] = A[i + 1]; }//знак в сдвиге не участвует
	A[31] = '0';
}
void sdvig_right(char A[]) {
	for (int i = 30; i > 0; i--) { A[i+1] = A[i]; }//знак в сдвиге не участвует
}
int predvarit_sdvig(char A[], char B[]) {
	int a = 0, b = 0;
	for (int i = 0; i < 32; i++) {
		if (A[i] != '0') { a = i;break; }
	}
	for (int i = 0; i < 32; i++) {
		if (B[i] != '0') { b = i;  break; }
	}
	return b-a;
}
pair <string, string> deleniye(char A[], char B[]) {	
	pair <string, string> result;
	char BD[32];
	int k = predvarit_sdvig(A, B);
	for (int i = 0; i < k; i++) { sdvig_left(B); }
	dop_code(B, BD);
	string res;
	for (int i = 0; i < k + 1; i++) {
		char summ[32];
		sum(A, BD, summ);
		if (summ[0] == '1') { sdvig_left(A); result.first.append("0"); } //если вычитание дало отрицательный результат - то добавили ноль к результату и сдвинули делимое
		else { result.first.append("1"); for (int i = 0; i < 32; i++) { A[i] = summ[i]; } sdvig_left(A); } //иначе добавили 1 к результату и заменили делимое частичным остатком и сдвинули его. Далее проделаем то же самое для частичного остатка
		 /*if (i == k) {//c остатком какие-то глюки, ошибается ровно на единицу, я пока не разобрался.
				bool f = true;
			for (int i = 0; i < 32; i++) { if (!(A[i] == '0')) f = false; }//если частичный остаток нулевой - то добавляем в остаток NIL
			if (f) { result.second = "NIL"; }
			else {
				for (int z = 0; z < k + 1; z++) { sdvig_right(A); }
				
				result.second = to_string(x32_bin_to_dec(A)); }*/
		}
	for (int i = 0; i < 15; i++) {//рассчет дробной части результата
		char summ[32];
		sum(A, BD, summ);
		if (summ[0] == '1') { sdvig_left(A); result.second.append("0"); } //если вычитание дало отрицательный результат - то добавили ноль к результату и сдвинули делимое
		else { result.second.append("1"); for (int i = 0; i < 32; i++) { A[i] = summ[i]; } sdvig_left(A); }
	}
	
	return result;
}
int string_checker(string n)
{
	int i = 0;
	int indification = 0;

	if (n == "")//пустая строка
	{
		indification = 3;
		return indification;
	}

	if ((n[i]<'0' || n[i]>'9') && n[i] != '-') //первый символ не число и не минус
	{

		indification = 2;//ошибки в строке
		return indification;
	}
	i++;


	while (n[i] != '\0')
	{
		if (n[i]<'0' || n[i]>'9')
			if (n[i] == '.'&& indification == 0)
				indification = 1;//дробное число
			else {
				indification = 2;//ошибки
				return indification;
			}
			i++;
	}
	return indification;//если 0 - то число целое (+или-)
}
void inp_in_32(string n, char b[32]) {//вставляем в массив (32 ячейки) 1-знак(только для операций),16-цел,15-дробн
	for (int i = 0; i < 32; i++) { b[i] = '0'; }//инициализируем выходной массив
	if (string_checker(n) == 0) {//если целое
		string binary = integer_dec_to_bin(abs(stoi(n)));//переводим в двоичный код число
		for (unsigned int i = 0; i < binary.length(); i++) { b[16 - i] = binary[binary.length() - 1 - i]; }
	}
	else {
		double f = atof(n.c_str());
		string int_binary = integer_dec_to_bin(abs((int)f));
		string fractional_binary = fractional__dec_to_bin(abs(f - (int)f));
		for (unsigned int i = 0; i < int_binary.length(); i++) { b[16 - i] = int_binary[int_binary.length() - 1 - i]; }
		for (unsigned int i = 0; i < fractional_binary.length(); i++) { b[17 + i] = fractional_binary[i]; }
	}
}
bool is_minus(string n, string m) {
	bool f = (n[0]=='-');
	bool g = (m[0] == '-');
	return f^g;
}
string shaping_result_string(int a, double b, bool minus) {
	string result;
	if (minus)  result += '-';
	result += to_string(a);
	if (b == 0.0) { return result; }
	else {
		result += '.';
		int poww = to_string(b).length() - 2;
		b = b*pow(10, poww);
		for (int i = 0; i < poww; i++) {
			result += to_string(b)[i];
		}

		return result;
	}
}
int main()
{
	
	//setlocale(LC_ALL, "rus");
	//char A[32] = { '0','0', '0', '0', '0', '0', '0', '0', '0', '1', '1', '1', '1', '1', '0', '1', '0',/*17*/ '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' };
	//char B[32] = { '0','0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '1', '0', '0', '1', '0',/**/ '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' };
	pair <string, string> result;
	
	/*______________________________________________________________________________________________________________________________________________________________________________*/
	ifstream fin("input.txt");
	ifstream finn("output.txt");
	ofstream fout("output.txt");
	char k = 9;
	cout << "To view the commands enter: 1" << endl;
	cout << endl << "Enter the command" << endl;
	while (k != '0')
	{
		cin >> k;
		switch (k) {
		case '0': {			
			break;
				}
		case '1': {
			cout << endl;
			cout << "0:Exit" << endl;
			cout << "1:Command list" << endl;
			cout << "2:Divide the numbers in the file and write the answer in the file" << endl;
			cout << "3:Divide the numbers entered in the console" << endl;
			cout << "4:Show output file" << endl;
			cout << endl << "Enter the command:" << endl;
			break;
		}
		case '2': {
			string n;
			fin >> n;
			cout << n << endl; break;
			/*while (!fin.eof()) {
				string n;
				fin >> n;
				switch (string_checker(n)) {
				case 2:
					{
						fout << n << "   " << "invalid input format of the first number" << endl;	//Сообщение об ошибке записи преобразованного числа в файл	
						break;
					}
				case 3:break;
				default: {
					string m;
					fin >> m;
					switch (string_checker(m)) {
					case 2:
					{
						fout << m << "   " << "invalid input format of the second number" << endl;	//Сообщение об ошибке записи преобразованного числа в файл	
						break;
					}
					case 3:break;
					default: {
						char A[32], B[32];
						inp_in_32(n, A);
						inp_in_32(m, B);
						result = deleniye(A, B);
						int a = integer_bin_to_dec(result.first);
						double b = fractional__bin_to_dec(result.second);
						string resulat = shaping_result_string(a, b, is_minus(n, m));
						fout << n <<" : "<<m<<" = "<< resulat << endl;
						cout << "the results of the calculations are written to the file" << endl;
						break;
					}
					}
				}

				}
			}
			break;*/
		}
		case '3': {
			string n,m;
			char A[32], B[32];
			cout << "Dividend: " << endl;
			cin >> n;
			if (string_checker(n) == 2) {cout<<"Invalid number format, try again"<<endl; break;}
			cout << "Divider:  " << endl;
			cin >> m;
			if (string_checker(m) == 2) { cout << "Invalid number format, try again" << endl; break; }
			if (stod(m) == 0) { cout << "Error: division by zero" << endl; break; }
			inp_in_32(n,A);				
			inp_in_32(m, B);					
			result = deleniye(A, B);
			int a = integer_bin_to_dec(result.first);
			double b = fractional__bin_to_dec(result.second);
			string resulat = shaping_result_string(a, b, is_minus(n,m));
			cout << resulat << endl;
			
		}
		}
	}

	fin.close();
	finn.close();
	fout.close();
	
	return 0;
}
