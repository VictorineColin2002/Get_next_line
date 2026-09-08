/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcolin <vcolin@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 11:47:40 by vcolin            #+#    #+#             */
/*   Updated: 2026/09/08 11:15:23 by vcolin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static t_gnl_list	*get_stock(t_gnl_list **stock, int fd)
{
	t_gnl_list	*current;

	current = *stock;
	while (current)
	{
		if (current->fd == fd)
			return (current);
		current = current->next;
	}
	current = malloc(sizeof(t_gnl_list));
	if (!current)
		return (NULL);
	current->fd = fd;
	current->rest = NULL;
	current->next = *stock;
	*stock = current;
	return (current);
}

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

static void	delete_node(t_gnl_list **stock, int fd)
{
	t_gnl_list	*current;
	t_gnl_list	*prev;

	current = *stock;
	prev = NULL;
	while (current)
	{
		if (current->fd == fd)
		{
			if (prev == NULL)
				*stock = current->next;
			else
				prev->next = current->next;
			free(current->rest);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

char	*get_next_line(int fd)
{
	static t_gnl_list	*stock;
	t_gnl_list			*current;
	char				*prov;
	char				*line;

	current = get_stock(&stock, fd);
	if (!current)
		return (NULL);
	if (!read_and_store(fd, &current->rest)
		|| !current->rest || !current->rest[0])
	{
		delete_node(&stock, fd);
		return (NULL);
	}
	prov = current->rest;
	line = extract_line(prov);
	current->rest = extract_rest(prov);
	free(prov);
	return (line);
}
