//Դ�ļ�ʵ�ֶ�Ӧ�ĺ����� 0��ָ��
#include "Utility.h"

bool d3d::IntD3D(HINSTANCE hinstance, int width, int height, bool windowed, D3DDEVTYPE deviceType, IDirect3DDevice9** device)
{
	WNDCLASS wc;    //���ڵĽṹ�壬Ȼ��ʵ����Ӧ���ֶ�
	static TCHAR szAppName[] = TEXT("Direct3DApp");				//�°汾����textת��һ��
	wc.style = CS_HREDRAW | CS_VREDRAW;							//���ô��ڵĸ�ʽ
	wc.lpfnWndProc = d3d::WndPro;								//���ô��ڵĻص�
	wc.cbClsExtra = 0;											//
	wc.cbWndExtra = 0;
	wc.hInstance = hinstance;
	wc.hCursor = LoadCursor(0,IDC_ARROW);					//���
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);				//ͼ��
	wc.hbrBackground = HBRUSH(GetStockObject(WHITE_BRUSH));	//�û�ˢ������ˢ�ɰ�ɫ
	wc.lpszMenuName = 0;									//�˵�����				
	wc.lpszClassName = szAppName;								//��������

	if (!RegisterClass(&wc))									//����ע��
	{
		//���ע�᲻�ɹ�����ʾ��ʾ��
		::MessageBox(0, TEXT("RegisterClass() -- Failed"), 0, 0);
		return false;
	}

	//��������
	HWND hwnd = 0;
	hwnd = ::CreateWindow(szAppName, TEXT("Direct3DApp"), WS_EX_TOPMOST, 0, 0, width, height, 0, 0, hinstance, 0);
	if (!hwnd)
	{
		::MessageBox(0, TEXT("CreateWindow() -- Failed"), 0, 0);
		return false;
	}

	::ShowWindow(hwnd, SW_SHOW);								//��ʾ����
	::UpdateWindow(hwnd);										//���´���

	//����Direct�豸
	HRESULT hr = 0;
	IDirect3D9* d3d9 = 0;									//����һ��ָ��
	d3d9 = Direct3DCreate9(D3D_SDK_VERSION);					//��ȡD3D�İ汾

	if (!d3d9)
	{
		::MessageBox(0, TEXT("Direct3DCreate9() -- Failed"), 0, 0);
		return false;
	}

	//��ȡ�豸����
	D3DCAPS9 caps;
	d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, deviceType, &caps);
	
	//�ж�����������ƻ���Ӳ������
	int vp = 0;													//�洢���Ƶ���Ϣ
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)			//�ж��豸�Ƿ�֧��Ӳ������
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	//���һ���ṹ�壬���ں�̨����
	D3DPRESENT_PARAMETERS d3dpp;
	d3dpp.BackBufferWidth = width;
	d3dpp.BackBufferHeight = height;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.hDeviceWindow = hwnd;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.Flags = 0;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	//�����豸
	hr = d3d9->CreateDevice(D3DADAPTER_DEFAULT, deviceType, hwnd, vp, &d3dpp, device);

	//�������ʧ�ܣ����õͰ汾���豸
	if (FAILED(hr))
	{
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;   //����ȸ�ʽ��ΪD16;
		hr = d3d9->CreateDevice(D3DADAPTER_DEFAULT, deviceType, hwnd, vp, &d3dpp, device);
		if (FAILED(hr))
		{
			//˵���Կ�̫��ͷŵ�
			d3d9->Release();
			::MessageBox(0, TEXT("DirectDirect() -- Failed"), 0, 0);
			return false;
		}
		d3d9->Release();

	}

	return true;
}

//������Ϣѭ��
int d3d::EnterMsgLoop(bool(*ptr_display)(float timeDelta))
{
	MSG msg;

	//����һ���ڴ�
	::ZeroMemory(&msg, sizeof(MSG));
	static float lastTime = (float)timeGetTime();   //��õ�ǰʱ��

	//������Ϣ
	while (msg.message != WM_QUIT)
	{
		if (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);   //ת����Ϣ
			::DispatchMessage(&msg);    //������Ϣ

		}
		else
		{
			float currentTime = (float)timeGetTime();
			float timeDelta = (currentTime - lastTime) * 0.001f;
			ptr_display(timeDelta);
			lastTime = currentTime;
		}
	}
	return msg.wParam;
}

