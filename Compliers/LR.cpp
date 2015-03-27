#include <cstring>
#include <iostream>
#include <string>
#include <cstdio>
#include <vector>
#include <cmath>
#include <stack>
#include <algorithm>
using namespace std;

#define Acc 0x7fffffff
#define mp make_pair
#define pb push_back

// Array size
#define MaxM 8
#define MaxN 3
#define MaxT 2
#define MaxSize 1000

// action table and go table
int action[MaxM][MaxN];
int go[MaxM][MaxT];

//2 stack 
stack<int> s1;// state stack
stack<char> s2;//input stack

// Production here
vector<pair<char,string> > Production;

void Init(){
	while(!s1.empty())s1.pop();
	while(!s2.empty())s2.pop();
	s1.push(0);
	s2.push('#');
	memset(action,0,sizeof(action));
	memset(go,0,sizeof(go));

	action[0][0]=action[2][0]=action[3][0]=3;
	action[0][1]=action[2][1]=action[3][1]=4;
	action[1][2]=Acc;

	action[4][0]=action[4][1]=action[4][2]=-3;
	action[5][0]=action[5][1]=action[5][2]=-1;
	action[6][0]=action[6][1]=action[6][2]=-2;

	go[0][0]=1,go[0][1]=2;
	go[2][1]=5,go[3][1]=6;

	Production.clear();
	Production.pb(mp(' ',""));
	Production.pb(mp('S',"BB"));
	Production.pb(mp('B',"aB"));
	Production.pb(mp('B',"b"));
}

inline void PrintProduction(const int &index){
	cout<<Production[index].first<<"->"<<Production[index].second<<endl;
}

inline int GetId(const char &s){
	switch (s){
		case 'a':return 0;
		case 'b':return 1;
		case '#':return 2;
		default:
			return -1;
	}
}

inline void HandelError(){
	cout<<"ERROR"<<endl;
}

inline void Gao(const string &s){
	int len=s.length();
	int i=0;
	for(;s[i];){
		int TopState=s1.top();
		int id=GetId(s[i]);
		if(id<0||id>2){
			HandelError();
			return;
		}
		int NextOp=action[TopState][id];
		if(NextOp>0&&NextOp!=Acc){
			s2.push(s[i]);
			s1.push(NextOp);
			i++;
			continue;
		}else if(NextOp<0){
			PrintProduction(-NextOp);
			int cnt=Production[-NextOp].second.length();
			while(cnt--){
				s1.pop();
				s2.pop();
			}
			TopState=s1.top();
			char pf=Production[-NextOp].first;
			s2.push(pf);
			s1.push(go[TopState][pf=='S'?0:1]);
		}
		else if(NextOp==Acc){
			cout<<"Accepted"<<endl;
			return;
		}else {
			HandelError();
			return;
		}
	}
}

int main(){
	string s;
	while(cin>>s){
		Init();
		s+="#";
		Gao(s);
	}
	return 0;
}
