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

		// skeletons are comprised of an asset-package, namely its objects. a skeleton could have an arbitrary number of objects, and we have no idea which object is which
		// a landmark represents a notable part of a skeleton (e.g its root location, its head, its right hand, etc...).
		// you can get a corresponding object_handle via `iskeleton::get_landmark(landmark)`.
		//
		// for example, to equip a helmet onto a head, you might get the object_handle corresponding to landmark::head, and then make your helmet object a child of it,
		// causing the helm to appear on the head.
		enum class landmark
		{
			// core area of where the skeleton lives. think of it as the origin of the skeleton. may not necessarily be in the middle!
			root,
			head,
			_count
		};
		static constexpr const char* landmark_get_name(landmark l)
		{
			return std::array<const char*, (int)landmark::_count>
			{
				"Root",
				"Head",
			}[(int)l];
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
		// retrieve the gltf corresponding to this skeleton. contains meshes, textures and default objects.
		virtual tz::io::gltf model_data() const = 0;
		// given a TRS representing the location of the skeleton in world space, return a new TRS representing a camera looking at the skeleton from behind.
		// offsets represent how far away and rotated the camera view is with respect to the back of the skeleton.
		virtual tz::trs follow(tz::trs location, tz::vec3 offset_displacement, tz::quat offset_rotation) const = 0;
		// represents a forward move vector (i.e direction where the skeleton would expect to go if you told it to "move forward").
		// note that this vector is in local-space. e.g something like {0, 0, -1}
		virtual tz::vec3 forward() const = 0;

		// retrieve the object handle corresponding to a landmark.
		// if this skeleton has no such landmark (e.g a banshee does not have legs), nullhand is returned.
		virtual tz::ren::animation_renderer::object_handle get_landmark(landmark l) const = 0;
		// query as to whether the skeleton has a landmark. returns true if the landmark exists for the skeleton, otherwise false.
		bool has_landmark(landmark l) const;

		// what if we want to attach other objects to a landmark on a skeleton? e.g equipment
		// each landmark supports a single attachment, which is an asset package of objects representing the equipped thing.
		// an attachment is a temporary set of children of the landmark's objects so it is attached to the landmark part.
		// e.g if a human equips a helmet, we might get the objects representing the helm, and mount it onto landmark::head.
		struct landmark_attachment
		{
			std::string name = "Nothing";
			// do we want to take ownership of attached objects? probably not.
			// let's pretend lifetime semantics are safe and sorted elsewhere.
			tz::ren::animation_renderer::asset_package pkg = {};

			bool empty() const;
		};

		// i.e equip.
		void landmark_set_attachment(landmark l, landmark_attachment attachment);
		// i.e unequip.
		void landmark_clear_attachment(landmark l);
		// i.e get equipped
		landmark_attachment landmark_get_attachment(landmark l) const;

		// which animation is the skeleton currently in?
		animation_state get_animation_state() const;
		// set the animation to something new.
		void set_animation_state(animation_state state);
		// re-seat the skeleton in a new context. 99.9% you dont want to ever invoke this.
		void set_context(context ctx);
	protected:
		using anim_mapping_t = std::array<std::size_t, static_cast<int>(animation_state::_count)>;
		constexpr virtual anim_mapping_t impl_anim_to_id() = 0;
		std::size_t get_animation(animation_state state);

		context ctx = {};
		animation_state old = animation_state::_undefined;
		std::array<landmark_attachment, (int)landmark::_count> attachments;
	};
}

#endif // DUELSANDBOX_SKELETON_HPP