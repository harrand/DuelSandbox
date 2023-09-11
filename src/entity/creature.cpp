#include "creature.hpp"
#include "system.hpp"

namespace game
{
	entity_creature::entity_creature(std::unique_ptr<iskeleton> skeleton, tz::ren::animation_renderer::asset_package resources):
	resources(resources),
	skeleton(std::move(skeleton))
	{

	}

	const iskeleton& entity_creature::get_skeleton() const
	{
		tz::assert(this->skeleton != nullptr);
		return *this->skeleton.get();
	}
	
	iskeleton& entity_creature::get_skeleton()
	{
		tz::assert(this->skeleton != nullptr);
		return *this->skeleton.get();
	}

	tz::trs entity_creature::get_base_transform(const entity_system& sys) const
	{
		tz::assert(this->resources.objects.size());
		return sys.get_renderer().get_object_base_transform(this->resources.objects.front());
	}

	void entity_creature::set_base_transform(entity_system& sys, tz::trs transform)
	{
		sys.get_renderer().set_object_base_transform(this->resources.objects.front(), transform);
	}
}