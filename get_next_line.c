/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsoto-ga <dsoto-ga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/14 14:59:26 by dsoto-ga          #+#    #+#             */
/*   Updated: 2026/07/24 18:22:36 by dsoto-ga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

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
	static char	*buffer;
	char		*line;
	char		*tmp;
	ssize_t		c_read;

	if (fd < 0 || BUFFER_SIZE <= 0 || !init_buf(&buffer))
		return (NULL);
	line = NULL;
	c_read = 0;
	while (1)
	{
		tmp = ft_strchr(buffer, '\n');
		if (tmp)
			return (cut_line(&line, tmp, buffer));
		if (!ft_realloc(&line, buffer, ft_strlen(buffer)))
			return (ft_freeline(&line));
		buffer[0] = '\0';
		c_read = read(fd, buffer, BUFFER_SIZE);
		if (c_read <= 0)
			break ;
		buffer[c_read] = '\0';
	}
	if (c_read < 0 || !line || !*line)
		return (ft_freeline(&line));
	return (line);
}
