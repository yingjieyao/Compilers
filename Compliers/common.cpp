#include <cstring>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
using namespace std;
#define maxSize 2000
#define ll long long
#define eps 1e-3
#define isreal 1
#define dotError 2
#define digError 3
#define pb push_back
#define mp make_pair
struct Map {
    string name;
    int type;
    int addr;
    Map() {}
    Map(string _name,int _type,int _addr) {
        name=_name;
        type=_type;
        addr=_addr;
    }
};
Map MAP[maxSize];
int mapCnt;

//key id need to insert into Trie

// map to 
map<string,int> M2Num;
map<string,string> M2Str;

#define FromSize 38


string From[FromSize]={"func","bool","int","double","char","break",
                    "continue","if","elsif","else","return","max","min","for",
                    "+","-","*","/","++","--","+=","-=","<",">","==",">=",
                    "<=","!=","&&","||","!","{","}","(",")","=",";",","
                };

string To[FromSize]={"FUNC","BOOL","INT","DOUBLE","CHAR","BREAK",
                    "CONTINUE","IF","ELSIF","ELSE","RETURN","MAX","MIN","FOR",
                    "PLUS","MINUS","MULTI","DIV","D_PLUS","D_MINUS","PLU_EQU","MINU_EQU",
                    "LT","GT","EQU","GE","LE","NE","AND","OR","NOT","L_GRACE","R_GRACE",
                    "L_C","R_C","ASSIGN","SEMIC","COMMA"
                };

void Ini() {
    mapCnt=0;
    M2Num.clear(),M2Str.clear();
    for(int i=0; i<FromSize; i++)M2Str[From[i]]=To[i];
    for(int i=0; i<FromSize; i++)M2Num[From[i]]=i+1;
}

int getId(string s){
	return 0;
}
// judge whether a word is key 
// contains <= etc
bool isKey(string s) {
    return M2Num[s]!=0;
}

// judge whether a word is a number
// not negative 
bool isInteger(string s) {
    if(!(isdigit(s[0])||s[0]=='-'))return false;
    for(int i=1; s[i]; i++)
        if(!isdigit(s[i]))return false;
    return true;
}

// whether a word is a legal ID
// begin with letter or '_' and only contains numbers,letters and '_'
bool isId(string s) {
    if(!isalpha(s[0]))return false;
    for(int i=0; s[i]; i++)
        if(!isdigit(s[i])&&!isalpha(s[i])&&s[i]!='_')return false;
    bool hasLetter=false,has_=false;
    for(int i=0; s[i]; i++)hasLetter|=(isalpha(s[i])),has_|=(s[i]=='_');
    if(has_&&!hasLetter)return false;
    return true;
}

// judge whether a word is a real number 
// not negative 

int isReal(string s) {
    bool dot=false;
    if(s[0]=='.'&&s.length()!=1)return dotError;
    for(int i=0; s[i]; i++) {
        if(!isdigit(s[i])&&s[i]!='.')return digError;
        if(s[i]=='.'&&i&&dot)return dotError;
        if(s[i]=='.'&&i)dot=true;
    }
    return isreal;
}

/*
* define error type here
*
*
*
*
*/

void HandleError(int ErrorType){
    cout<<"ERROR"<<endl;
}

// convert a integer to 2 bit 
string intto2bit(string s) {
    ll ans=0;
    for(int i=0; s[i]; i++)
        ans=ans*10+s[i]-'0';
    string ret="";
    do {
        ret=(char)(ans%2+'0')+ret;
        ans>>=1;
    } while(ans);
    return ret;
}
int intto10bit(string s) {
    int ans=0;
    for(int i=0; s[i]; i++)
        ans=ans*10+s[i]-'0';
    return ans;
    string ret="";
    do {
        ret=(char)(ans%2+'0')+ret;
        ans>>=1;
    } while(ans);
    // return ret;
}
//convert a real number to 2 bit
string doubleto2bit(string s) {
    ll ans=0;
    for(int i=0; s[i]!='.'; i++) {
        ans=ans*10+s[i]-'0';
    }
    string ans1="";
    do {
        ans1=(char)(ans%2+'0')+ans1;
        ans>>=1LL;
    } while(ans);
    double tmp=0.0;
    int len=s.length();
    for(int i=len-1; s[i]!='.'; i--) {
        tmp=tmp*0.1+s[i]-'0';
    }
    ans1+='.';
    if(tmp)tmp*=0.1;
    int cnt=0;
    while(fabs(tmp)>eps&&cnt<10) {
        ans1+=(int)(tmp*2)+'0';
        tmp=tmp*2-(int)(tmp*2);
        cnt++;
    }
    return ans1;

}

// output the answer
// need to change to store in vector
vector<pair<int,int> > Word;
int Column;
void Print(string &s) {
    int type=isReal(s);
    if(isKey(s)) {
    	Word.pb(mp(M2Num[s],0));
    }
    else if(isInteger(s)){
    	Word.pb(mp(FromSize+1,intto10bit(s)));
    }
    else if(type==isreal){
    	Word.pb(mp(FromSize+2,0));//?????
    }
    else if(type==dotError&&s.length()>1){
        cout<<s<<' '<<"Dot ERROR "<<Column<<endl;
    }else{
        int len=s.length();
        if(s[0]==s[len-1]&&s[0]=='\"'){
        	Word.pb(mp(FromSize+3,0));//????
        }
        else {
            if(!isalpha(s[0])&&s[0]!='_'){
                cout<<s<<' '<<"ID ERROR "<<Column<<endl;
            }else{
            	Word.pb(mp(FromSize+4,getId(s)));
                // cout<<s<<' '<<"ID"<<endl;
                MAP[mapCnt++]=Map(s,0,0);
            }
        }
    }
    s="";
}
vector<pair<int,int> > Scanner() {
    // freopen("in.txt","r",stdin);
    // freopen("out.txt","w",stdout);
    Ini();
    string s;
    Column=0;
    Word.clear();
    while(getline(cin,s)!=NULL) {
        string tmp="";
        Column++;
        for(int i=0; s[i]; i++) {
            if(s[i]==' ') {
                if(tmp.length())Print(tmp);
            } else if(s[i]=='('||s[i]==')'||
                      s[i]==','||s[i]==';'||s[i]=='['||s[i]==']'||s[i]=='\''||s[i]=='{'||
                      s[i]=='}') {
                if(tmp.length()) Print(tmp);
                tmp="";
                tmp=tmp+s[i];
                Print(tmp);
            } else if(s[i]=='<'||s[i]=='>') {
                if(tmp.length())Print(tmp);
                if(s[i+1]&&s[i+1]=='=') {
                    tmp="";
                    tmp=tmp+s[i]+s[i+1];
                    Print(tmp);
                    i++;
                } else {
                    tmp="";
                    tmp=tmp+s[i];
                    Print(tmp);
                }
            }else if(s[i]=='&'){
                if(tmp.length())Print(tmp);
                tmp="";
                tmp+=s[i];
                if(s[i+1]&&s[i+1]=='&'){
                    tmp=tmp+'&';
                    i++;
                }
                Print(tmp);
            }else if(s[i]=='|'){
                if(tmp.length())Print(tmp);
                tmp="";
                tmp+=s[i];
                if(s[i+1]&&s[i+1]=='|'){
                    tmp=tmp+s[i+1];
                    i++;
                }
                Print(tmp);
            }else if(s[i]=='+'||s[i]=='-'||s[i]=='*'||
                    s[i]=='='||s[i]=='!'||s[i]=='/'){
                if(tmp.length())Print(tmp);
                tmp="";
                tmp+=s[i];
                if(s[i+1]&&s[i+1]=='='){
                    tmp+=s[++i];
                }else if(s[i+1]&&s[i]=='+'&&s[i+1]=='+'){
                    tmp+=s[++i];
                }else if(s[i+1]&&s[i]=='-'&&s[i+1]=='-'){
                    tmp+=s[++i];
                }
                Print(tmp);
            } else if(s[i]=='*') {
                if(tmp.length())Print(tmp);
                tmp="";
                tmp+=s[i];
                if(s[i+1]&&s[i+1]=='*') {
                    tmp+='*';
                }
                Print(tmp);
            } else if(s[i]=='\"') {
                if(tmp.length())Print(tmp);
                tmp="";
                tmp+=s[i];
                while(s[i+1]&&s[i+1]!='\"') {
                    tmp+=s[++i];
                }
                tmp+=s[++i];
                Print(tmp);
            } else if(isdigit(s[i])) {
                //handle negative 
                if(tmp.length())Print(tmp);
                tmp="";
                tmp+=s[i];
                while(s[i+1]&&(isdigit(s[i+1])||(s[i+1]=='.'&&!(s[i]=='.'&&s[i+1]=='.'))))
                    tmp+=s[++i];
                if(isalpha(s[i+1])||s[i+1]=='_'){
                    while(s[i]!=' '&&s[i])tmp+=s[++i];
                }
                int len=tmp.length();
                if(tmp[len-1]=='.')i--,tmp=tmp.substr(0,len-1);
                Print(tmp);
            } else if(isalpha(s[i])||s[i]=='_') {
                if(tmp.length())Print(tmp);
                tmp="";
                tmp+=s[i];
                while(s[i+1]&&(isdigit(s[i+1])||isalpha(s[i+1])))tmp+=s[++i];
                Print(tmp);
            }
        }
    }
    return Word;
}
