/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tabadawi <tabadawi@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 14:25:47 by tabadawi          #+#    #+#             */
/*   Updated: 2024/03/26 16:05:36 by tabadawi         ###   ########.fr       */
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
# define TILE				120

typedef	struct s_position
{
	int	x;
	int	y;
}	t_position;

typedef struct s_elements
{
	int	p;
	int	c;
	int	e;
}	t_elements;


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

	length = ft_strlen(map[i]);
	if (lines < 3 || length < 3)
		return (-1);
	i++;
	while (map[i])
	{
		temp = ft_strlen(map[i]);
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

	len = ft_strlen(path) - 4;
	while (path[len])
		temp[i++] = path[len++];
	temp[i++] = '\0';
	len = ft_strlen(path) - 4;
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
	i = 0;
	while (map[i])
	{
		map[i] = ft_strtrim(map[i], "\n");
		i++;
	}
	close (fd);
	return (map);
}

static int	checkoccurance(char *row, char c)
{
	int	i;
	
	i = 0;
	int	len;

	len = ft_strlen(row);
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

	len = ft_strlen(map[0]);
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
	if (c != WALL && c != FLOOR && c != COIN && c != PLAYER && c != EXIT)
	{
		printf("invalid token\n");	
		return (-1);
	}
	return (0);
}

static int	check_elements(char **map, int lines, t_position *pos, t_elements *elem)
{
	int	i = 0;
	int	j = 0;
	int	len = ft_strlen(map[i]);
	
	while (i < lines)
	{
		while (j < len - 1)
		{
			if (validate_char(map[i][j]) == -1)
				return (-1);
			if (map[i][j] == COIN)
				elem->c++;
			if (map[i][j] == PLAYER)
			{
				elem->p++;
				pos->x = j;
				pos->y = i;
			}
			if (map[i][j] == EXIT)
				elem->e++;
			j++;		
		}
		j = 0;
		i++;
	}
	// printf("coins %d, player %d, exits %d\n", elem->c, elem->p, elem->e);
	if (elem->p != 1 || elem->e != 1 || elem->c < 1)
		return (-1);
	return (0);
}

static char	**make_copy(char **map, int lines)
{
	char	**new;
	int		i;
	
	i = 0;
	new = malloc(sizeof(char *) * (lines + 1));
	while (map[i])
	{
		new[i] = ft_strdup(map[i]);
		// printf("%s\n", new[i]);
		i++;
	}
	new[i] = NULL;
	return (new);
}

static int	check_validpath(char **map, int lines, int x, int y, t_elements *elements)
{
	if ((x < 0 || x > lines) || (y < 0 || y > (int)ft_strlen(map[0])) || (map[x][y] == '1' || map[x][y] == 'V'))
		return (-1);
	if (map[x][y] == COIN)
		elements->c--;
	if (map[x][y] == EXIT)
		elements->e--;
	map[x][y] = 'V';
	check_validpath(map, lines, x + 1, y, elements);
	check_validpath(map, lines, x - 1, y, elements);
	check_validpath(map, lines, x, y + 1, elements);
	check_validpath(map, lines, x, y - 1, elements);
	if (elements->c != 0 || elements->e != 0)
		return (-1);
	return (1);
}

static char	**parse_map(char *path)
{
	char		**map;
	char		**temp;
	int			lines;
	t_position	position;
	t_elements	elements;

	position.x = 0;
	position.y = 0;
	elements.c = 0;
	elements.e = 0;
	elements.p = 0;
	map = get_map(path, &lines);

	if (!map)
		return (0);
	temp = make_copy(map, lines);
	if (check_size(map, lines) == -1)
		return (freeer(map), freeer(temp), NULL);
	if (check_borders(map, lines) == -1)
		return (freeer(map), freeer(temp), NULL);
	if (check_elements(map, lines, &position, &elements) == -1)
		return (freeer(map), freeer(temp), NULL);
	if (check_validpath(temp, lines, position.y , position.x, &elements) == -1)
	{
		printf("hey there loser\n");
		return (freeer(map), freeer(temp), NULL);
	}
	// printf("%d	%d\n", elements.c, elements.e);
	freeer(temp);
	return (map);	
}

static void putmap(char **map)
{
	void	*mlx;
	void	*mlx_win;
	void	*img0;
	void	*img1;
	void	*img2;
	void	*img3;
	void	*img4;
	void	*img5;
	void	*img6;
	char	*relative_path0 = "./xpm/sonic_left.xpm";
	char	*relative_path1 = "./xpm/floortile4.xpm";
	char	*relative_path2 = "./xpm/ringresized.xpm";
	char	*relative_path3 = "./xpm/wall4.xpm";
	char	*relative_path4 = "./xpm/waal.xpm";
	char	*relative_path5 = "./xpm/walltilesize.xpm";
	char	*relative_path6 = "./xpm/wall3rotated.xpm";
	int		img_width0;
	int		img_width1;
	int		img_width2;
	int		img_width3;
	int		img_width4;
	int		img_width5;
	int		img_width6;
	int		img_height0;
	int		img_height1;
	int		img_height2;
	int		img_height3;
	int		img_height4;
	int		img_height5;
	int		img_height6;

	mlx = mlx_init();
	mlx_win = mlx_new_window(mlx, 6 * TILE, 8 * TILE, "sonic");
	img0 = mlx_xpm_file_to_image(mlx, relative_path0, &img_width0, &img_height0);
	img1 = mlx_xpm_file_to_image(mlx, relative_path1, &img_width1, &img_height1);
	img2 = mlx_xpm_file_to_image(mlx, relative_path2, &img_width2, &img_height2);
	img3 = mlx_xpm_file_to_image(mlx, relative_path3, &img_width3, &img_height3);
	img4 = mlx_xpm_file_to_image(mlx, relative_path4, &img_width4, &img_height4);
	img5 = mlx_xpm_file_to_image(mlx, relative_path5, &img_width5, &img_height5);
	img6 = mlx_xpm_file_to_image(mlx, relative_path6, &img_width6, &img_height6);
	int j = 0;
	while (map[0][j])
	{
		mlx_put_image_to_window(mlx, mlx_win, img3, j * TILE, 0 * TILE);
		mlx_put_image_to_window(mlx, mlx_win, img3, j * TILE, 7 * TILE);
		j++;
	}
	int i = 1;
	j = 0;
	while (i < 7)
	{
		while (map[i][j])
		{
			mlx_put_image_to_window(mlx, mlx_win, img3, 0 * TILE, i * TILE);
			mlx_put_image_to_window(mlx, mlx_win, img3, 5 * TILE, i * TILE);
			j++;
		}
		j = 0;
		i++;
	}
	i = 1;
	j = 1;
	while (i < 7)
	{
		while (j < 5)
		{
			if (map[i][j] == WALL)
				mlx_put_image_to_window(mlx, mlx_win, img4, j * TILE, i * TILE);
			if (map[i][j] == FLOOR || map[i][j] == COIN || map[i][j] == PLAYER || map[i][j] == EXIT)
				mlx_put_image_to_window(mlx, mlx_win, img1, j * TILE, i * TILE);
			j++;
		}
		j = 1;
		i++;
	}
	i = 1;
	j = 1;
	while (i < 7)
	{
		while (j < 5)
		{
			if (map[i][j] == PLAYER)
				mlx_put_image_to_window(mlx, mlx_win, img0, j * TILE, i * TILE);
			if (map[i][j] == COIN || map[i][j] == EXIT)
				mlx_put_image_to_window(mlx, mlx_win, img2, j * TILE, i * TILE);
			j++;
		}
		j = 1;
		i++;
	}
	mlx_loop(mlx);
}

int	main(int ac, char **av)
{
	(void)ac;
	char **map = parse_map(av[1]);
	putmap(map);
	return (0);
}
