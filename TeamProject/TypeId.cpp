#include "TypeId.h"
#include <typeinfo>
#include <iostream>
#include "PhysicsObject.h"
#include "RenderObject.h"
#include "Player.h"
#include "Destructable.h"
#include "Enemy.h"
#include "DamageControl.h"
#include "CameraControl.h"

using namespace std;

int TypeId::GetTypeId(const type_info &info)
{
	if (info.hash_code() == typeid(PhysicsObject*).hash_code()) { return 0; }
	if (info.hash_code() == typeid(RenderObject*).hash_code())  { return 1; }
	if (info.hash_code() == typeid(ScriptObject*).hash_code())  { return 3; }
	if (info.hash_code() == typeid(Player*).hash_code())		{ return 4; }
	if (info.hash_code() == typeid(Resource*).hash_code())		{ return 5; }
	if (info.hash_code() == typeid(Destructable*).hash_code())  { return 6; }
	if (info.hash_code() == typeid(Enemy*).hash_code())			{ return 7; }
	if (info.hash_code() == typeid(DamageControl*).hash_code()) { return 8; }
	if (info.hash_code() == typeid(HealthManager*).hash_code()) { return 9; }
	if (info.hash_code() == typeid(CameraControl*).hash_code()) { return 10; }
	else
	{
		cout << "Component type you are using is not registered. Please first register your component type with a unique id." << endl;
		system("pause");
		return -1;
	}
}
