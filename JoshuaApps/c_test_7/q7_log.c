#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
// #include "logger.h"
#include <syslog.h>
#include <limits.h>
#include <libgen.h>

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

#define LOG_LEVEL_DEBUG     0
#define LOG_LEVEL_INFO      1
#define LOG_LEVEL_WARN      2
#define LOG_LEVEL_ERROR     3

#define CURRENT_LOG_LEVEL LOG_LEVEL_DEBUG

#define LOGD(fmt, ...)  log_internal(LOG_LEVEL_DEBUG,  __FILE__, __func__, __LINE__, fmt, ##__VA_ARGS__)
#define LOGI(fmt, ...)  log_internal(LOG_LEVEL_INFO,  __FILE__, __func__, __LINE__, fmt, ##__VA_ARGS__)
#define LOGW(fmt, ...)  log_internal(LOG_LEVEL_WARN,  __FILE__, __func__, __LINE__, fmt, ##__VA_ARGS__)
#define LOGE(fmt, ...)  log_internal(LOG_LEVEL_ERROR, __FILE__, __func__, __LINE__, fmt, ##__VA_ARGS__)

#define LOG_FILENAME "log_ctest.txt"
#define LOG_FOLDER "mylogs"
#define LOG_PATH_SIZE PATH_MAX


void log_internal(int level, const char *file, const char *func, int line, const char *fmt, ...)
{
    if (level < CURRENT_LOG_LEVEL)
        return;

    //file and path
    static int initialized = 0;
    int syslog_level = LOG_INFO;
    char logpath[LOG_PATH_SIZE] = {0};
    static char full_log_file[LOG_PATH_SIZE] = {0};
    static char exe_path[PATH_MAX] = {0};
    static char exe_dir[PATH_MAX] = {0};
    char * dir = NULL;
    FILE * fptr = NULL;
    ssize_t len = {0};

    //log
    va_list args;
    va_start(args, fmt);
    va_list args_file, args_syslog;
    va_copy(args_file, args);      //file
    va_copy(args_syslog, args);      //syslog
    const char *level_str = {0};
    char log_msg [1024];

    //time counter
    int millisec = 0;
    struct timeval tv;
    struct tm *ltime;
    char timestamp[64] = {0};


    //set log level
    level_str = "UNKNOWN";
    switch (level)
    {
        case LOG_LEVEL_DEBUG:
            level_str = "DEBUG"; 
            syslog_level = LOG_DEBUG;
            break;
        case LOG_LEVEL_INFO: 
            level_str = "INFO";  
            syslog_level = LOG_INFO;
            break;
        case LOG_LEVEL_WARN: 
            level_str = "WARN";  
            syslog_level = LOG_WARNING;
            break;
        case LOG_LEVEL_ERROR: 
            level_str = "ERROR"; 
            syslog_level = LOG_ERR;
            break;
        default: 
            level_str = "UNKNOWN"; 
            break;
    }

    gettimeofday(&tv, NULL);
    ltime = localtime(&tv.tv_sec);
    millisec = tv.tv_usec / 1000;

    //create timestamp
    snprintf(timestamp, sizeof(timestamp), "[%02d:%02d:%02d.%03d]", 
        ltime->tm_hour, 
        ltime->tm_min, 
        ltime->tm_sec, 
        millisec); 

    //init log file path
    if (!initialized){
        len = readlink("/proc/self/exe", exe_path, sizeof(exe_path) - 1);
        if (len == -1) {
            perror("Failed to resolve /proc/self/exe");
            return;
        }

        exe_path[len] = '\0';
        dir = dirname(exe_path);
        strncpy(exe_dir, dir, sizeof(exe_dir) - 1);

        snprintf(logpath, sizeof(logpath), "%s/%s", dir, LOG_FOLDER);
        mkdir(logpath, 0755);

        snprintf(full_log_file, sizeof(full_log_file), "%s/%s", logpath, LOG_FILENAME);
        initialized = 1;
    }

    fptr = fopen(full_log_file, "a+");
    if (!fptr){
        perror("Failed to open log file\n");
        return;
    }
    
    // Print to log file
    fprintf(fptr, "[%s %s:%d %s()] ", level_str, file, line, func);
    vfprintf(fptr, fmt, args);
    fprintf(fptr, "\n");

    // Print to console
    fprintf(stderr, "[%s %s:%d %s()] ", level_str, file, line, func);
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");

    // Print to dmesg/syslog
    char syslog_buf[1024];
    vsnprintf(syslog_buf, sizeof(syslog_buf), fmt, args);
    syslog(LOG_INFO, "[%s %s:%d %s()] %s", level_str, file, line, func, syslog_buf);

    va_end(args);
    va_end(args_file);
    va_end(args_syslog);
}

void test_main()
{
    LOGD("Debug message");
    LOGI("System initialized.");
    LOGW("Low memory warning.");
    LOGE("Critical failure detected!");
}

int main()
{
    test_main();
    return 0;

    FILE * fp = NULL;
    char string[1024] = {};

    fp = popen("dmesg", "r");
    if (!fp){
        perror("Fail to open dmesg\n exit\n");
        return 1;
    }

    openlog("q7_log", LOG_PID | LOG_CONS, LOG_USER);

    while (fgets(string , sizeof(string), fp) )
    {   
        LOGI(string);                    //save to logfile
    }
    
    pclose(fp);
    closelog();

    return 0;
}
