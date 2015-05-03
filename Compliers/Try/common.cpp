#include <cstring>
#include <map>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <cstring>
#include <set>
using namespace std;
#define pb push_back
#define mp make_pair
#define ACC 0x7fffffff

vector<pair<string,vector<string> > > production;
map<string,int> Hash;
map<string,int> Terminal;
map<string,int> NonTerminal;
vector<vector<int> > action,go;
map<string,vector<string> > FIRST,FOLLOW;
//the file which stores the productoions
int tercnt;
int nontercnt;

inline void Ini(){
	Terminal.clear();
	NonTerminal.clear();
	FIRST.clear();
	FOLLOW.clear();
	action.clear();
	go.clear();
}

inline bool IsTerminal(string s){
	return Terminal.find(s)!=Terminal.end();
}
//get first set
inline void GetFirst(const string & s){
	vector<string> tmp;
	if(FIRST.find(s)!=FIRST.end())return;
	tmp.clear();
//  if s is a terminal mark
	if(IsTerminal(s)){
		tmp.pb(s);
		FIRST[s]=tmp;
		return;
	}
	bool HasNull=false;
	size_t SizeOfPro=production.size();
	for(size_t i=1;i<SizeOfPro;i++){
		pair<string,vector<string> > pro=production[i];
		if(pro.first!=s)continue;
		if(pro.second.size()==0){
			HasNull=true;
			continue;
		}
		string RightFirst=pro.second[0];
		size_t RightSize=pro.second.size();
		if(RightFirst=="")HasNull=true;
		else if(IsTerminal(RightFirst)){
			tmp.pb(RightFirst);
		}else{
			size_t st=0;
			bool HasE=true;
			while(HasE&&st<RightSize){
				HasE=false;
				string Right=pro.second[st];
				if(Right==s){
					break;
				}
				if(FIRST.find(Right)==FIRST.end())
					GetFirst(Right);
				vector<string> Rfirst=FIRST[Right];
				size_t RfirstSize=Rfirst.size();
				for(size_t R=0;R<RfirstSize;R++){
					if(Rfirst[R]!=""){
						tmp.pb(Rfirst[R]);
					}else HasE=true;
				}
				if(st==RightSize){
					if(HasE){
						tmp.pb("");
					}
				}
				st++;			
			}
		
		}
	}
	if(HasNull)tmp.pb("");
	sort(tmp.begin(),tmp.end());
	tmp.erase(unique(tmp.begin(),tmp.end()),tmp.end());
	FIRST[s]=tmp;
}
inline void GetFirst(){
	FIRST.clear();
	map<string,int>::iterator it;
	for(it=Terminal.begin();it!=Terminal.end();it++){
		GetFirst(it->first);
	}
	for(it=NonTerminal.begin();it!=NonTerminal.end();it++){
		GetFirst(it->first);
	}
}
inline vector<string> GetFirst(vector<string> s){
	vector<string> ans;
	ans.clear();
	if(IsTerminal(s[0])){
		ans.pb(s[0]);
	}else ans=FIRST[s[0]];
	return ans;
}
// need refer to some papers to complete these
inline void GetFollow(const string &s){
	if(FOLLOW.find(s)!=FOLLOW.end())return;

}
inline void GetFollow(){
	FOLLOW.clear();
	map<string,int>::iterator it;
	for(it=NonTerminal.begin();it!=NonTerminal.end();it++){
		GetFollow(it->first);
	}
}
 // split string by space and return the vector
inline vector<string> split(const string &s){
	vector<string> ans;
	ans.clear();
	int len=s.length();
	string tmp="";
	for(int i=0;i<len;i++){
		if(s[i]==' '){
			if(tmp.size())
				ans.pb(tmp);
			tmp="";
		}else {
			tmp+=s[i];
		}
	}
	if(tmp.length())ans.pb(tmp);
	return ans;
}
// just for read all the productions from files
inline void GetProduction(){
	string s;
	production.clear();
	vector<string> T;
	T.clear();
	production.pb(mp("",T));
	ifstream in("LR.txt");
// the first line , terminal marks
	getline(in,s);
	T=split(s);
	size_t len=T.size();
	string tmp=T[len-1];
	int length=tmp.length();
	tmp=tmp.substr(0,length-1);
	T[len-1]=tmp;
	for(size_t i=0;i<len;i++){
		Terminal[T[i]]=tercnt++;
	}
// the second line, non-terminal marks
	getline(in,s);
	T=split(s);
	len=T.size();
	tmp=T[len-1];
	length=tmp.length();
	tmp=tmp.substr(0,length-1);
	T[len-1]=tmp;
	for(size_t i=0;i<len;i++){
		cout<<T[i]<<endl;
		NonTerminal[T[i]]=nontercnt++;
	}
//  productions here
	while(getline(in,s)){
		int len=s.length();
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
		vector<string> right=split(second);
		production.pb(mp(first,right));
	}
	in.close();
	cout<<"complete read file!"<<endl;
}
//
inline void PrintProduction(){
	size_t len=production.size();
	for(size_t i=1;i<len;i++){
		cout<<production[i].first<<" => ";
		int len2=production[i].second.size();
		for(int j=0;j<len2;j++){
			cout<<production[i].second[j]<<" ";
		}
		cout<<endl;
	}
}
//
inline int GetIndexOfDot(vector<string> s){
	int len=s.size();
	for(int i=0;i<len;i++){
		if(s[i]==".")return i;
	}
	return -1;
}

int Main(){
	Ini();
	GetProduction();
	GetFirst();
	vector<string> tmp=FIRST["L"];
	size_t size=tmp.size();
	for(size_t i=0;i<size;i++){
		cout<<tmp[i]<<' '<<tmp[i].length()<<endl;
	}
	return 0;
}
