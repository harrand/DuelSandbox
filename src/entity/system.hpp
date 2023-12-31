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
		void remove(std::size_t entity);

		template<typename T>
		const T& get(std::size_t entity_id) const;
		template<typename T>
		T& get(std::size_t entity_id);

		std::size_t entity_count() const;

		damage_status event(eid_t ent, entity_take_damage_event e);
		damage_status event(eid_t ent, entity_deal_damage_event e);
		heal_status event(eid_t ent, entity_take_heal_event e);
		heal_status event(eid_t ent, entity_deal_heal_event e);

		const tz::ren::animation_renderer& get_renderer() const;
		tz::ren::animation_renderer& get_renderer();
		void set_tracked(std::size_t entity_id);
		std::optional<std::size_t> get_tracked() const;
		void clear_tracked();
		void update(float delta);
		void dbgui();
		void dbgui_bar();

		tz::vec3 get_follow_offset_displacement() const;
		void set_follow_offset_displacement(tz::vec3 displacement);
		tz::quat get_follow_offset_rotation() const;
		void set_follow_offset_rotation(tz::quat rotation);
		float get_follow_offset_scale() const;
		void set_follow_offset_scale(float scale);
	private:
		ientity* get(eid_t ent);
		bool is_cached(iskeleton::type t) const;
		void dbgui_impl();
		void dbgui_overview();
		void dbgui_renderer();
		void dbgui_bar_impl();

		tz::ren::animation_renderer renderer = {1024u, {tz::gl::renderer_option::no_clear_output}};
		std::vector<std::unique_ptr<ientity>> entities = {};
		std::vector<std::size_t> entity_free_list = {};
		std::optional<std::size_t> tracked_entity = std::nullopt;
		std::map<iskeleton::type, tz::ren::animation_renderer::asset_package> skeleton_cache = {};
		tz::vec3 follow_offset_displacement = {0.0f, 2.0f, 2.5f};
		tz::quat follow_offset_rotation = tz::quat::from_axis_angle(tz::vec3::zero(), 1.0f);
		float follow_offset_scale = 1.0f;
	};
}

#include "system.inl"
#endif // DUELSANDBOX_ENTITY_SYSTEM_HPP