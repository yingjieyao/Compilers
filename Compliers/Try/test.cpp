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
/*
 * store the production
 */
vector<pair<string,string> > production;
/* 
 * store the production with integer
 */
vector<vector<int> > Pro;
/*
 * from to mark to integer
 */
map<string,int> Hash;
/*
 * from integer to marks
 */
map<int,string> DeHash;
/*
 * the firstset of all marks
 */
vector<set<int> > FIRST;
/*
 * key's number
 */
int keycnt;
/*
 * the edge of terminal and non-terminal mark
 */
int edge;
inline void ini(){
	Hash.clear();
	DeHash.clear();
	keycnt=1;
	int len=Pro.size();
	for(int i=0;i<len;i++){
		Pro[i].clear();
	}
	len=FIRST.size();
	for(int i=0;i<len;i++){
		FIRST[i].clear();
	}
}
/*
 * split string by space 
 * and return the vector
 */
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
	ans.pb(tmp);
	return ans;
}
// is or not terminal mark
inline bool isTer(const int &t){
	if(t<edge)return false;
	return true;
}
// is or not non-terminal mark
inline bool isNonTer(const int &t){
	if(t<edge)return true;
	return false;
}
/* get first set
 * should be store in vector<set<int> > First
 * and there will be another function 
 *	which calc the first(a),where a in (V union T)*
 */
inline set<int> First(const int &S){
	set<int> ans;
	ans.clear();
	int st=S;
	if(isTer(st)){
		ans.insert(st);
		FIRST[S]=ans;
		return ans;
	}
	set<int> tmp,tmp2;
	tmp.clear();
	tmp2.clear();
	int TotProSize=Pro.size();
	for(int i=0;i<TotProSize;i++){
		if(Pro[i].size()>=2&&Pro[i][0]==st){
			if(isTer(Pro[i][1]))ans.insert(Pro[i][1]);//5
			else if(isNonTer(Pro[i][1])){//7
				tmp2.insert(First(Pro[i][1]).begin(),First(Pro[i][1]).end());
				tmp2.erase(0);
				tmp.insert(tmp2.begin(),tmp2.end());
				ans.insert(tmp.begin(),tmp.end());
			}
			if(Pro[i][1]==0&&Pro[i].size()==2)ans.insert(0);//5 4
		}
	}
	FIRST[S]=ans;
	return ans;
}
/* 
 * this is another function as described above
 */
inline set<int> First(const vector<int> &S){
	set<int> ans,tmp;
	vector<set<int> > TFIR;
	int size=TFIR.size();
	for(int i=0;i<size;i++)TFIR.clear();
	ans.clear();
	int len=S.size();
	for(int i=0;i<len;i++){
		tmp.insert(FIRST[S[i]].begin(),FIRST[S[i]].end());
		tmp.erase(0);
		TFIR.pb(tmp);
	}
	tmp=TFIR[0];
	ans.insert(tmp.begin(),tmp.end());
	int st=0;
	while(TFIR[st].find(0)!=TFIR[st].end()&&st<size-1){
		ans.insert(TFIR[st+1].begin(),TFIR[st+1].end());
		st++;
	}
	if(st==len&&TFIR[st].find(0)!=TFIR[st].end())
		ans.insert(0);
	return ans;
}
/*
 * just for read all the productions from files
*/
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
			    DeHash[keycnt-1]=s;
			}
			continue;
		}

		if(st==4)break;
	}
	vector<int> tmp;
	tmp.clear();
	// get production
	while(getline(in,s)){
		//cout<<s<<endl;
		tmp.clear();
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
		production.pb(mp(first,second));
		tmp.pb(Hash[first]);
		//cout<<Hash[first]<<" => ";
		vector<string> T=split(second);
		len=T.size();
		for(int i=0;i<len;i++){
			tmp.pb(Hash[T[i]]);
			//cout<<Hash[T[i]]<<' ';
		}
		//cout<<endl;
		Pro.pb(tmp);
	}
	in.close();
}
int main(){
	ini();
	GetProduction();
	set<int> ans;
	ans.clear();
	ans=First(Hash["id"]);
	set<int>::iterator it=ans.begin();
	cout<<"SDFSDFSDF   "<<ans.size()<<endl;
	for(;it!=ans.end();it++){
		cout<<*it<<endl;
	}
	return 0;
}
