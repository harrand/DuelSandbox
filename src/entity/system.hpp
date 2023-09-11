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
		std::size_t add()
		{
			auto sz = this->entities.size();
			std::unique_ptr<ientity> ent = nullptr;
			if constexpr(std::is_base_of_v<entity_creature, T>)
			{
				std::unique_ptr<iskeleton> skel = nullptr;
				// it has a skeleton.
				// but what type of skeleton?
				if constexpr(std::is_base_of_v<entity_generic_human, T>)
				{
					// human male.
					skel = std::make_unique<skeleton::human_male>();
					auto pkg = this->renderer.add_gltf(skel->model_data());
					skel->set_context
					({
						.renderer = &this->renderer,
						.pkg = pkg	
					});
					ent = std::make_unique<T>(std::move(skel), pkg);
				}
			}
			this->entities.push_back(std::move(ent));
			return sz;
		}

		damage_status event(eid_t ent, entity_take_damage_event e);
		damage_status event(eid_t ent, entity_deal_damage_event e);
		heal_status event(eid_t ent, entity_take_heal_event e);
		heal_status event(eid_t ent, entity_deal_heal_event e);

		const tz::ren::animation_renderer& get_renderer() const;
		tz::ren::animation_renderer& get_renderer();
		void update(float delta);
	private:
		ientity* get(eid_t ent);
		tz::ren::animation_renderer renderer = {};
		std::vector<std::unique_ptr<ientity>> entities = {};
	};
}

#endif // DUELSANDBOX_ENTITY_SYSTEM_HPP