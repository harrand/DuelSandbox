#include "player_human.hpp"
#include "tz/wsi/keyboard.hpp"
#include "tz/dbgui/dbgui.hpp"
#include <iostream>

namespace game
{
	void entity_player_human::dbgui(entity_system& sys)
	{
		entity_generic_human::dbgui(sys);
		ImGui::Checkbox("Controlled", &this->controlled);
	}

	bool is_key_down(tz::wsi::key k)
	{
		return !tz::dbgui::claims_keyboard() && tz::wsi::is_key_down(tz::window().get_keyboard_state(), k);
	}

	void entity_player_human::on_update(float delta, entity_system& sys)
	{
		if(!this->controlled)
		{
			return;
		}
		bool any_transform = false;
		if(is_key_down(tz::wsi::key::w))
		{
			any_transform = true;
			tz::trs transform = this->get_base_transform(sys);
			tz::vec3 forward = transform.rotate.rotate(this->get_skeleton().forward());
			transform.translate += forward.normalised() * this->get_movement_speed() * delta;
			this->set_base_transform(sys, transform);
			this->get_skeleton().set_animation_state(iskeleton::animation_state::run);
		}
		else if(is_key_down(tz::wsi::key::s))
		{
			any_transform = true;
			tz::trs transform = this->get_base_transform(sys);
			tz::vec3 forward = transform.rotate.rotate(this->get_skeleton().forward() * -1.0f);
			transform.translate += forward.normalised() * this->get_movement_speed() * delta;
			this->set_base_transform(sys, transform);
			this->get_skeleton().set_animation_state(iskeleton::animation_state::run);
		}

		constexpr float rotate_speed = 3.0f;

		if(is_key_down(tz::wsi::key::a))
		{
			any_transform = true;
			tz::trs transform = this->get_base_transform(sys);
			transform.rotate.combine(tz::quat::from_axis_angle({0.0f, 1.0f, 0.0f}, -1.57081f * delta * rotate_speed));
			this->set_base_transform(sys, transform);
		}
		else if(is_key_down(tz::wsi::key::d))
		{
			any_transform = true;
			tz::trs transform = this->get_base_transform(sys);
			transform.rotate.combine(tz::quat::from_axis_angle({0.0f, 1.0f, 0.0f}, 1.57081f * delta * rotate_speed));
			this->set_base_transform(sys, transform);
		}
		if(!any_transform)
		{
			// well he ait doing anything! idle them!
			this->get_skeleton().set_animation_state(iskeleton::animation_state::idle);
		}
	}
}