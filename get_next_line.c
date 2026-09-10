/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsoto-ga <dsoto-ga@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/14 14:59:26 by dsoto-ga          #+#    #+#             */
/*   Updated: 2026/09/10 18:13:36 by dsoto-ga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*ft_free(char **ptr)
{
	free(*ptr);
	*ptr = NULL;
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

static char	*read_line(int fd, char *buffer, char *line)
{
	char	*tmp;
	ssize_t	c_read;

	c_read = 1;
	while (c_read > 0)
	{
		tmp = ft_strchr(buffer, '\n');
		if (tmp)
		{
			if (!ft_realloc(&line, buffer, tmp - buffer + 1))
				return (ft_free(&line));
			shift(buffer, tmp - buffer + 1);
			return (line);
		}
		if (!ft_realloc(&line, buffer, ft_strlen(buffer)))
			return (ft_free(&line));
		buffer[0] = '\0';
		c_read = read(fd, buffer, BUFFER_SIZE);
		if (c_read > 0)
			buffer[c_read] = '\0';
	}
	if (c_read < 0 || !line || !*line)
		return (ft_free(&line));
	return (line);
}

char	*get_next_line(int fd)
{
	static char	*buffer;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!buffer)
	{
		buffer = malloc(BUFFER_SIZE + 1);
		if (!buffer)
			return (NULL);
		buffer[0] = '\0';
	}
	line = read_line(fd, buffer, NULL);
	if (!buffer[0])
	{
		free(buffer);
		buffer = NULL;
	}
	return (line);
}
