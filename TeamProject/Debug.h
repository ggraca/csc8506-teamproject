#pragma once
#include "../Plugins/OpenGLRendering/OGLRenderer.h"

using namespace std;

namespace NCL {
	class Debug
	{
	public:
		static void Print(const std::string& text, const Vector2&pos, const Vector4& colour = Vector4(1, 1, 1, 1));
		static void DrawLine(const Vector3& startpoint, const Vector3& endpoint, const Vector4& colour = Vector4(1, 1, 1, 1));
		static void AddStringToDebugMenu(const std::string& text);
		//static void DrawPoint();

		static void SetRenderer(OGLRenderer* r) {
			renderer = r;
		}

		static void FlushRenderables();

	protected:
		struct DebugStringEntry {
			std::string	data;
			Vector2 position;
			Vector4 colour;
			float size;
		};

		struct DebugLineEntry {
			Vector3 start;
			Vector3 end;
			Vector4 colour;
		};

		Debug() {}
		~Debug() {}

		static std::vector<DebugStringEntry>	stringEntries;
		static std::vector<DebugLineEntry>	lineEntries;
		static std::vector<DebugStringEntry> DebugMenu;

		static OGLRenderer* renderer;
	};
}
