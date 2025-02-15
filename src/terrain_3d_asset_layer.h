#ifndef TERRAIN_3D_MESHER_CLASS_H
#define TERRAIN_3D_MESHER_CLASS_H

#include <godot_cpp/classes/node3d.hpp>

#include "constants.h"
#include "logger.h"
#include "terrain_3d_instancer.h"
#include "terrain_3d_mesh_asset.h"

using namespace godot;

class Terrain3DAssetLayer : public Node3D {
	GDCLASS(Terrain3DAssetLayer, Node3D);
	CLASS_NAME();

private:
	bool _is_inside_world = false;
	bool _initialized = false;

	Terrain3D *_terrain = nullptr;
	Terrain3DInstancer *_instancer = nullptr;
	Ref<Terrain3DAssets> _assets;

	void _init();

	void _initialize();

public:
	Terrain3DAssetLayer();
	~Terrain3DAssetLayer();

	void set_terrain(Terrain3D *p_terrain);
	Terrain3D* get_terrain() const { return _terrain; }
	void set_assets(const Ref<Terrain3DAssets> &p_assets);
	Ref<Terrain3DAssets> get_assets() const { return _assets; }

protected:
	static void _bind_methods();
};

#endif // TERRAIN_3D_MESHER_CLASS_H