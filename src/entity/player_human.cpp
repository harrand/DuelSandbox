#include "player_human.hpp"
#include "system.hpp"
#include "tz/wsi/keyboard.hpp"
#include "tz/wsi/mouse.hpp"
#include "tz/dbgui/dbgui.hpp"
#include <iostream>

namespace game
{
	entity_player_human::entity_player_human(std::unique_ptr<iskeleton> skeleton, tz::ren::animation_renderer::asset_package resources):
	entity_generic_human(std::move(skeleton), resources)
	{
		ientity::static_flags |= static_entity_flag::player;
	}

	void entity_player_human::dbgui(entity_system& sys)
	{
		entity_generic_human::dbgui(sys);
		ImGui::Checkbox("Controlled", &this->controlled);
	}

	bool is_key_down(tz::wsi::key k)
	{
		return !tz::dbgui::claims_keyboard() && tz::wsi::is_key_down(tz::window().get_keyboard_state(), k);
	}

	bool is_mouse_down(tz::wsi::mouse_button b)
	{
		return !tz::dbgui::claims_mouse() && tz::wsi::is_mouse_button_down(tz::window().get_mouse_state(), b);
	}

	void entity_player_human::on_update(float delta, entity_system& sys)
	{
		tz::vec2ui new_mouse_pos = tz::window().get_mouse_state().mouse_position;
		tz::vec2 mouse_delta = static_cast<tz::vec2>(new_mouse_pos) - this->old_mouse_position;
		this->old_mouse_position = new_mouse_pos;

		int new_wheel_position = tz::window().get_mouse_state().wheel_position;
		int wheel_delta = new_wheel_position - this->old_mouse_wheel_position;
		this->old_mouse_wheel_position = new_wheel_position;
		if(!this->controlled)
		{
			return;
		}
		bool any_transform = false;
		bool any_rotate = false;
		if(wheel_delta != 0)
		{
			float sc = sys.get_follow_offset_scale();
			sc -= (0.1f * wheel_delta);
			sc = std::clamp(sc, 0.1f, 10.0f);
			sys.set_follow_offset_scale(sc);
		}
		if(is_mouse_down(tz::wsi::mouse_button::middle))
		{
			any_rotate = true;
			constexpr float angular_velocity = 8.0f;
			tz::vec2ui dims = tz::window().get_dimensions();
			tz::vec2 delta = mouse_delta;
			delta[0] /= dims[0];
			delta[1] /= dims[1];

			tz::quat r1 = tz::quat::from_axis_angle({0.0f, 1.0f, 0.0f}, delta[0] * angular_velocity);
			//tz::quat r2 = tz::quat::from_axis_angle({1.0f, 0.0f, 0.0f}, delta[1]);
			//r1.combine(r2);

			tz::quat old_rot = sys.get_follow_offset_rotation();
			old_rot.combine(r1);
			sys.set_follow_offset_rotation(old_rot);
		}
		if(is_key_down(tz::wsi::key::w))
		{
			any_transform = true;
			tz::trs transform = this->get_base_transform(sys);
			tz::vec3 forward = transform.rotate.rotate(this->get_skeleton().forward());
			tz::vec3 move_amt = forward.normalised() * this->get_movement_speed() * delta;
			if(is_key_down(tz::wsi::key::left_shift))
			{
				this->get_skeleton().set_animation_state(iskeleton::animation_state::walk);
			}
			else
			{
				this->get_skeleton().set_animation_state(iskeleton::animation_state::run);
				move_amt *= 2.5f;
			}
			transform.translate += move_amt;
			this->set_base_transform(sys, transform);
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
		if(any_transform && !any_rotate)
		{
			constexpr float normalise_rotation_factor = 2.5f;
			tz::quat rot = sys.get_follow_offset_rotation();
			rot = rot.slerp(tz::quat::from_axis_angle(tz::vec3::zero(), 1.0f), delta * normalise_rotation_factor);
			sys.set_follow_offset_rotation(rot);
		}
	}
}