#ifndef LOGGER_UTILS_H
# define LOGGER_UTILS_H

# include <stdarg.h>
# define _GNU_SOURCE
# include <stdio.h>
# include <stdlib.h>
# include <time.h>
# include <fcntl.h>
# include <string.h>
# include <unistd.h>

/*
** Macro to get the basename of the __FILE__ macro.
*/

# define __FILENAME__	\
	(strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

/*
** Global variables, the file descriptor to write on, and the log level.
*/

int fd(int fd);

/*
** Tools functions.
*/

int		logger_init_open_file(char *log_file);

void	logger_fatal(int fd, char const *file, int line, const char *fmt, ...);
void	logger_error(int fd, const char *file, int line, const char *fmt, ...);
void	logger_warn(int fd, const char *file, int line, const char *fmt, ...);
void	logger_success(int fd, const char *file, int line, const char *fmt, ...);
void	logger_info(int fd, const char *file, int line, const char *fmt, ...);
void	logger_debug(int fd, const char *file, int line, const char *fmt, ...);
void	logger_trace(int fd, const char *file, int line, const char *fmt, ...);

char	*logger_get_time(void);

#endif
