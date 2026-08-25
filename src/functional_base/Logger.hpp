#pragma once

#include <iostream>
#include <format>
#include <ctime>
#include <mutex>

#include <FilePath.hpp>

#include <SDL3/SDL.h>

extern const bool in_debug_mode;

class Logger {
    static std::mutex mtx;
    static path log_file;
    static void log(std::string log_string);
public:
    static void log_info(std::string log_string);

    static void log_warning(std::string log_string);

    static void log_error(std::string log_string);

    static void log_debug(std::string log_string);


    static void create_log_file();
};