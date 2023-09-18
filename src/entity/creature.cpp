#include "creature.hpp"
#include "system.hpp"

namespace game
{
	entity_creature::entity_creature(std::unique_ptr<iskeleton> skeleton, tz::ren::animation_renderer::asset_package resources):
	resources(resources),
	skeleton(std::move(skeleton))
	{
		ientity::static_flags |= static_entity_flag::creature;
	}

	void entity_creature::dbgui(entity_system& sys)
	{
		ientity::dbgui(sys);
		this->dbgui_impl(sys);
	}

	void entity_creature::track(entity_system& sys, tz::vec3 displacement_offset, tz::quat rotation_offset)
	{
		auto trans = this->get_global_transform(sys);
		trans.scale = tz::vec3::filled(1.0f);
		tz::trs follow_loc = this->get_skeleton().follow(trans, displacement_offset, rotation_offset);
		sys.get_renderer().set_camera_transform(follow_loc);
	}

	const iskeleton& entity_creature::get_skeleton() const
	{
		tz::assert(this->skeleton != nullptr);
		return *this->skeleton.get();
	}
	
	iskeleton& entity_creature::get_skeleton()
	{
		tz::assert(this->skeleton != nullptr);
		return *this->skeleton.get();
	}

	const tz::ren::animation_renderer::asset_package& entity_creature::get_asset_package() const
	{
		return this->resources;
	}

	tz::trs entity_creature::get_global_transform(const entity_system& sys) const
	{
		return this->get_global_transform(sys, iskeleton::landmark::root);
	}

	void entity_creature::set_global_transform(entity_system& sys, tz::trs global_transform)
	{
		this->set_global_transform(sys, global_transform, iskeleton::landmark::root);
	}

	tz::trs entity_creature::get_base_transform(const entity_system& sys) const
	{
		return this->get_base_transform(sys, iskeleton::landmark::root);
	}

	void entity_creature::set_base_transform(entity_system& sys, tz::trs transform)
	{
		this->set_base_transform(sys, transform, iskeleton::landmark::root);
	}

	tz::trs entity_creature::get_global_transform(const entity_system& sys, iskeleton::landmark landmark) const
	{
		tz::assert(this->resources.objects.size());
		auto objh = this->skeleton->get_landmark(landmark);
		tz::assert(std::find(this->resources.objects.begin(), this->resources.objects.end(), objh) != this->resources.objects.end(), "Landmark retrieved object handle %zu which is not owned by this skeleton", static_cast<std::size_t>(static_cast<tz::hanval>(objh)));
		return sys.get_renderer().get_object_global_transform(objh);
	}

	void entity_creature::set_global_transform(entity_system& sys, tz::trs global_transform, iskeleton::landmark landmark)
	{
		tz::assert(this->resources.objects.size());
		auto objh = this->skeleton->get_landmark(landmark);
		tz::assert(std::find(this->resources.objects.begin(), this->resources.objects.end(), objh) != this->resources.objects.end(), "Landmark retrieved object handle %zu which is not owned by this skeleton", static_cast<std::size_t>(static_cast<tz::hanval>(objh)));
		tz::trs local = sys.get_renderer().global_to_local_transform(objh, global_transform);
		this->set_base_transform(sys, local);
	}

	tz::trs entity_creature::get_base_transform(const entity_system& sys, iskeleton::landmark landmark) const
	{
		tz::assert(this->resources.objects.size());
		auto objh = this->skeleton->get_landmark(landmark);
		tz::assert(std::find(this->resources.objects.begin(), this->resources.objects.end(), objh) != this->resources.objects.end(), "Landmark retrieved object handle %zu which is not owned by this skeleton", static_cast<std::size_t>(static_cast<tz::hanval>(objh)));
		return sys.get_renderer().get_object_base_transform(objh);
	}

	void entity_creature::set_base_transform(entity_system& sys, tz::trs transform, iskeleton::landmark landmark)
	{
		tz::assert(this->resources.objects.size());
		auto objh = this->skeleton->get_landmark(landmark);
		tz::assert(std::find(this->resources.objects.begin(), this->resources.objects.end(), objh) != this->resources.objects.end(), "Landmark retrieved object handle %zu which is not owned by this skeleton", static_cast<std::size_t>(static_cast<tz::hanval>(objh)));
		sys.get_renderer().set_object_base_transform(objh, transform);
	}

	void entity_creature::dbgui_impl(entity_system& sys)
	{
		std::string objects_label_str = "Objects (" + std::to_string(this->resources.objects.size()) + ")";
		if(this->resources.objects.size() && ImGui::TreeNode(objects_label_str.c_str()))
		{
			for(tz::ren::animation_renderer::object_handle oh : this->resources.objects)
			{
				std::string_view name = sys.get_renderer().get_object_name(oh);
				ImGui::Text("%s (%zu)", name.data(), static_cast<std::size_t>(static_cast<tz::hanval>(oh)));
			}
			ImGui::TreePop();
		}

		ImGui::Text("Skeleton Type: \"%s\"", iskeleton::type_get_name(this->skeleton->get_type()));
	}
}