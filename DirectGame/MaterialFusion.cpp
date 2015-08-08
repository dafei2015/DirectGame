//�����ںϣ�ʵ�ֽ�һ�������һ��ͼƬ�����ںϣ����ð�����ͨ���ںϼ���
#include "Utility.h"

IDirect3DDevice9* device = nullptr; //ȫ�ֵ��豸ָ��

const int width = 640;
const int height = 480;

//����һ���ı��εĶ���ṹ
IDirect3DVertexBuffer9* BGQuad = 0;
//����һ������
IDirect3DTexture9* BGTex = 0;
//�ںϵ��ǽ����ʽ����ں�
D3DMATERIAL9 BGMtr;

//��Ҫ�ںϵ�mesh �Ͳ���
ID3DXMesh* teapot = 0;
D3DMATERIAL9 teapotMtr;

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
	}
	static const DWORD FVF;
};

//���ö���ĸ�ʽ.���û�з���������ʾ����
const DWORD Vertex::FVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_NORMAL;

//��ʼ����
bool SetUp()
{
	//����������ʺͱ�������
	teapotMtr = d3d::RED_MTRL;
	teapotMtr.Diffuse.a = 0.5f;
	BGMtr = d3d::WHITE_MTRL;

	//����������
	D3DXCreateTeapot(device, &teapot, 0);  //���

	//��������һ���ı��κ������������Σ�6������
	device->CreateVertexBuffer(6 * sizeof(Vertex), D3DUSAGE_WRITEONLY, Vertex::FVF, D3DPOOL_MANAGED, &BGQuad, 0);

	Vertex* v;
	BGQuad->Lock(0, 0, (void**)&v, 0);
	//����һ��������
	v[0] = Vertex(-10.0f, -10.0f, 5.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[1] = Vertex(-10.0f, 10.0f, 5.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[2] = Vertex(10.0f, 10.0f, 5.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

	//����ڶ���������
	v[3] = Vertex(-10.0f, -10.0f, 5.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[4] = Vertex(10.0f, 10.0f, 5.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[5] = Vertex(10.0f, -10.0f, 5.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	BGQuad->Unlock();

	//����һ�������
	D3DLIGHT9 dirlight;
	::ZeroMemory(&dirlight, sizeof(dirlight));
	dirlight.Type = D3DLIGHT_DIRECTIONAL;
	dirlight.Diffuse = d3d::WHITE;
	dirlight.Specular = d3d::WHITE * 0.2f;
	dirlight.Ambient = d3d::WHITE * 0.6f;
	dirlight.Direction = D3DXVECTOR3(0.707f, 0.0, 0.707f);

	//�����豸�Ĺ���
	device->SetLight(0, &dirlight);
	device->LightEnable(0, true);  //��������
	
	//������Ⱦ״̬
	device->SetRenderState(D3DRS_NORMALIZENORMALS, true);//���÷���
	device->SetRenderState(D3DRS_SPECULARENABLE, true); //���ø߹�


	//����ͼƬ
	D3DXCreateTextureFromFile(device, "test.jpg", &BGTex);
	device->SetTexture(0, BGTex);

	//���ò���ģʽΪ���Թ���
	device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

	//��������ɫ���
	device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	//���û������
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//���������
	D3DXVECTOR3 position(0.0f, 0.0f, -3.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);

	//����һ��������󣬽��������
	D3DXMATRIX view;
	D3DXMatrixLookAtLH(&view, &position, &target, &up);			//�趨�����λ��

	device->SetTransform(D3DTS_VIEW, &view);						//��������ӿھ�����
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
	d3d::Release<IDirect3DVertexBuffer9*>(BGQuad);
	d3d::Release<IDirect3DTexture9*>(BGTex);
	d3d::Release<ID3DXMesh*>(teapot);

}

bool Display(float timeDelta)
{
	//����豸���� ����գ��ڻ���
	if (device)
	{
		//ʹ�ü��̿���͸����
		if (::GetAsyncKeyState('A') & 0x8000f) //�жϼ��̵����뷽��
			teapotMtr.Diffuse.a += 0.0001f;
		if (::GetAsyncKeyState('D') & 0x8000f) //�жϼ��̵����뷽��
			teapotMtr.Diffuse.a -= 0.0001f;

		if (teapotMtr.Diffuse.a > 1.0f)
			teapotMtr.Diffuse.a = 1.0f;
		if (teapotMtr.Diffuse.a < 0.0f)
			teapotMtr.Diffuse.a = 0.0f;
		//��ʼ������
		device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xfffffff, 1.0f, 0);
		device->BeginScene();

		//������������������Ϸ��
		D3DXMATRIX w;
		D3DXMatrixIdentity(&w);
		device->SetTransform(D3DTS_WORLD, &w);
		device->SetFVF(Vertex::FVF);

		device->SetStreamSource(0, BGQuad, 0, sizeof(Vertex));
		device->SetMaterial(&BGMtr);
		device->SetTexture(0,BGTex);
		device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

		//�����
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		D3DXMatrixScaling(&w, 1.5f, 1.5f, 1.5f);//������Ŵ󣬲��ŵ����������
		device->SetTransform(D3DTS_WORLD, &w);
		device->SetMaterial(&teapotMtr);
		device->SetTexture(0, 0);
		teapot->DrawSubset(0);   //�����������Ӧ�ӿڻ��Ƹ�����

		device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
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
