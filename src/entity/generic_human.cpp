#include "generic_human.hpp"

namespace game
{
	entity_generic_human::entity_generic_human(std::unique_ptr<iskeleton> skeleton, tz::ren::animation_renderer::asset_package resources):
	entity_creature(std::move(skeleton), resources)
	{

	}

	void entity_generic_human::on_death(entity_death_event e, entity_system& sys)
	{
		this->get_skeleton().set_animation_state(iskeleton::animation_state::death);
	}
}