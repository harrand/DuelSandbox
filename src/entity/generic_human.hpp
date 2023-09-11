#ifndef DUELSANDBOX_ENTITY_GENERIC_HUMAN_HPP
#define DUELSANDBOX_ENTITY_GENERIC_HUMAN_HPP
#include "creature.hpp"
#include "../skeletons/human_male.hpp"

namespace game
{
	class entity_generic_human : public entity_creature
	{
	public:
		entity_generic_human(std::unique_ptr<iskeleton> skeleton, tz::ren::animation_renderer::asset_package resources);
		virtual void dbgui(entity_system& sys) override;
	protected:
		virtual void on_death(entity_death_event e, entity_system& sys) override;
	private:
		void dbgui_impl(entity_system& sys);
	};
}

#endif // DUELSANDBOX_ENTITY_GENERIC_HUMAN_HPP