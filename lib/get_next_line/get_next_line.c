/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meyu <meyu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 19:52:46 by meyu              #+#    #+#             */
/*   Updated: 2025/08/08 15:49:01 by meyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static void	delete_one_line(char *temp)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (temp[i] && temp[i] != '\n')
		i++;
	if (temp[i] == '\n')
		i++;
	while (temp[i])
		temp[j++] = temp[i++];
	temp[j] = '\0';
}

static char	*read_to_buffer(int fd, char *temp)
{
	ssize_t	bytes_read;
	char	*line;
	char	*new_line;

	line = ft_strdup(temp);
	if (!line)
		return (NULL);
	if (ft_strchr(line, '\n'))
		return (line);
	bytes_read = read(fd, temp, BUFFER_SIZE);
	while (bytes_read > 0)
	{
		temp[bytes_read] = '\0';
		new_line = ft_strjoin(line, temp);
		if (!new_line)
			return (free(line), NULL);
		free(line);
		line = new_line;
		if (ft_strchr(line, '\n'))
			return (line);
		bytes_read = read(fd, temp, BUFFER_SIZE);
	}
	if (bytes_read == -1 || (bytes_read == 0 && line[0] == '\0'))
		return (free(line), NULL);
	return (line);
}

static char	*get_one_line(char *line)
{
	size_t	i;
	size_t	j;
	char	*one_line;

	if (!line)
		return (NULL);
	i = 0;
	while (line[i] && line[i] != '\n')
		i++;
	if (line [i] == '\n')
		i++;
	one_line = (char *)malloc((i + 1));
	if (!one_line)
		return (NULL);
	j = 0;
	while (j < i)
	{
		one_line[j] = line[j];
		j++;
	}
	one_line[j] = '\0';
	return (one_line);
}

char	*get_next_line(int fd)
{
	static char	temp[BUFFER_SIZE + 1];
	char		*line;
	char		*rest;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	line = read_to_buffer(fd, temp);
	if (line == NULL)
		return (temp[0] = '\0', NULL);
	rest = get_one_line(line);
	free(line);
	if (rest == NULL)
		return (temp[0] = '\0', NULL);
	delete_one_line(temp);
	if (rest[0] == '\0')
	{
		free (rest);
		temp[0] = '\0';
		return (NULL);
	}
	return (rest);
}

/* #include <fcntl.h>
#include <stdio.h>
void leaks(void)
{
	system("leaks a.out");
}

int main()
{
	atexit(leaks);
	int fd = open("text.txt", O_RDONLY);
	char *line = get_next_line(fd);
	printf("%s", line);
	free(line);
	return (0);
}
 */