#ifndef DUELSANDBOX_ENTITY_CREATURE_HPP
#define DUELSANDBOX_ENTITY_CREATURE_HPP
#include "api.hpp"
#include "../skeleton/api.hpp"
#include "tz/ren/animation.hpp"

namespace game
{
	class entity_creature : public ientity
	{
	public:
		entity_creature() = default;
		entity_creature(std::unique_ptr<iskeleton> skeleton, tz::ren::animation_renderer::asset_package resources);
		virtual void dbgui(entity_system& sys) override;
		virtual void track(entity_system& sys, tz::vec3 displacement_offset, tz::quat rotation_offset) override;
		const iskeleton& get_skeleton() const;
		iskeleton& get_skeleton();
		const tz::ren::animation_renderer::asset_package& get_asset_package() const;

		// without landmarks
		tz::trs get_global_transform(const entity_system& sys) const;
		void set_global_transform(entity_system& sys, tz::trs global_transform);
		tz::trs get_base_transform(const entity_system& sys) const;
		void set_base_transform(entity_system& sys, tz::trs base_transform);


		// with landmarks
		tz::trs get_global_transform(const entity_system& sys, iskeleton::landmark landmark) const;
		void set_global_transform(entity_system& sys, tz::trs global_transform, iskeleton::landmark landmark);
		tz::trs get_base_transform(const entity_system& sys, iskeleton::landmark landmark) const;
		void set_base_transform(entity_system& sys, tz::trs transform, iskeleton::landmark landmark);
	private:
		void dbgui_impl(entity_system& sys);
		tz::ren::animation_renderer::asset_package resources = {};
		std::unique_ptr<iskeleton> skeleton = nullptr;
	};
}

#endif // DUELSANDBOX_ENTITY_CREATURE_HPP