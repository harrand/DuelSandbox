#include "api.hpp"
#include "system.hpp"
#include <algorithm>

namespace game
{
	std::size_t ientity::eid_count = 0;

	bool ientity::is(static_entity_flag sflag) const
	{
		return this->static_flags.contains(sflag);
	}

	float ientity::get_health() const
	{
		return this->current_health;
	}

	float ientity::get_max_health() const
	{
		return this->max_health;
	}

	float ientity::get_movement_speed() const
	{
		return this->movement_speed;
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

	void ientity::dbgui(entity_system& sys)
	{
		(void)sys;
		ImGui::Text("Entity %zu", this->eid);
		ImGui::SliderFloat("Health", &this->current_health, 0.0f, this->max_health);
		ImGui::SameLine();
		ImGui::InputFloat("Max", &this->max_health, 1.0f, 5.0f);
		ImGui::InputFloat("Movement Speed", &this->movement_speed, 0.1f, 0.5f);
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