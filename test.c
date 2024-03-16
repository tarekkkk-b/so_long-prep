/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tarekkkk <tarekkkk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 14:25:47 by tabadawi          #+#    #+#             */
/*   Updated: 2024/03/17 02:04:28 by tarekkkk         ###   ########.fr       */
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
	int		lastnotempty;
	int		lastopp = -1;

	lastnotempty = -1;
	i = 0;
	fd = open(path, O_RDONLY);
	line = get_next_line(fd);
	while (line != NULL)
	{
		if (line[0] == '\0' || line[0] == '\n')
			lastnotempty = i;
		else
			lastopp = i;
		i++;
		free (line);
		line = get_next_line(fd);
	}
	if (lastnotempty != -1 && lastnotempty < lastopp)
	{
		free (line);
		printf("lines found in the middle\n");
		exit(1);
	}
	// else if (lastopp != -1 && lastopp < lastnotempty)
	// 	printf("lines found at the end\n\n");
	free (line);
	close(fd);
	// printf("%d\n", lastnotempty + 1);
	// printf("%d\n", lastopp + 1);
	return (lastopp + 1);
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

static int	parse_map(char *path)
{
	char	**map;
	int		i;
	int		fd;
	int		lines;
	
	i = 0;
	lines = line_count(path);
	printf("%d\n", lines);
	fd = open(path, O_RDONLY);
	// printf("%d\n\n\n", fd);
	map = malloc (sizeof(char *) * (lines + 1));
	while (i < lines)
	{
		map[i] = get_next_line(fd);
		i++;
	}
	map[i] = NULL;
	freeer(map);
	return (0);
}

int	main(int ac, char **av)
{
	if (parse_map(av[1]) == 0)
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