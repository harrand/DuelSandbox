#ifndef DUELSANDBOX_SKELETON_HPP
#define DUELSANDBOX_SKELETON_HPP
#include "tz/ren/animation.hpp"
#include "tz/io/gltf.hpp"

namespace game
{
	class iskeleton
	{
	public:
		iskeleton() = default;
		struct context
		{
			tz::ren::animation_renderer* renderer = nullptr;
			tz::ren::animation_renderer::asset_package pkg = {};
		};
		iskeleton(context ctx);
		virtual ~iskeleton() = default;

		enum class type
		{
			human_male,
			_count
		};
		static constexpr const char* type_get_name(type t)
		{
			return std::array<const char*, (int)type::_count>
			{
				"Human Male",
			}[(int)t];
		}


		enum class animation_state
		{
			idle,
			walk,
			run,
			death,
			dance,
			_count,
			_undefined
		};
		static constexpr const char* animation_state_get_name(animation_state s)
		{
			if(s == animation_state::_undefined)
			{
				return "No Animation";
			}
			return std::array<const char*, (int)animation_state::_count>
			{
				"Idle",
				"Walk",
				"Run",
				"Death",
				"Dance"
			}[(int)s];
		}

		constexpr virtual type get_type() = 0;
		virtual tz::io::gltf model_data() const = 0;
		virtual tz::trs follow(tz::trs location, tz::vec3 offset_displacement, tz::quat offset_rotation) const = 0;
		virtual tz::vec3 forward() const = 0;
		animation_state get_animation_state() const;
		void set_animation_state(animation_state state);
		void set_context(context ctx);
	protected:
		using anim_mapping_t = std::array<std::size_t, static_cast<int>(animation_state::_count)>;
		constexpr virtual anim_mapping_t impl_anim_to_id() = 0;
		std::size_t get_animation(animation_state state);

		context ctx = {};
		animation_state old = animation_state::_undefined;
	};
}

#endif // DUELSANDBOX_SKELETON_HPP