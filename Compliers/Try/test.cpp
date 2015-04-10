#include <cstring>
#include <map>
#include<iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <cstring>
#include <set>
using namespace std;
#define pb push_back
#define mp make_pair

vector<pair<string,string> > production;
vector<vector<int> > Pro;
map<string,int> Hash;
int keycnt;
int edge;
inline void ini(){
	Hash.clear();
	keycnt=1;
	int len=Pro.size();
	for(int i=0;i<len;i++){
		Pro[i].clear();
	}
}
inline vector<string> split(const string &s){
	vector<string> ans;
	ans.clear();
	int len=s.length();
	string tmp="";
	for(int i=0;i<len;i++){
		if(s[i]==' '){
			ans.pb(tmp);
			tmp="";
		}else {
			tmp+=s[i];
		}
	}
	ans.pb(tmp);
	return ans;
}
// is or not terminal mark
inline bool isTerminal(const int &t){
	if(t<edge)return false;
	return true;
}
// is or not non-terminal mark
inline bool isNonTer(const int &t){
	if(t<edge)return true;
	return false;
}
// get first set
inline set<int> First(const vector<int> &S){
	set<int> ans;
	ans.clear();

}
inline void GetProduction(){
	string s;
	production.clear();
	production.pb(mp("",""));
	ifstream in("test6.grm");
	int st=0;
	while(getline(in,s)){
		if(s[0]=='['){
			if(st==1)edge=keycnt;
			st++;
		}else{
			int size=s.length();
			s=s.substr(0,size-1);
			if(Hash.find(s)==Hash.end()){
				Hash[s]=keycnt++;
			}
			continue;
		}

		if(st==4)break;
	}
	vector<int> tmp;
	tmp.clear();
	// get production
	while(getline(in,s)){
		cout<<s<<endl;
		tmp.clear();
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
		production.pb(mp(first,second));
		tmp.pb(Hash[first]);
		vector<string> T=split(second);
		len=T.size();
		for(int i=0;i<len;i++){
			tmp.pb(Hash[T[i]]);
		}
		Pro.pb(tmp);
	}
	in.close();
}
int main(){
	ini();
	GetProduction();
	return 0;
}
