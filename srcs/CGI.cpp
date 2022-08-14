//#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include "cgi_utils.hpp"

char *path_to_exec(char *pr)
{
	char	**all_paths;
	char	*path_;
	char	*path;
	int		a;

	a = 0;
	path = NULL;
	all_paths = ft_split(getenv("PATH"), ':');
	if (!all_paths)
		return (NULL);
	while (all_paths[a])
	{
		path_ = ft_strjoin(all_paths[a], "/");
		if (!path_)
			break ;
		path = ft_strjoin(path_, pr);
		free(path_);
		if (!path)
			break ;
		if (access(path, F_OK) == 0)
			break ;
		free(path);
		a++;
	}
	a = 0;
	while (all_paths[a])
	{
		free(all_paths[a]);
		++a;
	}
	free(all_paths);
	return (path);
}


void CGI()
{
	pid_t pid = 0;
	
	int fd = open("cgi.txt", O_CREAT | O_TRUNC | O_RDWR);
	if (fd == -1)
		exit(EXIT_FAILURE);
	char *cmd[3];
	if (!(cmd[0] = path_to_exec("EXTENSION")))
		return ;
	if (!(cmd[1] = strdup("SCRIPT")))
	{
		free(cmd[0]);
		return ;
	}
	cmd[2] = NULL;
	free(cmd[0]);
	free(cmd[1]);
}

/*
int main()
{
	char *str = getenv("PATH");
	CGI();
	printf("%s\n", str);
	return 0;
}
*/