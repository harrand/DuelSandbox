#include "system.hpp"
#include "imgui.h"
#include "tz/core/debug.hpp"
#include "tz/dbgui/dbgui.hpp"

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

	void entity_system::set_tracked(std::size_t entity_id)
	{
		this->tracked_entity = entity_id;
	}

	void entity_system::clear_tracked()
	{
		this->tracked_entity = std::nullopt;
	}
	
	void entity_system::update(float delta)
	{
		for(auto& ptr : this->entities)
		{
			ptr->update(delta, *this);
		}
		this->renderer.update(delta);
		if(this->tracked_entity.has_value())
		{
			this->entities[this->tracked_entity.value()]->track(*this);
		}
	}

	void entity_system::dbgui()
	{
		this->dbgui_impl();
	}

	ientity* entity_system::get(eid_t ent)
	{
		tz::assert(ent < this->entities.size());
		return this->entities[ent].get();
	}

	bool entity_system::is_cached(iskeleton::type t) const
	{
		return this->skeleton_cache.find(t) != this->skeleton_cache.end();
	}

	void entity_system::dbgui_impl()
	{
		if(ImGui::BeginTabBar("Entity System"))
		{
			if(ImGui::BeginTabItem("Overview"))
			{
				this->dbgui_overview();
				ImGui::EndTabItem();
			}
			if(ImGui::BeginTabItem("Renderer"))
			{
				this->dbgui_renderer();
				ImGui::EndTabItem();
			}
		}
		ImGui::EndTabBar();
	}

	void entity_system::dbgui_overview()
	{
		ImGui::TextColored(ImVec4{1.0f, 0.3f, 0.3f, 1.0f}, "ENTITIES");
		static int entity_cursor = 0;
		if(this->entities.size())
		{
			ImGui::VSliderInt("##entitycursor", ImVec2{18.0f, 160.0f}, &entity_cursor, 0, this->entities.size() - 1);
			ImGui::SameLine();
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10.0f);
			if(ImGui::BeginChild("#entityinfo", ImVec2{0.0f, 160.0f}, false, ImGuiWindowFlags_ChildWindow))
			{
				if(this->tracked_entity != entity_cursor && ImGui::Button("Track"))
				{
					this->tracked_entity = entity_cursor;
				}
				if(this->tracked_entity.has_value() && this->tracked_entity.value() == entity_cursor)
				{
					if(ImGui::Button("Untrack"))
					{
						this->tracked_entity = std::nullopt;
					}
				}
				this->entities[entity_cursor]->dbgui(*this);
			}
			ImGui::EndChild();
		}
	}

	void entity_system::dbgui_renderer()
	{
		this->renderer.dbgui();
	}
}