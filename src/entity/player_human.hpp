#ifndef DUELSANDBOX_ENTITY_PLAYER_HUMAN_HPP
#define DUELSANDBOX_ENTITY_PLAYER_HUMAN_HPP
#include "generic_human.hpp"

namespace game
{
	class entity_player_human : public entity_generic_human
	{
	public:
		entity_player_human(std::unique_ptr<iskeleton> skeleton, tz::ren::animation_renderer::asset_package resources);
		virtual void dbgui(entity_system& sys) override;
	protected:
		virtual void on_update(float delta, entity_system& sys) override;
	private:
		bool controlled = true;
		tz::vec2ui old_mouse_position = tz::vec2::zero();
		int old_mouse_wheel_position = 0;
		bool autorun_latch = false;
		bool autorun = false;
	};
}

#endif // DUELSANDBOX_ENTITY_PLAYER_HUMAN_HPP