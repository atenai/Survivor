//=======================================================================================
//						あたり判定描画
//						2017.06.05
//=======================================================================================
#include <math.h>
#include "judgement.h"
#include "main.h"


//注意！
//このあたり判定は中心座標から円を作っているのに対して、2D画像描画の方は左上を中心座標にして右下にのばしている為、あたり判定と2D画像がずれる
//対処法としてはまず画像の半径をとり、そこからはみ出た部分をx座標とy座標に足して正しい位置と大きさにするべし！

//GetEnemyPos()->x,    GetEnemyPos()->y,    ENEMY_WIDTH*0.5, GetBulletPos()->x,   GetBulletPos()->y,   BULLET_WIDTH*0.5
//ax = aの円の横の座標 ay = aの円の縦の座標 ar = aの円の半径 bx = bの円の横の座標 by = bの円の縦の座標 br = bの円の半径
bool atari(float ax, float ay, float ah, float bx, float by, float bh)//main.cpp(void Updata(void))//judgement.h
{
	float x = bx - ax;//自分のx座標 - 敵のx座標 = xの自分と敵の距離を出す
	float y = by - ay;//自分のy座標 - 敵のy座標 = yの自分と敵の距離を出す

	float zibuntekikixyori = x*x + y*y;//底辺の2乗 + 対辺の2乗 = 斜辺の2乗がでる(自分と敵の距離を出す)

	float zibuntekihankei = ah + bh; //自分と敵の半径を足した長さを出す

	return zibuntekikixyori < (zibuntekihankei * zibuntekihankei);//自分と敵の距離 < 自分と敵の半径を足した長さ の2乗(長さ比べ)
}

/*
bool atari(float zibunx, float zibuny, float zibunz,　float zibunhankei, float tekix, float tekiy, float tekiz, float tekihankei)
{

float x = zibunx - tekix; //自分のx座標 - 敵のx座標 = xの自分と敵の距離を出す
float y = zibuny - tekiy; //自分のy座標 - 敵のy座標 = yの自分と敵の距離を出す
float z = zibunz - tekiz; //自分のz座標 - 敵のz座標 = zの自分と敵の距離を出す

float zibuntekikixyori = x*x + y*y + z*z; //底辺の2乗 + 対辺の2乗 = 斜辺の2乗がでる(自分と敵の距離を出す)

float zibuntekihankei = zibunhankei + tekihankei; //自分と敵の半径を足した長さを出す

return zibuntekikixyori < (zibuntekihankei * zibuntekihankei); //自分と敵の距離 < 自分と敵の半径を足した長さ の2乗(長さ比べ)
}
*/