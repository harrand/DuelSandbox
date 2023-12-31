#include "generic_human.hpp"

namespace game
{
	entity_generic_human::entity_generic_human(std::unique_ptr<iskeleton> skeleton, tz::ren::animation_renderer::asset_package resources):
	entity_creature(std::move(skeleton), resources)
	{
		// rip in peace.
		this->get_skeleton().set_animation_state(iskeleton::animation_state::death);
	}

	void entity_generic_human::dbgui(entity_system& sys)
	{
		entity_creature::dbgui(sys);
		this->dbgui_impl(sys);
	}

	void entity_generic_human::on_death(entity_death_event e, entity_system& sys)
	{
		this->get_skeleton().set_animation_state(iskeleton::animation_state::death);
	}

	void entity_generic_human::dbgui_impl(entity_system& sys)
	{
		tz::assert(this->get_skeleton().get_type() == iskeleton::type::human_male);
		ImGui::Text("Current Animation: %s", iskeleton::animation_state_get_name(this->get_skeleton().get_animation_state()));
	}
}