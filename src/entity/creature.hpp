#ifndef DUELSANDBOX_ENTITY_CREATURE_HPP
#define DUELSANDBOX_ENTITY_CREATURE_HPP
#include "api.hpp"
#include "../skeleton.hpp"
#include "tz/ren/animation.hpp"

namespace game
{
	class entity_creature : public ientity
	{
	public:
		entity_creature() = default;
		entity_creature(std::unique_ptr<iskeleton> skeleton, tz::ren::animation_renderer::asset_package resources);
		const iskeleton& get_skeleton() const;
		iskeleton& get_skeleton();

		tz::trs get_base_transform(const entity_system& sys) const;
		void set_base_transform(entity_system& sys, tz::trs transform);
	private:
		tz::ren::animation_renderer::asset_package resources = {};
		std::unique_ptr<iskeleton> skeleton = nullptr;
	};
}

#endif // DUELSANDBOX_ENTITY_CREATURE_HPP