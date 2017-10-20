#pragma once

#include "ClassQueue.h"

//- �� Ŭ���� ���ο��� �ڽ� �������� ����, �ڽ� �������� ���ν���,
//������ ���� ��� ������.

//// �ڽ� ������ Ŭ���� ��� ////////////////////////////

#define UM_ALERT		WM_USER + 1
#define dfMAXCHILD		100
#define QUEUE_SIZE		30
#define TEXT_SIZE 32

// SendMessage(�θ��ڵ�, UM_ALERT...) �� ���ؼ� �θ𿡰� ��� �˸���
// �θ�� ��� �޽����� ������� ����� ���������� ĥ��.
// �����ð�(300~500 ms)���� �������� ���ƿ��� ���� ����.

class CMonitorGraphUnit
{
public:
	enum Color
	{
		RED,
		GREEN,
		BLUE,
		YELLOW,
		PINK
	};

	enum TYPE
	{
		BAR_SINGLE_VERT,
		BAR_COLUMN_VERT,
		LINE_SINGLE,
		LINE_MULTI,
		PIE
	};

	struct ST_HWNDtoTHIS
	{
		HWND				hWnd[dfMAXCHILD];
		CMonitorGraphUnit* pThis[dfMAXCHILD];
	};

	struct ST_COLUMN_INFO
	{
		ULONG64 u64ServerID;
		int iType;
		WCHAR szName[TEXT_SIZE];

		// ���⿡ Queue �� �߰��Ͽ��� ��.
	};

public:
	CMonitorGraphUnit(HINSTANCE hInstance, HWND hWndParent, Color color, TYPE enType,
		int iPosX, int iPosY, int iWidth, int iHeight, int iColumnNum = 1);
	~CMonitorGraphUnit();

	/////////////////////////////////////////////////////////
	// �׷��� ���� �Է�
	//
	// ������ �̸�, �ִ�ġ, �溸��ġ (0�̸� ����)
	/////////////////////////////////////////////////////////
	void	SetInformation(WCHAR *szTitle, int iDataMax, int iDataAlert);
	void	SetDataColumnInfo(int iColIndex, ULONG64 u64ServerID, int iType, WCHAR *szName = L"");

	/////////////////////////////////////////////////////////
	// ������ ���ν���
	/////////////////////////////////////////////////////////
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	/////////////////////////////////////////////////////////
	// ������ �ֱ�.
	/////////////////////////////////////////////////////////
	BOOL	InsertData(ULONG64 u64ServerID, int iType, int iData);
	void Init();
	void DrawInit();
	void WindowInit();

protected:
	/////////////////////////////////////////////////////////////
	// ����ġ �˶��߻�.  InsertData ���� ȣ��Ǵ� �Լ�
	/////////////////////////////////////////////////////////////
	BOOL	Alert(void);

	/////////////////////////////////////////////////////////////
	// Ÿ�Ժ� Paint �Լ�
	/////////////////////////////////////////////////////////////
	//void	Paint_Title(void);
	void	Paint_Grid(void);

	void	Paint_BarSingleVert(void);
	void	Paint_BarSingleHorz(void);
	void	Paint_BarColumnVert(void);
	void	Paint_BarColumnHorz(void);
	//
	void	Paint_LineSingle(void);
	//
	void	Paint_LineMulti(void);
	void	Paint_Pie(void);

	////////////////////////////////////////////////////////////
	// �޸�DC ȭ�� �����
	////////////////////////////////////////////////////////////
	void	ClearMemDC(void);

	/////////////////////////////////////////////////////////////
	// �޸�DC ȭ������ ��ȯ
	////////////////////////////////////////////////////////////
	void	FlipMemDC(HDC hDC);

	//------------------------------------------------------
	// ������ �ڵ�, this ������ ��Ī ���̺� ����.
	//------------------------------------------------------
	BOOL				PutThis(void);
	//static BOOL			RemoveThis(HWND hWnd);
	static CMonitorGraphUnit	*GetThis(HWND hWnd);

private:
	// ��Ƽ ����
	int		_iDataColumn; // �� �������� �÷� ����
	Queue<int>	*_DataArray;  // �������� �÷��� �ʿ��ϹǷ� ���� �迭 �Ҵ�
	ST_COLUMN_INFO	*_ColumArray; // �÷��� ������ Ÿ��

	//------------------------------------------------------
	// �θ� ������ �ڵ�, �� ������ �ڵ�, �ν��Ͻ� �ڵ�
	//------------------------------------------------------
	HWND _hWndParent;
	HWND _hWnd;
	HINSTANCE _hInstance;

	//------------------------------------------------------
	// ������ ��ġ,ũ��,����, �׷��� Ÿ�� ��.. �ڷ�
	//------------------------------------------------------
	TYPE _enGraphType;
	Color _backColor;

	HBRUSH _brushArr[5];
	HPEN _penArr[5];
	HFONT _font;

	WCHAR _szWindowClass[TEXT_SIZE];
	int _iWindowPosX;
	int _iWindowPosY;
	int _iWindowWidth;
	int _iWindowHeight;

	//------------------------------------------------------
	// ����â �̸�
	//------------------------------------------------------
	WCHAR _szTitle[TEXT_SIZE];

	//------------------------------------------------------
	// ������, �ְ�ġ, �溸 ��ġ
	//------------------------------------------------------
	int		_iDataMax;	// �׷��� �ƽ� ��
	int		_iDataAlert;	// �溸 �︮�� ���ذ�

	Queue<int>* _dataQ;

	HDC _hdc;
	HDC _memDC;
	HBITMAP _bitmap;
	HBITMAP _oldBitmap;
	RECT _rect;

	// static �ɹ� �Լ��� ���ν������� This �����͸� ã�� ����
	// HWND + Class Ptr �� ���̺�
	static ST_HWNDtoTHIS _childInfoTable[dfMAXCHILD];

	//------------------------------------------------------
	// ����� -
	//
	// ������ ����ġ ���޽� �θ� �����쿡 UM_ALERT �޽����� ������.
	//
	// _bAlertMode = TRUE �� �� �� Ÿ�̸Ӹ� ����Ͽ� �����ð� �� ����.
	//------------------------------------------------------
	BOOL _bAlertMode;
};