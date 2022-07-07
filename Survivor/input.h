//=============================================================================
//
// 入力処理 [input.h]
//これはいじる必要無し
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"
#include <Windows.h>
#define DIRECTINPUT_VERSION (0x0800)
#include <dinput.h>
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);
void UninitKeyboard(void);//main.cpp(void Uninit(void))
void UpdateKeyboard(void);//main.cpp(void Updata(void))

bool GetKeyboardPress(int nKey);//player.h(void UpdatePlayer(void)
bool GetKeyboardTrigger(int nKey);
bool GetKeyboardRepeat(int nKey);
bool GetKeyboardRelease(int nKey);

#endif