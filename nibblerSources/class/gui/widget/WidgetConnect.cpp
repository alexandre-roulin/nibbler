#include <gui/Gui.hpp>
#include "WidgetConnect.hpp"
#include "cores/Univers.hpp"

WidgetConnect::WidgetConnect(Gui &core) :
		AWidget(core),
		client_(false) {
	bzero(dnsBuffer_, IM_ARRAYSIZE(dnsBuffer_));
	bzero(portBuffer_, IM_ARRAYSIZE(portBuffer_));
	memcpy(dnsBuffer_, DEFAULT_DNS, sizeof(DEFAULT_DNS));
	memcpy(portBuffer_, DEFAULT_PORT, sizeof(DEFAULT_PORT));

}

void WidgetConnect::render(void) {

	SnakeClient::boost_shared_ptr ptr(core_.univers.getSnakeClient().lock());

	if (ptr && ptr->isConnect())
		ImGui::Begin("Connect", NULL,
					 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	else
		ImGui::Begin("Connect", NULL,
					 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

	if (client_)
		Gui::beginColor(Gui::HUE_RED);
	else
		Gui::beginColor(Gui::HUE_GREEN);
	if (ImGui::Button("Host", sf::Vector2f(ImGui::GetWindowSize().x / 2, 20)))
		client_ = false;
	ImGui::SameLine();
	Gui::endColor();

	if (client_)
		Gui::beginColor(Gui::HUE_GREEN);
	else
		Gui::beginColor(Gui::HUE_RED);
	if (ImGui::Button("Client", sf::Vector2f(ImGui::GetWindowSize().x / 2 - 1, 20)))
		client_ = true;

	Gui::endColor();


	if (ImGui::InputText("DNS", dnsBuffer_,
						 IM_ARRAYSIZE(dnsBuffer_),
						 ImGuiInputTextFlags_EnterReturnsTrue)) {

	}
	if (ImGui::InputText("PORT", portBuffer_,
						 IM_ARRAYSIZE(portBuffer_),
						 ImGuiInputTextFlags_EnterReturnsTrue)) {

	}

	ImGui::Spacing();
	ImGui::Spacing();

	Gui::beginColor(Gui::HUE_GREEN);

	if (client_ && !core_.univers.isServer()) {
		if (ptr && ptr->isConnect()) {
			if (ImGui::Button("Join new", sf::Vector2f(ImGui::GetWindowSize().x, 20))) {
				ptr->disconnect();
				ptr->connect(dnsBuffer_, portBuffer_);
			}
		} else if (ImGui::Button("Join", sf::Vector2f(ImGui::GetWindowSize().x, 20))) {
			if (!ptr) {
				core_.univers.createClient();
				{
					SnakeClient::boost_shared_ptr ptr(core_.univers.getSnakeClient().lock());
					if (!ptr)
						throw (std::runtime_error("During creation server : Client is not created"));
					ptr->connect(dnsBuffer_, portBuffer_);
				}
			}
			else
				ptr->connect(dnsBuffer_, portBuffer_);
		}
	}
	else if (!client_) {
		if (ImGui::Button("Create Server", sf::Vector2f(ImGui::GetWindowSize().x, 20))) {
			if (core_.univers.isServer())
				core_.univers.deleteServer();
			core_.univers.createServer();
			core_.univers.createClient();
			{
				SnakeClient::boost_shared_ptr ptr(core_.univers.getSnakeClient().lock());
				if (!ptr)
					throw (std::runtime_error("During creation server : Client is not created"));
				ptr->connect(dnsBuffer_, portBuffer_);
			}
		}
	}
	Gui::endColor();


	ImGui::End();
}

