#include "animation_system.hpp"
#include "skeletons/human_male.hpp"

namespace game
{
	animation_system::animation_system()
	{
		this->renderer.append_to_render_graph();
	}

	animation_system::entity_handle animation_system::add(model m, tz::trs local_transform)
	{
		renderer_t::override_package opkg;
		const bool already_contains = this->gltf_override_cache.find(m) != this->gltf_override_cache.end();
		if(already_contains)
		{
			// alreay added model of this type before. use override package.
			opkg.overrides = {renderer_t::override_flag::mesh, renderer_t::override_flag::texture};
			opkg.pkg = this->gltf_override_cache.at(m);
		}
		tz::io::gltf gltf;
		std::unique_ptr<iskeleton> skel = nullptr;
		switch(m)
		{
			case model::human_male:
				skel = std::make_unique<skeleton::human_male>();
				gltf = skel->model_data();
			break;
		}
		renderer_t::asset_package pkg = this->renderer.add_gltf(gltf, opkg);
		skel->set_context({.renderer = &this->renderer, .pkg = pkg});
		skel->set_animation_state(iskeleton::animation_state::idle);
		std::size_t entity_id = this->entities.size();
		this->entities.push_back
		({
			.skeleton = std::move(skel),
			.pkg = pkg
		});
		return static_cast<tz::hanval>(entity_id);
	}

	void animation_system::update(float delta)
	{
		this->renderer.update(delta);

		if(this->camera_follow_target != tz::nullhand)
		{
			auto hanval = static_cast<std::size_t>(static_cast<tz::hanval>(this->camera_follow_target));
			const auto& ent = this->entities[hanval];
			tz::assert(ent.pkg.objects.size());
			renderer_t::object_handle main_obj = ent.pkg.objects.front();
			tz::trs global = this->renderer.get_object(main_obj).global_transform;
			this->renderer.set_camera_transform(ent.skeleton->follow(global, this->camera_follow_offset, {}));
		}
	}

	void animation_system::dbgui()
	{
		ImGui::SliderFloat3("Camera Translation Offset", this->camera_follow_offset.data().data(), -50.0f, 50.0f);
		this->renderer.get_camera_transform().dbgui();
		int follow_entity_id = -1;
		if(this->camera_follow_target != tz::nullhand)
		{
			follow_entity_id = static_cast<std::size_t>(static_cast<tz::hanval>(this->camera_follow_target));
		}
		for(std::size_t i = 0; i < this->entities.size(); i++)
		{
			std::string ename = "Entity" + std::to_string(i);
			ImGui::RadioButton(ename.c_str(), &follow_entity_id, i);
		}
		if(follow_entity_id != -1 && static_cast<int>(static_cast<tz::hanval>(this->camera_follow_target)) != follow_entity_id)
		{
			this->set_camera_follow(static_cast<tz::hanval>(follow_entity_id));
		}
		this->renderer.dbgui();
	}

	void animation_system::set_camera_follow(entity_handle eh)
	{
		this->camera_follow_target = eh;
	}

	void animation_system::camera_stop_follow()
	{
		this->camera_follow_target = tz::nullhand;
	}
}