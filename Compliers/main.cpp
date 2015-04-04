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
    freopen("Scanner.txt","w",stdout);
	vector<pair<int,string> > ans=Scanner();
    for(int i=0;i<ans.size();i++){
        cout<<ans[i].first<<endl;
    }
    return 0;
}

