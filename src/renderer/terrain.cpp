#include "terrain.hpp"
#include "tz/core/matrix.hpp"
#include "tz/core/matrix_transform.hpp"
#include "tz/gl/imported_shaders.hpp"

#include ImportedShaderHeader(terrain, vertex)
#include ImportedShaderHeader(terrain, tesscon)
#include ImportedShaderHeader(terrain, tesseval)
#include ImportedShaderHeader(terrain, fragment)

namespace game
{
	struct buffer_data
	{
		tz::mat4 model = tz::model({0.0f, -15.0f, 0.0f}, tz::vec3::zero(), {1.0f, 0.4f, 1.0f});
		tz::mat4 view = tz::view(tz::vec3::zero(), tz::vec3::zero());
		tz::mat4 projection;
		tz::vec3 camera_position;
	};

	struct feedback_data
	{
		tz::vec3 pos = tz::vec3::zero();
	};

	void terrain_renderer::append_to_render_graph()
	{
		tz::gl::get_device().render_graph().timeline.push_back(static_cast<tz::gl::eid_t>(static_cast<tz::hanval>(this->render_pass.handle)));
	}

	void terrain_renderer::update()
	{
		auto& bufdat = tz::gl::get_device().get_renderer(this->render_pass.handle).get_resource(this->render_pass.buf)->data_as<buffer_data>().front();
		auto dimsf = static_cast<tz::vec2>(tz::window().get_dimensions());
		bufdat.projection = tz::perspective(1.5708f, dimsf[0] / dimsf[1], 0.1f, 1000.0f);

		tz::vec3 height_only = this->cam_pos;
		height_only[0] = height_only[2] = 0.0f;
		bufdat.view = tz::translate(height_only) * this->cam_rot.matrix();
		bufdat.camera_position[0] = -this->cam_pos[0];
		bufdat.camera_position[2] = -this->cam_pos[2];
	}

	void terrain_renderer::set_camera_position(tz::vec3 cam_pos)
	{
		this->cam_pos = cam_pos;
	}

	void terrain_renderer::set_camera_rotation(tz::quat rot)
	{
		this->cam_rot = rot;
	}

	void terrain_renderer::set_camera_transform(tz::trs camera_transform)
	{
		auto& bufdat = tz::gl::get_device().get_renderer(this->render_pass.handle).get_resource(this->render_pass.buf)->data_as<buffer_data>().front();
		bufdat.view = camera_transform.matrix().inverse();
	}

	terrain_renderer::render_pass_t::render_pass_t()
	{
		tz::gl::renderer_info rinfo;
		this->buf = rinfo.add_resource(tz::gl::buffer_resource::from_one(buffer_data{},
		{
			.access = tz::gl::resource_access::dynamic_access
		}));
		this->buf2 = rinfo.add_resource(tz::gl::buffer_resource::from_one(feedback_data{},
		{
			.access = tz::gl::resource_access::dynamic_access
		}));
		rinfo.set_options({tz::gl::renderer_option::no_present});
		rinfo.state().graphics.tri_count = 4;
		rinfo.state().graphics.clear_colour = tz::vec3{0.76f, 0.835f, 0.855f}.with_more(1.0f);
		rinfo.shader().set_shader(tz::gl::shader_stage::vertex, ImportedShaderSource(terrain, vertex));
		rinfo.shader().set_shader(tz::gl::shader_stage::tessellation_control, ImportedShaderSource(terrain, tesscon));
		rinfo.shader().set_shader(tz::gl::shader_stage::tessellation_evaluation, ImportedShaderSource(terrain, tesseval));
		rinfo.shader().set_shader(tz::gl::shader_stage::fragment, ImportedShaderSource(terrain, fragment));

		rinfo.debug_name("Terrain Renderer");

		this->handle = tz::gl::get_device().create_renderer(rinfo);
	}
}