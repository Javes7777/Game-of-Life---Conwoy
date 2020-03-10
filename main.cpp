#include<iostream>
#include<string>
#include<iomanip>
#include<cctype>
#include<sstream>
#include<fstream>
using namespace std;
const int MAX_ARRAY_SIZE = 50;

void InitGen(char lifeBoard[][MAX_ARRAY_SIZE], int& numRowsInBoard, int& numColsInBoard, int& generations);
void NextGen(char lifeBoard [][MAX_ARRAY_SIZE], int numRowsInBoard, int numColsInBoard, int generationNum);
void PrintGen(char lifeBoard[ ][MAX_ARRAY_SIZE], ostream& outStream, int numRowsInBoard,int numColsInBoard, int generationNum);
int org_counter(char lifeBoard[][MAX_ARRAY_SIZE], int Z, int Q);

int main() {
	char mylifeBoard[50][50];
	string temp;
	int numRowsInBoard, numColsInBoard, generations;
	
	InitGen(mylifeBoard , numRowsInBoard, numColsInBoard, generations);
	
	
	cout<<"Enter the name of the Output file: ";
	cin>>temp;
	
	ofstream myfout;
	myfout.open(temp);
	if(myfout.fail()) {
		cerr<<"ERROR: output file not opened correctly";
		abort();
	}
	
	PrintGen(mylifeBoard, cout, numRowsInBoard, numColsInBoard, 0);	//on console ...generations can be changed later 
	PrintGen(mylifeBoard, myfout, numRowsInBoard, numColsInBoard, 0);	//on file
	
	for(int i=1;i<=generations;i++) {
		
		NextGen(mylifeBoard, numRowsInBoard, numColsInBoard, i);
		PrintGen(mylifeBoard, myfout, numRowsInBoard, numColsInBoard, i);
			
		if(i==1 || i==generations) {
			PrintGen(mylifeBoard, cout, numRowsInBoard, numColsInBoard, i);
		}
	}
	myfout.close();
	return 0;
}



void InitGen(char lifeBoard[][50] , int& numRowsInBoard, int& numColsInBoard, int& generations) {
	string temp;		// to read in the name of input file
	ifstream fin;

	cout<<"Enter the name of the input file:";
	cin>>temp;
	fin.open(temp);
	if(fin.fail()) {
		cerr<<"ERROR: input file not opened correctly";
		abort();
	}
	fin>>ws;								//to eat up any leading whitespaces
	if(isdigit(fin.peek())) {				// to check if there is a number available to read
		fin>>numRowsInBoard;			    // reading number of rows
		if(numRowsInBoard<0 || numRowsInBoard>50) {
			cerr<<"ERROR: Read an illegal number of rows for the board";
			abort();
		}
	}
	else {
		cerr<<"ERROR: Cannot read number of rows";
		abort();
	}
	fin>>ws;								// again to eat up all leading white spaces
	if(isdigit(fin.peek())) {
		fin>>numColsInBoard;
		if(numColsInBoard<0 || numColsInBoard>50) {
			cerr<<"ERROR: Read an illegal number of columns for the board"<<endl;	
			abort();
		}
	}
	else {
		cerr<<"ERROR: Cannot read number of columns";
		abort();
	}
	fin>>ws;								// to eat up all the leading white spaces 
	if(isdigit(fin.peek())) {
		fin>>generations;
		if(generations<1)  {
			cerr<<"ERROR: Read an illegal number of generations"<<endl;
			abort();
		}
	}
	else {
		cerr<<"ERROR: Cannot read generations";
		abort();
	}

	temp.clear();
	getline(fin,temp);
	temp.clear();
	int i=0;
	
	while(i<numRowsInBoard) {
		getline(fin, temp);	//getting the first line
		//checking for non fatal errors
		if(temp.length() > numColsInBoard) {
			cerr<<"ERROR: Ignoring extra characters in the line "<<i<<" of input file"<<endl;
		}
		if(temp.length() < numColsInBoard) {
			cerr<<"ERROR: Not enough characters in the row "<<i<<" of input array";
			abort();
		}
		for(int j=0;j<numColsInBoard;j++) {
			
			lifeBoard[i][j] = temp[j];
			//checking for fatal errors:
			if(lifeBoard[i][j] != ' ' && lifeBoard[i][j] != 'X') {
				cerr<<"ERROR: Input data for initial board is incorrect"<<endl;
				cerr<<"Location ("<<i<<","<<j<<") is not valid"<<endl;
				abort();
			}
			else if((i==0 || i==numRowsInBoard-1) && lifeBoard[i][j]=='X') {
				cerr<<"ERROR: organisms are present in the border of the board, please correct your input file";
				abort();
			}
		}	
		//temp.clear();
		i++;
		if(fin.eof() == true) {
			cerr<<"Not enough characters in the row"<<i<<" of input array";
			abort();
		}
	}
	fin.close();
}

void NextGen(char lifeBoard [][MAX_ARRAY_SIZE], int numRowsInBoard, int numColsInBoard, int generationNum) {
	char nextGenBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE];		//initializing local board
	
	for(int j=0;j<numColsInBoard;j++) {
		nextGenBoard[0][j] = nextGenBoard[numRowsInBoard-1][j] = ' ';
		nextGenBoard[j][0] = nextGenBoard[j][numColsInBoard-1] = ' ';
	}	//making borders = ' ';
	
	for(int i=1;i<numRowsInBoard-1;i++) {
		for(int j=1;j<numColsInBoard-1;j++) {
			
			int count = org_counter(lifeBoard, i, j);
            
            if(count==3) 
            	nextGenBoard[i][j] = 'X';
			else if(count==2 && lifeBoard[i][j] == 'X') 
				nextGenBoard[i][j] = 'X';
			else
				nextGenBoard[i][j] = ' ';
		}
	}
	
	//copying into life Board:
	for(int i=0;i<numRowsInBoard;i++) {
		for(int j=0;j<numColsInBoard;j++) { 
			lifeBoard[i][j] = nextGenBoard[i][j];
		}
	}
	return;
}

void PrintGen(char lifeBoard[ ][MAX_ARRAY_SIZE], ostream& out, int numRowsInBoard,int numColsInBoard, int generationNum) {
	if(generationNum==0) 
		out<<"LIFE initial game board"<<endl;
	else 
		out	<<"LIFE gameboard: generation "<<generationNum<<endl;
		
	for(int j=0;j<numColsInBoard+12;j++) {
		out<<"_";	
	} out<<endl;
	
	out<<"|    |";
	
	stringstream ss;
	int count=0;
	for(int i=0;i<numColsInBoard;i++) {
		ss<<count;
		if(i%10==0 && i!=0) 
			count++;
	}
	out<<ss.str()<<"|    |"<<endl<<"|    |";
	
	ss.str("");
	count=0;
	for(int i=0;i<numColsInBoard;i++) {
		if(i%10==0 && i!=0) 
			count=0;
		ss<<count;
		count++;
	}	
	out<<ss.str()<<"|    |"<<endl<<"|____|";
	ss.str("");
	
	for(int b=0;b<numColsInBoard;b++) {
		out<<"_";
	}	out<<"|____|"<<endl;
	
	int i=0;
	while(i<numRowsInBoard) {
		out<<"|"<<setw(4)<<i<<"|";
		for(int j=0;j<numColsInBoard;j++) {			
			out<<lifeBoard[i][j];
		}
		out<<"|"<<setw(4)<<i<<"|"<<endl;
		i++;
	}	
	
	
	out<<"|____|";
	for(int j=0;j<numColsInBoard;j++) {
		out<<"_";
	}
	out<<"|____|";
	
	out<<endl<<endl<<endl;	//printing three blank lines to outStream
	return;
}

int org_counter(char lifeBoard[][MAX_ARRAY_SIZE], int Z, int Q) {
	//	This is an additional function made by the student.
	//	input -> the lifeBoard array, the current position needed to be evaluated by nextGen function.
	// 	output -> the number of organisms in the adjacent positions of the current organism at [Z][Q].
	int count=0;
	
	if(lifeBoard[Z+1][Q+1] == 'X')
		count++;
	if(lifeBoard[Z+1][Q] == 'X')
		count++;
	if(lifeBoard[Z+1][Q-1] == 'X')
		count++;
	if(lifeBoard[Z][Q-1] == 'X')
		count++;
	if(lifeBoard[Z][Q+1] == 'X')
		count++;
	if(lifeBoard[Z-1][Q-1] == 'X')
		count++;
	if(lifeBoard[Z-1][Q] == 'X')
		count++;
	if(lifeBoard[Z-1][Q+1] == 'X')
		count++;
		
	return count;
}
