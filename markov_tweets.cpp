#include <cstdio>
#include <iostream>
#include <string>
#include <string.h>
#include <ostream>
#include <fstream>
#include <vector>
#include <time.h>

using namespace std;


//Unique word structure. Takes in unique string word and has a vector of successors (words that follow instances of unique word)
struct u_word {
	string word;
	vector<string> successors;
	
	//Constructor for unique word
	u_word(string w){
		word = w;
	}

	//Adding successors to successor vector
	void addSuccessor(string w){
		successors.push_back(w);
	}
	//Word getter
	string getWord(){
		return word;
	}
	//Successor getter
	vector<string> getSuccessors(){
		return successors;
	}
	//Function to retrieve a random successor, each word has equal probability and each word may appear more than once
	string getRandomSuccessor(){
		int index = rand() % successors.size();
		string randomSuccessor = successors[index];
		return randomSuccessor;
	}
	//Debug function to print out total successors
	void displaySuccessors(){
		if(successors.size() == 0) return;
		else{
			cout << "Successors: ";
			for(int i = 0; i < successors.size(); i++){
				if(i == successors.size() - 1) cout << successors[i] << endl;
				else cout << successors[i] << " - ";
			}
		}
	}

	

} ;

//Vector used to store unique words
vector<u_word> words;

//Debug function to display all unique words with all their successors
void displayDictionary(){
	for(int i = 0; i < words.size(); i++){
		cout << "Unique word: " << words[i].word << endl;
		words[i].displaySuccessors();
	}
}

//User input function to retrive filepath or twitter handle
string retrieveTextSource(){
	cout << "Please enter a filepath of a text file or twitter handle: ";
	string text_source;
	cin >> text_source;
	return text_source;
}


//Function that takes a word and traverses the entire unique word vector to see if it already exists, returns -1 if it is unique or it returns an index if it does.
int checkUniqueWord(string str){
	int index = -1;
	for(int i = 0; i < words.size(); i++){
		if(str.compare(words[i].word) == 0){
			index = i;
		}
	}

	return index;
}

//Takes in a sentence and returns an array of each word in that sentence without the whitespace between them
vector<string> getEachWord(string str){
	vector<string> tokens;
	char *token = strtok(const_cast<char*>(str.c_str()), " ");
	while(token){
		string s(token);
		tokens.push_back(s);
		token = strtok(NULL, " ");
	}
	return tokens;
}

//Markov chain text generator function. Uses unique word vector to generate a markov tweet and print it out.
void generateTweet(bool writeToFile){
	ofstream writeFile;
	ifstream checkFile;
	if(writeToFile){						//If you would like to write to a file

		int num = 0;
		string fileName = "textGen" + to_string(num) + ".txt";	//Check to see if a file has been written with same name
		checkFile.open(fileName);
		while(!checkFile.fail()){
			checkFile.close();
			num++;
			fileName = "textGen" + to_string(num) + ".txt";
			checkFile.open(fileName);
		}
		writeFile.open(fileName);				//Once unique name found, write.writing a makefile c++writing a makefile c++
		cout << "Writing to file: " << fileName << endl;
	}



	int index = rand() % words.size();           				//Retrieves a random unique word to start the Tweet
	int remaining_characters = 280;              				//Total number of remaining characters (280 in a tweet)
	bool successorPresent = false;		     				//Boolean value that is used to see if current unique word has at least one successor

	u_word unique = words[index];		     				//First unique word to be pulled to start tweet
	string word_to_print;

	while(remaining_characters > 0){                                        //While tweet is less than 280 characters
		if(successorPresent == false){                                  //If no sucessors are present...
			word_to_print = unique.word;                            //Use the unique word to print next word
		}
		if((word_to_print.length() + 1) > remaining_characters){        //If the length of the current word to print is greater than 280 word limit...
			cout << endl;			//Print newline and end tweet
			if(writeToFile){
				writeFile << "\n";
				writeFile.close();
			}
		       	break; 
		}
		cout << word_to_print << " ";		//Print out word
		if(writeToFile) writeFile << word_to_print << " ";
		remaining_characters -= (word_to_print.length() + 1);           //Subtract length of characters in word + whitespace from total character limit
		if(unique.successors.size() == 0){                              //If there are no successors....
			index = rand() % words.size();                          //Retrieve a random unique word index
			unique = words[index];                                  //Get the word
			successorPresent = false;                               //No successors present
		}
		else{								//Otherwise if there is at least one
			word_to_print = unique.getRandomSuccessor();            //Get a random successor
			index = checkUniqueWord(word_to_print);			//Set it as the current unique word
			unique = words[index];					//
			successorPresent = true;				//Indicate that there is a successor present
		}
			
	}
}

int main(){
	//Seed random number generator
	srand(time(NULL));
	//Get twitter handle or filepath
	string source = retrieveTextSource();
	
	//Check to see if a filepath or twitter handle is entered
	if(source[0] == '@' or source[0] == '/'){
		//cout << "Valid path or twitter handle, nice!" << endl;
		string pythonCommand = "python3 extract_tweets.py " + source;
        	cout << pythonCommand << endl;
        	system(pythonCommand.c_str());
	}
	else{
		cout << "Invalid path or twitter handle, try again!" << endl;
		return 0;
	}

	
	ifstream readFile;
	readFile.open("invalid.txt");    //Try to open an error file
	if(!readFile.fail()){            //If an error file actually exists, you entered an invalid path or twitter handle
		cout << "You have entered an invalid path or twitter handle, please try again!" << endl;
		readFile.close();
		return 0;
	}
	//If using a twitter handle, open the correct file
	else if(source[0] == '@'){
		string file = "text" + source + ".txt";
		readFile.open(file);
	}
	//If using filepath, open the markov.txt file
	else if(source[0] == '/'){
		string file = "markov.txt";
		readFile.open(file);
	}
	
	string line;

	//Code that reads cleaned file line by line and stores into vectors and unique word structues
	while(getline(readFile, line)){
		vector<string> wordVector = getEachWord(line);          //Get each word into a word vector from line
		//for(int i = 0; i < test.size(); i++){
		//	cout << test[i] << endl;
		//}
		for(int i = 0; i < wordVector.size(); i++){
			u_word unique = u_word(wordVector[i]);                  //Create unique word for each word
			int index = checkUniqueWord(unique.word);		//Check to see if word is unique
			if(index == -1){					//If unique...
				if(i != (wordVector.size() - 1)){		//and if there is a successor following the unique word...
					unique.addSuccessor(wordVector[i+1]);   //Add the successor to the unique words successor vector
				}
				//cout << unique.word << " ";
				//unique.displaySuccessors();
				words.push_back(unique);			//Finally, add the unique word to the unique word vector
			}
			else{							//Else, if the word exists already
				if(i != (wordVector.size() - 1)){		//Check to see if it has a successor following it
                                        words[index].addSuccessor(wordVector[i+1]);	//If so, add the successor to its successor index
                        	}
			}

		}

	}
	readFile.close();
	cout << "Data ingestion complete." << endl << endl;;
	cout << "Would you like to write tweet to a file? (y/n)" << endl;
	string write;
	cin >> write;
	
	while(write.compare("y") != 0 and write.compare("n") != 0){
		cout << "Invalid entry, try again." << endl;
		cout << "Would you like to write tweet to a file? (y/n)" << endl;
		cin >> write;
	}
	cout << endl;
	if(write.compare("y") == 0){
		generateTweet(true);
	}
	else{
		generateTweet(false);
	}

	
	//displayDictionary();
	

	//Finally, generate that tweet!
	//generateTweet(write);


}
