#include "LayerAndTag.h"

bool LayerAndTag::CanLayersCollide(ObjectLayer layer1, ObjectLayer layer2)
{
	return LayerMatrix[layer1][layer2];
}

void LayerAndTag::InitializeLayerMatrixCollisions()
{
	LayerMatrix[ObjectLayer::Default][ObjectLayer::Default] = true;
	//Becareful about collision indexing
}
