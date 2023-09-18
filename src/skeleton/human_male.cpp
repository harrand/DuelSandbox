#include "human_male.hpp"

namespace game::skeleton
{
	tz::io::gltf human_male::model_data() const
	{
		return tz::io::gltf::from_file("./res/models/human_male.glb");
	}

	tz::ren::animation_renderer::object_handle human_male::get_landmark(landmark l) const
	{
		tz::assert(this->ctx.pkg.objects.size());
		switch(l)
		{
			case iskeleton::landmark::root:
				return this->ctx.pkg.objects.front();
			break;
			case iskeleton::landmark::head:
				return this->ctx.pkg.objects[9];
			break;
			default: break;
		}
		tz::error("Unimplemented landmark %s", landmark_get_name(l));
		return tz::nullhand;
	}
}