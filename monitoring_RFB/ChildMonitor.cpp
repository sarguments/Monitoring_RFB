#include "stdafx.h"
#include "ChildMonitor.h"

/*
static 테이블
static인 이유가 콜백함수라서?
*/

CMonitorGraphUnit::CMonitorGraphUnit(HINSTANCE hInstance, HWND hWndParent, Color color, TYPE enType, int iPosX, int iPosY, int iWidth, int iHeight)
{
	// 각종 멤버 초기화
	_dataQ = new Queue<int>(100);
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

	// 윈도우 등록
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

	// 그리기 초기화, 윈도우 생성
	Init();

	// 객체 생성시 this 포인터와 hWnd 저장
	PutThis();
}

CMonitorGraphUnit::~CMonitorGraphUnit()
{
}

LRESULT CMonitorGraphUnit::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다.

		CMonitorGraphUnit* pThis = CMonitorGraphUnit::GetThis(hWnd);
		if (pThis == nullptr)
		{
			wcout << L"getThis nullptr : " << GetLastError() << endl;

			return NULL;
		}
		
		FillRect(pThis->_memDC, &pThis->_rect, pThis->_brushArr[pThis->_backColor]);

		int firstValue = 0;
		pThis->_dataQ->Peek(&firstValue, 0);

		if (!pThis->_dataQ->IsEmpty())
		{
			MoveToEx(pThis->_memDC, 0, firstValue, NULL);
		}

		for (int i = 1; i < pThis->_dataQ->Count(); i++)
		{
			int peekValue = 0;
			pThis->_dataQ->Peek(&peekValue, i);

			LineTo(pThis->_memDC, static_cast<int>(i * (static_cast<float>(pThis->_rect.right) / 100)), peekValue);
		}

		BitBlt(hdc, 0, 0,
			pThis->_rect.right, pThis->_rect.bottom, 
			pThis->_memDC, 0, 0, 
			SRCCOPY);

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

BOOL CMonitorGraphUnit::InsertData(int iData)
{
	if (this == nullptr)
	{
		wcout << L"InsertData nullptr" << endl;

		return FALSE;
	}

	if (this->_dataQ->IsFull())
	{
		int getValue = 0;
		this->_dataQ->Get(&getValue);
		wcout << getValue << L"is getOut" << endl;
	}

	this->_dataQ->Put(iData);
	wcout << iData << L"is put" << endl;

	InvalidateRect(this->_hWnd, NULL, TRUE);

	return TRUE;
}

void CMonitorGraphUnit::Init()
{
	WindowInit();
	DrawInit();
}

void CMonitorGraphUnit::DrawInit()
{
	// TODO : 리소스 해제는 나중에 하자
	// 브러시, 펜 생성
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

	// 랜덤 그래프 그리기
	srand((unsigned int)time(NULL));
	SetTimer(_hWnd, 1, 150, NULL);

	// 더블 버퍼링 준비
	GetClientRect(_hWnd, &_rect);
	HDC hdc = GetDC(_hWnd);
	_memDC = CreateCompatibleDC(hdc);

	_bitmap = CreateCompatibleBitmap(hdc, _rect.right, _rect.bottom);
	_oldBitmap = (HBITMAP)SelectObject(_memDC, _bitmap);
}

void CMonitorGraphUnit::WindowInit()
{
	// 윈도우 생성
	_hWnd = CreateWindow(_szWindowClass, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
		_iWindowPosX, _iWindowPosY, _iWindowWidth, _iWindowHeight, _hWndParent, NULL, _hInstance, NULL);

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