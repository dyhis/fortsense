#pragma once
inline static const char* CurrentWeaponCfg = "Rifle";
inline const char* WeaponCfgs[] = { "Rifle", "Shotgun", "Smg", "Pistol", "Sniper" };

inline int norm_hitbox = 0;
inline const char* norm_Hitboxes[] = { "Head", "Neck", "Chest", "Pelvis" };

inline int CurrentHitbox = 0;
inline const char* Hitboxes[] = { "Head", "Neck", "Chest", "Pelvis", "Random", "Closest" };

inline int aimbot_type = 0;
inline const char* aimbottype[] = { "Memory", "Mouse", "Mouse 2 (prob dtc)" };

inline int CurrentHitbox1 = 0;
inline const char* Hitboxes1[] = { "Head", "Neck", "Chest", "Pelvis", "Random", "Closest" };

inline int CurrentCloseHitbox = 0;
inline const char* CloseHitboxes[] = { "Head", "Neck", "Chest", "Pelvis" };

inline int RifleCurrentHitbox = 1;
inline const char* RifleHitboxes[] = { "Head", "Neck", "Chest", "Pelvis" };
inline int ShotgunCurrentHitbox = 0;
inline const char* ShotgunHitboxes[] = { "Head", "Neck", "Chest", "Pelvis" };
inline int SmgCurrentHitbox = 1;
inline const char* SmgHitboxes[] = { "Head", "Neck", "Chest", "Pelvis" };
inline int PistolCurrentHitbox = 1;
inline const char* PistolHitboxes[] = { "Head", "Neck", "Chest", "Pelvis" };
inline int SniperCurrentHitbox = 1;
inline const char* SniperHitboxes[] = { "Head", "Neck", "Chest", "Pelvis" };
inline const char* SnapLines[] = { "Bottom", "Middle", "Top"};
inline const char* Boxes[] = { "2D", "Corner", "3D", "Rounded"};
inline const char* Fonts[] = { "Default","Proggy", "Verdana", "Fortnite", "Roboto" , "Arial",  "Poppins" };


namespace PlayerColor {
	float BoxVisible[4] = { 0, 1, 0.3, 1.0f };
	float BoxNotVisible[4] = { 0, 0.8, 1, 1.0f };
	
	float targetcol[4] = { 1.f, 0, 0, 1.0f };

	float visible_color[4] = { 0.7f, 1, 0, 1.0f };
	float nonvisible_color[4] = { 0, 0.6, 1, 1.0f };

	float accentcolor[4] = { 255.0f, 255.0f, 255.0f, 1.0f };

	float SkeletonVisible[4] = { 0, 1, 0.3, 1.0f };
	float SkeletonNotVisible[4] =  { 0, 0.8, 1, 1.0f };

	float LineVisible[4] = { 0, 1, 0.3, 1.0f };
	float LineNotVisible[4] = { 0, 0.8, 1, 1.0f };


	float SnaplineVisible[4];
	float SnaplineNotVisible[4];

	float TopTextVisible[4] = { 0, 1, 0.3, 1.0f };
	float TopTextNotVisible[4] =  { 0, 0.8, 1, 1.0f };

	float BottomTextVisible[4] = { 0, 1, 0.3, 1.0f };
	float BottomTextNotVisible[4] =  { 0, 0.8, 1, 1.0f };


	float RadarVisible[4] = { 0, 1, 0.3, 1.0f };
	float RadarNotVisible[4] =  { 0, 0.8, 1, 1.0f };

	float Crosshair_color[4] = { 1.f, 1, 1, 1.0f };
	float fov_col[4] = { 1.f, 1, 1, 1.0f };
	float target_line[4] = { 0, 1, 0, 1.0f };
	float target_line_not_visible[4] = { 1.f, 0, 0, 1.0f };

}

inline int current_font = 0;

//ImColor AccentCol = ImVec4(PlayerColor::accentcolor[0], PlayerColor::accentcolor[1], PlayerColor::accentcolor[2], 1.0f);
int aimkey;
static int menu_animation = 0;
static int border_animation = 0;
static int menu_slide = 0;
inline bool menu_open = true;
inline bool debug = false;
inline bool v_sync = true;
inline bool aimbot1 = false;
inline bool aimbot = false;
inline bool aimbo3t = false;
inline bool exploit12 = false;
inline float smoothing = 15;
inline bool build_chehck = true;
inline bool no_downed = true;
inline bool distance_check = true;
inline bool aim_vis_cveck = true;

inline bool humanization = false;
inline float smoothingx = 15;
inline float smoothingy = 15;

inline float smoothingx1 = 15;
inline float smoothingy1 = 15;

inline int smoothing5 = 8;
inline int target_bone = 0;
inline float fov = 250;
inline float max_trig_distance = 30;
inline float trig_delay = 1;
inline float fov1 = 250;

inline float fov32 = 90;
inline bool draw_fov = false;

inline bool draw_fov1 = false;

inline bool dynamic_fov = true;

inline float max_aimbot_distance = 250;
inline bool triggerbot = false;
inline bool shotgun_only = false;

inline bool targetline = false;
inline bool exploit32 = true;
inline bool targettex = false;
inline bool memory = false;
inline bool Silent  = false;
inline bool targetcolor = false;

inline bool prediction = false;
inline bool prediction1 = false;

inline bool snapline_bottom = false;
inline bool snapline_middle = false;
inline int snap = 0;

inline bool enable_snap = false;

inline bool snapline_top = false;
inline bool streamproof = false;
inline bool background = true;
inline bool quit_cheat = false;
inline bool exit_cheat = false;

inline bool streamproof_once = false;
inline bool targetcirlce = false;
inline bool HeadBone = false;
inline bool NeckBbone = false;
inline bool ChestBone = false;
inline bool PelvisBone = false;
inline bool weaponconfigs = false;

inline bool RBUTTOM = false;
inline bool LBUTTOM = false;
inline bool CAPS = false;
inline bool SHIFT = false;
inline int aimdelay = 0;

inline int norm_fov = 8;
inline int norm_smooth = 19;

inline int smoothness1 = 13;
inline int smoothness2 = 6;
inline int smoothness3 = 11;
inline int smoothness4 = 12;
inline int smoothness5 = 4;

inline int fov2 = 9;
inline int fov3 = 10;
inline int fov4 = 11;
inline int fov5 = 8;

inline int real_hitbox;
inline int real_smooth;



inline bool CRFOV = false;
inline int distance69 = 10;
inline bool close_distance = false;
inline int close_fov = 13;
inline float close_smooth = 4;
inline int close_hitbox = 0;
inline int AimSpeedCloseRange = 14;
inline bool radar = false;
inline bool mini_map_radar = false;

inline bool esp = false;
inline bool box_esp = false;
inline bool rainbow = false;
//inline bool customcolor = false;
inline bool box_outline = false;
inline bool skeleton_outline = false;
inline bool text_outline = false;
inline bool line_outline = false;

inline float font_size = 13;
inline bool box_round = false;
inline bool line = false;
inline bool Reloading = false;
inline bool line1 = false;
inline bool enable_exploits = false;

inline bool SmallPlayer = false;
inline bool player_fly = false;
inline bool bigplayers = false;
inline bool airstuck = false;
inline bool infinitefuel = false;
inline bool instareload = false;
inline bool fovcanger = false;
inline bool AimWhileJumping = false;
inline bool bullshit32 = false;
inline bool bullshit33 = false;

inline bool aspectest = true;
inline bool first_person = false;
inline bool ammo_changer = false;
inline bool wire_frame = false;
inline bool fov_changer = false;
inline float fov_changer_value = 90;
inline bool player_teleport = false;

inline bool norecoiul = false;
inline bool boldspeed = false;

inline bool enianr = true;
inline bool spinbot = false;
inline bool carFly = false;
float skelthick = 2;
float line_thick = 2;

float headcirclesize = 15.0f;

float Linethick = 2.0;
float Playersize = 1.0;
float Playersize1 = 3;
inline bool line2 = false;
inline bool viewangle = false;
inline bool skeleton = true;
inline bool bChams = false;
inline bool headesp = false;
inline bool hand = false;
inline bool head = false;
inline bool feet = false;
inline bool draw_username = true;
inline bool draw_platform = false;
inline bool draw_distance = true;
inline bool always_spotted = true;

inline bool weapon_rarity = false;

inline bool draw_distance1 = false;
inline bool Outline = false;
inline bool draw_held_weapon = true;
inline bool image_esp = false;

inline bool ammo = true;
inline bool testdbg = true;
inline bool loot_boxes = false;

inline bool show_chests = false;
inline bool show_mcnugget = false;

inline bool show_ammo_box = false;
inline bool loot_snapline = false;
inline bool show_traps = false;
inline bool show_fishing_rod_barrels = false;
inline bool show_campfires = false;
inline bool show_llamas = false;
inline bool show_pickups = false;
inline bool show_common = false;
inline bool show_uncommon = false;
inline bool show_rare = false;
inline bool show_epic = false;
inline bool show_legendary = false;
inline bool show_mythic = false;
inline bool show_vehicles = false;
inline bool show_cooler = false;
inline bool food_stuff = false;
inline bool mod_station = false;
inline bool enable_cache = true;
inline bool bullshit = false;
bool no_spread = false; bool no_spread_old = false;

inline bool Gold = false;

inline bool developer = false;

inline float level_render_distance = 100.0f;
inline float llama_render_distance = 300.0f;
inline float vehicle_render_distance = 100.0f;
inline float cooler_render_distance = 70.0f;
inline float food_render_distance = 70.0f;
inline float mod_render_distance = 70.0f;

inline float pickup_render_distance = 70.0f;
inline float mcnugget_render_distance = 70.0f;

inline float gold_render_distance = 70.0f;

inline float chest_render_distance = 70.0f;
inline float ammo_render_distance = 70.0f;
inline bool Common = 1;

inline bool rare = 1;

inline bool purple = 1;

inline bool gold = 1;

inline bool mythic = 1;

inline bool UnCommon = 1;

inline bool vehicles = true;
inline bool loot = true;

inline int box_type = 0;
inline bool threed;
inline bool Corner;
inline float box_thickness = 1.955f;
inline float box_outline_thickness = 2.5;
inline float max_esp_distance = 500;
inline bool fov_circle_outline = true;
inline bool crosshair_outline = true;
inline bool watermark = true;

inline bool draw_crosshair = false;
inline bool draw_fps = false;
inline float cache_sleep = 0.5;
inline float Resx = 1920;
inline float Resy = 1080;

inline float fov_thick = 2;
 float CrosshairSize = 10.0f;
 float CrosshairThickness = 1.0f;
 float CrosshairSpacing = 2.0f; // Adjust as needed

inline bool rifle_aimbot = false;
inline bool rifle_memory = false;
inline bool rifle_draw_fov = false;
inline bool rifle_triggerbot = false;
inline bool rifle_targetline = false;
inline bool rifle_targetcirlce = false;
inline bool rifle_targettex = false;
inline bool rifle_targetname = false;

inline float rifle_smoothingx = 15;
inline float rifle_smoothingy = 15;

inline float rifle_fov = 250;
inline float rifle_max = 250;

inline bool rifle_HeadBone = false;
inline bool rifle_ChestBone = false;
inline bool rifle_Pelvisbone = false;
inline float rifle_max_trig_distance = 30;
inline float rifle_trig_delay = 1;
inline bool rifle_NeckBbone = false;
inline bool rifle_RBUTTOM = false;
inline bool rifle_LBUTTOM = false;
inline bool rifle_CAPS = false;
inline bool rifle_SHIFT = false;
inline bool rifle_Humanization = false;
inline bool rifle_SkipNocked = false;
inline bool rifle_DistanceCheck = false;

// For SMG
inline bool smg_aimbot = false;
inline bool smg_memory = false;
inline bool smg_draw_fov = false;
inline bool smg_triggerbot = false;
inline bool smg_targetline = false;
inline bool smg_targetcirlce = false;
inline bool smg_targettex = false;
inline float smg_smoothingx = 15;
inline bool smg_Humanization = false;
inline bool smg_SkipNocked = false;
inline bool smg_DistanceCheck = false;

inline float smg_smoothingy = 15;
inline float smg_fov = 250;
inline float smg_max = 250;
inline float smg_max_trig_distance = 30;
inline float smg_trig_delay = 1;
inline bool smg_HeadBone = false;
inline bool smg_ChestBone = false;
inline bool smg_NeckBbone = false;
inline bool smg_Pelvisbone = false;
inline bool smg_RBUTTOM = false;
inline bool smg_LBUTTOM = false;
inline bool smg_CAPS = false;
inline bool smg_SHIFT = false;

// For Shotgun
inline bool shotgun_aimbot = false;
inline bool shotgun_memory = false;
inline bool shotgun_draw_fov = false;
inline bool shotgun_triggerbot = false;
inline bool shotgun_targetline = false;
inline bool shotgun_targetcirlce = false;
inline bool shotgun_targettex = false;
inline bool shotgun_targename = false;
inline float shotgun_max_trig_distance = 30;
inline float shotgun_trig_delay = 1;
inline bool shotgun_Humanization = false;
inline bool shotgun_SkipNocked = false;
inline bool shotgun_DistanceCheck = false;

inline float shotgun_smoothingx = 15;
inline float shotgun_smoothingy = 15;


inline float shotgun_max = 250;

inline float shotgun_fov = 250;
inline bool shotgun_HeadBone = false;
inline bool shotgun_ChestBone = false;
inline bool shotgun_Pelvisbone = false;

inline bool shotgun_NeckBbone = false;
inline bool shotgun_RBUTTOM = false;
inline bool shotgun_LBUTTOM = false;
inline bool shotgun_CAPS = false;
inline bool shotgun_SHIFT = false;

// For Sniper
inline bool sniper_aimbot = false;
inline bool sniper_memory = false;
inline bool sniper_draw_fov = false;
inline bool sniper_triggerbot = false;
inline bool sniper_targetline = false;
inline bool sniper_targetcirlce = false;
inline bool sniper_targettex = false;
inline float sniper_smoothingx = 15;
inline float sniper_smoothingy = 15;
inline float sniper_max = 250;
inline float sniper_max_trig_distance = 30;
inline float sniper_trig_delay = 1;
inline float sniper_fov = 250;
inline bool sniper_HeadBone = false;
inline bool sniper_ChestBone = false;
inline bool sniper_Pelvisbone = false;
inline bool sniper_NeckBbone = false;
inline bool sniper_RBUTTOM = false;
inline bool sniper_LBUTTOM = false;
inline bool sniper_CAPS = false;
inline bool sniper_SHIFT = false;
inline bool sniper_Humanization = false;
inline bool sniper_SkipNocked = false;
inline bool sniper_DistanceCheck = false;