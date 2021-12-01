
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int throw_error(void)
{
	write(1, "Error: Operation file corrupted\n", 32);
	return (1);
}

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
	printf("%i %i %c\n", z.width, z.height, z.background);
//	printf("%i\n", n);
	if (n == 3 && z.width > 0 && z.width <= 300 && z.height > 0 && z.height <= 300)
	{
		map = create_map(z);
		n = fscanf(f, "%c %f %f %f %f %c\n", &sq.type, &sq.x, &sq.y, &sq.width, &sq.height, &sq.draw);
	}
	else
		return(throw_error());
	while (n == 6)
	{
//		printf("%i\n", n);
		fill_square(&map, &z, &sq);
		if (sq.type != 'r' && sq.type != 'R')
			return (throw_error());
		if (sq.width == 0.000000 || sq.height == 0.000000)
			return (throw_error());
		printf("%c %f %f %f %f %c\n", sq.type, sq.x, sq.y, sq.width, sq.height, sq.draw);
		n = fscanf(f, "%c %f %f %f %f %c\n", &sq.type, &sq.x, &sq.y, &sq.width, &sq.height, &sq.draw);
	}
	fclose(f);
	if (n == -1)
	{
		print_map(map, z);
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
int	i;
int	j;

i = 0;
j = 0;
while ((*map)[i] && i < z->height)
{
	j = 0;
	while ((*map)[i][j] && j < z->width)
	{
		//If a point is defined as (Xa, Ya)
		//And a rectangle with a top left corner (Xtl, Ytl) and a bottom right corner (Xbr, Ybr)
		//If Xtl <= Xa <= Xbr and Ytl <= Ya <= Ybr then the point is in the rectangle%
//		if (sq.type == 'r')
//		{
			if ((sq->x <= j && j <= sq->x + sq->height) && (sq->y <= i && i <= sq->y + sq->width))
			{
		//		if (sq->type == 'r' && (i - sq->x) < 1 && (j - sq->y) < 1)
		//				(*map)[i][j] = sq->draw;
	//				else
						(*map)[i][j] = sq->draw;
				}

	//		}
	//		else if (sq.type == 'R')
	//		{
	//			if ((sq.x <= j && j <= sq.x + sq.height) && (sq.y <= i && i <= sq.y + sq.width))
	//				(*map)[i][j] = sq.draw;
	//		}
			j++;
		}
		i++;
	}

}	
