#include "player_human.hpp"
#include "tz/wsi/keyboard.hpp"

namespace game
{
	void entity_player_human::on_update(float delta, entity_system& sys)
	{
		if(tz::wsi::is_key_down(tz::window().get_keyboard_state(), tz::wsi::key::w))
		{
			tz::trs transform = this->get_base_transform(sys);
			transform.combine({.translate = {0.0f, 0.0f, delta}});
			this->set_base_transform(sys, transform);
			this->get_skeleton().set_animation_state(iskeleton::animation_state::run);
		}
		else if(tz::wsi::is_key_down(tz::window().get_keyboard_state(), tz::wsi::key::s))
		{
			tz::trs transform = this->get_base_transform(sys);
			transform.combine({.translate = {0.0f, 0.0f, -delta}});
			this->set_base_transform(sys, transform);
			this->get_skeleton().set_animation_state(iskeleton::animation_state::run);
		}
		// rotations are really hard!
		else
		{
			// well he ait doing anything! idle them!
			this->get_skeleton().set_animation_state(iskeleton::animation_state::idle);
		}
	}
}