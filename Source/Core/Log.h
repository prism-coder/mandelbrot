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

/**
 * Represents the specification for the logging system.
 * 
 * It includes the name of the application, the filepath for the log file, and whether to write logs to a file or not.
 */
struct LogSpecification {
	/// @brief The name of the application, used in log messages and as the default log file name.
	std::string Name = "Application";

	/// @brief The path to the log file. By default, it is set to "Application.log".
	std::filesystem::path Filepath = "Application.log";

	/// @brief A boolean flag indicating whether to write logs to a file. If set to true, logs will be written to the specified file; otherwise, they will only be output to the console.
	bool WriteToFile = true;
};

/**
 * The log levels supported by the logging system, ordered from least severe to most severe.
 * 
 * Each level represents a different severity of log message.
 * 
 * The logging system will only output messages that are at or above the currently set log level.
 */
enum class Level {
	/// @brief The most detailed level, used for fine-grained debugging information.
	Trace,

	/// @brief Used for general debugging information that may be useful during development.
	Debug,

	/// @brief Used for informational messages that highlight the progress of the application.
	Info,

	/// @brief Used for potentially harmful situations that do not cause the application to stop but may indicate a problem.
	Warning,

	/// @brief Used for error events that might still allow the application to continue running, but indicate a significant problem.
	Error,

	/// @brief The most severe level, used for critical errors that cause the application to terminate immediately.
	Fatal
};

/**
 * A logging system that provides various log levels (`Trace`, `Debug`, `Info`, `Warning`, `Error`, `Fatal`) and allows for logging messages to both the console and a file.
 * 
 * The `Log` class is designed to be initialized with a `LogSpecification`, which configures the logging behavior.
 * It provides static methods for logging messages at different levels, as well as methods for setting the log level, application name, and log file path.
 */
class Log {
public:
	/**
	 * Initializes the logging system with the given specification.
	 * If no specification is provided, it uses default values.
	 * 
	 * @param specification The configuration for the logging system, including application name, log file path, and whether to write logs to a file.
	 */
	static void Init(const LogSpecification& specification = {});

	/**
	 * Sets the current log level.
	 * Only messages at or above this level will be logged.
	 * 
	 * @param level The log level to set. Messages below this level will be ignored.
	 */
	static void SetLevel(Level level);

	/**
	 * Sets the name of the application, which is used in log messages and as the default log file name.
	 * 
	 * @param name The name of the application to set. This will be included in log messages and used as the default name for the log file if no custom filepath is provided.
	 */
	static void SetAppName(const std::string& name);

	/**
	 * Sets the log file path. If a log file is currently open, it will be closed before opening the new file.
	 * If the specified file cannot be opened, an error message will be logged to the console. 
	 * 
	 * @param filepath The path to the log file to set.
	 */
	static void SetLogFile(const std::filesystem::path& filepath);

	/**
	 * Logs a message at the `Trace` level.
	 * 
	 * @param message The message to log.
	 */
	static void Trace(const std::string& message);

	/**
	 * Logs a message at the `Debug` level.
	 * 
	 * @param message The message to log.
	 */
	static void Debug(const std::string& message);

	/**
	 * Logs a message at the `Info` level.
	 * 
	 * @param message The message to log.
	 */
	static void Info(const std::string& message);

	/**
	 * Logs a message at the `Warning` level.
	 * 
	 * @param message The message to log.
	 */
	static void Warning(const std::string& message);

	/**
	 * Logs a message at the `Error` level.
	 * 
	 * @param message The message to log.
	 */
	static void Error(const std::string& message);

	/**
	 * Logs a message at the `Fatal` level.
	 * 
	 * @param message The message to log.
	 */
	static void Fatal(const std::string& message);

	/**
	 * Retrieves the current log specification, which includes the application name, log file path, and whether to write logs to a file.
	 * 
	 * @return A constant reference to the current `LogSpecification` used by the logging system.
	 */
	static const LogSpecification& GetSpecification() { return s_Specification; }
private:
	/**
	 * Writes a log message to the console and, if enabled, to the log file.
	 * The message is formatted with a timestamp, log level, application name, process ID, and thread ID.
	 * 
	 * @param level The log level of the message being written. Only messages at or above the current log level will be logged.
	 * @param message The message to log.
	 */
	static void Write(Level level, const std::string& message);

	/**
	 * Converts a log level to its corresponding string representation.
	 * For example, `Level::Trace` will be converted to `TRACE`, `Level::Debug` to `DEBUG`, and so on.
	 * 
	 * @param level The log level to convert to a string.
	 * @return A string representation of the log level.
	 */
	static const char* LevelToString(Level level);

	/**
	 * Retrieves the current process ID.
	 * This is used in log messages to identify which process is generating the log entry, especially useful in multi-process applications.
	 * 
	 * @return The process ID of the current process.
	 */
	static int GetProcessID();
private:
	Log() = default;

	/// @brief The current log specification, which includes the application name, log file path, and whether to write logs to a file.
	inline static LogSpecification s_Specification;

	/// @brief The current log level. Only messages at or above this level will be logged.
	inline static Level s_CurrentLevel = Level::Trace;
	
	/// @brief Indicates whether to write logs to a file or not.
	inline static bool s_UseFile = false;

	/// @brief The log file stream. If `s_UseFile` is true, logs will be written to this file.
	inline static Scope<std::ofstream> s_LogFile = nullptr;
};