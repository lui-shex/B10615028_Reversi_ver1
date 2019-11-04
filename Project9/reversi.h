//�d�ƭ�
//2018/9/20
//�¥մѹC��
#pragma once

#include <iostream> 
//#include <string> 
#include <vector>
using namespace std;
#define TABLE_LEN 8		//�C�����O�j�p 8*8

class Point {		//����x,y�y�СA���������I���l����½���C
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

	char white = '��';
	char black = '��';
	//  NowFlag �P�_�{�b�O�¤l�U�Υդl�U	
	//  whiteNum/blackNum �����ثe�դl�ƶq�M�¤l�ƶq
	//	rowIndex/colIndex �P�_�{�b�O���ǫ���Q�פU(row : a~h	col : 1~8)
	int NowFlag = 1, whiteNum = 2, blackNum = 2, rowIndex = -1, colIndex = -1;

	short table[TABLE_LEN][TABLE_LEN] = { 0 };	//��ܹC�����O 1 : �¤l		2 : �դl
	short tmptable[TABLE_LEN][TABLE_LEN] = { 0 };
	void restart() {	//���s�}�l�C���A��l�ƩҦ��Ѽ�
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
	void UndoOrRedo(Reversi tmp) {		//Undo �� Redo
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

	int checkPos(int rowi, int coli, int mode) {		//�P�_�O�_�����Ī����l�Amode : 1 �����l�ˬd
														//  mode : 0 �ˬd�U�l�観�L���l�I
		int eated = NowFlag > 0 ? 2 : 1;	// NowFlag ��1�� �դl���Q�Y��
		int eat = NowFlag > 0 ? 1 : 2;		// NowFlag ��1�� �¤l���Y��
		int i = rowi, j = coli, f = 0, F = 0;
		int eatedNum = 0;	//�������l�Y���ﭱ���ƶq
		vector<Point> L, R, U, D, LU, RU, LD, RD;	//�����Q�Y�o�l���y��
		Point tmp;
		while (++i < TABLE_LEN) {	//���U�j��
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
		while (--i >= 0) {	//���W�j��
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
		while (--j >= 0) {	//�����j��
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
		while (++j < TABLE_LEN) {	//���k�j��
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
		while ((++j) < TABLE_LEN && (++i) < TABLE_LEN) {	//���k�U�j��
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
		while (++j < TABLE_LEN && --i >= 0) {	//���k�W�j��
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
		while (--j >= 0 && ++i < TABLE_LEN) {	//�����W�j��
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
		while (--j >= 0 && --i >= 0) {	//�����U�j��
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
		if (NowFlag > 0 && mode) {		//�̷ӦY�l�ƶq�A���ܶ¤l�ƶq�M�դl�ƶq
			blackNum += eatedNum + F;
			whiteNum -= eatedNum;
		}
		else if (NowFlag < 0 && mode) {
			blackNum -= eatedNum;
			whiteNum += eatedNum + F;
		}
		if ((blackNum + whiteNum >= 64) || blackNum == 0 || whiteNum == 0) eatedNum = 64;	//�P�_�C���O�_����
		return eatedNum;
	}
	int ifPASS() {		//�ˬd���l�観�L���l�I
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

