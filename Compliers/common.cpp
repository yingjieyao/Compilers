//used to describe basic data structure 
// author : Yogurt 
#include <cstring>
#include <string>
#include <cstdio>
#include <iostream>
#include <fstream>

using namespace std;
void read(){
	ofstream out("out.txt");
	for(int i=0;i<100;i++){
		out<<i<<endl;
	}
	out.close();
}

void write(){
	ifstream in("out.txt");
	int d;
	while(in>>d){
		cout<<d<<endl;
	}
	in.close();
}
int main(){
	read();
	write();
	return 0;

}
