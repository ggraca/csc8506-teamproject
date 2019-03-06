#include "AudioEngine.h"

const float DISTANCEFACTOR = 1.0f;
float min = 0.0f;
float max = 0.0f;


Implementation::Implementation() {
	mpStudioSystem = NULL;
	CAudioEngine::ErrorCheck(FMOD::Studio::System::create(&mpStudioSystem));
	CAudioEngine::ErrorCheck(mpStudioSystem->initialize(32, FMOD_STUDIO_INIT_LIVEUPDATE, FMOD_INIT_PROFILE_ENABLE, NULL));

	mpSystem = NULL;
	CAudioEngine::ErrorCheck(mpStudioSystem->getLowLevelSystem(&mpSystem));
}

Implementation::~Implementation() {
	CAudioEngine::ErrorCheck(mpStudioSystem->unloadAll());
	CAudioEngine::ErrorCheck(mpStudioSystem->release());
}

void Implementation::Update() {   // if channel stops playing this will delete it
	vector<ChannelMap::iterator> pStoppedChannels;
	for (auto it = mChannels.begin(), itEnd = mChannels.end(); it != itEnd; ++it)
	{
		bool bIsPlaying = false;
		it->second->isPlaying(&bIsPlaying);
		if (!bIsPlaying)
		{
			pStoppedChannels.push_back(it);
		}
	}
	for (auto& it : pStoppedChannels)
	{
		mChannels.erase(it);
	}
	CAudioEngine::ErrorCheck(mpStudioSystem->update());
}

Implementation* sgpImplementation = nullptr;

void CAudioEngine::Init() {
	sgpImplementation = new Implementation;
}

void CAudioEngine::Update() {
	sgpImplementation->Update();
}

void CAudioEngine::LoadSound(const string &strSoundName, bool b3d, bool bLooping, bool bStream) // loads sounds (filename, streaming (Y/N), looping (Y/N), 3D sound (Y/N)) this will store it in the sound cloud
{
	auto tFoundIt = sgpImplementation->mSounds.find(strSoundName);
	if (tFoundIt != sgpImplementation->mSounds.end())
		return;

	FMOD_MODE eMode = FMOD_DEFAULT;
	eMode |= b3d ? FMOD_3D : FMOD_2D;
	eMode |= bLooping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
	eMode |= bStream ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE;

	FMOD::Sound* pSound = nullptr;
	CAudioEngine::ErrorCheck(sgpImplementation->mpSystem->createSound(strSoundName.c_str(), eMode, nullptr, &pSound));
	if (pSound) {
		sgpImplementation->mSounds[strSoundName] = pSound;
	}


	if (b3d == true) {
		pSound->set3DMinMaxDistance(min * DISTANCEFACTOR, max*DISTANCEFACTOR);
	}

}

void CAudioEngine::UnLoadSound(const std::string& strSoundName) // unloads sound out of clod need filename
{
	auto tFoundIt = sgpImplementation->mSounds.find(strSoundName);
	if (tFoundIt == sgpImplementation->mSounds.end())
		return;

	CAudioEngine::ErrorCheck(tFoundIt->second->release());
	sgpImplementation->mSounds.erase(tFoundIt);
}

int CAudioEngine::PlaySounds(const string& strSoundName, const Vector3& vPosition, float fVolumedB) // (filename, world pos, volume) it pauses sound to comfirm it is set up correctly then unpauses and returns a channel ID
{
	int nChannelId = sgpImplementation->mnNextChannelId++;
	auto tFoundIt = sgpImplementation->mSounds.find(strSoundName);
	if (tFoundIt == sgpImplementation->mSounds.end())
	{
		LoadSound(strSoundName);
		tFoundIt = sgpImplementation->mSounds.find(strSoundName);
		if (tFoundIt == sgpImplementation->mSounds.end())
		{
			return nChannelId;
		}
	}
	FMOD::Channel* pChannel = nullptr;
	CAudioEngine::ErrorCheck(sgpImplementation->mpSystem->playSound(tFoundIt->second, nullptr, true, &pChannel));
	if (pChannel)
	{
		FMOD_MODE currMode;
		tFoundIt->second->getMode(&currMode);
		if (currMode & FMOD_3D) {
			FMOD_VECTOR position = VectorToFmod(vPosition);
			CAudioEngine::ErrorCheck(pChannel->set3DAttributes(&position, nullptr));
		}
		CAudioEngine::ErrorCheck(pChannel->setVolume(dbToVolume(fVolumedB)));
		CAudioEngine::ErrorCheck(pChannel->setPaused(false));
		sgpImplementation->mChannels[nChannelId] = pChannel;
	}
	return nChannelId;
}


void CAudioEngine::SetChannel3dPosition(int nChannelId, const Vector3& vPosition) // sets position of sound use as well as prevoius function
{
	auto tFoundIt = sgpImplementation->mChannels.find(nChannelId);
	if (tFoundIt == sgpImplementation->mChannels.end())
		return;

	FMOD_VECTOR position = VectorToFmod(vPosition);
	CAudioEngine::ErrorCheck(tFoundIt->second->set3DAttributes(&position, NULL));
}

void CAudioEngine::SetChannelVolume(int nChannelId, float fVolumedB) // sets volume of sound use as well as prevoius function
{
	auto tFoundIt = sgpImplementation->mChannels.find(nChannelId);
	if (tFoundIt == sgpImplementation->mChannels.end())
		return;

	CAudioEngine::ErrorCheck(tFoundIt->second->setVolume(dbToVolume(fVolumedB)));
}


void CAudioEngine::LoadBank(const std::string& strBankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags) { // loads banks, banks stoe information  and sounds for events
	auto tFoundIt = sgpImplementation->mBanks.find(strBankName);
	if (tFoundIt != sgpImplementation->mBanks.end())
		return;
	FMOD::Studio::Bank* pBank;
	CAudioEngine::ErrorCheck(sgpImplementation->mpStudioSystem->loadBankFile(strBankName.c_str(), flags, &pBank));
	if (pBank) {
		sgpImplementation->mBanks[strBankName] = pBank;
	}
}

void CAudioEngine::LoadEvent(const std::string& strEventName) { // load events, each event loaded seperately to save memory 
	auto tFoundit = sgpImplementation->mEvents.find(strEventName);
	if (tFoundit != sgpImplementation->mEvents.end())
		return;
	FMOD::Studio::EventDescription* pEventDescription = NULL;
	CAudioEngine::ErrorCheck(sgpImplementation->mpStudioSystem->getEvent(strEventName.c_str(), &pEventDescription));
	if (pEventDescription) {
		FMOD::Studio::EventInstance* pEventInstance = NULL;
		CAudioEngine::ErrorCheck(pEventDescription->createInstance(&pEventInstance));
		if (pEventInstance) {
			sgpImplementation->mEvents[strEventName] = pEventInstance;
		}
	}
}

void CAudioEngine::PlayEvent(const string &strEventName) { // play event, if not loaded this will load it
	auto tFoundit = sgpImplementation->mEvents.find(strEventName);
	if (tFoundit == sgpImplementation->mEvents.end()) {
		LoadEvent(strEventName);
		tFoundit = sgpImplementation->mEvents.find(strEventName);
		if (tFoundit == sgpImplementation->mEvents.end())
			return;
	}
	tFoundit->second->start();
}

void CAudioEngine::StopEvent(const string &strEventName, bool bImmediate) { // stop event
	auto tFoundIt = sgpImplementation->mEvents.find(strEventName);
	if (tFoundIt == sgpImplementation->mEvents.end())
		return;

	FMOD_STUDIO_STOP_MODE eMode;
	eMode = bImmediate ? FMOD_STUDIO_STOP_IMMEDIATE : FMOD_STUDIO_STOP_ALLOWFADEOUT;
	CAudioEngine::ErrorCheck(tFoundIt->second->stop(eMode));
}

bool CAudioEngine::IsEventPlaying(const string &strEventName) const { // will tell us if event is playing
	auto tFoundIt = sgpImplementation->mEvents.find(strEventName);
	if (tFoundIt == sgpImplementation->mEvents.end())
		return false;

	FMOD_STUDIO_PLAYBACK_STATE* state = NULL;
	if (tFoundIt->second->getPlaybackState(state) == FMOD_STUDIO_PLAYBACK_PLAYING) {
		return true;
	}
	return false;
}

void CAudioEngine::GetEventParameter(const string &strEventName, const string &strParameterName, float* parameter) { //allows to get events dyamically, allows sound scapes
	auto tFoundIt = sgpImplementation->mEvents.find(strEventName);
	if (tFoundIt == sgpImplementation->mEvents.end())
		return;

	FMOD::Studio::ParameterInstance* pParameter = NULL;
	CAudioEngine::ErrorCheck(tFoundIt->second->getParameter(strParameterName.c_str(), &pParameter));
	CAudioEngine::ErrorCheck(pParameter->getValue(parameter));
}

void CAudioEngine::SetEventParameter(const string &strEventName, const string &strParameterName, float fValue) { //allows to set events dyamically
	auto tFoundIt = sgpImplementation->mEvents.find(strEventName);
	if (tFoundIt == sgpImplementation->mEvents.end())
		return;

	FMOD::Studio::ParameterInstance* pParameter = NULL;
	CAudioEngine::ErrorCheck(tFoundIt->second->getParameter(strParameterName.c_str(), &pParameter));
	CAudioEngine::ErrorCheck(pParameter->setValue(fValue));
}

FMOD_VECTOR CAudioEngine::VectorToFmod(const Vector3& vPosition) { // sets world sapce to fmod co ord
	FMOD_VECTOR fVec;
	fVec.x = vPosition.x;
	fVec.y = vPosition.y;
	fVec.z = vPosition.z;
	return fVec;
}

float  CAudioEngine::dbToVolume(float dB) // converts dB volume to linear
{
	return powf(10.0f, 0.05f * dB);
}

float  CAudioEngine::VolumeTodB(float volume) // converts linear volume to dB
{
	return 20.0f * log10f(volume);
}

int CAudioEngine::ErrorCheck(FMOD_RESULT result) { //error checks
	if (result != FMOD_OK) {
		cout << "FMOD ERROR " << result << endl;
		return 1;
	}
	// cout << "FMOD all good" << endl;
	return 0;
}

void CAudioEngine::Shutdown() { //deletes implememntation
	delete sgpImplementation;
}

void CAudioEngine::setMinMaxDistance(float x, float y) {
	min = x;
	max = y;
}


// code tutorial from https://codyclaborn.me/tutorials/making-a-basic-fmod-audio-engine-in-c/

















