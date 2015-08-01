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
}
#endif