#ifndef TERRAIN_3D_ASSET_LAYER_CLASS_H
#define TERRAIN_3D_ASSET_LAYER_CLASS_H

#include <godot_cpp/classes/editor_plugin.hpp>
#include <godot_cpp/classes/node3d.hpp>

#include "constants.h"
#include "logger.h"
#include "terrain_3d_assets.h"
#include "terrain_3d_instancer.h"

using namespace godot;

class Terrain3D;

class Terrain3DAssetLayer : public Node3D {
	GDCLASS(Terrain3DAssetLayer, Node3D);
	CLASS_NAME();

private:
	bool _is_inside_world = false;
	bool _initialized = false;

	Terrain3D *_terrain = nullptr;
	Terrain3DInstancer *_instancer = nullptr;
	Ref<Terrain3DAssets> _assets;
	String _al_mmi_parent_node_name;

	Node3D *_al_mmi_parent_node;

	void _init();

public:
	Terrain3DAssetLayer();
	~Terrain3DAssetLayer();

	void _destroy_instancer();

	virtual void _enter_tree() override;
	virtual void _exit_tree() override;

	virtual void _ready() override;

	void _initialize();

	void set_terrain(Terrain3D *p_terrain);
	Terrain3D *get_terrain() const { return _terrain; }
	void set_assets(const Ref<Terrain3DAssets> &p_assets);
	Ref<Terrain3DAssets> get_assets() const { return _assets; }
	Terrain3DInstancer *get_instancer() const { return _instancer; }

	Node *get_al_mmi_parent_node() const { return _al_mmi_parent_node; }

	void _build_containers();
	void _destroy_containers();

protected:
	void _notification(int p_notification);
	static void _bind_methods();
};

#endif // TERRAIN_3D_ASSET_LAYER_CLASS_H