#pragma once
class ITask{
public:
	virtual int Execute(LPVOID lParam) = 0;
};
