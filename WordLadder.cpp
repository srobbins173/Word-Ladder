//
//  WordLadder.cpp
//
//  Created by Sumanth Lingala on 2010-02-15.
//  Copyright (c) 2011 Sumanth Lingla. All rights reserved.
//


#include<iostream>
#include<string>
#include<fstream>
#include<map>
#include<list>
#include<vector>
#include<cassert>
#include<cctype>

using namespace std;

int front = -1;			// Pointer for insertion in the front end of the queue
int rear = 0;			// Pointer for insertion at the rear of the queue
int p = -1;				// Pointer for pred[p]
int step_count = 0;		// Count the number of steps to get from input1 to input2


/*
 *	queue_push - This funtion updates the queue with all possible nodes that a given node can go to 
 *	Arguments:
 *		str1 - Push the current node ie., string into the bfsqueue with incrementing rear pointer
 *		bfsqueue - reference vector to bfsqueue
 *		value - the address of the parent of the given node
 *		prev - a vector array of nodes pointing to its parent
 *  Return Value 
 *		void
 */

void queue_push(const string str1, vector<string> &bfsqueue, const int &value, vector<int> &prev)
{
	bfsqueue.push_back(str1);
	prev.push_back(value); 
	p++;
	front++;
	return ;
}

/*
 *	queue_pop - This function pops(does not remove) the element from the front
 *	Arguments:
 *		bfsqueue - reference vector to bfsqueue for BFS(breath first search)
 *	Return Value:
 *		string - it returns the first node in the queue ie., a string
 */

string queue_pop(const vector<string> &bfsqueue)
{
	vector<string> tempvec;
	string tempstr;
	tempstr = bfsqueue[rear++];
	return(tempstr);
}	

/*
 *	printconnection - This funtion prints all the possible connection from input1 to input2
 *	Arguments:
 *		a - integer value pointing to the last node updated in the prev array
 *		bfsqueue - reference vector to bfsqueue
 *		prev - a vector array of nodes pointing to its parent
 * 	 Return Value 
 *		void
 *		
 *		This is a recursive funtion that holds a backtrack map to all its parents. Useful to reverse our path.
 */

void printconnection(int a, vector<string> &bfsqueue, vector<int> &prev)
{
	if(a == -1) 
		return;
	int b = prev[a];
	printconnection(b, bfsqueue, prev);
	step_count++;
	cout<<bfsqueue[a]<<" ==> ";
	return;
}

/*
bool comparefn(string s1, string s2)
{
	return s1.compare(s2);
}
*/

/*
 *	performbfs - This funtion does a breath first search on all the nodes that have a distance of 1 from it.
 *		Also, instead of scaning through the entire dictionary, I am changing the character one at a time and binary searching on it.
 *		So the total time taken is:
 *			m = length of the given input string (very small example: mall = 4)
 *			n = size of the dictionary
 *			26 letters in english
 *		For a 4 letter word(small letter words) =~ O (26*m log(n)) ie., m =4 =~ O(log n) worst case time to generate neighbours.
 *
 *	Arguments:
 *		dict - string vector containing all the words in the dictionary 
 *		str1 - input string 1
 *		str2 - input string 2
 *  	Return Value 
 *		void
 *		
 *		This funtion calls other functions such as queue_push, queue_pop based on requirement.
 */

void performbfs(vector<string> &dict, string &str1, string &str2)
{
	vector<int> prev;
	vector<string> bfsqueue;
	string temp, s, temp1;
	int string_size = str1.size();
	bool visit[dict.size()];
	std::vector<string>::iterator it;
	int noresult = 0;
	for(int k =0; k<dict.size(); k++)
		visit[k] = 0;
	queue_push(str1, bfsqueue, -1, prev);
	
	while((temp1 != str2) && (rear <= front)) {
		temp1 = temp = queue_pop(bfsqueue);
		if(temp.empty()) {
			cout<<"pop_empty"<<temp<<endl;
			exit(0);
		}
		for(int i=0;i < string_size; i++) {
			temp = temp1;
			for(int j=0;j<26;j++) {
				s = ('a' + j);
				temp.replace(i, 1, s);
				if( binary_search(dict.begin(), dict.end(), temp) ) {
					it = find(dict.begin(), dict.end(), temp);
					int index = it - dict.begin();
					if(!visit[index]) {
						queue_push(temp, bfsqueue, (rear -1), prev);
						visit[index] = 1;
					}
					if(temp == str2) {
						cout<<"String matched !!!!"<<endl;
						cout<<"Word Ladder is as follows:"<<endl;
						printconnection(p, bfsqueue, prev);
						cout<<"Number of steps: "<<step_count<<endl;
						cout<<endl;
						noresult = 1;
						goto ending;
					}
				}
			}
		}
	}
ending:
	if(!noresult)
		cout<<endl<<"A word ladder could not be constructed for the words"<<endl;
	prev.clear();
	bfsqueue.clear();
	front = -1;
	rear = 0;
	p = -1;
	step_count = 0;
	return ;
}

/*
 *	stolower - This funtion coverts the input word into lower case letter
 *	Arguments:
 *		s - input string
 *	Return Value:
 *		void
 *		This funtion checks if the string is in lowercase if not then it will convert into upper case.
 */

void stolower(string& s)
{
	int spot = -1;
	spot = s.find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ");			// Transforming string to lower case directly will save cpu cycles as
	if(spot == -1)								// we do not need to check for 26*m times. m - size of input string
		return;
	std::transform(s.begin(), s.end(), s.begin(), ::tolower);
	return;
}


/*
 *	isNull - This funtion check is the dictionary has no null values
 *	Arguments:
 *		s - input string
 *	Return Value:
 *		int
 */

int isNull(string &s)
{
	if(!s.compare(""))
		return 1;
	else
		return 0;
}

/*
 *	stringrange - This funtion checks if the string has only alphabets
 *	Arguments:
 *		s - input string
 *	Return Value:
 *		int
 */

int stringrange(string &s)
{
	string::iterator p;
	for(p=s.begin(); p != s.end(); ++p) {
		if (!((*p > 65  && *p < 91)|| (*p > 96 && *p < 123))) {
			cout<<"Not in range"<<endl;
			return 1;
		}
	}
	return 0;	
}

int main(int argc, char *argv[])
{
	ifstream inp(argv[1]);
	int dict_size;
	int i =0;

	string one_word;
	string istring1, istring2;
	vector<string> dict;
	clock_t start, end;

	if(!inp.is_open()) {
			cout<<"There was some error opening the file!!!!"<<endl;
			exit(0);
	}

	while(inp >> one_word){
		stolower(one_word);
		if(isNull(one_word)) {
			continue;
		}
		if(stringrange(one_word)) {
			continue;
		}
		if(i > 0)
			if((one_word.compare(dict[i-1])) <= 0)				//	Check if the input there are similar values 
				continue;
		dict.push_back(one_word);
		i++;
	}
	inp.close();
	dict_size = dict.size();
	cout<<"Number of dictionary words: "<<dict.size()<<endl;
	
	do {
		cout<<endl<<"Enter String1: ";
		cin>>istring1;
		cout<<"Enter String2: ";
		cin>>istring2;

		stolower(istring1);
		stolower(istring2);

		start = clock( );
		if(istring1.size() == 0 && istring2.size()==0) {			//	Check if both the input strings are empty or 
			cout<<"Empty strings"<<endl;					//	one them are empty.
			continue;													
		}
		
		if(istring1.size() != istring2.size()) {
			cout<<"Error !!!! Input string must have same size"<<endl;	//	Assumption: Given input strings will be of equal length.
			continue;
		}
	
		if(istring1 == istring2) {
			cout<<"Strings are Equal !!!"<<endl;				// Input strings are equal. Computation not required.
			continue;
		}
	
		if(!((binary_search(dict.begin(), dict.end(), istring1)) &&
			(binary_search(dict.begin(), dict.end(), istring2)))) {		// Check if one or more input strings exist in the dictionary.
			cout<<"Error!!!!  One or more input strings do not exist in the dictionary"<<endl;	
			continue;
		}
	
		cout<<istring1<< " and " <<istring2<<" exist in the dictionary !!"<<endl;
		cout<<"Performing BFS: "<<endl;
		
		performbfs(dict, istring1, istring2);
		end = clock( );
    		cout << "Elapsed time: " << double(end-start)/CLOCKS_PER_SEC << endl;
		cout<<endl;
		
	} while(1);	// Please change the value to 1 if needed to run many times
	
	return 0;
}
