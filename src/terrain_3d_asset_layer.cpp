#include "terrain_3d_asset_layer.h"

#include <godot_cpp/classes/engine.hpp>

Terrain3DAssetLayer::Terrain3DAssetLayer() {
}

Terrain3DAssetLayer::~Terrain3DAssetLayer() {
	_destroy_instancer();
	_destroy_containers();
}

void Terrain3DAssetLayer::_destroy_instancer() {
	if (_instancer != nullptr) {
		LOG(INFO, "Destroying Instancer");
		memdelete_safely(_instancer);
	}
}

void Terrain3DAssetLayer::_enter_tree() {
	UtilityFunctions::print("Enter tree");

	call_deferred("_initialize");
}

void Terrain3DAssetLayer::_exit_tree() {
	UtilityFunctions::print("Exit tree");
	_destroy_instancer();
	_destroy_containers();
	_initialized = false;
}

void Terrain3DAssetLayer::_ready() {
	UtilityFunctions::print("Ready");
}

void Terrain3DAssetLayer::_init() {
}

void Terrain3DAssetLayer::_initialize() {
	/*if (_terrain != p_terrain) {
		LOG(INFO, "Terrain asset layer already initialized");
		_terrain = p_terrain;
	}*/

	if (_terrain == nullptr) {
		LOG(DEBUG, "Set the terrain first");
		return;
	}

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

	// Initialize the system
	if (!_initialized && is_inside_tree()) {
		_build_containers();

		//_mesh_asset->initialize(this);
		_instancer->set_asset_layer(this);
		_instancer->initialize(_terrain);

		_initialized = true;
	}

	// Initialize the system
	//if (!_initialized && _is_inside_world && is_inside_tree()) {
	//	//_mesh_asset->initialize(this);
	//	_instancer->set_asset_layer(this);
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
		_initialize();
		//emit_signal("assets_changed");
	}
}

void Terrain3DAssetLayer::set_assets(const Ref<Terrain3DAssets> &p_assets) {
	if (_assets != p_assets) {
		//_clear_meshes();
		LOG(INFO, "Setting asset list");
		_assets = p_assets;
		_initialize();
		//emit_signal("assets_changed");
	}
}

void Terrain3DAssetLayer::_notification(int p_notification) {
	if (p_notification == NOTIFICATION_POSTINITIALIZE) {
		//UtilityFunctions::print("NOTIFICATION_POSTINITIALIZE received!");
		// Perform any setup needed after initialization
	}
	if (p_notification == NOTIFICATION_ENTER_WORLD) {
		//UtilityFunctions::print("NOTIFICATION_ENTER_WORLD received!");
		// Perform any setup needed after initialization
	}
	if (p_notification == NOTIFICATION_EXIT_WORLD) {
		//UtilityFunctions::print("NOTIFICATION_EXIT_WORLD received!");
		// Perform any setup needed after initialization
	}
}

void Terrain3DAssetLayer::_build_containers() {
	if (_terrain == nullptr) {
		LOG(DEBUG, "Set the terrain first");
		return;
	}

	String index = String::num_int64(std::chrono::system_clock::now().time_since_epoch().count() % 10000);
	_al_mmi_parent_node_name = "AL_MMI_" + index;

	_al_mmi_parent_node = memnew(Node3D);
	_al_mmi_parent_node->set_name(_al_mmi_parent_node_name);
	_terrain->add_child(_al_mmi_parent_node, true);
}

void Terrain3DAssetLayer::_destroy_containers() {
	if (_al_mmi_parent_node != nullptr) {
		_al_mmi_parent_node->queue_free();
	}
}

void Terrain3DAssetLayer::_bind_methods() {
	// bind the class methods
	ClassDB::bind_method(D_METHOD("_initialize"), &Terrain3DAssetLayer::_initialize);
	ClassDB::bind_method(D_METHOD("set_terrain", "terrain"), &Terrain3DAssetLayer::set_terrain);
	ClassDB::bind_method(D_METHOD("get_terrain"), &Terrain3DAssetLayer::get_terrain);
	ClassDB::bind_method(D_METHOD("set_assets", "assets"), &Terrain3DAssetLayer::set_assets);
	ClassDB::bind_method(D_METHOD("get_assets"), &Terrain3DAssetLayer::get_assets);
	ClassDB::bind_method(D_METHOD("get_instancer"), &Terrain3DAssetLayer::get_instancer);
	ClassDB::bind_method(D_METHOD("_build_containers"), &Terrain3DAssetLayer::_build_containers);

	// create the UI properties
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "terrain", PROPERTY_HINT_NODE_TYPE, "Terrain3D"), "set_terrain", "get_terrain");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "assets", PROPERTY_HINT_RESOURCE_TYPE, "Terrain3DAssets"), "set_assets", "get_assets");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "instancer", PROPERTY_HINT_NONE, "Terrain3DInstancer", PROPERTY_USAGE_NONE), "", "get_instancer");
}
