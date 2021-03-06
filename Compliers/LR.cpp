// in this program 
// we need productions,marks,and goto & action table
// should change  data-structure stored production
// should split terminal and non-terminal marks' map to two part just like Try/test.cpp
// should change many things
// oh, fuck 
// so boring lab-class
#include <cstring>
#include <iostream>
#include <string>
#include <cstdio>
#include <vector>
#include <cmath>
#include <stack>
#include <map>
#include <algorithm>
#include <fstream>
#include "Scanner.cpp"
using namespace std;

#define Acc 0x7fffffff
#define mp make_pair
#define pb push_back
#define inf -Acc
//2 stack 
stack<int> s1;// state stack
stack<int> s2;//input stack


vector<vector<int> > action,go;
// store each ID's number and no-ter's number
map<string,int> Hash;
// totoal number
// action's number ==> edge
int HashCnt,edge;
int Tot;
//all productions
vector<pair<string,string> > production;

//get production from test6.grm and store them in production
inline void GetProduction(){
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
		//cout<<"get "<<first<<" => "<<second<<endl;
		production.pb(mp(first,second));
	}
	in.close();
}
//get production's id
inline int GetProID(const string &first,const string &second){
	int len=production.size();
	for(int i=1;i<len;i++){
		if(first==production[i].first&&second==production[i].second)return i;
	}
	return -inf;

}
//get all keys from LALR1.txt
//store them in Hash
//0..edge 
//edge+1 .. tot
inline void GetKey(ifstream &input){
	string s;
	while(getline(input,s)){
		if(s=="</tr>"){
			if(Hash.find("Yao'")==Hash.end())Hash["Yao'"]=HashCnt++;
			Tot=HashCnt;
			return;
		}
		string tmp="";
		int len=s.length();
		for(int i=11;i<len;i++){
			if(s.substr(i)=="</td>")break;
			tmp+=s[i];
		}
		if(tmp=="$")edge=HashCnt;
		if(Hash.find(tmp)==Hash.end()){
			Hash[tmp]=HashCnt++;
		}
	}
}
// fill the action and goto table
// from LALR1.txt
inline void FillTable(ifstream &input){
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
		}else if(isdigit(s[11])){
			for(int i=11;i<len;i++){
				if(s.substr(i,5)=="</td>")break;
				tmp=tmp*10+s[i]-'0';
			}
			ans.pb(tmp);
		}else if(s.substr(11,6)=="accept"){
			ans.pb(Acc);
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
				}else if(s[st]<0){
					second="";
					break;
				}
				else second+=s[st++];
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
		if(ans.size()>(size_t)edge){
			action.pb(ans);
			ans.clear();
		}
	}
	go.pb(ans);
}
// start Parse program
inline int Parse(){
	GetProduction();
	ifstream input("LALR1.txt");
	string s;
	int cnt=0;
	HashCnt=0;
	Hash.clear();
	for(size_t i=0;i<action.size();i++)action[i].clear();
	for(size_t i=0;i<go.size();i++)go[i].clear();
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
	input.close();
	return 0;
}
//=================begin here============================
//!!!!!!!!!!!!!   do not change again   !!!!!!!!!!!!!!!!!
//before all of this
//should contain production,marks's map,action and goto table
//initilalize 
inline void Init(){
	while(!s1.empty())s1.pop();
	while(!s2.empty())s2.pop();
	s1.push(0);
	s2.push(edge);
	Parse();
}
// output the production used to reduce
inline void PrintProduction(const int &index){
	int len=production[index].second.length();
	cout<<"( "<<index<<" ) "<<production[index].first<<" => "<<(len==0?"ε":production[index].second)<<endl;
}
//get key's id
inline int GetId(const string  &s){
	string tmp="";
	tmp+=s;
	if(Hash.find(tmp)==Hash.end())return -1;
	return Hash[tmp];
}
//get non terminal's id
inline int GetNonTID(const string &s){
	return Hash[s]-edge-1;
}
//error handle
inline void HandelError(){
	cout<<"ERROR"<<endl;
}
//count number in production's right
inline int CountEmp(const string &s){
	int ans=1;
	int len=s.length();
	for(int i=0;i<len;i++){
		if(s[i]==' ')ans++;
	}
	if(s.length()==0)return 0;
	return ans;
}
//main function of parse program
inline void Gao(const vector<int> &s){
	int len=s.size();
	int i=0;
	for(;i<len;){
		int TopState=s1.top();
		int id=s[i];
		if(id<0||id>edge){
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
			int cnt=CountEmp(production[-NextOp].second);
			while(cnt--){
				s1.pop();
				s2.pop();
			}
			TopState=s1.top();
			string spf=production[-NextOp].first;
			int id=GetId(spf);
			s2.push(id);
			id=GetNonTID(spf);
			s1.push(go[TopState][id]);
		}else if(NextOp==Acc){
			cout<<"Accepted"<<endl;
			return;
		}else{
		//	return;
			cout<<"=====error====="<<endl;
			return;
	        id=GetId("semic");
			NextOp=action[TopState][id];
			if(NextOp<0){
				cout<<"need semic here but find other marks"<<endl;
				PrintProduction(-NextOp);
				int cnt=CountEmp(production[-NextOp].second);
				while(cnt--){
					s1.pop(),s2.pop();
				}
				TopState=s1.top();
				string spf=production[-NextOp].first;
				id=GetId(spf);
				s2.push(id);
				id=GetNonTID(spf);
				s1.push(go[TopState][id]);
				s2.push(GetId("semic"));
				TopState=s1.top();
				id=GetId("semic");
				NextOp=action[TopState][id];
				s1.push(NextOp);
				s2.push(id);
				
			}
		}
	}
}

int main(int argc,char *args[]){
	vector<int> Out;
	vector<pair<int,string> > Ans=Scanner(args[1]);
	int len=Ans.size();
	Out.clear();
	for(int i=0;i<len;i++){
		Out.pb(Ans[i].first);
	}
	Init();
	Gao(Out);
	return 0;
}
