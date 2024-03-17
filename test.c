/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tabadawi <tabadawi@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 14:25:47 by tabadawi          #+#    #+#             */
/*   Updated: 2024/03/17 16:57:40 by tabadawi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./mlx/mlx.h"
#include "./libft/libft.h"
#include <fcntl.h>
#include <stdio.h>

static int	line_count(char	*path)
{
	char	*line;
	int		i;
	int		fd;
	int		check;
	int		emptyline;
	
	emptyline = -1;
	check = -1;
	i = 0;
	fd = open(path, O_RDONLY);
	line = get_next_line(fd);
	while (line != NULL)
	{
		i++;
		if (line[0] == '\n' && line[1] == '\0')
			emptyline = 1;
		if (line[0] != '\n' && line[1] != '\0' && emptyline == 1)
		{
			printf("middle line found\n");
			free (line);
			close (fd);
			exit (1);
		}
		else if (line[0] != '\n' && line[1] != '\0')
			check = i;
		free (line);
		line = get_next_line(fd);
	}
	free (line);
	close(fd);
	return (check);
}

static void	freeer(char **map)
{
	int	i;

	i = 0;
	while (map[i])
	{
		free (map[i]);
		i++;
	}
	free (map);
}
static int	check_size(char **map, int lines)
{
	int	i = 0;
	int length;
	int temp;

	length = ft_strlen(map[i], 1);
	if (lines < 3 || length < 3)
		return (-1);
	i++;
	while (map[i])
	{
		temp = ft_strlen(map[i], 1);
		if (temp != length)
			return (-1);
		i++;	
	}
	if (length * lines < 15)
		return (-1);
	return (0);
}

static int	get_map(char *path)
{
	char	**map;
	int		i;
	int		fd;
	int		lines;
	
	i = 0;
	lines = line_count(path);
	fd = open(path, O_RDONLY);
	map = malloc (sizeof(char *) * (lines + 1));
	while (i < lines)
	{
		map[i] = get_next_line(fd);
		i++;
	}
	map[i] = NULL;
	if (check_size(map, lines) == -1)
		return (freeer(map), 0);
	return (1);
}


int	main(int ac, char **av)
{
	if (get_map(av[1]) == 1)
		write (1, "parsed\n", 7);
	return (0);
}

	// void	*mlx;
	// void	*mlx_win;
	// void	*img;
	// char	*relative_path = "./sonic_left000.xpm";
	// int		img_width;
	// int		img_height;

	// mlx = mlx_init();
	// mlx_win = mlx_new_window(mlx, 1024, 720, "sonic");
	// img = mlx_xpm_file_to_image(mlx, relative_path, &img_width, &img_height);
	// mlx_put_image_to_window(mlx, mlx_win, img, 500, 300);
	// mlx_loop(mlx);