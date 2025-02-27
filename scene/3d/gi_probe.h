/*************************************************************************/
/*  gi_probe.h                                                           */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2021 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2021 Godot Engine contributors (cf. AUTHORS.md).   */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#ifndef GIPROBE_H
#define GIPROBE_H

#include "multimesh_instance_3d.h"
#include "scene/3d/visual_instance_3d.h"

class GIProbeData : public Resource {
	GDCLASS(GIProbeData, Resource);

	RID probe;

	void _set_data(const Dictionary &p_data);
	Dictionary _get_data() const;

	Transform3D to_cell_xform;
	AABB bounds;
	Vector3 octree_size;

	float dynamic_range = 4.0;
	float energy = 1.0;
	float bias = 1.5;
	float normal_bias = 0.0;
	float propagation = 0.7;
	float anisotropy_strength = 0.5;
	float ao = 0.0;
	float ao_size = 0.5;
	bool interior = false;
	bool use_two_bounces = false;

protected:
	static void _bind_methods();
	void _validate_property(PropertyInfo &property) const override;

public:
	void allocate(const Transform3D &p_to_cell_xform, const AABB &p_aabb, const Vector3 &p_octree_size, const Vector<uint8_t> &p_octree_cells, const Vector<uint8_t> &p_data_cells, const Vector<uint8_t> &p_distance_field, const Vector<int> &p_level_counts);
	AABB get_bounds() const;
	Vector3 get_octree_size() const;
	Vector<uint8_t> get_octree_cells() const;
	Vector<uint8_t> get_data_cells() const;
	Vector<uint8_t> get_distance_field() const;
	Vector<int> get_level_counts() const;
	Transform3D get_to_cell_xform() const;

	void set_dynamic_range(float p_range);
	float get_dynamic_range() const;

	void set_propagation(float p_propagation);
	float get_propagation() const;

	void set_anisotropy_strength(float p_anisotropy_strength);
	float get_anisotropy_strength() const;

	void set_ao(float p_ao);
	float get_ao() const;

	void set_ao_size(float p_ao_size);
	float get_ao_size() const;

	void set_energy(float p_energy);
	float get_energy() const;

	void set_bias(float p_bias);
	float get_bias() const;

	void set_normal_bias(float p_normal_bias);
	float get_normal_bias() const;

	void set_interior(bool p_enable);
	bool is_interior() const;

	void set_use_two_bounces(bool p_enable);
	bool is_using_two_bounces() const;

	virtual RID get_rid() const override;

	GIProbeData();
	~GIProbeData();
};

class GIProbe : public VisualInstance3D {
	GDCLASS(GIProbe, VisualInstance3D);

public:
	enum Subdiv {
		SUBDIV_64,
		SUBDIV_128,
		SUBDIV_256,
		SUBDIV_512,
		SUBDIV_MAX

	};

	typedef void (*BakeBeginFunc)(int);
	typedef void (*BakeStepFunc)(int, const String &);
	typedef void (*BakeEndFunc)();

private:
	Ref<GIProbeData> probe_data;

	RID gi_probe;

	Subdiv subdiv = SUBDIV_128;
	Vector3 extents = Vector3(10, 10, 10);

	struct PlotMesh {
		Ref<Material> override_material;
		Vector<Ref<Material>> instance_materials;
		Ref<Mesh> mesh;
		Transform3D local_xform;
	};

	void _find_meshes(Node *p_at_node, List<PlotMesh> &plot_meshes);
	void _debug_bake();

protected:
	static void _bind_methods();

public:
	static BakeBeginFunc bake_begin_function;
	static BakeStepFunc bake_step_function;
	static BakeEndFunc bake_end_function;

	void set_probe_data(const Ref<GIProbeData> &p_data);
	Ref<GIProbeData> get_probe_data() const;

	void set_subdiv(Subdiv p_subdiv);
	Subdiv get_subdiv() const;

	void set_extents(const Vector3 &p_extents);
	Vector3 get_extents() const;
	Vector3i get_estimated_cell_size() const;

	void bake(Node *p_from_node = nullptr, bool p_create_visual_debug = false);

	virtual AABB get_aabb() const override;
	virtual Vector<Face3> get_faces(uint32_t p_usage_flags) const override;

	TypedArray<String> get_configuration_warnings() const override;

	GIProbe();
	~GIProbe();
};

VARIANT_ENUM_CAST(GIProbe::Subdiv)

#endif // GIPROBE_H
