//=======================================================================================
//						�����蔻��`��
//						2017.06.05
//=======================================================================================
#include <math.h>
#include "judgement.h"
#include "main.h"


//���ӁI
//���̂����蔻��͒��S���W����~������Ă���̂ɑ΂��āA2D�摜�`��̕��͍���𒆐S���W�ɂ��ĉE���ɂ̂΂��Ă���ׁA�����蔻���2D�摜�������
//�Ώ��@�Ƃ��Ă͂܂��摜�̔��a���Ƃ�A��������͂ݏo��������x���W��y���W�ɑ����Đ������ʒu�Ƒ傫���ɂ���ׂ��I

//GetEnemyPos()->x,    GetEnemyPos()->y,    ENEMY_WIDTH*0.5, GetBulletPos()->x,   GetBulletPos()->y,   BULLET_WIDTH*0.5
//ax = a�̉~�̉��̍��W ay = a�̉~�̏c�̍��W ar = a�̉~�̔��a bx = b�̉~�̉��̍��W by = b�̉~�̏c�̍��W br = b�̉~�̔��a
bool atari(float ax, float ay, float ah, float bx, float by, float bh)//main.cpp(void Updata(void))//judgement.h
{
	float x = bx - ax;//������x���W - �G��x���W = x�̎����ƓG�̋������o��
	float y = by - ay;//������y���W - �G��y���W = y�̎����ƓG�̋������o��

	float zibuntekikixyori = x*x + y*y;//��ӂ�2�� + �Εӂ�2�� = �Εӂ�2�悪�ł�(�����ƓG�̋������o��)

	float zibuntekihankei = ah + bh; //�����ƓG�̔��a�𑫂����������o��

	return zibuntekikixyori < (zibuntekihankei * zibuntekihankei);//�����ƓG�̋��� < �����ƓG�̔��a�𑫂������� ��2��(�������)
}

/*
bool atari(float zibunx, float zibuny, float zibunz,�@float zibunhankei, float tekix, float tekiy, float tekiz, float tekihankei)
{

float x = zibunx - tekix; //������x���W - �G��x���W = x�̎����ƓG�̋������o��
float y = zibuny - tekiy; //������y���W - �G��y���W = y�̎����ƓG�̋������o��
float z = zibunz - tekiz; //������z���W - �G��z���W = z�̎����ƓG�̋������o��

float zibuntekikixyori = x*x + y*y + z*z; //��ӂ�2�� + �Εӂ�2�� = �Εӂ�2�悪�ł�(�����ƓG�̋������o��)

float zibuntekihankei = zibunhankei + tekihankei; //�����ƓG�̔��a�𑫂����������o��

return zibuntekikixyori < (zibuntekihankei * zibuntekihankei); //�����ƓG�̋��� < �����ƓG�̔��a�𑫂������� ��2��(�������)
}
*/