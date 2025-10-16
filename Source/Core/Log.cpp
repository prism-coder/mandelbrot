#include "Log.h"

const char* Log::LevelToString(Level level) {
	switch (level) {
		case Level::Trace:   return "TRACE";
		case Level::Debug:   return "DEBUG";
		case Level::Info:    return "INFO";
		case Level::Warning: return "WARN";
		case Level::Error:   return "ERROR";
		case Level::Fatal:   return "FATAL";
		default:             return "UNKNOWN";
	}
}

int Log::GetProcessID() {
	return getpid();
}

void Log::Init(const LogSpecification& specification) {
	s_Specification = specification;

	if (s_Specification.WriteToFile) {
		SetLogFile(s_Specification.Filepath);
	}
}

void Log::SetLevel(Level level) {
	s_CurrentLevel = level;
}

void Log::SetAppName(const std::string& name) {
	s_Specification.Name = name;
}

void Log::SetLogFile(const std::filesystem::path& filepath) {
	if (s_LogFile && s_LogFile->is_open()) {
		s_LogFile->close();
	}

	if (!filepath.parent_path().empty()) {
		std::filesystem::create_directories(filepath.parent_path());
	}

	s_LogFile = CreateScope<std::ofstream>(filepath, std::ios::out | std::ios::app);
	*s_LogFile << "";
	s_UseFile = s_LogFile && s_LogFile->is_open();

	if (!s_UseFile) {
		Error("Couldn't open the log file: " + filepath.string());
	}
}

void Log::Trace(const std::string& message) {
	Write(Level::Trace, message);
}

void Log::Debug(const std::string& message) {
	Write(Level::Debug, message);
}

void Log::Info(const std::string& message) {
	Write(Level::Info, message);
}

void Log::Warning(const std::string& message) {
	Write(Level::Warning, message);
}

void Log::Error(const std::string& message) {
	Write(Level::Error, message);
}

void Log::Fatal(const std::string& message) {
	Write(Level::Fatal, message);
}

void Log::Write(Level level, const std::string& message) {
	if (level < s_CurrentLevel) {
		return;
	}

	auto now = std::chrono::system_clock::now();
	auto nowAsTimeT = std::chrono::system_clock::to_time_t(now);
	auto nowMs = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count() % 1000;

	std::ostringstream threadIdStream;
	threadIdStream << std::this_thread::get_id();

	std::stringstream logStream;
	logStream << std::put_time(std::gmtime(&nowAsTimeT), "%Y-%m-%dT%H:%M:%S")
		<< '.' << std::setfill('0') << std::setw(3) << nowMs << "Z "
		<< std::setfill(' ') << std::left << std::setw(7) << LevelToString(level) << " "
		<< GetProcessID() << " "
		<< "--- [" << s_Specification.Name << "] "
		<< "[" << std::setfill('0') << std::right << std::setw(15) << threadIdStream.str() << "] "
		<< ": " << message << std::endl;

	std::ostream& consoleStream = (level >= Level::Error) ? std::cerr : std::cout;
	consoleStream << logStream.str();

	if (s_UseFile) {
		*s_LogFile << logStream.str();
		s_LogFile->flush();
	}
}
