#pragma once
#include "QCompile.h"

class QState
{
public:
	void RunCommand(const char *command);
private:
	QCompile _compiler;
};