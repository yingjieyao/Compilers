#include <cstring>
#include <cstdio>
#include <string>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <cstring>
#include <vector>
#include <iostream>
#include <map>
#include <fstream>
#include "tmp.cpp"
using namespace std;
#define acc 0x7fffffff
#define pb push_back
#define mp make_pair
vector<vector<int> > Return;
map<string,int> Hash;
int HashCnt,edge;
int Tot;


int find(string first,string second){
	return 0;
	
}
void Handle(){
	string s;
	while(getline(cin,s)){
		if(s=="</tr>")return;
		string tmp="";
		int len=s.length();
		for(int i=11;i<len;i++){
			if(s.substr(i)=="</td>")break;
			tmp+=s[i];
		}
		if(tmp=="$")edge=HashCnt;
		if(Hash.find(tmp)==Hash.end())Hash[tmp]=HashCnt++;
	}
	Tot=HashCnt;
}

void FillTable(){
	string s;
	vector<int> ans;
	ans.clear();
	getline(cin,s);
	while(getline(cin,s)){
		int tmp=0;
		int len=s.length();
		if(s=="</tr>")break;
		if(s.substr(11,6)=="&nbsp;")ans.pb(0);
		else if(s.substr(11,5)=="shift"){
			for(int i=22;i<len;i++){
				if(s.substr(i,5)=="</td>"){
					break;
				}
				tmp=tmp*10+s[i]-'0';
			}
			ans.pb(tmp);
		}
		else if(isdigit(s[11])){
			for(int i=11;i<len;i++){
				if(s.substr(i,5)=="</td>")break;
				tmp=tmp*10+s[i]-'0';
			}
			ans.pb(tmp);
		}else if(s.substr(11,6)=="accept"){
			ans.pb(acc);
		}else if(s.substr(11,6)=="reduce"){
			int st=23;
			string first="",second="";
			while(st<len&&s[st]!='&'){
				first+=s[st++];
			}
			st+=14;
			while(st<len&&s[st]!='<'&&s[st+1]!='/'){
				if(s[st]=='&'&&s[st+1]=='n'&&s[st+2]=='b'){
					second+=" ";
					st+=6;
				}else second+=s[st++];
			}
			cout<<"Production  "<<first<<" => "<<second<<endl;
			int ret=find(first,second);
			ans.pb(ret);
		}else ans.pb(0);
	}
	Return.pb(ans);
}

int main(){
	//freopen("out.txt","w",stdout);
	freopen("LALR1.txt","r",stdin);
	string s;
	int cnt=0;
	HashCnt=0;
	Return.clear();
	Hash.clear();
	while(getline(cin,s)){
		if(s=="</table>")break;
		if(s=="<tr>"){
			cnt++;
		}
		if(cnt==2){
			Handle();
		}else if(cnt>2){
			FillTable();
		}
	}
	return 0;
}
/*
 *must tran the htm file to txt to delete \r 
	need to tran in production first
 * */

