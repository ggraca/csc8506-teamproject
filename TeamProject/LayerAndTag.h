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
		Resources,
		ResourceParent,
		CaptureParent,
		Destructable,
		Occupied,
		Player,
		Enemy,
		EnemyProjectile
	};

	void SetLayerMatrixElement(ObjectLayer layer1, ObjectLayer layer2, bool collide);

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