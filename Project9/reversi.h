//留希哲
//2018/9/20
//黑白棋遊戲
#pragma once

#include <iostream> 
//#include <string> 
#include <vector>
using namespace std;
#define TABLE_LEN 8		//遊戲面板大小 8*8

class Point {		//紀錄x,y座標，紀錄哪些點的子應該翻面。
public:
	Point(void) {
		x = 0;
		y = 0;
	}
	Point(int X, int Y) {
		x = X;
		y = Y;
	}
	int x, y;
};

class Reversi
{
public:
	Reversi(void);
	~Reversi(void);

	char white = '●';
	char black = '○';
	//  NowFlag 判斷現在是黑子下或白子下	
	//  whiteNum/blackNum 紀錄目前白子數量和黑子數量
	//	rowIndex/colIndex 判斷現在是那些按件被案下(row : a~h	col : 1~8)
	int NowFlag = 1, whiteNum = 2, blackNum = 2, rowIndex = -1, colIndex = -1;

	short table[TABLE_LEN][TABLE_LEN] = { 0 };	//顯示遊戲面板 1 : 黑子		2 : 白子
	short tmptable[TABLE_LEN][TABLE_LEN] = { 0 };
	void restart() {	//重新開始遊戲，初始化所有參數
		Reversi tmp;
		for (int i = 0; i < TABLE_LEN; i++) {
			for (int j = 0; j < TABLE_LEN; j++) {
				table[i][j] = tmp.table[i][j];
			}
		}
		setTmpTable();
		NowFlag = 1;
		whiteNum = 2;
		blackNum = 2;
	}
	void UndoOrRedo(Reversi tmp) {		//Undo 或 Redo
		NowFlag = tmp.NowFlag;
		whiteNum = tmp.whiteNum;
		blackNum = tmp.blackNum;
		rowIndex = tmp.rowIndex;
		colIndex = tmp.colIndex;
		for (int i = 0; i < TABLE_LEN; i++)
			for (int j = 0; j < TABLE_LEN; j++)
				table[i][j] = tmp.table[i][j];
	}
	void setTmpTable() {
		int row = TABLE_LEN / 2, col = TABLE_LEN / 2;
		tmptable[row + 1][col - 1] = -1;
		tmptable[row - 1][col + 1] = -1;
		tmptable[row][col - 2] = -1;
		tmptable[row - 2][col] = -1;
	}
	void initTmpTable() {
		for (int i = 0; i < TABLE_LEN; i++) {
			for (int j = 0; j < TABLE_LEN; j++) {
				tmptable[i][j] = 0;
			}
		}
	}

	int checkPos(int rowi, int coli, int mode) {		//判斷是否為有效的落子，mode : 1 為落子檢查
														//  mode : 0 檢查下子方有無落子點
		int eated = NowFlag > 0 ? 2 : 1;	// NowFlag 為1時 白子為被吃方
		int eat = NowFlag > 0 ? 1 : 2;		// NowFlag 為1時 黑子為吃方
		int i = rowi, j = coli, f = 0, F = 0;
		int eatedNum = 0;	//此次落子吃掉對面的數量
		vector<Point> L, R, U, D, LU, RU, LD, RD;	//紀錄被吃得子的座標
		Point tmp;
		while (++i < TABLE_LEN) {	//往下搜索
			if (table[i][coli] == 0 || (table[i][coli] == eat && !f)) break;
			else if (table[i][coli] == eated) {
				tmp.y = i;
				tmp.x = coli;
				D.push_back(tmp);
				f = 1;
			}
			else if (table[i][coli] == eat && f) {
				f = 0;
				F = 1;
				if (mode) {
					table[rowi][coli] = eat;
					for (int k = 0; k < D.size(); k++)
						table[D[k].y][D[k].x] = eat;
				}
				eatedNum += D.size();
				break;
			}
		}
		f = 0;
		i = rowi;
		while (--i >= 0) {	//往上搜索
			if (table[i][j] == 0 || (table[i][j] == eat && !f)) break;
			else if (table[i][j] == eated) {
				tmp.y = i;
				tmp.x = j;
				U.push_back(tmp);
				f = 1;
			}
			else if (table[i][j] == eat && f) {
				f = 0;
				F = 1;
				if (mode) {
					table[rowi][coli] = eat;
					for (int k = 0; k < U.size(); k++)
						table[U[k].y][U[k].x] = eat;
				}
				eatedNum += U.size();
				break;
			}
		}
		f = 0;
		i = rowi;
		while (--j >= 0) {	//往左搜索
			if (table[i][j] == 0 || (table[i][j] == eat && !f)) break;
			else if (table[i][j] == eated) {
				tmp.y = i;
				tmp.x = j;
				L.push_back(tmp);
				f = 1;
			}
			else if (table[i][j] == eat && f) {
				f = 0;
				F = 1;
				if (mode) {
					table[rowi][coli] = eat;
					for (int k = 0; k < L.size(); k++)
						table[L[k].y][L[k].x] = eat;
				}
				eatedNum += L.size();
				break;
			}
		}
		j = coli;
		f = 0;
		while (++j < TABLE_LEN) {	//往右搜索
			if (table[i][j] == 0 || (table[i][j] == eat && !f)) break;
			else if (table[i][j] == eated) {
				tmp.y = i;
				tmp.x = j;
				R.push_back(tmp);
				f = 1;
			}
			else if (table[i][j] == eat && f) {
				f = 0;
				F = 1;
				if (mode) {
					table[rowi][coli] = eat;
					for (int k = 0; k < R.size(); k++)
						table[R[k].y][R[k].x] = eat;
				}
				eatedNum += R.size();
				break;
			}
		}
		j = coli;
		f = 0;
		while ((++j) < TABLE_LEN && (++i) < TABLE_LEN) {	//往右下搜索
			if (table[i][j] == 0 || (table[i][j] == eat && !f)) break;
			else if (table[i][j] == eated) {
				tmp.y = i;
				tmp.x = j;
				RD.push_back(tmp);
				f = 1;
			}
			else if (table[i][j] == eat && f) {
				f = 0;
				F = 1;
				if (mode) {
					table[rowi][coli] = eat;
					for (int k = 0; k < RD.size(); k++)
						table[RD[k].y][RD[k].x] = eat;
				}
				eatedNum += RD.size();
				break;
			}
		}
		i = rowi;
		j = coli;
		f = 0;
		while (++j < TABLE_LEN && --i >= 0) {	//往右上搜索
			if (table[i][j] == 0 || (table[i][j] == eat && !f)) break;
			else if (table[i][j] == eated) {
				tmp.y = i;
				tmp.x = j;
				RU.push_back(tmp);
				f = 1;
			}
			else if (table[i][j] == eat && f) {
				f = 0;
				F = 1;
				if (mode) {
					table[rowi][coli] = eat;
					for (int k = 0; k < RU.size(); k++)
						table[RU[k].y][RU[k].x] = eat;
				}
				eatedNum += RU.size();
				break;
			}
		}
		i = rowi;
		j = coli;
		f = 0;
		while (--j >= 0 && ++i < TABLE_LEN) {	//往左上搜索
			if (table[i][j] == 0 || (table[i][j] == eat && !f)) break;
			else if (table[i][j] == eated) {
				tmp.y = i;
				tmp.x = j;
				LU.push_back(tmp);
				f = 1;
			}
			else if (table[i][j] == eat && f) {
				f = 0;
				F = 1;
				if (mode) {
					table[rowi][coli] = eat;
					for (int k = 0; k < LU.size(); k++)
						table[LU[k].y][LU[k].x] = eat;
				}
				eatedNum += LU.size();
				break;
			}
		}
		i = rowi;
		j = coli;
		f = 0;
		while (--j >= 0 && --i >= 0) {	//往左下搜索
			if (table[i][j] == 0 || (table[i][j] == eat && !f)) break;
			else if (table[i][j] == eated) {
				tmp.y = i;
				tmp.x = j;
				LD.push_back(tmp);
				f = 1;
			}
			else if (table[i][j] == eat && f) {
				f = 0;
				F = 1;
				if (mode) {
					table[rowi][coli] = eat;
					for (int k = 0; k < LD.size(); k++)
						table[LD[k].y][LD[k].x] = eat;
				}
				eatedNum += LD.size();
				break;
			}
		}
		i = rowi;
		j = coli;
		f = 0;
		if (NowFlag > 0 && mode) {		//依照吃子數量，改變黑子數量和白子數量
			blackNum += eatedNum + F;
			whiteNum -= eatedNum;
		}
		else if (NowFlag < 0 && mode) {
			blackNum -= eatedNum;
			whiteNum += eatedNum + F;
		}
		if ((blackNum + whiteNum >= 64) || blackNum == 0 || whiteNum == 0) eatedNum = 64;	//判斷遊戲是否結束
		return eatedNum;
	}
	int ifPASS() {		//檢查落子方有無落子點
		int passf = 0, count = 0, max = 0;
		for (int i = 0; i < TABLE_LEN; i++) {
			for (int j = 0; j < TABLE_LEN; j++) {
				if (table[i][j] == 0) {
					passf = checkPos(i, j, 0);
					if (passf) tmptable[i][j] = -1;
				}
				if (max < passf) {
					max = passf;
					rowIndex = i;
					colIndex = j;
				}
			}
		}
		return max;
	}
};

