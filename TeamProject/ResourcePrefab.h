#pragma once
#include "Resource.h"
#include "CubePrefab.h"

using namespace NCL;
using namespace CSC8503;

class ResourcePrefab : public CubePrefab, public Resource
{
public:
	ResourcePrefab();
	~ResourcePrefab();
};

