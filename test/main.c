/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcolin <vcolin@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 14:02:23 by vcolin            #+#    #+#             */
/*   Updated: 2026/09/07 14:39:38 by vcolin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "get_next_line.h"

int	main(void)
{
	int		fd;
	char	*line;

	fd = open("test2.txt", O_RDONLY);
	if (fd < 0)
	{
		printf("Erreur d'ouverture du fichier\n");
		return (1);
	}
	while ((line = get_next_line(fd)))
	{
		printf("[%s]", line);
		free(line);
	}
	close(fd);
	return (0);
}
