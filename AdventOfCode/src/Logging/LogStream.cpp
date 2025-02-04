#include "Logging/LogStream.h"
#include <sstream>

struct LogStream::LogStreamBuffer : public std::stringbuf {
	LogStreamBuffer(LogStream::LogFunc log) : Log(log) {}

	int sync() override {
		Log(str(), {});
		str("");
		return 0;
	}

	LogStream::LogFunc Log;
};

LogStream::LogStream(LogFunc log) 
	: LogStream(std::make_unique<LogStreamBuffer>(log))
{}

LogStream::LogStream(std::unique_ptr<LogStreamBuffer>&& buffer)
	: std::ostream(buffer.get())
	, m_Buffer(std::move(buffer))
{}

LogStream::~LogStream() {
	m_Buffer.reset();
}