#include "api.hpp"
#include "system.hpp"
#include <algorithm>

namespace game
{
	std::size_t ientity::eid_count = 0;

	float ientity::get_health() const
	{
		return this->current_health;
	}

	bool ientity::operator==(const ientity& rhs) const
	{
		return this->eid == rhs.eid;
	}

	bool ientity::operator!=(const ientity& rhs) const
	{
		return this->eid != rhs.eid;
	}

	bool ientity::operator==(const world_entity_t& w) const
	{
		return this->eid == w;
	}

	bool ientity::operator!=(const world_entity_t& w) const
	{
		return this->eid != w;
	}

	void ientity::update(float delta, entity_system& sys)
	{
		this->on_update(delta, sys);
	}

	damage_status ientity::on_deal_damage(entity_deal_damage_event e, entity_system& sys)
	{
		return sys.event(e.target,
		entity_take_damage_event{
			.striker = this->eid,
			.damage = e.damage
		});
	}

	damage_status ientity::on_take_damage(entity_take_damage_event e, entity_system& sys)
	{
		if(e.damage >= this->current_health)
		{
			this->current_health = 0.0f;
			this->on_death
			({
				.provoking_event = e
			}, sys);
		}
		else
		{
			this->current_health -= e.damage;
		}
		return damage_status::success;
	}

	heal_status ientity::on_deal_heal(entity_deal_heal_event e, entity_system& sys)
	{
		return sys.event(e.target,
		entity_take_heal_event{
			.healer = this->eid,
			.heal = e.heal
		});
	}

	heal_status ientity::on_take_heal(entity_take_heal_event e, entity_system& sys)
	{
		if(this->is_dead())
		{
			return heal_status::immune;
		}
		this->current_health = std::clamp(this->current_health + e.heal, this->current_health, this->max_health);
		return heal_status::success;
	}
}