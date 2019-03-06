#pragma once

#include "fmod_studio.hpp"
#include "fmod.hpp"
#include <string>
#include <map>
#include <vector>
#include <math.h>
#include <iostream>
#include "../Common/Vector3.h"
#include "../../Common/Assets.h"
#include "GameObject.h"

using namespace std;
using namespace NCL;
using namespace NCL::Maths;
using namespace NCL::CSC8503;

struct Implementation {
	Implementation();
	~Implementation();

	void Update();

	FMOD::Studio::System* mpStudioSystem;
	FMOD::System* mpSystem;

	int mnNextChannelId;

	typedef map<string, FMOD::Sound*> SoundMap;
	typedef map<int, FMOD::Channel*> ChannelMap;
	typedef map<string, FMOD::Studio::EventInstance*> EventMap;
	typedef map<string, FMOD::Studio::Bank*> BankMap;
	BankMap mBanks;
	EventMap mEvents;
	SoundMap mSounds;
	ChannelMap mChannels;
};

class CAudioEngine {
public:
	CAudioEngine();
	void Init();
	void Update();
	void Shutdown();
	static int ErrorCheck(FMOD_RESULT result);
		
	void LoadBank(const std::string& strBankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags);
	void LoadEvent(const std::string& strEventName, const Vector3& vPosition);
	void LoadSound(const string &strSoundName, bool b3d = true, bool bLooping = false, bool bStream = false);
	void UnLoadSound(const string &strSoundName);
	int PlaySounds(const string &strSoundName, const Vector3& vPos = Vector3{ 0, 0, 0 }, float fVolumedB = 0.0f);
	void PlayEvent(const string &strEventName, const Vector3& vPosition = Vector3(0, 0, 0));
	void StopEvent(const string &strEventName, bool bImmediate = false);
	void GetEventParameter(const string &strEventName, const string &strEventParameter, float* parameter);
	void SetEventParameter(const string &strEventName, const string &strParameterName, float fValue);
	void SetChannel3dPosition(int nChannelId, const Vector3& vPosition);
	void SetChannelVolume(int nChannelId, float fVolumedB);
	bool IsEventPlaying(const string &strEventName) const;
	float dbToVolume(float dB);
	float VolumeTodB(float volume);
	FMOD_VECTOR VectorToFmod(const Vector3& vPosition);
	void SetCamera(GameObject*);
	void SetCameraPos();
	void setMinMaxDistance(float min, float max);
	void setNumList(int num);
	void SetPlayer(GameObject*);
	void SetOrientation();

private:
	Implementation* sgpImplementation = nullptr;
	const float DISTANCEFACTOR = 1.0f;
	float min;
	float max;
	FMOD_VECTOR listenerpos;
	FMOD_VECTOR cPos;
	FMOD_VECTOR forward;
	FMOD_VECTOR up = { 0.0f,1.0f,0.0f };
	float M_PI = 3.14159265358979323846;
	GameObject* camera;
	GameObject* player;
};