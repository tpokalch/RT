
#include "rtv1.h"

int		free_hits(t_global *g)
{
	int i;
	int j;

	// free 2D texture of t_objethit **hits
	printf("freeing hits...\n");
	i = -1;
	printf("freeing hits[i][j]\n");
	while (++i < HEIGHT && (j = -1))
		while(++j < WIDTH)
			free(*(*(g->hits + i) + j));
	printf("freeing hits[i]\n");
	i = -1;
	while (++i < HEIGHT)
		free(*(g->hits + i));
	printf("freeing hits\n");
	free(g->hits);

	printf("freeing rays...\n");
	// free 2D texture of t_vector **rays
	i = -1;
	while (++i < HEIGHT && (j = -1))
		while(++j < WIDTH)
			free(*(*(g->rays + i) + j));
	i = -1;
	while (++i < HEIGHT)
		free(*(g->rays + i));
	free(g->rays);

	// free thread copies
	printf("free thread copies\n");
	i = -1;
	while (++i < CORES)
	{
		printf("freeing %d thread\n", i);
		free(g->tcps[i]->recursion);
		free(g->tcps[i]->cosa);
		free(g->tcps[i]);
	}
	// free objects and agragated resources, if any
	i = 0;
	while (++i < g->argc + 1)
	{
		if (g->obj[i].name == complex)
		{
			printf("free complex\n");
			int j = 0;
			while (g->obj[i].pts[j])
			{
				free(g->obj[i].pts[j]);
				j++;
			}
			free(g->obj[i].pts);
			free(g->obj[i].tris);
		}
		if (g->obj[i].tile[0].data_ptr)
		{
			printf("freeing tiles\n");
			free(g->obj[i].tile[0].data_ptr);
			free(g->obj[i].tile[0].vectile);
			//and vectile!!
		}
	}
	i = 0;
	while (++i < g->argc + 1)
	{
		printf("freeing obj[%d]\n", i);
		free(g->obj[i].ctr);
	}

	free(g->obj);
	free(g->li);
	free(g->liz);
	return (1);
}

void		copy_obj(t_object *t, t_object *g)
{
	t = g;
//	t->tile = g->tile;
}
	
void		copy(t_global *tcps, t_global *g)
{
	tcps->mlx_ptr = g->mlx_ptr;
	tcps->win_ptr = g->win_ptr;
	tcps->img_ptr = g->img_ptr;
	tcps->data_ptr = g->data_ptr;
	tcps->bpp = g->bpp;
	tcps->sz_l = g->sz_l;
	tcps->e = g->e;
	tcps->light_switch = g->light_switch;

	tcps->cone[0] = g->cone[0];
	tcps->cone[1] = g->cone[1];


//	tcps->core = g->core; //correct core is assigned in copy_tcp
	tcps->ray = g->ray;
	tcps->angle = g->angle;
	tcps->normal = g->normal;
	tcps->li = g->li;
	tcps->cam_pos = g->cam_pos;

//	copy_obj(tcps->obj, g->obj);

	tcps->obj = g->obj;

	tcps->objn = g->objn;
	tcps->argc = g->argc;
	tcps->core = g->core;
	tcps->hits = g->hits;
	tcps->rays = g->rays;
	tcps->prim = g->prim;
	tcps->ambient = g->ambient;
	tcps->base[0] = g->base[0];
	tcps->base[1] = g->base[1];
//	tcps->base[1].y = -1;
	tcps->base[2] = g->base[2];

//	tcps->line_taken = g->line_taken;
//	tcps->e1 = g->e1;
//	tcps->sz_l1 = g->sz_l1;
//	tcps->bpp1 = g->bpp1;
//	tcps->tile_data_ptr = g->tile_data_ptr;
	tcps->mip_map = g->mip_map;
	tcps->recursion = g->recursion;
	tcps->lights = g->lights;
	tcps->white = g->white;
	tcps->spec_con = g->spec_con;;

//	tcps->hitli = (t_vector *)malloc(sizeof(t_vector) * g->lights); // deleted from  global
//	tcps->savehitli = (t_vector *)malloc(sizeof(t_vector) * g->lights); //also
	tcps->cosa = (double *)calloc(sizeof(double) * g->lights, 0);
	tcps->recursion = (int *)calloc(sizeof(int) * (g->argc + 1), 0);
//	ft_bzero(tcps->recursion, sizeof(int) * (g->argc + 1));
	//	tcps->ctrli = (t_vector *)malloc(sizeof(t_vector) * g->lights);
}

void		copy_tcps(t_global *g)
{
	int i;

	i = -1;
	while (++i < CORES)
	{
		copy(g->tcps[i], g);
		g->tcps[i]->core = i;
	}
}

//		init hits takes a pointer to a
//		local variable edfined in init data function.
//		that variable is a 2d array of POINTERS to objecthit
//		this is why ****hits is written

void		init_hits(t_objecthit ****hits)
{
	int i;
	int j;

        (*hits) = (t_objecthit ***)malloc(sizeof(t_objecthit **) * HEIGHT + 1);
	printf("hits is at addr %p\n", *hits);
	i = -1;
	while (++i < HEIGHT)
	{
		*(*hits + i) = (t_objecthit **)malloc(sizeof(t_objecthit) * WIDTH + 1);
		printf("hits[i] is at addr %p\n", *(*hits + i));
	}
	i = -1;
	while (++i < HEIGHT)
	{
		j = -1;
		while(++j < WIDTH)
		{
			*(*(*hits + i) + j)= (t_objecthit *)malloc(sizeof(t_objecthit));
//			printf("hits[i][j] is %p\n",*(*(*hits + i) + j));
		}
	}
}

void		init_rays(t_vector ****ray)
{
	int i;
	int j;

	(*ray) = (t_vector ***)malloc(sizeof(t_vector **) * HEIGHT + 1);
	printf("ray is at addr %p\n", *ray);
	i = -1;
	while (++i < HEIGHT)
	{
		*(*ray + i) = (t_vector **)malloc(sizeof(t_vector) * WIDTH + 1);
		printf("ray[i] is at addr %p\n", *(*ray + i));
	
	}
	i = -1;
	while (++i < HEIGHT)
	{
		j = -1;
		while(++j < WIDTH)
		{
			*(*(*ray + i) + j)= (t_vector *)malloc(sizeof(t_vector));

		}
	}
}
