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
#define ITEM pair<pair<string,vector<string> > ,string >
#define SET vector<ITEM > 
//                           left       right          search
SET projects;
vector<SET> Union;
inline void debug(vector<vector<int> > T){
	size_t size=T.size();
	for(size_t i=0;i<size;i++){
		size_t tsize=T[i].size();
		for(size_t j=0;j<tsize;j++){
			cout<<T[i][j]<<' ';
		}
		cout<<endl;
	}
	cout<<endl<<endl;
}
inline void debug(ITEM I){
	cout<<"[ "<<I.first.first<<" => ";
	size_t size=I.first.second.size();
	for(size_t i=0;i<size;i++){
		cout<<I.first.second[i]<<' ';
	}
	cout<<" , "<<I.second<<" ]"<<endl;
}
inline void debug(SET I){
	size_t size=I.size();
	for(size_t i=0;i<size;i++){
		debug(I[i]);
	}
}
inline void debug(vector<SET> U){
	size_t size=U.size();
	for(size_t i=0;i<size;i++){
		cout<<"I"<<i<<endl;
		debug(U[i]);
	}
	cout<<endl;
}
inline int GetIndexOfDot(ITEM I){
	vector<string> tmp=I.first.second;
	size_t size=tmp.size();
	for(size_t t=0;t<size;t++)
		if(tmp[t]==".")return t;
	return -1;
}
inline bool IsAlreadyIn(SET C,ITEM b){
	size_t size=C.size();
	for(size_t i=0;i<size;i++){
		if(C[i]==b)return true;
	}
	return false;
}
inline SET CLOSURE(SET I){
	SET ans;
	ans.clear();
	size_t TotSize=production.size();
	size_t size=I.size();
	for(size_t t=0;t<size;t++)
		ans.pb(I[t]);
	size_t i=0;
	for(;i<size;){
		ITEM tmp=ans[i];
		i++;
		size_t RightSize=tmp.first.second.size();
		int DotIndex=GetIndexOfDot(tmp);
		if(DotIndex==(int)RightSize-1)continue;
		string B=tmp.first.second[DotIndex+1];
		if(B=="D"){
			cout<<"D"<<endl;
		}
		vector<string> Bb;
		Bb.clear();
		for(size_t t=DotIndex+2;t<RightSize;t++)
			Bb.pb(tmp.first.second[t]);
		Bb.pb(tmp.second);

		vector<string> first=GetFirst(Bb);
		size_t firstsize=first.size();
		for(size_t t=1;t<TotSize;t++){
			if(production[t].first==B){
				ITEM NewItem;
				vector<string> newright;
				newright.clear();
				newright.pb(".");
				size_t oldsize=production[t].second.size();
				for(size_t a=0;a<oldsize;a++){
					newright.pb(production[t].second[a]);
				}
				for(size_t j=0;j<firstsize;j++){
					if(first[j].length()==0)continue;
					NewItem=mp(mp(production[t].first,newright),first[j]);

					if(IsAlreadyIn(ans,NewItem))continue;
					ans.pb(NewItem);
				}
			}
		}
		size=ans.size();
	}
	return ans;
}

inline SET GO(SET I,string X){
	SET ans;
	ans.clear();
	size_t size=I.size();
	for(size_t t=0;t<size;t++){
		ITEM curr=I[t];
		if(curr.first.first=="D"&&X=="Define"&&curr.first.second[1]=="Define"){
			cout<<"!!!!"<<endl;
			int a=1;
			a++;
		}
		int dotindex=GetIndexOfDot(curr);
		
		size_t RightSize=curr.first.second.size();
		if(dotindex==(int)RightSize-1)continue;
		if(I[t].first.second[dotindex+1]!=X)continue;
		vector<string> newright;
		newright.clear();
		for(int i=0;i<dotindex;i++)
			newright.pb(curr.first.second[i]);
		newright.pb(X);
		newright.pb(".");
		for(size_t i=dotindex+2;i<RightSize;i++)
			newright.pb(I[t].first.second[i]);
		ITEM NewItem;
		NewItem=mp(mp(curr.first.first,newright),curr.second);
		ans.pb(NewItem);
	}
	return CLOSURE(ans);
}

inline int Exits(SET I){
	//cout<<I.size()<<endl;
	size_t size=Union.size();
	for(size_t i=0;i<size;i++){
		if(Union[i]==I)return i;
	}
	return -1;
}
inline int GetIndexProduction(ITEM I){
	vector<string> tright;
	tright.clear();
	size_t size=I.first.second.size();
	for(size_t t=0;t<size;t++){
		if(I.first.second[t]!=".")
			tright.pb(I.first.second[t]);
	}
	pair<string,vector<string> > T=mp(I.first.first,tright);
	size=production.size();
	for(size_t t=0;t<size;t++){
		if(T==production[t])return (int)t;
	}
	return -1;
}
inline void GetActionGoto(){
	Union.clear();
	ITEM start;
	vector<string> righttmp;
	righttmp.clear();
	righttmp.pb(".");
	righttmp.pb("Yao");
	SET T;
	T.clear();
	start=mp(mp("Yao'",righttmp),"#");
	T.pb(start);
	debug(start);
	SET C1=CLOSURE(T);
	debug(C1);
	//return;
	Union.pb(C1);
	vector<int> azero;
	azero.clear();
	vector<int> gzero;
	gzero.clear();
	for(int i=0;i<tercnt;i++)azero.pb(0);
	for(int i=0;i<nontercnt;i++)gzero.pb(0);
	size_t totsize=Union.size();
	for(size_t i=0;i<totsize;i++){
		//cout<<"============="<<i<<"==========="<<endl;
		//debug(Union[i]);
		vector<int> azero,gzero;
		azero.clear();
		gzero.clear();
		for(int j=0;j<tercnt;j++)azero.pb(0);
		for(int j=0;j<nontercnt;j++)gzero.pb(0);
		action.pb(azero);
		go.pb(gzero);
		size_t partsize=Union[i].size();
		map<string,int>::iterator it;
		for(it=Terminal.begin();it!=Terminal.end();it++){
			SET tmpans;
			tmpans.clear();
			for(size_t t=0;t<partsize;t++){
				ITEM curr=Union[i][t];
				size_t currsize=curr.first.second.size();
				if(curr.first.second[currsize-1]=="."&&it->first==curr.second){
					if(curr.first.first=="Yao'"){
						if(action[i][Terminal["#"]])cout<<"error"<<endl;	
						action[i][Terminal["#"]]=ACC;
						continue;
					}
					int index=GetIndexProduction(curr);
					if(action[i][Terminal[it->first]])cout<<"error"<<endl;
					action[i][Terminal[it->first]]=-index;	
					continue;
				}
				SET nextt;
				nextt.clear();
				nextt.pb(curr);
				SET next=GO(nextt,it->first);
				next=CLOSURE(next);
				size_t nextsize=next.size();
				if(nextsize>0){
					for(size_t tt=0;tt<nextsize;tt++){
						tmpans.pb(next[tt]);
					}
				}
			}
			size_t tmpsize=tmpans.size();
			SET tans;
			tans.clear();
			bool *is=new bool[tmpsize];
			memset(is,true,sizeof(bool)*tmpsize);
			for(size_t a=0;a<tmpsize;a++){
				for(size_t b=a+1;b<tmpsize;b++){
					if(tmpans[a]==tmpans[b]){
						is[a]=false;
						break;
					}
				}
				if(is[a])tans.pb(tmpans[a]);
			}
			if(!tans.size())continue;
			int index=Exits(tans);
			if(index==-1){
				index=Union.size();
				Union.pb(tans);
			}
			if(action[i][Terminal[it->first]])cout<<"error"<<endl;
			action[i][Terminal[it->first]]=index;
		}



		for(it=NonTerminal.begin();it!=NonTerminal.end();it++){
			SET tmpans;
			tmpans.clear();
			for(size_t t=0;t<partsize;t++){

				if(it->first=="Define"&&i==18&&t==2){
					int a=123;
					a++;
				}
				ITEM curr=Union[i][t];
				SET nextt;
				nextt.clear();
				nextt.pb(curr);
				SET next=GO(nextt,it->first);
				next=CLOSURE(next);
				size_t nextsize=next.size();
				if(nextsize){
					for(size_t tt=0;tt<nextsize;tt++){
						tmpans.pb(next[tt]);
					}
				}
			}
			size_t tmpsize=tmpans.size();
			SET tans;
			tans.clear();
			bool *is=new bool[tmpsize];
			memset(is,true,sizeof(bool)*tmpsize);
			for(size_t a=0;a<tmpsize;a++){
				for(size_t b=a+1;b<tmpsize;b++){
					if(tmpans[a]==tmpans[b]){
						is[a]=false;
						break;
					}
				}
				if(is[a])tans.pb(tmpans[a]);
			}
			if(tans.size()==0)continue;
			int index=Exits(tans);
			if(index==-1){
				index=Union.size();
				Union.pb(tans);
			}
			if(go[i][NonTerminal[it->first]])cout<<"error"<<endl;
			go[i][NonTerminal[it->first]]=index;
	//		cout<<"goto["<<i<<"]["<<it->first<<"]="<<index<<endl;
		}
		totsize=Union.size();
	}
}	

int main(){
	Ini();
	GetProduction();
//	PrintProduction();
	cout<<"start!!!"<<endl;
	GetFirst();
	cout<<"GetFirst done! "<<endl;
	GetActionGoto();
	cout<<Union.size()<<endl;
	///debug(Union);
	debug(action);
//	debug(go);
	return 0;

}
