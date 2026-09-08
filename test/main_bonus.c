#include <stdio.h>
#include <fcntl.h>
#include "get_next_line_bonus.h"

int	main(void)
{
	int		fd1;
	int		fd2;
	char	*line1;
	char	*line2;
	int		fd1_done;
	int		fd2_done;

	fd1 = open("test1.txt", O_RDONLY);
	fd2 = open("test2.txt", O_RDONLY);
	if (fd1 < 0 || fd2 < 0)
	{
		printf("Erreur d'ouverture d'un des fichiers\n");
		return (1);
	}
	fd1_done = 0;
	fd2_done = 0;
	while (!fd1_done || !fd2_done)
	{
		if (!fd1_done)
		{
			line1 = get_next_line(fd1);
			if (!line1)
				fd1_done = 1;
			else
			{
				printf("[fd1] %s", line1);
				free(line1);
			}
		}
		if (!fd2_done)
		{
			line2 = get_next_line(fd2);
			if (!line2)
				fd2_done = 1;
			else
			{
				printf("[fd2] %s", line2);
				free(line2);
			}
		}
	}
	close(fd1);
	close(fd2);
	return (0);
}
