#include "reactgd.h"

using namespace godot;

// Called when our language runtime will be initialized
godot_pluginscript_language_data *lps_language_init() {
    // TODO
    return NULL;
}

// Called when our language runtime will be terminated
void lps_language_finish(godot_pluginscript_language_data *data) {
    // TODO
}

// Called when Godot registers globals in the language, such as Autoload nodes
void lps_language_add_global_constant(godot_pluginscript_language_data *data, const godot_string *name, const godot_variant *value) {
    // TODO
}

// Called when a Lua script is loaded, e.g.: const SomeScript = preload("res://some_script.lua")
godot_pluginscript_script_manifest lps_script_init(godot_pluginscript_language_data *data, const godot_string *path, const godot_string *source, godot_error *error) {
    godot_pluginscript_script_manifest manifest = {};
    // All Godot objects must be initialized, or else our plugin SEGFAULTs
    godot_string_name_new_data(&manifest.name, "");
    godot_string_name_new_data(&manifest.base, "");
    godot_dictionary_new(&manifest.member_lines);
    godot_array_new(&manifest.methods);
    godot_array_new(&manifest.signals);
    godot_array_new(&manifest.properties);
    // TODO
    return manifest;
}

// Called when a Lua script is unloaded
void lps_script_finish(godot_pluginscript_script_data *data) {
    // TODO
}

// Called when a Script Instance is being created, e.g.: var instance = SomeScript.new()
godot_pluginscript_instance_data *lps_instance_init(godot_pluginscript_script_data *data, godot_object *owner) {
    // TODO
    return NULL;
}

// Called when a Script Instance is being finalized
void lps_instance_finish(godot_pluginscript_instance_data *data) {
    // TODO
}

// Called when setting a property on an instance, e.g.: instance.prop = value
godot_bool lps_instance_set_prop(godot_pluginscript_instance_data *data, const godot_string *name, const godot_variant *value) {
    // TODO
    return false;
}

// Called when getting a property from instance, e.g.: var value = instance.prop
godot_bool lps_instance_get_prop(godot_pluginscript_instance_data *data, const godot_string *name, godot_variant *ret) {
    // TODO
    return false;
}

// Called when calling a method on an instance, e.g.: instance.method(args)
godot_variant lps_instance_call_method(godot_pluginscript_instance_data *data, const godot_string_name *method, const godot_variant **args, int argcount, godot_variant_call_error *error) {
    // TODO
	godot_variant ret;
	godot_variant_new_nil(&ret);
    return ret;
}

// Called when a notification is sent to instance
void lps_instance_notification(godot_pluginscript_instance_data *data, int notification) {
    // TODO
}