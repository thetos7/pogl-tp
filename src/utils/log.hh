#pragma once

#include <ostream>
#include <string>

namespace pogl::log
{
#define LOG_INFO (::pogl::log::info(__FILE__, __LINE__))
#define LOG_WARNING (::pogl::log::warning(__FILE__, __LINE__))
#define LOG_ERROR (::pogl::log::error(__FILE__, __LINE__))
    struct LogHeader
    {
        LogHeader(std::string style_str, std::string log_level,
                  std::string file, int line);
        std::string style_str;
        std::string log_level;
        std::string file;
        int line;
    };

    std::ostream &operator<<(std::ostream &out, const LogHeader &header);

    LogHeader info(const char *file, int line);
    LogHeader warning(const char *file, int line);
    LogHeader error(const char *file, int line);
} // namespace pogl::log
