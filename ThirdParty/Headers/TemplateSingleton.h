#pragma once

template < typename T >
class TSingleton
{
protected:
	TSingleton() {}
	virtual ~TSingleton() {}

public:
	static T& Inst() {
		static T Instance;
		return Instance;
	}
};