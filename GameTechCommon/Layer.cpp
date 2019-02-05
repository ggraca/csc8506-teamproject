#include "Layer.h"

bool Layer::CanLayersCollide(ObjectLayer layer1, ObjectLayer layer2)
{
	return LayerMatrix[layer1][layer2];
}

void Layer::InitializeLayerMatrixCollisions()
{
	LayerMatrix[ObjectLayer::Default][ObjectLayer::Default] = true;
	//Becareful about collision indexing
}
