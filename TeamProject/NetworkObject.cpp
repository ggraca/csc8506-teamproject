#include "NetworkObject.h"

using namespace NCL;
using namespace CSC8503;

NetworkObject::NetworkObject(GameObject* o, int id) : Component(o)	{
	
	networkID   = id;
}

NetworkObject::~NetworkObject()	{
}

bool NetworkObject::ReadPacket(/*GamePacket& p*/) {
	return false; //this isn't a packet we care about!
}

bool NetworkObject::WritePacket(/*GamePacket** p,*/ bool deltaFrame) {
	return false; //this isn't a packet we care about!
}
//Client objects recieve these packets
//bool NetworkObject::ReadDeltaPacket(/*DeltaPacket &p*/) {
//	return true;
//}
//
//bool NetworkObject::ReadFullPacket(/*FullPacket &p*/) {
//	return true;
//}
//
//bool NetworkObject::WriteDeltaPacket(/*GamePacket**p*/) {
//	return true;
//}
//
//bool NetworkObject::WriteFullPacket(/*GamePacket**p*/) {
//	return true;
//}
