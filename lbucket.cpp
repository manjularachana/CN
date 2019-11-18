#include<iostream>
#include<stdlib.h>
#include<unistd.h>
#include <time.h>
#include<bits/stdc++.h>
#define bucketSize 512
using namespace std;
bool contains(vector<int>&a,int b){
	auto i=find(a.begin(),a.end(),b);
	if(i!=a.end()){
		return true;}
		else
			return false;
	}
int generateUniqueInt()
{
  static vector<int> generatedValues;

  int num = rand() % 1000;

  while(contains(generatedValues, num)) //You'll need a function to check whether the num is in this vector
  {
    num = rand() % 1000;
  }
  return num;
}
void bktInput(int a,int b) {
if(a>bucketSize)
    cout<<"\n\t\tBucket overflow";
else {
    usleep(500);
while(a>b){
	cout<<"\n\t\t"<<b<<" bytes outputted.";
	a-=b;
	usleep(500);
    }
    if (a>0) cout<<"\n\t\tLast "<<a<<" bytes sent\t";
    cout<<"\n\t\tBucket output successful";
  }
}

int main() {
int op=100, pktSize;
  cout<<"hello";
for(int i=1;i<=5;i++){
    usleep(rand()%1000);
    pktSize=generateUniqueInt();
    cout<<"\nPacket no "<<i<<"\tPacket size = "<<pktSize;
    bktInput(pktSize,op);
  }
  return 0;
}