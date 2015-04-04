#include<cstring>
#include<cstdio>
#include<iostream>
#include<cstdio>
#include<string>
using namespace std;
int main(){
	freopen("LALR.htm","r",stdin);
	freopen("LALR1.txt","w",stdout);
	char c;
	while((c=getchar())!=EOF){
		if(c=='\r')continue;
		cout<<c;
	}
}
