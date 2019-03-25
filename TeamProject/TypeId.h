#pragma once

class TypeId
{
public:
#ifdef _WIN32
	static int GetTypeId(const type_info &info);
#endif
#ifdef __ORBIS__
	static int GetTypeId(const std::type_info &info);
#endif

};


