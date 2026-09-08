/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcolin <vcolin@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 11:47:40 by vcolin            #+#    #+#             */
/*   Updated: 2026/09/07 14:42:20 by vcolin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static int	read_and_store(int fd, char	**rest)
{
	char	buffer[BUFFER_SIZE + 1];
	int		bytes_read;

	while (1)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
		{
			free(*rest);
			*rest = NULL;
			return (0);
		}
		if (bytes_read == 0)
			return (1);
		buffer[bytes_read] = '\0';
		*rest = join_rest(*rest, buffer);
		if (*rest == NULL)
			return (0);
		if (find_new_line(*rest) != NULL)
			return (1);
	}
}

char	*get_next_line(int fd)
{
	static char	*rest;
	char		*prov;
	char		*line;

	if (!read_and_store(fd, &rest) || !rest || !rest[0])
	{
		free(rest);
		rest = NULL;
		return (NULL);
	}
	prov = rest;
	line = extract_line(prov);
	rest = extract_rest(prov);
	free(prov);
	return (line);
}
