#include "stdafx.h"
#include "ChildMonitor.h"

/*
static ���̺�
static�� ������ �ݹ��Լ���?
*/

CMonitorGraphUnit::CMonitorGraphUnit(HINSTANCE hInstance, HWND hWndParent, Color color,
	TYPE enType, int iPosX, int iPosY, int iWidth, int iHeight, int iColumnNum)
{
	// ���� ��� �ʱ�ȭ
	//_dataQ = new Queue<int>(QUEUE_SIZE);
	_iDataColumn = iColumnNum;
	_ColumArray = new ST_COLUMN_INFO[_iDataColumn];
	_dataQ = new Queue<int>[_iDataColumn];
	_backColor = color;
	_enGraphType = enType;

	static int titleNum = 0;
	WCHAR titleText[20];
	wsprintf(titleText, L"childTitle%d", titleNum);
	++titleNum;

	wcscpy_s(_szWindowClass, sizeof(_szWindowClass) / 2, titleText);
	_iWindowPosX = iPosX;
	_iWindowPosY = iPosY;
	_iWindowWidth = iWidth;
	_iWindowHeight = iHeight;
	_hWndParent = hWndParent;
	_hInstance = hInstance;

	// ������ ���
	WNDCLASSEXW wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = NULL;
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = _szWindowClass;
	wcex.hIconSm = NULL;

	RegisterClassEx(&wcex);

	// �׸��� �ʱ�ȭ, ������ ����
	Init();

	// ��ü ������ this �����Ϳ� hWnd ����
	PutThis();
}

CMonitorGraphUnit::~CMonitorGraphUnit()
{
}

void CMonitorGraphUnit::SetInformation(WCHAR * szTitle, int iDataMax, int iDataAlert)
{
	wcscpy_s(this->_szTitle, sizeof(this->_szTitle) / 2, szTitle);
	SetWindowText(this->_hWnd, this->_szTitle);

	_iDataMax = iDataMax;

	if (iDataAlert == 0)
	{
		_iDataAlert = INT_MAX;
	}
	else
	{
		_iDataAlert = iDataAlert;
	}
}

void CMonitorGraphUnit::SetDataColumnInfo(int iColIndex, ULONG64 u64ServerID, int iType, WCHAR * szName)
{
	this->_ColumArray[iColIndex].u64ServerID = u64ServerID;
	this->_ColumArray[iColIndex].iType = iType; // ������ ���� �ĺ� ��ȣ?
	wcscpy_s(this->_ColumArray[iColIndex].szName, TEXT_SIZE / 2, szName);
}

LRESULT CMonitorGraphUnit::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_TIMER:
	{
		CMonitorGraphUnit* pThis = CMonitorGraphUnit::GetThis(hWnd);
		if (pThis == nullptr)
		{
			wcout << L"getThis nullptr : " << GetLastError() << endl;

			return NULL;
		}

		KillTimer(pThis->_hWnd, 1);
		pThis->_bAlertMode = false;

		HPEN blackPen = (HPEN)GetStockObject(BLACK_PEN);
		SelectObject(pThis->_memDC, blackPen);
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: ���⿡ hdc�� ����ϴ� �׸��� �ڵ带 �߰��մϴ�.

		CMonitorGraphUnit* pThis = CMonitorGraphUnit::GetThis(hWnd);
		if (pThis == nullptr)
		{
			wcout << L"getThis nullptr : " << GetLastError() << endl;

			return NULL;
		}

		//SetROP2(pThis->_memDC, R2_XORPEN);
		//SetROP2(pThis->_memDC, R2_COPYPEN);

		switch (pThis->_enGraphType)
		{
		case LINE_SINGLE:
			pThis->Paint_LineSingle();
			break;

		case LINE_MULTI:
			pThis->Paint_LineMulti();
			break;
		}

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

BOOL CMonitorGraphUnit::InsertData(ULONG64 u64ServerID, int iType, int iData)
{
	for (int j = 0; j < this->_iDataColumn; j++)
	{
		if (this->_ColumArray[j].u64ServerID != u64ServerID)
		{
			continue;
		}

		if (this->_ColumArray[j].iType != iType)
		{
			continue;
		}

		if (this->_dataQ[j].IsFull())
		{
			int getValue = 0;
			this->_dataQ[j].Get(&getValue);
			//wcout << getValue << L"is getOut" << endl;
		}

		this->_dataQ[j].Put(iData);
		//wcout << iData << L"is put" << endl;

		// �ش� ������ InvalidateRect
		InvalidateRect(this->_hWnd, NULL, TRUE);

		// iData�� _iDataAlert ���� ũ��
		if (iData > this->_iDataAlert)
		{
			Alert();
			SetTimer(this->_hWnd, 1, 200, NULL);
			this->_bAlertMode = true;
		}
	}
	return TRUE;
}

void CMonitorGraphUnit::Init()
{
	WindowInit();
	DrawInit();
}

void CMonitorGraphUnit::DrawInit()
{
	// TODO : ���ҽ� ������ ���߿� ����
	// �귯��, �� ����
	_brushArr[Color::RED] = CreateSolidBrush(RGB(255, 0, 0));
	_brushArr[Color::GREEN] = CreateSolidBrush(RGB(0, 255, 0));
	_brushArr[Color::BLUE] = CreateSolidBrush(RGB(0, 0, 255));
	_brushArr[Color::YELLOW] = CreateSolidBrush(RGB(255, 255, 0));
	_brushArr[Color::PINK] = CreateSolidBrush(RGB(255, 102, 204));

	_penArr[Color::RED] = CreatePen(BS_SOLID, 1, RGB(255, 0, 0));
	_penArr[Color::GREEN] = CreatePen(BS_SOLID, 1, RGB(0, 255, 0));
	_penArr[Color::BLUE] = CreatePen(BS_SOLID, 1, RGB(0, 0, 255));
	_penArr[Color::YELLOW] = CreatePen(BS_SOLID, 1, RGB(255, 255, 0));
	_penArr[Color::PINK] = CreatePen(BS_SOLID, 1, RGB(255, 102, 204));

	// ��Ʈ
	_font = CreateFont(10, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_SWISS, L"���ü");

	// ���� �׷��� �׸���
	srand((unsigned int)time(NULL));
	SetTimer(_hWnd, 1, 150, NULL);

	// ���� ���۸� �غ�
	GetClientRect(_hWnd, &_rect);
	_hdc = GetDC(_hWnd);
	_memDC = CreateCompatibleDC(_hdc);

	_bitmap = CreateCompatibleBitmap(_hdc, _rect.right, _rect.bottom);
	_oldBitmap = (HBITMAP)SelectObject(_memDC, _bitmap);
}

void CMonitorGraphUnit::WindowInit()
{
	// ������ ����
	_hWnd = CreateWindow(_szWindowClass, _szWindowClass, WS_CAPTION | WS_CHILD |
		WS_VISIBLE | WS_CLIPSIBLINGS, _iWindowPosX, _iWindowPosY, _iWindowWidth,
		_iWindowHeight, _hWndParent, nullptr, _hInstance, nullptr);
	if (_hWnd != NULL)
	{
		wcout << L"Child window created : " << _szWindowClass << endl;
	}
	else
	{
		wcout << L"Child window create error : " << GetLastError() << endl;
		return;
	}
}

BOOL CMonitorGraphUnit::Alert(void)
{
	if (this->_bAlertMode)
	{
		// �� �˶��� �︰ �����̸� return FALSE
		return FALSE;
	}

	SendMessage(this->_hWndParent, UM_ALERT, NULL, NULL);

	SelectObject(this->_memDC, this->_penArr[Color::RED]);

	return TRUE;
}

void CMonitorGraphUnit::Paint_Grid(void)
{
	const int gridOffset = static_cast<int>(static_cast<float>(this->_rect.bottom) / 4);

	for (int i = 0; i < 3; i++)
	{
		MoveToEx(this->_memDC, 0, gridOffset + gridOffset * i, NULL);
		LineTo(this->_memDC, this->_rect.right, gridOffset + gridOffset * i);
	}

	SetBkMode(this->_memDC, TRANSPARENT);
	HFONT oldFont = (HFONT)SelectObject(this->_memDC, this->_font);
	int gridTextNum = this->_iDataMax;

	for (int i = 0; i < 4; i++)
	{
		WCHAR gridText[5];
		_itow_s(gridTextNum, gridText, 5, 10);
		TextOut(this->_memDC, 5, i * gridOffset + 1, gridText, 3);
		gridTextNum -= (this->_iDataMax / 4);
	}
	SelectObject(this->_memDC, oldFont);
}

BOOL CMonitorGraphUnit::PutThis(void)
{
	for (int i = 0; i < dfMAXCHILD; i++)
	{
		if (_childInfoTable->hWnd[i] != NULL)
		{
			continue;
		}

		_childInfoTable->hWnd[i] = _hWnd;
		_childInfoTable->pThis[i] = this;

		return TRUE;
	}
	return FALSE;
}

CMonitorGraphUnit * CMonitorGraphUnit::GetThis(HWND hWnd)
{
	for (int i = 0; i < dfMAXCHILD; i++)
	{
		if (_childInfoTable->hWnd[i] != hWnd)
		{
			continue;
		}
		return _childInfoTable->pThis[i];
	}
	return nullptr;
}

CMonitorGraphUnit::ST_HWNDtoTHIS CMonitorGraphUnit::_childInfoTable[dfMAXCHILD];

void CMonitorGraphUnit::ClearMemDC(void)
{
	// ����
	FillRect(this->_memDC, &this->_rect, this->_brushArr[this->_backColor]);
}

void CMonitorGraphUnit::FlipMemDC(HDC hDC)
{
	BitBlt(hDC, 0, 0,
		this->_rect.right, this->_rect.bottom,
		this->_memDC, 0, 0,
		SRCCOPY);
}

void CMonitorGraphUnit::Paint_LineSingle(void)
{
	// ����
	this->ClearMemDC();

	// �׸���
	this->Paint_Grid();

	// �� �׸���
	int firstValue = 0;
	this->_dataQ->Peek(&firstValue, 0);

	float fixedValue = static_cast<float>(firstValue) / this->_iDataMax * this->_rect.bottom;

	if (!this->_dataQ->IsEmpty())
	{
		MoveToEx(this->_memDC, 0, static_cast<int>(this->_rect.bottom - fixedValue), NULL);
	}

	for (int i = 1; i < this->_dataQ->Count(); i++)
	{
		int peekValue = 0;
		this->_dataQ->Peek(&peekValue, i);

		fixedValue = static_cast<float>(peekValue) / this->_iDataMax * this->_rect.bottom;

		LineTo(this->_memDC,
			static_cast<int>(i * (static_cast<float>(this->_rect.right) / QUEUE_SIZE)),
			static_cast<int>(this->_rect.bottom - fixedValue));
	}

	// �ø�
	this->FlipMemDC(this->_hdc);
}

void CMonitorGraphUnit::Paint_LineMulti(void)
{
	// ����
	this->ClearMemDC();

	// �׸���
	this->Paint_Grid();

	for (int i = 0; i < this->_iDataColumn; i++)
	{
		HPEN localOldPen = (HPEN)SelectObject(this->_memDC, this->_penArr[i]);

		// �� �׸���
		int firstValue = 0;
		this->_dataQ[i].Peek(&firstValue, 0);

		float fixedValue = static_cast<float>(firstValue) / this->_iDataMax * this->_rect.bottom;

		if (!this->_dataQ[i].IsEmpty())
		{
			MoveToEx(this->_memDC, 0, static_cast<int>(this->_rect.bottom - fixedValue), NULL);
		}

		for (int j = 1; j < this->_dataQ[i].Count(); j++)
		{
			int peekValue = 0;
			this->_dataQ[i].Peek(&peekValue, j);

			fixedValue = static_cast<float>(peekValue) / this->_iDataMax * this->_rect.bottom;

			LineTo(this->_memDC,
				static_cast<int>(j * (static_cast<float>(this->_rect.right) / QUEUE_SIZE)),
				static_cast<int>(this->_rect.bottom - fixedValue));
		}

		SelectObject(this->_memDC, localOldPen);
	}

	// �ø�
	this->FlipMemDC(this->_hdc);
}