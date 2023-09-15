#include "human_male.hpp"

namespace game::skeleton
{
	tz::io::gltf human_male::model_data() const
	{
		return tz::io::gltf::from_file("./res/models/human_male.glb");
	}

	tz::trs human_male::follow(tz::trs location, tz::vec3 offset_displacement, tz::quat offset_rotation) const
	{
		location.scale = tz::vec3::filled(1.0f);
		offset_displacement = offset_rotation.inversed().rotate(offset_displacement);
		std::swap(offset_rotation[2], offset_rotation[1]);
		tz::trs result{.translate = offset_displacement.swizzle<0, 2, 1>(), .rotate = tz::quat{0.7071f, 0.0f, 0.0f, 0.7071f}.combined(offset_rotation)};
		result.combine(location);
		result.scale[1] *= -1.0f;
		return result;
	}

	tz::vec3 human_male::forward() const
	{
		return {0.0f, 1.0f, 0.0f};
	}
}