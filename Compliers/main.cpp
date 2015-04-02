#include <cstring>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include "Scanner.cpp"
using namespace std;

int main(){
	freopen("in.txt","r",stdin);
    freopen("out.txt","w",stdout);
	vector<pair<int,int> > ans=Scanner();
    for(int i=0;i<ans.size();i++){
        printf("%d\n",ans[i].first);
    }
    return 0;
}

