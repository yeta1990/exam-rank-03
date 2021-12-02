#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct s_zone
{
	int		width;
	int		height;
	char	background;

}	t_zone;

typedef struct s_square
{
	char	type;
	float	x;
	float	y;
	float	width;
	float	height;
	char	draw;
}	t_square;

char **create_map(t_zone z);
void	print_map(char **map, t_zone z);
void	fill_square(char ***map, t_zone *z, t_square *sq);
int throw_error(void);
void	free_map(char **map, t_zone *z);

int main(int argc, char **argv)
{
	FILE 	*f;
	int		n;
	t_zone	z;
	t_square sq;
	char	**map;

	n = 0;
	map = 0;
	if (argc != 2)
	{
		write(1, "Error: argument\n", 16);
		return (1);
	}
	f = fopen(argv[1], "r");
	if (!f)
		return (throw_error());
	n = fscanf(f, "%d %d %c\n", &z.width, &z.height, &z.background);
	if (n == 3 && z.width > 0 && z.width <= 300 && z.height > 0 && z.height <= 300)
	{
		map = create_map(z);
		n = fscanf(f, "%c %f %f %f %f %c\n", &sq.type, &sq.x, &sq.y, &sq.width, &sq.height, &sq.draw);
	}
	else
		return(throw_error());
	while (n == 6)
	{
		fill_square(&map, &z, &sq);
		if (sq.type != 'r' && sq.type != 'R')
			return (throw_error());
		if (sq.width <= 0.00000000 || sq.height <= 0.00000000)
			return (throw_error());
		n = fscanf(f, "%c %f %f %f %f %c\n", &sq.type, &sq.x, &sq.y, &sq.width, &sq.height, &sq.draw);
	}
	if (n == -1)
	{
		print_map(map, z);
		fclose(f);
		free_map(map, &z);
	//	system("leaks micro_paint");
		return (0);	
	}
	return(throw_error());
}

char **create_map(t_zone z)
{
	char	**map;
	int		i;
	int		j;

	j = 0;
	i = 0;
	map = malloc(sizeof(char *) * (z.height + 1));
	map[z.height] = 0;
	while (i < z.height)
	{
		map[i] = malloc(sizeof(char) * (z.width + 1));
		j = 0;
		while (j < z.width)
		{
			map[i][j] = z.background;
			j++;
		}
		map[i][j] = 0;
		i++;
	}
	map[i] = 0;
	return (map);
}

void	print_map(char **map, t_zone z)
{
	int	i;
	int	j;
	
	i = 0;
	while (map[i] && i < z.height)
	{
		j = 0;
		while (map[i][j] && j < z.width)
		{
			write(1, &map[i][j], 1);	
			j++;
		}
		write(1, "\n", 1);
		i++;
	}	
}

void	fill_square(char ***map, t_zone *z, t_square *sq)
{
	int	x;
	int	y;
	
	x = 0;
	y = 0;
	while ((*map)[y] && y < z->height)
	{
		x = 0;
		while ((*map)[y][x] && x < z->width)
		{
					if (sq->x <= x && x <= sq->x + sq->width && sq->y <= y && y <= sq->y + sq->height)
			{
				if (sq->type == 'r' && ( x - sq->x < 1.00000000 ||
						   	y - sq->y < 1.00000000 || sq->x + sq->width - x < 1.00000000 ||
						   	sq->y + sq->height - y < 1.00000000 ))
					(*map)[y][x] = sq->draw;
				else if (sq->type == 'R')
					(*map)[y][x] = sq->draw;
			}
			x++;
		}
		y++;
	}
}

int throw_error(void)
{
	write(1, "Error: Operation file corrupted\n", 32);
	return (1);
}

void	free_map(char **map, t_zone *z)
{
	int	y;
	
	y = 0;
	while (y <= z->height)
	{
		free(map[y]);
		y++;
	}
	free(map);

}
