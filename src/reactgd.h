#ifndef REACTGD_H
#define REACTGD_H
#include <Godot.hpp>

godot_pluginscript_language_data *lps_language_init();
void lps_language_finish(godot_pluginscript_language_data *data);
void lps_language_add_global_constant(godot_pluginscript_language_data *data, const godot_string *name, const godot_variant *value);
godot_pluginscript_script_manifest lps_script_init(godot_pluginscript_language_data *data, const godot_string *path, const godot_string *source, godot_error *error);
void lps_script_finish(godot_pluginscript_script_data *data);
godot_pluginscript_instance_data *lps_instance_init(godot_pluginscript_script_data *data, godot_object *owner);
void lps_instance_finish(godot_pluginscript_instance_data *data);
godot_bool lps_instance_set_prop(godot_pluginscript_instance_data *data, const godot_string *name, const godot_variant *value);
godot_bool lps_instance_get_prop(godot_pluginscript_instance_data *data, const godot_string *name, godot_variant *ret);
godot_variant lps_instance_call_method(godot_pluginscript_instance_data *data, const godot_string_name *method, const godot_variant **args, int argcount, godot_variant_call_error *error);
void lps_instance_notification(godot_pluginscript_instance_data *data, int notification);

#endif