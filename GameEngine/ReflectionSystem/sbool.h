#pragma once

class sbool
{
public:
	sbool() : _initialized(false){}
	bool Initialized() const { return _initialized; }
	void SetInitialized(bool init) { _initialized = init; }
private:
	bool _initialized;
};