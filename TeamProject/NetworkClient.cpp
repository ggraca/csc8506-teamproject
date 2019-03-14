#include "NetworkClient.h"

void NetworkClient::ReceivePacket(int type, GamePacket* payload, int source) {
	if (type == StringMessage)
	{
		StringPacket* realPacket = (StringPacket*)payload;
		string msg = realPacket->GetStringFromData();
		std::cout << "received message: " << msg << std::endl;
	}
	else if (type == PlayerPos)
	{
		PlayerPosPacket* realPacket = (PlayerPosPacket*)payload;
		float* pos = realPacket->GetPosFromData();
		std::cout << "X: " << pos[0] << " Y: " << pos[1] << " Z: " << pos[2] << std::endl;
	}

}