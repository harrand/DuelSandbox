#include "tz/tz.hpp"
#include "tz/core/time.hpp"
#include "tz/dbgui/dbgui.hpp"
#include "tz/ren/animation.hpp"

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
	tz::ren::animation_renderer ar;
	auto pkg = ar.add_gltf(tz::io::gltf::from_file("./res/models/human_male.glb"));
	ar.append_to_render_graph();

	tz::duration update_timer = tz::system_time();
	while(!tz::window().is_close_requested())
	{
		TZ_FRAME_BEGIN;
		tz::begin_frame();
		// draw
		tz::gl::get_device().render();
		ar.update((tz::system_time() - update_timer).seconds<float>());
		update_timer = tz::system_time();
		// advance dbgui
		tz::dbgui::run([&ar]()
		{
			if(dbgui_data.animation_renderer_enabled)
			{
				if(ImGui::Begin("Animation Renderer", &dbgui_data.animation_renderer_enabled))
				{
					ar.dbgui();
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