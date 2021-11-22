#include "QState.h"

void QState::RunCommand(const char* command)
{
	_compiler.Compile(command);
}
