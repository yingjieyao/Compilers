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
using namespace std;
#define acc 0x7fffffff
#define pb push_back
#define mp make_pair
#define inf acc
//action and goto table
vector<vector<int> > action,go;
// store each ID's number and no-ter's number
map<string,int> Hash;
// totoal number
// action's number ==> edge
int HashCnt,edge;
int Tot;
//all productions
vector<pair<string,string> > production;

void GetProduction(){
	string s;
	production.clear();
	production.pb(mp("",""));
	ifstream in("test6.grm");
	int st=0;
	while(getline(in,s)){
		if(s[0]=='[')st++;
		if(st==4)break;
	}
	while(getline(in,s)){
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
	}
	in.close();
}

int GetProID(string first,string second){
	int len=production.size();
	for(int i=1;i<len;i++){
		if(first==production[i].first&&second==production[i].second)return i;
	}
	return -inf;

}
void GetKey(ifstream &input){
	string s;
	while(getline(input,s)){
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

void FillTable(ifstream &input){
	string s;
	vector<int> ans;
	ans.clear();
	getline(input,s);
	while(getline(input,s)){
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
			while(st<len-5){
				if(s[st]=='&'&&s[st+1]=='n'&&s[st+2]=='b'){
					second+=" ";
					st+=6;
				}else second+=s[st++];
			}
			int ret=GetProID(first,second);
			if(ret==-inf){
				cout<<s<<endl;
				cout<<"Production  "<<first<<" => "<<second<<endl;
				cout<<"ERROR"<<endl;
				return;
			}
			else ans.pb(-ret);
		}else ans.pb(0);
		if(ans.size()>edge){
			action.pb(ans);
			ans.clear();
		}
	}
	go.pb(ans);
}
int main(){
	GetProduction();
	ifstream input("LALR1.txt");
	string s;
	int cnt=0;
	HashCnt=0;
	Hash.clear();
	while(getline(input,s)){
		if(s=="</table>")break;
		if(s=="<tr>")
			cnt++;
		if(cnt==2){
			GetKey(input);
		}else if(cnt>2){
			FillTable(input);
		}
	}
	ofstream output("out.txt");
	int size=action.size();
	for(int i=0;i<size;i++){
		for(int j=0;j<action[i].size();j++){
			output<<action[i][j]<<' ';
		}
		output<<endl;
	}
	output.close();
	input.close();
	return 0;
}
