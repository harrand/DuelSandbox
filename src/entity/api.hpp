#ifndef DUELSANDBOX_ENTITY_API_HPP
#define DUELSANDBOX_ENTITY_API_HPP
#include "tz/core/data/vector.hpp"
#include "tz/core/data/quat.hpp"
#include <cstddef>
#include <limits>
#include <optional>

namespace game
{
	using eid_t = std::size_t;

	struct entity_take_damage_event
	{
		eid_t striker;
		float damage;
	};

	struct entity_deal_damage_event
	{
		eid_t target;
		float damage;
	};

	struct entity_take_heal_event
	{
		eid_t healer;
		float heal;
	};

	struct entity_deal_heal_event
	{
		eid_t target;
		float heal;
	};

	struct entity_death_event
	{
		std::optional<entity_take_damage_event> provoking_event = std::nullopt;
	};

	enum class damage_status
	{
		success,
		absorb,
		immune
	};

	enum class heal_status
	{
		success,
		absorb,
		immune
	};

	constexpr struct world_entity_t
	{
		constexpr eid_t operator()() const{return std::numeric_limits<eid_t>::max();}
	} world_entity;
	constexpr bool operator==(eid_t eid, [[maybe_unused]] const world_entity_t&)
	{
		return eid == std::numeric_limits<eid_t>::max();
	}
	constexpr bool operator!=(eid_t eid, [[maybe_unused]] const world_entity_t&)
	{
		return eid != std::numeric_limits<eid_t>::max();
	}

	class entity_system;

	class ientity
	{
	public:
		friend class entity_system;
		constexpr ientity() = default;
		~ientity() = default;

		float get_health() const;
		float get_movement_speed() const;
		bool operator==(const ientity& rhs) const;
		bool operator!=(const ientity& rhs) const;
		bool operator==(const world_entity_t&) const;
		bool operator!=(const world_entity_t&) const;
		void update(float delta, entity_system& sys);
		virtual bool is_dead() const{return this->current_health == 0.0f;};
		virtual void dbgui(entity_system& sys);
		virtual void track(entity_system& sys, tz::vec3 displacement_offset, tz::quat rotation_offset){};
	protected:
		virtual damage_status on_deal_damage(entity_deal_damage_event e, entity_system& sys);
		virtual damage_status on_take_damage(entity_take_damage_event e, entity_system& sys);
		virtual heal_status on_deal_heal(entity_deal_heal_event e, entity_system& sys);
		virtual heal_status on_take_heal(entity_take_heal_event e, entity_system& sys);
		virtual void on_death(entity_death_event e, entity_system& sys){};
		virtual void on_update(float delta, entity_system& sys){};
	private:
		static eid_t eid_count;

		eid_t eid = eid_count++;
		float current_health = 0.0f;
		float max_health = 0.0f;
		float movement_speed = 1.5f;
	};
}

#endif // DUELSANDBOX_ENTITY_API_HPP