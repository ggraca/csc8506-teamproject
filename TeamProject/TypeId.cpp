#include "TypeId.h"
#include <typeinfo>
#include <iostream>
#include "PhysicsObject.h"
#include "RenderObject.h"
#include "Player.h"
#include "Destructible.h"
#include "Enemy.h"
#include "DamageControl.h"
#include "CameraControl.h"
#include "Animator.h"
#include "Light.h"
#include "GunControl.h"
#include "HammerControl.h"
#include "ParticleSystem.h"
#include "ShieldControl.h"

using namespace std;

int TypeId::GetTypeId(const type_info &info)
{
	if (info.hash_code() == typeid(PhysicsObject*).hash_code()) { return 0; }
	if (info.hash_code() == typeid(RenderObject*).hash_code())  { return 1; }
	if (info.hash_code() == typeid(Animator*).hash_code())		{ return 2; }
	if (info.hash_code() == typeid(ScriptObject*).hash_code())  { return 3; }
	if (info.hash_code() == typeid(Player*).hash_code())		{ return 4; }
	if (info.hash_code() == typeid(Resource*).hash_code())		{ return 5; }
	if (info.hash_code() == typeid(Destructible*).hash_code())  { return 6; }
	if (info.hash_code() == typeid(Enemy*).hash_code())			{ return 7; }
	if (info.hash_code() == typeid(DamageControl*).hash_code()) { return 8; }
	if (info.hash_code() == typeid(HealthManager*).hash_code()) { return 9; }
	if (info.hash_code() == typeid(CameraControl*).hash_code()) { return 10; }
	if (info.hash_code() == typeid(Light*).hash_code())			{ return 11; }
	if (info.hash_code() == typeid(GunControl*).hash_code())	{ return 12; }
	if (info.hash_code() == typeid(HammerControl*).hash_code()) { return 13; }
	if (info.hash_code() == typeid(ParticleSystem*).hash_code()){ return 14; }
	if (info.hash_code() == typeid(NetworkObject*).hash_code()) { return 15; }
	if (info.hash_code() == typeid(ShieldControl*).hash_code()) { return 16; }
	else
	{
		cout << "Component type you are using is not registered. Please first register your component type with a unique id." << endl;
		system("pause");
		return -1;
	}
}
