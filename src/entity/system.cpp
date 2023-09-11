#include "system.hpp"
#include "tz/core/debug.hpp"

namespace game
{
	entity_system::entity_system()
	{
		this->renderer.append_to_render_graph();
	}

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

	const tz::ren::animation_renderer& entity_system::get_renderer() const
	{
		return this->renderer;
	}

	tz::ren::animation_renderer& entity_system::get_renderer()
	{
		return this->renderer;
	}
	
	void entity_system::update(float delta)
	{
		for(auto& ptr : this->entities)
		{
			ptr->update(delta, *this);
		}
		this->renderer.update(delta);
	}

	ientity* entity_system::get(eid_t ent)
	{
		tz::assert(ent < this->entities.size());
		return this->entities[ent].get();
	}
}