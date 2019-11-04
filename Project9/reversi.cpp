#include "reversi.h"
#include <iostream> 
#include <vector>
using namespace std;

#define TABLE_LEN 8

Reversi::Reversi()
{
	/*for (int i = 0; i < 10; i++) table[i][0] = 1;
	table[0][1] = 2;
	table[0][3] = 2;*/
	int row = TABLE_LEN / 2, col = TABLE_LEN / 2;
	for (int i = 0; i < TABLE_LEN; i++) {
		for (int j = 0; j < TABLE_LEN; j++) {
			table[i][j] = 0;
		}
	}
	table[row - 1][col - 1] = 1;
	table[row - 1][col] = 2;
	table[row][col] = 1;
	table[row][col-1] = 2;
	setTmpTable();
}


Reversi::~Reversi()
{
}
