#include "stdafx.h"
#include "UIStyleSettings.h"


UIStyleSettings::UIStyleSettings()
{
	api_ = (RimeLeversApi*)rime_get_api()->find_module("levers")->get_api();
	settings_ = api_->custom_settings_init("weasel", "Weasel::UIStyleSettings");
}

bool UIStyleSettings::GetPresetColorSchemes(std::vector<ColorSchemeInfo>* result) {
	if (!result) return false;
	result->clear();
	RimeConfig config = {0};
	api_->settings_get_config(settings_, &config);
	RimeApi* rime = rime_get_api();
	RimeConfigIterator preset = {0};
	if (!rime->config_begin_map(&preset, &config, "preset_color_schemes")) {
		return false;
	}
	while (rime->config_next(&preset)) {
		std::string name_key(preset.path);
		name_key += "/name";
		const char* name = rime->config_get_cstring(&config, name_key.c_str());
		std::string author_key(preset.path);
		author_key += "/author";
		const char* author = rime->config_get_cstring(&config, author_key.c_str());
		if (!name) continue;
		ColorSchemeInfo info;
		info.color_scheme_id = preset.key;
		info.name = name;
		if (author) info.author = author;
		result->push_back(info);
	}
	return true;
}

std::string UIStyleSettings::GetColorSchemePreview(const std::string& color_scheme_id) {
	return std::string(rime_get_api()->get_shared_data_dir())
		+ "\\preview\\color_scheme_" + color_scheme_id + ".png";
}

std::string UIStyleSettings::GetActiveColorScheme() {
	RimeConfig config = {0};
	api_->settings_get_config(settings_, &config);
	const char* value = rime_get_api()->config_get_cstring(&config, "style/color_scheme");
	if (!value) return std::string();
	return std::string(value);
}

bool UIStyleSettings::SelectColorScheme(const std::string& color_scheme_id) {
	api_->customize_string(settings_, "style/color_scheme", color_scheme_id.c_str());
	return true;
}
