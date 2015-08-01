//定义给一个头文件，如果没有定义，则先定义一个头文件
//头文件中主要是声明相应的函数体，在源文件中实现对应的函数体
#ifndef _UTILITY_H
#define _UTILITY_H

#include <d3dx9.h>
#include <string>

namespace d3d
{
	bool IntD3D(HINSTANCE hInstance, int width, int height, bool windowed, D3DDEVTYPE deviceType, IDirect3DDevice9** device);
//	bool IntD3D(
//		HINSTANCE hInstance,		//句柄
//		int width,					//窗口宽度
//		int height,					//窗口高度
//		bool windowed,				//是否窗口化
//		D3DDEGREETYPE deviceType,	//设备类型
//		IDirect3DDevice9** device);   //设备的指针

	//声明一个函数指针，进行消息循环
	int EnterMsgLoop(bool(*ptr_display)(float timeDelta));

	//定义一个回调函数,用于处理窗口的回调
	LRESULT CALLBACK WndPro(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	//定义一个释放模板
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

	//加入颜色，灯光和材质声明
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