#include "AudioEngine.h"

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

CAudioEngine::CAudioEngine() {
	Init();
	setNumList(1);
	setMinMaxDistance(10.0f, 10000.0f);
	LoadSound(Assets::SOUNDSDIR + "jaguar.wav", true, false, false);
	LoadSound(Assets::SOUNDSDIR + "bat.wav", true, false, false);
	LoadSound(Assets::SOUNDSDIR + "swords.mp3", true, false, false);
	LoadSound(Assets::SOUNDSDIR + "1.mp3", true, false, false);
	LoadSound(Assets::SOUNDSDIR + "2.mp3", true, false, false);
	LoadSound(Assets::SOUNDSDIR + "ole.wav", true, false, false);
	LoadSound(Assets::SOUNDSDIR + "thud.wav", true, false, false);
	LoadSound(Assets::SOUNDSDIR + "jump.wav", true, false, false);
}

void CAudioEngine::Init() {
	sgpImplementation = new Implementation();
	sgpImplementation->mpSystem->set3DSettings(NULL, DISTANCEFACTOR, 0.1f);
	listenerpos = { 0.0f, 0.0f, 0.0f };
	//FMOD::System_Create(&system);
}

void CAudioEngine::Update() {
	SetOrientation();
	SetCameraPos();
	sgpImplementation->mpSystem->set3DListenerAttributes(0, &cPos, NULL,&forward, &up);
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
		pSound->set3DMinMaxDistance(min * DISTANCEFACTOR, max*DISTANCEFACTOR);
		//sgpImplementation->mpSystem->set3DListenerAttributes(0, &listenerpos, NULL, &forward, &up);
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


void CAudioEngine::SetChannel3dPosition(int nChannelId, const Vector3& vPosition) // sets position of sound use as well as previous function
{
	auto tFoundIt = sgpImplementation->mChannels.find(nChannelId);
	if (tFoundIt == sgpImplementation->mChannels.end())
		return;

	FMOD_VECTOR position = VectorToFmod(vPosition);
	CAudioEngine::ErrorCheck(tFoundIt->second->set3DAttributes(&position, NULL));
}

void CAudioEngine::SetChannelVolume(int nChannelId, float fVolumedB) // sets volume of sound use as well as previous function
{
	auto tFoundIt = sgpImplementation->mChannels.find(nChannelId);
	if (tFoundIt == sgpImplementation->mChannels.end())
		return;

	CAudioEngine::ErrorCheck(tFoundIt->second->setVolume(dbToVolume(fVolumedB)));
}


void CAudioEngine::LoadBank(const std::string& strBankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags) { // loads banks, banks store information and sounds for events
	auto tFoundIt = sgpImplementation->mBanks.find(strBankName);
	if (tFoundIt != sgpImplementation->mBanks.end())
		return;
	FMOD::Studio::Bank* pBank;
	CAudioEngine::ErrorCheck(sgpImplementation->mpStudioSystem->loadBankFile(strBankName.c_str(), flags, &pBank));
	if (pBank) {
		sgpImplementation->mBanks[strBankName] = pBank;
	}
}

void CAudioEngine::LoadEvent(const string& strEventName, const Vector3& vPosition) { // load events, each event loaded separately to save memory 
	auto tFoundit = sgpImplementation->mEvents.find(strEventName);
	if (tFoundit != sgpImplementation->mEvents.end())
		return;
	FMOD::Studio::EventDescription* pEventDescription = NULL;
	CAudioEngine::ErrorCheck(sgpImplementation->mpStudioSystem->getEvent(strEventName.c_str(), &pEventDescription));
	FMOD::Studio::EventInstance* pEventInstance = NULL;
	if (pEventDescription) {
		
		CAudioEngine::ErrorCheck(pEventDescription->createInstance(&pEventInstance));
		if (pEventInstance) {
			sgpImplementation->mEvents[strEventName] = pEventInstance;
		}
	}
	pEventInstance->setReverbLevel(10, 100);

	FMOD_3D_ATTRIBUTES test;
	test.position = VectorToFmod(vPosition);
	test.forward = forward;
	test.up = up;
	test.velocity = VectorToFmod(Vector3(0,0,0));
//	test.position = VectorToFmod(Vector3(-19, 0, 5));
	pEventInstance->set3DAttributes(&test);
//	CAudioEngine::ErrorCheck(tFoundit->second->set3DAttributes(&position, NULL));
}

void CAudioEngine::PlayEvent(const string& strEventName, const Vector3& vPosition) { // play event, if not loaded this will load it
	auto tFoundit = sgpImplementation->mEvents.find(strEventName);
	if (tFoundit == sgpImplementation->mEvents.end()) {
		LoadEvent(strEventName, vPosition);
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

void CAudioEngine::GetEventParameter(const string &strEventName, const string &strParameterName, float* parameter) { //allows to get events dynamically, allows sound scapes
	auto tFoundIt = sgpImplementation->mEvents.find(strEventName);
	if (tFoundIt == sgpImplementation->mEvents.end())
		return;

	FMOD::Studio::ParameterInstance* pParameter = NULL;
	CAudioEngine::ErrorCheck(tFoundIt->second->getParameter(strParameterName.c_str(), &pParameter));
	CAudioEngine::ErrorCheck(pParameter->getValue(parameter));
}

void CAudioEngine::SetEventParameter(const string &strEventName, const string &strParameterName, float fValue) { //allows to set events dynamically
	auto tFoundIt = sgpImplementation->mEvents.find(strEventName);
	if (tFoundIt == sgpImplementation->mEvents.end())
		return;

	FMOD::Studio::ParameterInstance* pParameter = NULL;
	CAudioEngine::ErrorCheck(tFoundIt->second->getParameter(strParameterName.c_str(), &pParameter));
	CAudioEngine::ErrorCheck(pParameter->setValue(fValue));
}

FMOD_VECTOR CAudioEngine::VectorToFmod(const Vector3& vPosition) { // sets world space to fmod co ord
	FMOD_VECTOR fVec;
	fVec.x = vPosition.x;
	fVec.y = vPosition.y;
	fVec.z = vPosition.z;
	return fVec;
}

void CAudioEngine::SetCamera(GameObject* cam) { // sets world space to fmod coord
	
	camera = cam;
}

void CAudioEngine::SetCameraPos() {
	Vector3 vPosition = camera->GetTransform().GetWorldPosition();
	cPos.x = vPosition.x;
	cPos.y = vPosition.y;
	cPos.z = vPosition.z;
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

void CAudioEngine::setNumList(int num) {
	sgpImplementation->mpSystem->set3DNumListeners(num);
}

void CAudioEngine::SetPlayer(GameObject* player1) {

	player = player1;
}

void CAudioEngine::SetOrientation() {

	forward.x = sin(player->GetTransform().GetLocalOrientation().ToEuler().y * (M_PI / 180)) * cos(player->GetTransform().GetLocalOrientation().ToEuler().x * (M_PI / 180));
	forward.y = sin(-player->GetTransform().GetLocalOrientation().ToEuler().x * (M_PI / 180));
	forward.z = cos(player->GetTransform().GetLocalOrientation().ToEuler().x * (M_PI / 180)) * cos(player->GetTransform().GetLocalOrientation().ToEuler().y * (M_PI / 180));
}

// code tutorial from https://codyclaborn.me/tutorials/making-a-basic-fmod-audio-engine-in-c/