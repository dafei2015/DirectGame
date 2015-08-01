//�����һ��ͷ�ļ������û�ж��壬���ȶ���һ��ͷ�ļ�
//ͷ�ļ�����Ҫ��������Ӧ�ĺ����壬��Դ�ļ���ʵ�ֶ�Ӧ�ĺ�����
#ifndef _UTILITY_H
#define _UTILITY_H

#include <d3dx9.h>
#include <string>

namespace d3d
{
	bool IntD3D(HINSTANCE hInstance, int width, int height, bool windowed, D3DDEVTYPE deviceType, IDirect3DDevice9** device);
//	bool IntD3D(
//		HINSTANCE hInstance,		//���
//		int width,					//���ڿ��
//		int height,					//���ڸ߶�
//		bool windowed,				//�Ƿ񴰿ڻ�
//		D3DDEGREETYPE deviceType,	//�豸����
//		IDirect3DDevice9** device);   //�豸��ָ��

	//����һ������ָ�룬������Ϣѭ��
	int EnterMsgLoop(bool(*ptr_display)(float timeDelta));

	//����һ���ص�����,���ڴ����ڵĻص�
	LRESULT CALLBACK WndPro(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	//����һ���ͷ�ģ��
	template <class T> void Release(T t)
	{
		if (t)
		{
			t->Release();
			t = 0;
		}
	}

	template <class T> void Delete(T t)
	{
		if (t)
		{
			delete t;
			t = 0;
		}
	}

	//������ɫ���ƹ�Ͳ�������
	const D3DXCOLOR WHITE(D3DCOLOR_XRGB(255, 255, 255));
	const D3DXCOLOR BLACK(D3DCOLOR_XRGB(0, 0, 0));
	const D3DXCOLOR RED(D3DCOLOR_XRGB(255, 0, 0));
	const D3DXCOLOR GREEN(D3DCOLOR_XRGB(0, 255, 0));
	const D3DXCOLOR BLUE(D3DCOLOR_XRGB(0, 0, 255));
	const D3DXCOLOR YELLOW(D3DCOLOR_XRGB(255, 255, 0));
	const D3DXCOLOR CYAN(D3DCOLOR_XRGB(0, 255, 255));
	const D3DXCOLOR MAGENAT(D3DCOLOR_XRGB(255, 0, 255));

	D3DLIGHT9 InitDirectionalLight(D3DXVECTOR3* direction, D3DXCOLOR* color);
	D3DLIGHT9 InitPointLight(D3DXVECTOR3* position, D3DXCOLOR* color);
	D3DLIGHT9 InitSpotLight(D3DXVECTOR3* position, D3DXVECTOR3* direction, D3DXCOLOR* color);

	D3DMATERIAL9 InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p);

	const D3DMATERIAL9 WHITE_MTRL = InitMtrl(WHITE, WHITE, WHITE, BLACK, 2.0f);
	const D3DMATERIAL9 RED_MTRL = InitMtrl(RED, RED, RED, BLACK, 2.0f);
	const D3DMATERIAL9 GREEN_MTRL = InitMtrl(GREEN, GREEN, GREEN, BLACK, 2.0f);
	const D3DMATERIAL9 BLUE_MTRL = InitMtrl(BLUE, BLUE, BLUE, BLACK, 2.0f);

}
#endif