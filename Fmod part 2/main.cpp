#include "AudioEngine.h"
#include "Asset.h"
#include <fstream>
#include <windows.h>

using namespace std;
using namespace NCL;

inline bool exists_test0(const std::string& name) {
	ifstream f(name.c_str());
	return f.good();
}

void main() {
		
	Vector3 origin = Vector3(0.0f, 0.0f, 0.0f);

	CAudioEngine ae = CAudioEngine();

	if (exists_test0(Assets::SOUNDSDIR + "jaguar.wav")) {

		ae.Init();
		ae.setMinMaxDistance(5.0f, 5000.0f);
		ae.LoadSound(Assets::SOUNDSDIR + "jaguar.wav",false, true, false);
		ae.PlaySounds(Assets::SOUNDSDIR + "jaguar.wav", origin, 10.0f);

		int i = 0;
		do
		{
			ae.Update();
			i++;
			Sleep(10);
		} while (i < 10000);
	}

}
	
