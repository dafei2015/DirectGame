//��ʼ��һ����������ı���
#include "Utility.h"

IDirect3DDevice9* device = nullptr; //ȫ�ֵ��豸ָ��

const int width = 640;
const int height = 480;

//����һ���ı��εĶ���ṹ
IDirect3DVertexBuffer9* quad = 0;
//����һ������
IDirect3DTexture9* tex = 0;

//����һ���ṹ��
struct Vertex
{
	//��ά��
	float _x, _y, _z;

	//����
	float _nx, _ny, _nz;

	//UV����
	float _u, _v;

	Vertex()
	{
	};


	Vertex(float x, float y, float z, float nx, float ny, float nz, float u, float v)
		: _x(x),
		  _y(y),
		  _z(z),
		  _nx(nx),
		  _ny(ny),
		  _nz(nz),
		  _u(u),
		  _v(v)
	{
		_x = x; _y = y; _z = z;
		_nx = nx; _ny = ny; _nz = nz;
		_u = u; _v = v;
	}

	static const DWORD FVF;
};

//���ö���ĸ�ʽ.���û�з���������ʾ����
const DWORD Vertex::FVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_NORMAL;

//��ʼ����
bool SetUp()
{
	//��������һ���ı��κ������������Σ�6������
	device->CreateVertexBuffer(6 * sizeof(Vertex), D3DUSAGE_WRITEONLY, Vertex::FVF, D3DPOOL_MANAGED, &quad, 0);

	Vertex* v;
	quad->Lock(0, 0, (void**)&v, 0);
	//����һ��������
	v[0] = Vertex(-1.0f, -1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[1] = Vertex(-1.0f, 1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[2] = Vertex(1.0f, 1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

	//����ڶ���������
	v[3] = Vertex(-1.0f, -1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[4] = Vertex(1.0f, 1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[5] = Vertex(1.0f, -1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	quad->Unlock();

	//����ͼƬ
	D3DXCreateTextureFromFile(device, "test.jpg", &tex);
	device->SetTexture(0, tex);

	//���ò���ģʽΪ���Թ���
	device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

	//������Ⱦ,���ƹ�ص�
	device->SetRenderState(D3DRS_LIGHTING, false);
	//����ͶӰ����
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI * 0.5f, (float)width / (float)height, 1.0f, 1000.0f);

	//����ͶӰ����Ļ��
	device->SetTransform(D3DTS_PROJECTION, &proj);

	
	return true;
}

//�ͷ�
void Cleanup()
{
	d3d::Release<IDirect3DVertexBuffer9*>(quad);
	d3d::Release<IDirect3DTexture9*>(tex);
}

bool Display(float timeDelta)
{
	//����豸���� ����գ��ڻ���
	if (device)
	{
		//��ʼ������
		device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xfffffff, 1.0f, 0);
		device->BeginScene();

		//�������ļ�
		device->SetStreamSource(0, quad, 0, sizeof(Vertex));
		//���ö����ʽ
		device->SetFVF(Vertex::FVF);
		//��������
		device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

		device->EndScene();
		device->Present(0, 0, 0, 0);
	}
	return true;
}

//���ڵĻص�
LRESULT CALLBACK d3d::WndPro(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		::PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			::DestroyWindow(hwnd);
		break;
	}

	return ::DefWindowProcA(hwnd, msg, wParam, lParam);
}

//������
int WINAPI WinMain(__in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd)
{
	if (!d3d::IntD3D(hInstance, 640, 480, true, D3DDEVTYPE_HAL, &device))
	{
		::MessageBox(0, TEXT("InitD3D()-- failed"), 0, 0);
		return 0;
	}

	if (!SetUp())
	{
		::MessageBox(0, TEXT("SetUp()-- failed"), 0, 0);
		return 0;
	}
	d3d::EnterMsgLoop(Display);
	Cleanup();
	device->Release();
	return 0;
}
