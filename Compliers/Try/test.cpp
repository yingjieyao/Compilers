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
vector<pair<string,vector<string> > > production;
vector<pair<string,vector<string> > > projects;
map<string,int> Hash;
vector<set<int> > FIRST;
vector<vector<int> > action,go;
int keycnt;
int edge;
inline void ini(){
	Hash.clear();
	keycnt=1;
	action.clear();
	go.clear();
	int len=FIRST.size();
	for(int i=0;i<len;i++){
		FIRST[i].clear();
	}
}
 // split string by space 
 // and return the vector
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
inline void GetProduction(){/*{{{*/
	string s;
	production.clear();
	vector<string> T;
	T.clear();
	production.pb(mp("",T));
	ifstream in("LR.txt");
	getline(in,s);
	T=split(s);
	size_t len=T.size();
	for(size_t i=0;i<len;i++){
		Hash[T[i]]=keycnt++;
	}
	edge=len;
	getline(in,s);
	len=T.size();
	for(size_t i=0;i<len;i++){
		Hash[T[i]]=keycnt++;
	}
	// get production
	while(getline(in,s)){
		//cout<<s<<endl;
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
}/*}}}*/
//get all the projects
void GetProjects(){
	size_t len=production.size();
	for(size_t i=1;i<len;i++){
		string left=production[i].first;
		vector<string> ans=(production[i].second);
		size_t size=ans.size();
		
		vector<string> newright;
		newright.clear();
		newright.pb(".");
		for(size_t j=0;j<size;j++){
			newright.pb(ans[j]);
		}
		projects.pb(mp(left,newright));
		newright.clear();

		vector<string> tmp;
		tmp.clear();
		for(size_t j=0;j<size;j++){
			tmp.clear();

			for(size_t t=0;t<=j;t++)tmp.pb(ans[t]);
			tmp.pb(".");
			
			for(size_t t=j+1;t<size;t++)tmp.pb(ans[t]);
			
			projects.pb(mp(left,tmp));
		}
	}
}
void PrintProjects(){
	size_t len=projects.size();
	for(size_t i=0;i<len;i++){
		cout<<projects[i].first<<' ';
	
		int len1=projects[i].second.size();
		for(int j=0;j<len1;j++){
			cout<<projects[i].second[j]<<' ';
		}
		
		cout<<endl;
	}
}
//
void PrintProduction(){/*{{{*//*{{{*/
	size_t len=production.size();
	for(size_t i=0;i<len;i++){
		cout<<production[i].first<<" => ";
		int len2=production[i].second.size();
		for(int j=0;j<len2;j++){
			cout<<production[i].second[j]<<" ";
		}
		cout<<endl;
	}
}/*}}}*//*}}}*/
//
int GetIndexOfDot(vector<string> s){
	int len=s.size();
	for(int i=0;i<len;i++){
		if(s[i]==".")return i;
	}
	return -1;
}
bool IsTerminal(string s){
	return Hash[s]>edge;
}
bool NotIn(pair<string,vector<string> > pro,vector<pair<string,vector<string> > > ret){
	size_t size=ret.size();
	for(size_t i=0;i<size;i++){
		if(ret[i].first==pro.first){
			size_t size1=pro.second.size();
			size_t size2=ret[i].second.size();
			bool same=true;
			if(size1==size2){
				for(size_t t=0;t<size1;t++){
					if(pro.second[t]!=ret[i].second[t]){
						same=false;
						break;
					}
				}
			}
			if(same)return false;
		}
	}
	return true;
}
#define SET vector<pair<string,vector<string> > >
vector<pair<string,vector<string> > > CLOSURE (SET I){
	vector<pair<string,vector<string> > > ret;
	ret.clear();
	size_t st=I.size();
	for(size_t i=0;i<st;i++){
		ret.pb(I[i]);
	}
	st=0;
	int len=projects.size();
	bool change=false;
	while(st<ret.size()||change){
		change=false;
		int dot=GetIndexOfDot(ret[st].second);
		if(dot==(int)ret[st].second.size()-1){
			st++;
			continue;
		}
		string next=ret[st].second[dot+1];
		if(IsTerminal(next)){
			st++;
			continue;
		}
		for(int i=0;i<len;i++){
			if(projects[i].first==next){
				if(NotIn(projects[i],ret)){
					ret.pb(projects[i]);
					change=true;
				}
			}
		}
		st++;
	}
	return ret;
}
SET GO(SET I,string X){
	SET ans;
	ans.clear();
	size_t size=I.size();
	for(size_t i=0;i<size;i++){
		size_t len=I[i].second.size();
		size_t dot=GetIndexOfDot(I[i].second);
		if(dot==len-1){
			continue;
		}
		vector<string> newright;
		newright.clear();
		for(size_t j=0;j<dot;j++){
			newright.pb(I[i].second[j]);
		}
		newright.pb(I[i].second[dot+1]);
		newright.pb(".");
		for(size_t j=dot+2;j<size;j++){
			newright.pb(I[i].second[j]);
		}
		
		ans.pb(mp(I[i].first,newright));
	}
	return CLOSURE(ans);

}
vector<SET> Union;
bool AlreadyIn(SET I){
	size_t len=Union.size();
	size_t size2=I.size();
	for(size_t i=0;i<len;i++){
		bool same=true;
		size_t size1=Union[i].size();
		if(size1!=size2)continue;
		for(size_t j=0;j<size1;j++){
			if(Union[i][j]!=I[j]){
				same=false;
				break;
			}
		}
		if(same)return true;
	}
	return false;
}
void GetDFA(){
	Union.clear();
	SET start;
	start.clear();
	vector<string> righttmp;
	righttmp.clear();
	righttmp.pb(".");
	righttmp.pb("Yao");
	start.pb(mp("Yao'",righttmp));
	start=CLOSURE(start);
	Union.pb(start);
	//
}
//test CLOSURE function
void test(){
	pair<string,vector<string> > I;
	I.first="Yao'";
	I.second.clear();
	I.second.pb(".");
	I.second.pb("Yao");
	SET S;
	S.clear();
	S.pb(I);
	vector<pair<string,vector<string> > > ret=GO(S,"Yao");
	size_t size=ret.size();
	cout<<"start"<<endl;
	for(size_t i=0;i<size;i++){
		cout<<ret[i].first<<" => ";
		size_t t=ret[i].second.size();
		for(size_t j=0;j<t;j++){
			cout<<ret[i].second[j]<<' ';
		}
		cout<<endl;
	}
}
int main(){
	ini();
	GetProduction();
	GetProjects();
///	PrintProjects();
	test();
//PrintProduction();
	return 0;
}
