#include "reactgd.h"

godot_pluginscript_language_desc lps_language_desc = {
    .name = "GDX",
    .type = "GDX",
    .extension = "gdx",
    .recognized_extensions = (const char *[]){"gdx", NULL},
    .init = &lps_language_init,
    .finish = &lps_language_finish,
    .reserved_words = (const char *[]){
        "and", "break", "do", "else", "elseif", "end",
        "false", "for", "function", "goto", "if", "in",
        "local", "nil", "not", "or", "repeat", "return",
        "then", "true", "until", "while",
        "self", "_G", "_ENV", "_VERSION",
        NULL},
    .comment_delimiters = (const char *[]){"--", "--[[ ]]", NULL},
    .string_delimiters = (const char *[]){"' '", "\" \"", "[[ ]]", "[=[ ]=]", NULL},
    .has_named_classes = false,
    .supports_builtin_mode = false,

    .add_global_constant = &lps_language_add_global_constant,
    .script_desc = {
        .init = &lps_script_init,
        .finish = &lps_script_finish,
        .instance_desc = {
            .init = &lps_instance_init,
            .finish = &lps_instance_finish,
            .set_prop = &lps_instance_set_prop,
            .get_prop = &lps_instance_get_prop,
            .call_method = &lps_instance_call_method,
            .notification = &lps_instance_notification,
        },
    },
};

extern "C" void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options *o)
{
    godot::Godot::gdnative_init(o);
}

extern "C" void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options *o)
{
    godot::Godot::gdnative_terminate(o);
}

extern "C" void GDN_EXPORT godot_gdnative_singleton(void *handle) {
    godot_pluginscript_register_language(&lps_language_desc);
    godot::Godot::print("Hello world!");
}

extern "C" void GDN_EXPORT godot_nativescript_init(void *handle)
{
    godot::Godot::nativescript_init(handle);
}
