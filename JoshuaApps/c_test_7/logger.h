#ifndef LOGGER_H
#define LOGGER_H

void log_internal(FILE *fp, int level, const char *file, const char *func, int line, const char *fmt, ...);

#endif
