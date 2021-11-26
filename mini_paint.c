
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>

typedef struct s_first
{
	int	width;
	int height;
	char	background;

}	t_first;

typedef struct s_row
{
	char	type;
	float center_x;
	float center_y;
	float	radius;
	char	c;
}	t_row;

char **assign_mem(t_first *first);
void print_map(char **map, t_first *first);
int	calc_distance(t_row *r, t_first *f, char ***map);
int	circ_border(t_row *r, int i, int j);
int	throw_error(char **map);
void	free_map(char **map);

void	check_leaks(void)
{
	system("leaks minipaint");
}

int	main(int argc, char **argv)
{
	t_first first;
	FILE *f;
	int rd;
	t_row row;
	char **map;
	
//	atexit(check_leaks);
	if (argc != 2)
		return (throw_error(0));
	f = fopen(argv[1], "r");
	if (!f)
		return (throw_error(0));
	rd = fscanf(f, "%d %d %c\n", &first.width, &first.height, &first.background);
	if (rd == 3 && first.width > 0 && first.width <= 300 && first.height > 0 && first.height <= 300)
	{
		map = assign_mem(&first);
		rd = fscanf(f, "%c %f %f %f %c\n", &row.type, &row.center_x, &row.center_y, &row.radius, &row.c);
	}
	else
		return (throw_error(0));
	while (rd == 5)
	{
		if (row.radius <= 0.0000000)
			return (throw_error(map));
		if (row.type == 'C' || row.type == 'c')
			calc_distance(&row, &first, &map);
		else
			return (throw_error(map));
		rd = fscanf(f, "%c %f %f %f %c\n", &row.type, &row.center_x, &row.center_y, &row.radius, &row.c);
	}
	if (rd == -1)
	{
		print_map(map, &first);
		free_map(map);
		return (0);
	}
	return (throw_error(map));
}

int	throw_error(char **map)
{
	if (map)
		free_map(map);
	write(1, "Error: Operation file corrupted\n", 32);
	return (1);
}

void	free_map(char **map)
{
	int	i;

	i = 0;
	while (map && map[i])
	{
		free(map[i]);
		i++;
	}
	free(map);
}
//La distancia entre dos puntos (Xa,Ya) y (Xb,Yb) es la raíz cuadrada de (Xa - Xb) * (Xa - Xb) + (Ya - Yb) * (Ya - Yb)
//Si la distancia entre la esquina superior izquierda de un pixel y el centro del círculo es menor o igual al radio del círculo, el pixel es parte del círculo.
//
//Un pixel cuya esquina superior izquierda está a una distancia inferior a uno del borde del círculo forma parte de un círculo vacío.
int	calc_distance(t_row *r, t_first *f, char ***map)
{
	int	i;
	int j;

	i = 0;
	j = 0;
	while (i < f->height && i <= 300)
	{
		j = 0;
		while (j < f->width && j <= 300)
		{
			if (sqrtf(powf(j - r->center_x, 2) + powf(i - r->center_y, 2)) <= r->radius)
			{
				if (r->type == 'c' && r->radius - sqrtf(powf(j - r->center_x, 2) + powf(i - r->center_y, 2)) < 1)
					(*map)[i][j] = r->c;
				else if (r->type == 'C')
					(*map)[i][j] = r->c;
			}
			j++;
		}
		i++;
	}
	return (1);
}

void print_map(char **map, t_first *first)
{
	int	i;
	int	j;

	i = 0;
	while (i < first->height)
	{
		j = 0;
		while (j < first->width)
		{
			write(1, &map[i][j], 1);
			j++;
		}
		write(1, "\n", 1);
		i++;
	}
}

char **assign_mem(t_first *first)
{
	int	i;
	char **map;
	int	j;

	j = 0;
	i = 0;
	map = malloc(sizeof(t_row *) * (first->height + 1));
	while (i < first->height)
	{
		map[i] = malloc(sizeof(t_row) * (first->width + 1));
		j = 0;
		while (j < first->width)
		{
			map[i][j] = first->background;
			j++;
		}
		map[i][j] = 0;
		i++;
	}
	map[i] = 0;
	return (map);
}
