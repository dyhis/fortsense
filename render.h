#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include <d3dx9.h>
#include "protect/skCrypt.h"
#include "settings.h"
#include "spoofer.h"
#include "OVERLAY.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"
#include "draw.h"
#include "esp.hpp"
#include "shit.h"
#include "fonts.h"
#include "logo.h"
#include "menu/MenuControls.h"
#include <time.h>
#include "auth.hpp"

ImFont* LexendRegular;
ImFont* LexendLight;

ImFont* TabsFont;
ImFont* WeaponFont;
ImFont* iconfont = nullptr;

ImFont* MainFont;
IDirect3DTexture9* Logo;

bool  ShowMenu = true;

static float switch_alpha[3], open_alpha = 0;
static bool toggled = true;
static int m_tabs;
static int enemy_tab;
static int visual_tab;
static int world_visual_tab;
static int color_visual_tab;
static int misc_tab;
std::string EAC_Status;
std::string BE_Status;


inline uintptr_t uRandomSeed;
inline 	IDirect3DTexture9* menuBg;



std::string tm_to_readable_time(tm ctx) {
	char buffer[80];

	strftime(buffer, sizeof(buffer), "%m/%d/%y %H:%M", &ctx);

	return std::string(buffer);
}

static std::time_t string_to_timet(std::string timestamp) {
	auto cv = strtol(timestamp.c_str(), NULL, 10); // long

	return (time_t)cv;
}

static std::tm timet_to_tm(time_t timestamp) {
	std::tm context;

	localtime_s(&context, &timestamp);

	return context;
}


static int tab = 0;

using namespace KeyAuth;


auto name = skCrypt("fortsense"); // Application Name
auto ownerid = skCrypt("oZwAojVIkT"); // Owner ID
auto secret = skCrypt("b8f1121bb3dfdd0c01f89aa763045e2348c9f5f31fec6bcb697a8c35149e8a45"); // Application Secret
auto version = skCrypt("1.0"); // Application Version
auto url = skCrypt("https://keyauth.win/api/1.2/"); // change if you're self-hosting

api KeyAuthApp(name.decrypt(), ownerid.decrypt(), secret.decrypt(), version.decrypt(), url.decrypt(), "");



void ColorPicker(const char* name, float* color, bool alpha) {

	ImGuiWindow* window = ImGui::GetCurrentWindow();
	ImGuiStyle* style = &ImGui::GetStyle();

	auto alphaSliderFlag = alpha ? ImGuiColorEditFlags_AlphaBar : ImGuiColorEditFlags_NoAlpha;

	ImGui::SameLine(219.f);
	ImGui::ColorEdit4(std::string{ "##" }.append(name).append(SK("Picker")).c_str(), color, alphaSliderFlag | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoTooltip);
}


bool LegitButton(const char* label, int* selected, int num, int total, bool isLegitTab, int& currentTab) {
	ImGuiStyle& style = ImGui::GetStyle();
	ImVec2 label_size = ImGui::CalcTextSize(label);

	ImVec2 draw_pos = ImVec2(24, 19) + ImVec2((461 + total) / total * num, 0);
	ImVec2 size = ImVec2(55 + 20, 40);
	ImVec2 offset = ImVec2(1, 1);

	const int sizes[] = { 69, 69, 69, 69 };
	const ImVec2 positions[] = { ImVec2(35, 17), ImVec2(125, 17),  ImVec2(225, 17), ImVec2(325, 17), };

	if (isLegitTab) {
		size = ImVec2(sizes[num], 68);
		draw_pos = positions[num];
	}

	if (num != 0)
		draw_pos = ImVec2(draw_pos.x + (18) * num, draw_pos.y);

	ImGuiWindow* window = ImGui::GetCurrentWindow();
	bool hovered = ImGui::IsMouseHoveringRect(window->Pos + draw_pos, window->Pos + draw_pos + size);

	if (GetAsyncKeyState(VK_LBUTTON) && hovered) {
		*selected = num;
		currentTab = num; // Update the current tab
	}

	ImU32 text_color = IM_COL32(90, 90, 90, 255);
	if (hovered)
		text_color = IM_COL32(140, 140, 140, 255);
	if (*selected == num)
		text_color = IM_COL32(210, 210, 210, 255);

	if (isLegitTab)
		ImGui::GetWindowDrawList()->AddText(legit_icons, 30, window->Pos + draw_pos, text_color, label);
	else
		ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), window->Pos + draw_pos + ImVec2(size.x / 2 - label_size.x / 2, size.y / 2 - label_size.y / 2 - 1), text_color, label);

	return true;
}



static char inputText[256] = "";


void Aimbot() {

	ImGuiStyle* style = &ImGui::GetStyle();
	InsertSpacer("Top Spacer");

	ImGui::Columns(2, NULL, false); {

		InsertGroupBoxLeft("Aimbot", 506.f); {
			style->ItemSpacing = ImVec2(4, 2);
			style->WindowPadding = ImVec2(4, 4);
			ImGui::CustomSpacing(9.f);

		//	ImGui::SetCursorPos(ImVec2(20, 20));
			InsertCheckbox("Aimbot", aimbot);
			InsertCombo("Aim Type", aimbot_type, aimbottype);

			//ImGui::SetCursorPos(ImVec2(20, 40));
			InsertCheckbox("Draw Fov", draw_fov);
			InsertCheckbox("Prediction", prediction);
			InsertCheckbox("Humanization", humanization);	
			InsertCheckbox("Weapon Configs", weaponconfigs);
			InsertCheckbox("Target Line", targetline);
			InsertCheckbox("Target Circle", targetcirlce);		
			InsertCheckbox("Skip Downed", no_downed);
			InsertCheckbox("Distance Check", distance_check);

			

			InsertCheckbox("Visible Check", aim_vis_cveck);


			
			InsertCombo("Hit Box", CurrentHitbox, Hitboxes);
			InsertSlider("Fov", fov, 0.f, 500.f, "%1.f%");
			InsertSlider("Smoothing X", smoothingx, 0.f, 50, "%1.f%");
			InsertSlider("Smoothing Y", smoothingy, 0.f, 50, "%1.f%");

			if (distance_check)
			{
				InsertSlider("Max Distance", max_aimbot_distance, 0.f, 500, "%1.f%");
			}
			ImGui::Spacing();
			ImGui::SetCursorPosX(40);
			HotkeyButton(aimkey, ChangeKey, keystatus);
			
			//InsertColorPicker("##Name color", PlayerColor::visible_color, false);
			//InsertComboWithoutText("##hands material", CurrentHitbox, Hitboxes);



		} InsertEndGroupBoxLeft("Aimbot Cover", "Aimbot");
	}
	ImGui::NextColumn(); {

		InsertGroupBoxRight("Other", 506.f); {
			style->ItemSpacing = ImVec2(4, 2);
			style->WindowPadding = ImVec2(4, 4);
			ImGui::CustomSpacing(9.f);
			InsertCheckbox("Triggerbot", triggerbot);
			InsertCheckbox("Shotgun Only", shotgun_only);
			InsertSlider("Triggerbot Delay", trig_delay, 0.f, 5, "%1.f%");
			InsertSlider("Triggerbot Max Distance", max_trig_distance, 0.f, 5, "%1.f%");

			
			


		} InsertEndGroupBoxRight("Other Cover", "Other");

	}
}


int weapon_tab = 0;
int test_tab = 0;

void Legit() {

	ImGuiStyle* style = &ImGui::GetStyle();
	InsertSpacer("Top Spacer");

	InsertGroupBoxTop("Weapon Selection", ImVec2(535.f, 61.f)); {

	
		LegitButton(SK("G"), &weapon_tab, 0, 4, 1, test_tab);
		LegitButton(SK("e"), &weapon_tab, 1, 4, 1, test_tab);
		LegitButton(SK("W"), &weapon_tab, 2, 4, 1, test_tab);
		LegitButton(SK("Z"), &weapon_tab, 3, 4, 1, test_tab);
	//	LegitButton("a", &weapon_tab, 5, 6, 1);

	} InsertEndGroupBoxTop("Weapon Selection Cover", "Weapon Selection", ImVec2(536.f, 11.f));

	InsertSpacer("Weapon Selection - Main Group boxes Spacer");

	ImGui::Columns(2, NULL, false); {

		if (weaponconfigs)
		{
			if (test_tab == 0)
			{


				InsertGroupBoxLeft("Aimbot", 427.f); {
					style->ItemSpacing = ImVec2(4, 2);
					style->WindowPadding = ImVec2(4, 4);
					ImGui::CustomSpacing(9.f);

					InsertCheckbox("Aimbot", smg_aimbot);
					InsertCheckbox("Draw Fov", smg_draw_fov);
					//	InsertCheckbox("Prediction", prediction);
					InsertCheckbox("Skip Downed", smg_SkipNocked);		
					InsertCheckbox("Distance Check", smg_DistanceCheck);
					InsertCheckbox("Humanization", smg_Humanization);

					InsertCombo("Hit Box", SmgCurrentHitbox, SmgHitboxes);
					InsertSlider("Fov", smg_fov, 0.f, 500.f, "%1.f%");
					InsertSlider("Smoothing X", smg_smoothingx, 0.f, 50, "%1.f%");
					InsertSlider("Smoothing Y", smg_smoothingy, 0.f, 50, "%1.f%");
					if (smg_DistanceCheck)
					{
						InsertSlider("Max Distance", smg_max, 0.f, 500, "%1.f%");
					}

					InsertEndGroupBoxLeft("Aimbot Cover", "Aimbot");
				}


			}
			if (test_tab == 1)
			{


				InsertGroupBoxLeft("Aimbot", 427.f); {
					style->ItemSpacing = ImVec2(4, 2);
					style->WindowPadding = ImVec2(4, 4);
					ImGui::CustomSpacing(9.f);

					InsertCheckbox("Aimbot", shotgun_aimbot);
					InsertCheckbox("Draw Fov", shotgun_draw_fov);
					InsertCheckbox("Skip Downed", shotgun_SkipNocked);
					InsertCheckbox("Distance Check", shotgun_DistanceCheck);
					InsertCheckbox("Humanization", shotgun_Humanization);

					//InsertCheckbox("Prediction", prediction);
					InsertCombo("Hit Box", ShotgunCurrentHitbox, ShotgunHitboxes);
					InsertSlider("Fov", shotgun_fov, 0.f, 500.f, "%1.f%");
					InsertSlider("Smoothing X", shotgun_smoothingx, 0.f, 50, "%1.f%");
					InsertSlider("Smoothing Y", shotgun_smoothingy, 0.f, 50, "%1.f%");

					if (shotgun_DistanceCheck)
					{
						InsertSlider("Max Distance", shotgun_max, 0.f, 500, "%1.f%");

					}

					InsertEndGroupBoxLeft("Aimbot Cover", "Aimbot");
				}
			}
			if (test_tab == 2)
			{


				InsertGroupBoxLeft("Aimbot", 427.f); {
					style->ItemSpacing = ImVec2(4, 2);
					style->WindowPadding = ImVec2(4, 4);
					ImGui::CustomSpacing(9.f);

					InsertCheckbox("Aimbot", rifle_aimbot);
					InsertCheckbox("Draw Fov", rifle_draw_fov);
					//InsertCheckbox("Prediction", prediction);
					InsertCheckbox("Skip Downed", rifle_SkipNocked);
					InsertCheckbox("Distance Check", rifle_DistanceCheck);
					InsertCheckbox("Humanization", rifle_Humanization);

					InsertCombo("Hit Box", RifleCurrentHitbox, RifleHitboxes);
					InsertSlider("Fov", rifle_fov, 0.f, 500.f, "%1.f%");
					InsertSlider("Smoothing X", rifle_smoothingx, 0.f, 50, "%1.f%");
					InsertSlider("Smoothing Y", rifle_smoothingy, 0.f, 50, "%1.f%");
					if (rifle_DistanceCheck)
					{
						InsertSlider("Max Distance", rifle_max, 0.f, 500, "%1.f%");
					}

					InsertEndGroupBoxLeft("Aimbot Cover", "Aimbot");
				}
			}
			if (test_tab == 3)
			{


				InsertGroupBoxLeft("Aimbot", 427.f); {
					style->ItemSpacing = ImVec2(4, 2);
					style->WindowPadding = ImVec2(4, 4);
					ImGui::CustomSpacing(9.f);

					InsertCheckbox("Aimbot", sniper_aimbot);
					InsertCheckbox("Draw Fov", sniper_draw_fov);
					//InsertCheckbox("Prediction", prediction);

					InsertCheckbox("Skip Downed", sniper_SkipNocked);
					InsertCheckbox("Distance Check", sniper_DistanceCheck);
					InsertCheckbox("Humanization", sniper_Humanization);

					InsertCombo("Hit Box", SniperCurrentHitbox, SniperHitboxes);
					InsertSlider("Fov", sniper_fov, 0.f, 2000	, "%1.f%");
					InsertSlider("Smoothing X", sniper_smoothingx, 0.f, 50, "%1.f%");
					InsertSlider("Smoothing Y", sniper_smoothingy, 0.f, 50, "%1.f%");

					if (sniper_DistanceCheck)
					{
						InsertSlider("Max Distance", sniper_max, 0.f, 500, "%1.f%");
					}
					InsertEndGroupBoxLeft("Aimbot Cover", "Aimbot");
				}
			}
		}
		else
		{
			InsertGroupBoxLeft("Aimbot", 427.f); {
				style->ItemSpacing = ImVec2(4, 2);
				style->WindowPadding = ImVec2(4, 4);
				ImGui::CustomSpacing(9.f);
		
				ImGui::SetCursorPosX(10);
				ImGui::SetCursorPos(ImVec2(10, 20));
				ImGui::PushFont(boldMenuFont);
				ImGui::Text(SK("Enable Weapon Configs In Aimbot Settings"));

				InsertEndGroupBoxLeft("Aimbot Cover", "Aimbot");
			}
		}
	}
	ImGui::NextColumn(); {
		if (weaponconfigs)
		{
			if (test_tab == 0)
			{


				InsertGroupBoxRight("Triggerbot", 277.f); {
					style->ItemSpacing = ImVec2(4, 2);
					style->WindowPadding = ImVec2(4, 4);
					ImGui::CustomSpacing(9.f);
					InsertCheckbox("Triggerbot", smg_triggerbot);
					InsertSlider("Triggerbot Delay", smg_trig_delay, 0.f, 5, "%1.f%");
					InsertSlider("Triggerbot Max Distance", smg_max_trig_distance, 0.f, 5, "%1.f%");


				} InsertEndGroupBoxRight("Triggerbot Cover", "Triggerbot");

				InsertSpacer("Triggerbot - Other Spacer");

				InsertGroupBoxRight("Other", 132.f); {
					style->ItemSpacing = ImVec2(4, 2);
					style->WindowPadding = ImVec2(4, 4);
					ImGui::CustomSpacing(9.f);
					InsertCheckbox("Target Line", smg_targetline);
					InsertCheckbox("Target Circle", smg_targetcirlce);

				} InsertEndGroupBoxRight("Other Cover", "Other");


			}
			if (test_tab == 1)
			{

				if (shotgun_only)
				{ 
					InsertGroupBoxRight("Triggerbot", 277.f); {
						style->ItemSpacing = ImVec2(4, 2);
						style->WindowPadding = ImVec2(4, 4);
						ImGui::CustomSpacing(9.f);
						ImGui::SetCursorPosX(10);
						ImGui::SetCursorPos(ImVec2(10, 20));
						ImGui::PushFont(boldMenuFont);
						ImGui::Text("Shotugn Only Enabled");


					} InsertEndGroupBoxRight("Triggerbot Cover", "Triggerbot");
				}
				else
				{
					InsertGroupBoxRight("Triggerbot", 277.f); {
						style->ItemSpacing = ImVec2(4, 2);
						style->WindowPadding = ImVec2(4, 4);
						ImGui::CustomSpacing(9.f);
						InsertCheckbox("Triggerbot", shotgun_triggerbot);
						//InsertCheckbox("Shotgun Only", shotgun_only);
						InsertSlider("Triggerbot Delay", shotgun_trig_delay, 0.f, 5, "%1.f%");
						InsertSlider("Triggerbot Max Distance", shotgun_max_trig_distance, 0.f, 5, "%1.f%");


					} InsertEndGroupBoxRight("Triggerbot Cover", "Triggerbot");
				}

					InsertSpacer("Triggerbot - Other Spacer");

					InsertGroupBoxRight("Other", 132.f); {

						style->ItemSpacing = ImVec2(4, 2);
						style->WindowPadding = ImVec2(4, 4);
						ImGui::CustomSpacing(9.f);
						InsertCheckbox("Target Line", shotgun_targetline);
						InsertCheckbox("Target Circle", shotgun_targetcirlce);
					} InsertEndGroupBoxRight("Other Cover", "Other");
				
			}
			if (test_tab == 2)
			{

				if (shotgun_only)
				{
					InsertGroupBoxRight("Triggerbot", 277.f); {
						style->ItemSpacing = ImVec2(4, 2);
						style->WindowPadding = ImVec2(4, 4);
						ImGui::CustomSpacing(9.f);
						ImGui::SetCursorPosX(10);
						ImGui::SetCursorPos(ImVec2(10, 20));
						ImGui::PushFont(boldMenuFont);
						ImGui::Text("Shotugn Only Enabled");


					} InsertEndGroupBoxRight("Triggerbot Cover", "Triggerbot");
				}
				else
				{
					InsertGroupBoxRight("Triggerbot", 277.f); {
						style->ItemSpacing = ImVec2(4, 2);
						style->WindowPadding = ImVec2(4, 4);
						ImGui::CustomSpacing(9.f);
						InsertCheckbox("Triggerbot", rifle_triggerbot);
						//InsertCheckbox("Shotgun Only", shotgun_only);
						InsertSlider("Triggerbot Delay", rifle_trig_delay, 0.f, 5, "%1.f%");
						InsertSlider("Triggerbot Max Distance", rifle_max_trig_distance, 0.f, 5, "%1.f%");


					} InsertEndGroupBoxRight("Triggerbot Cover", "Triggerbot");
				}

				InsertSpacer("Triggerbot - Other Spacer");

				InsertGroupBoxRight("Other", 132.f); {
					style->ItemSpacing = ImVec2(4, 2);
					style->WindowPadding = ImVec2(4, 4);
					ImGui::CustomSpacing(9.f);
					InsertCheckbox("Target Line", rifle_targetline);
					InsertCheckbox("Target Circle",rifle_targetcirlce);

				} InsertEndGroupBoxRight("Other Cover", "Other");
			}
			if (test_tab == 3)
			{
				if (shotgun_only)
				{
					InsertGroupBoxRight("Triggerbot", 277.f); {
						style->ItemSpacing = ImVec2(4, 2);
						style->WindowPadding = ImVec2(4, 4);
						ImGui::CustomSpacing(9.f);
						ImGui::SetCursorPosX(10);
						ImGui::SetCursorPos(ImVec2(10, 20));
						ImGui::PushFont(boldMenuFont);
						ImGui::Text("Shotugn Only Enabled");


					} InsertEndGroupBoxRight("Triggerbot Cover", "Triggerbot");
				}
				else
				{

					InsertGroupBoxRight("Triggerbot", 277.f); {
						style->ItemSpacing = ImVec2(4, 2);
						style->WindowPadding = ImVec2(4, 4);
						ImGui::CustomSpacing(9.f);
						InsertCheckbox("Triggerbot", sniper_triggerbot);
						InsertSlider("Triggerbot Delay", sniper_trig_delay, 0.f, 5, "%1.f%");
						InsertSlider("Triggerbot Max Distance", sniper_max_trig_distance, 0.f, 5, "%1.f%");


					} InsertEndGroupBoxRight("Triggerbot Cover", "Triggerbot");
					
				}

				InsertSpacer("Triggerbot - Other Spacer");

				InsertGroupBoxRight("Other", 132.f); {
					style->ItemSpacing = ImVec2(4, 2);
					style->WindowPadding = ImVec2(4, 4);
					ImGui::CustomSpacing(9.f);
					InsertCheckbox("Target Line", sniper_targetline);
					InsertCheckbox("Target Circle", sniper_targetcirlce);

				} InsertEndGroupBoxRight("Other Cover", "Other");
			}

			
		}
		else
		{
			InsertGroupBoxRight("Triggerbot", 277.f); {
			


			} InsertEndGroupBoxRight("Triggerbot Cover", "Triggerbot");

			InsertSpacer("Triggerbot - Other Spacer");

			InsertGroupBoxRight("Other", 132.f); {


			} InsertEndGroupBoxRight("Other Cover", "Other");
		}
	}
}

void Visuals() {

	ImGuiStyle* style = &ImGui::GetStyle();
	InsertSpacer("Top Spacer");

	ImGui::Columns(2, NULL, false); {

		InsertGroupBoxLeft("Player ESP", 280.f); {

			style->ItemSpacing = ImVec2(4, 2);
			style->WindowPadding = ImVec2(4, 4);
			ImGui::CustomSpacing(9.f);
			InsertCheckbox("Enable", esp);

			InsertCheckbox("Boxes", box_esp);
			if (box_esp)
			{
				InsertCombo("Box Type", box_type, Boxes)
			}
			InsertCheckbox("Name", draw_username);
			InsertCheckbox("Weapon text", draw_held_weapon);
			InsertCheckbox("Distance", draw_distance);
			InsertCheckbox("Skeleton", skeleton);
			InsertCheckbox("Head", head);
			InsertCheckbox("Custom Image", image_esp);
			//InsertCheckbox("Rainbow Colors", rainbow);
			InsertCheckbox("Snap Lines", enable_snap);
			if (enable_snap)
			{
				InsertCombo("Snap Type", snap, SnapLines)
			}

			
		




			style->ItemSpacing = ImVec2(0, 0);
			style->WindowPadding = ImVec2(6, 6);

		} InsertEndGroupBoxLeft("Player ESP Cover", "Player ESP");

		InsertSpacer("Player ESP - Esp Settings Spacer");

		InsertGroupBoxLeft("Esp Settings", 220.f); {

			style->ItemSpacing = ImVec2(4, 2);	
			style->WindowPadding = ImVec2(4, 4);
			ImGui::CustomSpacing(9.f);
			InsertCheckbox("Box Outline", box_outline);
			InsertCheckbox("Skeleton Outline", skeleton_outline);
			InsertCheckbox("Text Outline", text_outline);
			InsertCheckbox("Line Outline", line_outline);

			InsertSlider("Box Thickness", box_thickness, 0.f, 5, "%1.f%");
			InsertSlider("Skeleton Thickness", skelthick, 0.f, 5, "%1.f%");
			InsertSlider("Line Thickness", line_thick, 0.f, 5, "%1.f%");

			InsertSlider("Text Size", font_size, 0.f, 30, "%1.f%");

			

		/*	ImGui::SetCursorPos(ImVec2(20, 166));
			ImGui::Text("Visible Color");
			InsertColorPicker("Visible Color", PlayerColor::visible_color, true);
			ImGui::SetCursorPosX(10);
			ImGui::SetCursorPos(ImVec2(20, 180));

			ImGui::Text("Non Visible Color");

			InsertColorPicker("Non Visible Color", PlayerColor::nonvisible_color, true);*/

			style->ItemSpacing = ImVec2(0, 0);
			style->WindowPadding = ImVec2(6, 6);

		} InsertEndGroupBoxLeft("Esp Settings Cover", "Esp Settings");
	}
	ImGui::NextColumn(); {

		InsertGroupBoxRight("Other ESP", 159.f); {

			style->ItemSpacing = ImVec2(4, 2);
			style->WindowPadding = ImVec2(4, 4);
			ImGui::CustomSpacing(9.f);


			InsertCheckbox("Mini Map", mini_map_radar);
			InsertCheckbox("Normal Radar", radar);
			InsertSlider("Radar Pox X", radar_position_x , 0.f, 2500, "%1.f%");
			InsertSlider("Radar Pox Y", radar_position_y, 0.f, 2000, "%1.f%");
			InsertSlider("Size", radar_size, 0.f, 500, "%1.f%");


			style->ItemSpacing = ImVec2(0, 0);
			style->WindowPadding = ImVec2(6, 6);

		} InsertEndGroupBoxRight("Other ESP Cover", "Other ESP");

		InsertSpacer("Other ESP - World Esp Spacer");

		InsertGroupBoxRight("World Esp", 340.f); {

			style->ItemSpacing = ImVec2(4, 2);
			style->WindowPadding = ImVec2(4, 4);
			ImGui::CustomSpacing(9.f);
			InsertCheckbox("Item Boxes", loot_boxes);

			InsertCheckbox("Chests", show_chests);
			InsertCheckbox("Ammo Box", show_ammo_box);
			InsertCheckbox("MC Nugget", show_mcnugget);
			InsertCheckbox("Pickups", show_pickups);
			InsertCheckbox("Vehicles", show_vehicles);
			InsertCheckbox("Gold", Gold);
			InsertSlider("Chest Max Distance", chest_render_distance, 0.f, 500, "%1.f%");
			InsertSlider("Ammo Max Distance", ammo_render_distance, 0.f, 500, "%1.f%");
			InsertSlider("Mc Nugget Max Distance", mcnugget_render_distance, 0.f, 500, "%1.f%");
			InsertSlider("Pickups Max Distance", pickup_render_distance, 0.f, 500, "%1.f%");
			InsertSlider("Vehicles Max Distance", vehicle_render_distance, 0.f, 500, "%1.f%");
			InsertSlider("Gold Max Distance", gold_render_distance, 0.f, 500, "%1.f%");


			style->ItemSpacing = ImVec2(0, 0);
			style->WindowPadding = ImVec2(6, 6);

		} InsertEndGroupBoxRight("World Esp Cover", "World Esp");
	}
}

void Misc() {

	ImGuiStyle* style = &ImGui::GetStyle();
	InsertSpacer("Top Spacer");

	ImGui::Columns(2, NULL, false); {

		InsertGroupBoxLeft("Colors", 506.f); {

			style->ItemSpacing = ImVec2(4, 2);
			style->WindowPadding = ImVec2(4, 4);
			ImGui::CustomSpacing(9.f);

		ImGui::SetCursorPosX(10);
		ImGui::SetCursorPos(ImVec2(20, 20));
		ImGui::PushFont(boldMenuFont);
		ImGui::Text(SK("ESP Colors"));
		ImGui::PushFont(menuFont);
		ImGui::SetCursorPosX(10);
		ImGui::SetCursorPos(ImVec2(20, 40));
		ImGui::Text(SK("Box Visible Color"));
		InsertColorPicker("Box Visible Color", PlayerColor::BoxVisible, true);
		ImGui::SetCursorPosX(10);
		ImGui::SetCursorPos(ImVec2(20, 60));

		ImGui::Text(SK("Box Non Visible Color"));

		InsertColorPicker("Box Non Visible Color", PlayerColor::BoxNotVisible, true);

		ImGui::SetCursorPos(ImVec2(20, 80));
		ImGui::Text(SK("Skeleton Visible Color"));
		InsertColorPicker("Skeleton Visible Color", PlayerColor::SkeletonVisible, true);
		ImGui::SetCursorPosX(10);
		ImGui::SetCursorPos(ImVec2(20, 100));

		ImGui::Text(SK("Skeleton Non Visible Color"));

		InsertColorPicker("Skeleton Non Visible Color", PlayerColor::SkeletonNotVisible, true);

		ImGui::SetCursorPos(ImVec2(20, 120));
		ImGui::Text(SK("Snapline Visible Color"));
		InsertColorPicker("Line Visible Color", PlayerColor::LineVisible, true);
		ImGui::SetCursorPosX(10);
		ImGui::SetCursorPos(ImVec2(20, 140));

		ImGui::Text(SK("Snapline Non Visible Color"));

		InsertColorPicker("Snapline Non Visible Color", PlayerColor::LineNotVisible, true);

		ImGui::SetCursorPos(ImVec2(20, 160));
		ImGui::Text(SK("Top Text Visible Color"));
		InsertColorPicker("Top Text Visible Color", PlayerColor::TopTextVisible, true);
		ImGui::SetCursorPosX(10);
		ImGui::SetCursorPos(ImVec2(20, 180));

		ImGui::Text(SK("Top Text Non Visible Color"));

		InsertColorPicker("Top Text Non Visible Color", PlayerColor::TopTextNotVisible, true);

		ImGui::SetCursorPos(ImVec2(20, 200));
		ImGui::Text(SK("Bottom Text Visible Color"));
		InsertColorPicker("Bottom Text Visible Color", PlayerColor::BottomTextVisible, true);
		ImGui::SetCursorPosX(10);
		ImGui::SetCursorPos(ImVec2(20, 220));

		ImGui::Text(SK("Bottom Text Non Visible Color"));
		InsertColorPicker("Bottom Text Non Visible Color", PlayerColor::BottomTextNotVisible, true);

		ImGui::SetCursorPosX(10);
		ImGui::SetCursorPos(ImVec2(20, 240));
		ImGui::PushFont(boldMenuFont);
		ImGui::Text(SK("Radar Colors"));
		ImGui::PushFont(menuFont);
		ImGui::SetCursorPosX(10);
		ImGui::SetCursorPos(ImVec2(20, 260));

		ImGui::Text(SK("Radar Visible Color"));
		InsertColorPicker("Radar Visible Color", PlayerColor::RadarVisible, true);

		ImGui::SetCursorPosX(10);
		ImGui::SetCursorPos(ImVec2(20, 280));

		ImGui::Text(SK("Radar Not Visible Color"));
		InsertColorPicker("Radar Not Visible Color", PlayerColor::RadarNotVisible, true);
		ImGui::SetCursorPosX(10);
		ImGui::SetCursorPos(ImVec2(20, 300));
		ImGui::PushFont(boldMenuFont);
		ImGui::Text(SK("Other Colors"));
		ImGui::PushFont(menuFont);
		ImGui::SetCursorPosX(10);
		ImGui::SetCursorPos(ImVec2(20, 320));

		ImGui::Text(SK("Fov Circle Color"));
		InsertColorPicker("Fov Circle Color", PlayerColor::fov_col, true);

		ImGui::SetCursorPosX(10);
		ImGui::SetCursorPos(ImVec2(20, 340));

		ImGui::Text(SK("Crosshair Color"));
		InsertColorPicker("Crosshair Color", PlayerColor::Crosshair_color, true);
		ImGui::SetCursorPosX(10);
		ImGui::SetCursorPos(ImVec2(20, 360));
		ImGui::PushFont(boldMenuFont);
		ImGui::Text(SK("Aimbot Colors"));
		ImGui::PushFont(menuFont);
		ImGui::SetCursorPosX(10);
		ImGui::SetCursorPos(ImVec2(20, 380));

		ImGui::Text(SK("Target Visible Color"));
		InsertColorPicker("Target Visible Color", PlayerColor::target_line, true);

		ImGui::SetCursorPosX(10);
		ImGui::SetCursorPos(ImVec2(20, 400));

		ImGui::Text(SK("Target Not Visible Color"));
		InsertColorPicker("Target Not Visible Color", PlayerColor::target_line_not_visible, true);
		ImGui::SetCursorPosX(10);
		ImGui::SetCursorPos(ImVec2(20, 420));
		ImGui::PushFont(boldMenuFont);
		ImGui::Text(SK("Rainbow Stuff"));
		ImGui::PushFont(menuFont);
		InsertCheckbox("Rainbow Colors", rainbow);
		InsertSlider("Rainbow Speed", color_speed, 0.f, 10, "%1.f%");



			style->ItemSpacing = ImVec2(0, 0);
			style->WindowPadding = ImVec2(6, 6);

		} InsertEndGroupBoxLeft("Colors Cover", "Colors");

	}
	ImGui::NextColumn(); {

		InsertGroupBoxRight("Fonts", 156.f); {
			style->ItemSpacing = ImVec2(4, 2);
			style->WindowPadding = ImVec2(4, 4);
			ImGui::CustomSpacing(9.f);

			InsertCombo("Current Font", current_font, Fonts)


			style->ItemSpacing = ImVec2(0, 0);
			style->WindowPadding = ImVec2(6, 6);

		} InsertEndGroupBoxRight("Fonts Cover", "Fonts");

		InsertSpacer("Fonts - Other Spacer");

		InsertGroupBoxRight("Other", 332.f); {
			style->ItemSpacing = ImVec2(4, 2);
			style->WindowPadding = ImVec2(4, 4);
			ImGui::CustomSpacing(9.f);			
			InsertCheckbox("Watermark", watermark);

			InsertCheckbox("Show FPS", draw_fps);
			
			InsertCheckbox("Stream Proof", streamproof);
			InsertCheckbox("Fov Circle Outline", fov_circle_outline);
			InsertCheckbox("Crosshair Outline", crosshair_outline);
			InsertCheckbox("Dynamic Fov", dynamic_fov);
			InsertCheckbox("Crosshair", draw_crosshair);
			InsertSlider("Fov Thickness", fov_thick, 1, 10, "%1.f%");
			InsertSlider("Crosshair Size", CrosshairSize, 1, 30, "%1.f%");
			InsertSlider("Crosshair Spacing", CrosshairSpacing, 1, 30, "%1.f%");
			InsertSlider("Crosshair Thickness", CrosshairThickness, 1, 10, "%1.f%");

	
			

		} InsertEndGroupBoxRight("Other Cover", "Other");
	}
}

void Skins() {

	ImGuiStyle* style = &ImGui::GetStyle();
	InsertSpacer("Top Spacer");

	ImGui::Columns(2, NULL, false); {

		InsertGroupBoxLeft("Configs (soon)", 112.f); {
			style->ItemSpacing = ImVec2(4, 2);
			style->WindowPadding = ImVec2(4, 4);
			ImGui::CustomSpacing(9.f);

			style->Colors[ImGuiCol_Button] = ImColor(ImGuiCol_MenuTheme);
			style->Colors[ImGuiCol_ButtonHovered] = ImColor(10, 10, 10, 255);
			style->Colors[ImGuiCol_ButtonActive] = ImColor(15, 15, 15, 255);

		/*	ImGui::SetCursorPos(ImVec2(30, 15));
			if (ImGui::Button("Save Config", ImVec2(200, 30)))
			{

			}
			ImGui::SetCursorPos(ImVec2(30, 60));
			if (ImGui::Button("Load Config", ImVec2(200, 30)))
			{

			}*/


		} InsertEndGroupBoxLeft("Config Cover", "Config options");

		
	}

}

void Players() {

	ImGuiStyle* style = &ImGui::GetStyle();
	InsertSpacer("Top Spacer");

	ImGui::Columns(2, NULL, false); {

		InsertGroupBoxLeft("Exploits (ban risk)", 506.f); {

			style->ItemSpacing = ImVec2(4, 2);
			style->WindowPadding = ImVec2(4, 4);
			ImGui::CustomSpacing(9.f);
			InsertCheckbox("Enable Exploits", enable_exploits);

			if (enable_exploits)
			{ 
				InsertCheckbox("Player Fly", player_fly);
				InsertCheckbox("Aim In Air", AimWhileJumping);				
				InsertCheckbox("Shoot Through Walls", bullshit32);
				

				InsertCheckbox("First Person (shitty)", first_person);	
				InsertCheckbox("Fov Changer", fov_changer);
				InsertSlider("Fov Value", fov_changer_value, 1, 180, "%1.f%");
			}

			




		} InsertEndGroupBoxLeft("Exploits Cover", "Exploits (Bank risk)");
	}
	ImGui::NextColumn(); {

		

		InsertGroupBoxRight("Cheat Info", 156.f); {
			style->ItemSpacing = ImVec2(4, 2);
			style->WindowPadding = ImVec2(4, 4);
			ImGui::CustomSpacing(9.f);
			ImGui::PushFont(menuFont);

			ImGui::SetCursorPosX(10);
			ImGui::Text(SK("Username: %s"), KeyAuthApp.user_data.username);
			ImGui::SetCursorPosX(10);
			for (int i = 0; i < KeyAuthApp.user_data.subscriptions.size(); i++) {
				auto sub = KeyAuthApp.user_data.subscriptions.at(i);
				ImGui::Text("Key Expiry: %s", tm_to_readable_time(timet_to_tm(string_to_timet(sub.expiry))));
			}
			ImGui::SetCursorPosX(10);
			auto OnlineUsers = SK("Online Users: ").decrypt() + KeyAuthApp.app_data.numOnlineUsers;
			auto TotalUsers = SK("Total Users: ").decrypt() + KeyAuthApp.app_data.numUsers;
			ImGui::Text(SK("Cheat Version: %s"), KeyAuthApp.app_data.version);
			ImGui::SetCursorPosX(10);
			ImGui::Text(OnlineUsers.c_str());
			ImGui::SetCursorPosX(10);
			ImGui::Text(TotalUsers.c_str());
			auto EAC = SK("Eac Status: ").decrypt() + EAC_Status;
			auto BE = SK("Be Status: ").decrypt() + BE_Status;
			ImGui::SetCursorPosX(10);
			ImGui::Text(EAC.c_str());
			ImGui::SetCursorPosX(10);
			ImGui::Text(BE.c_str());

			style->ItemSpacing = ImVec2(0, 0);
			style->WindowPadding = ImVec2(6, 6);

		} InsertEndGroupBoxRight("Cheat Info Cover", "Cheat Info");

		InsertSpacer("User Info - Announcements Spacer");

		InsertGroupBoxRight("Announcements", 332.f); {
			style->ItemSpacing = ImVec2(4, 2);
			style->WindowPadding = ImVec2(4, 4);
			ImGui::CustomSpacing(9.f);

			for (const auto& chat : KeyAuthApp.response.channeldata) {
				std::string formattedTime = tm_to_readable_time(timet_to_tm(string_to_timet(chat.timestamp)));
				ImGui::PushFont(menuFont);
				ImGui::SetCursorPosX(10);
				ImGui::Text(SK("Dev: %s"), chat.author.c_str());
				ImGui::SetCursorPosX(10);
				ImGui::Text(SK("Message: %s"), chat.message.c_str());
				ImGui::SetCursorPosX(10);
				ImGui::Text(SK("Sent At: %s"), formattedTime.c_str());
			}


		} InsertEndGroupBoxRight("Announcements Cover", "Announcements");
	}
}

bool is_open = true;
void Render()
{
	ImGuiStyle* style = &ImGui::GetStyle();

	style->WindowPadding = ImVec2(6, 6);
	style->WindowRounding = 0;
	style->ScrollbarSize = 3.f;
	style->ScrollbarRounding = 2.f;

	ImGui::PushFont(menuFont);
	ImGui::StyleColorsDark(style);

	ImGui::SetNextWindowSize(ImVec2(660.f, 560.f));
	ImGui::BeginMenuBackground(SK("Fort Sense"), &is_open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar); {

		ImGui::BeginChild(SK("Complete Border"), ImVec2(648.f, 548.f), false); {
			auto pos = ImGui::GetWindowPos();
			ImGui::Image(menuBg, ImVec2(648.f, 548.f));

		} ImGui::EndChild();

		ImGui::SameLine(6.f);

		style->Colors[ImGuiCol_ChildBg] = ImColor(0, 0, 0, 0);

		ImGui::BeginChild(SK("Menu Contents"), ImVec2(648.f, 548.f), false); {

			ImGui::ColorBar(SK("Bar"), ImVec2(648.f, 2.f));

			style->ItemSpacing = ImVec2(0.f, -1.f);

			ImGui::BeginTabs(SK("Tabs"), ImVec2(75.f, 542.f), false); {

				style->ItemSpacing = ImVec2(0.f, 0.f);

				style->ButtonTextAlign = ImVec2(0.5f, 0.47f);

				ImGui::PopFont();
				ImGui::PushFont(tabFont);

				switch (tab) {
				case 0:
					ImGui::TabSpacer(SK("##Top Spacer"), ImVec2(75.f, 10.f));

					if (ImGui::SelectedTab(SK("A"), ImVec2(75.f, 75.f))) tab = 0;
					if (ImGui::Tab(SK("H"), ImVec2(75.f, 75.f))) tab = 1;
					if (ImGui::Tab(SK("D"), ImVec2(75.f, 75.f))) tab = 2;
					if (ImGui::Tab(SK("G"), ImVec2(75.f, 75.f))) tab = 3;
					if (ImGui::Tab(SK("I"), ImVec2(75.f, 75.f))) tab = 4;
					if (ImGui::Tab(SK("E"), ImVec2(75.f, 75.f))) tab = 5;
					ImGui::Tab(SK(""), ImVec2(75.f, 75.f));


					ImGui::TabSpacer2(SK("##Bottom Spacer"), ImVec2(75.f, 7.f));
					break;
				case 1:
					ImGui::TabSpacer(SK("##Top Spacer"), ImVec2(75.f, 10.f));

					if (ImGui::Tab(SK("A"), ImVec2(75.f, 75.f))) tab = 0;
					if (ImGui::SelectedTab(SK("H"), ImVec2(75.f, 75.f))) tab = 1;
					if (ImGui::Tab(SK("D"), ImVec2(75.f, 75.f))) tab = 2;
					if (ImGui::Tab(SK("G"), ImVec2(75.f, 75.f))) tab = 3;
					if (ImGui::Tab(SK("I"), ImVec2(75.f, 75.f))) tab = 4;
					if (ImGui::Tab(SK("E"), ImVec2(75.f, 75.f))) tab = 5;
					ImGui::Tab(SK(""), ImVec2(75.f, 75.f));

					ImGui::TabSpacer2(SK("##Bottom Spacer"), ImVec2(75.f, 7.f));
					break;
				case 2:
					ImGui::TabSpacer(SK("##Top Spacer"), ImVec2(75.f, 10.f));

					if (ImGui::Tab(SK("A"), ImVec2(75.f, 75.f))) tab = 0;
					if (ImGui::Tab(SK("H"), ImVec2(75.f, 75.f))) tab = 1;
					if (ImGui::SelectedTab(SK("D"), ImVec2(75.f, 75.f))) tab = 2;
					if (ImGui::Tab(SK("G"), ImVec2(75.f, 75.f))) tab = 3;
					if (ImGui::Tab(SK("I"), ImVec2(75.f, 75.f))) tab = 4;
					if (ImGui::Tab(SK("E"), ImVec2(75.f, 75.f))) tab = 5;
					ImGui::Tab(SK(""), ImVec2(75.f, 75.f));


					ImGui::TabSpacer2(SK("##Bottom Spacer"), ImVec2(75.f, 7.f));
					break;
				case 3:
					ImGui::TabSpacer(SK("##Top Spacer"), ImVec2(75.f, 10.f));

					if (ImGui::Tab(SK("A"), ImVec2(75.f, 75.f))) tab = 0;
					if (ImGui::Tab(SK("H"), ImVec2(75.f, 75.f))) tab = 1;
					if (ImGui::Tab(SK("D"), ImVec2(75.f, 75.f))) tab = 2;
					if (ImGui::SelectedTab(SK("G"), ImVec2(75.f, 75.f))) tab = 3;
					if (ImGui::Tab(SK("I"), ImVec2(75.f, 75.f))) tab = 4;
					if (ImGui::Tab(SK("E"), ImVec2(75.f, 75.f))) tab = 5;
					ImGui::Tab(SK(""), ImVec2(75.f, 75.f));

					ImGui::TabSpacer2(SK("##Bottom Spacer"), ImVec2(75.f, 7.f));
					break;
				case 4:
					ImGui::TabSpacer(SK("##Top Spacer"), ImVec2(75.f, 10.f));

					if (ImGui::Tab(SK("A"), ImVec2(75.f, 75.f))) tab = 0;
					if (ImGui::Tab(SK("H"), ImVec2(75.f, 75.f))) tab = 1;
					if (ImGui::Tab(SK("D"), ImVec2(75.f, 75.f))) tab = 2;
					if (ImGui::Tab(SK("G"), ImVec2(75.f, 75.f))) tab = 3;
					if (ImGui::SelectedTab(SK("I"), ImVec2(75.f, 75.f))) tab = 4;
					if (ImGui::Tab(SK("E"), ImVec2(75.f, 75.f))) tab = 5;
					ImGui::Tab(SK(""), ImVec2(75.f, 75.f));

					ImGui::TabSpacer2(SK("##Bottom Spacer"), ImVec2(75.f, 7.f));
					break;
				case 5:
					ImGui::TabSpacer(SK("##Top Spacer"), ImVec2(75.f, 10.f));

					if (ImGui::Tab(SK("A"), ImVec2(75.f, 75.f))) tab = 0;
					if (ImGui::Tab(SK("H"), ImVec2(75.f, 75.f))) tab = 1;
					if (ImGui::Tab(SK("D"), ImVec2(75.f, 75.f))) tab = 2;
					if (ImGui::Tab(SK("G"), ImVec2(75.f, 75.f))) tab = 3;
					if (ImGui::Tab(SK("I"), ImVec2(75.f, 75.f))) tab = 4;
					if (ImGui::SelectedTab(SK("E"),  ImVec2(75.f, 75.f))) tab = 5;
					ImGui::Tab(SK(""),  ImVec2(75.f, 75.f));

					ImGui::TabSpacer2(SK("##Bottom Spacer"), ImVec2(75.f, 7.f));
					break;
			
				}

				ImGui::PopFont();
				ImGui::PushFont(menuFont);

				style->ButtonTextAlign = ImVec2(0.5f, 0.5f);

			} ImGui::EndTabs();

			ImGui::SameLine(75.f);

			ImGui::BeginChild(SK("Tab Contents"), ImVec2(572.f, 542.f), false); {

				style->Colors[ImGuiCol_Border] = ImColor(0, 0, 0, 0);

				switch (tab) {

				case 0:
					Aimbot();
					break;
				case 1:
					Legit();
					break;
				case 2:
					Visuals();
					break;
				case 3:
					Skins();
					break;
				case 4:
					Misc();
					break;
				case 5:
					Players();
					break;	
				}

				style->Colors[ImGuiCol_Border] = ImColor(10, 10, 10, 255);

			} ImGui::EndChild();

			style->ItemSpacing = ImVec2(4.f, 4.f);
			style->Colors[ImGuiCol_ChildBg] = ImColor(17, 17, 17, 255);

		} ImGui::EndChild();

		ImGui::PopFont();

	} ImGui::End();
}

void Shutdown() {

	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
}


void CreateStyle()
{
	auto& style = ImGui::GetStyle();
	auto& color = style.Colors;
	style.FrameRounding = 0;
	style.WindowBorderSize = 0;
	style.ScrollbarRounding = 0;
	style.ScrollbarSize = 5;
}

inline void DrawMenu() {

	if (GetAsyncKeyState_Spoofed(VK_INSERT))
	{
		menu_open = !menu_open;
		Sleep(300);
	}

	RECT screen_rect;
	::GetWindowRect(::GetDesktopWindow(), &screen_rect);

	ImGui::PushFont(NoramlFOnt);

	CreateStyle();

	if (menu_open)
	{
		Render();

	}
	else
	{
	}
}


#include "menu/MenuFonts.h"
#include "MenuBackground.h"

__forceinline VOID directx_init()
{
	SPOOF_FUNC
		if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &DirectX9Interface::p_object)))
			exit(3);

	ZeroMemory(&DirectX9Interface::p_params, sizeof(DirectX9Interface::p_params));
	DirectX9Interface::p_params.Windowed = TRUE;
	DirectX9Interface::p_params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	DirectX9Interface::p_params.hDeviceWindow = DirectX9Interface::hijacked_hwnd;
	DirectX9Interface::p_params.MultiSampleQuality = D3DMULTISAMPLE_NONE;
	DirectX9Interface::p_params.BackBufferFormat = D3DFMT_A8R8G8B8;
	DirectX9Interface::p_params.BackBufferWidth = screen_width;
	DirectX9Interface::p_params.BackBufferHeight = screen_height;
	DirectX9Interface::p_params.EnableAutoDepthStencil = TRUE;
	DirectX9Interface::p_params.AutoDepthStencilFormat = D3DFMT_D16;
	if (v_sync)
	{
		DirectX9Interface::p_params.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	}
	else
	{
		DirectX9Interface::p_params.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	}
	if (FAILED(DirectX9Interface::p_object->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, DirectX9Interface::hijacked_hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &DirectX9Interface::p_params, 0, &DirectX9Interface::p_device)))
	{
		DirectX9Interface::p_object->Release();
		Sleep(1000);
		exit(1);
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGui_ImplWin32_Init(DirectX9Interface::hijacked_hwnd);
	ImGui_ImplDX9_Init(DirectX9Interface::p_device);
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	static const ImWchar icons_ranges[] = { 0xf000, 0xf5ff, 0 };
	ImFontConfig icons_config;

	io.IniFilename = nullptr;
	io.LogFilename = nullptr;
	ImFontConfig config;

	icons_config.MergeMode = true;
	icons_config.PixelSnapH = true;
	icons_config.OversampleH = 1;
	icons_config.OversampleV = 1;
	ImFontConfig CustomFont;
	CustomFont.FontDataOwnedByAtlas = false;

	if (menuBg == nullptr)
		D3DXCreateTextureFromFileInMemoryEx(DirectX9Interface::p_device, &menuBackground, sizeof(menuBackground), 1000, 1000, D3DUSAGE_DYNAMIC, 0, D3DFMT_X8B8G8R8, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &menuBg);

	D3DXCreateTextureFromFileEx(DirectX9Interface::p_device, SK("C:\\pic.png"), 1000, 1000, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &custom_image);


	io.Fonts->AddFontFromFileTTF(SK("C:\\Windows\\Fonts\\Verdana.ttf"), 13, nullptr, io.Fonts->GetGlyphRangesDefault());
	FortniteFont = io.Fonts->AddFontFromMemoryTTF(burbank_bold, sizeof(burbank_bold), 13, NULL, io.Fonts->GetGlyphRangesCyrillic());
	Arial = io.Fonts->AddFontFromFileTTF(SK("C:\\Windows\\Fonts\\Arial.ttf"), 13);
	Verdana = io.Fonts->AddFontFromFileTTF(SK("C:\\Windows\\Fonts\\Verdana.ttf"), 13);
	proggy = io.Fonts->AddFontFromMemoryTTF(proggyclean, sizeof(proggyclean), 13);
	roboto = io.Fonts->AddFontFromMemoryTTF(&roboto1, sizeof roboto1, 13, NULL, io.Fonts->GetGlyphRangesCyrillic());
	poppins = io.Fonts->AddFontFromMemoryTTF(poppins1, sizeof(poppins1), 13);
	WeaponFont = io.Fonts->AddFontFromMemoryTTF(astriumwep, sizeof(astriumwep), 20, NULL, io.Fonts->GetGlyphRangesCyrillic());
	menuFont = io.Fonts->AddFontFromMemoryCompressedTTF(verdana_compressed_data, verdana_compressed_size, 12, NULL, io.Fonts->GetGlyphRangesCyrillic());
	tabFont = io.Fonts->AddFontFromMemoryCompressedTTF(cherryfont_compressed_data, cherryfont_compressed_size, 40, NULL, io.Fonts->GetGlyphRangesCyrillic());
	controlFont = io.Fonts->AddFontFromMemoryCompressedTTF(comboarrow_compressed_data, comboarrow_compressed_size, 12, NULL, io.Fonts->GetGlyphRangesCyrillic());
	boldMenuFont = io.Fonts->AddFontFromMemoryCompressedTTF(verdanab_compressed_data, verdanab_compressed_size, 11, NULL, io.Fonts->GetGlyphRangesCyrillic());
	legit_icons = io.Fonts->AddFontFromFileTTF(SK("C:\\test_font.ttf"), 30);
	test_font = io.Fonts->AddFontFromFileTTF(SK("C:\\FastHand-lgBMV.ttf"), 13);
	watermark_font = io.Fonts->AddFontFromFileTTF(SK("C:\\smallest_pixel-7.ttf"), 13);

	DirectX9Interface::p_object->Release();
}

VOID render_loop()
{
	SPOOF_FUNC;
	static RECT old_rc;
	RtlSecureZeroMemory(&DirectX9Interface::message, sizeof(MSG));
	while (DirectX9Interface::message.message != WM_QUIT)
	{
		if (PeekMessageA(&DirectX9Interface::message, DirectX9Interface::hijacked_hwnd, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&DirectX9Interface::message);
			DispatchMessageA(&DirectX9Interface::message);
		}

		RECT rc;
		POINT xy;
		RtlSecureZeroMemory(&rc, sizeof(RECT));
		RtlSecureZeroMemory(&xy, sizeof(POINT));
		GetClientRect(DirectX9Interface::game_wnd, &rc);
		ClientToScreen(DirectX9Interface::game_wnd, &xy);
		rc.left = xy.x;
		rc.top = xy.y;

		ImGuiIO& io = ImGui::GetIO();
		io.ImeWindowHandle = DirectX9Interface::game_wnd;
		screen_width = io.DisplaySize.x;
		screen_height = io.DisplaySize.y;
		center_x = io.DisplaySize.x / 2;
		center_y = io.DisplaySize.y / 2;

		POINT p;
		GetCursorPosA(&p);
		long styleProc = GetWindowLongA_Spoofed(DirectX9Interface::hijacked_hwnd, GWL_EXSTYLE);
		io.MousePos.x = p.x - xy.x;
		io.MousePos.y = p.y - xy.y;
		if (menu_open)
		{
			styleProc &= ~WS_EX_LAYERED;
			SetWindowLongA_Spoofed(DirectX9Interface::hijacked_hwnd,
				GWL_EXSTYLE, styleProc);
			SetForegroundWindow(DirectX9Interface::hijacked_hwnd);
		}
		else
		{
			styleProc |= WS_EX_LAYERED;
			SetWindowLong(DirectX9Interface::hijacked_hwnd,
				GWL_EXSTYLE, styleProc);
		}
		if (GetAsyncKeyState_Spoofed(0x1))
		{
			io.MouseDown[0] = true;
			io.MouseClicked[0] = true;
			io.MouseClickedPos[0].x = io.MousePos.x;
			io.MouseClickedPos[0].x = io.MousePos.y;
		}
		else
		{
			io.MouseDown[0] = false;
		}
		if (rc.left != old_rc.left || rc.right != old_rc.right || rc.top != old_rc.top || rc.bottom != old_rc.bottom)
		{
			old_rc = rc;
			DirectX9Interface::p_params.BackBufferWidth = screen_width;
			DirectX9Interface::p_params.BackBufferHeight = screen_height;
			SetWindowPos_Spoofed(DirectX9Interface::hijacked_hwnd, (HWND)0, xy.x, xy.y, screen_width, screen_height, SWP_NOREDRAW);
			DirectX9Interface::p_device->Reset(&DirectX9Interface::p_params);
		}

		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();


		

		ImDrawList* draw_list = ImGui::GetForegroundDrawList();
		auto RGB = ImGui::GetColorU32({ color_red, color_green, color_blue, 255 });


		int dynamicfov;
		if (dynamic_fov)
		{
			dynamicfov = dynamicfovval - (80 + fov);
		}
		else
		{
			dynamicfov = fov;
		}

		if (draw_fov) {

			if (rainbow)
			{
				if (fov_circle_outline)
				{
					draw_list->AddCircle(ImVec2(screen_width / 2, screen_height / 2), dynamicfov, ImColor(RGB), 33, fov_thick + 3.5);
				}
				draw_list->AddCircle(ImVec2(screen_width / 2, screen_height / 2), dynamicfov + 1, ImColor(20, 0, 0, 255), 33, fov_thick);
			}
			else
			{
				if (fov_circle_outline)
				{
					draw_list->AddCircle(ImVec2(screen_width / 2, screen_height / 2), dynamicfov, ImColor(0, 0, 0, 255), 100, fov_thick + 3.5);
				}
				draw_list->AddCircle(ImVec2(screen_width / 2, screen_height / 2), dynamicfov, ImColor(ImVec4(PlayerColor::fov_col[0], PlayerColor::fov_col[1], PlayerColor::fov_col[2], 1.0f)), 100, fov_thick);


			}
		}

		if (draw_crosshair)
		{

			ImVec2 Center = ImVec2(screen_width / 2, screen_height / 2);

			if (crosshair_outline)
			{
				draw_list->AddLine(ImVec2(Center.x, Center.y - CrosshairSize - CrosshairSpacing), ImVec2(Center.x, Center.y - CrosshairSpacing), ImColor(0, 0, 0), CrosshairThickness + 3.5);
				draw_list->AddLine(ImVec2(Center.x, Center.y + CrosshairSpacing), ImVec2(Center.x, Center.y + CrosshairSize + CrosshairSpacing), ImColor(0, 0, 0), CrosshairThickness + 3.5);
				draw_list->AddLine(ImVec2(Center.x - CrosshairSize - CrosshairSpacing, Center.y), ImVec2(Center.x - CrosshairSpacing, Center.y), ImColor(0, 0, 0), CrosshairThickness + 3.5);
				draw_list->AddLine(ImVec2(Center.x + CrosshairSpacing, Center.y), ImVec2(Center.x + CrosshairSize + CrosshairSpacing, Center.y), ImColor(0, 0, 0), CrosshairThickness + 3.5);
			}

			draw_list->AddLine(ImVec2(Center.x, Center.y - CrosshairSize - CrosshairSpacing), ImVec2(Center.x, Center.y - CrosshairSpacing), ImColor(ImVec4(PlayerColor::Crosshair_color[0], PlayerColor::Crosshair_color[1], PlayerColor::Crosshair_color[2], 1.0f)), CrosshairThickness);
			draw_list->AddLine(ImVec2(Center.x, Center.y + CrosshairSpacing), ImVec2(Center.x, Center.y + CrosshairSize + CrosshairSpacing), ImColor(ImVec4(PlayerColor::Crosshair_color[0], PlayerColor::Crosshair_color[1], PlayerColor::Crosshair_color[2], 1.0f)), CrosshairThickness);
			draw_list->AddLine(ImVec2(Center.x - CrosshairSize - CrosshairSpacing, Center.y), ImVec2(Center.x - CrosshairSpacing, Center.y), ImColor(ImVec4(PlayerColor::Crosshair_color[0], PlayerColor::Crosshair_color[1], PlayerColor::Crosshair_color[2], 1.0f)), CrosshairThickness);
			draw_list->AddLine(ImVec2(Center.x + CrosshairSpacing, Center.y), ImVec2(Center.x + CrosshairSize + CrosshairSpacing, Center.y), ImColor(ImVec4(PlayerColor::Crosshair_color[0], PlayerColor::Crosshair_color[1], PlayerColor::Crosshair_color[2], 1.0f)), CrosshairThickness);
		}


		ImGui::PushFont(watermark_font);

		if (watermark)
		{

			if (draw_fps)
			{
				char fpsinfo[256];
				sprintf_s(fpsinfo, SK(" | fps: %.0f"), ImGui::GetIO().Framerate);

				DrawString(13, 15, 15, ImColor(255, 255, 255), false, true, SK("fort"));
				DrawString(13, 35, 15, ImColor(150, 200, 60), false, true, SK("sense"));
				DrawString(13, 65, 15, ImColor(255, 255, 255), false, true, fpsinfo);

			}
			else
			{
			
					DrawString(13, 15, 15, ImColor(255, 255, 255), false, true, SK("fort"));
				DrawString(13, 35, 15, ImColor(150, 200, 60), false, true, SK("sense"));
			}
		}



		ColorChange();
		gaybow;
		fortnite_radar(radar_position_x, radar_position_y, radar_size, false);

		actor_loop();

		floot->LevelDrawing();

		DrawMenu();
		if (!streamproof_once && streamproof) {
			hide_overlay();
			streamproof_once = true;
		}

		if (streamproof_once && !streamproof) {
			show_overlay();
			streamproof_once = false;
		}

		if (GetAsyncKeyState(VK_DELETE))
		{
			quit_cheat = true;
			
			

		}

		

		ImGui::EndFrame();

		DirectX9Interface::p_device->SetRenderState(D3DRS_ZENABLE, false);
		DirectX9Interface::p_device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
		DirectX9Interface::p_device->SetRenderState(D3DRS_SCISSORTESTENABLE, false);
		DirectX9Interface::p_device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
		if (DirectX9Interface::p_device->BeginScene() >= 0)
		{
			ImGui::Render();
			ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
			DirectX9Interface::p_device->EndScene();
		}
		HRESULT result = DirectX9Interface::p_device->Present(NULL, NULL, NULL, NULL);
		if (result == D3DERR_DEVICELOST && DirectX9Interface::p_device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		{
			ImGui_ImplDX9_InvalidateDeviceObjects();
			DirectX9Interface::p_device->Reset(&DirectX9Interface::p_params);
			ImGui_ImplDX9_CreateDeviceObjects();
		}

	}
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	if (DirectX9Interface::p_device != NULL)
	{
		DirectX9Interface::p_device->EndScene();
		DirectX9Interface::p_device->Release();
	}
	if (DirectX9Interface::p_object != NULL)
	{
		DirectX9Interface::p_object->Release();
	}
	DestroyWindow(DirectX9Interface::hijacked_hwnd);
}