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
		tz::trs result{.translate = offset_displacement, .rotate = offset_rotation.inversed()};
		result.combine(location);
		return result;
	}

	tz::vec3 human_male::forward() const
	{
		return {0.0f, 0.0f, -1.0f};
	}

	tz::ren::animation_renderer::object_handle human_male::get_landmark(landmark l) const
	{
		tz::assert(this->ctx.pkg.objects.size());
		switch(l)
		{
			case iskeleton::landmark::root:
				return this->ctx.pkg.objects.front();
			break;
			default: break;
		}
		tz::error("Unimplemented landmark %s", landmark_get_name(l));
		return tz::nullhand;
	}
}