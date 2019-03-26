#include "Debug.h"

using namespace NCL;

OGLRenderer* Debug::renderer = nullptr;

std::vector<Debug::DebugStringEntry>	Debug::stringEntries;
std::vector<Debug::DebugLineEntry>		Debug::lineEntries;
std::vector<Debug::DebugStringEntry>    Debug::DebugMenu;


void Debug::Print(const std::string& text, const Vec2&pos, const Vec4& colour) {
	DebugStringEntry newEntry;

	newEntry.data		= text;
	newEntry.position	= pos;
	newEntry.colour		= colour;

	stringEntries.emplace_back(newEntry);
}

void Debug::DrawLine(const Vec3& startpoint, const Vec3& endpoint, const Vec4& colour) {
	DebugLineEntry newEntry;

	newEntry.start = startpoint;
	newEntry.end = endpoint;
	newEntry.colour = colour;

	lineEntries.emplace_back(newEntry);
}

void Debug::AddStringToDebugMenu(const std::string &text) {
	DebugStringEntry newEntry;

	newEntry.data = text;
	newEntry.position = Vec2(0, 0);
	newEntry.colour = Vec4(1, 1, 1, 1);
	newEntry.size = 0.5f;

	DebugMenu.emplace_back(newEntry);
}

void Debug::FlushRenderables() {
	if (!renderer) {
		return;
	}
	for (const auto& i : stringEntries) {
		renderer->DrawString(i.data, i.position);
	}

	for (const auto& i : lineEntries) {
		renderer->DrawLine(i.start, i.end, i.colour);
	}

	for (unsigned int i = 0; i < DebugMenu.size(); i++)
	{
		renderer->DrawString(DebugMenu[i].data, Vec2(20.0f, (DebugMenu.size() - i) * 20.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f), DebugMenu[i].size);
	}

	DebugMenu.clear();
	stringEntries.clear();
	lineEntries.clear();
}