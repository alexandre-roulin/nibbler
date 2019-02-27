#include <cores/Univers.hpp>
#include <gui/Gui.hpp>
#include "WidgetOption.hpp"

WidgetOption::WidgetOption(Gui &core) :
		AWidget(core),
		sound_(core_.univers.getSoundManager().hasLibraryLoaded()),
		rNoise_(core_.univers.getSoundManager().getNoise()),
		rMusique_(core_.univers.getSoundManager().getMusique()) {
	SnakeClient::boost_shared_ptr ptr(core_.univers.getSnakeClient().lock());
	if (ptr)
		memcpy(nameBuffer_, ptr->getSnake().name, NAME_BUFFER);

	boost::filesystem::path pathSound(NIBBLER_ROOT_PROJECT_PATH);
	pathSound_ = (pathSound / "ressources" / "sound" / "zelda.ogg").generic_string();
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

	ImGui::End();
}