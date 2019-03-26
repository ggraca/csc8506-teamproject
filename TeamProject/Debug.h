#pragma once
#include "../Plugins/OpenGLRendering/OGLRenderer.h"

using namespace std;

namespace NCL {
	class Debug
	{
	public:
		static void Print(const std::string& text, const Vec2&pos, const Vec4& colour = Vec4(1, 1, 1, 1));
		static void DrawLine(const Vec3& startpoint, const Vec3& endpoint, const Vec4& colour = Vec4(1, 1, 1, 1));
		static void AddStringToDebugMenu(const std::string& text);
		//static void DrawPoint();

		static void SetRenderer(OGLRenderer* r) {
			renderer = r;
		}

		static void FlushRenderables();

	protected:
		struct DebugStringEntry {
			std::string	data;
			Vec2 position;
			Vec4 colour;
			float size;
		};

		struct DebugLineEntry {
			Vec3 start;
			Vec3 end;
			Vec4 colour;
		};

		Debug() {}
		~Debug() {}

		static std::vector<DebugStringEntry>	stringEntries;
		static std::vector<DebugLineEntry>	lineEntries;
		static std::vector<DebugStringEntry> DebugMenu;

		static OGLRenderer* renderer;
	};
}

