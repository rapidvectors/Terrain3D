#include "terrain_3d_asset_layer.h"

Terrain3DAssetLayer::Terrain3DAssetLayer() {
}

Terrain3DAssetLayer::~Terrain3DAssetLayer() {
}

void Terrain3DAssetLayer::_init() {
}

void Terrain3DAssetLayer::_initialize(Terrain3D *p_terrain) {
	if (_terrain != p_terrain) {
		LOG(INFO, "Terrain asset layer already initialized");
		_terrain = p_terrain;
	}

	/*if (_terrain == nullptr) {
		LOG(DEBUG, "Set the terrain first");
		return;
	}*/

	if (_assets.is_null()) {
		LOG(DEBUG, "Terrain assets not created.");
		return;
	}

	if (_instancer == nullptr) {
		LOG(DEBUG, "Creating instancer");
		_instancer = memnew(Terrain3DInstancer);
	}

	// MeshAssets changed, update instancer
	if (!_assets->is_connected("meshes_changed", callable_mp(_instancer, &Terrain3DInstancer::_update_mmis).bind(V2I_MAX, -1))) {
		LOG(DEBUG, "Connecting _assets.meshes_changed to _instancer->_update_mmis()");
		_assets->connect("meshes_changed", callable_mp(_instancer, &Terrain3DInstancer::_update_mmis).bind(V2I_MAX, -1));
	}

	if (!_initialized && is_inside_tree()) {
		//_mesh_asset->initialize(this);
		_instancer->initialize(_terrain);
		_initialized = true;
	}

	// Initialize the system
	//if (!_initialized && _is_inside_world && is_inside_tree()) {
	//	//_mesh_asset->initialize(this);
	//	_instancer->initialize(_terrain);
	//	_initialized = true;
	//}
	update_configuration_warnings();
}

void Terrain3DAssetLayer::set_terrain(Terrain3D *p_terrain) {
	if (_terrain != p_terrain) {
		//_clear_meshes();
		LOG(INFO, "Setting asset list");
		_terrain = p_terrain;
		_initialize(_terrain);
		//emit_signal("assets_changed");
	}
}

void Terrain3DAssetLayer::set_assets(const Ref<Terrain3DAssets> &p_assets) {
	if (_assets != p_assets) {
		//_clear_meshes();
		LOG(INFO, "Setting asset list");
		_assets = p_assets;
		_initialize(_terrain);
		//emit_signal("assets_changed");
	}
}

void Terrain3DAssetLayer::_bind_methods() {
	// bind the class methods
	ClassDB::bind_method(D_METHOD("set_terrain", "terrain"), &Terrain3DAssetLayer::set_terrain);
	ClassDB::bind_method(D_METHOD("get_terrain"), &Terrain3DAssetLayer::get_terrain);
	ClassDB::bind_method(D_METHOD("set_assets", "assets"), &Terrain3DAssetLayer::set_assets);
	ClassDB::bind_method(D_METHOD("get_assets"), &Terrain3DAssetLayer::get_assets);
	ClassDB::bind_method(D_METHOD("get_instancer"), &Terrain3DAssetLayer::get_instancer);

	// create the UI properties
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "assets", PROPERTY_HINT_RESOURCE_TYPE, "Terrain3DAssets"), "set_assets", "get_assets");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "instancer", PROPERTY_HINT_NONE, "Terrain3DInstancer", PROPERTY_USAGE_NONE), "", "get_instancer");
}
