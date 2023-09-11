#include "skeleton.hpp"

namespace game
{
	iskeleton::iskeleton(context ctx):
	ctx(ctx){}

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