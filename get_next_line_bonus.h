/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcolin <vcolin@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 11:47:29 by vcolin            #+#    #+#             */
/*   Updated: 2026/09/08 11:18:42 by vcolin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# include <unistd.h>
# include <stdlib.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 100
# endif

typedef struct s_gnl_list
{
	int					fd;
	char				*rest;
	struct s_gnl_list	*next;
}						t_gnl_list;

char	*find_new_line(char *str);
char	*extract_line(char *prov);
char	*extract_rest(char	*prov);
char	*join_rest(char	*rest, char	*buffer);
char	*get_next_line(int fd);

#endif
