#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <set>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <random>
#include <unordered_set>
#include <unordered_map>
#include <climits>

using namespace std;

vector<vector<string>> board(8,vector<string>(8));
unordered_map<string,vector<pair<int,int>>> pieces;
string player;
int counter =0;
vector<vector<int>> allMoves;

//MVVLVA - most valuable victim, least valuable aggressor
//Row: MVV - [0] pawn, [1] knight, [2] bishop, [3] rook, [4] queen
//Col" LVA - [0] pawn, [1] knight, [2] bishop, [3] rook, [4] queen, [5] king
unordered_map<char,int> pieceIndex = {{'K',5},{'Q',4},{'R',3},{'B',2},{'k',1},{'P',0}};
//lowest number is K x p
vector<vector<int>> MVVLVA = {
   {6,  5,  4,  3,  2,  1},
   {12, 11, 10, 9,  8,  7},
   {18, 17, 16, 15, 14, 13},
   {24, 23, 22, 21, 20, 19},
   {30, 29, 28, 27, 26, 25},
   {36, 35, 34, 33, 32, 21}
};
void sortMovesMVVLVA()
{
	vector<vector<int>> sortedMoves;
	

	for (int i = 0; i < allMoves.size(); i++)
	{
		if (board[allMoves[i][2]][allMoves[i][3]]=="  ")
			sortedMoves.push_back({0,allMoves[i][0],allMoves[i][1],allMoves[i][2],allMoves[i][3]});
		else
		{
			int MVVLVA_row = pieceIndex[board[allMoves[i][2]][allMoves[i][3]][1]];//victim
			int MVVLVA_col = pieceIndex[board[allMoves[i][0]][allMoves[i][1]][1]];//attacker
			
			sortedMoves.push_back({MVVLVA[MVVLVA_row][MVVLVA_col],allMoves[i][0],allMoves[i][1],allMoves[i][2],allMoves[i][3]});
		}
	}
	sort(sortedMoves.rbegin(),sortedMoves.rend());
	allMoves.clear();
	for (int i = 0; i < sortedMoves.size(); i++)
	{
		allMoves.push_back({sortedMoves[i][1],sortedMoves[i][2],sortedMoves[i][3],sortedMoves[i][4]});
	}
	
	return;
}


void getMoves(int i, int j, char c, vector<pair<int,int>> move, vector<bool> allowed, int counter);
void getAllMoves(char c);

void initialize()
{
	
	string temp; temp.push_back(player[0]);

	string opp = temp=="w" ? "b" : "w";
	board[0][0]=opp+"R"; board[0][1]=opp+"k"; board[0][2]=opp+"B"; board[0][5]=opp+"B"; board[0][6]=opp+"k"; board[0][7]=opp+"R";
	board[7][0]=temp+"R"; board[7][1]=temp+"k"; board[7][2]=temp+"B";  board[7][5]=temp+"B"; board[7][6]=temp+"k"; board[7][7]=temp+"R";
	
	//board[4][0]=opp+"R";
	//board[4][4]=temp+"R";
	//board[4][5]=temp+"K";
	
	if (temp=="w")
	{
		board[0][3]=opp+"Q"; board[0][4]=opp+"K"; 
		board[7][3]=temp+"Q"; board[7][4]=temp+"K";
	}
	else
	{
		board[0][3]=opp+"K"; board[0][4]=opp+"Q"; 
		board[7][3]=temp+"K"; board[7][4]=temp+"Q";
	}
	
	for (int j= 0 ;j< 8; j++)
	{
		board[1][j]=opp+ "P";
		board[6][j]=temp + "P";
	}
	//board[0][3]="";
	//board[1][3]=opp + "Q";
	//board[1][1]=opp+"B";
	for (int i = 0 ;i < 8; i++)
	{
		for (int j= 0 ; j < 8; j++)
		{
			if (board[i][j]!="") pieces[board[i][j]].push_back({i,j});
			else board[i][j]="  ";
		}
	}
	return;
}

void boardState()
{
	cout << endl;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			cout << board[i][j] <<" ";
		}
		cout << endl;
	}
	return;
}

void makeMove(int i, int j, int movex, int movey, string temp, string temp2)
{
	board[i][j]="  ";
	pieces[temp].erase(remove(pieces[temp].begin(),pieces[temp].end(),make_pair(i,j)),pieces[temp].end());
	pieces[temp].push_back({movex,movey});
	if (temp2 != "  ") pieces[temp2].erase(remove(pieces[temp2].begin(),pieces[temp2].end(),make_pair(movex,movey)),pieces[temp2].end());
	board[movex][movey]=temp;
}

void backTrack(int i, int j, int movex, int movey, string temp, string temp2)
{
	board[i][j]=temp;
	pieces[temp].erase(remove(pieces[temp].begin(),pieces[temp].end(),make_pair(movex,movey)),pieces[temp].end());
	pieces[temp].push_back({i,j});
	if (temp2 != "  ") pieces[temp2].push_back({movex,movey});
	board[movex][movey]=temp2;
}

void pawnMoves(int i, int j, char c)
{
	char opp;
	if (c=='w') opp='b';
	else opp='w';
	
	if (board[i][j][0]==player[0]) //if the passed square is same colour as player,
	{
		if (i-1>=0&&j-1>=0) 
		{
			if (board[i-1][j-1][0]==opp) allMoves.push_back({i,j,i-1,j-1});
			//coord2.push_back({i-1,j-1});
		}
		if (i-1>=0&&j+1<8)
		{
			if (board[i-1][j+1][0]==opp) allMoves.push_back({i,j,i-1,j+1});
			//coord2.push_back({i-1,j+1});
		}
		if (i-1>=0&&board[i-1][j]=="  ") allMoves.push_back({i,j,i-1,j});
		if (i-2>=0&&board[i-2][j]=="  " && board[i-1][j]=="  " && i==6) allMoves.push_back({i,j,i-2,j});
		
	}
	else
	{
		if (i+1<8&&j-1>=0) 
		{
			if (board[i+1][j-1][0]==opp) allMoves.push_back({i,j,i+1,j-1});
			//coord2.push_back({i+1,j-1});
		}
		if (i+1<8&&j+1<8) 
		{
			if (board[i+1][j+1][0]==opp) allMoves.push_back({i,j,i+1,j+1});
			//coord2.push_back({i+1,j+1});
		}
		if (i+1<8&&board[i+1][j]=="  ") allMoves.push_back({i,j,i+1,j});
		if (i+2<8&&board[i+2][j]=="  " && board[i+1][j]=="  " && i==1) allMoves.push_back({i,j,i+2,j});
	}

	return;
}

vector<bool> allowed(4,true);
vector<pair<int,int>> bishopMove = {{-1,-1},{-1,1},{1,-1},{1,1}};
//vector<pair<int,int>> kingMove = {{-1,-1},{-1,1},{1,-1},{1,1},{0,-1},{-1,0},{1,0},{0,1}};
vector<pair<int,int>> rookMove = {{0,-1},{-1,0},{1,0},{0,1}};
vector<pair<int,int>> knightMove={{-1,-2},{-1,2},{1,2},{1,-2}};
void knightMoves(int i, int j, char c)
{
	char opp;
	if (c=='w') opp='b';
	else opp='w';
	for (int a = 0; a < knightMove.size(); a++)
	{
		int x1= i+knightMove[a].first;
		int y1 = j+knightMove[a].second;
		int x2= i+knightMove[a].second;
		int y2 = j+knightMove[a].first;
		if (x1>=0 && x1 < 8 && y1>=0 && y1 < 8) if (board[x1][y1]=="  "||board[x1][y1][0]==opp) allMoves.push_back({i,j,x1,y1});
		if (x2>=0 && x2 < 8 && y2>=0 && y2 < 8) if (board[x2][y2]=="  "||board[x2][y2][0]==opp) allMoves.push_back({i,j,x2,y2});
	}
	//sort(coord.begin(),coord.end());
	return;
}

void bishopMoves(int i, int j, char c)
{
	getMoves(i,j,c,bishopMove,allowed,1);
	return;
}


void rookMoves(int i, int j, char c)
{
	getMoves(i,j,c,rookMove,allowed,1);
	return;
}

void kingMoves(int i, int j, char c)
{
	bishopMoves(i,j,c);
	rookMoves(i,j,c);
	return;
}

void queenMoves(int i, int j, char c)
{
	bishopMoves(i,j,c);
	rookMoves(i,j,c);
	return;
}
/*
//test other pins
//pinning,checking
//pinning->if you move and results in check, then dont move
bool check(int i, int j, int movex, int movey, char c)
{
	//cout << "check" << endl;
	
	//make the move. check if king is attacked.
	string temp = board[i][j];
	string temp2 = board[movex][movey];
	
	makeMove(i,j,movex,movey,temp,temp2);
	//cout << "check: " << endl;
	//boardState();
	char opp;
	if (c=='w') opp='b';
	else opp='w';

	vector<vector<int>>moves = getAllMoves(opp,1);
	
	string king; //= c + "K";
	king.push_back(c);
	king += "K";
	
	bool check = false;
	//cout << pieces[king][0].first << " " << pieces[king][0].second << endl;
	for (int a = 0; a < moves.size(); a++)
	{
		if (moves[a][2]==pieces[king][0].first && moves[a][3]==pieces[king][0].second)
		{
			//cout << "THIS MOVE RESULTS IN CHECK" << endl;
			//cout << moves[a][0] << " " << moves[a][1] << " " << moves[a][2] << " " << moves[a][3] << endl;
			//boardState();
			//getchar();
			check = true;
			break;
		}
	}
	
	backTrack(i,j,movex,movey,temp,temp2);

	return check;

}
*/

void getAllMoves(char c)
{
	for (auto thing: pieces)
	{
		string piece = thing.first;
		if (piece[0]==c)//
		{
			if (piece[1]=='P') 
				for (auto& v: thing.second) 
					pawnMoves(v.first,v.second,c);
			if (piece[1]=='k') 
				for (auto& v: thing.second) 
					knightMoves(v.first,v.second,c); 
			if (piece[1]=='B') 
				for (auto& v: thing.second)
					bishopMoves(v.first,v.second,c);
			if (piece[1]=='Q')
				for (auto& v: thing.second)
					queenMoves(v.first,v.second,c); 
			if (piece[1]=='K')
				for (auto& v: thing.second)
					kingMoves(v.first,v.second,c);	
			if (piece[1]=='R') 
				for (auto& v: thing.second)
					rookMoves(v.first,v.second,c);
		}
	}
	
	return;
	
}


void getMoves(int i, int j, char c, vector<pair<int,int>> move, vector<bool> allowed, int counter)
{

	char opp;
	if (c=='w') opp='b';
	else opp='w';
	
	while (true)
	{
		int len = allMoves.size();
		for (int a = 0; a < move.size(); a++)
		{
			int x = i + move[a].first*counter;
			int y = j + move[a].second*counter;
			if (x>=0 && x < 8 && y>=0 && y < 8) 
			{
				if (allowed[a])
				{
					if (board[x][y]=="  ") 
					{
						allMoves.push_back({i,j,x,y});
					}
					else if (board[x][y][0]==opp)
					{
						allMoves.push_back({i,j,x,y});
						allowed[a]=false;
					}
					else 
						allowed[a]=false;
				}				
			}
		}
		counter++;
		if (allMoves.size()==len || board[i][j][1]=='K') break;
	}
	return;
}

vector<pair<int,int>> findPieces(string piece)
{
	if (counter%2==0) piece.insert(0,"w");
	else piece.insert(0,"b");

	return pieces[piece];
}

vector<vector<int>> pawnTable = {
 {0,  0,  0,  0,  0,  0,  0,  0},
{50, 50, 50, 50, 50, 50, 50, 50},
{10, 10, 20, 30, 30, 20, 10, 10},
 {5,  5, 10, 45, 45, 10,  5,  5},
 {0,  0,  0, 20, 20,  0,  0,  0},
 {5, -5,-10,  0,  0,-10, -5,  5},
 {5, 10, 10,-20,-20, 10, 10,  5},
 {0,  0,  0,  0,  0,  0,  0,  0}
};

vector<vector<int>> knightTable = {
{-50,-40,-30,-30,-30,-30,-40,-50},
{-40,-20,  0,  0,  0,  0,-20,-40},
{-30,  0, 10, 15, 15, 10,  0,-30},
{-30,  5, 15, 20, 20, 15,  5,-30},
{-30,  0, 15, 20, 20, 15,  0,-30},
{-30,  5, 10, 15, 15, 10,  5,-30},
{-40,-20,  0,  5,  5,  0,-20,-40},
{-50,-40,-30,-30,-30,-30,-40,-50},
};

vector<vector<int>> bishopTable = {
{-20,-10,-10,-10,-10,-10,-10,-20},
{-10,  0,  0,  0,  0,  0,  0,-10},
{-10,  0,  5, 10, 10,  5,  0,-10},
{-10,  5,  5, 10, 10,  5,  5,-10},
{-10,  0, 10, 10, 10, 10,  0,-10},
{-10, 10, 10, 10, 10, 10, 10,-10},
{-10,  5,  0,  0,  0,  0,  5,-10},
{-20,-10,-10,-10,-10,-10,-10,-20},
};
vector<vector<int>> rookTable = {
  {0,  0,  0,  0,  0,  0,  0,  0},
  {5, 10, 10, 10, 10, 10, 10,  5},
 {-5,  0,  0,  0,  0,  0,  0, -5},
 {-5,  0,  0,  0,  0,  0,  0, -5},
 {-5,  0,  0,  0,  0,  0,  0, -5},
 {-5,  0,  0,  0,  0,  0,  0, -5},
 {-5,  0,  0,  0,  0,  0,  0, -5},
  {0,  0,  0,  5,  5,  0,  0,  0}
};

vector<vector<int>> queenTable = {
{-20,-10,-10, -5, -5,-10,-10,-20},
{-10,  0,  0,  0,  0,  0,  0,-10},
{-10,  0,  5,  5,  5,  5,  0,-10},
 {-5,  0,  5,  5,  5,  5,  0, -5},
  {0,  0,  5,  5,  5,  5,  0, -5},
{-10,  5,  5,  5,  5,  5,  0,-10},
{-10,  0,  5,  0,  0,  0,  0,-10},
{-20,-10,-10, -5, -5,-10,-10,-20}
};
vector<vector<int>> kingTable = {
{-30,-40,-40,-50,-50,-40,-40,-30},
{-30,-40,-40,-50,-50,-40,-40,-30},
{-30,-40,-40,-50,-50,-40,-40,-30},
{-30,-40,-40,-50,-50,-40,-40,-30},
{-20,-30,-30,-40,-40,-30,-30,-20},
{-10,-20,-20,-20,-20,-20,-20,-10},
{20, 20,  0,  0,  0,  0, 20, 20},
{20, 30, 10,  0,  0, 10, 30, 20 },
};
//const int pawn = 95,bishop = 330,knight = 320,rook = 500,queen = 900,king = 4000;
int calculateScore()
{
	int ans = 0;
	int val = 0;
	//boardState();
	for (auto& thing: pieces)
	{
		string piece = thing.first;
		if (piece[1]=='P') 
		{
			for (auto& v : thing.second)
			{
				val += piece[0]=='b' ? (pawnTable[v.first][v.second] + 95):-(pawnTable[7-v.first][7-v.second] + 95);
			}
		}
		if (piece[1]=='k') 
		{
			for (auto& v : thing.second)
			{
				val += piece[0]=='b' ? (knightTable[v.first][v.second] + 320):-(knightTable[7-v.first][7-v.second] + 320);
			}
		}
		if (piece[1]=='B') 
		{
			for (auto& v : thing.second)
			{
				val += piece[0]=='b' ? (bishopTable[v.first][v.second] + 330):-(bishopTable[7-v.first][7-v.second] + 330);
			}
		}
		if (piece[1]=='R') 
		{
			for (auto& v : thing.second)
			{
				val += piece[0]=='b' ? (rookTable[v.first][v.second] + 500):-(rookTable[7-v.first][7-v.second] + 500);
			}
		}
		if (piece[1]=='Q') 
		{
			for (auto& v : thing.second)
			{
				val += piece[0]=='b' ? (queenTable[v.first][v.second] + 900):-(queenTable[7-v.first][7-v.second] + 900);
			}
		}
		if (piece[1]=='K') 
		{
			for (auto& v : thing.second)
			{
				val += piece[0]=='b' ? (kingTable[v.first][v.second] + 4000):-(kingTable[7-v.first][7-v.second] + 4000);
			}
		}
		
	}
	return val;

}

int minimax(int depth, int maxPlayer, int alpha, int beta)
{
	if (depth==0) 
		return calculateScore();
		
	int val = 0;

	if (maxPlayer)
	{
		val = INT_MIN;
		allMoves.clear();
		getAllMoves('b'); 
		sortMovesMVVLVA();
		vector<vector<int>> moveStore = allMoves;

		for (auto& thing: moveStore)
		{
			string temp = board[thing[0]][thing[1]];
			string temp2 = board[thing[2]][thing[3]];

			makeMove(thing[0],thing[1],thing[2],thing[3],temp,temp2);
			val = max(val,minimax(depth-1,0,alpha,beta));
			backTrack(thing[0],thing[1],thing[2],thing[3],temp,temp2);

			alpha=max(alpha,val);
			
			if (val >= beta) break;
		}
		return val;
	}
	else
	{
		val = INT_MAX;
		allMoves.clear();
		getAllMoves('w'); 
		sortMovesMVVLVA();
		vector<vector<int>> moveStore = allMoves;

		for (auto& thing: moveStore)
		{
			string temp = board[thing[0]][thing[1]];
			string temp2 = board[thing[2]][thing[3]];

			makeMove(thing[0],thing[1],thing[2],thing[3],temp,temp2);
			val = min(val,minimax(depth-1,1,alpha,beta));
			backTrack(thing[0],thing[1],thing[2],thing[3],temp,temp2);

			beta = min(beta,val);
			
			if (alpha >= val) break;
		}
		return val;
	}
	return 0;
}


vector<int> aiMove()//
{
	clock_t tStart = clock();
	int maxscore = INT_MIN;
	//if (player[0]=='w') maxscore = INT_MAX;
	//else maxscore = INT_MIN;
	
	cout << "Calculating best move..." << endl;
	char c;
	if (player[0]=='w') c = 'b';
	else c = 'w';
	allMoves.clear();
	getAllMoves(c);
	if (allMoves.size()==0) cout << "Game Over" << endl;
	int score = 0;

	sortMovesMVVLVA();
	vector<vector<int>> moveStore = allMoves;
	
	vector<int> bestMove;
	for (auto& thing: moveStore)
	{
		
		string temp = board[thing[0]][thing[1]];
		string temp2 = board[thing[2]][thing[3]];
		
		makeMove(thing[0],thing[1],thing[2],thing[3],temp,temp2);

		int maxPlayer = player[0]=='w' ? 0:1; 
		score = minimax(5,maxPlayer,INT_MIN,INT_MAX); //initial call
		
		backTrack(thing[0],thing[1],thing[2],thing[3],temp,temp2);
		if (score>maxscore)
		{
			//cout << "TRYING TO MAXIMIZE" << endl;
			maxscore = score;
			bestMove = thing;
		}
	}

	printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
	cout << "Press ENTER" << endl;
	return bestMove;
}



int main(void)
{
	//vector<vector<int>> allMoves;
	//cout<<"White or Black?" << endl;
	/*while (true)
	{
		cin >> player;
		for (int i =0 ; i < player.size(); i++)
			player[i]=tolower(player[i]);
		if (player=="white" || player=="w" || player=="black"||player=="b") break;
		else cout << "Enter valid response" << endl;
	}*/
	player="white";
	initialize();
	while(true)
	{
		boardState();
		cout << "COUNTER: " << counter << endl;
		//getchar();
		bool ai = false;
		if (counter%2==0)
		{
			cout << "White to move" << endl;
			if (player[0]=='b') ai=true;
		}
		else 
		{
			cout << "Black to move" << endl;
			if (player[0]=='w') ai=true;
		}
		if (ai)
		{

			vector<int> aiMovev = aiMove();

			string temp = board[aiMovev[0]][aiMovev[1]];
			string temp2 = board[aiMovev[2]][aiMovev[3]];
			
			makeMove(aiMovev[0],aiMovev[1],aiMovev[2],aiMovev[3],temp,temp2);
			if (temp2=="wK") 
			{
				boardState();
				cout << "GAME OVER" << endl;
				return 0;
			}
		}
		else
		{
			string piece;
			vector<pair<int,int>> coord;
			
			vector<pair<int,int>> moves;
			vector<pair<int,int>> filteredmoves;
			int choice = 1;
			int movechoice = 1;
			int pieceToMoveX = 0;
			int pieceToMoveY = 0;
			char c = player[0];

			while (true)
			{
				allMoves.clear();
				cout << endl;
				cout << "Which piece do you want to move? (P, k, B, R, Q, K)" << endl;
				while (true)
				{
					cin >> piece;
					
					coord=findPieces(piece);
					if (coord.size()==0)
					{
						cout << "Piece nonexistent. Pick another" << endl;
						continue;
					}
					if (piece=="P" || piece=="k" ||piece=="B" ||piece=="R" ||piece=="Q" ||piece=="K")break;
					//cout << "Enter case sensitive valid response" << endl;
				}
				
				choice=1;
				if (coord.size()>1)
				{
					cout << "Which one?" << endl;
					for (int i= 0; i < coord.size(); i++)
					{
						cout << i+1 << ": (" << coord[i].first << ", "<< coord[i].second << ")"<< endl;
					}
					while (true)
					{
						cin >> choice;
						if (choice>0 && choice<=coord.size()) 
						{
							
							break;
						}
						cout << "Pick valid choice." << endl;
					}
				}
				
				pieceToMoveX = coord[choice-1].first;
				pieceToMoveY = coord[choice-1].second;
				
				//cout<< "asdf" << endl;
				
				if (piece=="P") pawnMoves(pieceToMoveX,pieceToMoveY,c);
				if (piece=="k") knightMoves(pieceToMoveX,pieceToMoveY,c);
				if (piece=="B") bishopMoves(pieceToMoveX,pieceToMoveY,c);
				if (piece=="R") rookMoves(pieceToMoveX,pieceToMoveY,c);
				if (piece=="Q") queenMoves(pieceToMoveX,pieceToMoveY,c);
				if (piece=="K") kingMoves(pieceToMoveX,pieceToMoveY,c);
				
				
				cout << "Which move?" << endl;
				
				for (int i = 0; i < allMoves.size(); i++)
				{
					cout << i+1 << ": (" << allMoves[i][2] << ", " << allMoves[i][3] << ")" << endl;
				}
				if (allMoves.size()==0)
				{
					cout << "No legal moves. Repick piece." << endl;
					continue;
				}
				
				movechoice = 1;
				
				cin >> movechoice;
				if (movechoice>0&&movechoice<=allMoves.size())
				{
					
					break;
				}
				cout << "Move choice out of bounds. Pick valid choice." << endl;
			}
			
			string temp = board[pieceToMoveX][pieceToMoveY];
			string temp2 = board[allMoves[movechoice-1][2]][allMoves[movechoice-1][3]];
			
			makeMove(pieceToMoveX,pieceToMoveY,allMoves[movechoice-1][2],allMoves[movechoice-1][3],temp,temp2);
			
			if (temp2=="bK")
			{
				boardState();
				cout << "GAME OVER" << endl;
				return 0;
			}
			//if king moves then 
		}
		counter++;
		getchar();
	}

	return 0;
}
