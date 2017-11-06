// AVSLR2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "iostream"
#include <string>
#include <math.h>

using namespace std;

int integer_bin_to_dec(string n) {
	int length = n.length();
	int res = 0; 
	for (int i = 0; i < length; i++) {
		bool f = (n[i]=='1');
		if (f) { res += pow(2 , (length - 1 - i)); }
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
string deleniye(char A[], char B[]) {
	char BD[32];
	int k = predvarit_sdvig(A, B);
	for (int i = 0; i < k; i++) { sdvig_left(B); }
	dop_code(B, BD);
	string result;
	for (int i = 0; i < k + 1; i++) {
		char summ[32];
		sum(A, BD, summ);
		if (summ[0] == '1') { sdvig_left(A); result.append("0"); } //если вычитание дало отрицательный результат - то добавили ноль к результату и сдвинули делимое
		else { result.append("1"); for (int i = 0; i < 32; i++) { A[i] = summ[i]; } sdvig_left(A); } //иначе добавили 1 к результату и заменили делимое частичным остатком и сдвинули его. Далее проделаем то же самое для частичного остатка
	}
	return result;
}
int main()
{	
	char A[32] = { '0','0', '0', '0', '0', '0', '0', '0', '0', '1', '1', '1', '1', '1', '0', '1', '0',/**/ '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' };
	char B[32] = { '0','0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '1', '0', '0', '1', '0',/**/ '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' };
	string d = deleniye(A, B);
	cout << d;
	return 0;
}
