#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <syslog.h>

void log_internal(FILE *fp, int level, const char *file, const char *func, int line, const char *fmt, ...) 
{
    va_list args;
    va_start(args, fmt);

    const char *level_str = "";
    switch (level) {
        case 0: level_str = "DEBUG"; break;
        case 1: level_str = "INFO"; break;
        case 2: level_str = "WARN"; break;
        case 3: level_str = "ERROR"; break;
        default: level_str = "UNKNOWN"; break;
    }

    //format = timestamp : log_level : file : line : function
    fprintf(fp, "[%s %s:%d %s()] ", level_str, file, line, func);               //print to file
    vfprintf(fp, fmt, args);
    fprintf(fp, "\n");

    //print to console
    fprintf(stderr, "[%s %s:%d %s()] ", level_str, file, line, func);               //print to file
    vfprintf(stderr, fmt, args);
    //fprintf(stderr, "\n");

    //print to kernel (dmesg)
    // printk("[%s %s:%d %s()] ", level_str, file, line, func);                 //kernel-level only. userspace cannot use
    syslog(LOG_INFO, "[%s %s:%d %s()] ...", level_str, file, line, func);       //save to dmesg log
    closelog();                                                                 //close path to dmesg log

    va_end(args);
}
