#ifndef DUELSANDBOX_ENTITY_PLAYER_HUMAN_HPP
#define DUELSANDBOX_ENTITY_PLAYER_HUMAN_HPP
#include "generic_human.hpp"

namespace game
{
	class entity_player_human : public entity_generic_human
	{
	public:
		using entity_generic_human::entity_generic_human;
	protected:
		virtual void on_update(float delta, entity_system& sys) override;
	};
}

#endif // DUELSANDBOX_ENTITY_PLAYER_HUMAN_HPP