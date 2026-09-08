/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcolin <vcolin@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 11:47:47 by vcolin            #+#    #+#             */
/*   Updated: 2026/09/08 11:06:17 by vcolin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*find_new_line(char	*str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '\n')
		i++;
	if (str[i] == '\n')
		return (&str[i]);
	return (NULL);
}

char	*extract_line(char *prov)
{
	size_t	i;
	char	*line;

	i = 0;
	while (prov[i] && prov[i] != '\n')
		i++;
	line = malloc(sizeof(char) * (i + 2));
	if (!line)
		return (NULL);
	i = 0;
	while (prov[i] && prov[i] != '\n')
	{
		line[i] = prov[i];
		i++;
	}
	if (prov[i] == '\n')
		line[i++] = '\n';
	line[i] = '\0';
	return (line);
}

char	*extract_rest(char	*prov)
{
	size_t	i;
	size_t	j;
	char	*line;

	i = 0;
	while (prov[i] && prov[i] != '\n')
		i++;
	if (!prov[i])
		return (NULL);
	j = 0;
	while (prov[i + 1 + j])
		j++;
	line = malloc(sizeof(char) * (j + 1));
	if (!line)
		return (NULL);
	j = 0;
	while (prov[i + 1 + j])
	{
		line[j] = prov[i + 1 + j];
		j++;
	}
	line[j] = '\0';
	return (line);
}

static void	copy_str(char *dest, char *src, size_t *i)
{
	size_t	j;

	if (!src)
		return ;
	j = 0;
	while (src[j])
	{
		dest[*i] = src[j];
		(*i)++;
		j++;
	}
}

char	*join_rest(char *rest, char *buffer)
{
	size_t	len_rest;
	size_t	len_buffer;
	size_t	i;
	char	*joined_str;

	len_rest = 0;
	len_buffer = 0;
	while (rest && rest[len_rest])
		len_rest++;
	while (buffer[len_buffer])
		len_buffer++;
	joined_str = malloc(sizeof(char) * (len_rest + len_buffer + 1));
	if (!joined_str)
		return (NULL);
	i = 0;
	copy_str(joined_str, rest, &i);
	copy_str(joined_str, buffer, &i);
	joined_str[i] = '\0';
	free(rest);
	return (joined_str);
}
