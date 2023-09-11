#include "tz/tz.hpp"
#include "tz/core/time.hpp"
#include "tz/wsi/mouse.hpp"
#include "tz/dbgui/dbgui.hpp"
#include "tz/ren/animation.hpp"
#include "animation_system.hpp"

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
	bool animation_renderer_enabled = false;
} dbgui_data;

void init()
{
	dbgui_init();
	game::animation_system asys;
	game::animation_system::entity_handle e1 = asys.add(game::model::human_male);
	game::animation_system::entity_handle e2 = asys.add(game::model::human_male);

	tz::duration update_timer = tz::system_time();
	while(!tz::window().is_close_requested())
	{
		TZ_FRAME_BEGIN;
		tz::begin_frame();
		// draw
		tz::gl::get_device().render();
		asys.update((tz::system_time() - update_timer).seconds<float>());
		update_timer = tz::system_time();

		// advance dbgui
		tz::dbgui::run([&asys]()
		{
			if(dbgui_data.animation_renderer_enabled)
			{
				if(ImGui::Begin("Animation System", &dbgui_data.animation_renderer_enabled))
				{
					asys.dbgui();
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
		ImGui::MenuItem("Animation Renderer", nullptr, &dbgui_data.animation_renderer_enabled);
	});
}