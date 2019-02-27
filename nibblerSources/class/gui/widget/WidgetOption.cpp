#include <cores/Univers.hpp>
#include <gui/Gui.hpp>
#include "WidgetOption.hpp"
#include "cores/ExternalLibraryDisplayManager.hpp"

WidgetOption::WidgetOption(Gui &core) :
		AWidget(core),
		sound_(core_.univers.getSoundManager().hasLibraryLoaded()),
		rNoise_(core_.univers.getSoundManager().getNoise()),
		rMusique_(core_.univers.getSoundManager().getMusique()),
		indexLibrary_(0) {
	SnakeClient::boost_shared_ptr ptr(core_.univers.getSnakeClient().lock());
	if (ptr)
		memcpy(nameBuffer_, ptr->getSnake().name, NAME_BUFFER);

	boost::filesystem::path pathSound(NIBBLER_ROOT_PROJECT_PATH);
	pathSound_ = (pathSound / "ressources" / "sound" / "zelda.ogg").generic_string();
}

bool getNameOfDisplayLibraryInfo(void *data, int idx, const char **out_str) {
	*out_str = ((const char**)data)[idx];
	return true;
}

void WidgetOption::render() {
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

	if (ImGui::Checkbox("Son", &sound_)) {
		if (sound_)
			core_.univers.loadSound(eSound::kSoundSfmlLibrary);
		else
			core_.univers.unloadSound();
	}

	if (sound_) {
		ImGui::Checkbox("Bruitage", &rNoise_);
		if (ImGui::Checkbox("Musique", &rMusique_)) {
			if (!rMusique_)
				core_.univers.getSoundManager().stopMusic();
			else {

				core_.univers.getSoundManager().playMusic(pathSound_);
			}
		}
	}

	if (core_.univers.isServer() && ptr && ptr->allSnakeIsReady()) {
		Gui::beginColor(Gui::HUE_GREEN);
		if (ImGui::Button("Run the game")) {
		}
		Gui::endColor();
	}

	if (ImGui::BeginCombo("Display", ExternalLibraryDisplayManager::libraryInfo[indexLibrary_].title, 0))  {
		for (int n = 0; n < IM_ARRAYSIZE(ExternalLibraryDisplayManager::libraryInfo); n++) {
			bool is_selected = (indexLibrary_ == n);
			if (ImGui::Selectable(ExternalLibraryDisplayManager::libraryInfo[n].title, is_selected)) {
				indexLibrary_ = n;
				core_.univers.getDisplayManager().setKDisplay(ExternalLibraryDisplayManager::libraryInfo[n].kDisplay); 
			}
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	ImGui::End();
}