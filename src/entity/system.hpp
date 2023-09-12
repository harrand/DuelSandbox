#ifndef DUELSANDBOX_ENTITY_SYSTEM_HPP
#define DUELSANDBOX_ENTITY_SYSTEM_HPP
#include "api.hpp"
#include "creature.hpp"
#include "generic_human.hpp"
#include "tz/ren/animation.hpp"
#include <vector>
#include <memory>

namespace game
{
	class entity_system
	{
	public:
		entity_system();

		template<typename T>
		std::size_t add();

		damage_status event(eid_t ent, entity_take_damage_event e);
		damage_status event(eid_t ent, entity_deal_damage_event e);
		heal_status event(eid_t ent, entity_take_heal_event e);
		heal_status event(eid_t ent, entity_deal_heal_event e);

		const tz::ren::animation_renderer& get_renderer() const;
		tz::ren::animation_renderer& get_renderer();
		void set_tracked(std::size_t entity_id);
		void clear_tracked();
		void update(float delta);
		void dbgui();
	private:
		ientity* get(eid_t ent);
		bool is_cached(iskeleton::type t) const;
		void dbgui_impl();
		void dbgui_overview();
		void dbgui_renderer();

		tz::ren::animation_renderer renderer = {};
		std::vector<std::unique_ptr<ientity>> entities = {};
		std::optional<std::size_t> tracked_entity = std::nullopt;
		std::map<iskeleton::type, tz::ren::animation_renderer::asset_package> skeleton_cache = {};
	};
}

#include "system.inl"
#endif // DUELSANDBOX_ENTITY_SYSTEM_HPP