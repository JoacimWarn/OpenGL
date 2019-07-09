//
//
#include <iostream>
#include <stack>
#include <fstream>
#include <string>
//
//
using namespace std;

int main(){
	
	ifstream infil;
	
	infil.open("main.c");
	
	if(!infil) infil.open("main.cpp");
	if(!infil){
		
		cout << "could not find file" << endl;
		return 1;
		
	}
	
	stack<char> parenthese;
	stack<char> curlyBracket;
	
	string inmat("");
	
	while(getline(infil,inmat)){
		
		for(int i = 0; i < inmat.size(); i++){
			
			if(inmat.at(i) == '('){
				
				parenthese.push(inmat.at(i));
				
			}
			else if(inmat.at(i) == ')'){
				
				if(parenthese.empty()) cout << "wrong with parenthese" << endl;
				
			}
			
			if(inmat.at(i) == '{'){
				
				curlyBracket.push(inmat.at(i));
				
			}
			else if(inmat.at(i) == '}'){
				
				if(curlyBracket.empty()) cout << "wrong with curly brackets" << endl;
				
			}
			
		}
		
	}
	
	return 0;
	
}
//
//