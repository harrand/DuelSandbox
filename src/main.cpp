#include "tz/tz.hpp"
#include "tz/core/time.hpp"
#include "tz/wsi/mouse.hpp"
#include "tz/dbgui/dbgui.hpp"
#include "tz/ren/animation.hpp"

#include "entity/system.hpp"
#include "entity/player_human.hpp"

void init();
void dbgui_init();

int main()
{
	tz::initialise
	({
		.name = "Duel Sandbox"
	});
	init();
	tz::terminate();
}

struct dbgui_data_t
{
	bool entity_system_enabled = false;
} dbgui_data;

void init()
{
	dbgui_init();
	game::entity_system esys;
	auto player = esys.add<game::entity_player_human>();
	esys.add<game::entity_generic_human>();
	esys.set_tracked(player);

	tz::duration update_timer = tz::system_time();
	while(!tz::window().is_close_requested())
	{
		TZ_FRAME_BEGIN;
		tz::begin_frame();
		// draw
		tz::gl::get_device().render();
		auto millis_diff = (tz::system_time() - update_timer).nanos<std::uint64_t>();
		esys.update(millis_diff / 1000000000.0f);
		update_timer = tz::system_time();

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