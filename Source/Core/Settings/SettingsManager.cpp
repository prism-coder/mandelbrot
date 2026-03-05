#include "SettingsManager.h"

#include "Core/Log.h"

#include "Core/Settings/SettingsSerializer.h"

bool SettingsManager::Load(const std::filesystem::path& filepath) {
	Log::Trace("SettingsManager::Load - Loading Settings");

	// Create a SettingsSerializer instance to handle the deserialization of settings.
	SettingsSerializer serializer(s_Settings);

	// Attempt to deserialize the settings from the specified file path.
	if (serializer.Deserialize(filepath)) {
		Log::Info("SettingsManager::Load - Settings loaded: " + filepath.string());

		// Return true to indicate success.
		return true;
	}

	Log::Error("SettingsManager::Load - Couldn't load Settings: " + filepath.string());

	// Return false to indicate failure.
	return false;
}

bool SettingsManager::Save(const std::filesystem::path& filepath) {
	Log::Trace("SettingsManager::Save - Saving Settings");

	// Create a SettingsSerializer instance to handle the serialization of settings.
	SettingsSerializer serializer(s_Settings);

	// Attempt to serialize the settings to the specified file path.
	if (serializer.Serialize(filepath)) {
		Log::Info("SettingsManager::Save - Settings saved: " + filepath.string());

		// Return true to indicate success.
		return true;
	}

	Log::Error("SettingsManager::Save - Couldn't save Settings: " + filepath.string());

	// Return false to indicate failure.
	return false;
}
