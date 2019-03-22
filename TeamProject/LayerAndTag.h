#pragma once
class LayerAndTag
{
public:
	enum ObjectLayer 
	{
		Default,
		UI,
		MAX
	};


	enum Tags
	{
		Untagged,
		Player,
		Resources,
		ResourceParent,
		CaptureParent,
		Destructible,
		Occupied0,
		Occupied1,
		Occupied2,
		Occupied3,
		Enemy,
		EnemyProjectile,
		HammerHead,
		Ground
	};

	LayerAndTag()
	{

		ResetLayerMatrix();

		InitializeLayerMatrixCollisions();
	}

	

	~LayerAndTag(){}

	bool CanLayersCollide(ObjectLayer layer1, ObjectLayer layer2);
	


private:
	void ResetLayerMatrix();
	
	bool LayerMatrix[ObjectLayer::MAX][ObjectLayer::MAX];

	void InitializeLayerMatrixCollisions();
};