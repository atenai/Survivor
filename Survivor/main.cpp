//=======================================================================================
//						メイン関数
//						柏原雄太
//なんか主人公の打つ弾と戦車の体力かつあたり判定を作っている最中らしいので注意！
//=======================================================================================
#include <math.h>
#include "Polygon.h"
#include "input.h"
#include "judgement.h"
//***************************************************************************************
//	マクロ定義
//***************************************************************************************
#define CLASS_NAME		"DX2年生トリクロ"
#define WINDOW_NAME		"サバイバー"

//***************************************************************************************
//	構造体宣言
//***************************************************************************************
//(1)画像表示
typedef struct
{
	D3DXVECTOR4 pos; //pos=ポリゴン座標//最初の水色は型名
	D3DCOLOR color; //頂点の色
	D3DXVECTOR2 texcoord; //テクスチャ（０～１）
}VERTEX_2D;

#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)


//***************************************************************************************
//	プロトタイプ
//***************************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//////////////////////////////////////////////////
#define BULLET_SPEED (5.0f)//弾の速さ
#define BULLET_WIDTH (16.0f)//( 横幅 ),
#define BULLET_HEIGHT (16.0f)//( 縦幅 ),

void ShootBullet(float x, float y);
const D3DXVECTOR2* GetBulletPos(void);//main.cpp

static D3DXVECTOR2 g_vBulletPos(0.0f, 0.0f);//player

//////////////////////////////////////////////////

#define PLYAER_WIDTH (48.0f)
#define PLYAER_HEIGHT (48.0f)

#define ENEMY_WIDTH (96)//戦車の横幅
#define ENEMY_HEIGHT (48)//戦車の縦幅

#define CAR_WIDTH (48)//脱出よう車の横幅かつあたり判定数値
#define CAR_HEIGHT (48)//脱出よう車の縦幅かつあたり判定数値

//////////////////////////////////////////////////
//***************************************************************************************
//	グローバル変数宣言
//***************************************************************************************
DWORD g_fOldTime;//前のフレームの時間//ゲームループ
DWORD g_fNowTime;//今の時間//ゲームループ
LPDIRECT3D9		g_pD3D = NULL;// Direct3Dインターフェイス
static LPDIRECT3DDEVICE9	g_pD3DDevice = NULL;// Direct3DDeviceインターフェイス
	
static int nflag = 0;//タイトル⇒ゲームプレイ⇒ゲームオーバー⇒タイトル

//staticはこのファイルだけのグローバル変数です,付けない方は全ファイル使えます
//プレイヤー
static D3DXVECTOR2 PlayerPos(220.0f, 55.0f);//プレイヤー座標
static bool playerBullet;//boolはtrueとfalseの２択//playerの弾の表示・非表示
static float jump = 7.0f;//ジャンプ力
static float gura = 3.0f;//重力
static bool plyer;//プレイヤーの表示・非表示

int jump2 = 0;
float v0 = 0;
float h = 0;
float t = 0;
float g = 9.8;

//フィールド
static float kiritoriX = 0.0f;//画像の切り取り

//爆撃機
static D3DXVECTOR2 bakugekikiPos(1500.0f, 10.0f);//爆撃機座標

static bool bakudan;//爆弾
static D3DXVECTOR2 bakudan1Pos(750.0f, 70.0f);//爆弾1投下座標数値
static bool bakudan2;//爆弾2
static D3DXVECTOR2 bakudan2Pos(500.0f, 70.0f);//爆弾2投下座標数値
static bool bakudan3;//爆弾3
static D3DXVECTOR2 bakudan3Pos(150.0f, 70.0f);//爆弾3投下座標数値


//戦車
static bool teki;//画像の表示・非表示
static D3DXVECTOR2 sensixyaPos(1000.0f, 472.0f);//戦車座標

static bool tekitama;////砲弾の表示・非表示
static D3DXVECTOR2 houdanPos(850.0f, 555.0f);//砲弾の座標数値
static bool life1;
static bool life2;
static bool life3;

//テクニカル
static float zikan = 0.0f;//脱出よう車の時間を計る
static bool car;//脱出よう車の表示・非表示
static D3DXVECTOR2 tekunikaruPos(1000.0f, 550.0f);//脱出よう車の座標数値

//=======================================================================================
//　エントリーポイント関数
//=======================================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)//void mainようなもの																	
{

	lpCmdLine = lpCmdLine;//警告回避用
	hPrevInstance = hPrevInstance;//警告回避用

	WNDCLASSEX wc = {//必要な情報を入れる
						sizeof(WNDCLASSEX),//構造体のサイズ
						CS_HREDRAW | CS_VREDRAW,//ウィンドウクラスのスタイル
						WndProc,//ウィンドウプロシージャのアドレス
						0,//追加メモリが必要な場合、そのバイト数を指定
						0,//同上
						hInstance,//クラスを作成するインスタンスハンドル
						NULL,//アイコンのハンドル
						LoadCursor(NULL,IDC_ARROW),//カーソル(マウスポインタ)のハンドル
						(HBRUSH)(COLOR_WINDOW + 1),//背景色用のブラシのハンドル
						NULL,//メニューリソースを表す名前
						CLASS_NAME,//ウィンドウクラスの名前
						NULL//アイコン(小)のハンドル
						};

	RegisterClassEx(//WNDCLASSEXで入れた情報をwindowsに登録する
					&wc//WNDCLASSEX構造体のポインタ
					);

	HWND hWnd = CreateWindowEx(//ウィンドウを作成する
							0,//??
							CLASS_NAME,//登録されているクラス名
							WINDOW_NAME,//ウィンドウ名
							WS_OVERLAPPEDWINDOW & ~(WS_MAXIMIZEBOX | WS_THICKFRAME),//ウィンドウスタイル	
							CW_USEDEFAULT,//ウィンドウのx位置
							CW_USEDEFAULT,//ウィンドウのy位置
							SCREEN_WIDTH,//ウィンドウの横サイズ
							SCREEN_HEIGHT,//ウィンドウの縦サイズ
							NULL,//親ウィンドウまたはオーナーウィンドウのハンドル
							NULL,//メニューハンドルまたは子ウィンドウ
							hInstance,//アプリケーションインスタンスのハンドル
							NULL//ウィンドウ作成のデータ
							);
	
	ShowWindow(//CreateWindowで作成したウィンドウを表示する
		hWnd,//ウィンドウハンドル
		nCmdShow//表示状態
	);


	MSG msg;//PeekMessage関数が受信したイベントメッセージはMSG構造体の変数に記録される

	//初期化処理(ウィンドウを作成してから
	if (!Init(hInstance, hWnd, TRUE))
	{
		MessageBox(hWnd, "終了します", "初期化できない", MB_OK);
		return -1;
	}


	

	//======================ゲームループ開始==============
	//分解能
	timeBeginPeriod(1);

	for (;;)	//for泣き
	{
		//メッセージループつまりウィンドウを表示させ続ける
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))//Windowsからイベントメッセージを受け取る
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				//メッセージ処理
				TranslateMessage(&msg);//WM_KEYDOWNメッセージからWM_CHARメッセージを作成
				DispatchMessage(&msg);//ウィンドウプロシージャにイベントメッセージを転送
			}
		}
		else
		{
			//ゲーム処理
			g_fNowTime = timeGetTime();//今の時間 = timeGetTime();
									   //		ミリ秒　　　ミリ秒
			if ((g_fNowTime - g_fOldTime) >= (1000 / 60))//if((今の時間 - 前のフレームの時間) >= (1000/60))
			{

				//タイトル⇒ゲームプレイ⇒ゲームオーバー⇒タイトル
				if (nflag == 0)
				{
					//タイトル画面
					title();
				}

				if (nflag == 1)
				{
					//ゲーム開始
					game();

				}

				if (nflag == 2)
				{
					//ゲーム終了
					end();
				}

				//タイトル画面に戻る処理
				if (nflag == 3)
				{
					//ゲームオーバー
					gameover();
				}


				g_fOldTime = g_fNowTime;//前フレームの時間 = 今の時間;
			}
		}
	}

	timeEndPeriod(1);
	//=======================ゲームループ完了============================

	


	Uninit();//終了処理

	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	int nID;

	switch (Msg)
	{
		case WM_CREATE:
			break;

		case WM_COMMAND:
			//BUTTONが押された
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		case WM_CLOSE://xボタン押す`
			nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);
			if (nID == IDYES)
			{
				DestroyWindow(hWnd);      //ウィンドウを閉じる
			}
			else
			{
				return 0;
			}
			break;

		case WM_KEYDOWN:
			switch (wParam)
			{
			case VK_ESCAPE://[ESC]を押したら
				nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);
				if (nID == IDYES)
				{
					DestroyWindow(hWnd);      //ウィンドウを閉じる
				}
				break;
			case VK_RETURN://エンターキーが押されたら
				if (nflag == 0)//もしタイトル画面だったら
				{
					nflag = 1;//ゲームプレイ画面へ移動
				}
				if (nflag == 2 || nflag == 3)//ゲームクリアー画面またはゲームオーバー画面だったら
				{
					nflag = 0;//タイトル画面へ移動
				}
				break;
			case VK_SPACE://スペースキーが押されたら
				if (nflag == 0)//もしタイトル画面だったら
				{
					nflag = 1;//ゲームプレイ画面へ移動
				}
				if (nflag == 2 || nflag == 3)//ゲームクリアー画面またはゲームオーバー画面だったら
				{
					nflag = 0;//タイトル画面へ移動
				}
				break;

			}
			break;

	default:
	break;
	}

	//DefWindowProc:ウィンドウ調整の関数
	return DefWindowProc(hWnd, Msg, wParam, lParam);
}
//=================================================
//		初期化処理
//=================================================
bool Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	//Direct3Dオブジェクトを作成//no
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return false;
	}

	//現在のディスプレイモードを
	D3DDISPLAYMODE d3ddm;

	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return false;
	}

	//デバイスのプレゼンテーションバラメータ
	D3DPRESENT_PARAMETERS	d3dpp;
	//構造体の中に全部0にして
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	//裏画面の幅
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	//裏画面の高さ
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	//カラーモード
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.BackBufferCount = 1;
	//映像信号
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	//2次元はいらない　3Dは必要
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	//true =windowモード	false = screenモード
	d3dpp.Windowed = bWindow;
	//フルーモード自動で画面サイズ
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	//画面更新
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	//デバイスの生成
	if (FAILED(g_pD3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{
		//returnのメッセージbox
		MessageBox(hWnd, "エラー", "起動失敗", MB_OK);
		return false;
	}

	//レンダーステートの設定
	//αブレンドを行う
	//SRC....今から描くもの。つまりポリゴン
	//DEST...すでに描画されている画面のこと
	//SRC_RGB*SRC_α+DEST_RGB*(1-SRC_α)
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);//αソース
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);//αデスト

	//テクスチャステージステートの設定		//α演算子			掛け算
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	//サンプラーステートの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);//(テクスチャは(0~1)座標外の対処)
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_BORDER);//D3DTADDRESS_MIRROR(特点 超えた部分をコッピーして180度反転)

	//フィルタリング
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);


	InitKeyboard(hInstance, hWnd);//いじらない

	//ポリゴン初期化失敗対処
	if (!(InitPolygon()))//polygon.h//いじらない
	{
		MessageBox(hWnd,"ポリゴンの初期化失敗した","エラー",MB_OK);
		return false;
	}

	///////////////////////////敵の初期化/////////////////
	teki = true;//ここで敵をtrueにして表示する
	///////////////////////////敵の初期化/////////////////
	//プレイヤーの初期ポジション/////////////////////////
	PlayerPos.x = 220.0f;//プレイヤーの初期ポジションx
	PlayerPos.y = 550.0f;//プレイヤーの初期ポジションy
	//プレイヤーの初期ポジション/////////////////////////
	//弾の初期化/////////////////////////////////////
	g_vBulletPos = D3DXVECTOR2(0.0f, 0.0f);//plyer
	playerBullet = false;//弾の初期状態をfalseにする//plyer
	//弾の初期化/////////////////////////////////////
	/////爆弾///////
	bakudan = false;
	bakudan2 = false;
	bakudan3 = false;
	/////爆弾/////
	//life//
	life1 = true;
	life2 = true;
	life3 = true;
	//life//
	//プレイヤーの初期化//
	plyer = true;
	//プレイヤーの初期化//
	//脱出用車の初期化//
	car = false;
	//脱出用車の初期化//
	//敵の弾///
	tekitama = false;
	//敵の弾///


	return true;
}

void title(void) 
{
	//描画
	Draw();
	//ゲームの初期化処理
	Sixyokika();
}

void game(void) 
{
	//更新
	Update();
	//描画
	Draw();
}

void end(void) 
{
	//描画
	Draw();
}

void gameover(void) 
{
	//描画
	Draw();
}


void Update(void)
{
	UpdateKeyboard();//input.h//いじらない
/////////////////ここから↓の中身をいじる/////////////////////////////////
////プレイヤーのアップデート/////////////////////////////
	if (GetKeyboardPress(DIK_W))//キーボードのWを押したら//input.h
	{
		PlayerPos.y = PlayerPos.y - jump;//上移動
		//jump2 = 1;
		//v0 = 40.0;//初速度 
		//t = 0;
	}

	//if (jump2 == 1) 
	//{
	//	h = v0 * t - (1.0 / 2.0)*g*t*t;//公式
	//	PlayerPos.y = PlayerPos.y - h ;//
	//	t += 1;
	//	if (PlayerPos.y > 571) 
	//	{
	//		jump2 = 0;
	//		PlayerPos.y = 571;
	//	}
	//}


	if (GetKeyboardPress(DIK_S))//キーボードのSを押したら
	{
		//tateidou = tateidou + 5;//下移動
	}
	if (GetKeyboardPress(DIK_A))//キーボードのAを押したら
	{
		PlayerPos.x = PlayerPos.x - 5.0f;//左移動
	}
	if (GetKeyboardPress(DIK_D))//キーボードのDを押したら
	{
		PlayerPos.x = PlayerPos.x + 5.0f;//右移動
	}

	if (GetKeyboardPress(DIK_L))//弾を撃つ
	{
		ShootBullet(PlayerPos.x, PlayerPos.y);//ShootBullet(xのポリゴン表示座標,yのポリゴン表示座標);
	}

////プレイヤーの重力/////////////////////////////
	if (572 > PlayerPos.y)//プレイヤーのy座標が562より小さい場合
	{
		PlayerPos.y = PlayerPos.y + gura;//下移動//重力
	}

////ステージの範囲設定とバックグラウンド画像/////////////////////////////
	if ((572 < PlayerPos.y))//キャラクターのポジションYがtateidouより大きい場合(フィールドの下設定)
	{
		PlayerPos.y = 571;//キャラクターの乗る位置  
	}
	if (PlayerPos.y < 180)//キャラクターのポジションYがtateidouより小さかった場合(フィールドの上設定)
	{
		PlayerPos.y = 181;//キャラクターの乗る位置  
	}
	if (PlayerPos.x < 0.0f)//キャラクターのポジションXがyokoidouより小さかった場合(フィールドの左設定)
	{
		PlayerPos.x = 0.0f;
	}
	if (PlayerPos.x > 1270.0f)//キャラクターのポジションXがyokoidouより大きい場合(フィールドの右設定)
	{
		PlayerPos.x = 1270.0f;
	}

	//背景を動かす
	kiritoriX = kiritoriX + 2.0f;
	if (kiritoriX > 1080.0f) 
	{
		kiritoriX = 0.0f;
	}
////ステージ設定/////////////////////////////

////弾のアップデート///////////////////////////////////////
	if (playerBullet)//trueの場合//player
	{
//弾の弾道
		g_vBulletPos.x += BULLET_SPEED;//弾を右にうつ
		if (g_vBulletPos.x + BULLET_WIDTH>1024)//弾の位置が0以下なら
		{
			playerBullet = false;//弾の表示を消す
		}	
	}
////弾のアップデート///////////////////////////////////////

/////敵のアップデート////////////////////////////////////////
	zikan++;
	if (zikan >= 1000) 
	{
		car = true;
		tekunikaruPos.x = tekunikaruPos.x - 7;
		if (tekunikaruPos.x <= 420)
		{
			tekunikaruPos.x = 420;
		}
	}


///////////////敵の移動///////////////////////////////////////////////////////////
	//戦車の移動
	if (sensixyaPos.x >= 850.0f && sensixyaPos.x <= 1000.0f)//idouxの数値が650～850の間なら画像が左に移動 
	{
		sensixyaPos.x = sensixyaPos.x - 5.0f;//左に移動
	}
	else if (sensixyaPos.x <= 850.0f)//idouxが650以下なら645で数値を固定 
	{
		sensixyaPos.x = 845;
	}

	//敵の弾を発射
	if (sensixyaPos.x == 845 )
	{
		tekitama = true;
	}
	if (tekitama == true) 
	{
		houdanPos.x = houdanPos.x - 5;//砲弾が左に移動
	}
	if (houdanPos.x < -50)//弾の位置が0以下なら
	{
		tekitama = false;//弾の表示を消す
		houdanPos.x = 950;
	}
	

	//爆撃機移動
	bakugekikiPos.x = bakugekikiPos.x - 3.0f;
	if (bakugekikiPos.x < -200.0f) {
		bakugekikiPos.x = 1500.0f;
		
	}
	//爆弾投下
	if (bakugekikiPos.x == 750.0f) {
		bakudan = true;
	}
	if (bakudan) {
		bakudan1Pos.y = bakudan1Pos.y + 2.0f;//下に移動
	}
	if (bakudan1Pos.y > 900.0f) {
		bakudan = false;
		bakudan1Pos.y = 70.0f;
	}
	
	//爆弾投下2
	if (bakugekikiPos.x <= 500.0f) {
		bakudan2 = true;
	}
	if (bakudan2) {
		bakudan2Pos.y = bakudan2Pos.y + 2.0f;//下に移動
	}
	if (bakudan2Pos.y > 900.0f) {
		bakudan2 = false;
		bakudan2Pos.y = 70.0f;
	}
	
	//爆弾投下3
	if (bakugekikiPos.x <= 150.0f) {
		bakudan3 = true;
	}
	if (bakudan3) {
		bakudan3Pos.y = bakudan3Pos.y + 2.0f;//下に移動
	}
	if (bakudan3Pos.y > 900.0f) {
		bakudan3 = false;
		bakudan3Pos.y = 70.0f;
	}
	/*
	//ミサイル移動
	misairuidou = misairuidou - 10;
	if (misairuidou < -50) {
		misairuidou = 1500;
	}
	*/
///////////////敵の移動///////////////////////////////////////////////////////////

/////敵のアップデート////////////////////////////////////////

//あたり判定//////////////////////////////////////////////////////////////////////////////
		//敵 & プレイヤーの弾////////////////////////////////////////
			bool bHit = atari(//judgement.h//あたり判定//teki
				sensixyaPos.x, sensixyaPos.y, ENEMY_WIDTH*0.5,//敵の位置とその範囲を指定//48*0.5=24
				GetBulletPos()->x, GetBulletPos()->y, BULLET_WIDTH*0.5//弾の位置
			);
			if (life1 == false && life2 == false && life3 == true) {
				if (bHit)//あたったら下の内容を起動
				{
					playerBullet = false;//プレイヤーの弾を消す
					life3 = false;
				}
			}
			if (life1 == false && life2 == true && life3 == true) {
				if (bHit)//あたったら下の内容を起動
				{
					playerBullet = false;//プレイヤーの弾を消す
				
					life2 = false;
				}
			}
			if (life1 == true && life2 == true && life3 == true) {
				if (bHit)//あたったら下の内容を起動
				{
					playerBullet = false;//プレイヤーの弾を消す
					life1 = false;
				}
			}
			if (life1 == false && life2 == false && life3 == false) {
				teki = false;//敵を消す
				sensixyaPos.x = -50;//敵を画面外に出す邪魔だから
			}
			//敵 & プレイヤーの弾////////////////////////////////////////

			//プレイヤー & 爆弾1//////////////////////////////////////////
			bool bHit1 = atari(//judgement.h//あたり判定//plyer
				PlayerPos.x + 48.0f, PlayerPos.y + 48.0f, 48.0f,//プレイヤーの位置
				bakudan1Pos.x + 24.0f, bakudan1Pos.y + 24.0f, 24.0f//爆弾の位置
			);
			if (bHit1) {
				plyer = false;
			}
			//プレイヤー & 爆弾1//////////////////////////////////////////
			//プレイヤー & 爆弾2//////////////////////////////////////////
			bool bHit2 = atari(//judgement.h//あたり判定//plyer
				PlayerPos.x + 48.0f, PlayerPos.y + 48.0f, 48.0f,//プレイヤーの位置
				bakudan2Pos.x + 24.0f, bakudan2Pos.y + 24.0f, 24.0f//爆弾の位置
			);
			if (bHit2) {
				plyer = false;
			}
			//プレイヤー & 爆弾2//////////////////////////////////////////
			//プレイヤー & 爆弾3//////////////////////////////////////////
			bool bHit3 = atari(//judgement.h//あたり判定//plyer
				PlayerPos.x + 48.0f, PlayerPos.y + 48.0f, 48.0f,//プレイヤーの位置
				bakudan3Pos.x + 24.0f, bakudan3Pos.y + 24.0f, 24.0f//爆弾の位置
			);
			if (bHit3) {
				plyer = false;
			}
			//プレイヤー & 爆弾3//////////////////////////////////////////
			//プレイヤー & 砲弾//////////////////////////////////////////
			bool bHit4 = atari(//judgement.h//あたり判定//plyer
				PlayerPos.x + 48.0f, PlayerPos.y + 48.0f, 48.0f,//プレイヤーの位置
				houdanPos.x + 24.0f, houdanPos.y + 24.0f, 24.0f//砲弾の位置
			);
			if (bHit4) {
				plyer = false;
			}
			//プレイヤー & 砲弾//////////////////////////////////////////
			//プレイヤー & 助け車//////////////////////////////////////////
			bool bHit5 = atari(//judgement.h//あたり判定//plyer
				PlayerPos.x + 48.0f, PlayerPos.y + 48.0f, 48.0f,//プレイヤーの位置
				tekunikaruPos.x + 72.0f, tekunikaruPos.y + 72.0f, 72.0f//テクニカルの位置
			);
			if (bHit5) {
				//ゲームクリアー処理
				nflag = 2;
			}
			//プレイヤー & 砲弾//////////////////////////////////////////
//あたり判定//////////////////////////////////////////////////////////////////////////////	
			if (plyer == false) 
			{
				nflag = 3;//ゲームオーバー処理
			}
////////////////ここから↑の中身をいじる/////////////////////////////////////	


}

//=================================
//		描画処理
//=================================
void Draw(void)
{//画面クリア
	g_pD3DDevice->Clear(
		0,
		NULL,
		//ターゲット　　　　ZBUFFER
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(32, 64, 128, 255),//背景色の変更
		1.0f,
		0
		);

	//Direct3Dによる描画の開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{

//////////////////ここから↓の中身をいじる/////////////////////////////////
		//タイトル//
		if (nflag == 0) 
		{

			//タイトル画面
			SetPolygonColor(D3DCOLOR_RGBA(255, 255, 255, 255));//ポリゴンカラーの設定
			DrawPolygon(
				TEX_8,//使うテクスチャ,
				0.0f, //x座標,
				0.0f,//y座標,
				1366,//テクスチャの表示する横の長さ
				768,//テクスチャの表示する縦の長さ
				0,//元の画像の切り取り開始場所（横）（画像の左から数える）
				0,//元の画像の切り取り開始場所（縦）（画像の上から数える）
				900,//元の画像の切り取りたい幅
				480 //元の画像の切り取りたい高さ
			);
		}
		//ゲームクリアー
		if (nflag == 2) 
		{
			SetPolygonColor(D3DCOLOR_RGBA(255, 255, 255, 255));//ポリゴンカラーの設定
			DrawPolygon(
				TEX_9,//使うテクスチャ,
				0.0f, //x座標,
				0.0f,//y座標,
				1366,//テクスチャの表示する横の長さ
				768,//テクスチャの表示する縦の長さ
				0,//元の画像の切り取り開始場所（横）（画像の左から数える）
				0,//元の画像の切り取り開始場所（縦）（画像の上から数える）
				900,//元の画像の切り取りたい幅
				480 //元の画像の切り取りたい高さ
			);
		}
		//ゲームオーバー
		if (nflag == 3) 
		{
			SetPolygonColor(D3DCOLOR_RGBA(255, 255, 255, 255));//ポリゴンカラーの設定
			DrawPolygon(
				TEX_10,//使うテクスチャ,
				0.0f, //x座標,
				0.0f,//y座標,
				1366,//テクスチャの表示する横の長さ
				768,//テクスチャの表示する縦の長さ
				0,//元の画像の切り取り開始場所（横）（画像の左から数える）
				0,//元の画像の切り取り開始場所（縦）（画像の上から数える）
				900,//元の画像の切り取りたい幅
				480 //元の画像の切り取りたい高さ
			);
		}

		if (nflag == 1) 
		{
			/////////////////背景の描画////////////////////////
			SetPolygonColor(D3DCOLOR_RGBA(255, 255, 255, 255));//ポリゴンカラーの設定
			DrawPolygon(
				TEX_5,//使うテクスチャ,
				0.0f, //x座標,
				0.0f,//y座標,
				1366,//テクスチャの表示する横の長さ
				768,//テクスチャの表示する縦の長さ
				kiritoriX,//元の画像の切り取り開始場所（横）（画像の左から数える）
				300,//元の画像の切り取り開始場所（縦）（画像の上から数える）
				200,//元の画像の切り取りたい幅
				200 //元の画像の切り取りたい高さ
			);
			/////////////////背景の描画////////////////////////
			//////////////////プレイヤーの描画//////////////////	
			if (plyer == true) 
			{
				SetPolygonColor(D3DCOLOR_RGBA(255, 255, 255, 255));//ポリゴンカラーの設定
				DrawPolygon(
					TEX_7,//使うテクスチャ,
					PlayerPos.x, //x座標,
					PlayerPos.y,//y座標,
					PLYAER_WIDTH * 2.0f,//テクスチャの表示する横の長さ
					PLYAER_HEIGHT * 2.0f,//テクスチャの表示する縦の長さ
					0,//元の画像の切り取り開始場所（横）（画像の左から数える）
					0,//元の画像の切り取り開始場所（縦）（画像の上から数える）
					48,//元の画像の切り取りたい幅
					48 //元の画像の切り取りたい高さ
				);
			}
			//////////////////プレイヤーの描画//////////////////

			////////////////////弾の描画///////////////////////////
			if (playerBullet)//trueの場合//player
			{
				//プレイヤーの弾のテクスチャを描画
				DrawPolygon(
					TEX_6,//テクスチャ,
					g_vBulletPos.x + 80,//xのポリゴン表示座標,
					g_vBulletPos.y + 8,//yのポリゴン表示座標,
					BULLET_WIDTH,//( 横幅 ),
					BULLET_HEIGHT,//( 縦幅 ),
					0,//切り取り開始地点x,
					0,//切り取り開始地点y,
					16,//切り取り幅,
					16//切り取り高さ
				);//bullet	

			}

			////////////////////弾の描画///////////////////////////
			////////////////助け車の描画////////////////////////////////
			if (car == true)
			{
				DrawPolygon
				(
					TEX_3,//テクスチャ,
					tekunikaruPos.x,//xのポリゴン表示座標,
					tekunikaruPos.y,//yのポリゴン表示座標,
					CAR_WIDTH * 3.0f,//( 横幅 ),
					CAR_HEIGHT * 3.0f,//( 縦幅 ),
					0,//切り取り開始地点x,
					0,//切り取り開始地点y,
					48,//切り取り幅,
					48//切り取り高さ
				);//enemy
			}
			////////////////助け車の描画////////////////////////////////
			////////////////戦車の描画////////////////////////////////
			if (teki == true)
			{
				DrawPolygon
				(
					TEX_2,//テクスチャ,
					sensixyaPos.x,//xのポリゴン表示座標,
					sensixyaPos.y,//yのポリゴン表示座標,
					ENEMY_WIDTH * 5.0f,//( 横幅 ),
					ENEMY_HEIGHT * 5.0f,//( 縦幅 ),
					0,//切り取り開始地点x,
					0,//切り取り開始地点y,
					96,//切り取り幅,
					48//切り取り高さ
				);//enemy
			}
			////////////////戦車の描画////////////////////////////////
					//戦車の砲弾///
			if (tekitama) {
				DrawPolygon(
					TEX_6,//テクスチャ,
					houdanPos.x,//xのポリゴン表示座標,
					houdanPos.y,//yのポリゴン表示座標,
					BULLET_WIDTH * 3,//( 横幅 ),
					BULLET_HEIGHT * 3,//( 縦幅 ),
					0,//切り取り開始地点x,
					0,//切り取り開始地点y,
					16,//切り取り幅,
					16//切り取り高さ
				);//bullet
			}
			//戦車の砲弾///
	////////////爆撃機///////////////////////////////////
			DrawPolygon
			(
				TEX_11,//テクスチャ,
				bakugekikiPos.x,//xのポリゴン表示座標,
				bakugekikiPos.y,//yのポリゴン表示座標,
				48*3,//( 横幅 ),
				48*3,//( 縦幅 ),
				0,//切り取り開始地点x,
				0,//切り取り開始地点y,
				48,//切り取り幅,
				48//切り取り高さ
			);//enemy
	////////////爆撃機///////////////////////////////////

	////////////////爆弾の描画////////////////////////////////
			if (bakudan) {
				DrawPolygon
				(
					TEX_12,//テクスチャ,
					bakudan1Pos.x,//xのポリゴン表示座標,
					bakudan1Pos.y,//yのポリゴン表示座標,
					48,//( 横幅 ),
					48,//( 縦幅 ),
					0,//切り取り開始地点x,
					0,//切り取り開始地点y,
					48,//切り取り幅,
					48//切り取り高さ
				);
			}
			if (bakudan2) {
				DrawPolygon
				(
					TEX_12,//テクスチャ,
					bakudan2Pos.x,//xのポリゴン表示座標,
					bakudan2Pos.y,//yのポリゴン表示座標,
					48,//( 横幅 ),
					48,//( 縦幅 ),
					0,//切り取り開始地点x,
					0,//切り取り開始地点y,
					48,//切り取り幅,
					48//切り取り高さ
				);
			}
			if (bakudan3) {
				DrawPolygon
				(
					TEX_12,//テクスチャ,
					bakudan3Pos.x,//xのポリゴン表示座標,
					bakudan3Pos.y,//yのポリゴン表示座標,
					48,//( 横幅 ),
					48,//( 縦幅 ),
					0,//切り取り開始地点x,
					0,//切り取り開始地点y,
					48,//切り取り幅,
					48//切り取り高さ
				);
			}
			////////////////爆弾の描画////////////////////////////////
			/////////////////地面の描画////////////////////////
			DrawPolygon(
				TEX_4,//テクスチャ,
				0,//xのポリゴン表示座標,
				668,//yのポリゴン表示座標,
				1366,//横幅,
				200,//縦幅,
				0,//kiritoriX,//切り取り開始地点x,
				0,//kiritoriY,//切り取り開始地点y,
				50,//切り取り幅,
				2//切り取り高さ
			);
			/////////////////地面の描画////////////////////////
		}
////////////////ここから↑の中身をいじる/////////////////////////////////////

		g_pD3DDevice->EndScene();//Direct3Dによる描画の終了//いじらない
	}
	//必ず
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}


LPDIRECT3DDEVICE9 GetD3DDevice(void)
{
	//デバイスの取得
	return g_pD3DDevice;
}

//====================================
//		終了処理
//====================================
void Uninit(void)
{
	UninitKeyboard();//input.h//いじらない

	UninitPolygon();//polygon.h//いじらない

	if (g_pD3DDevice != NULL)
	{//デバイス解放
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	if (g_pD3D != NULL)
	{//Direct3Dオブジェクトの開放
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

//////////////////////////////////////////////////////////////////////

void ShootBullet(float x, float y)//bullet.h//player.cpp//プレイヤーの弾をうつ
{
	if (!playerBullet)//!はfalseの場合
	{
		playerBullet = true;
		//g_vBulletPos = D3DXVECTOR2(x, y);
		g_vBulletPos.x = x;
		g_vBulletPos.y = y;
	}
}

const D3DXVECTOR2* GetBulletPos(void)//bullet.h//main.cpp//あたり判定
{
	return &g_vBulletPos;
}
//////////////////////////////////////////////////////////////////////

//他人との差を思い知らされるばかり...

//2018/11/18(1年後のおれ参上！！大丈夫！インターンシップ先でプログラム力褒められたぞ！「昔プログラムやってたの？」ってさｗ)
//初期化処理
void Sixyokika(void) 
{

	

	/////爆撃機///////
	bakugekikiPos.x = 1500.0f;//爆撃機移動
	bakudan = false;
	bakudan2 = false;
	bakudan3 = false;
	bakudan1Pos.y = 70.0f;//爆弾投下数値
	bakudan2Pos.y = 70.0f;//爆弾投下数値2
	bakudan3Pos.y = 70.0f;//爆弾投下数値3
	/////爆撃機/////

	//戦車//
	teki = true;//ここで敵をtrueにして表示する
	life1 = true;
	life2 = true;
	life3 = true;
	tekitama = false;
	sensixyaPos.x = 1000.0f;//画像の横移動用の数値
	houdanPos.x = 850.0f;//砲弾の移動数値
	//戦車//

	//プレイヤーの初期化//
	plyer = true;
	PlayerPos.x = 220.0f;//プレイヤーの初期ポジションx
	PlayerPos.y = 550.0f;//プレイヤーの初期ポジションy
	jump = 7.0f;//ジャンプ力
	gura = 3.0f;//重力
	//弾の初期化/////////////////////////////////////
	g_vBulletPos = D3DXVECTOR2(0.0f, 0.0f);//plyer
	playerBullet = false;//弾の初期状態をfalseにする//plyer
	//弾の初期化/////////////////////////////////////
	//プレイヤーの初期化//

	//脱出用車の初期化//
	car = false;
	zikan = 0.0f;//脱出よう車の時間を計る
	tekunikaruPos.x = 1000.0f;//脱出よう車の移動数値
	//脱出用車の初期化//

	
	//フィールド//
	kiritoriX = 0.0f;//画像の切り取り
	//フィールド//
	
}