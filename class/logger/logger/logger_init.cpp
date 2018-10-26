#include <logger.h>

int logger_init( char *log_file) {
	int ret;

	if ((ret = logger_init_open_file(log_file)) < 0)
		return (ret);
	dprintf(fd(-1), "\n\033[32m>>> \033[0m");
	dprintf(fd(-1), "\033[32mNEW INSTANCE OF THE APPLICATION\033[0m");
	dprintf(fd(-1), "\033[32m <<<\033[0m\n\n");
	return (0);
}
