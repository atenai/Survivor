
//=======================================================================================
//						ポリゴン描画
//					これはいじる必要無し
//=======================================================================================

#ifndef _POLYGON_H_
#define _POLYGON_H_
#include <d3d9.h>
#include <math.h>

typedef struct
{
	char fileName[256];
	int width;
	int height;
}TEXTURE;

//テクスチャ列挙型(0)
typedef enum
{
	TEX_1 = 0,//装甲車
	TEX_2,//戦車
	TEX_3,//テクニカル
	TEX_4,//地面
	TEX_5,//砂漠
	TEX_6,//弾
	TEX_7,//兵士
	TEX_8,//タイトル
	TEX_9,//ゲームクリアー
	TEX_10,//ゲームオーバー
	TEX_11,//爆撃機
	TEX_12,//爆弾
	TEX_13,//ミサイル
	TEX_14,//バーニアン
	TEX_MAX
}TEXTURE_NUM;

bool InitPolygon(void);//ポリゴンの初期化処理
void UninitPolygon(void);//ポリゴンの終了処理
void SetPolygonColor(D3DCOLOR color);//ポリゴンカラーの設定
void DrawPolygon(//画像を表示
	int texNum,//使うテクスチャ,
	float dx, //描画位置X軸
	float dy, //描画位置Y軸
	int dw, //テクスチャの表示する横の長さ
	int dh, //テクスチャの表示する縦の長さ
	int tcx, //元の画像の切り取り開始場所（横）（画像の左から数える）
	int tcy, //元の画像の切り取り開始場所（縦）（画像の上から数える）
	int tcw, //元の画像の切り取りたい幅
	int tch //元の画像の切り取りたい高さ
);//ポリゴンの描画処理

#endif // _POLYGON_H_
