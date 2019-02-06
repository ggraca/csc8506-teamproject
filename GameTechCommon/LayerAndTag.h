#pragma once
class LayerAndTag
{
public:
	static enum ObjectLayer 
	{
		Default,
		UI,
		MAX
	};


	static enum Tags
	{
		Untagged,
		Player
	};

	LayerAndTag()
	{
		LayerMatrix[ObjectLayer::MAX][ObjectLayer::MAX] = { 0 };
		InitializeLayerMatrixCollisions();
	}

	bool CanLayersCollide(ObjectLayer layer1, ObjectLayer layer2);
	


private:

	bool LayerMatrix[ObjectLayer::MAX][ObjectLayer::MAX];

	void InitializeLayerMatrixCollisions();
};