#include "LayerAndTag.h"

bool LayerAndTag::CanLayersCollide(ObjectLayer layer1, ObjectLayer layer2)
{
	return LayerMatrix[layer1][layer2];
}

void LayerAndTag::ResetLayerMatrix()
{
	
	for (int i = 0; i < ObjectLayer::MAX; i++)
	{
		for (int j = 0; j < ObjectLayer::MAX; j++)
		{
			LayerMatrix[i][j] = false;
		}
	}
	
}

void LayerAndTag::InitializeLayerMatrixCollisions()
{
	LayerMatrix[ObjectLayer::Default][ObjectLayer::Default] = true;
	//Becareful about collision indexing
}
