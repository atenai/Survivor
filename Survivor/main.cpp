//=======================================================================================
//						���C���֐�
//						�����Y��
//�Ȃ񂩎�l���̑łe�Ɛ�Ԃ̗̑͂������蔻�������Ă���Œ��炵���̂Œ��ӁI
//=======================================================================================
#include <math.h>
#include "Polygon.h"
#include "input.h"
#include "judgement.h"
//***************************************************************************************
//	�}�N����`
//***************************************************************************************
#define CLASS_NAME		"DX2�N���g���N��"
#define WINDOW_NAME		"�T�o�C�o�["

//***************************************************************************************
//	�\���̐錾
//***************************************************************************************
//(1)�摜�\��
typedef struct
{
	D3DXVECTOR4 pos; //pos=�|���S�����W//�ŏ��̐��F�͌^��
	D3DCOLOR color; //���_�̐F
	D3DXVECTOR2 texcoord; //�e�N�X�`���i�O�`�P�j
}VERTEX_2D;

#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)


//***************************************************************************************
//	�v���g�^�C�v
//***************************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//////////////////////////////////////////////////
#define BULLET_SPEED (5.0f)//�e�̑���
#define BULLET_WIDTH (16.0f)//( ���� ),
#define BULLET_HEIGHT (16.0f)//( �c�� ),

void ShootBullet(float x, float y);
const D3DXVECTOR2* GetBulletPos(void);//main.cpp

static D3DXVECTOR2 g_vBulletPos(0.0f, 0.0f);//player

//////////////////////////////////////////////////

#define PLYAER_WIDTH (48.0f)
#define PLYAER_HEIGHT (48.0f)

#define ENEMY_WIDTH (96)//��Ԃ̉���
#define ENEMY_HEIGHT (48)//��Ԃ̏c��

#define CAR_WIDTH (48)//�E�o�悤�Ԃ̉����������蔻�萔�l
#define CAR_HEIGHT (48)//�E�o�悤�Ԃ̏c���������蔻�萔�l

//////////////////////////////////////////////////
//***************************************************************************************
//	�O���[�o���ϐ��錾
//***************************************************************************************
DWORD g_fOldTime;//�O�̃t���[���̎���//�Q�[�����[�v
DWORD g_fNowTime;//���̎���//�Q�[�����[�v
LPDIRECT3D9		g_pD3D = NULL;// Direct3D�C���^�[�t�F�C�X
static LPDIRECT3DDEVICE9	g_pD3DDevice = NULL;// Direct3DDevice�C���^�[�t�F�C�X
	
static int nflag = 0;//�^�C�g���˃Q�[���v���C�˃Q�[���I�[�o�[�˃^�C�g��

//static�͂��̃t�@�C�������̃O���[�o���ϐ��ł�,�t���Ȃ����͑S�t�@�C���g���܂�
//�v���C���[
static D3DXVECTOR2 PlayerPos(220.0f, 55.0f);//�v���C���[���W
static bool playerBullet;//bool��true��false�̂Q��//player�̒e�̕\���E��\��
static float jump = 7.0f;//�W�����v��
static float gura = 3.0f;//�d��
static bool plyer;//�v���C���[�̕\���E��\��

int jump2 = 0;
float v0 = 0;
float h = 0;
float t = 0;
float g = 9.8;

//�t�B�[���h
static float kiritoriX = 0.0f;//�摜�̐؂���

//�����@
static D3DXVECTOR2 bakugekikiPos(1500.0f, 10.0f);//�����@���W

static bool bakudan;//���e
static D3DXVECTOR2 bakudan1Pos(750.0f, 70.0f);//���e1�������W���l
static bool bakudan2;//���e2
static D3DXVECTOR2 bakudan2Pos(500.0f, 70.0f);//���e2�������W���l
static bool bakudan3;//���e3
static D3DXVECTOR2 bakudan3Pos(150.0f, 70.0f);//���e3�������W���l


//���
static bool teki;//�摜�̕\���E��\��
static D3DXVECTOR2 sensixyaPos(1000.0f, 472.0f);//��ԍ��W

static bool tekitama;////�C�e�̕\���E��\��
static D3DXVECTOR2 houdanPos(850.0f, 555.0f);//�C�e�̍��W���l
static bool life1;
static bool life2;
static bool life3;

//�e�N�j�J��
static float zikan = 0.0f;//�E�o�悤�Ԃ̎��Ԃ��v��
static bool car;//�E�o�悤�Ԃ̕\���E��\��
static D3DXVECTOR2 tekunikaruPos(1000.0f, 550.0f);//�E�o�悤�Ԃ̍��W���l

//=======================================================================================
//�@�G���g���[�|�C���g�֐�
//=======================================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)//void main�悤�Ȃ���																	
{

	lpCmdLine = lpCmdLine;//�x�����p
	hPrevInstance = hPrevInstance;//�x�����p

	WNDCLASSEX wc = {//�K�v�ȏ�������
						sizeof(WNDCLASSEX),//�\���̂̃T�C�Y
						CS_HREDRAW | CS_VREDRAW,//�E�B���h�E�N���X�̃X�^�C��
						WndProc,//�E�B���h�E�v���V�[�W���̃A�h���X
						0,//�ǉ����������K�v�ȏꍇ�A���̃o�C�g�����w��
						0,//����
						hInstance,//�N���X���쐬����C���X�^���X�n���h��
						NULL,//�A�C�R���̃n���h��
						LoadCursor(NULL,IDC_ARROW),//�J�[�\��(�}�E�X�|�C���^)�̃n���h��
						(HBRUSH)(COLOR_WINDOW + 1),//�w�i�F�p�̃u���V�̃n���h��
						NULL,//���j���[���\�[�X��\�����O
						CLASS_NAME,//�E�B���h�E�N���X�̖��O
						NULL//�A�C�R��(��)�̃n���h��
						};

	RegisterClassEx(//WNDCLASSEX�œ��ꂽ����windows�ɓo�^����
					&wc//WNDCLASSEX�\���̂̃|�C���^
					);

	HWND hWnd = CreateWindowEx(//�E�B���h�E���쐬����
							0,//??
							CLASS_NAME,//�o�^����Ă���N���X��
							WINDOW_NAME,//�E�B���h�E��
							WS_OVERLAPPEDWINDOW & ~(WS_MAXIMIZEBOX | WS_THICKFRAME),//�E�B���h�E�X�^�C��	
							CW_USEDEFAULT,//�E�B���h�E��x�ʒu
							CW_USEDEFAULT,//�E�B���h�E��y�ʒu
							SCREEN_WIDTH,//�E�B���h�E�̉��T�C�Y
							SCREEN_HEIGHT,//�E�B���h�E�̏c�T�C�Y
							NULL,//�e�E�B���h�E�܂��̓I�[�i�[�E�B���h�E�̃n���h��
							NULL,//���j���[�n���h���܂��͎q�E�B���h�E
							hInstance,//�A�v���P�[�V�����C���X�^���X�̃n���h��
							NULL//�E�B���h�E�쐬�̃f�[�^
							);
	
	ShowWindow(//CreateWindow�ō쐬�����E�B���h�E��\������
		hWnd,//�E�B���h�E�n���h��
		nCmdShow//�\�����
	);


	MSG msg;//PeekMessage�֐�����M�����C�x���g���b�Z�[�W��MSG�\���̂̕ϐ��ɋL�^�����

	//����������(�E�B���h�E���쐬���Ă���
	if (!Init(hInstance, hWnd, TRUE))
	{
		MessageBox(hWnd, "�I�����܂�", "�������ł��Ȃ�", MB_OK);
		return -1;
	}


	

	//======================�Q�[�����[�v�J�n==============
	//����\
	timeBeginPeriod(1);

	for (;;)	//for����
	{
		//���b�Z�[�W���[�v�܂�E�B���h�E��\������������
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))//Windows����C�x���g���b�Z�[�W���󂯎��
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				//���b�Z�[�W����
				TranslateMessage(&msg);//WM_KEYDOWN���b�Z�[�W����WM_CHAR���b�Z�[�W���쐬
				DispatchMessage(&msg);//�E�B���h�E�v���V�[�W���ɃC�x���g���b�Z�[�W��]��
			}
		}
		else
		{
			//�Q�[������
			g_fNowTime = timeGetTime();//���̎��� = timeGetTime();
									   //		�~���b�@�@�@�~���b
			if ((g_fNowTime - g_fOldTime) >= (1000 / 60))//if((���̎��� - �O�̃t���[���̎���) >= (1000/60))
			{

				//�^�C�g���˃Q�[���v���C�˃Q�[���I�[�o�[�˃^�C�g��
				if (nflag == 0)
				{
					//�^�C�g�����
					title();
				}

				if (nflag == 1)
				{
					//�Q�[���J�n
					game();

				}

				if (nflag == 2)
				{
					//�Q�[���I��
					end();
				}

				//�^�C�g����ʂɖ߂鏈��
				if (nflag == 3)
				{
					//�Q�[���I�[�o�[
					gameover();
				}


				g_fOldTime = g_fNowTime;//�O�t���[���̎��� = ���̎���;
			}
		}
	}

	timeEndPeriod(1);
	//=======================�Q�[�����[�v����============================

	


	Uninit();//�I������

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
			//BUTTON�������ꂽ
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		case WM_CLOSE://x�{�^������`
			nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO);
			if (nID == IDYES)
			{
				DestroyWindow(hWnd);      //�E�B���h�E�����
			}
			else
			{
				return 0;
			}
			break;

		case WM_KEYDOWN:
			switch (wParam)
			{
			case VK_ESCAPE://[ESC]����������
				nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO);
				if (nID == IDYES)
				{
					DestroyWindow(hWnd);      //�E�B���h�E�����
				}
				break;
			case VK_RETURN://�G���^�[�L�[�������ꂽ��
				if (nflag == 0)//�����^�C�g����ʂ�������
				{
					nflag = 1;//�Q�[���v���C��ʂֈړ�
				}
				if (nflag == 2 || nflag == 3)//�Q�[���N���A�[��ʂ܂��̓Q�[���I�[�o�[��ʂ�������
				{
					nflag = 0;//�^�C�g����ʂֈړ�
				}
				break;
			case VK_SPACE://�X�y�[�X�L�[�������ꂽ��
				if (nflag == 0)//�����^�C�g����ʂ�������
				{
					nflag = 1;//�Q�[���v���C��ʂֈړ�
				}
				if (nflag == 2 || nflag == 3)//�Q�[���N���A�[��ʂ܂��̓Q�[���I�[�o�[��ʂ�������
				{
					nflag = 0;//�^�C�g����ʂֈړ�
				}
				break;

			}
			break;

	default:
	break;
	}

	//DefWindowProc:�E�B���h�E�����̊֐�
	return DefWindowProc(hWnd, Msg, wParam, lParam);
}
//=================================================
//		����������
//=================================================
bool Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	//Direct3D�I�u�W�F�N�g���쐬//no
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return false;
	}

	//���݂̃f�B�X�v���C���[�h��
	D3DDISPLAYMODE d3ddm;

	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return false;
	}

	//�f�o�C�X�̃v���[���e�[�V�����o�����[�^
	D3DPRESENT_PARAMETERS	d3dpp;
	//�\���̂̒��ɑS��0�ɂ���
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	//����ʂ̕�
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	//����ʂ̍���
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	//�J���[���[�h
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.BackBufferCount = 1;
	//�f���M��
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	//2�����͂���Ȃ��@3D�͕K�v
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	//true =window���[�h	false = screen���[�h
	d3dpp.Windowed = bWindow;
	//�t���[���[�h�����ŉ�ʃT�C�Y
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	//��ʍX�V
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	//�f�o�C�X�̐���
	if (FAILED(g_pD3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{
		//return�̃��b�Z�[�Wbox
		MessageBox(hWnd, "�G���[", "�N�����s", MB_OK);
		return false;
	}

	//�����_�[�X�e�[�g�̐ݒ�
	//���u�����h���s��
	//SRC....������`�����́B�܂�|���S��
	//DEST...���łɕ`�悳��Ă����ʂ̂���
	//SRC_RGB*SRC_��+DEST_RGB*(1-SRC_��)
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);//���\�[�X
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);//���f�X�g

	//�e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�		//�����Z�q			�|���Z
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	//�T���v���[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);//(�e�N�X�`����(0~1)���W�O�̑Ώ�)
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_BORDER);//D3DTADDRESS_MIRROR(���_ �������������R�b�s�[����180�x���])

	//�t�B���^�����O
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);


	InitKeyboard(hInstance, hWnd);//������Ȃ�

	//�|���S�����������s�Ώ�
	if (!(InitPolygon()))//polygon.h//������Ȃ�
	{
		MessageBox(hWnd,"�|���S���̏��������s����","�G���[",MB_OK);
		return false;
	}

	///////////////////////////�G�̏�����/////////////////
	teki = true;//�����œG��true�ɂ��ĕ\������
	///////////////////////////�G�̏�����/////////////////
	//�v���C���[�̏����|�W�V����/////////////////////////
	PlayerPos.x = 220.0f;//�v���C���[�̏����|�W�V����x
	PlayerPos.y = 550.0f;//�v���C���[�̏����|�W�V����y
	//�v���C���[�̏����|�W�V����/////////////////////////
	//�e�̏�����/////////////////////////////////////
	g_vBulletPos = D3DXVECTOR2(0.0f, 0.0f);//plyer
	playerBullet = false;//�e�̏�����Ԃ�false�ɂ���//plyer
	//�e�̏�����/////////////////////////////////////
	/////���e///////
	bakudan = false;
	bakudan2 = false;
	bakudan3 = false;
	/////���e/////
	//life//
	life1 = true;
	life2 = true;
	life3 = true;
	//life//
	//�v���C���[�̏�����//
	plyer = true;
	//�v���C���[�̏�����//
	//�E�o�p�Ԃ̏�����//
	car = false;
	//�E�o�p�Ԃ̏�����//
	//�G�̒e///
	tekitama = false;
	//�G�̒e///


	return true;
}

void title(void) 
{
	//�`��
	Draw();
	//�Q�[���̏���������
	Sixyokika();
}

void game(void) 
{
	//�X�V
	Update();
	//�`��
	Draw();
}

void end(void) 
{
	//�`��
	Draw();
}

void gameover(void) 
{
	//�`��
	Draw();
}


void Update(void)
{
	UpdateKeyboard();//input.h//������Ȃ�
/////////////////�������火�̒��g��������/////////////////////////////////
////�v���C���[�̃A�b�v�f�[�g/////////////////////////////
	if (GetKeyboardPress(DIK_W))//�L�[�{�[�h��W����������//input.h
	{
		PlayerPos.y = PlayerPos.y - jump;//��ړ�
		//jump2 = 1;
		//v0 = 40.0;//�����x 
		//t = 0;
	}

	//if (jump2 == 1) 
	//{
	//	h = v0 * t - (1.0 / 2.0)*g*t*t;//����
	//	PlayerPos.y = PlayerPos.y - h ;//
	//	t += 1;
	//	if (PlayerPos.y > 571) 
	//	{
	//		jump2 = 0;
	//		PlayerPos.y = 571;
	//	}
	//}


	if (GetKeyboardPress(DIK_S))//�L�[�{�[�h��S����������
	{
		//tateidou = tateidou + 5;//���ړ�
	}
	if (GetKeyboardPress(DIK_A))//�L�[�{�[�h��A����������
	{
		PlayerPos.x = PlayerPos.x - 5.0f;//���ړ�
	}
	if (GetKeyboardPress(DIK_D))//�L�[�{�[�h��D����������
	{
		PlayerPos.x = PlayerPos.x + 5.0f;//�E�ړ�
	}

	if (GetKeyboardPress(DIK_L))//�e������
	{
		ShootBullet(PlayerPos.x, PlayerPos.y);//ShootBullet(x�̃|���S���\�����W,y�̃|���S���\�����W);
	}

////�v���C���[�̏d��/////////////////////////////
	if (572 > PlayerPos.y)//�v���C���[��y���W��562��菬�����ꍇ
	{
		PlayerPos.y = PlayerPos.y + gura;//���ړ�//�d��
	}

////�X�e�[�W�͈̔͐ݒ�ƃo�b�N�O���E���h�摜/////////////////////////////
	if ((572 < PlayerPos.y))//�L�����N�^�[�̃|�W�V����Y��tateidou���傫���ꍇ(�t�B�[���h�̉��ݒ�)
	{
		PlayerPos.y = 571;//�L�����N�^�[�̏��ʒu  
	}
	if (PlayerPos.y < 180)//�L�����N�^�[�̃|�W�V����Y��tateidou��菬���������ꍇ(�t�B�[���h�̏�ݒ�)
	{
		PlayerPos.y = 181;//�L�����N�^�[�̏��ʒu  
	}
	if (PlayerPos.x < 0.0f)//�L�����N�^�[�̃|�W�V����X��yokoidou��菬���������ꍇ(�t�B�[���h�̍��ݒ�)
	{
		PlayerPos.x = 0.0f;
	}
	if (PlayerPos.x > 1270.0f)//�L�����N�^�[�̃|�W�V����X��yokoidou���傫���ꍇ(�t�B�[���h�̉E�ݒ�)
	{
		PlayerPos.x = 1270.0f;
	}

	//�w�i�𓮂���
	kiritoriX = kiritoriX + 2.0f;
	if (kiritoriX > 1080.0f) 
	{
		kiritoriX = 0.0f;
	}
////�X�e�[�W�ݒ�/////////////////////////////

////�e�̃A�b�v�f�[�g///////////////////////////////////////
	if (playerBullet)//true�̏ꍇ//player
	{
//�e�̒e��
		g_vBulletPos.x += BULLET_SPEED;//�e���E�ɂ���
		if (g_vBulletPos.x + BULLET_WIDTH>1024)//�e�̈ʒu��0�ȉ��Ȃ�
		{
			playerBullet = false;//�e�̕\��������
		}	
	}
////�e�̃A�b�v�f�[�g///////////////////////////////////////

/////�G�̃A�b�v�f�[�g////////////////////////////////////////
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


///////////////�G�̈ړ�///////////////////////////////////////////////////////////
	//��Ԃ̈ړ�
	if (sensixyaPos.x >= 850.0f && sensixyaPos.x <= 1000.0f)//idoux�̐��l��650�`850�̊ԂȂ�摜�����Ɉړ� 
	{
		sensixyaPos.x = sensixyaPos.x - 5.0f;//���Ɉړ�
	}
	else if (sensixyaPos.x <= 850.0f)//idoux��650�ȉ��Ȃ�645�Ő��l���Œ� 
	{
		sensixyaPos.x = 845;
	}

	//�G�̒e�𔭎�
	if (sensixyaPos.x == 845 )
	{
		tekitama = true;
	}
	if (tekitama == true) 
	{
		houdanPos.x = houdanPos.x - 5;//�C�e�����Ɉړ�
	}
	if (houdanPos.x < -50)//�e�̈ʒu��0�ȉ��Ȃ�
	{
		tekitama = false;//�e�̕\��������
		houdanPos.x = 950;
	}
	

	//�����@�ړ�
	bakugekikiPos.x = bakugekikiPos.x - 3.0f;
	if (bakugekikiPos.x < -200.0f) {
		bakugekikiPos.x = 1500.0f;
		
	}
	//���e����
	if (bakugekikiPos.x == 750.0f) {
		bakudan = true;
	}
	if (bakudan) {
		bakudan1Pos.y = bakudan1Pos.y + 2.0f;//���Ɉړ�
	}
	if (bakudan1Pos.y > 900.0f) {
		bakudan = false;
		bakudan1Pos.y = 70.0f;
	}
	
	//���e����2
	if (bakugekikiPos.x <= 500.0f) {
		bakudan2 = true;
	}
	if (bakudan2) {
		bakudan2Pos.y = bakudan2Pos.y + 2.0f;//���Ɉړ�
	}
	if (bakudan2Pos.y > 900.0f) {
		bakudan2 = false;
		bakudan2Pos.y = 70.0f;
	}
	
	//���e����3
	if (bakugekikiPos.x <= 150.0f) {
		bakudan3 = true;
	}
	if (bakudan3) {
		bakudan3Pos.y = bakudan3Pos.y + 2.0f;//���Ɉړ�
	}
	if (bakudan3Pos.y > 900.0f) {
		bakudan3 = false;
		bakudan3Pos.y = 70.0f;
	}
	/*
	//�~�T�C���ړ�
	misairuidou = misairuidou - 10;
	if (misairuidou < -50) {
		misairuidou = 1500;
	}
	*/
///////////////�G�̈ړ�///////////////////////////////////////////////////////////

/////�G�̃A�b�v�f�[�g////////////////////////////////////////

//�����蔻��//////////////////////////////////////////////////////////////////////////////
		//�G & �v���C���[�̒e////////////////////////////////////////
			bool bHit = atari(//judgement.h//�����蔻��//teki
				sensixyaPos.x, sensixyaPos.y, ENEMY_WIDTH*0.5,//�G�̈ʒu�Ƃ��͈̔͂��w��//48*0.5=24
				GetBulletPos()->x, GetBulletPos()->y, BULLET_WIDTH*0.5//�e�̈ʒu
			);
			if (life1 == false && life2 == false && life3 == true) {
				if (bHit)//���������牺�̓��e���N��
				{
					playerBullet = false;//�v���C���[�̒e������
					life3 = false;
				}
			}
			if (life1 == false && life2 == true && life3 == true) {
				if (bHit)//���������牺�̓��e���N��
				{
					playerBullet = false;//�v���C���[�̒e������
				
					life2 = false;
				}
			}
			if (life1 == true && life2 == true && life3 == true) {
				if (bHit)//���������牺�̓��e���N��
				{
					playerBullet = false;//�v���C���[�̒e������
					life1 = false;
				}
			}
			if (life1 == false && life2 == false && life3 == false) {
				teki = false;//�G������
				sensixyaPos.x = -50;//�G����ʊO�ɏo���ז�������
			}
			//�G & �v���C���[�̒e////////////////////////////////////////

			//�v���C���[ & ���e1//////////////////////////////////////////
			bool bHit1 = atari(//judgement.h//�����蔻��//plyer
				PlayerPos.x + 48.0f, PlayerPos.y + 48.0f, 48.0f,//�v���C���[�̈ʒu
				bakudan1Pos.x + 24.0f, bakudan1Pos.y + 24.0f, 24.0f//���e�̈ʒu
			);
			if (bHit1) {
				plyer = false;
			}
			//�v���C���[ & ���e1//////////////////////////////////////////
			//�v���C���[ & ���e2//////////////////////////////////////////
			bool bHit2 = atari(//judgement.h//�����蔻��//plyer
				PlayerPos.x + 48.0f, PlayerPos.y + 48.0f, 48.0f,//�v���C���[�̈ʒu
				bakudan2Pos.x + 24.0f, bakudan2Pos.y + 24.0f, 24.0f//���e�̈ʒu
			);
			if (bHit2) {
				plyer = false;
			}
			//�v���C���[ & ���e2//////////////////////////////////////////
			//�v���C���[ & ���e3//////////////////////////////////////////
			bool bHit3 = atari(//judgement.h//�����蔻��//plyer
				PlayerPos.x + 48.0f, PlayerPos.y + 48.0f, 48.0f,//�v���C���[�̈ʒu
				bakudan3Pos.x + 24.0f, bakudan3Pos.y + 24.0f, 24.0f//���e�̈ʒu
			);
			if (bHit3) {
				plyer = false;
			}
			//�v���C���[ & ���e3//////////////////////////////////////////
			//�v���C���[ & �C�e//////////////////////////////////////////
			bool bHit4 = atari(//judgement.h//�����蔻��//plyer
				PlayerPos.x + 48.0f, PlayerPos.y + 48.0f, 48.0f,//�v���C���[�̈ʒu
				houdanPos.x + 24.0f, houdanPos.y + 24.0f, 24.0f//�C�e�̈ʒu
			);
			if (bHit4) {
				plyer = false;
			}
			//�v���C���[ & �C�e//////////////////////////////////////////
			//�v���C���[ & ������//////////////////////////////////////////
			bool bHit5 = atari(//judgement.h//�����蔻��//plyer
				PlayerPos.x + 48.0f, PlayerPos.y + 48.0f, 48.0f,//�v���C���[�̈ʒu
				tekunikaruPos.x + 72.0f, tekunikaruPos.y + 72.0f, 72.0f//�e�N�j�J���̈ʒu
			);
			if (bHit5) {
				//�Q�[���N���A�[����
				nflag = 2;
			}
			//�v���C���[ & �C�e//////////////////////////////////////////
//�����蔻��//////////////////////////////////////////////////////////////////////////////	
			if (plyer == false) 
			{
				nflag = 3;//�Q�[���I�[�o�[����
			}
////////////////�������灪�̒��g��������/////////////////////////////////////	


}

//=================================
//		�`�揈��
//=================================
void Draw(void)
{//��ʃN���A
	g_pD3DDevice->Clear(
		0,
		NULL,
		//�^�[�Q�b�g�@�@�@�@ZBUFFER
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(32, 64, 128, 255),//�w�i�F�̕ύX
		1.0f,
		0
		);

	//Direct3D�ɂ��`��̊J�n
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{

//////////////////�������火�̒��g��������/////////////////////////////////
		//�^�C�g��//
		if (nflag == 0) 
		{

			//�^�C�g�����
			SetPolygonColor(D3DCOLOR_RGBA(255, 255, 255, 255));//�|���S���J���[�̐ݒ�
			DrawPolygon(
				TEX_8,//�g���e�N�X�`��,
				0.0f, //x���W,
				0.0f,//y���W,
				1366,//�e�N�X�`���̕\�����鉡�̒���
				768,//�e�N�X�`���̕\������c�̒���
				0,//���̉摜�̐؂���J�n�ꏊ�i���j�i�摜�̍����琔����j
				0,//���̉摜�̐؂���J�n�ꏊ�i�c�j�i�摜�̏ォ�琔����j
				900,//���̉摜�̐؂��肽����
				480 //���̉摜�̐؂��肽������
			);
		}
		//�Q�[���N���A�[
		if (nflag == 2) 
		{
			SetPolygonColor(D3DCOLOR_RGBA(255, 255, 255, 255));//�|���S���J���[�̐ݒ�
			DrawPolygon(
				TEX_9,//�g���e�N�X�`��,
				0.0f, //x���W,
				0.0f,//y���W,
				1366,//�e�N�X�`���̕\�����鉡�̒���
				768,//�e�N�X�`���̕\������c�̒���
				0,//���̉摜�̐؂���J�n�ꏊ�i���j�i�摜�̍����琔����j
				0,//���̉摜�̐؂���J�n�ꏊ�i�c�j�i�摜�̏ォ�琔����j
				900,//���̉摜�̐؂��肽����
				480 //���̉摜�̐؂��肽������
			);
		}
		//�Q�[���I�[�o�[
		if (nflag == 3) 
		{
			SetPolygonColor(D3DCOLOR_RGBA(255, 255, 255, 255));//�|���S���J���[�̐ݒ�
			DrawPolygon(
				TEX_10,//�g���e�N�X�`��,
				0.0f, //x���W,
				0.0f,//y���W,
				1366,//�e�N�X�`���̕\�����鉡�̒���
				768,//�e�N�X�`���̕\������c�̒���
				0,//���̉摜�̐؂���J�n�ꏊ�i���j�i�摜�̍����琔����j
				0,//���̉摜�̐؂���J�n�ꏊ�i�c�j�i�摜�̏ォ�琔����j
				900,//���̉摜�̐؂��肽����
				480 //���̉摜�̐؂��肽������
			);
		}

		if (nflag == 1) 
		{
			/////////////////�w�i�̕`��////////////////////////
			SetPolygonColor(D3DCOLOR_RGBA(255, 255, 255, 255));//�|���S���J���[�̐ݒ�
			DrawPolygon(
				TEX_5,//�g���e�N�X�`��,
				0.0f, //x���W,
				0.0f,//y���W,
				1366,//�e�N�X�`���̕\�����鉡�̒���
				768,//�e�N�X�`���̕\������c�̒���
				kiritoriX,//���̉摜�̐؂���J�n�ꏊ�i���j�i�摜�̍����琔����j
				300,//���̉摜�̐؂���J�n�ꏊ�i�c�j�i�摜�̏ォ�琔����j
				200,//���̉摜�̐؂��肽����
				200 //���̉摜�̐؂��肽������
			);
			/////////////////�w�i�̕`��////////////////////////
			//////////////////�v���C���[�̕`��//////////////////	
			if (plyer == true) 
			{
				SetPolygonColor(D3DCOLOR_RGBA(255, 255, 255, 255));//�|���S���J���[�̐ݒ�
				DrawPolygon(
					TEX_7,//�g���e�N�X�`��,
					PlayerPos.x, //x���W,
					PlayerPos.y,//y���W,
					PLYAER_WIDTH * 2.0f,//�e�N�X�`���̕\�����鉡�̒���
					PLYAER_HEIGHT * 2.0f,//�e�N�X�`���̕\������c�̒���
					0,//���̉摜�̐؂���J�n�ꏊ�i���j�i�摜�̍����琔����j
					0,//���̉摜�̐؂���J�n�ꏊ�i�c�j�i�摜�̏ォ�琔����j
					48,//���̉摜�̐؂��肽����
					48 //���̉摜�̐؂��肽������
				);
			}
			//////////////////�v���C���[�̕`��//////////////////

			////////////////////�e�̕`��///////////////////////////
			if (playerBullet)//true�̏ꍇ//player
			{
				//�v���C���[�̒e�̃e�N�X�`����`��
				DrawPolygon(
					TEX_6,//�e�N�X�`��,
					g_vBulletPos.x + 80,//x�̃|���S���\�����W,
					g_vBulletPos.y + 8,//y�̃|���S���\�����W,
					BULLET_WIDTH,//( ���� ),
					BULLET_HEIGHT,//( �c�� ),
					0,//�؂���J�n�n�_x,
					0,//�؂���J�n�n�_y,
					16,//�؂��蕝,
					16//�؂��荂��
				);//bullet	

			}

			////////////////////�e�̕`��///////////////////////////
			////////////////�����Ԃ̕`��////////////////////////////////
			if (car == true)
			{
				DrawPolygon
				(
					TEX_3,//�e�N�X�`��,
					tekunikaruPos.x,//x�̃|���S���\�����W,
					tekunikaruPos.y,//y�̃|���S���\�����W,
					CAR_WIDTH * 3.0f,//( ���� ),
					CAR_HEIGHT * 3.0f,//( �c�� ),
					0,//�؂���J�n�n�_x,
					0,//�؂���J�n�n�_y,
					48,//�؂��蕝,
					48//�؂��荂��
				);//enemy
			}
			////////////////�����Ԃ̕`��////////////////////////////////
			////////////////��Ԃ̕`��////////////////////////////////
			if (teki == true)
			{
				DrawPolygon
				(
					TEX_2,//�e�N�X�`��,
					sensixyaPos.x,//x�̃|���S���\�����W,
					sensixyaPos.y,//y�̃|���S���\�����W,
					ENEMY_WIDTH * 5.0f,//( ���� ),
					ENEMY_HEIGHT * 5.0f,//( �c�� ),
					0,//�؂���J�n�n�_x,
					0,//�؂���J�n�n�_y,
					96,//�؂��蕝,
					48//�؂��荂��
				);//enemy
			}
			////////////////��Ԃ̕`��////////////////////////////////
					//��Ԃ̖C�e///
			if (tekitama) {
				DrawPolygon(
					TEX_6,//�e�N�X�`��,
					houdanPos.x,//x�̃|���S���\�����W,
					houdanPos.y,//y�̃|���S���\�����W,
					BULLET_WIDTH * 3,//( ���� ),
					BULLET_HEIGHT * 3,//( �c�� ),
					0,//�؂���J�n�n�_x,
					0,//�؂���J�n�n�_y,
					16,//�؂��蕝,
					16//�؂��荂��
				);//bullet
			}
			//��Ԃ̖C�e///
	////////////�����@///////////////////////////////////
			DrawPolygon
			(
				TEX_11,//�e�N�X�`��,
				bakugekikiPos.x,//x�̃|���S���\�����W,
				bakugekikiPos.y,//y�̃|���S���\�����W,
				48*3,//( ���� ),
				48*3,//( �c�� ),
				0,//�؂���J�n�n�_x,
				0,//�؂���J�n�n�_y,
				48,//�؂��蕝,
				48//�؂��荂��
			);//enemy
	////////////�����@///////////////////////////////////

	////////////////���e�̕`��////////////////////////////////
			if (bakudan) {
				DrawPolygon
				(
					TEX_12,//�e�N�X�`��,
					bakudan1Pos.x,//x�̃|���S���\�����W,
					bakudan1Pos.y,//y�̃|���S���\�����W,
					48,//( ���� ),
					48,//( �c�� ),
					0,//�؂���J�n�n�_x,
					0,//�؂���J�n�n�_y,
					48,//�؂��蕝,
					48//�؂��荂��
				);
			}
			if (bakudan2) {
				DrawPolygon
				(
					TEX_12,//�e�N�X�`��,
					bakudan2Pos.x,//x�̃|���S���\�����W,
					bakudan2Pos.y,//y�̃|���S���\�����W,
					48,//( ���� ),
					48,//( �c�� ),
					0,//�؂���J�n�n�_x,
					0,//�؂���J�n�n�_y,
					48,//�؂��蕝,
					48//�؂��荂��
				);
			}
			if (bakudan3) {
				DrawPolygon
				(
					TEX_12,//�e�N�X�`��,
					bakudan3Pos.x,//x�̃|���S���\�����W,
					bakudan3Pos.y,//y�̃|���S���\�����W,
					48,//( ���� ),
					48,//( �c�� ),
					0,//�؂���J�n�n�_x,
					0,//�؂���J�n�n�_y,
					48,//�؂��蕝,
					48//�؂��荂��
				);
			}
			////////////////���e�̕`��////////////////////////////////
			/////////////////�n�ʂ̕`��////////////////////////
			DrawPolygon(
				TEX_4,//�e�N�X�`��,
				0,//x�̃|���S���\�����W,
				668,//y�̃|���S���\�����W,
				1366,//����,
				200,//�c��,
				0,//kiritoriX,//�؂���J�n�n�_x,
				0,//kiritoriY,//�؂���J�n�n�_y,
				50,//�؂��蕝,
				2//�؂��荂��
			);
			/////////////////�n�ʂ̕`��////////////////////////
		}
////////////////�������灪�̒��g��������/////////////////////////////////////

		g_pD3DDevice->EndScene();//Direct3D�ɂ��`��̏I��//������Ȃ�
	}
	//�K��
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}


LPDIRECT3DDEVICE9 GetD3DDevice(void)
{
	//�f�o�C�X�̎擾
	return g_pD3DDevice;
}

//====================================
//		�I������
//====================================
void Uninit(void)
{
	UninitKeyboard();//input.h//������Ȃ�

	UninitPolygon();//polygon.h//������Ȃ�

	if (g_pD3DDevice != NULL)
	{//�f�o�C�X���
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	if (g_pD3D != NULL)
	{//Direct3D�I�u�W�F�N�g�̊J��
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

//////////////////////////////////////////////////////////////////////

void ShootBullet(float x, float y)//bullet.h//player.cpp//�v���C���[�̒e������
{
	if (!playerBullet)//!��false�̏ꍇ
	{
		playerBullet = true;
		//g_vBulletPos = D3DXVECTOR2(x, y);
		g_vBulletPos.x = x;
		g_vBulletPos.y = y;
	}
}

const D3DXVECTOR2* GetBulletPos(void)//bullet.h//main.cpp//�����蔻��
{
	return &g_vBulletPos;
}
//////////////////////////////////////////////////////////////////////

//���l�Ƃ̍����v���m�炳���΂���...

//2018/11/18(1�N��̂���Q��I�I���v�I�C���^�[���V�b�v��Ńv���O�����͖J�߂�ꂽ���I�u�̃v���O��������Ă��́H�v���Ă���)
//����������
void Sixyokika(void) 
{

	

	/////�����@///////
	bakugekikiPos.x = 1500.0f;//�����@�ړ�
	bakudan = false;
	bakudan2 = false;
	bakudan3 = false;
	bakudan1Pos.y = 70.0f;//���e�������l
	bakudan2Pos.y = 70.0f;//���e�������l2
	bakudan3Pos.y = 70.0f;//���e�������l3
	/////�����@/////

	//���//
	teki = true;//�����œG��true�ɂ��ĕ\������
	life1 = true;
	life2 = true;
	life3 = true;
	tekitama = false;
	sensixyaPos.x = 1000.0f;//�摜�̉��ړ��p�̐��l
	houdanPos.x = 850.0f;//�C�e�̈ړ����l
	//���//

	//�v���C���[�̏�����//
	plyer = true;
	PlayerPos.x = 220.0f;//�v���C���[�̏����|�W�V����x
	PlayerPos.y = 550.0f;//�v���C���[�̏����|�W�V����y
	jump = 7.0f;//�W�����v��
	gura = 3.0f;//�d��
	//�e�̏�����/////////////////////////////////////
	g_vBulletPos = D3DXVECTOR2(0.0f, 0.0f);//plyer
	playerBullet = false;//�e�̏�����Ԃ�false�ɂ���//plyer
	//�e�̏�����/////////////////////////////////////
	//�v���C���[�̏�����//

	//�E�o�p�Ԃ̏�����//
	car = false;
	zikan = 0.0f;//�E�o�悤�Ԃ̎��Ԃ��v��
	tekunikaruPos.x = 1000.0f;//�E�o�悤�Ԃ̈ړ����l
	//�E�o�p�Ԃ̏�����//

	
	//�t�B�[���h//
	kiritoriX = 0.0f;//�摜�̐؂���
	//�t�B�[���h//
	
}