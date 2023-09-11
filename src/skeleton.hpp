#ifndef DUELSANDBOX_SKELETON_HPP
#define DUELSANDBOX_SKELETON_HPP
#include "tz/ren/animation.hpp"
#include "tz/io/gltf.hpp"

namespace game
{
	class iskeleton
	{
	public:

		struct context
		{
			tz::ren::animation_renderer* renderer = nullptr;
			tz::ren::animation_renderer::asset_package pkg = {};
		};

		iskeleton(context ctx):
		ctx(ctx){}

		iskeleton() = default;
		virtual ~iskeleton() = default;

		enum class type
		{
			human_male
		};
		enum class animation_state
		{
			idle,
			walk,
			run,
			death,
			_count,
			_undefined
		};

		constexpr virtual type get_type() = 0;
		virtual tz::io::gltf model_data() const = 0;
		virtual tz::trs follow(tz::trs location, tz::vec3 offset_displacement) const = 0;
		virtual void set_animation_state(animation_state state)
		{
			this->old = state;
		}
		void set_context(context ctx)
		{
			this->ctx = ctx;
		}
	protected:
		using anim_mapping_t = std::array<std::size_t, static_cast<int>(animation_state::_count)>;
		constexpr virtual anim_mapping_t impl_anim_to_id() = 0;
		bool impl_has_ctx() const
		{
			return this->ctx.renderer != nullptr;
		}
		context& impl_get_ctx()
		{
			tz::assert(this->impl_has_ctx());
			return this->ctx;
		}
		context ctx = {};
		animation_state old = animation_state::_undefined;
	};
}

#endif // DUELSANDBOX_SKELETON_HPP