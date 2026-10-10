#include<iostream>
#include<string>
using namespace std;


int main(){
  string str;
  char ch;
  cout<<"Enter a string"<<endl;
  getline(cin, str);

  cout<<"Enter a character you wnat to delete: "<<endl;
  cin>>ch;

  int j=0;
  for(int i=0; i<str.length(); i++){
    if(str[i]!=ch){
      str[j++]=str[i];
    }
    
    
  }
  str.resize(j);
  cout<<str;

  return 0;
}