//=============================================================================
//  
// �|���S���`��
//2D�摜��\������ׂ�
//SetPolygonColor();//�|���S���J���[�̐ݒ�
//DrawPolygon();//�|���S���̐ݒ�
//��2���g���וK�v
//
//=============================================================================

#include <Windows.h>
#include<d3d9.h>
#include<d3dx9.h>
#include <math.h>
#include "Polygon.h"
#include "Main.h"

//FVF�̐錾(�萔)
//���W			//�J���[		//�e�N�X�`��
#define FVF_VERTEX_2D  (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)//�ݒ�

//===============
//�\���̒�`//������Ȃ�
//===============
typedef struct
{
	D3DXVECTOR4 pos;//���W
	D3DCOLOR color;//�J���[
	D3DXVECTOR2 texcoord;//�e�N�X�`��
}VERTEX_2D;

//////////////////�������火�̒��g��������/////////////////////////////////


static D3DCOLOR g_color = D3DCOLOR_RGBA(255, 255, 255, 255);//�v���~�e�B�u�|���S���̐F//�|���S���J���[�̐ݒ�
static LPDIRECT3DTEXTURE9	g_pTexture[TEX_MAX] = { NULL };//�����Ŏg���e�N�X�`���̐�����NULL��ǉ����Ȃ��Ƃ����Ȃ�(2)

TEXTURE g_Texture[TEX_MAX] = //�����Ŏg���e�N�X�`���Ƃ��̉摜�̃T�C�Y����͂���(3)
{
	{ "resource/soukou2.png", 48, 48 },//���b��
	{ "resource/sensi.png", 96, 48 },//���
	{ "resource/tekunikaru.png", 48, 48 },//�e�N�j�J��
	{ "resource/zimen.png",50,2 },//�n��
	{ "resource/sabaku2.jpg",1280,853 },//����
	{ "resource/bullet.png",32,64 },//�e
	{ "resource/heisi3.png", 48, 48 },//���m
	{ "resource/title.png", 900, 480 },//�^�C�g��
	{ "resource/end.png", 900, 480 },//�Q�[���N���A�[
	{ "resource/gameover.png",900,480 },//�Q�[���I�[�o�[
	{ "resource/bakugeki.png",48,48 },//�����@
	{ "resource/bomb.png",48,48 },//���e
	{ "resource/misairu2.png",48,48 },//�~�T�C��
	{ "resource/banian.png", 48, 48 },//�o�[�j�A��
};

//�|���S���̏���������
bool InitPolygon(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();//������Ȃ�//main.h//pDevice���g���ꍇ���ꂪ�K�v
	if (pDevice == NULL)
	{
		return false;
	}

	//�e�N�X�`��
	HRESULT hr;

	for (int i = 0; i < TEX_MAX; i++)
	{
		//�e�N�X�`���𐶐�����֐�//������Ȃ�
		hr = D3DXCreateTextureFromFile(pDevice, g_Texture[i].fileName, &g_pTexture[i]);
		//�G���[�`�F�b�N
		if (FAILED(hr))
		{
			MessageBox(NULL, "�e�N�X�`�����ǂݍ��߂܂���", "�G���[", MB_OK);
			return false;
		}

	}


	//���l
	//�����_�[�X�e�[�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//�e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	//
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);

	//�t�B���^�����O
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	return true;
}

//�|���S���̏I������
void UninitPolygon(void)//������Ȃ�
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

void DrawPolygon(int texNum, float dx, float dy, int dw, int dh, int tcx, int tcy, int tcw, int tch)//�摜��\��
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();//������Ȃ�

	if (pDevice == NULL) { return; }

	//////////////////�������火�̒��g��������/////////////////////////////////

	float tw = g_Texture[texNum].width;
	float th = g_Texture[texNum].height;

	float u0 = (float)tcx / (float)tw;
	float v0 = (float)tcy / (float)th;
	float u1 = (float)(tcx + tcw) / (float)tw;
	float v1 = (float)(tcy + tch) / (float)th;


	//�@�|���S���̕`�揈��
	VERTEX_2D vtx[] = {//D3DPT_TRIANGLESTRIP
		{ D3DXVECTOR4(dx + 0.0f - 0.5f, dy + 0.0f - 0.5f, 1.0f, 1.0f), g_color, D3DXVECTOR2(u0, v0) },//����
		{ D3DXVECTOR4(dx + dw - 0.5f, dy + 0.0f - 0.5f, 1.0f, 1.0f), g_color, D3DXVECTOR2(u1, v0) },//�E��
		{ D3DXVECTOR4(dx + 0.0f - 0.5f, dy + dh - 0.5f, 1.0f, 1.0f), g_color, D3DXVECTOR2(u0, v1) },//����
		{ D3DXVECTOR4(dx + dw - 0.5f, dy + dh - 0.5f, 1.0f, 1.0f), g_color, D3DXVECTOR2(u1, v1) },//�E��
	};

	//�@FVF�i������g�p���钸�_���j�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);
	//�e�N�X�`���̓\��t��
	pDevice->SetTexture(0, g_pTexture[texNum]);

	//�v���~�e�B�u
	pDevice->DrawPrimitiveUP//���ꂪ�Ă΂�Ȃ��ƃ|���S�����`��ł��Ȃ�
	(
		D3DPT_TRIANGLESTRIP,//�g���C�A���O���X�g���b�v//�|���S�����ǂ̂悤�ɕ\�����邩
		2,//�O�p�`�̖ʂ�2�\������//�\������ʂ̐��������
		&vtx[0],//VERTEX_2D vtx[] = {};�ō�������e��0�Ԗڂ���Ăяo��
		sizeof(VERTEX_2D)//���ō�����T�C�Y���Ăяo��
	);

	////////////////�������灪�̒��g��������/////////////////////////////////////

}

void SetPolygonColor(D3DCOLOR color)//�|���S���J���[�̐ݒ�
{
	g_color = color;
}