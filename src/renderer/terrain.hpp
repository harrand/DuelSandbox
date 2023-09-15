#ifndef DUELSANDBOX_RENDERER_TERRAIN_HPP
#define DUELSANDBOX_RENDERER_TERRAIN_HPP
#include "tz/core/data/vector.hpp"
#include "tz/core/data/trs.hpp"
#include "tz/gl/resource.hpp"
#include "tz/gl/renderer.hpp"

namespace game
{
	class terrain_renderer
	{
	public:
		terrain_renderer() = default;
		void append_to_render_graph();
		void update();
		void set_camera_position(tz::vec3 cam_pos);
		void set_camera_rotation(tz::quat rot);
	private:
		void set_camera_transform(tz::trs camera_transform);
		struct render_pass_t
		{
			render_pass_t();
			tz::gl::resource_handle buf;
			tz::gl::resource_handle buf2;
			tz::gl::renderer_handle handle;	
		};

		tz::vec3 cam_pos = tz::vec3::zero();
		tz::quat cam_rot = tz::quat::from_axis_angle(tz::vec3::zero(), 1.0f);
		render_pass_t render_pass;
	};
}

#endif // DUELSANDBOX_RENDERER_TERRAIN_HPP