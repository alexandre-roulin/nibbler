#include <cores/Univers.hpp>
#include <gui/Gui.hpp>
#include "WidgetOption.hpp"
#include "dynamicLinkLibrary/DisplayDynamicLibrary.hpp"

WidgetOption::WidgetOption(Gui &core) :
		AWidget(core, "Options", NIBBLER_IMGUI_WINDOW_FLAGS_BASIC),
		sound_(core_.univers.getSoundManager().hasLibraryLoaded()),
		rNoise_(core_.univers.getSoundManager().getNoise()),
		rMusique_(core_.univers.getSoundManager().getMusique()) {
	SnakeClient::boost_shared_ptr ptr(core_.univers.getSnakeClient().lock());
	if (ptr)
		memcpy(nameBuffer_, ptr->getSnake().name, NAME_BUFFER);
	else
		memset(nameBuffer_, 0, NAME_BUFFER);

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
			return core_.univers.loadSound();
		} else {
			core_.univers.unloadSound();
			rMusique_ = false;
			rNoise_ = false;
			return false;
		}
	} catch (std::exception const &e) {
		core_.addMessageChat(eColorLog::kRed, e.what());
		return false;
	}
}

bool WidgetOption::musicManagemet_() {
	try {
		if (!rMusique_) {
			core_.univers.getSoundManager().stopMusic();
			return false;
		} else {
			core_.univers.getSoundManager().playMusic(pathSound_);
			return true;
		}
	} catch (std::exception const &e) {
		core_.addMessageChat(eColorLog::kRed, e.what());
		return false;
	}
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

	if (ImGui::BeginCombo("Sound", core_.univers.getSoundManager().getNextLibraryInfo().title.c_str(), 0)) {
		for (unsigned long n = 0; n < SoundDynamicLibrary::libraryInfo.size(); n++) {
			bool is_selected = (core_.univers.getSoundManager().getNextLibraryInfo().kLibrary == SoundDynamicLibrary::libraryInfo[n].kLibrary);
			if (ImGui::Selectable(SoundDynamicLibrary::libraryInfo[n].title.c_str(), is_selected)) {
				if (sound_) {
					sound_ = false;
					sound_ = soundManagement_();
				}
				if (!sound_)
					core_.univers.getSoundManager().setNextKInstance(SoundDynamicLibrary::libraryInfo[n].kLibrary);
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

	if (ImGui::BeginCombo("Display", core_.univers.getDisplayManager().getNextLibraryInfo().title.c_str(), 0)) {
		for (unsigned long n = 0; n < DisplayDynamicLibrary::libraryInfo.size(); n++) {
			bool is_selected = (core_.univers.getDisplayManager().getNextLibraryInfo().kLibrary == n);
			if (ImGui::Selectable(DisplayDynamicLibrary::libraryInfo[n].title.c_str(), is_selected))
				core_.univers.getDisplayManager().setNextKInstance(DisplayDynamicLibrary::libraryInfo[n].kLibrary);
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
}
