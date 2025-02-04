#pragma once

#include "Core/Instrumentation/Logging.h"
#include "Core/Instrumentation/ISink.h"
#include "Core/Utilities/TimeUtils.h"

#include <fstream>
#include <iostream>

struct FileLogWriter : public Log::ISink {
    std::ofstream m_File{};

    FileLogWriter(Log::Filter filter = {}) : ISink(filter) {
        auto fileName = TimeUtils::TodayNowLocalToString("%Y-%m-%d_%H-%M-%S");
        fileName += ".log";
        m_File.open(fileName);
        if (!m_File.is_open()) {
            std::cerr << "Failed to open log file" << fileName << "\n";
            std::abort();
        }
    }
    void Write(const Log::Entry& entry) override {
        m_File << entry.Message << '\n';
    }
};

struct StdLogWriter : public Log::ISink {
    StdLogWriter(Log::Filter filter = {}) : ISink(filter) {
        // Disable synchronization with C stdio
        // No interop with C
        std::ios_base::sync_with_stdio(false);

        // Untie stdio streams from C++ streams
        // Not using cin, so no need to synchronize with cout
        std::cin.tie(nullptr);
    }
    void Write(const Log::Entry& entry) override {
        std::cout << entry.Message << '\n';
    }
};
