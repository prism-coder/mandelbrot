#pragma once

#include "Core/Core.h"

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <memory>
#include <thread>
#include <sstream>
#include <filesystem>

#ifdef _WIN32
#include <process.h>
#else
#include <unistd.h>
#endif

struct LogSpecification {
	std::string Name = "Application";
	std::filesystem::path Filepath = "Application.log";
	bool WriteToFile = true;
};

class Log {
public:
	enum class Level {
		Trace,
		Debug,
		Info,
		Warning,
		Error,
		Fatal
	};

	static void Init(const LogSpecification& specification = {});

	static void SetLevel(Level level);
	static void SetAppName(const std::string& name);
	static void SetLogFile(const std::filesystem::path& filepath);

	static void Trace(const std::string& message);
	static void Debug(const std::string& message);
	static void Info(const std::string& message);
	static void Warning(const std::string& message);
	static void Error(const std::string& message);
	static void Fatal(const std::string& message);

	static const LogSpecification& GetSpecification() { return s_Specification; }
private:
	static void Write(Level level, const std::string& message);
	static const char* LevelToString(Level level);
	static int GetProcessID();
private:
	Log() = default;

	inline static LogSpecification s_Specification;
	inline static Level s_CurrentLevel = Level::Trace;
	inline static bool s_UseFile = false;
	inline static Scope<std::ofstream> s_LogFile = nullptr;
};