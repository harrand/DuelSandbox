#include "tz/tz.hpp"
#include "tz/core/time.hpp"
#include "tz/wsi/mouse.hpp"
#include "tz/dbgui/dbgui.hpp"
#include "tz/ren/animation.hpp"

#include "tz/lua/state.hpp"
#include "tz/lua/api.hpp"

#include "entity/system.hpp"
#include "entity/player_human.hpp"

void init();
void dbgui_init();

int main()
{
	tz::initialise
	({
		.name = "Duel Sandbox",
		.flags = {tz::application_flag::window_transparent}
	});
	init();
	tz::terminate();
}

struct dbgui_data_t
{
	bool entity_system_enabled = false;
} dbgui_data;

game::entity_system* esys_glob = nullptr;

LUA_BEGIN(spawn_human)
	std::size_t r = esys_glob->add<game::entity_player_human>();
	esys_glob->set_tracked(r);
	state.stack_push_uint(r);
	return 1;
LUA_END

void init()
{
	dbgui_init();
	game::entity_system esys;
	esys_glob = &esys;
	auto player = esys.add<game::entity_player_human>();
	esys.add<game::entity_generic_human>();
	esys.set_tracked(player);
	esys.get_renderer().add_gltf(tz::io::gltf::from_file("./res/models/tile5x5.glb"));

	LUA_REGISTER_ALL(spawn_human);

	tz::duration update_timer = tz::system_time();
	while(!tz::window().is_close_requested())
	{
		TZ_FRAME_BEGIN;
		tz::begin_frame();
		// draw
		tz::gl::get_device().render();
		auto now = tz::system_time();
		auto millis_diff = (now - update_timer).nanos<std::uint64_t>();
		esys.update(millis_diff / 1000000000.0f);
		update_timer = now;

		// advance dbgui
		tz::dbgui::run([&esys]()
		{
			if(dbgui_data.entity_system_enabled)
			{
				if(ImGui::Begin("Entity System", &dbgui_data.entity_system_enabled))
				{
					esys.dbgui();
					ImGui::End();
				}
			}
		});
		tz::end_frame();
		TZ_FRAME_END;
	}
}

void dbgui_init()
{
	tz::dbgui::game_menu().add_callback([]()
	{
		ImGui::MenuItem("Entity System", nullptr, &dbgui_data.entity_system_enabled);
	});
}