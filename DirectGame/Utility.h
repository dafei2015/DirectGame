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
}
#endif