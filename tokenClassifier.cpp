//============================================================================
//
//% Student Name 1: Shixin Yang
//% Student 1 #: 301269683
//% Student 1 userid (email): shixiny (shixiny@sfu.ca)
//
//% Student Name 2: student2
//% Student 2 #: 123456782
//% Student 2 userid (email): stu2 (stu2@sfu.ca)
//
//% Below, edit to list any people who helped you with the code in this file,
//%      or put 'None' if nobody helped (the two of) you.
//
// Helpers: _everybody helped us/me with the assignment (list names or put 'None')__
//           Professor : Craig
//			 TA : Mohammad
//
// Also, list any resources beyond the course textbook and the course pages on Piazza
// that you used in making your submission.
//
// Resources: From  http://www.cplusplus.com/ I got some function reference
//
//%% Instructions:
//% * Put your name(s), student number(s), userid(s) in the above section.
//% * Enter the above information in tokenClasses.cpp  too.
//% * Edit the "Helpers" line and "Resources" line.
//% * Your group name should be "P5_<userid1>_<userid2>" (eg. P5_stu1_stu2)
//% * Form groups as described at:  https://courses.cs.sfu.ca/docs/students
//% * Submit files to courses.cs.sfu.ca
//
// Name        : tokenClassifier.cpp
// Description : Course Project Part 2 (Token Classifier)
// Original portions Copyright (c) 2017 School of Engineering Science, Simon Fraser University
//============================================================================

#include <vector>
#include <sstream>
#include <algorithm>
#include "tokenClasses.hpp"

using namespace std;
using namespace ensc251;

#define WHITE "\r\t\v\f "

std::string delim3[] =  {">>=", "<<="};
std::string delim2[] =  {"+=","-=", "/=", "%=", "<<", ">>", "++", "--"};
std::string delim1andWhite =  WHITE "/()*%:;=+-~?";
std::string white = WHITE;

#define errorind "errno"





bool processDigits(std::string line, std::string::size_type &index)
{
	bool DigiFound=false;
	while (index < line.length() && isdigit(line.at(index)))
		{
			index++;
			DigiFound=true;
		}
	return DigiFound;
}
// Construct token objects categorized into appropriate types
// Input: a stream
// Output: a vector containing the list of Token objects properly categorized
std::vector<ensc251::Token*> tokenClassifier(istream& code)
{
        std::vector<ensc251::Token*> tokenObjectPs;
        std::vector<std::string> result;
       // vector<std::string> tokensearch(std::string &temp); //help function declearation
        //TODO: Write Code for classifying each token into an object of the proper class
        // store a pointer to the object in vector tokenObjectPs
       // void tokenCheck(std::string & temp);
		// For example, if you want to classify the number '10' in the input "errno = 10+2.5;":
   //tokenObjectPs.push_back(new ensc251::numeric_int_const<int>(10, 1 /* lineNumber*/, 8 /*indexNumber*/));
        // where the 2nd argument is the line Number and the 3rd argument is the
        //    indexNumber (the position of the start of the token in that line).
       // std::vector<std::string> result;

        int linecounter = 0;
        std::string line;
        	while (getline (code, line)) {
        		if (line.at(0) == '#')
        			continue;

        		std::string::size_type index2 = 0;
                auto length = line.length();

        		while(index2 < length ) {
        			auto index1 = line.find_first_not_of(white, index2);
        			if (index1 == std::string::npos)

        				break;

        			/// Dealing with int and float consts
        			if (isdigit(line.at(index1)) || line.at(index1) == '.')
        			{
        				index2 = index1;

        				// integer consts
        				bool preDigitFound = processDigits(line, index2);

        				// floats
        				if (index2 < length && line.at(index2) == '.')
        				{
        					index2++;
        					bool postDigitFound = processDigits(line, index2);

        					if(!preDigitFound && !postDigitFound) // detecting the single "." as an invalid token!
        					{
        						cout << "Error -- invalid token in input: " << line.substr(index1, index2 - index1) << endl;
        						result.push_back("");
        						tokenObjectPs.push_back(new ensc251::incorrect("",0, index1));
        						continue;
        					}
        				}

        				// exponentials
        				if (index2 < length && (line.at(index2) == 'e' || line.at(index2) == 'E'))
        				{
        					if(line.at(index2+1) == '+' || line.at(index2+1) == '-')
        						index2+=2;
        					else
        						index2++;

        					if(!processDigits(line, index2)) // invalid float -> roll back ...
        					{
        						if(line.at(index2-1) == 'e' || line.at(index2-1) == 'E')
        							index2--;
        						else
        							index2-=2;
        					}
        				}

        				// suffix f and F
        				if (index2 < length && (line.at(index2) == 'f' || line.at(index2) == 'F'))
        					index2++;

        				auto subs = line.substr(index1, index2 - index1);
        				result.push_back(subs);
//Classifier section that classifies what type of each tokens is*******************************************************************
        				std::string::size_type i = result.size();                                         //size of token vector
        				std::string::size_type u = subs.size();                                            //size of each token ,which are strings
        				if (subs ==";" || subs == "*" || subs == ":" || subs == "(" || subs == ")")        // classify punctuators
        					tokenObjectPs.push_back(new ensc251::punctuator(subs,linecounter,index1));
        				//********************************************************************for case 5

        				else if(subs== "errno")                                                                // classify string "errno"

        					tokenObjectPs.push_back(new ensc251::int_id(subs,linecounter,index1));

        				else if (subs == "char" || subs == "int" || subs == "float" )                            //classify type_spec
        					tokenObjectPs.push_back(new ensc251::type_spec(subs,linecounter,index1));
        				else if (subs =="=" || subs == "/=" || subs == "+=" || subs =="-=")                          //classify gen_assignment_operator
        					tokenObjectPs.push_back(new ensc251::gen_assignment_operator(subs,linecounter,index1));
        				else if (subs =="%=" || subs == "<<=" || subs == ">>=")                                           //classify int_assignment_operator
        					tokenObjectPs.push_back(new ensc251::int_assignment_operator(subs,linecounter,index1));
        				else if (subs =="?")
        					tokenObjectPs.push_back(new ensc251::conditional_operator(subs,linecounter,index1));
        				else if (subs =="<<" || subs == ">>")
        					tokenObjectPs.push_back(new ensc251::shift_operator(subs,linecounter,index1));
        				else if (subs == "+" || subs == "-")
        					tokenObjectPs.push_back(new ensc251::additive_operator(subs,linecounter,index1));
        				else if (subs == "/")
        					tokenObjectPs.push_back(new ensc251::div_operator(subs,linecounter,index1));
        				else if (subs == "%")
        					tokenObjectPs.push_back(new ensc251::mod_operator(subs,linecounter,index1));
        				else if (subs == "~")
        					tokenObjectPs.push_back(new ensc251::comp_operator(subs,linecounter,index1));
        				else if (subs == "++" || subs == "--")
        					tokenObjectPs.push_back(new ensc251::postfix_operator(subs,linecounter, index1));
        				else if (subs[0] == '\"' && subs[u-1] == '\"')
        					tokenObjectPs.push_back(new ensc251::string(subs,linecounter,index1));


        				 //ID check section
        				//***************************************************************
        				else if((isalpha(subs[0])||subs[0]=='_') && subs != "int" && subs != "float" && subs != "char")
        						{


        							if(std::find(result.begin(), result.end(), "*") != result.end())
        							{
        								tokenObjectPs.push_back(new ensc251::pointer_id(subs,linecounter,index1));
        							}
        							if(std::find(result.begin(), result.end(), "int") != result.end() && std::find(result.begin(), result.end(), "*") == result.end())
        							{
        								tokenObjectPs.push_back(new ensc251::int_id(subs,linecounter,index1));
        							}
        							if(std::find(result.begin(), result.end(), "char") != result.end() && std::find(result.begin(), result.end(), "*") == result.end())
        							{
        								tokenObjectPs.push_back(new ensc251::int_id(subs,linecounter,index1));
        							}
        							if(std::find(result.begin(), result.end(), "float") != result.end() && std::find(result.begin(), result.end(), "*") == result.end())
        							{
        								tokenObjectPs.push_back(new ensc251::numeric_id(subs,linecounter,index1));
        							}




        				}
        				//************************************************************************************
        				else if((std::isdigit(subs[0]) && subs.find("E") != std::string::npos)||
        						(std::isdigit(subs[0]) && subs.find("e") != std::string::npos))
        				{
        					float value = stof(subs.c_str()); //convert a string called subs to float called value
        					tokenObjectPs.push_back(new ensc251::numeric_const<float>(value,linecounter,index1));
        				}
//***************************************************************************************************************************

        				else if((subs[0] == '0'|| subs[0] == '1' || subs[0] == '2'|| subs[0] == '3' || subs[0] == '4' ||subs[0] == '5'
        						|| subs[0] == '6' || subs[0] == '7' || subs[0] == '8' || subs[0] == '9') && (subs.find(".") == std::string::npos) )
        					{
        					int value = atoi(subs.c_str()); //convert a string called subs to integer called value

        					tokenObjectPs.push_back(new ensc251::numeric_int_const<int>(value,linecounter,index1));
        					}
        				else if((subs[0] == '0'|| subs[0] == '1' || subs[0] == '2'|| subs[0] == '3' || subs[0] == '4' ||subs[0] == '5'
        						|| subs[0] == '6' || subs[0] == '7' || subs[0] == '8' || subs[0] == '9'|| subs[0] == '\'') && (subs.find(".") != std::string::npos) )
        					{
        					float value = stof(subs.c_str()); //convert a string called subs to float called value

        					tokenObjectPs.push_back(new ensc251::numeric_const<float>(value,linecounter,index1));
        					}
        				else if (subs[0] == '\'' && subs[2] == '\'')
        					tokenObjectPs.push_back(new ensc251::numeric_int_const<char>(subs[1],linecounter,index1));

        				else if (subs[0] == '\"' && subs[u-1] != '\"')
        					tokenObjectPs.push_back(new ensc251::incorrect(subs,linecounter,index1));

        				else
        					tokenObjectPs.push_back(new ensc251::incorrect(subs,linecounter,index1));


        		continue;
        			}

        			for (auto delim : delim3) {
        				if (line.substr(index1, 3) == delim) {
        					std::string subs = line.substr(index1,3);
        					result.push_back(subs);
            				std::string::size_type i = result.size();
            				std::string::size_type u = subs.size();
            				if (subs ==";" || subs == "*" || subs == ":" || subs == "(" || subs == ")")
            					tokenObjectPs.push_back(new ensc251::punctuator(subs,linecounter,index1));

            				else if(subs== "errno")

            					tokenObjectPs.push_back(new ensc251::int_id(subs,linecounter,index1));

            				else if (subs == "char" || subs == "int" || subs == "float" )
            					tokenObjectPs.push_back(new ensc251::type_spec(subs,linecounter,index1));
            				else if (subs =="=" || subs == "/=" || subs == "+=" || subs =="-=")
            					tokenObjectPs.push_back(new ensc251::gen_assignment_operator(subs,linecounter,index1));
            				else if (subs =="%=" || subs == "<<=" || subs == ">>=")
            					tokenObjectPs.push_back(new ensc251::int_assignment_operator(subs,linecounter,index1));
            				else if (subs =="?")
            					tokenObjectPs.push_back(new ensc251::conditional_operator(subs,linecounter,index1));
            				else if (subs =="<<" || subs == ">>")
            					tokenObjectPs.push_back(new ensc251::shift_operator(subs,linecounter,index1));
            				else if (subs == "+" || subs == "-")
            					tokenObjectPs.push_back(new ensc251::additive_operator(subs,linecounter,index1));
            				else if (subs == "/")
            					tokenObjectPs.push_back(new ensc251::div_operator(subs,linecounter,index1));
            				else if (subs == "%")
            					tokenObjectPs.push_back(new ensc251::mod_operator(subs,linecounter,index1));
            				else if (subs == "~")
            					tokenObjectPs.push_back(new ensc251::comp_operator(subs,linecounter,index1));
            				else if (subs == "++" || subs == "--")
            					tokenObjectPs.push_back(new ensc251::postfix_operator(subs,linecounter, index1));
            				else if (subs[0] == '\"' && subs[u-1] == '\"')
            					tokenObjectPs.push_back(new ensc251::string(subs,linecounter,index1));


            				//ID check section
            				//***************************************************************
            				else if((isalpha(subs[0])||subs[0]=='_') && subs != "int" && subs != "float" && subs != "char")
            						{


            							if(std::find(result.begin(), result.end(), "*") != result.end())
            							{
            								tokenObjectPs.push_back(new ensc251::pointer_id(subs,linecounter,index1));
            							}
            							if(std::find(result.begin(), result.end(), "int") != result.end() && std::find(result.begin(), result.end(), "*") == result.end())
            							{
            								tokenObjectPs.push_back(new ensc251::int_id(subs,linecounter,index1));
            							}
            							if(std::find(result.begin(), result.end(), "char") != result.end() && std::find(result.begin(), result.end(), "*") == result.end())
            							{
            								tokenObjectPs.push_back(new ensc251::int_id(subs,linecounter,index1));
            							}
            							if(std::find(result.begin(), result.end(), "float") != result.end() && std::find(result.begin(), result.end(), "*") == result.end())
            							{
            								tokenObjectPs.push_back(new ensc251::numeric_id(subs,linecounter,index1));
            							}

            					}

            				//********************************************************************
            				else if(subs== errorind)
            				{
            					tokenObjectPs.push_back(new ensc251::int_id(subs,linecounter,index1));
            				}
            			      //************************************************************************************8
            				else if((std::isdigit(subs[0]) && subs.find("E") != std::string::npos)||
            						(std::isdigit(subs[0]) && subs.find("e") != std::string::npos))
            				{
            					float value = stof(subs.c_str()); //convert a string called subs to float called value
            					tokenObjectPs.push_back(new ensc251::numeric_const<float>(value,linecounter,index1));
            				}
    //***************************************************************************************************************************


            				else if((subs[0] == '0'|| subs[0] == '1' || subs[0] == '2'|| subs[0] == '3' || subs[0] == '4' ||subs[0] == '5'
            						|| subs[0] == '6' || subs[0] == '7' || subs[0] == '8' || subs[0] == '9') && (subs.find(".") == std::string::npos) )
            					{
            					int value = atoi(subs.c_str()); //convert a string called subs to integer called value

            					tokenObjectPs.push_back(new ensc251::numeric_int_const<int>(value,linecounter,index1));
            					}
            				else if((subs[0] == '0'|| subs[0] == '1' || subs[0] == '2'|| subs[0] == '3' || subs[0] == '4' ||subs[0] == '5'
            						|| subs[0] == '6' || subs[0] == '7' || subs[0] == '8' || subs[0] == '9'|| subs[0] == '\'') && (subs.find(".") != std::string::npos) )
            					{
            					float value = stof(subs.c_str()); //convert a string called subs to float called value

            					tokenObjectPs.push_back(new ensc251::numeric_const<float>(value,linecounter,index1));
            					}
            				else if (subs[0] == '\'' && subs[2] == '\'')
            					tokenObjectPs.push_back(new ensc251::numeric_int_const<char>(subs[1],linecounter,index1));
            				else if (subs[0] == '\"' && subs[u-1] != '\"')
            					tokenObjectPs.push_back(new ensc251::incorrect(subs,linecounter,index1));
            				else
            					tokenObjectPs.push_back(new ensc251::incorrect(subs,linecounter,index1));

        					index2 = index1 + 3;
        					break;
        				}
        			}
        			if (index2 > index1) continue;

        			// the below follows the pattern from the lines above.
        			for (auto delim : delim2) {
        				if (line.substr(index1, 2) == delim) {
        					std::string subs = line.substr(index1,2);


        					result.push_back(subs);

            				std::string::size_type i = result.size();
            				std::string::size_type u = subs.size();
            				if (subs ==";" || subs == "*" || subs == ":" || subs == "(" || subs == ")")
            					tokenObjectPs.push_back(new ensc251::punctuator(subs,linecounter,index1));


            				else if(subs== "errno")

            					tokenObjectPs.push_back(new ensc251::int_id(subs,linecounter,index1));

            				else if (subs == "char" || subs == "int" || subs == "float" )
            					tokenObjectPs.push_back(new ensc251::type_spec(subs,linecounter,index1));
            				else if (subs =="=" || subs == "/=" || subs == "+=" || subs =="-=")
            					tokenObjectPs.push_back(new ensc251::gen_assignment_operator(subs,linecounter,index1));
            				else if (subs =="%=" || subs == "<<=" || subs == ">>=")
            					tokenObjectPs.push_back(new ensc251::int_assignment_operator(subs,linecounter,index1));
            				else if (subs =="?")
            					tokenObjectPs.push_back(new ensc251::conditional_operator(subs,linecounter,index1));
            				else if (subs =="<<" || subs == ">>")
            					tokenObjectPs.push_back(new ensc251::shift_operator(subs,linecounter,index1));
            				else if (subs == "+" || subs == "-")
            					tokenObjectPs.push_back(new ensc251::additive_operator(subs,linecounter,index1));
            				else if (subs == "/")
            					tokenObjectPs.push_back(new ensc251::div_operator(subs,linecounter,index1));
            				else if (subs == "%")
            					tokenObjectPs.push_back(new ensc251::mod_operator(subs,linecounter,index1));
            				else if (subs == "~")
            					tokenObjectPs.push_back(new ensc251::comp_operator(subs,linecounter,index1));
            				else if (subs == "++" || subs == "--")
            					tokenObjectPs.push_back(new ensc251::postfix_operator(subs,linecounter, index1));
            				else if (subs[0] == '\"' && subs[u-1] == '\"')
            					tokenObjectPs.push_back(new ensc251::string(subs,linecounter,index1));


            				 //ID check section
            				//***************************************************************
            				else if((isalpha(subs[0])||subs[0]=='_') && subs != "int" && subs != "float" && subs != "char")
            						{


            							if(std::find(result.begin(), result.end(), "*") != result.end())
            							{
            								tokenObjectPs.push_back(new ensc251::pointer_id(subs,linecounter,index1));
            							}
            							if(std::find(result.begin(), result.end(), "int") != result.end() && std::find(result.begin(), result.end(), "*") == result.end())
            							{
            								tokenObjectPs.push_back(new ensc251::int_id(subs,linecounter,index1));
            							}
            							if(std::find(result.begin(), result.end(), "char") != result.end() && std::find(result.begin(), result.end(), "*") == result.end())
            							{
            								tokenObjectPs.push_back(new ensc251::int_id(subs,linecounter,index1));
            							}
            							if(std::find(result.begin(), result.end(), "float") != result.end() && std::find(result.begin(), result.end(), "*") == result.end())
            							{
            								tokenObjectPs.push_back(new ensc251::numeric_id(subs,linecounter,index1));
            							}

            					}

            				//********************************************************************
            				//********************************************************************for case 5

            				else if(subs== errorind)
            				{
            					tokenObjectPs.push_back(new ensc251::int_id(subs,linecounter,index1));
            				}
            				//************************************************************************************8
            				else if((std::isdigit(subs[0]) && subs.find("E") != std::string::npos)||
            						(std::isdigit(subs[0]) && subs.find("e") != std::string::npos))
            				{
            					float value = stof(subs.c_str()); //convert a string called subs to float called value
            					tokenObjectPs.push_back(new ensc251::numeric_const<float>(value,linecounter,index1));
            				}
    //***************************************************************************************************************************


            				else if((subs[0] == '0'|| subs[0] == '1' || subs[0] == '2'|| subs[0] == '3' || subs[0] == '4' ||subs[0] == '5'
            						|| subs[0] == '6' || subs[0] == '7' || subs[0] == '8' || subs[0] == '9') && (subs.find(".") == std::string::npos) )
            					{
            					int value = atoi(subs.c_str()); //convert a string called subs to integer called value

            					tokenObjectPs.push_back(new ensc251::numeric_int_const<int>(value,linecounter,index1));
            					}
            				else if((subs[0] == '0'|| subs[0] == '1' || subs[0] == '2'|| subs[0] == '3' || subs[0] == '4' ||subs[0] == '5'
            						|| subs[0] == '6' || subs[0] == '7' || subs[0] == '8' || subs[0] == '9'|| subs[0] == '\'') && (subs.find(".") != std::string::npos) )
            					{
            					float value = stof(subs.c_str()); //convert a string called subs to float called value

            					tokenObjectPs.push_back(new ensc251::numeric_const<float>(value,linecounter,index1));
            					}
            				else if (subs[0] == '\'' && subs[2] == '\'')
            					tokenObjectPs.push_back(new ensc251::numeric_int_const<char>(subs[1],linecounter,index1));


            				else if (subs[0] == '\"' && subs[u-1] != '\"')
            					tokenObjectPs.push_back(new ensc251::incorrect(subs,linecounter,index1));
            				else
            					tokenObjectPs.push_back(new ensc251::incorrect(subs,linecounter,index1));



        					index2 = index1 + 2;
        					break;
        				}
        			}

        			if (index2 > index1) continue;

        			if (line[index1] == '"') {
        				index2 = index1 + 1;
        				while (index2 < length) {
        					if (line[index2] == '\\') {
        						index2++; // skip over escape character
        					}
        					else
        						if (line.at(index2) == '"') {

        							//***************************string classifier*********************************
        							result.push_back(line.substr(index1, index2 + 1 - index1));
        							std::string subs = line.substr(index1, index2+1 -index1);
        							tokenObjectPs.push_back(new ensc251::string(subs,linecounter,index1));
        							//****************************************************************************
        							break;
        						}
        					index2++;
        				}
        				if (index2 >= length) { // String is not terminated!
        					//cout << "Error:  string is not terminated!" << endl;
        					result.push_back(line.substr(index1, index2+1-index1));

        					//case 8
        					std::string subs = line.substr(index1,index2+1-index1);
        					tokenObjectPs.push_back(new ensc251::incorrect(subs,linecounter,index1));
        					//case 8

        					break;
        				}
        				index2++; // non-empty string was pushed on the result vector
        				continue;
        			}

        			if (line[index1] == '\'') {
        				index2 = index1 + 1;
        				if (index2 < length && line[index2] == '\\') {
        					index2++; // skip over escape character
        				}
        				index2++; // skip over character
        				if (index2 >= length || line.at(index2) != '\'') // Character constant is not terminated!
        				{
        					cout << "Error:  character constant is not terminated!" << endl;
        					result.push_back("");
        					tokenObjectPs.push_back(new ensc251::incorrect("",0,1));
        				}
        				else {
        					index2++;
        					result.push_back(line.substr(index1,index2-index1));

        					std::string subs = line.substr(index1,index2-index1);
            				std::string::size_type i = result.size();
            				std::string::size_type u = subs.size();
            				if (subs ==";" || subs == "*" || subs == ":" || subs == "(" || subs == ")")
            					tokenObjectPs.push_back(new ensc251::punctuator(subs,linecounter,index1));
            				//********************************************************************

            				else if(subs== "errno")

            					tokenObjectPs.push_back(new ensc251::int_id(subs,linecounter,index1));

            				else if (subs == "char" || subs == "int" || subs == "float" )
            					tokenObjectPs.push_back(new ensc251::type_spec(subs,linecounter,index1));
            				else if (subs =="=" || subs == "/=" || subs == "+=" || subs =="-=")
            					tokenObjectPs.push_back(new ensc251::gen_assignment_operator(subs,linecounter,index1));
            				else if (subs =="%=" || subs == "<<=" || subs == ">>=")
            					tokenObjectPs.push_back(new ensc251::int_assignment_operator(subs,linecounter,index1));
            				else if (subs =="?")
            					tokenObjectPs.push_back(new ensc251::conditional_operator(subs,linecounter,index1));
            				else if (subs =="<<" || subs == ">>")
            					tokenObjectPs.push_back(new ensc251::shift_operator(subs,linecounter,index1));
            				else if (subs == "+" || subs == "-")
            					tokenObjectPs.push_back(new ensc251::additive_operator(subs,linecounter,index1));
            				else if (subs == "/")
            					tokenObjectPs.push_back(new ensc251::div_operator(subs,linecounter,index1));
            				else if (subs == "%")
            					tokenObjectPs.push_back(new ensc251::mod_operator(subs,linecounter,index1));
            				else if (subs == "~")
            					tokenObjectPs.push_back(new ensc251::comp_operator(subs,linecounter,index1));
            				else if (subs == "++" || subs == "--")
            					tokenObjectPs.push_back(new ensc251::postfix_operator(subs,linecounter, index1));
            				else if (subs[0] == '\"' && subs[u-1] == '\"')
            					tokenObjectPs.push_back(new ensc251::string(subs,linecounter,index1));


            				else if (result[i-1] == "*" && result[i-2] == "char")
            					tokenObjectPs.push_back(new ensc251::pointer_id(subs,linecounter,index1));
            				else if (result[i-2] == "int" || result[i-2] == "char")
            					tokenObjectPs.push_back(new ensc251::int_id(subs,linecounter,index1));
            				else if (result[i-2] == "float")
            					tokenObjectPs.push_back(new ensc251::numeric_id(subs,linecounter,index1));  //ID check section

            				else if((isalpha(subs[0])||subs[0]=='_')  && subs != "int" && subs != "float" && subs != "char")
            						{


            							if(std::find(result.begin(), result.end(), "*") != result.end())
            							{
            								tokenObjectPs.push_back(new ensc251::pointer_id(subs,linecounter,index1));
            							}
            							if(std::find(result.begin(), result.end(), "int") != result.end() && std::find(result.begin(), result.end(), "*") == result.end())
            							{
            								tokenObjectPs.push_back(new ensc251::int_id(subs,linecounter,index1));
            							}
            							if(std::find(result.begin(), result.end(), "char") != result.end() && std::find(result.begin(), result.end(), "*") == result.end())
            							{
            								tokenObjectPs.push_back(new ensc251::int_id(subs,linecounter,index1));
            							}
            							if(std::find(result.begin(), result.end(), "float") != result.end() && std::find(result.begin(), result.end(), "*") == result.end())
            							{
            								tokenObjectPs.push_back(new ensc251::numeric_id(subs,linecounter,index1));
            							}

            					}

            				//********************************************************************
            				//********************************************************************for case 5

            				else if(subs== errorind)
            				{
            					tokenObjectPs.push_back(new ensc251::int_id(subs,linecounter,index1));
            				}
            				//************************************************************************************8
            				else if((std::isdigit(subs[0]) && subs.find("E") != std::string::npos)||
            						(std::isdigit(subs[0]) && subs.find("e") != std::string::npos))
            				{
            					float value = stof(subs.c_str()); //convert a string called subs to float called value
            					tokenObjectPs.push_back(new ensc251::numeric_const<float>(value,linecounter,index1));
            				}
    //***************************************************************************************************************************


            				else if((subs[0] == '0'|| subs[0] == '1' || subs[0] == '2'|| subs[0] == '3' || subs[0] == '4' ||subs[0] == '5'
            						|| subs[0] == '6' || subs[0] == '7' || subs[0] == '8' || subs[0] == '9') && (subs.find(".") == std::string::npos) )
            					{
            					int value = atoi(subs.c_str()); //convert a string called subs to integer called value

            					tokenObjectPs.push_back(new ensc251::numeric_int_const<int>(value,linecounter,index1));
            					}
            				else if((subs[0] == '0'|| subs[0] == '1' || subs[0] == '2'|| subs[0] == '3' || subs[0] == '4' ||subs[0] == '5'
            						|| subs[0] == '6' || subs[0] == '7' || subs[0] == '8' || subs[0] == '9'|| subs[0] == '\'') && (subs.find(".") != std::string::npos) )
            					{
            					float value = stof(subs.c_str()); //convert a string called subs to float called value

            					tokenObjectPs.push_back(new ensc251::numeric_const<float>(value,linecounter,index1));
            					}
            				else if (subs[0] == '\'' && subs[2] == '\'')
            					tokenObjectPs.push_back(new ensc251::numeric_int_const<char>(subs[1],linecounter,index1));
            				else if (subs[0] == '\"' && subs[u-1] != '\"')
            					tokenObjectPs.push_back(new ensc251::incorrect(subs,linecounter,index1));
            				else
            					tokenObjectPs.push_back(new ensc251::incorrect(subs,linecounter,index1));


        				}
        				continue;
        			}

        			if(line[index1]=='_' || isalpha(line[index1])) {
        				index2 = index1 + 1;
        				while (index2 < length && (line[index2]=='_' || isalnum(line[index2])))
        					index2 ++;
        				std::string subs = line.substr(index1, index2-index1);

        				result.push_back(subs);

        				std::string::size_type i = result.size();
        				std::string::size_type u = subs.size();
        				if (subs ==";" || subs == "*" || subs == ":" || subs == "(" || subs == ")")
        					tokenObjectPs.push_back(new ensc251::punctuator(subs,linecounter,index1));
        				//********************************************************************for case 5

        				else if(subs== "errno")

        					tokenObjectPs.push_back(new ensc251::int_id(subs,linecounter,index1));

        				else if (subs == "char" || subs == "int" || subs == "float" )
        					tokenObjectPs.push_back(new ensc251::type_spec(subs,linecounter,index1));
        				else if (subs =="=" || subs == "/=" || subs == "+=" || subs =="-=")
        					tokenObjectPs.push_back(new ensc251::gen_assignment_operator(subs,linecounter,index1));
        				else if (subs =="%=" || subs == "<<=" || subs == ">>=")
        					tokenObjectPs.push_back(new ensc251::int_assignment_operator(subs,linecounter,index1));
        				else if (subs =="?")
        					tokenObjectPs.push_back(new ensc251::conditional_operator(subs,linecounter,index1));
        				else if (subs =="<<" || subs == ">>")
        					tokenObjectPs.push_back(new ensc251::shift_operator(subs,linecounter,index1));
        				else if (subs == "+" || subs == "-")
        					tokenObjectPs.push_back(new ensc251::additive_operator(subs,linecounter,index1));
        				else if (subs == "/")
        					tokenObjectPs.push_back(new ensc251::div_operator(subs,linecounter,index1));
        				else if (subs == "%")
        					tokenObjectPs.push_back(new ensc251::mod_operator(subs,linecounter,index1));
        				else if (subs == "~")
        					tokenObjectPs.push_back(new ensc251::comp_operator(subs,linecounter,index1));
        				else if (subs == "++" || subs == "--")
        					tokenObjectPs.push_back(new ensc251::postfix_operator(subs,linecounter, index1));
        				else if (subs[0] == '\"' && subs[u-1] == '\"')
        					tokenObjectPs.push_back(new ensc251::string(subs,linecounter,index1));


        				  //ID check section
        				//***************************************************************
        				else if((isalpha(subs[0])||subs[0]=='_')  && subs != "int" && subs != "float" && subs != "char")
        						{


        							if(std::find(result.begin(), result.end(), "*") != result.end())
        							{
        								tokenObjectPs.push_back(new ensc251::pointer_id(subs,linecounter,index1));
        							}
        							if(std::find(result.begin(), result.end(), "int") != result.end() && std::find(result.begin(), result.end(), "*") == result.end())
        							{
        								tokenObjectPs.push_back(new ensc251::int_id(subs,linecounter,index1));
        							}
        							if(std::find(result.begin(), result.end(), "char") != result.end() && std::find(result.begin(), result.end(), "*") == result.end())
        							{
        								tokenObjectPs.push_back(new ensc251::int_id(subs,linecounter,index1));
        							}
        							if(std::find(result.begin(), result.end(), "float") != result.end() && std::find(result.begin(), result.end(), "*") == result.end())
        							{
        								tokenObjectPs.push_back(new ensc251::numeric_id(subs,linecounter,index1));
        							}

        					}


        				//********************************************************************
        				//********************************************************************for case 5

        				else if(subs== errorind)
        				{
        					tokenObjectPs.push_back(new ensc251::int_id(subs,linecounter,index1));
        				}
        				//************************************************************************************
        				else if((std::isdigit(subs[0]) && subs.find("E") != std::string::npos)||
        						(std::isdigit(subs[0]) && subs.find("e") != std::string::npos))
        				{
        					float value = stof(subs.c_str()); //convert a string called subs to float called value
        					tokenObjectPs.push_back(new ensc251::numeric_const<float>(value,linecounter,index1));
        				}
//***************************************************************************************************************************


        				else if((subs[0] == '0'|| subs[0] == '1' || subs[0] == '2'|| subs[0] == '3' || subs[0] == '4' ||subs[0] == '5'
        						|| subs[0] == '6' || subs[0] == '7' || subs[0] == '8' || subs[0] == '9') && (subs.find(".") == std::string::npos) )
        					{
        					int value = atoi(subs.c_str()); //convert a string called subs to integer called value

        					tokenObjectPs.push_back(new ensc251::numeric_int_const<int>(value,linecounter,index1));
        					}
        				else if((subs[0] == '0'|| subs[0] == '1' || subs[0] == '2'|| subs[0] == '3' || subs[0] == '4' ||subs[0] == '5'
        						|| subs[0] == '6' || subs[0] == '7' || subs[0] == '8' || subs[0] == '9'|| subs[0] == '\'') && (subs.find(".") != std::string::npos) )
        					{
        					float value = stof(subs.c_str()); //convert a string called subs to float called value

        					tokenObjectPs.push_back(new ensc251::numeric_const<float>(value,linecounter,index1));
        					}
        				else if (subs[0] == '\'' && subs[2] == '\'')
        					tokenObjectPs.push_back(new ensc251::numeric_int_const<char>(subs[1],linecounter,index1));

        				else if (subs[0] == '\"' && subs[u-1] != '\"')
        					tokenObjectPs.push_back(new ensc251::incorrect(subs,linecounter,index1));

        				else
        					tokenObjectPs.push_back(new ensc251::incorrect(subs,linecounter,index1));


        				continue;
        			}

        			index2 = line.find_first_of(delim1andWhite, index1);
        			if (index2 == index1) {


        				index2++; // we found a 1-character token
        				std::string subs = line.substr(index1,index2-index1);

        				result.push_back(subs);
        				std::string::size_type i = result.size();
        				std::string::size_type u = subs.size();
        				if (subs ==";" || subs == "*" || subs == ":" || subs == "(" || subs == ")")
        					tokenObjectPs.push_back(new ensc251::punctuator(subs,linecounter,index1));
        				//********************************************************************for case 5

        				else if(subs== "errno")

        					tokenObjectPs.push_back(new ensc251::int_id(subs,linecounter,index1));

        				else if (subs == "char" || subs == "int" || subs == "float" )
        					tokenObjectPs.push_back(new ensc251::type_spec(subs,linecounter,index1));
        				else if (subs =="=" || subs == "/=" || subs == "+=" || subs =="-=")
        					tokenObjectPs.push_back(new ensc251::gen_assignment_operator(subs,linecounter,index1));
        				else if (subs =="%=" || subs == "<<=" || subs == ">>=")
        					tokenObjectPs.push_back(new ensc251::int_assignment_operator(subs,linecounter,index1));
        				else if (subs =="?")
        					tokenObjectPs.push_back(new ensc251::conditional_operator(subs,linecounter,index1));
        				else if (subs =="<<" || subs == ">>")
        					tokenObjectPs.push_back(new ensc251::shift_operator(subs,linecounter,index1));
        				else if (subs == "+" || subs == "-")
        					tokenObjectPs.push_back(new ensc251::additive_operator(subs,linecounter,index1));
        				else if (subs == "/")
        					tokenObjectPs.push_back(new ensc251::div_operator(subs,linecounter,index1));
        				else if (subs == "%")
        					tokenObjectPs.push_back(new ensc251::mod_operator(subs,linecounter,index1));
        				else if (subs == "~")
        					tokenObjectPs.push_back(new ensc251::comp_operator(subs,linecounter,index1));
        				else if (subs == "++" || subs == "--")
        					tokenObjectPs.push_back(new ensc251::postfix_operator(subs,linecounter, index1));
        				else if (subs[0] == '\"' && subs[u-1] == '\"')
        					tokenObjectPs.push_back(new ensc251::string(subs,linecounter,index1));


        				//ID check section
        				//***************************************************************
        				else if((isalpha(subs[0])||subs[0]=='_')  && subs != "int" && subs != "float" && subs != "char")
        						{


        							if(std::find(result.begin(), result.end(), "*") != result.end())
        							{
        								tokenObjectPs.push_back(new ensc251::pointer_id(subs,linecounter,index1));
        							}
        							if(std::find(result.begin(), result.end(), "int") != result.end() && std::find(result.begin(), result.end(), "*") == result.end())
        							{
        								tokenObjectPs.push_back(new ensc251::int_id(subs,linecounter,index1));
        							}
        							if(std::find(result.begin(), result.end(), "char") != result.end() && std::find(result.begin(), result.end(), "*") == result.end())
        							{
        								tokenObjectPs.push_back(new ensc251::int_id(subs,linecounter,index1));
        							}
        							if(std::find(result.begin(), result.end(), "float") != result.end() && std::find(result.begin(), result.end(), "*") == result.end())
        							{
        								tokenObjectPs.push_back(new ensc251::numeric_id(subs,linecounter,index1));
        							}

        					}

        				//********************************************************************
        				//********************************************************************for case 5

        				else if(subs== errorind)
        				{
        					tokenObjectPs.push_back(new ensc251::int_id(subs,linecounter,index1));
        				}
        				//************************************************************************************8
        				else if((std::isdigit(subs[0]) && subs.find("E") != std::string::npos)||
        						(std::isdigit(subs[0]) && subs.find("e") != std::string::npos))
        				{
        					float value = stof(subs.c_str()); //convert a string called subs to float called value
        					tokenObjectPs.push_back(new ensc251::numeric_const<float>(value,linecounter,index1));
        				}
//***************************************************************************************************************************


        				else if((subs[0] == '0'|| subs[0] == '1' || subs[0] == '2'|| subs[0] == '3' || subs[0] == '4' ||subs[0] == '5'
        						|| subs[0] == '6' || subs[0] == '7' || subs[0] == '8' || subs[0] == '9') && (subs.find(".") == std::string::npos) )
        					{
        					int value = atoi(subs.c_str()); //convert a string called subs to integer called value

        					tokenObjectPs.push_back(new ensc251::numeric_int_const<int>(value,linecounter,index1));
        					}
        				else if((subs[0] == '0'|| subs[0] == '1' || subs[0] == '2'|| subs[0] == '3' || subs[0] == '4' ||subs[0] == '5'
        						|| subs[0] == '6' || subs[0] == '7' || subs[0] == '8' || subs[0] == '9'|| subs[0] == '\'') && (subs.find(".") != std::string::npos) )
        					{
        					float value = stof(subs.c_str()); //convert a string called subs to float called value

        					tokenObjectPs.push_back(new ensc251::numeric_const<float>(value,linecounter,index1));
        					}
        				else if (subs[0] == '\'' && subs[2] == '\'')
        					tokenObjectPs.push_back(new ensc251::numeric_int_const<char>(subs[1],linecounter,index1));




        				else if (subs[0] == '"' && subs[u-1] != '"')
        					tokenObjectPs.push_back(new ensc251::incorrect(subs,linecounter,index1));

        				else
        					tokenObjectPs.push_back(new ensc251::incorrect(subs,linecounter,index1));

        				continue;
        			}
        			//cout << "Error -- invalid token in input: " <<
        					//line.substr(index1, index2 - index1) << endl;
        			std::string subs = line.substr(index1,index2-index1);
        			result.push_back(subs);
    				std::string::size_type i = result.size();
    				std::string::size_type u = subs.size();
    				if (subs ==";" || subs == "*" || subs == ":" || subs == "(" || subs == ")")
    					tokenObjectPs.push_back(new ensc251::punctuator(subs,linecounter,index1));
    				//********************************************************************for case 5

    				else if(subs== "errno")

    					tokenObjectPs.push_back(new ensc251::int_id(subs,linecounter,index1));
    				else if (subs == "char" || subs == "int" || subs == "float" )
    					tokenObjectPs.push_back(new ensc251::type_spec(subs,linecounter,index1));
    				else if (subs =="=" || subs == "/=" || subs == "+=" || subs =="-=")
    					tokenObjectPs.push_back(new ensc251::gen_assignment_operator(subs,linecounter,index1));
    				else if (subs =="%=" || subs == "<<=" || subs == ">>=")
    					tokenObjectPs.push_back(new ensc251::int_assignment_operator(subs,linecounter,index1));
    				else if (subs =="?")
    					tokenObjectPs.push_back(new ensc251::conditional_operator(subs,linecounter,index1));
    				else if (subs =="<<" || subs == ">>")
    					tokenObjectPs.push_back(new ensc251::shift_operator(subs,linecounter,index1));
    				else if (subs == "+" || subs == "-")
    					tokenObjectPs.push_back(new ensc251::additive_operator(subs,linecounter,index1));
    				else if (subs == "/")
    					tokenObjectPs.push_back(new ensc251::div_operator(subs,linecounter,index1));
    				else if (subs == "%")
    					tokenObjectPs.push_back(new ensc251::mod_operator(subs,linecounter,index1));
    				else if (subs == "~")
    					tokenObjectPs.push_back(new ensc251::comp_operator(subs,linecounter,index1));
    				else if (subs == "++" || subs == "--")
    					tokenObjectPs.push_back(new ensc251::postfix_operator(subs,linecounter, index1));
    				else if (subs[0] == '\"' && subs[u-1] == '\"')
    					tokenObjectPs.push_back(new ensc251::string(subs,linecounter,index1));



    				  //ID check section
    				//***************************************************************
    				else if((isalpha(subs[0])||subs[0]=='_') && subs != "int" && subs != "float" && subs != "char")
    						{


    							if(std::find(result.begin(), result.end(), "*") != result.end())
    							{
    								tokenObjectPs.push_back(new ensc251::pointer_id(subs,linecounter,index1));
    							}
    							if(std::find(result.begin(), result.end(), "int") != result.end() && std::find(result.begin(), result.end(), "*") == result.end())
    							{
    								tokenObjectPs.push_back(new ensc251::int_id(subs,linecounter,index1));
    							}
    							if(std::find(result.begin(), result.end(), "char") != result.end() && std::find(result.begin(), result.end(), "*") == result.end())
    							{
    								tokenObjectPs.push_back(new ensc251::int_id(subs,linecounter,index1));
    							}
    							if(std::find(result.begin(), result.end(), "float") != result.end() && std::find(result.begin(), result.end(), "*") == result.end())
    							{
    								tokenObjectPs.push_back(new ensc251::numeric_id(subs,linecounter,index1));
    							}

    					}

    				//********************************************************************
    				//********************************************************************for case 5

    				else if(subs== errorind)
    				{
    					tokenObjectPs.push_back(new ensc251::int_id(subs,linecounter,index1));
    				}
    				//************************************************************************************8
    				else if((std::isdigit(subs[0]) && subs.find("E") != std::string::npos)||
    						(std::isdigit(subs[0]) && subs.find("e") != std::string::npos))
    				{
    					float value = stof(subs.c_str()); //convert a string called subs to float called value
    					tokenObjectPs.push_back(new ensc251::numeric_const<float>(value,linecounter,index1));
    				}
//***************************************************************************************************************************


    				else if((subs[0] == '0'|| subs[0] == '1' || subs[0] == '2'|| subs[0] == '3' || subs[0] == '4' ||subs[0] == '5'
    						|| subs[0] == '6' || subs[0] == '7' || subs[0] == '8' || subs[0] == '9') && (subs.find(".") == std::string::npos) )
    					{
    					int value = atoi(subs.c_str()); //convert a string called subs to integer called value

    					tokenObjectPs.push_back(new ensc251::numeric_int_const<int>(value,linecounter,index1));
    					}
    				else if((subs[0] == '0'|| subs[0] == '1' || subs[0] == '2'|| subs[0] == '3' || subs[0] == '4' ||subs[0] == '5'
    						|| subs[0] == '6' || subs[0] == '7' || subs[0] == '8' || subs[0] == '9'|| subs[0] == '\'') && (subs.find(".") != std::string::npos) )
    					{
    					float value = stof(subs.c_str()); //convert a string called subs to float called value

    					tokenObjectPs.push_back(new ensc251::numeric_const<float>(value,linecounter,index1));
    					}
    				else if (subs[0] == '\'' && subs[2] == '\'')
    					tokenObjectPs.push_back(new ensc251::numeric_int_const<char>(subs[1],linecounter,index1));



    				else if (subs[0] == '\"' && subs[u-1] != '\"')
    					tokenObjectPs.push_back(new ensc251::incorrect(subs,linecounter,index1));
    				else
    					tokenObjectPs.push_back(new ensc251::incorrect(subs,linecounter,index1));

        		}
        		linecounter++;
        	}

        return tokenObjectPs;


}
