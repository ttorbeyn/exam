#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>

typedef struct	s_data
{
	int		width;
	int		height;
	char	background;
	char	*matrice;
}				t_data;

typedef struct	s_circle
{
	char type;
	float x;
	float y;
	float radius;
	char draw;
}				t_circle;

void ft_putchar(char c)
{
	write(1, &c, 1);
}

int	ft_strlen(char *str)
{
	int	i = 0;

	while (str[i])
		i++;
	return (i);
}

void ft_putstr(char *str)
{
	int i;

	i = 0;
	while (i < ft_strlen(str))
	{
		ft_putchar(str[i]);
		i++;
	}
}

int	get_info(FILE *file, t_data *data)
{
	int	ret;
	char	*tmp;
	int i;

	ret = fscanf(file, "%d %d %c\n", &data->width, &data->height, &data->background);
	if (ret == 3)
	{
		if (data->width < 1 || data->height < 1 || data->width > 300 || data->height > 300)
			return (1);
		tmp = malloc(sizeof(char *) * data->width * data->height);
		data->matrice = tmp;
		if (!data->matrice)
			return (1);
		i = 0;
		while (i < data->width * data->height)
			data->matrice[i++] = data->background;
		return (0);
	}
	return (1);
}

float	sq(float a)
{
	return(a * a);
}

float	sq_dist(float x1, float y1, float x2, float y2)
{
	float dist_x;
	float dist_y;

	dist_x = sq(x1 - x2);
	dist_y = sq(y1 - y2);
	return(dist_x + dist_y);
}

int	is_in_circle(float x, float y, t_circle *circle)
{
	float distance;
	float distance_sqrt;

	distance_sqrt = sqrtf(sq_dist(x, y, circle->x, circle->y));
	distance = distance_sqrt - circle->radius;
	if (distance <= 0.00000000)
	{
		if (distance <= -1.00000000)
			return (1); //inside
		return (2); //border
	}
	return (0);
}

void draw_one(t_data *data, t_circle *circle, int i, int j)
{
	int	is_in;
	is_in = is_in_circle((float)i, (float)j, circle);
	if (is_in == 2 || (is_in == 1 && circle->type == 'C'))
		data->matrice[i + j * data->width] = circle->draw;

}

int	draw_circle(t_circle *circle, t_data *data)
{
	int i;
	int j;

	if (circle->radius <= 0.00000000 || (circle->type != 'c' && circle->type != 'C'))
		return (1);
	i = 0;
	while (i < data->width)
	{
		j = 0;
		while (j < data->height)
		{
			draw_one(data, circle, i, j);
			j++;
		}
		i++;
	}
	return (0);
}

void	print_draw(t_data *data)
{
	int i;
	int j;

	i = 0;
	while (i < data->width * data->height)
	{
		j = 0;
		while (j < data->width)
		{
			ft_putchar(data->matrice[i++]);
			j++;
		}
		ft_putchar('\n');
	}
}

int	execute(FILE *file, t_circle *circle, t_data *data)
{
	int	ret;

	ret = fscanf(file, "%c %f %f %f %c\n", &circle->type, &circle->x, &circle->y, &circle->radius, &circle->draw);
	while (ret == 5)
	{
		if (draw_circle(circle, data))
			return (1);
		ret = fscanf(file, "%c %f %f %f %c\n", &circle->type, &circle->x, &circle->y, &circle->radius, &circle->draw);
	}
	if (ret == -1)
	{
		print_draw(data);
		return (0);
	}
	return (1);
}

int main(int ac, char **av)
{
	FILE	*file;
	t_data	data;
	t_circle circle;

	if (ac != 2)
		return (1);
	if (!(file = fopen(av[1], "r")))
	{
		ft_putstr("Error: Operation file corrupted\n");
		return (1);
	}
	if (get_info(file, &data))
		return (1);
	if (execute(file, &circle, &data))
		return (1);
}
