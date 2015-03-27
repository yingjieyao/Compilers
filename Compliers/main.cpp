#include <cstring>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
using namespace std;
#define maxSize 1000
#define ll long long
#define eps 1e-3
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
} MAP[maxSize];
int mapCnt;

map<string,int> M2Num;
map<string,string> M2Str;
string From[66]= {"and","array","begin","case","const","div","do",
                  "downto","else","end","file","for","function",
                  "goto","if","in","label","mod","nil","not","of","or",
                  "packed","procedure","program","record","repeat",
                  "set","then","to","type","until","var","while","with",
                  "+","-","*","/","=","<",">","<=",">=","<>","(",")",",",
                  "\"",".","..",":",":=",";","^","**","[","]","'","{","}","int","double"
                 };
string To[66]= {"AND","ARRAY","BEGIN","CASE","CONST","DIV","DO","DOWNTO",
                "ELSE","END","FILE","FOR","FUNC","GOTO","IF","IN","LABEL",
                "MOD","NIL","NOT","OF","OR","PACKED","PROC","PROG","RECORD",
                "REPEAT","SET","THEN","TO","TYPE","UNTIL","VAR","WHILE","WITH",
                "PLUS","MINUS","MULTI","RDIV","EQ","LT","GT","LE","GE","NE",
                "LR_BRAC","RR_BRAC","COMMA","P_MARK","F_STOP","RANGE","COLON",
                "ASSIGN","SEMIC","CAP","EXP","LS_BRAC","RS_BRAC","Q_MARK","L_BRACE","R_BRACE",
                "INT","DOUBLE"
               };


/*
int double
*/

void Ini() {
    memset(MAP,0,sizeof(MAP));
    mapCnt=0;
    M2Num.clear(),M2Str.clear();
    for(int i=0; i<62; i++)M2Str[From[i]]=To[i];
    for(int i=0; i<35; i++)M2Num[From[i]]=i+1;
    for(int i=35; i<62; i++)M2Num[From[i]]=i+5;
}
bool isKey(string s) {
    return M2Num[s]!=0;
}
bool isInteger(string s) {
    for(int i=0; s[i]; i++)
        if(!isdigit(s[i]))return false;
    return true;
}
bool isId(string s) {
    if(!isalpha(s[0]))return false;
    for(int i=0; s[i]; i++)
        if(!isdigit(s[i])&&!isalpha(s[i])&&s[i]!='_')return false;
    bool hasLetter=false,has_=false;
    for(int i=0; s[i]; i++)hasLetter|=(isalpha(s[i])),has_|=(s[i]=='_');
    if(has_&&!hasLetter)return false;
    return true;
}

#define isreal 1
#define dotError 2
#define digError 3

int isReal(string s) {
    bool dot=false;
    for(int i=0; s[i]; i++) {
        if(!isdigit(s[i])&&s[i]!='.')return digError;
        if(s[i]=='.'&&i&&dot)return dotError;
        if(s[i]=='.'&&i)dot=true;
    }
    return isreal;
}
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
//    cout<<ans1<<endl;
    return ans1;

}
void Print(string &s) {
    int type=isReal(s);
//    doubleto2bit(s);
    if(isKey(s)) cout<<s<<' '<<M2Str[s]<<endl;
    else if(isInteger(s))cout<<intto2bit(s)<<' '<<"INTEGER"<<endl;
    else if(type==isreal)cout<<doubleto2bit(s)<<' '<<"REAL"<<endl;
    else if(type==dotError)cout<<s<<' '<<"Multi Dot ERROR"<<endl;
    else {
        int len=s.length();
        if(s[0]==s[len-1]&&s[0]=='\"')cout<<s<<' '<<"STRING"<<endl;
        else {
            cout<<s<<' '<<"ID"<<endl;
            // to do
//            MAP[mapCnt++]=Map(s,0,0);
        }
    }
    s="";
}
int main() {
    freopen("in.txt","r",stdin);
    freopen("out.txt","w",stdout);
    Ini();
    ///
    string s;
    // char s[11111];
    while(getline(cin,s)) {
    // while(gets(s)!=NULL){
        // cout<<s<<endl;
        string tmp="";
        for(int i=0; s[i]; i++) {
            if(s[i]==' ') {
                if(tmp.length())Print(tmp);
            } else if(s[i]=='+'||s[i]=='-'||s[i]=='*'||s[i]=='='||s[i]=='('||s[i]==')'||
                      s[i]==','||s[i]==';'||s[i]=='['||s[i]==']'||s[i]=='\''||s[i]=='{'||
                      s[i]=='}') {
                if(tmp.length()) Print(tmp);
                tmp="";
                tmp=tmp+s[i];
                Print(tmp);
            } else if(s[i]=='.'&&s[i+1]=='.') {
                if(tmp.length())Print(tmp);
                tmp="";
                tmp=tmp+s[i]+s[i+1];
                Print(tmp);
            } else if(s[i]=='.') {
                if(tmp.length())Print(tmp);
                tmp="";
                tmp=tmp+s[i];
                Print(tmp);
            } else if(s[i]=='/'&&s[i+1]=='/') {
                break;
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
                    if(s[i]=='<'&&s[i+1]=='>') {
                        tmp+=s[i=1];
                        i++;
                    }
                    Print(tmp);
                }
            } else if(s[i]==':') {
                if(tmp.length())Print(tmp);
                tmp="";
                tmp+=s[i];
                if(s[i+1]&&s[i+1]=='=') {
                    tmp+='=';
                    i++;
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
                if(tmp.length())Print(tmp);
                tmp="";
                tmp+=s[i];
                while(s[i+1]&&(isdigit(s[i+1])||(s[i+1]=='.'&&!(s[i]=='.'&&s[i+1]=='.'))))
                    tmp+=s[++i];
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
    return 0;
}
