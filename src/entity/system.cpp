#include "system.hpp"
#include "tz/core/debug.hpp"

namespace game
{
	damage_status entity_system::event(eid_t ent, entity_take_damage_event e)
	{
		return this->get(ent)->on_take_damage(e, *this);
	}

	damage_status entity_system::event(eid_t ent, entity_deal_damage_event e)
	{
		return this->get(ent)->on_deal_damage(e, *this);
	}

	heal_status entity_system::event(eid_t ent, entity_take_heal_event e)
	{
		return this->get(ent)->on_take_heal(e, *this);
	}

	heal_status entity_system::event(eid_t ent, entity_deal_heal_event e)
	{
		return this->get(ent)->on_deal_heal(e, *this);
	}

	ientity* entity_system::get(eid_t ent)
	{
		tz::assert(ent < this->entities.size());
		return this->entities[ent].get();
	}
}