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
		Player,
		Resources,
		ResourceParent,
		CaptureParent,
		Destructable,
		Occupied,
		Enemy,
		EnemyProjectile
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