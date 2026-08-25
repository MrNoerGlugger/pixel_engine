#include "Logger.hpp"

std::mutex Logger::mtx;
path Logger::log_file;

void Logger::log(std::string log_string) {
    //use a mutex for thread safety
    mtx.lock();

    //add time & date info to the log string
    std::time_t current_time = std::time(0);
    std::tm* now = std::localtime(&current_time);
    log_string = std::format("[{:02}:{:02}:{:02}|{:02}.{:02}.{}]{}",now->tm_hour, now->tm_min, now->tm_sec, now->tm_mday, now->tm_mon + 1, now->tm_year + 1900, log_string);

    //print & save the log string
    std::cout << log_string;
    std::fstream log_stream;
    log_stream.open(log_file, std::ios_base::out | std::ios_base::in);
    if (!log_stream.is_open()) {
        std::cout << "[ERROR]:Failed to open log file!\n";
    }
    log_stream.seekp(0, std::fstream::end);
    log_stream << log_string;
    log_stream.close();
    mtx.unlock();
}

void Logger::log_info(std::string log_string) {
    log("[INFO]:" + log_string + "\n");
}

void Logger::log_warning(std::string log_string) {
    log("[WARNING]:" + log_string + "\n");
}

void Logger::log_error(std::string log_string) {
    log("[ERROR]:" + log_string + "\n");
}

void Logger::log_debug(std::string log_string) {
    if(!in_debug_mode) {
        return;
    }
    log("[DEBUG]:" + log_string + "\n");
}


void Logger::create_log_file()  {
    log_file = FilePath::filePath;
    log_file += "log.txt";
    std::ofstream {log_file};
}