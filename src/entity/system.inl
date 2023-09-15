namespace game
{
	template<typename T>
	std::size_t entity_system::add()
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
				using A = tz::ren::animation_renderer;
				A::override_package opkg;
				if(this->is_cached(skel->get_type()))
				{
					opkg =
					{
						.pkg = this->skeleton_cache.at(skel->get_type()),
						.overrides = {A::override_flag::mesh, A::override_flag::texture}
					};
				}
				auto pkg = this->renderer.add_gltf(skel->model_data(), opkg);
				this->skeleton_cache[skel->get_type()] = pkg;
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

	template<typename T>
	const T& entity_system::get(std::size_t entity_id) const
	{
		return *static_cast<const T*>(this->entities[entity_id].get());
	}

	template<typename T>
	T& entity_system::get(std::size_t entity_id)
	{
		return *static_cast<T*>(this->entities[entity_id].get());
	}
}