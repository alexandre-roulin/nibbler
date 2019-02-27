#include <cores/Univers.hpp>
#include <gui/Gui.hpp>
#include "WidgetOption.hpp"

WidgetOption::WidgetOption(Gui &core) :
		AWidget(core),
		mapSize_(core_.univers.getMapSize()),
		sound_(core_.univers.getSoundManager().hasLibraryLoaded()) {
	SnakeClient::boost_shared_ptr ptr(core_.univers.getSnakeClient().lock());

	if (ptr)
		memcpy(nameBuffer_, ptr->getSnake().name, NAME_BUFFER);
}

void WidgetOption::render(void) {
	SnakeClient::boost_shared_ptr ptr(core_.univers.getSnakeClient().lock());

	ImGui::Begin("Options", NULL,
				 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
				 ImGuiWindowFlags_NoCollapse);

	if (ImGui::InputText("Name", nameBuffer_,
						 IM_ARRAYSIZE(nameBuffer_) - 1,
						 ImGuiInputTextFlags_EnterReturnsTrue) && ptr) {
		if (strlen(nameBuffer_) > 2)
			ptr->changeName(nameBuffer_);
		memcpy(nameBuffer_, ptr->getSnake().name, NAME_BUFFER);

		ptr->changeName(nameBuffer_);
		std::memcpy(nameBuffer_,ptr->getSnake().name,NAME_BUFFER);
	}

	if (ImGui::InputInt("Size map", reinterpret_cast<int *>(&mapSize_), 1,
						4, ImGuiInputTextFlags_CharsDecimal |
						   ImGuiInputTextFlags_CharsNoBlank |
						   ImGuiInputTextFlags_EnterReturnsTrue) && ptr) {
		if (mapSize_ < MAP_MIN)
			mapSize_ = MAP_MIN;
		else if (mapSize_ > MAP_MAX)
			mapSize_ = MAP_MAX;

		core_.univers.setMapSize(mapSize_);
		core_.univers.updateSizeMap();
	}

	if (ImGui::Checkbox("Sound", &sound_)) {
		if (sound_)
			core_.univers.loadSound(eSound::kSoundSfmlLibrary);
		else
			core_.univers.unloadSound();
	}


	bool borderless = core_.univers.isBorderless();
	if (ImGui::Checkbox("Borderless", &borderless)) {
		core_.univers.switchBorderless();
	}

	if (core_.univers.isServer() && ptr && ptr->allSnakeIsReady()) {
		Gui::beginColor(Gui::HUE_GREEN);
		if (ImGui::Button("Run the game")) {
		}
		Gui::endColor();
	}

	ImGui::End();
}