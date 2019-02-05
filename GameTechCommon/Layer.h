#pragma once
class Layer
{
public:
	static enum ObjectLayer {
		Default,
		UI,
		MAX
	};

	Layer()
	{
		LayerMatrix[ObjectLayer::MAX][ObjectLayer::MAX] = { 0 };
		InitializeLayerMatrixCollisions();
	}

	bool CanLayersCollide(ObjectLayer layer1, ObjectLayer layer2);
	


private:

	bool LayerMatrix[ObjectLayer::MAX][ObjectLayer::MAX];

	void InitializeLayerMatrixCollisions();
};