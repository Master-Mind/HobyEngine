#pragma once
class DebugWindow
{
public:
	DebugWindow();
	DebugWindow(long long flags);
	bool Display(int num);
private:
	long long _flags;
};