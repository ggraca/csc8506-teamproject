#pragma once

#include <enet/enet.h>
#include <map>
#include <iostream>
#include <string>

using namespace std;


enum BasicNetworkMessages {
	None,
	Hello,
	Message,
	StringMessage,
	Delta_State,	//1 byte per channel since the last state
	Full_State,		//Full transform etc
	Received_State, //received from a client, informs that its received packet n
	Player_Connected,
	Player_Disconnected,
	Shutdown,
	PlayerPos
};

struct GamePacket {
	short size;
	short type;

	GamePacket() {
		type		= BasicNetworkMessages::None;
		size		= 0;
	}

	GamePacket(short type) {
		this->type	= type;
	}

	int GetTotalSize() {
		return sizeof(GamePacket) + size;
	}
};

struct StringPacket : public GamePacket {
	char	stringData[256];

	StringPacket(const std::string& message) {
		type		= BasicNetworkMessages::StringMessage;
		size		= (short)message.length();

		memcpy(stringData, message.data(), size);
	};

	std::string GetStringFromData() {
		std::string realString(stringData);
		realString.resize(size);
		return realString;
	}
};

struct PlayerPosPacket : public GamePacket {
	float	x;
	float	y;
	float	z;

	PlayerPosPacket(const float& newX, const float& newY, const float& newZ) 
	{
		type = BasicNetworkMessages::PlayerPos;
		size = sizeof(float) * 3;

		x = newX;
		y = newY;
		z = newZ;
		//memcpy(x, newX, size);
		//memcpy(y, newY, size);
		//memcpy(z, newZ, size);
	}

	float* GetPosFromData() {
		float* posData = new float[3];
		posData[0] = x;
		posData[1] = y;
		posData[2] = z;
		return posData;
	}
};

struct NewPlayerPacket : public GamePacket {
	int playerID;
	NewPlayerPacket(int p ) {
		type		= BasicNetworkMessages::Player_Connected;
		playerID	= p;
		size		= sizeof(int);
	}
};

struct PlayerDisconnectPacket : public GamePacket {
	int playerID;
	PlayerDisconnectPacket(int p) {
		type		= BasicNetworkMessages::Player_Disconnected;
		playerID	= p;
		size		= sizeof(int);
	}
};

class PacketReceiver {
public:
	virtual void ReceivePacket(int type, GamePacket* payload, int source = -1) = 0;
};

class TestPacketReceiver : public PacketReceiver {
public:
	TestPacketReceiver(string name)
	{
		this->name = name;
	}

	void ReceivePacket(int type, GamePacket* payload, int source) override {
		if (type == StringMessage)
		{
			StringPacket* realPacket = (StringPacket*)payload;
			string msg = realPacket->GetStringFromData();
			cout << "received message: " << msg << endl;
		}
		else if (type == PlayerPos)
		{
			PlayerPosPacket* realPacket = (PlayerPosPacket*)payload;
			float* pos = realPacket->GetPosFromData();

			std::cout << "X: " << pos[0] << " Y: " << pos[1] << " Z: " << pos[2] << std::endl;
		}
	}
protected:
	string name;
};

class NetworkBase	{
public:
	static void Initialise();
	static void Destroy();

	static int GetDefaultPort() {
		return 1234;
	}

	void RegisterPacketHandler(int msgID, TestPacketReceiver* receiver) {
		//receiver = new TestPacketReceiver("Server");
		packetHandlers.insert(std::make_pair(msgID, receiver));
	}

protected:
	NetworkBase();
	~NetworkBase();

	bool ProcessPacket(GamePacket* p, int peerID = -1);

	typedef std::multimap<int, TestPacketReceiver*>::const_iterator PacketHandlerIterator;

	bool GetPackethandlers(int msgID, PacketHandlerIterator& first, PacketHandlerIterator& last) const {
		auto range = packetHandlers.equal_range(msgID);

		if (range.first == packetHandlers.end()) {
			return false; //no handlers for this message type!
		}
		first	= range.first;
		last	= range.second;
		return true;
	}

	ENetHost* netHandle;

	std::multimap<int, TestPacketReceiver*> packetHandlers;
};
