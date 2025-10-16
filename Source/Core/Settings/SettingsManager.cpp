#include "SettingsManager.h"

#include "Core/Log.h"

#include "Core/Settings/SettingsSerializer.h"

bool SettingsManager::Load(const std::filesystem::path& filepath) {
	Log::Trace("SettingsManager::Load - Loading Settings");

	SettingsSerializer serializer(s_Settings);

	if (serializer.Deserialize(filepath)) {
		Log::Info("SettingsManager::Load - Settings loaded: " + filepath.string());

		return true;
	}

	Log::Error("SettingsManager::Load - Couldn't load Settings: " + filepath.string());

	return false;
}

bool SettingsManager::Save(const std::filesystem::path& filepath) {
	Log::Trace("SettingsManager::Save - Saving Settings");

	SettingsSerializer serializer(s_Settings);

	if (serializer.Serialize(filepath)) {
		Log::Info("SettingsManager::Save - Settings saved: " + filepath.string());

		return true;
	}

	Log::Error("SettingsManager::Save - Couldn't save Settings: " + filepath.string());

	return false;
}
