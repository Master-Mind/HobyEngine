#include "DebugWindow.h"
#include "Debugger.h"
#include <imgui.h>
#include "MetaEnum.h"

DebugWindow::DebugWindow() : _flags(DTag::Default)
{
}

DebugWindow::DebugWindow(long long flags) : _flags(flags)
{
}

bool DebugWindow::Display(int num)
{
	ImGui::Begin(("Debug Output" + std::to_string(num)).c_str(), 0, ImGuiWindowFlags_MenuBar);
	if(ImGui::BeginMenuBar())
	{
		if (ImGui::Button("Close"))
		{
			return true;
		}
		if (ImGui::BeginMenu("Tags"))
		{
			auto Enum = static_cast<MetaEnum<DTag::DebugTag> *>(AllEnums::GetEnum("DebugTag"));
			int i = 0;
			for (auto meta_enum = Enum->begin(); meta_enum != Enum->end(); ++meta_enum)
			{
				std::string wew = meta_enum->first + (_flags & (1 << i) ? '*' : ' ');
				if (ImGui::Button(wew.c_str()))
				{
					_flags ^= 1 << i;
				}
				i++;
			}
			ImGui::EndMenu();

		}
		ImGui::EndMenuBar();
	}
	Debugger::DumpLogToEditWIndow(_flags);
	ImGui::End();
	return false;
}
