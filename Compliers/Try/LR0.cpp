#include <cstring>
#include <map>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <cstring>
#include <set>
#include "common.cpp"
using namespace std;
#define pb push_back
#define mp make_pair
#define ACC 0x7fffffff
#define SET vector<pair<string,vector<string> > >

vector<pair<string,vector<string> > > projects;
vector<SET> Union;

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
	size_t tot=I.size();
	for(size_t i=0;i<tot;i++)
		ret.pb(I[i]);
	size_t st=0;
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
				//
				//====================SLR1===================
				//vector<string> dfollow=FOLLOW[curr.first];
				//size_t size =dfollow.size();
				//for(size_t t=0;t<size;t++){
				//	action[i][Terminal[dfollow[t]]=-tindex;
				//}
				//=====================SLR1 end==============
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
	cout<<"====action===="<<endl;
	cout<<"====go===="<<endl;
	GetFirst();
	return 0;
}
