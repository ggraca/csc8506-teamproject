#include "MarketPrefab.h"


MarketPrefab::MarketPrefab()
{

}

MarketPrefab::MarketPrefab(const Vector3& Scale, const Vector3& Position, const Quaternion& orient)
{
	OBJGeometry* objGeometry = Assets::AssetManager::LoadOBJ("market-stand.obj");
	GameObject* go = GameObject::FromOBJ(objGeometry);
	go->GetTransform().SetLocalScale(Scale);
	go->GetTransform().SetWorldPosition(Position);
	go->GetTransform().SetLocalOrientation(orient);
}

MarketPrefab::~MarketPrefab()
{
}
