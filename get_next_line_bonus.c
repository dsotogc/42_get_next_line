/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsoto-ga <dsoto-ga@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/08 18:22:22 by dsoto-ga          #+#    #+#             */
/*   Updated: 2026/08/09 11:03:24 by dsoto-ga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static char	*ft_freeline(char **line)
{
	free(*line);
	*line = NULL;
	return (NULL);
}

static void	shift(char *buf, size_t start)
{
	size_t	c;

	c = 0;
	while (buf[start + c])
	{
		buf[c] = buf[start + c];
		c++;
	}
	buf[c] = '\0';
}

static int	init_buf(char **b)
{
	if (*b)
		return (1);
	*b = malloc(BUFFER_SIZE + 1);
	if (!*b)
		return (0);
	*b[0] = '\0';
	return (1);
}

static char	*cut_line(char **line, char *tmp, char *buffer)
{
	if (!ft_realloc(line, buffer, tmp - buffer + 1))
		return (ft_freeline(line));
	shift(buffer, tmp - buffer + 1);
	return (*line);
}

char	*get_next_line(int fd)
{
	static char	*fd_table[FD_MAX];
	char		*line;
	char		*tmp;
	ssize_t		c_read;

	if (fd < 0 || fd >= FD_MAX || BUFFER_SIZE <= 0 || !init_buf(&fd_table[fd]))
		return (NULL);
	line = NULL;
	c_read = 0;
	while (1)
	{
		tmp = ft_strchr(fd_table[fd], '\n');
		if (tmp)
			return (cut_line(&line, tmp, fd_table[fd]));
		if (!ft_realloc(&line, fd_table[fd], ft_strlen(fd_table[fd])))
			return (ft_freeline(&line));
		fd_table[fd][0] = '\0';
		c_read = read(fd, fd_table[fd], BUFFER_SIZE);
		if (c_read <= 0)
			break ;
		fd_table[fd][c_read] = '\0';
	}
	if (c_read < 0 || !line || !*line)
		return (ft_freeline(&line));
	return (line);
}
