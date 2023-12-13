#include <iostream>
#include"MemoryRiver.hpp"
#include"store.hpp"
#include<string>
#include<sstream>

void transfer(char object[] , const std::string & a){
  int size = a.size();
  for(int i =0;i<size;++i){
	object[i]=a[i];
  }
  object[size]='\0';

}

int main() {
  int n;
  std::cin>>n;
  std::getchar();
  store date_base;
  date_base.initialise();
  for(int i =0; i <n;++i){
	std::string words;
	getline(std::cin,words);
	std::istringstream iss(words);
	std::string token;
	iss>>token;
	if(token=="insert"){
	  std::string name;
	  int value;
	  iss>>name;
	  iss>>token;
	  value= std::stoi(token);
	  key_value date ;
	  transfer(date.name,name);
	  date.value = value;
	  date_base.insert(date);
	}
	if(token =="delete"){
	  std::string name;
	  int value;
	  iss>>name;
	  iss>>token;
	  value= std::stoi(token);
	  key_value date ;
	  transfer(date.name,name);
	  date.value = value;
	  date_base.erase(date);
	}
	if(token=="find"){
	  std::string name;
	  iss>>name;
	  date_base.find(name);
	}

  }
}
