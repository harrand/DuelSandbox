#include "human_male.hpp"

namespace game::skeleton
{
	tz::io::gltf human_male::model_data() const
	{
		return tz::io::gltf::from_file("./res/models/human_male.glb");
	}

	tz::trs human_male::follow(tz::trs location, tz::vec3 offset_displacement) const
	{
		location.scale = tz::vec3::filled(1.0f);
		tz::trs result{.translate = offset_displacement.swizzle<0, 2, 1>(), .rotate = {0.7071f, 0.0f, 0.0f, 0.7071f}};
		result.combine(location);
		result.scale[1] *= -1.0f;
		return result;
	}
}