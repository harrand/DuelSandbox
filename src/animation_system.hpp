#ifndef DUELSANDBOX_ANIMATION_SYSTEM_HPP
#define DUELSANDBOX_ANIMATION_SYSTEM_HPP
#include "tz/ren/animation.hpp"
#include "skeleton.hpp"

namespace game
{
	namespace detail
	{
		struct entity_tag_t{};
	}

	enum class model
	{
		human_male
	};

	class animation_system
	{
	public:
		animation_system();
		using entity_handle = tz::handle<detail::entity_tag_t>;
		entity_handle add(model m, tz::trs local_transform = {});
		void update(float delta);
		void dbgui();

		void set_camera_follow(entity_handle eh);
		void camera_stop_follow();
	private:
		using renderer_t = tz::ren::animation_renderer;
		struct entity
		{
			std::unique_ptr<iskeleton> skeleton;
			renderer_t::asset_package pkg;
		};
		renderer_t renderer = {};
		std::map<model, renderer_t::asset_package> gltf_override_cache = {};
		std::vector<entity> entities = {};
		tz::vec3 camera_follow_offset = {0.0f, -2.0f, -2.5f};
		entity_handle camera_follow_target = tz::nullhand;
	};
}

#endif // DUELSANDBOX_ANIMATION_SYSTEM_HPP