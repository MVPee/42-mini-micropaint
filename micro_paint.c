#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

typedef struct s_data
{
    int width;
    int height;
    char *matrice;
    char background;
}   t_data;

typedef struct s_rectangle
{
    char mode;
    float x;
    float y;
    float width;
    float height;
    char color;
}   t_rectangle;

int init_data(t_data *data, FILE *file)
{
    if (fscanf(file, "%d %d %c\n", &data->width, &data->height, &data->background) != 3)
        return (1);
    if (data->width <= 0 || data->width > 300 || data->height <= 0 || data->height > 300)
        return (1);
    if (!(data->matrice = malloc(sizeof(char) * (data->width * data->height))))
        return (1);
    memset(data->matrice, data->background, data->width * data->height);
    return (0);
}

int is_in_rectangle(float x, float y, t_rectangle rect)
{
	if (x < rect.x || rect.x + rect.width < x || y < rect.y || rect.y + rect.height < y)
		return (0);
    if (rect.mode == 'R')
        return (1);
    if (x - rect.x < 1.00000000 || rect.x + rect.width - x < 1.00000000 || y - rect.y < 1.00000000 || rect.y + rect.height - y < 1.00000000)
		return (1);
    return (0);
}

int process(t_data *data, FILE *file)
{
    t_rectangle rect;

    while(fscanf(file, "%c %f %f %f %f %c\n", &rect.mode, &rect.x, &rect.y, &rect.width, &rect.height, &rect.color) == 6)
    {
        if ((rect.mode != 'r' && rect.mode != 'R') || rect.width <= 0.00000000 || rect.height <= 0.00000000)
            return (1);
        for (int x = 0; x < data->width; x++)
            for (int y = 0; y < data->height; y++)
                if (is_in_rectangle((float)x, (float)y, rect))
                    data->matrice[y * data->width + x] = rect.color;
    }
    return (0);
}

int main(int ac, char **av)
{
    FILE *file;
    t_data data;

    if (ac != 2)
        return (write(1, "Error: argument\n", 16), 1);
    if (!(file = fopen(av[1], "r")) || init_data(&data, file))
        return (write(1, "Error: Operation file corrupted\n", 32), 1);
    if (process(&data, file))
        return (free(data.matrice), fclose(file), write(1, "Error: Operation file corrupted\n", 32), 1);
    for (int y = 0; y < data.height; y++)
    {
        write(1, y * data.width + data.matrice, data.width);
        write(1, "\n", 1);
    }
    return (free(data.matrice), fclose(file), 0);
}
