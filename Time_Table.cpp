#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <windows.h>
using namespace std;
vector<string> subN;
unordered_map<string, short> wday;
unordered_map<string, short> timeT;
int chc = 0;
enum {
	BLACK,DARK_BLUE,DARK_GREEN,DARK_SKYBLUE,DARK_RED,DARK_VOILET,DAKR_YELLOW,GRAY,DARK_GRAY,BLUE,GREEN,SKYBLUE,RED,VIOLET,YELLOW,WHITE,
};
void setColor(unsigned short text) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), text);
}
vector<string> split(string s, string di)
{
	vector<string> v;
	int start = 0, d = (int)s.find(di);
	while (d != string::npos)
	{
		v.push_back(s.substr(start, d - start));
		start = d + (int)di.length();
		d = (int)s.find(di, start);
	}
	v.push_back(s.substr(start, d - start));
	for (int i = 0; i < v.size(); ++i)
		v[i].erase(remove(v[i].begin(), v[i].end(), ' '), v[i].end()); //공백 제거
	return v;
}
short getA(char c)
{
	int k = (int)c - 'A' + 1;
	return (short)k * 3 - 1;
}
short getG(char c)
{
	int k = (int)c - '0';
	return (short)k * 2;
}
vector<vector<short>> exF(string s)
{
	vector<string> t = split(s, ")"); //@화13:30~15:00(팔409    수7(팔328    @목E(성203    @월8.5(연암105
	vector<vector<short>> F(32, vector<short>(6, 0));
	for (int i = 0; i < t.size(); ++i)
	{
		if (t[i].length() < 3)	continue;
		string day = t[i].substr(0, 2);
		short dayD = wday[day] - 1;
		t[i] = t[i].substr(2);
		if ('A' <= t[i][0] && t[i][0] <= 'Z') //알파벳 타입
		{
			if (t[i][0] == 'Z')
			{
				F[0][dayD] = chc;
				F[1][dayD] = chc;
			}
			else
			{
				short start = getA(t[i][0]);
				F[start][dayD] = chc;
				F[start + 1][dayD] = chc;
				F[start + 2][dayD] = chc;
			}
		}
		else if (t[i].find("~") != -1) //시간 범위 타입
		{
			string s = t[i].substr(0, 5);
			string e = t[i].substr(6, 5);
			//cout << s << ' ' << e << '\n';
			for (short j = timeT[s]; j < timeT[e]; ++j)
				F[j][dayD] = chc;
		}
		else
		{
			string a;
			bool h = false;
			vector<string> nt = split(t[i], "(");
			double t1 = stod(nt[0]) + 8;
			if (t1 != floor(t1))	h = true;
			if (t1 < 10)	a += '0';
			a += to_string((int)floor(t1));
			a += ':';
			h ? a += "30" : a += "00";
			short start = timeT[a];
			F[start][dayD] = chc;
			F[start + 1][dayD] = chc;
		}
	}
	return F;
}
bool printF(vector<short>& selec, vector<vector<vector<vector<short>>>>& fi, bool pro)
{
	vector<vector<short>> print(32, vector<short>(6, 0));
	int i, j, l;
	for (i = 0; i < fi.size(); ++i)
		for (j = 0; j < 32; ++j)
			for (l = 0; l < 6; ++l)
			{
				if (print[j][l] && fi[i][selec[i]][j][l])
				{
					if (pro)	printf("겹침!!!!!!!!!!!!!!!\n\n");
					return false;
				}
				print[j][l] += fi[i][selec[i]][j][l]; //시간표 합치기
			}
	if (!pro)	return true;
	printf("         월 화 수 목 금 토\n");
	for (i = 0; i < 32; ++i)
	{
		if (i / 2 + 8 == 12 && !(i & 1))	printf("-------------------------\n");
		if (i / 2 + 8 == 17 && !(i & 1))	printf("-------------------------\n");
		printf("%02d:%02d    ", i / 2 + 8, i & 1 ? 30 : 0);
		for (j = 0; j < 6; ++j)
		{
			if (print[i][j])	printf("%d%d ", print[i][j], print[i][j]);
			else
			{
				setColor(DARK_GRAY);
				printf("|  ");
				setColor(WHITE);
			}
		}
		printf("\n");
	}
	printf("\n과목 :     ");
	for (i = 0; i < subN.size(); ++i)
		cout << subN[i] << '(' << i + 1 << ") ";
	printf("\n선택 옵션 : ");
	for (i = 0; i < selec.size(); ++i)
		printf("%d(%d)   ", selec[i] + 1, (int)fi[i].size());
	printf("\n\n");
	return false;
}
void make(int n, vector<short>& sel, vector<vector<vector<vector<short>>>>& sub)
{
	if (n < sel.size())
		for (int i = 0; i < sub[n].size(); ++i)
		{
			sel[n] = i;
			make(n + 1, sel, sub);
		}
	else if (printF(sel, sub, false))
	{
		for (int i = 0; i < sel.size(); ++i)
			printf("%d ", sel[i] + 1);
		printf("\n");
	}
	return;
}
int main()
{
	wday.insert({ "월",1 });		wday.insert({ "화",2 });		wday.insert({ "수",3 });		wday.insert({ "목",4 });		wday.insert({ "금",5 });		wday.insert({ "토",6 });

	for (int i = 8; i <= 23; ++i)
	{
		string a;
		if (i < 10)	a += '0';
		a += to_string(i);
		string t1 = a + ":00";
		string t2 = a + ":30";
		timeT.insert({ t1,(i - 8) * 2 });
		timeT.insert({ t2,(i - 8) * 2 + 1 });
	}

	ifstream file("Time_Table_Data.txt");
	string dac;
	if (!file.is_open())
	{
		ofstream WriteFile("Time_Table_Data.txt");
		WriteFile << "<<((예시))>>\n//시간표는 요일만 가능, ':' 문자 반드시 써야됨, 공백 무시됨, 오타 주의\n//주소에 (&),(\")이 있으면 앞에 \\ 붙힐 것.\n\n>>과목\n수학1 :\n@(수강신청 사이트 데이터 그대로 복붙)\n@(여러개 해도 되고 1개 해도 됨)\n*****\n수학2 :\n@(마찬가지)\n\n\n########## 이제부터 본문 작성--------------------------------------------------\n>>과목\n";
		cout << "################\n#\n# 이 프로그램이 있는 위치에 Time_Table_Data.txt 이라는 텍스트 파일이 생성되었습니다.\n#\n# 예시처럼 과목과 시간을 입력해주세요.\n#\n################\n\n\n\n";
		return 0;
	}

	int i, j;

	stringstream ss;
	ss << file.rdbuf();
	string t = ss.str();
	t = t.substr(t.find("##########") + 86); //이러면 t는 본문 내용만 // 딱 >>시간표\n뒤부터

	vector<string> subD = split(t, "*****");

	for (i = 0; i < subD.size(); ++i)
		subN.push_back(subD[i].substr(1, subD[i].find(":") - 1)); //과목명 sub에 입력

	vector<vector<vector<vector<short>>>> sub(subN.size()); //field 1개가 한 과목의 하나의 옵션

	for (i = 0; i < subN.size(); ++i)
	{
		vector<string> vt = split(subD[i], "@"); //과목별 시간표 전체 한 줄씩
		chc++;
		for (j = 1; j < vt.size(); ++j)
			sub[i].push_back(exF(vt[j]));
	} //과목 필드 생성 끝

	vector<short> selection(subN.size(), 0);
	printf("##################\n가능한 시간 옵션들 : \n\n");
	make(0, selection, sub);
	printf("##################\n\n입력 :\n");
	while (1)
	{
		for (i = 0; i < selection.size(); ++i)
		{
			int td;
			scanf("%d", &td);
			td--;
			if (td < 0 || td >= sub[i].size())	return 0;
			selection[i] = td;
		}
		printF(selection, sub, true);
	}
	
	return 0;
}