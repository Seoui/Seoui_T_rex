#include "stdafx.h"
#include "Keyboard.h"

Keyboard::Keyboard()
{
	for (int i = 0; i < KEYMAX; i++)
	{
		up.set(i, false);
		down.set(i, false);
	}
}

// up.set(i) : i�� ���� ���� (true or false)
// up.reset() : �ε����� ���� false�� ����
// up.flip() : ��Ʈ���� �ݴ�� ��ȯ������(false->true, true->false)
// up.all() : ��� ������ ���� true�̸� true��ȯ    and����
// up.none() : ��� ������ ���� false�̸� true��ȯ     and����
// up.any() : �� ���� ���Ҷ�true�̸� true         or����
// up.count() : true�� ������ ����

Keyboard::~Keyboard()
{

}

bool Keyboard::Down(int key)
{
	if (GetAsyncKeyState(key) & 0x8000)
	{
		if (down[key] == false)
		{
			down.set(key, true);
			return true;
		}
	}
	else
	{
		down.set(key, false);
	}

	return false;
}

bool Keyboard::Up(int key)
{
	if (GetAsyncKeyState(key) & 0x8000)
	{
		up.set(key, true);
	}
	else
	{
		if (up[key] == true)
		{
			up.set(key, false);
			return true;
		}
	}

	return false;
}

bool Keyboard::Press(int key)
{
	if (GetAsyncKeyState(key) & 0x8000)
		return true;
	return false;
}

bool Keyboard::Toggle(int key)
{
	if (GetAsyncKeyState(key) & 0x0001) // toggle �Ǵ��ϴ� ����ũ ��
		return true;

	return false;
}
