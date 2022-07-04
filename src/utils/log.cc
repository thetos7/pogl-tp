#include "log.hh"

#include <filesystem>

#define GRAY_CODE "\e[90m"
#define RESET_CODE "\e[39m"

namespace pogl::log
{
    namespace fs = std::filesystem;

    LogMessageDecorator::LogMessageDecorator(std::string style_str, std::string log_level,
                         std::string file, int line)
        : style_str(style_str)
        , log_level(log_level)
        , file(file)
        , line(line)
    {}

    std::ostream &operator<<(std::ostream &out, const LogMessageDecorator &header)
    {
        out << header.style_str << "[" << header.log_level << "]" GRAY_CODE "("
            << header.file << ":" << header.line << ")" RESET_CODE ": ";
        return out;
    }

    LogMessageDecorator info(const char *file, int line)
    {
        fs::path file_path(file);
        return LogMessageDecorator("\e[94m", "INFO", file_path.filename(), line);
    }
    LogMessageDecorator warning(const char *file, int line)
    {
        fs::path file_path(file);
        return LogMessageDecorator("\e[33m", "WARN", file_path.filename(), line);
    }
    LogMessageDecorator error(const char *file, int line)
    {
        fs::path file_path(file);
        return LogMessageDecorator("\e[31m", "!ERR", file_path.filename(), line);
    }
} // namespace pogl::log
