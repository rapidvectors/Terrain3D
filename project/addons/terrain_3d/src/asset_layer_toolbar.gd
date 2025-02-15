# Copyright © 2025 Cory Petkovsek, Roope Palmroos, and Contributors.
# Toolbar for Terrain3D
extends VFlowContainer

signal tool_changed(p_tool: Terrain3DEditor.Tool, p_operation: Terrain3DEditor.Operation)

const ICON_INSTANCER: String = "res://addons/terrain_3d/icons/multimesh.svg"

var add_tool_group: ButtonGroup = ButtonGroup.new()
var sub_tool_group: ButtonGroup = ButtonGroup.new()


func _init() -> void:
	set_custom_minimum_size(Vector2(20, 0))

func _ready() -> void:
	add_tool_group.pressed.connect(_on_tool_selected)
	sub_tool_group.pressed.connect(_on_tool_selected)

	add_tool_button({ "tool":Terrain3DEditor.INSTANCER,
		"add_text":"Instance Meshes", "add_op":Terrain3DEditor.ADD, "add_icon":ICON_INSTANCER,
		"sub_text":"Remove Meshes", "sub_op":Terrain3DEditor.SUBTRACT })

	# Select first button
	var buttons: Array[BaseButton] = add_tool_group.get_buttons()
	buttons[0].set_pressed(true)
	show_add_buttons(true)


func add_tool_button(p_params: Dictionary) -> void:
	# Additive button
	var button := Button.new()
	button.set_name(p_params.get("add_text", "blank").to_pascal_case())
	button.set_meta("Tool", p_params.get("tool", 0))
	button.set_meta("Operation", p_params.get("add_op", 0))
	button.set_meta("ID", add_tool_group.get_buttons().size() + 1)
	button.set_tooltip_text(p_params.get("add_text", "blank"))
	button.set_button_icon(load(p_params.get("add_icon")))
	button.set_flat(true)
	button.set_toggle_mode(true)
	button.set_h_size_flags(SIZE_SHRINK_END)
	button.set_button_group(p_params.get("group", add_tool_group))
	add_child(button, true)

	# Subtractive button
	var button2: Button
	if p_params.has("sub_text"):
		button2 = Button.new()
		button2.set_name(p_params.get("sub_text", "blank").to_pascal_case())
		button2.set_meta("Tool", p_params.get("tool", 0))
		button2.set_meta("Operation", p_params.get("sub_op", 0))
		button2.set_meta("ID", button.get_meta("ID"))
		button2.set_tooltip_text(p_params.get("sub_text", "blank"))
		button2.set_button_icon(load(p_params.get("sub_icon", p_params.get("add_icon"))))
		button2.set_flat(true)
		button2.set_toggle_mode(true)
		button2.set_h_size_flags(SIZE_SHRINK_END)
	else:
		button2 = button.duplicate()
	button2.set_button_group(p_params.get("group", sub_tool_group))
	add_child(button2, true)


func show_add_buttons(p_enable: bool) -> void:
	for button in add_tool_group.get_buttons():
		button.visible = p_enable
	for button in sub_tool_group.get_buttons():
		button.visible = !p_enable


func _on_tool_selected(p_button: BaseButton) -> void:
	# Select same tool on negative bar
	var group: ButtonGroup = p_button.get_button_group()
	var change_group: ButtonGroup = add_tool_group if group == sub_tool_group else sub_tool_group
	var id: int = p_button.get_meta("ID", -2)
	for button in change_group.get_buttons():
		button.set_pressed_no_signal(button.get_meta("ID", -1) == id)
	
	emit_signal("tool_changed", p_button.get_meta("Tool", Terrain3DEditor.TOOL_MAX), p_button.get_meta("Operation", Terrain3DEditor.OP_MAX))
