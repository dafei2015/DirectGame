//��ʼ��һ�������
#include "Utility.h"

IDirect3DDevice9* device = nullptr; //ȫ�ֵ��豸ָ��

const int width = 640;
const int height = 480;


ID3DXMesh* objects[4] = { 0, 0, 0, 0 };	//ģ��
D3DXMATRIX worlds[4];					//����
D3DMATERIAL9 mtrls[4];					//����


//��ʼ����
bool SetUp()
{
	//��������
	D3DXCreateTeapot(device, &objects[0], 0);  //���
	D3DXCreateSphere(device, 1.0f, 20, 20, &objects[1], 0);
	D3DXCreateTorus(device, 0.5f, 1.0f, 20, 20, &objects[2], 0);
	D3DXCreateCylinder(device, 0.5f, 0.5f, 2.0f, 20, 20, &objects[3], 0);

	//ȡ���������Ȼ�󽫶���Ž�ȥ,ֻ������������вŻ���ʾ
	D3DXMatrixTranslation(&worlds[0], 0.0f, 2.0f, 0.0f);
	D3DXMatrixTranslation(&worlds[1], 0.0f, -2.0f, 0.0f); 
	D3DXMatrixTranslation(&worlds[2], -3.0f, 0.0f, 0.0f);
	D3DXMatrixTranslation(&worlds[3], 3.0f, 2.0f, 0.0f);

	//���ö���Ĳ���
	mtrls[0] = d3d::RED_MTRL;
	mtrls[1] = d3d::BLUE_MTRL;
	mtrls[2] = d3d::GREEN_MTRL;
	mtrls[3] = d3d::RED_MTRL;

	//���÷����
	D3DXVECTOR3 dir(1.0f, -0.0f, 0.25f);
	D3DXCOLOR c = d3d::WHITE;
	D3DLIGHT9 dirlight = d3d::InitDirectionalLight(&dir, &c);

	//�����豸�Ĺ���
	device->SetLight(0, &dirlight);
	device->LightEnable(0, true);  //��������


	//������Ⱦ,��������
	device->SetRenderState(D3DRS_NORMALIZENORMALS, true); //��������
	device->SetRenderState(D3DRS_SPECULARENABLE, false);//�رո߹�

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
	for (int i = 0; i < 4; i++)
	{
		d3d::Release<ID3DXMesh*>(objects[i]);
	}
}

bool Display(float timeDelta)
{
	//����豸���� ����գ��ڻ���
	if (device)
	{
		//ͨ���������ת��ʾ������
		static float angle = (3.0f*D3DX_PI) / 2.0f;  //����һ����ת�Ƕ�
		static float height = 5.0f;

		if (::GetAsyncKeyState(VK_LEFT) & 0x8000f) //�жϼ��̵����뷽��
			angle -= 0.5f * timeDelta;
		if (::GetAsyncKeyState(VK_RIGHT) & 0x8000f) //�жϼ��̵����뷽��
			angle += 0.5f * timeDelta;
		if (::GetAsyncKeyState(VK_UP) & 0x8000f) //�жϼ��̵����뷽��
			height -= 0.5f * timeDelta;
		if (::GetAsyncKeyState(VK_DOWN) & 0x8000f) //�жϼ��̵����뷽��
			height += 0.5f * timeDelta;

		//������ƶ�
		D3DXVECTOR3 position(cosf(angle)*7.0f, height, sinf(angle)*7.0f);
		D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);

		//����һ��������󣬽��������
		D3DXMATRIX v;
		D3DXMatrixLookAtLH(&v, &position, &target, &up);  //����Ϊ��������ϵ
		device->SetTransform(D3DTS_VIEW, &v);                     //��������ӿھ�����

		//��ʼ������
		device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x0000000, 1.0f, 0);
		device->BeginScene();
		
		for (int i = 0; i < 4; i++)
		{
			device->SetMaterial(&mtrls[i]);
			device->SetTransform(D3DTS_WORLD, &worlds[i]);
			objects[i]->DrawSubset(0);
		}
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
