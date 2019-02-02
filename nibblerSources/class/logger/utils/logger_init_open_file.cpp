#include <logger.h>
#include <iostream>
int fd(int fd) {
	static int g_log_fd;
	if (fd > 0) {
		g_log_fd = fd;
	}
	return g_log_fd;
}

int	logger_init_open_file(char *log_file)
{
	fd(open(log_file, O_WRONLY | O_APPEND | O_CREAT, 0755));
	return fd(-1);
}
