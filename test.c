/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tabadawi <tabadawi@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 14:25:47 by tabadawi          #+#    #+#             */
/*   Updated: 2024/03/18 16:59:03 by tabadawi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./mlx/mlx.h"
#include "./libft/libft.h"
#include <fcntl.h>
#include <stdio.h>

# define WALL				'1'
# define FLOOR				'0'
# define COIN				'C'
# define PLAYER				'P'
# define EXIT				'E'

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
	if (fd == -1)
		return (-1);
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

static int	check_name(char *path)
{
	int		i = 0;
	int		len;
	char	*temp = malloc(5);

	len = ft_strlen(path, 0) - 4;
	while (path[len])
		temp[i++] = path[len++];
	temp[i++] = '\0';
	len = ft_strlen(path, 0) - 4;
	if (ft_strncmp(temp, ".ber", 4) != 0)
	{
		free (temp);
		return (-1);
	}
	free (temp);
	return (1);
}

static char	**get_map(char *path, int *lines)
{
	char	**map;
	int		i;
	int		fd;
	
	i = 0;
	if (check_name(path) == -1)
	{
		printf("wrong name\n");
		return (NULL);
	}
	(*lines) = line_count(path);
	if ((*lines) == -1)
		return (NULL);
	fd = open(path, O_RDONLY);
	map = malloc (sizeof(char *) * ((*lines) + 1));
	while (i < (*lines))
	{
		map[i] = get_next_line(fd);
		i++;
	}
	map[i] = NULL;
	close (fd);
	return (map);
}

static int	checkoccurance(char *row, char c)
{
	int	i;
	
	i = 0;
	int	len;

	len = ft_strlen(row, 1);
	while (i < len)
	{
		if (row[i] != c)
			return (-1);
		i++;	
	}
	return (1);
}

static int	check_borders(char **map, int lines)
{
	int	i;
	int	len;

	len = ft_strlen(map[0], 1);
	i = 1;
	if (checkoccurance(map[0], '1') == -1
		|| checkoccurance(map[lines - 1], '1') == -1)
			return (-1);
	while (i < lines)
	{
		if (map[i][0] != '1' || map[i][len - 1] != '1')
			return (-1);
		i++;
	}
	return (1);
}

static int	validate_char(char c)
{
	// printf("")
	if (c != WALL && c != FLOOR && c != COIN && c != PLAYER && c != EXIT)
	{
		printf("invalid token\n");	
		return (-1);
	}
	return (0);
}

static int	check_elements(char **map, int lines)
{
	int	p = 0;
	int	c = 0;
	int	e = 0;
	int	i = 0;
	int	j = 0;
	int	len = ft_strlen(map[i], 1);
	
	while (i < lines)
	{
		while (j < len - 1)
		{
			if (validate_char(map[i][j]) == -1)
				return (-1);
			if (map[i][j] == COIN)
				c++;
			if (map[i][j] == PLAYER)
				p++;
			if (map[i][j] == EXIT)
				e++;
			j++;		
		}
		j = 0;
		i++;
	}
	printf("coins %d, player %d, exits %d\n", c, p, e);
	if (p != 1 || e != 1 || c < 1)
		return (-1);
	return (0);
}

static int	parse_map(char	*path)
{
	char	**map;
	int		lines;
	
	map = get_map(path, &lines);
	if (!map)
		return (0);
	if (check_size(map, lines) == -1)
		return (freeer(map), 0);
	if (check_borders(map, lines) == -1)
		return (freeer(map), 0);
	if (check_elements(map, lines) == -1)
	{
		printf("missing tokens\n");
		return (freeer(map), 0);
	}
	else
		return (freeer(map), 1);
}

int	main(int ac, char **av)
{
	if (parse_map(av[1]) == 1)
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