#include "api.hpp"

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
				6u, // dance
			};
		}
	public:
		constexpr virtual game::iskeleton::type get_type() override
		{
			return game::iskeleton::type::human_male;
		}

		virtual tz::io::gltf model_data() const override;
		virtual tz::trs follow(tz::trs location, tz::vec3 offset_displacement, tz::quat offset_rotation) const override;
		virtual tz::vec3 forward() const override;
	};
}