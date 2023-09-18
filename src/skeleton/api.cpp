#include "api.hpp"

namespace game
{
	iskeleton::iskeleton(context ctx):
	ctx(ctx){}

	tz::trs iskeleton::follow(tz::trs location, tz::vec3 offset_displacement, tz::quat offset_rotation) const
	{
		location.scale = tz::vec3::filled(1.0f);
		offset_displacement = offset_rotation.inversed().rotate(offset_displacement);
		tz::trs result{.translate = offset_displacement, .rotate = offset_rotation.inversed()};
		result.combine(location);
		return result;
	}

	tz::vec3 iskeleton::forward() const
	{
		return {0.0f, 0.0f, -1.0f};
	}

	bool iskeleton::has_landmark(landmark l) const
	{
		return this->get_landmark(l) != tz::nullhand;
	}

	bool iskeleton::landmark_attachment::empty() const
	{
		return this->pkg.objects.empty();
	}

	void iskeleton::landmark_set_attachment(landmark l, landmark_attachment attachment)
	{
		tz::assert(this->has_landmark(l));
		auto parent_objh = this->get_landmark(l);

		// if we already have an attachment, we need to remove the necessary children.
		auto old_attachment = this->landmark_get_attachment(l);
		if(!old_attachment.empty())
		{
			for(auto objh : old_attachment.pkg.objects)
			{
				// TODO: remove `objh` as a child from `parent_objh`.
				// first assert that `parent_objh` does indeed contain `objh` as a child.
				// blocked on being able to re-seat parents/children of objects within tz::ren::mesh/animation_renderer.
				(void)0;
			}
		}

		// set the new children.
		for(auto objh : attachment.pkg.objects)
		{
			// TODO: set `objh` as a child of `parent_objh`
			(void)0;
		}
		tz::error("NYI");
	}

	void iskeleton::landmark_clear_attachment(landmark l)
	{
		this->landmark_set_attachment(l, {});
	}

	iskeleton::landmark_attachment iskeleton::landmark_get_attachment(landmark l) const
	{
		return this->attachments[(int)l];
	}

	iskeleton::animation_state iskeleton::get_animation_state() const
	{
		return old;
	}

	void iskeleton::set_animation_state(animation_state state)
	{
		tz::assert(this->ctx.renderer != nullptr && this->ctx.pkg.objects.size());
		if(iskeleton::old != state)
		{
			const bool should_loop = state != animation_state::death;
			ctx.renderer->play_animation(ctx.pkg, iskeleton::get_animation(state), should_loop);
		}
		this->old = state;
	}

	void iskeleton::set_context(context ctx)
	{
		this->ctx = ctx;
	}

	std::size_t iskeleton::get_animation(animation_state state)
	{
		return this->impl_anim_to_id()[static_cast<int>(state)];
	}
}