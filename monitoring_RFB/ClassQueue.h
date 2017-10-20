#pragma once

#include "stdafx.h"

template <typename T>
class Queue
{
private:
	T *buffer;
	const int size;
	int front;
	int rear;
	int count = 0;

public:
	Queue(int size = 30) :size(size)
	{
		buffer = new T[size];
		front = rear = 0;
	}
	~Queue()
	{
		Clear();
		delete[] buffer;
	}

	void Clear()
	{
		T temp = 0; // 실제로 지우진 않음. 지우는건 알아서 따로 구현해야
		for (int i = 0; i <= count; i++)
		{
			//T* temp = nullptr;
			//Get(temp);
			Get(&temp);
			temp = 0;
		}
	}

	bool Put(T data) // 풋 Enqueue
	{
		if (IsFull())
		{
			return false;
		}
		buffer[rear] = data;
		rear = Next(rear);
		count++;
		return true;
	}

	bool Get(T* param) // 겟 Dequeue
	{
		if (IsEmpty())
		{
			return false;
		}
		*param = buffer[front];
		buffer[front] = 0; // test
		front = Next(front);
		count--;

		return true;
	}
	bool IsFull() // 꽉참?
	{
		return Next(rear) == front;
	}
	bool IsEmpty() // 빔?
	{
		return front == rear;
	}

	bool Peek(T* param, int peekIndex) // 픽
	{
		if (IsEmpty())
		{
			return false;
		}

		*param = buffer[(front + peekIndex) % size];

		return true;
	}

	int Count(void) // 몇개들어 있냐?
	{
		return count;
	}

private:
	int Next(int now) // 다음 인덱스
	{
		return (now + 1) % size;
	}
};