#include <SDL.h>
#include <iostream>
#include <stdlib.h>
#include "Scene.h"
#include "reversi.h"
#include <SDL_image.h>
#include <SDL_mixer.h>
//#include <SDL_ttf.h>

using namespace std;
#define width 800
#define heigh 600
#define SnowNum 100
#define StarNum 10
#define tableSize 8
#define tableposx 300
#define tableposy 100
#define BWsize 50
#define offsetx 6
#define offsety 2
int indexAlpha = 0;
int mode = 0, ex = 0;
int Index = 0, ifwin = 0, pass = 0, back = 0;
vector<Reversi> tmp;

Point chang(int windowX, int windowY) {
	Point p;
	p.x = windowX / BWsize - offsetx;
	p.y = windowY / BWsize - offsety;
	return p;
}

void Transition(SDL_Surface * blackSurface, SDL_Texture * black, SDL_Renderer * rend, int alpha) {
	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = width;
	rect.h = heigh;
	SDL_SetTextureAlphaMod(black, alpha);
	SDL_RenderCopy(rend, black, NULL, &rect);
	SDL_RenderPresent(rend);
	SDL_Delay(50);
}

void Win(int w, SDL_Renderer *render, SDL_Texture * win, SDL_Rect WinR) {
	while (1) {
		SDL_RenderCopy(render, win, NULL, &WinR);
		SDL_RenderPresent(render);
		WinR.w += 5;
		if (WinR.w >= w) break;
		SDL_Delay(10);
	}
}

int main(int argc, char **argv) {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window *window = SDL_CreateWindow("SDL_Test",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED
		, width, heigh, SDL_WINDOW_SHOWN);
	SDL_Renderer *render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_Surface *BGsurface = IMG_Load("nightStar.png");
	SDL_Surface *BG2surface = IMG_Load("BG2.png");
	SDL_Surface *BG3surface = IMG_Load("BG3.png");
	SDL_Surface *blacksurface = IMG_Load("black.png");
	SDL_Surface *circlesurface = IMG_Load("circle.png");
	SDL_Surface *Bubblesurface = IMG_Load("Bubble.png");
	SDL_Surface *Starsurface = IMG_Load("star2.png");
	SDL_Surface *SnowSurface = IMG_Load("index.png");
	SDL_Surface *wordSurface = IMG_Load("ttf.png");
	SDL_Surface *playSurface = IMG_Load("play.png");
	SDL_Surface *AIsurface = IMG_Load("AImode.png");
	SDL_Surface *exitSurface = IMG_Load("exit.png");

	SDL_SetColorKey(wordSurface, SDL_TRUE, SDL_MapRGB(wordSurface->format, 0, 0, 0));
	SDL_SetColorKey(playSurface, SDL_TRUE, SDL_MapRGB(playSurface->format, 0, 0, 0));
	SDL_SetColorKey(AIsurface, SDL_TRUE, SDL_MapRGB(AIsurface->format, 0, 0, 0));
	SDL_SetColorKey(exitSurface, SDL_TRUE, SDL_MapRGB(exitSurface->format, 0, 0, 0));

	SDL_Texture *BG = SDL_CreateTextureFromSurface(render, BGsurface);
	SDL_Texture *BG2 = SDL_CreateTextureFromSurface(render, BG2surface);
	SDL_Texture *BG3 = SDL_CreateTextureFromSurface(render, BG3surface);
	SDL_Texture *black = SDL_CreateTextureFromSurface(render, blacksurface);
	SDL_Texture *circle = SDL_CreateTextureFromSurface(render, circlesurface);
	SDL_Texture *Bubble = SDL_CreateTextureFromSurface(render, Bubblesurface);
	SDL_Texture *starT = SDL_CreateTextureFromSurface(render, Starsurface);
	SDL_Texture *index = SDL_CreateTextureFromSurface(render, SnowSurface);
	SDL_Texture *word = SDL_CreateTextureFromSurface(render, wordSurface);
	SDL_Texture *play = SDL_CreateTextureFromSurface(render, playSurface);
	SDL_Texture *AI = SDL_CreateTextureFromSurface(render, AIsurface);
	SDL_Texture *exit = SDL_CreateTextureFromSurface(render, exitSurface);

	//TTF_Init();

	SDL_Rect BGrect;
	BGrect.x = 0;
	BGrect.y = 0;
	BGrect.h = heigh;
	BGrect.w = width;

	SDL_Rect WordRect;
	WordRect.x = 160;
	WordRect.y = 100;
	WordRect.h = wordSurface->h;
	WordRect.w = wordSurface->w;

	SDL_Rect WW;
	WW.x = tableposx;
	WW.y = tableposy;
	WW.w = TABLE_LEN * BWsize;
	WW.h = TABLE_LEN * BWsize;

	SDL_Rect SnowRect;
	SnowRect.x = 260;
	SnowRect.y = 200;
	SnowRect.h = SnowSurface->h;
	SnowRect.w = SnowSurface->w;

	SDL_Rect PlayRect;
	PlayRect.x = 160;
	PlayRect.y = 200;
	PlayRect.h = wordSurface->h;
	PlayRect.w = wordSurface->w;

	SDL_Rect AIRect;
	AIRect.x = 160;
	AIRect.y = 280;
	AIRect.h = wordSurface->h;
	AIRect.w = wordSurface->w;

	SDL_Rect ExitRect;
	ExitRect.x = 160;
	ExitRect.y = 360;
	ExitRect.h = wordSurface->h;
	ExitRect.w = wordSurface->w;

	

//SDL_SetTextureBlendMode(circle, SDL_BLENDMODE_BLEND);

	Spot spots[SnowNum];
	int i;
	for (i = 0; i < SnowNum; i++) {
		spots[i].x = rand() % width;
		spots[i].y = rand() % heigh;
		spots[i].r = rand() % 4 + 1;
		spots[i].speed = rand() % 2 + 1;
		spots[i].alpha = rand() % 255 + 10;
	}

	Star stars[StarNum];
	for (i = 0; i < StarNum; i++) {
		stars[i].x = rand() % 700;
		stars[i].y = rand() % 300;
		if (stars[i].x <= 770 && stars[i].x >= 600 && stars[i].y >= 50 && stars[i].y <= 220) 
			stars[i].y -= 100;
		stars[i].w = Starsurface->w;
		stars[i].h = Starsurface->h;
		stars[i].vspeed = rand() % 2 + 1;
		stars[i].unvspeed = rand() % 2 + 1;
	}

	Spot snows[10];
	for (i = 0; i < 10; i++) {
		snows[i].x = rand() % width;
		snows[i].y = rand() % heigh;
		snows[i].r = rand() % 10 + 20;
		snows[i].speed = rand() % 2 + 1;
		snows[i].alpha = rand() % 150 + 50;
	}
	SDL_Event event;

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	Mix_Chunk *click = Mix_LoadWAV("click.wav");
	Mix_Chunk *dom = Mix_LoadWAV("chrunk.wav");
	Mix_Music * bgm = Mix_LoadMUS("bgm.wav");
	Mix_PlayMusic(bgm, 10000);

	bool quit = false;
	while (!quit) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				quit = true;
				ex = 1;
			}
			else if (event.type == SDL_MOUSEMOTION) {
				if (event.motion.x <= 440 && event.motion.x >= 345
					&& event.motion.y <= 280 && event.motion.y >= 240) {
					indexAlpha = 255;
					SnowRect.y = 230;
					SnowRect.x = 260;
					PlayRect.w = playSurface->w + 20;
					PlayRect.h = playSurface->h + 5;
				}
				else if (event.motion.x <= 440 && event.motion.x >= 345
					&& event.motion.y <= 355 && event.motion.y >= 320) {		// AI mode
					indexAlpha = 253;
					SnowRect.y = 310;
					SnowRect.x = 220;
					AIRect.w = exitSurface->w + 20;
					AIRect.h = exitSurface->h + 5;
				}
				else if (event.motion.x <= 440 && event.motion.x >= 345
					&& event.motion.y <= 430 && event.motion.y >= 400) {
					indexAlpha = 254;
					SnowRect.y = 390;
					SnowRect.x = 260;
					ExitRect.w = exitSurface->w + 20;
					ExitRect.h = exitSurface->h + 5;
				}
				else {
					indexAlpha = 0;
					PlayRect.w = playSurface->w;
					PlayRect.h = playSurface->h;
					ExitRect.w = exitSurface->w;
					ExitRect.h = exitSurface->h;
					AIRect.w = exitSurface->w;
					AIRect.h = exitSurface->h;
				}
			}
			else if (event.type == SDL_MOUSEBUTTONDOWN) {
				if (indexAlpha == 254) {
					Mix_PlayChannel(-1, click, 0);
					quit = true;
					ex = 1;
				}
				else if (indexAlpha == 255) {
					Mix_PlayChannel(-1, click, 0);
					quit = true;
					mode = 1;
				}
				else if (indexAlpha == 253) {
					Mix_PlayChannel(-1, click, 0);
					quit = true;
					mode = 2;
				}
			}
			else if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == SDLK_1) {
					if (Mix_PlayingMusic()) {
						if (Mix_PausedMusic())
							Mix_ResumeMusic();
						else
							Mix_PauseMusic();
					}
				}
			}
		}
		if (!mode) {
			SDL_RenderCopy(render, BG, NULL, &BGrect);
			for (int i = 0; i < 10; i++) {
				snows->drawSpot(&snows[i], render, index);
				snows->moveSpot(&snows[i], 1);
			}
			SDL_SetTextureAlphaMod(index, indexAlpha);
			SDL_RenderCopy(render, index, NULL, &SnowRect);
			SDL_RenderCopy(render, word, NULL, &WordRect);
			SDL_RenderCopy(render, play, NULL, &PlayRect);
			SDL_RenderCopy(render, AI, NULL, &AIRect);
			SDL_RenderCopy(render, exit, NULL, &ExitRect);
			for (int i = 0; i < SnowNum; i++) {
				spots->drawSpot(&spots[i], render, circle);
				spots->moveSpot(&spots[i], 1);
			}
			for (int i = 0; i < StarNum; i++) {
				stars->drawStar(&stars[i], render, starT);
				stars->flashing(&stars[i]);
			}
			SDL_RenderPresent(render);
			SDL_Delay(10);
		}
	}
	int Talpha = 0, Tf = 0, count = 0;
	while (Tf == 0 && !ex) {
		Transition(blacksurface, black, render, Talpha);
		Talpha++;
		if (Talpha >= 50)
			Tf = 1;
	}
	quit = false;
	//Reversi table;
	SDL_Surface *Gsurface = IMG_Load("g.png");
	SDL_Surface *tGsurface = IMG_Load("tg.png");
	SDL_Surface *Bsurface = IMG_Load("b.png");
	SDL_Surface *Wsurface = IMG_Load("w.png");
	SDL_Surface *MenuSurface = IMG_Load("menu.png");
	SDL_Surface *undosurface = IMG_Load("undo.png");
	SDL_Surface *redosurface = IMG_Load("redo.png");
	SDL_Surface *restartsurface = IMG_Load("restart.png");
	SDL_Surface *nowWsurface = IMG_Load("nowW.png");
	SDL_Surface *nowBsurface = IMG_Load("nowb.png");
	SDL_Surface *numSurface = IMG_Load("num.png");
	SDL_Surface *numBsurface = IMG_Load("numb.png");
	SDL_Surface *numberSurface = IMG_Load("number.png");
	SDL_Surface *WwinSurface = IMG_Load("winW.png");
	SDL_Surface *BwinSurface = IMG_Load("winB.png");
	SDL_Surface *peaceSurface = IMG_Load("peace.png");
	SDL_SetColorKey(numberSurface, SDL_TRUE, SDL_MapRGB(numberSurface->format, 255, 255, 255));

	SDL_Texture *G = SDL_CreateTextureFromSurface(render, Gsurface);
	SDL_Texture *tG = SDL_CreateTextureFromSurface(render, tGsurface);
	SDL_Texture *B = SDL_CreateTextureFromSurface(render, Bsurface);
	SDL_Texture *W = SDL_CreateTextureFromSurface(render, Wsurface);
	SDL_Texture *menu = SDL_CreateTextureFromSurface(render, MenuSurface);
	SDL_Texture *Undo = SDL_CreateTextureFromSurface(render, undosurface);
	SDL_Texture *Redo = SDL_CreateTextureFromSurface(render, redosurface);
	SDL_Texture *Restart = SDL_CreateTextureFromSurface(render, restartsurface);
	SDL_Texture *nowW = SDL_CreateTextureFromSurface(render, nowWsurface);
	SDL_Texture *nowB = SDL_CreateTextureFromSurface(render, nowBsurface);
	SDL_Texture *num = SDL_CreateTextureFromSurface(render, numSurface);
	SDL_Texture *numb = SDL_CreateTextureFromSurface(render, numBsurface);
	SDL_Texture *number = SDL_CreateTextureFromSurface(render, numberSurface);
	SDL_Texture *winW = SDL_CreateTextureFromSurface(render, WwinSurface);
	SDL_Texture *winB = SDL_CreateTextureFromSurface(render, BwinSurface);
	SDL_Texture *peace = SDL_CreateTextureFromSurface(render, peaceSurface);
	SDL_SetTextureBlendMode(G, SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod(G, 150);

	SDL_Rect GR[tableSize][tableSize];
	for (int i = 0; i < tableSize; i++) {
		for (int j = 0; j < tableSize; j++) {
			GR[i][j].x = tableposx + BWsize * i;
			GR[i][j].y = tableposy + BWsize * j;
			GR[i][j].w = BWsize;
			GR[i][j].h = BWsize;
		}
	}
	SDL_Rect menuR;
	menuR.x = 40;
	menuR.y = 60;
	menuR.w = 160;
	menuR.h = 400;

	SDL_Rect undoR;
	undoR.x = 70;
	undoR.y = 100;
	undoR.h = undosurface->h / 2 - 20;
	undoR.w = undosurface->w / 2 - 40;

	SDL_Rect redoR;
	redoR.x = 70;
	redoR.y = undoR.y + undoR.h + 20;
	redoR.h = redosurface->h / 2 - 20;
	redoR.w = redosurface->w / 2 - 40;

	SDL_Rect restartR;
	restartR.x = 70;
	restartR.y = redoR.y + redoR.h + 20;
	restartR.h = restartsurface->h / 2 - 20;
	restartR.w = restartsurface->w / 2 - 40;

	
	
	int w = BwinSurface->w - 90, h = BwinSurface->h;

	SDL_Rect WinR;
	WinR.x = 350;
	WinR.y = 200;
	WinR.w = 0;
	WinR.h = BwinSurface->h;

	SDL_Rect nowR;
	nowR.x = 70;
	nowR.y = restartR.y + restartR.h + 20;
	nowR.h = 100;
	nowR.w = 94;

	Spot spots2[30];
	for (i = 0; i < 30; i++) {
		spots2[i].x = rand() % 370 + 310;
		spots2[i].y = rand() % 360 + 110;
		spots2[i].r = rand() % 5 + 10;
		spots2[i].speed = rand() % 1 + 1;
		spots2[i].alpha = rand() % 200 + 100;
	}
	
	SDL_Rect wrect;
	wrect.x = 400;
	wrect.y = 20;
	wrect.w = 40;
	wrect.h = 30;

	SDL_Rect wrect2;
	wrect2.x = 430;
	wrect2.y = 20;
	wrect2.w = 40;
	wrect2.h = 30;

	SDL_Rect brect;
	brect.x = 550;
	brect.y = 20;
	brect.w = 40;
	brect.h = 30;

	SDL_Rect brect2;
	brect2.x = 580;
	brect2.y = 20;
	brect2.w = 40;
	brect2.h = 30;

	SDL_Rect clips[10];
	for (int i = 0; i < 10; i++) {
		clips[i].x = 0;
		clips[i].y = i * (numberSurface->h / 10);
		clips[i].w = numberSurface->w;
		clips[i].h = numberSurface->h / 10;
	}

	Reversi Table;
	int wWidth = 200, bWidth = 200;
	SDL_Rect wnumR;
	wnumR.x = 410;
	wnumR.y = 50;
	wnumR.w = wWidth / 2;
	wnumR.h = 15;
	SDL_Rect bnumR;
	bnumR.x = wnumR.x + wnumR.w;
	bnumR.y = 50;
	bnumR.w = bWidth / 2;
	bnumR.h = 15;

	int wl = Table.whiteNum / 10, wr = Table.whiteNum % 10, bl = Table.blackNum / 10, br = Table.blackNum % 10;
	tmp.push_back(Table);
	while (!quit && !ex) {
		while (SDL_PollEvent(&event)) {
			int spcial = 0;
			if (event.type == SDL_QUIT)
				quit = true;
			else if (event.type == SDL_MOUSEMOTION) {
				int x = event.motion.x, y = event.motion.y;
				if (x <= undoR.x + undoR.w && x >= undoR.x && y <= undoR.y + undoR.h && y >= undoR.y) {
					undoR.w = undosurface->w / 2 - 30;
					undoR.h = undosurface->h / 2 - 18;
				}
				else if (x <= redoR.x + redoR.w && x >= redoR.x && y <= redoR.y + redoR.h && y >= redoR.y) {
					redoR.w = redosurface->w / 2 - 30;
					redoR.h = redosurface->h / 2 - 18;
				}
				else if (x <= restartR.x + restartR.w && x >= restartR.x && y <= restartR.y + restartR.h && y >= restartR.y) {
					restartR.w = restartsurface->w / 2 - 30;
					restartR.h = restartsurface->h / 2 - 18;
				}
				else {
					undoR.w = undosurface->w / 2 - 40;
					undoR.h = undosurface->h / 2 - 20;
					redoR.w = redosurface->w / 2 - 40;
					redoR.h = redosurface->h / 2 - 20;
					restartR.w = restartsurface->w / 2 - 40;
					restartR.h = restartsurface->h / 2 - 20;
				}
			}
			else if (event.type == SDL_MOUSEBUTTONDOWN) {
				//Mix_PlayChannel(-1, click, 0);
				//cout << Table.blackNum << "  " << Table.whiteNum << endl;
				int x = event.motion.x, y = event.motion.y;
				if (x <= (tableSize * BWsize + tableposx)&& x >= tableposx && y >= tableposy && y <= (BWsize * tableSize + tableposy)) {
						Point pos = chang(x, y);
						int flag = 0;
						if (Table.table[pos.x][pos.y] == 0) 
							flag = Table.checkPos(pos.x, pos.y, 1);
						if (flag == 64)
							ifwin = 1;
						else if (flag) {
							Table.initTmpTable();
							Mix_PlayChannel(-1, click, 0);
							if (back && mode == 1) {
								for (int k = 0; k < tmp.size() - Index; k++) tmp.pop_back();
								back = 0;
							}
							Table.NowFlag *= -1;
							if (mode == 1) {
								tmp.push_back(Table);	//儲存目前狀態
								Index++;
							}
							int passf = Table.ifPASS();		//檢查落子方是否有落子點
							if (passf == 64)
								ifwin = 1;
							else if (passf == 0) {	//無落子點，換另一方落子
								Table.NowFlag *= -1;
								Table.ifPASS();
								pass++;
								if (pass == 2) ifwin = 1;
							}
							else pass = 0;
							if (mode == 2 && Table.NowFlag < 0) {
								Table.initTmpTable();
								//cout << Table.rowIndex << "  " << Table.colIndex << endl;
								flag = Table.checkPos(Table.rowIndex, Table.colIndex, 1);
								if (flag == 64) ifwin = 1;
								else if (flag) {
									Table.NowFlag *= -1;
									if (back) {
										for (int k = 0; k < tmp.size() - Index; k++) tmp.pop_back();
										back = 0;
									}
									tmp.push_back(Table);	//儲存目前狀態
									Index++;
									int passf = Table.ifPASS();		//檢查落子方是否有落子點
									if (passf == 64)
										ifwin = 1;
									else if (passf == 0) {	//無落子點，換另一方落子
										Table.NowFlag *= -1;
										pass++;
										if (pass == 2) ifwin = 1;
										Table.initTmpTable();
										Table.ifPASS();
									}
									else pass = 0;
								}
							}
						}else Mix_PlayChannel(-1, dom, 0);
				}
				else if (x <= undoR.x + undoR.w && x >= undoR.x && y <= undoR.y + undoR.h && y >= undoR.y) {		//Undo
					if (!ifwin) {
						Mix_PlayChannel(-1, click, 0);
						spcial = 1;
						if (Index > 0) {
							Table.UndoOrRedo(tmp[Index - 1]);
							Index--;
							back = 1;
							Table.initTmpTable();
							Table.ifPASS();
						}
					}
				}
				else if (x <= redoR.x + redoR.w && x >= redoR.x && y <= redoR.y + redoR.h && y >= redoR.y) {		//Redo
					if (!ifwin) {
						Mix_PlayChannel(-1, click, 0);
						spcial = 1;
						if (Index < tmp.size() - 1) {
							Table.UndoOrRedo(tmp[Index + 1]);
							Index++;
							Table.initTmpTable();
							Table.ifPASS();
						}
					}
				}
				else if (x <= restartR.x + restartR.w && x >= restartR.x && y <= restartR.y + restartR.h && y >= restartR.y) {		//Restart
					Mix_PlayChannel(-1, click, 0);
					spcial = 1;
					int size = tmp.size();
					for (int i = 0; i < size; i++) {
						tmp.pop_back();
					}
					Table.restart();
					tmp.push_back(Table);
					Index = 0;
					ifwin = 0;
					back = 0;
					Table.initTmpTable();
					Table.ifPASS();
				}
				cout << event.motion.x << "  " << event.motion.y << endl;
				wl = Table.whiteNum / 10;
				wr = Table.whiteNum % 10;
				bl = Table.blackNum / 10;
				br = Table.blackNum % 10;

			}
			else if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == SDLK_1) {
					if (Mix_PlayingMusic()) {
						if (Mix_PausedMusic())
							Mix_ResumeMusic();
						else
							Mix_PauseMusic();
					}
				}
			}
		}
		int total = Table.whiteNum + Table.blackNum;
		wnumR.w = wWidth * Table.whiteNum / total;
		bnumR.x = wnumR.x + wnumR.w;
		bnumR.w = bWidth * Table.blackNum / total;
		SDL_RenderClear(render);
		SDL_RenderCopy(render, BG3, NULL, &BGrect);
		SDL_RenderCopy(render, BG2, NULL, &WW);
		SDL_RenderCopy(render, menu, NULL, &menuR);
		SDL_RenderCopy(render, Undo, NULL, &undoR);
		SDL_RenderCopy(render, Redo, NULL, &redoR);
		SDL_RenderCopy(render, Restart, NULL, &restartR);
		if(Table.NowFlag > 0) SDL_RenderCopy(render, nowB, NULL, &nowR);
		else 
			SDL_RenderCopy(render, nowW, NULL, &nowR);
		SDL_RenderCopy(render, num, NULL, &wnumR);
		SDL_RenderCopy(render, numb, NULL, &bnumR);
		SDL_RenderCopy(render, number, &clips[wl], &wrect);
		SDL_RenderCopy(render, number, &clips[wr], &wrect2);
		SDL_RenderCopy(render, number, &clips[bl], &brect);
		SDL_RenderCopy(render, number, &clips[br], &brect2);
		for(int i = 0; i < tableSize; i++)
			for (int j = 0; j < tableSize; j++) {
				if(Table.table[i][j] == 0)
					SDL_RenderCopy(render, G, NULL, &GR[i][j]);
				else if(Table.table[i][j] == 1)
					SDL_RenderCopy(render, B, NULL, &GR[i][j]);
				else if(Table.table[i][j] == 2)
					SDL_RenderCopy(render, W, NULL, &GR[i][j]);
				if(Table.tmptable[i][j] == -1)
					SDL_RenderCopy(render, tG, NULL, &GR[i][j]);
			}
		for (int i = 0; i < 30; i++) {
			spots2->drawSpot(&spots2[i], render, Bubble);
			spots2->moveSpot(&spots2[i], 2);
		}
		if (ifwin) {
			Table.initTmpTable();
			int mode = Table.blackNum > Table.whiteNum ? 1 : 0;
			if (Table.blackNum == Table.whiteNum) mode = 2;
			if(mode == 1)
				Win(w,render , winB, WinR);
			else if(mode == 0)
				Win(w, render, winW, WinR);
			else Win(w, render, peace, WinR);
			WinR.w = w;
		}
		SDL_RenderPresent(render);
		SDL_Delay(10);
	}
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}