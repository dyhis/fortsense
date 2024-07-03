#pragma once
#pragma comment(lib, "libcurl.lib")
#include <mutex>
#include <vector>
#include "driver.hpp"
#pragma once
#include "SKD.h"
#include "settings.h"
#include "defs.h"
#include "imports.h"


#define u uintptr_t
class aactor;
class aplayerstate;
template<class T>
class tarray
{
public:
	int Length() const
	{
		SPOOF_FUNC
			return m_nCount;
	}

	bool IsValid() const
	{
		SPOOF_FUNC
			if (m_nCount > m_nMax)
				return false;
		if (!m_Data)
			return false;
		return true;
	}

	uint64_t GetAddress() const
	{
		SPOOF_FUNC
			return m_Data;
	}
	T operator[](int i) {
		SPOOF_FUNC
			return request->read<T>(m_Data + i * sizeof(T));
	};
protected:
	uint64_t m_Data;
	uint32_t m_nCount;
	uint32_t m_nMax;
};



class aplayercameramanager {
public:
	camera_position_s get_camera_cache() const {
		SPOOF_FUNC
			if (!(uintptr_t)this) return {};
		return request->read<camera_position_s>((uintptr_t)this + 0x1310 + 0x10);
	}
};
class uschenecomponent {
public:
	vector3 get_relative_location() const {
		SPOOF_FUNC
			if (!(u)this) return {};
		return request->read<vector3>((u)this + 0x120);
	}
	vector3 get_relative_rotation() const {
		SPOOF_FUNC
			if (!(u)this) return {};
		return request->read<vector3>((u)this + 0x138);
	}
	vector3 ComponentVelocity() const {
		SPOOF_FUNC
			if (!(u)this) return {};
		return request->read<vector3>((u)this + 0x168);
	}
};
class uskeletalmeshcomponent {
public:
	vector3 get_bone_3d(int id) const {
		SPOOF_FUNC
			if (!(uintptr_t)this) return {};


		uintptr_t bone_array = request->read<uintptr_t>((u)this + 0x5B8);
		if (bone_array == NULL) bone_array = request->read<uintptr_t>((u)this + 0x5B8 + 0x10);
		FTransform bone = request->read<FTransform>(bone_array + (id * 0x60));
		FTransform component_to_world = request->read<FTransform>((u)this + 0x1c0);
		D3DMATRIX matrix = MatrixMultiplication(bone.ToMatrixWithScale(), component_to_world.ToMatrixWithScale());
		return vector3(matrix._41, matrix._42, matrix._43);


	}

	/*vector3 get_bone_3d(int id) const {
		if (!(uintptr_t)this) return {};

		int is_cached = request->read<int>((u)this + 0x600);
		auto bone_transform = request->read<FTransform>(request->read<u>((u)this + 0x10 * is_cached + 0x5B8) + 0x60 * id);

		FTransform ComponentToWorld = request->read<FTransform>((u)this + 0x1c0);

		D3DMATRIX Matrix = {};
		Matrix = MatrixMultiplication(bone_transform.ToMatrixWithScale(), ComponentToWorld.ToMatrixWithScale());

		return vector3(Matrix._41, Matrix._42, Matrix._43);
	}*/

	vector3 get_head() const {
		SPOOF_FUNC
			if (!(uintptr_t)this) return {};
		return get_bone_3d(110);
	}
	uintptr_t GetWireframeBit()
	{
		SPOOF_FUNC
			if (!(u)this) return 0;
		return request->read<uintptr_t>((u)this + 0x7c6);
	}

	vector3 get_base() const {
		SPOOF_FUNC
			if (!(uintptr_t)this) return {};
		return get_bone_3d(0);
	}
	float get_last_submit_time() const {
		SPOOF_FUNC
			if (!(uintptr_t)this) return 0;
		return request->read<float>((u)this + 0x2E8);
	}
	float get_last_render_time() const {
		SPOOF_FUNC
			if (!(uintptr_t)this) return 0;
		return request->read<float>((u)this + 0x2F0);
	}
	bool test_visible() const {
		SPOOF_FUNC
			return get_last_submit_time() == get_last_render_time();

	}
	bool visible() const {
		SPOOF_FUNC
			float fLastSubmitTime = get_last_submit_time();
		float fLastRenderTimeOnScreen = get_last_render_time();
		const float fVisionTick = 0.06f;
		bool bVisible = fLastRenderTimeOnScreen + fVisionTick >= fLastSubmitTime;
		return bVisible;
	}

};


class aactor {
public:

	uschenecomponent* get_root_component() const {
		SPOOF_FUNC
			if (!(u)this) return 0;
			return request->read<uschenecomponent*>((u)this + 0x198);
		}
	aplayerstate* get_player_state() {
		SPOOF_FUNC
			if (!(u)this) return 0;
		return request->read<aplayerstate*>((u)this + 0x2b0);
	}
	uskeletalmeshcomponent* get_mesh() const {
		SPOOF_FUNC
			if (!(u)this) return 0;
		return request->read<uskeletalmeshcomponent*>((u)this + 0x318);
	}






};
class aplayerstate {
public:
	aactor* get_pawn_private() const {
		SPOOF_FUNC
			if (!(u)this) return 0;
		return request->read<aactor*>((u)this + 0x308);
	}
	int get_player_id() const {
		SPOOF_FUNC
			if (!(u)this) return 0;
		return request->read<int>((u)this + 0x1211); //ok
	}
};
class aplayercontroller {
public:
	aactor* get_acknowledged_pawn() const {
		SPOOF_FUNC
			if (!(u)this) return 0;
		return request->read<aactor*>((u)this + 0x338);
	}

	float get_fov() const {
		SPOOF_FUNC
			if (!(u)this) return 0;
		return request->read<float>((u)this + 0x38C) * 90.f;
	}
	aplayercameramanager* get_camera_manager() const {
		SPOOF_FUNC
			if (!(uintptr_t)this) return 0;
		return (aplayercameramanager*)request->read<uintptr_t>((uintptr_t)this + 0x348);
	}
};

class  UFortClientSettingsRecord
{
	public: 

		float HUDScale() const {
			SPOOF_FUNC
				if (!(u)this) return 0;
			return request->read<float>((u)this + 0x508);
		}

};

class ulocalplayer {
public:
	aplayercontroller* get_player_controller() const {
		SPOOF_FUNC
			if (!(u)this) return 0;
		return request->read<aplayercontroller*>((u)this + 0x30);
	}

	UFortClientSettingsRecord* get_ClientSettingsRecord() const {
		SPOOF_FUNC
			if (!(u)this) return 0;
		return request->read<UFortClientSettingsRecord*>((u)this + 0x378);
	}


};
class ugameinstance {
public:
	ulocalplayer* get_local_player() const {
		SPOOF_FUNC
			if (!(u)this) return 0;
		return request->read<ulocalplayer*>((u)request->read<ulocalplayer*>((u)this + 0x38));
	}

};

class agamestate {
public:

	tarray<aplayerstate*> get_player_array() const {
		SPOOF_FUNC
			if (!(u)this) return {};

		return request->read<tarray<aplayerstate*>>((u)this + 0x2A8);
	}

};
class uworld {
public:

	ugameinstance* get_game_instance() const {
		SPOOF_FUNC
			if (!(u)this) return 0;
		return request->read<ugameinstance*>((u)this + 0x1d8);
	}
	agamestate* get_game_state() const {
		SPOOF_FUNC
			if (!(u)this) return 0;
		return request->read<agamestate*>((u)this + 0x160);
	}
};


struct WeaponInformation
{
	int32_t ammo_count;
	int32_t max_ammo;

	BYTE tier;
	std::string weapon_name;
	std::string buildplan;

};

struct WeaponInformation1
{
	int32_t ammo_count1;
	int32_t max_ammo1;

	BYTE tier;
	std::string weapon_name1;
	std::string buildplan1;

};

std::string LocalPlayerWeapon;

struct player_data {
	aactor* player{};
	uskeletalmeshcomponent* mesh{};
	uschenecomponent* root_component{};
	aplayerstate* player_state{};
	vector3 relative_location{};
	int player_team_id{};
	int player_distance{};

	player_data() = default;
};

struct global_s {
	uworld* world{};
	ugameinstance* game_instance{};
	ulocalplayer* local_player{};
	aplayercontroller* player_controller{};
	WeaponInformation WeaponInfo;
	WeaponInformation1 WeaponInfo1;

	aactor* local_pawn{};
	uschenecomponent* local_root_component{};
	aplayerstate* local_player_state{};
	aplayercameramanager* player_camera_manager{};
	std::string weapon_name{};
	int local_player_team_id{};

	agamestate* game_state{};
	std::vector<std::shared_ptr<player_data>> players{};
	std::mutex data_mutex;
};  inline std::shared_ptr<global_s> global = std::make_shared<global_s>();
inline int fps = 0;
void cache_players();
bool populate_global_data(const std::shared_ptr<global_s>& global_temp);
bool populate_player_data(const std::shared_ptr<global_s>& global_temp,
	const std::shared_ptr<player_data>& player_data_temp,
	aplayerstate* player_state);
uintptr_t CurrentVechile;
namespace pointer
{
	inline uintptr_t uworld;
	inline uintptr_t ULevel;
	inline uintptr_t Actors;
	inline uintptr_t gobjects;
	inline uintptr_t game_instance;
	inline uintptr_t local_players;
	inline uintptr_t player_controller;
	inline uintptr_t local_pawn;
	inline uintptr_t root_component;
	inline uintptr_t player_state;
	inline uintptr_t game_state;
	inline uintptr_t player_array;
	inline uintptr_t closest_pawn;
	inline uintptr_t AFortPlayerState;
	inline float closest_distance;
	inline vector3 local_relative_location;
	inline uintptr_t current_actor;
	int PlayerState;
	uintptr_t current_player_state;
}

typedef struct _ItemBase {
	int32 actorscount;
	uint64_t actorsarray;
}_ItemBase;
std::vector<_ItemBase> itemBaseList;

typedef struct _Item {
	uint64_t actor;
	std::string ItemName;
	BYTE bWTier;
	int type;
}_Item;
std::vector<_Item> itemList;
uint64_t AFortPickupAthena = 0;

typedef struct _Entity {
	int type;
	uint64_t actor;
	uint64_t mesh;
	uint64_t currweapon;
	uint64_t playerstate;
}_Entity;
std::vector<_Entity> EntityList;

void cache_players() {
	while (true) {
		SPOOF_FUNC


			auto global_temp = std::make_shared<global_s>();


		if (!populate_global_data(global_temp))
			continue;

		tarray<aplayerstate*> players = global_temp->game_state->get_player_array();
		std::vector<std::shared_ptr<player_data>> players_temp;

		for (int i = 0; i < players.Length(); i++) {
			auto player_data_temp = std::make_shared<player_data>();

			if (populate_player_data(global_temp, player_data_temp, players[i]))
				players_temp.push_back(player_data_temp);
		}
		global_temp->players = players_temp;

		std::unique_lock<std::mutex> lock(global->data_mutex);
		global = global_temp;
		lock.unlock();
		std::this_thread::sleep_for(std::chrono::seconds(0));
	}
}


inline void debug_ptr(uintptr_t p, const char* s) {
	if (debug)
		printf(SK("%s %I64u \n"), s, p);
}
inline void debug_3d(vector3 three, const char* s) {
	if (debug)
		printf(SK("%s %f, %f, %f \n"), s, three.x, three.y, three.z);
}
inline void debug_2d(vector3 two, const char* s) {
	if (debug)
		printf(SK("%s %f, %f \n"), s, two.x, two.y);
}
inline void debug_1d(double one, const char* s) {
	if (debug)
		printf(SK("%s %f \n"), s, one);
}
inline void print_3d(vector3 three, const char* s) {

	printf(SK("%s %f, %f, %f \n"), s, three.x, three.y, three.z);
}
int getinfo = false;


bool populate_global_data(const std::shared_ptr<global_s>& global_temp) {


	SPOOF_FUNC

	global_temp->world = request->read<uworld*>(Bypass::GameBase + 0x12BA7050);
	debug_ptr((u)global_temp->world, SK("world"));
	if (!(u)global_temp->world) return false;

	global_temp->game_instance = global_temp->world->get_game_instance();
	debug_ptr((u)global_temp->game_instance, SK("game_instance"));
	if (!(u)global_temp->game_instance) return false;

	global_temp->local_player = global_temp->game_instance->get_local_player();
	debug_ptr((u)global_temp->local_player, SK("local_player"));
	if (!(u)global_temp->local_player) return false;

	global_temp->player_controller = global_temp->local_player->get_player_controller();
	debug_ptr((u)global_temp->player_controller, SK("player_controller"));
	if (!(u)global_temp->player_controller) return false;

	global_temp->player_camera_manager = global_temp->player_controller->get_camera_manager();
	debug_ptr((u)global_temp->player_camera_manager, SK("player_camera_manager"));

	global_temp->local_pawn = global_temp->player_controller->get_acknowledged_pawn();
	debug_ptr((u)global_temp->local_pawn, SK("local_pawn"));

	global_temp->local_root_component = global_temp->local_pawn->get_root_component();
	debug_ptr((u)global_temp->local_root_component, SK("local_root_component"));

	global_temp->local_player_state = global_temp->local_pawn->get_player_state();
	debug_ptr((u)global_temp->local_player_state, SK("local_player_state"));

	global_temp->local_player_team_id = global_temp->local_player_state->get_player_id();
	debug_ptr(global_temp->local_player_team_id, SK("local_player_team_id"));

	global_temp->game_state = global_temp->world->get_game_state();
	debug_ptr((u)global_temp->game_state, SK("game_state"));
	if (!(u)global_temp->game_state) return false; 

	return true;
}

bool populate_player_data(const std::shared_ptr<global_s>& global_temp,
	const std::shared_ptr<player_data>& player_data_temp,
	aplayerstate* player_state) {
	SPOOF_FUNC

	player_data_temp->player_state = player_state;
	debug_ptr((u)player_data_temp->player_state, SK("player_state"));
	if (!(u)player_data_temp->player_state) return false;

	player_data_temp->player_team_id = player_data_temp->player_state->get_player_id();
	debug_ptr((u)player_data_temp->player_team_id, SK("player_team_id"));

	if (player_data_temp->player_team_id == global_temp->local_player_team_id && player_data_temp->player_team_id != 0) return false;

	player_data_temp->player = player_data_temp->player_state->get_pawn_private();
	debug_ptr((u)player_data_temp->player, SK("player"));
	if (!(u)player_data_temp->player) return false;
	if ((u)global_temp->local_pawn == (u)player_data_temp->player) return false;

	player_data_temp->root_component = player_data_temp->player->get_root_component();
	debug_ptr((u)player_data_temp->root_component, SK("root_component"));
	if (!(u)player_data_temp->root_component) return false;

	player_data_temp->relative_location = player_data_temp->root_component->get_relative_location();
	debug_3d(player_data_temp->relative_location, "relative_location");

	player_data_temp->player_distance = camera_postion.location.distance(player_data_temp->relative_location) / 100;
	debug_1d(player_data_temp->player_distance, "distance");
	if ((u)global_temp->local_pawn)
		if (player_data_temp->player_distance > max_esp_distance) return false;

	player_data_temp->mesh = player_data_temp->player->get_mesh();
	debug_ptr((u)player_data_temp->mesh, SK("mesh"));
	if (!(u)player_data_temp->mesh) return false;


	return true;
}




