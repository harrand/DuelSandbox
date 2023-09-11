#include "../skeleton.hpp"

namespace game::skeleton
{
	class human_male : public game::iskeleton
	{
	private:
		using game::iskeleton::iskeleton;
		constexpr virtual anim_mapping_t impl_anim_to_id() override
		{
			return
			{
				8u, // idle
				12u, // walk
				10u, // run
				7u, // death
			};
		}
	public:
		constexpr virtual game::iskeleton::type get_type() override
		{
			return game::iskeleton::type::human_male;
		}

		virtual tz::io::gltf model_data() const override
		{
			return tz::io::gltf::from_file("./res/models/human_male.glb");
		}

		virtual tz::trs follow(tz::trs location, tz::vec3 offset_displacement) const override
		{
			location.scale = tz::vec3::filled(1.0f);
			tz::trs result{.translate = offset_displacement.swizzle<0, 2, 1>(), .rotate = {0.7071f, 0.0f, 0.0f, 0.7071f}};
			result.combine(location);
			result.scale[1] *= -1.0f;
			return result;
		}

		virtual void set_animation_state(animation_state state) override
		{
			tz::assert(iskeleton::impl_has_ctx());
			switch(state)
			{
				case animation_state::idle:
					if(iskeleton::old != animation_state::idle)
					{
						ctx.renderer->play_animation(ctx.pkg, this->impl_anim_to_id()[(int)state], true);
					}
				break;	
			}
			iskeleton::set_animation_state(state);
		}
	};
}