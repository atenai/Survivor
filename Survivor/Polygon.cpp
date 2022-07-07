//=============================================================================
//  
// ポリゴン描画
//2D画像を表示する為の
//SetPolygonColor();//ポリゴンカラーの設定
//DrawPolygon();//ポリゴンの設定
//の2つを使う為必要
//
//=============================================================================

#include <Windows.h>
#include<d3d9.h>
#include<d3dx9.h>
#include <math.h>
#include "Polygon.h"
#include "Main.h"

//FVFの宣言(定数)
//座標			//カラー		//テクスチャ
#define FVF_VERTEX_2D  (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)//設定

//===============
//構造体定義//いじらない
//===============
typedef struct
{
	D3DXVECTOR4 pos;//座標
	D3DCOLOR color;//カラー
	D3DXVECTOR2 texcoord;//テクスチャ
}VERTEX_2D;

//////////////////ここから↓の中身をいじる/////////////////////////////////


static D3DCOLOR g_color = D3DCOLOR_RGBA(255, 255, 255, 255);//プリミティブポリゴンの色//ポリゴンカラーの設定
static LPDIRECT3DTEXTURE9	g_pTexture[TEX_MAX] = { NULL };//ここで使うテクスチャの数だけNULLを追加しないといけない(2)

TEXTURE g_Texture[TEX_MAX] = //ここで使うテクスチャとその画像のサイズを入力する(3)
{
	{ "resource/soukou2.png", 48, 48 },//装甲車
	{ "resource/sensi.png", 96, 48 },//戦車
	{ "resource/tekunikaru.png", 48, 48 },//テクニカル
	{ "resource/zimen.png",50,2 },//地面
	{ "resource/sabaku2.jpg",1280,853 },//砂漠
	{ "resource/bullet.png",32,64 },//弾
	{ "resource/heisi3.png", 48, 48 },//兵士
	{ "resource/title.png", 900, 480 },//タイトル
	{ "resource/end.png", 900, 480 },//ゲームクリアー
	{ "resource/gameover.png",900,480 },//ゲームオーバー
	{ "resource/bakugeki.png",48,48 },//爆撃機
	{ "resource/bomb.png",48,48 },//爆弾
	{ "resource/misairu2.png",48,48 },//ミサイル
	{ "resource/banian.png", 48, 48 },//バーニアン
};

//ポリゴンの初期化処理
bool InitPolygon(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();//いじらない//main.h//pDeviceを使う場合これが必要
	if (pDevice == NULL)
	{
		return false;
	}

	//テクスチャ
	HRESULT hr;

	for (int i = 0; i < TEX_MAX; i++)
	{
		//テクスチャを生成する関数//いじらない
		hr = D3DXCreateTextureFromFile(pDevice, g_Texture[i].fileName, &g_pTexture[i]);
		//エラーチェック
		if (FAILED(hr))
		{
			MessageBox(NULL, "テクスチャが読み込めません", "エラー", MB_OK);
			return false;
		}

	}


	//α値
	//レンダーステートの設定
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//テクスチヤステージステートの設定
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	//
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);

	//フィルタリング
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	return true;
}

//ポリゴンの終了処理
void UninitPolygon(void)//いじらない
{
	int nIndex;

	for (nIndex = TEX_1; nIndex < TEX_MAX; nIndex++)
	{
		if (g_pTexture[nIndex] != NULL)
		{
			g_pTexture[nIndex]->Release();
			g_pTexture[nIndex] = NULL;
		}
	}

}

void DrawPolygon(int texNum, float dx, float dy, int dw, int dh, int tcx, int tcy, int tcw, int tch)//画像を表示
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();//いじらない

	if (pDevice == NULL) { return; }

	//////////////////ここから↓の中身をいじる/////////////////////////////////

	float tw = g_Texture[texNum].width;
	float th = g_Texture[texNum].height;

	float u0 = (float)tcx / (float)tw;
	float v0 = (float)tcy / (float)th;
	float u1 = (float)(tcx + tcw) / (float)tw;
	float v1 = (float)(tcy + tch) / (float)th;


	//　ポリゴンの描画処理
	VERTEX_2D vtx[] = {//D3DPT_TRIANGLESTRIP
		{ D3DXVECTOR4(dx + 0.0f - 0.5f, dy + 0.0f - 0.5f, 1.0f, 1.0f), g_color, D3DXVECTOR2(u0, v0) },//左上
		{ D3DXVECTOR4(dx + dw - 0.5f, dy + 0.0f - 0.5f, 1.0f, 1.0f), g_color, D3DXVECTOR2(u1, v0) },//右上
		{ D3DXVECTOR4(dx + 0.0f - 0.5f, dy + dh - 0.5f, 1.0f, 1.0f), g_color, D3DXVECTOR2(u0, v1) },//左下
		{ D3DXVECTOR4(dx + dw - 0.5f, dy + dh - 0.5f, 1.0f, 1.0f), g_color, D3DXVECTOR2(u1, v1) },//右下
	};

	//　FVF（今から使用する頂点情報）の設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	//テクスチャの貼り付け
	pDevice->SetTexture(0, g_pTexture[texNum]);

	//プリミティブ
	pDevice->DrawPrimitiveUP//これが呼ばれないとポリゴンが描画できない
	(
		D3DPT_TRIANGLESTRIP,//トライアングルストリップ//ポリゴンをどのように表示するか
		2,//三角形の面を2つ表示する//表示する面の数をいれる
		&vtx[0],//VERTEX_2D vtx[] = {};で作った内容を0番目から呼び出す
		sizeof(VERTEX_2D)//↑で作ったサイズを呼び出す
	);

	////////////////ここから↑の中身をいじる/////////////////////////////////////

}

void SetPolygonColor(D3DCOLOR color)//ポリゴンカラーの設定
{
	g_color = color;
}