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
//%      or put ‘none’ if nobody helped (the two of) you.
//
// Helpers: _everybody helped us/me with the assignment (list names or put ‘none’)__
//          Craig , Mohammad
//
// Also, list any resources beyond the course textbook and the course pages on Piazza
// that you used in making your submission.
//
// Resources:  YouTube videos     SFU CMPT225 2017 Summer Lecture Note online ( Instructor: Anne Lavergne )
//
//%% Instructions:
//% * Put your name(s), student number(s), userid(s) in the above section.
//% * Enter the above information in any other files you are also submitting.
//% * Edit the "Helpers" line and "Resources" line.
//% * Your group name should be "P3_<userid1>_<userid2>" (eg. P3_stu1_stu2)
//% * Form groups as described at:  https://courses.cs.sfu.ca/docs/students
//% * Submit files to courses.cs.sfu.ca
//
// Name        : Directory.hpp
// Description : Course Project Part 3 (Directory Tree)
//============================================================================


#ifndef SRC_DIRECTORY_HPP_
#define SRC_DIRECTORY_HPP_

#include "TreeNode.hpp"
#include <iomanip>
#include <string>
#include <vector>

class Directory : public ensc251::TreeNode
{
	typedef std::vector<std::string> stringVect;

	private:
		std::string dir_name;
		stringVect file_names;

	public:
		Directory(std::string m_dir_name): dir_name(m_dir_name) {}
		Directory(std::string m_dir_name, stringVect m_file_names): dir_name(m_dir_name), file_names(m_file_names) {}
		virtual Directory* clone() const                                        //clone function for copy the directory node
		{
			Directory *temp = new Directory(dir_name, file_names);


			for (auto childP : childPVector)
				temp->add_childP(childP->clone());
			return temp;
		/* ***** Complete this member function ***** */ }



		void set_dir_name(const std::string& m_dir_name){ dir_name = m_dir_name;/* ***** Complete this member function ***** */ }
		const std::string & get_dir_name() { return dir_name; }
		void add_file(const std::string& m_fileName) { file_names.push_back(m_fileName); /* ***** Complete this member function ***** */ }
		void add_multiple_files(const stringVect& m_files) { file_names.insert(file_names.end(), m_files.begin(), m_files.end()); }
		const stringVect & get_files() const { return file_names; }

		void print_action()
		{
			// Do not modify insertion on OUT in this member function!
			OUT << std::setw(20) << dir_name+"\t|" << " ";
			OUT << this->childPVector.size() << "  ";
			for (auto file : file_names)
			{
				OUT << "  -" << file;
			}
			OUT << std::endl;
		}

		void print_traverse()
		{
			// ***** this needs work *****
			// ***** encode the rules in the instructions
			if(childPVector.size() < 2)                                                  //As requirement, traverse using pre-order when there are
			traverse_children_pre_order(                                                 //less then 2 chlderen
					static_cast<traverse_func>(&Directory::print_traverse),
					static_cast<action_func>(&Directory::print_action));
			else if (childPVector.size()==2)                                             //traverse using in-order when there are 2 children
			traverse_children_in_order(
					static_cast<traverse_func>(&Directory::print_traverse),
					static_cast<action_func>(&Directory::print_action));
			else if (childPVector.size()>2)                                              //travser using post-order when there are more than 2 children
			traverse_children_post_order(
					static_cast<traverse_func>(&Directory::print_traverse),
					static_cast<action_func>(&Directory::print_action));
		}
};

#endif /* SRC_DIRECTORY_HPP_ */
