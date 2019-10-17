#pragma once

#include <string>
#include <fstream>
#include <sstream>

class Log
{
public :
	Log(const std::string& Filename);
	~Log();
	static Log& getInstance();
	static void destroy();
	template <class T> Log& operator <<(const T& ToLog);
 
private :
	const std::string filename;
	void write(const std::string& Message);
	static Log* instance;
	std::ofstream logFile;
};

template <class T> Log& Log::operator <<(const T& ToLog) {
	std::ostringstream stream;
	stream << ToLog;
	write(stream.str());

	return *instance;
}

#define LOG Log::getInstance()
#define ENDL "\n"