//============================================================================
//
//% Student Name 1: Zihao Wu
//% Student 1 #: 301280709
//% Student 1 userid (email): zwa95 (zwa95@sfu.ca)
//
//% Student Name 2: Shixin Yang
//% Student 2 #: 301269683
//% Student 2 userid (email): shixiny (shixiny@sfu.ca)
//
//% Below, edit to list any people who helped you with the code in this file,
//%      or put �none� if nobody helped (the two of) you.
//
// Helpers: NONE
//
// Also, list any resources beyond the course textbook and the course pages on Piazza
// that you used in making your submission.
//
// Resources:
//		Part1-1174 tutorial from Craig Scratchley
//		http://www.cplusplus.com/reference/
//		https://stackoverflow.com/questions/3418231/replace-part-of-a-string-with-another-string
//%% Instructions:
//% * Put your name(s), student number(s), userid(s) in the above section.
//% * Enter the above information in Test.cpp if you are submitting that file too.
//% * Edit the "Helpers" line and "Resources" line.
//% * Your group name should be "<userid1>_<userid2>" (eg. stu1_stu2)
//% * Form groups as described at:  https://courses.cs.sfu.ca/docs/students
//% * Submit one or two files to courses.cs.sfu.ca
//
// Name        : TestPart1.cpp
// Description : Testing Code for Course Project Part 1 (Tokenizer)
// Original Version Copyright (c) 2017 Simon Fraser University
//============================================================================

#include "Part1Tokenizer.hpp"

using namespace std;
vector<string> comment_extracter(string& input){	//extracter the comment between two " " and return it in a vector<string>
    vector<string> comment;
    vector<string::size_type> pos;
    string::size_type index=0;
    string::size_type shrink=0;
    while((index=input.find("\"",index))!=input.npos){
        pos.push_back(index);
        index++;
    }
    for(int i=0;i<(pos.size()/2)*2;i=i+2){
        comment.push_back(input.substr(pos[i],pos[i+1]-pos[i]+1));
    }
    for(int i=0;i<(pos.size()/2)*2;i+=2){
        string::size_type indexStart=pos[i]-shrink;
        string::size_type indexEnd=pos[i+1]-shrink;
        input.replace(indexStart,indexEnd-indexStart+1,"@");    //replace comment by a @ symbol to track comment location
        shrink+=pos[i+1]-pos[i];
    }
    return comment;
}
void comment_inserter(vector<string>& input,vector<string> comment){	// inserter comment back to @ loaction
    string::size_type comment_index=0;
    for(int i=0;i<input.size();i++){
        if(input[i]=="@"){
            input[i]=comment[comment_index];
            comment_index++;
        }
    }
}
void comment_line_cleaner(string& input){	// replace anything start with " end with; with ||
	vector<string::size_type> quotation_pos;
	string::size_type index=0;
	string::size_type semicolon_index;
	int flag,length;
	while((index=input.find("\"",index))!=input.npos){
		quotation_pos.push_back(index);
		index++;
	}
	if((quotation_pos.size()%2)==0){
		return;
	}
	if((quotation_pos.size()%2)!=0){
		semicolon_index=input.find(";");
	}
	bool found=false;
	for(int i=0;i<quotation_pos.size()&&found==false;i++){
		if(semicolon_index>quotation_pos[i]){
			length=semicolon_index-quotation_pos[i]+1;
			flag=quotation_pos[i];
			found=true;
		}
	}
	input.replace(flag,length,"||");

}
void eraser (string& input, const string target){		//erase target from a string
	string::size_type index=0;
	while((index=input.find(target,index))!=input.npos){
		//index=input.find(target);
		input.erase(index,target.length());
	}
}
void replace_all(string& input,const string target,const string replacement){	// replace all target in a string by replacement
    string::size_type index=0;
    while((index=input.find(target,index))!=input.npos){
        //index=input.find(target);
        input.replace(index, target.length(), replacement);
        index = index + replacement.length();
    }
}
void merge_node (vector<string>& input,string::size_type first,string::size_type second){	//merge two nodes in a vector<string>
    string temp=input[first]+input[second];
    input[first]=temp;
    input.erase(input.begin()+second);
}
void spacer (string& input,const string target){
    string::size_type index=0;
    while((index=input.find(target,index))!=input.npos){
        input.insert(index," ");
        input.insert(index+target.length()+1," ");
        index = index +target.length()+2;
    }
}
vector<string> WSspliter (string input){	// push_back token into a vector<string> according to white space
	vector<string> output;
	string::size_type indexWS = 0;
	while (indexWS < input.length()-2) {
		string::size_type indexNWS = input.find_first_not_of(" \t", indexWS);
		indexWS = input.find_first_of(" \t", indexNWS);
		string token = input.substr(indexNWS, indexWS - indexNWS);
		output.push_back(token);
	}
	return output;
}
vector<string> tokenizeCodeStrip(istream& code) {	//main function
		string lineofcode;
		getline(code,lineofcode,'\0');
	    vector<string> result;
	    vector<string> comment;
	    replace_all(lineofcode,"\'\n","||");
	    replace_all(lineofcode,"\"\\\"\"","!quotation_mark");
	    comment_line_cleaner(lineofcode);
	    comment=comment_extracter(lineofcode);
	    eraser(lineofcode,"\n");
	    eraser(lineofcode,"\r");
	    vector <string> space_symbol={"=","+",";","||","*","eerrno","e-"};	//{"target1",.........}
	    for(int i=0;i<space_symbol.size();i++){
	    	spacer(lineofcode,space_symbol[i]);
	    }
	    vector <string> merge_symbol={"+  +  +","++ +","..",". .","+  +","++","e -","e-","E -","E-"};				//{"target 1","replace 1","target 2","replace 2"}
	    for(int i=0;i<merge_symbol.size();i=i+2){
	    	replace_all(lineofcode,merge_symbol[i],merge_symbol[i+1]);
	    }
	    result=WSspliter(lineofcode);
	    for(int i=0 ; i<result.size(); i++){		// postprocessing
	    	string temp =result[i];
	    	bool front_is_num;
	    	bool back_is_num;
	    	if(i>0){
	    		string front_temp = result[i-1];
	    		front_is_num=isdigit(front_temp.back());
	    	}
	    	if((i+1)<result.size()){
	    		string back_temp = result[i+1];
	    		back_is_num=isdigit(back_temp.front());
	    	}
	    	if(temp=="#define"){
	    		result.erase(result.begin()+i);
	    		result.erase(result.begin()+i);
	    	}
	    	if(temp[0]=='\''){
	    		if(temp.front()=='\''&&temp.back()=='\''){
	    			result[i]="\'\\n\'";
	    		}
	    		else{
	    			result[i]="";
	    		}
	    	}
	    	if(temp=="||"){
	    		result[i]="";
	    	}
	    	if(temp=="!quotation_mark"){
	    		result[i]="\"\\\"\"";
	    	}
	    	if(temp=="."){
	    		result[i]="";
	    	}
	    	if((temp=="e-"||temp=="E-")&&(front_is_num==1&&back_is_num==1)){
	    	    merge_node(result,i,i+1);
	    	    merge_node(result,i-1,i);
	    	}
	    	if((temp=="e-"||temp=="E-")&&(front_is_num==1&&back_is_num==0)){
	    		result[i]="e";
	    		result.insert(result.begin()+i+1,"-");
	    	}
	    }
	    	comment_inserter(result,comment);
	    	return result;
}
