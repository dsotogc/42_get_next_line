/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsoto-ga <dsoto-ga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/14 15:51:55 by dsoto-ga          #+#    #+#             */
/*   Updated: 2026/07/24 18:24:24 by dsoto-ga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_strchr(const char *s, int c)
{
	while (*s != (unsigned char) c)
	{
		if (*s == '\0')
			return (NULL);
		s++;
	}
	return ((char *) s);
}

size_t	ft_strlen(const char *s)
{
	size_t	c;

	if (!s)
		return (0);
	c = 0;
	while (s[c])
		c++;
	return (c);
}

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char		*d;
	const unsigned char	*src_c;
	size_t				i;

	i = 0;
	d = (unsigned char *) dest;
	src_c = (const unsigned char *) src;
	while (i < n)
	{
		d[i] = src_c[i];
		i++;
	}
	return (dest);
}

int	ft_realloc(char **line, char *buff, size_t size2)
{
	char	*tmp;
	size_t	linelen;

	linelen = ft_strlen(*line);
	tmp = malloc(linelen + size2 + 1);
	if (!tmp)
		return (0);
	ft_memcpy(tmp, *line, linelen);
	ft_memcpy(tmp + linelen, buff, size2);
	tmp[linelen + size2] = '\0';
	free(*line);
	*line = tmp;
	return (1);
}
