#include <cores/Univers.hpp>
#include <gui/Gui.hpp>
#include "WidgetOption.hpp"
#include "cores/ExternalLibraryDisplayManager.hpp"

WidgetOption::WidgetOption(Gui &core) :
		AWidget(core, "Options", NIBBLER_IMGUI_WINDOW_FLAGS_BASIC),
		sound_(core_.univers.getSoundManager().hasLibraryLoaded()),
		rNoise_(core_.univers.getSoundManager().getNoise()),
		rMusique_(core_.univers.getSoundManager().getMusique()),
		indexDisplayLibrary_(0),
		indexSoundLibrary_(0) {
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

void WidgetOption::update_() {}

bool WidgetOption::soundManagement_() {
	try {
		if (sound_) {
			return core_.univers.loadSound(eSound::kSoundSfmlLibrary);;
		} else {
			core_.univers.unloadSound();
		}
	} catch (std::exception const &e) {
		core_.addMessageChat(eColorLog::kRed, e.what());
		return false;
	}
	return true;
}

bool WidgetOption::musicManagemet_() {
	try {
		if (!rMusique_)
			core_.univers.getSoundManager().stopMusic();
		else {
			core_.univers.getSoundManager().playMusic(pathSound_);
		}
	} catch (std::exception const &e) {
		core_.addMessageChat(eColorLog::kRed, e.what());
		return false;
	}
	return true;
}

void WidgetOption::beginContent_() {
	SnakeClient::boost_shared_ptr ptr(core_.univers.getSnakeClient().lock());

	if (ImGui::InputText("Name", nameBuffer_,
						 IM_ARRAYSIZE(nameBuffer_) - 1,
						 ImGuiInputTextFlags_EnterReturnsTrue) && ptr) {
		if (strlen(nameBuffer_) > 2)
			ptr->changeName(nameBuffer_);
		memcpy(nameBuffer_, ptr->getSnake().name, NAME_BUFFER);

		ptr->changeName(nameBuffer_);
		std::memcpy(nameBuffer_,ptr->getSnake().name,NAME_BUFFER);
	}

	if (ImGui::BeginCombo("Sound", ExternalLibrarySoundManager::libraryInfo[indexSoundLibrary_].title, 0)) {
		for (int n = 0; n < IM_ARRAYSIZE(ExternalLibrarySoundManager::libraryInfo); n++) {
			bool is_selected = (indexSoundLibrary_ == n);
			if (ImGui::Selectable(ExternalLibrarySoundManager::libraryInfo[n].title, is_selected)) {
				indexSoundLibrary_ = n;
				core_.univers.getSoundManager().setKSound(ExternalLibrarySoundManager::libraryInfo[n].kLibrary);
			}
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	if (ImGui::Checkbox("Son", &sound_))
		sound_ = soundManagement_();

	if (sound_) {
		ImGui::Checkbox("Bruitage", &rNoise_);
		if (ImGui::Checkbox("Musique", &rMusique_))
			rMusique_ = musicManagemet_();
	}

	if (ImGui::BeginCombo("Display", ExternalLibraryDisplayManager::libraryInfo[indexDisplayLibrary_].title, 0)) {
		for (int n = 0; n < IM_ARRAYSIZE(ExternalLibraryDisplayManager::libraryInfo); n++) {
			bool is_selected = (indexDisplayLibrary_ == n);
			if (ImGui::Selectable(ExternalLibraryDisplayManager::libraryInfo[n].title, is_selected)) {
				indexDisplayLibrary_ = n;
				core_.univers.getDisplayManager().setKDisplay(ExternalLibraryDisplayManager::libraryInfo[n].kLibrary);
			}
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
}
