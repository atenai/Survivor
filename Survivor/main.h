//=======================================================================================
//						�|���S���`��
//						2017.06.05
//=======================================================================================

#ifndef _MAIN_H_
#define _MAIN_H_

//13�C���`��ʃp�\�R���p�𑜓x�Ő���
#define SCREEN_WIDTH	(1366)//main.cpp/Player.cpp
#define SCREEN_HEIGHT	(768)//main.cpp/Player.cpp



#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <math.h>
LPDIRECT3DDEVICE9 GetD3DDevice(void);

bool Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Update(void);
void Draw(void);
void Uninit(void);
void title(void);
void game(void);
void end(void);
void gameover(void);
void Sixyokika(void);

#endif // _MAIN_H_
