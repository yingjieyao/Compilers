#include <cstring>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;
#define pb push_back
#define mp make_pair

vector<pair<string,string> > production;

void Gao(){
	string s;
	while(getline(cin,s)){
		int len=s.length();
		bool find=false;
		string first="",second="";
		for(int i=0;i<len;i++){
			if(s[i]==' ')break;
			first+=s[i];
		}
		for(int i=0;i<len;i++){
			if(s[i]=='='&&s[i+1]=='>'){
				int j=i+3;
				while(j<len&&s[j]!=';'){
					second+=s[j++];
				}
				break;
			}
		}
		cout<<first<<" => "<<second<<endl;
		production.pb(mp(first,second));
	}
}
int main(){
	production.clear();
	freopen("test6.grm","r",stdin);
	string s;
	int cnt=0;
	while(getline(cin,s)){
		if(s[0]=='[')cnt++;
		if(cnt==4){
			Gao();
		}
	}
	return 0;
}
