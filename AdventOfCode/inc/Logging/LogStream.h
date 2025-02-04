#pragma once

#include <ostream>
#include <memory>
#include <source_location>

struct LogStream : public std::ostream {
	using LogFunc = std::function<void(const std::string&, std::source_location)>;
	LogStream(LogFunc log);
	~LogStream();

private:
	struct LogStreamBuffer;
	LogStream(std::unique_ptr<LogStreamBuffer>&& buffer);
	std::unique_ptr<LogStreamBuffer> m_Buffer;
};