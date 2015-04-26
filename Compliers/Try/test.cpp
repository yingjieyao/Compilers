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
#define SET vector<pair<string,vector<string> > >

vector<pair<string,vector<string> > > production;
vector<pair<string,vector<string> > > projects;
map<string,int> Hash;
map<string,int> Terminal;
map<string,int> NonTerminal;
vector<vector<int> > action,go;
vector<SET> Union;
map<string,vector<string> > FIRST,FOLLOW;
//the file which stores the productoions
string file="LR1.txt";
int tercnt;
int nontercnt;

inline void Ini(){
	Terminal.clear();
	NonTerminal.clear();
	FIRST.clear();
	FOLLOW.clear();
	//
	action.clear();
	go.clear();
	//
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
	for(size_t i=0;i<SizeOfPro;i++){
		pair<string,vector<string> > pro=production[i];
		if(pro.first!=s)continue;
		string RightFirst=pro.second[0];
		size_t RightSize=pro.second.size();
		if(RightFirst=="")HasNull=true;
		else if(IsTerminal(RightFirst)){
			tmp.pb(RightFirst);
		}else{
			size_t st=0;
			bool HasE=true;
			while(HasE||st<RightSize){
				HasE=false;
				string Right=pro.second[i];
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
	ifstream in("LR1.txt");
// the first line , terminal marks
	getline(in,s);
	cout<<s<<endl;
	T=split(s);
	size_t len=T.size();
	for(size_t i=0;i<len;i++){
		Terminal[T[i]]=tercnt++;
	}
// the second line, non-terminal marks
	getline(in,s);
	cout<<s<<endl;
	T=split(s);
	len=T.size();
	for(size_t i=0;i<len;i++){
		NonTerminal[T[i]]=nontercnt++;
	}
	cout<<"done"<<endl;
//  productions here
	while(getline(in,s)){
		cout<<s<<endl;
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
}
//get all the projects, which means add dots to productions
inline void GetProjects(){
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
//give a project just like S => .SS
//return S => SS 's id in productions
//means which production it comes from
inline int IndexOfProjects(pair<string,vector<string> > I){
	size_t size1=production.size();
	pair<string,vector<string> > tmp;
	vector<string> right;
	right.clear();
	size_t size2=I.second.size();
	for(size_t i=0;i<size2;i++){
		if(I.second[i]!="."){
			right.pb(I.second[i]);
		}
	}
	tmp=mp(I.first,right);
	for(size_t i=0;i<size1;i++){
		pair<string,vector<string> > tmp2;
		tmp2=production[i];
		if(tmp==tmp2)return i;
	}
	return -1;
}
inline void PrintProjects(){
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
inline void PrintProduction(){
	size_t len=production.size();
	for(size_t i=0;i<len;i++){
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
// judge whether a projects is in the union 
inline bool NotIn(pair<string,vector<string> > pro,SET ret){
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
			else continue;
			if(same)return false;
		}
	}
	return true;
}
//
inline SET CLOSURE (SET I){
	SET ret;
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
				if(projects[i].second[0]!=".")continue;
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
//
inline SET GO(SET I,string X){
	SET ans;
	if(X=="")return I;
	ans.clear();
	size_t size=I.size();
	for(size_t i=0;i<size;i++){
		size_t len=I[i].second.size();
		size_t dot=GetIndexOfDot(I[i].second);
		if(dot==len-1){
			continue;
		}
		if(I[i].second[dot+1]!=X)continue;
		vector<string> newright;
		newright.clear();
		for(size_t j=0;j<dot;j++){
			newright.pb(I[i].second[j]);
		}
		newright.pb(I[i].second[dot+1]);
		newright.pb(".");
		for(size_t j=dot+2;j<len;j++){
			newright.pb(I[i].second[j]);
		}
		
		ans.pb(mp(I[i].first,newright));
	}
	return CLOSURE(ans);

}
//in tot union,set i has already in the set and is not a new state
inline int AlreadyIn(SET I){
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
		if(same)return (int)i;
	}
	return -1; 
}
//debug
inline void debug(pair<string,vector<string> > I){
	size_t len=I.second.size();
	cout<<I.first<<" => ";
	for(size_t i=0;i<len;i++){
		cout<<I.second[i]<<' ';
	}
	cout<<endl;
}
inline void debug(SET I){
	size_t len=I.size();
	for(size_t i=0;i<len;i++){
		debug(I[i]);
	}
}
inline void debug(vector<SET> I){
	size_t size=I.size();
	for(size_t i=0;i<size;i++){
		cout<<"I"<<i<<endl;
		debug(I[i]);
		cout<<endl;
	}
}
inline void debug(){
	cout<<"----production----"<<endl;
	size_t len=production.size();
	for(size_t i=0;i<len;i++){
		cout<<production[i].first<<" => ";
		size_t len2=production[i].second.size();
		for(size_t j=0;j<len2;j++){
			cout<<production[i].second[j]<<' ';
		}
		cout<<endl;
	}
	cout<<endl<<endl<<"----projects----"<<endl;
	len=projects.size();
	for(size_t i=0;i<len;i++){
		cout<<projects[i].first<<" => ";
		size_t len2=projects[i].second.size();
		for(size_t j=0;j<len2;j++){
			cout<<projects[i].second[j]<<' ';
		}
		cout<<endl;
	}
	cout<<endl<<endl<<endl;
}
inline void debug(vector<vector<int> > A){
	size_t len=A.size();
	for(size_t i=0;i<len;i++){
		size_t len2=A[i].size();
		for(size_t j=0;j<len2;j++){
			cout<<A[i][j]<<' ';
		}
		cout<<endl;
	}
	cout<<endl;
}
inline void debug(map<string,vector<string> > M){
	map<string,vector<string> >::iterator it;
	for(it=M.begin();it!=M.end();it++){
		cout<<it->first<<' ';
		size_t size=it->second.size();
		for(size_t i=0;i<size;i++){
			cout<<it->second[i]<<' ';
		}
		cout<<"!!size  "<<size<<' '<<endl;
	}
}
//  Create action and goto table
inline void GetActionGoto(){
	Union.clear();
	SET start;
	start.clear();
//  get ready for the first STATE
	vector<string> righttmp;
	righttmp.clear();
//	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	righttmp.pb(".");
	righttmp.pb("S");
	start.pb(mp("S'",righttmp));
//  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	start=CLOSURE(start);
	//I0
	Union.pb(start);
	//start from the nth closure and then goto get more closures
	//when find a new closure,add it to SET add update the action table
	vector<int> azero,gzero;
	azero.clear();
	gzero.clear();
	for(int i=0;i<tercnt;i++)azero.pb(0);
	for(int i=0;i<nontercnt;i++)gzero.pb(0);
	size_t totsize=Union.size();
	for(size_t i=0;i<totsize;i++){
		size_t partsize=Union[i].size();
		action.pb(azero);
		go.pb(gzero);
		for(size_t j=0;j<partsize;j++){
			pair<string,vector<string> > curr=Union[i][j];
			if("S"==curr.second[0]){
				action[i][Terminal["$"]]=ACC;
				continue;
			}

			int tindex=IndexOfProjects(curr);
			size_t dot=GetIndexOfDot(curr.second);
			if(dot==curr.second.size()-1){
				for(int key=0;key<tercnt;key++)action[i][key]=-tindex;
				continue;
			}
			string tmp=curr.second[dot+1];
			SET next=GO(Union[i],tmp);
			int index=AlreadyIn(next);
			if(index==-1){
				Union.pb(next);
				index=Union.size()-1;
				totsize=Union.size();
			}
			if(IsTerminal(tmp)){
				action[i][Terminal[tmp]]=tindex;	
			}else{
				go[i][NonTerminal[tmp]]=index;
			}
		}
	}
}
int main(){
	Ini();
	GetProduction();
	GetProjects();
	GetActionGoto();
	debug(Union);
	cout<<"====action===="<<endl;
	debug(action);
	cout<<"====go===="<<endl;
	debug(go);
	GetFirst();
	debug(FIRST);
	return 0;
}
