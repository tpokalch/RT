
#include "rtv1.h"

//	what needs to be done with RT
//	Fix bug of transparent complex object. look for place, where
//	reflection changes the the normal of an object
//	but look out for a bug appearing in transparency
//
//	combine projection matrix perpective drawing for complex obj.
//	int ohter words combine opengl project and rtv1. implement
//	opengl by yourself, or inject opengl into this project
//
//	implement function pointers for decision making. test if
//	polymorphysm in c++ is faster for decision making then if()
//	test if implemented polymorphysm is faster.
//
//	rotation of normal map vectors doesn't work
//
//	implement snells law or transparent obect
//
//	dirty trick in obstructed is done even when 
//	the point is obstructed.
//
//	light ambient at distance

int			con(t_global *g)
{
	return (shot.x == g->ray->x && shot.y == g->ray->y);
}

int		mouse_press(int button, int x, int y, void *param)
{
	t_global *g;
	t_object a;

	g = param;
//	ft_bzero((int *)g->data_ptr, g->sz_l * HEIGHT);
	if (button == 1)
	{
		shot.x = -WIDTH / 2 + x;
		shot.y = HEIGHT / 2 - y;
		printf("\n%f, %f\n", shot.x, shot.y);
//		a = g->obj[g->objn];
//		printf("object is %d %s %f, %f, %f\n%f,%f, %f\n", g->objn, a.name, a.ctr->x, a.ctr->y, a.ctr->z, a.nr.x, a.nr.y, a.nr.z);
		printf("\nmouse press -> realc\n");
		start_threads(recalc, g);
//		start_threads(toimg, g);
	}
	return (1);
}

int	loop(void *p)
{
	t_global *g = (t_global *)p;
	g->li->x += 10;
	*g->cam_pos = diff(*g->cam_pos, *g->normal);
	printf("loop starting threads\n");
	start_threads(move, g);
	return (1);
}

void		free_arr(char **arr)
{
	int i;

	i = 0;
	while (*(arr + i) != NULL)
	{
		printf("now freeing %s\n", *(arr + i));
		free(*(arr + i));
		i++;
	}
}

int		putstr(char *s, int ret)
{
	int i;

	i = 0;
	while (*(s + i) != '\0')
	{
		write(1, s + i, 1);
		i++;
	}
	return (ret);
}

void		screen(int *a, int x, int y, t_global *g)
{
	int i = 0;
	int j = 0;
	while (j < y)
	{
		i = 0;
		while (i < x)
		{
			mlx_pixel_put(g->mlx_ptr, g->win_ptr, i, j, *(a + j * x + i));
			i++;
		}
		j++;
	}
}

void		draw_vectile(t_vector *t, int w, int h, t_global *g)
{
	int i;
	int j;

	i = 0;
	j = 0;
	while (j < h)
	{
		i = 0;
		while (i < w)
		{
//			printf("we are at %d,%d\n", i, j);
			mlx_pixel_put(g->mlx_ptr, g->win_ptr, i, j, brg(scale(255, *(t + j * h + i))));
			i++;
		}
		j++;
	}
}

void		draw_func(t_global *g)
{
	int i = -WIDTH / 2;

	while (i < WIDTH / 2)
	{
		mlx_pixel_put(g->mlx_ptr, g->win_ptr, i + WIDTH / 2, HEIGHT / 2- 20 * acos(i / (float)20), 0xFFFFFF);
		i++;
	}
}

int fd; 

int		main(int argc, char **argv)
{
	t_global g;
	t_vector ctr[argc];
	t_vector kenobi[6];
	int h;
	int w;

	g.mlx_ptr = mlx_init();

	if (RECORD_VIDEO)
		fd = open("./video", O_WRONLY | O_CREAT | O_APPEND, S_IRWXU);
	h = WIDTH;
	w = HEIGHT;
	g.cam_pos = &kenobi[0];
	g.angle = &kenobi[1];
	g.ray = &kenobi[2];
	g.li = &kenobi[3];
	g.normal = &kenobi[4];
	g.right = &kenobi[5];
	
	printf("mlx init\n");
	printf("ginit\n");
	ginit(&g);
	printf("check arg\n");
	if (!check_arg(argv, argc, &g, ctr))
		return (0);
	printf("new image\n");
	g.mlx_ptr = mlx_init();
	// with mymlx window should be created always after mlx_init
	// not necessery here, only for homogenity between versions
	g.win_ptr = mlx_new_window(g.mlx_ptr, WIDTH, HEIGHT, "window1");
	g.img_ptr = mlx_new_image(g.mlx_ptr, WIDTH, HEIGHT);
	g.data_ptr = (int *)mlx_get_data_addr(g.img_ptr, &g.bpp, &g.sz_l, &g.e);
//	g.win_ptr = mlx_new_window(g.mlx_ptr, WIDTH, HEIGHT, "window1");
	copy_tcps(&g);

	printf("starting threads\n");
	start_threads(recalc, &g);
	mlx_hook(g.win_ptr, 4, 4, mouse_press, &g);
	mlx_hook(g.win_ptr, 2, 2, key_press, &g);
	mlx_hook(g.win_ptr, 6, 6, mouse_move, &g);
//	mlx_loop_hook(g.mlx_ptr, loop, &g);
	mlx_loop(g.mlx_ptr);
}
