#include "Log.h"

#include <iostream>

Log* Log::instance = NULL;

Log::Log(const std::string& Filename = "out.log") : filename(Filename), logFile(Filename) {
	write("Log Open\n");
}

Log::~Log() {
	(*instance) << "Log Closed (" << filename << ")\n";
	logFile.close();
}

Log& Log::getInstance() {
	if(!instance) {
		instance = new Log();
	}

	return *instance;
}

void Log::destroy() {
	delete instance;
}

void Log::write(const std::string& Message) {
	std::cerr << Message;
	logFile << Message;
	logFile.flush();
}