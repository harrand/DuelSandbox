#ifndef DUELSANDBOX_ENTITY_SYSTEM_HPP
#define DUELSANDBOX_ENTITY_SYSTEM_HPP
#include "api.hpp"
#include <vector>
#include <memory>

namespace game
{
	class entity_system
	{
	public:
		entity_system() = default;
		damage_status event(eid_t ent, entity_take_damage_event e);
		damage_status event(eid_t ent, entity_deal_damage_event e);
		heal_status event(eid_t ent, entity_take_heal_event e);
		heal_status event(eid_t ent, entity_deal_heal_event e);
	private:
		ientity* get(eid_t ent);
		std::vector<std::unique_ptr<ientity>> entities = {};
	};
}

#endif // DUELSANDBOX_ENTITY_SYSTEM_HPP