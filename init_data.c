/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpokalch <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/03 15:51:47 by tpokalch          #+#    #+#             */
/*   Updated: 2019/08/28 21:38:21 by tpokalch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void		ginit(t_global *g)
{
	int i;
	t_objecthit ***hits;

	g->ray->z = lround(WIDTH / (double)2000 * 1600);
	i = -1;
	g->lights = 1;
	g->li = (t_vector *)malloc(sizeof(t_vector) * g->lights);
	while(++i < g->lights)
		init_vector(&g->li[i], 302, 200, 150);
//	init_vector(&g->li[0], -800,-400,300);
//	init_vector(&g->li[1], -200, 400, 100);
	g->liz = (double *)malloc(sizeof(double) * g->lights); 	
	i = -1;
	while(++i < g->lights)
		g->liz[i] = g->li[i].z;
//	g->liz[1] = g->li[1].z;
//	g->liz[2] = g->li[2].z;

	g->ambient = 90;
//	g->step_bri = (255 - g->ambient) / (double)g->lights;
	g->mip_map = 0;
//	g->mutex = PTHREAD_MUTEX_INITIALIZER;
	init_vector(&g->_0015, 0, 0, 15);

	init_vector(&g->base[0], 1, 0, 0);
	init_vector(&g->base[1], 0, 1, 0);
	init_vector(&g->base[2], 0, 0, 1);

	init_vector(g->angle, 0.45, 0, 0);
//	init_vector(g->angle, 0, 0, 0);

	init_vector(g->normal, 0, 0, 20);
	*g->normal = rotate(*g->normal, *g->angle);
//	init_vector(g->cam_pos, 0.135995, 100, 100.919620);
	init_vector(g->cam_pos, 0, 0, 100);
	init_vector(&g->white, 1, 1, 1);
	g->light_switch = 0;
	g->objn = 0;
	g->prim = 0;
//	g->obj = (t_object *)malloc(sizeof(t_object) * (g->argc + 1));
	(hits) = (t_objecthit ***)malloc(sizeof(t_objecthit **) * HEIGHT + 1);
	init_hits(hits);
	g->hits = hits;
	init_rays(&g->rays);
	g->line_taken = (int *)malloc(sizeof(int) * HEIGHT);
	ft_bzero(g->line_taken, 4 * WIDTH);
	g->recursion = 0;
//	while (++i < WIDTH)
//		printf("the line taken is %d\n", g->line_taken[i]);
	i = -1;
	while (++i < CORES)
		g->tcps[i] = (t_global *)malloc(sizeof(t_global));
//	copy_tcps(g);
	printf("end ginit\n");
}

void		stretch(int *a, int d, int h)
{
	int i;
	int j;

	i = d / 2 - 1;
	j = h / 2 - 1;
//	printf("d is %d\n", d);
//	printf("at last place is %d\n", *(a + d * 16 + 12));
	while (j >= 0)
	{
		i = d / 2 - 1;
		while (i >= 0)
		{
			*(a + d * 2 * j + 2 * i) = *(a + d * j + i);
//			if (i > 0)
				*(a + d * 2 * j + 2 * i + 1) = *(a + d * j + i);
				*(a + d * (2 * j + 1) + 2 * i) = *(a + d * j + i);
				*(a + d * (2 * j + 1) + 2 * i + 1) = *(a + d * j + i);
			if (i == d && j == d)
				printf("copy to %d, %d\n", 2 * i - 1, 2 * (j - 1));
			i--;
		}
		j--;
	}
}

t_vector		*norm_tile(int *tile, int w, int h, t_global *g)
{
	int i;
	int j;
	t_vector *ret;

	i = 0;
	j = 0;
	ret = (t_vector *)malloc(sizeof(t_vector) * h * w);
	while (j < w)
	{
		i = 0;
		while (i < h)
		{
//			printf("copying at %d,%d\n", i, j);
			*(ret + j * h + i) = base255(rgb(*(tile + j * h + i)));
			i++;
		}
		j++;
	}
	return (ret);
}

void		init_tile(int i, char *tile, t_object *obj, t_global *g)
{
//	printf("writing xpm file\n");
	obj[i].tile[0].ptr = mlx_xpm_file_to_image
		(g->mlx_ptr,
		tile,
		&obj[i].tile[0].w,
		&obj[i].tile[0].h
		);
//	printf("coppying xpm adress\n");
	obj[i].tile[0].data_ptr = (int *)mlx_get_data_addr
		(
		obj[i].tile[0].ptr,	
		&obj[i].tile[0].bpp,
		&obj[i].tile[0].sz_l,
		&obj[i].tile[0].e
		);
	obj[i].tile[0].w2 = obj[i].tile[0].w / 2;
	obj[i].tile[0].h2 = obj[i].tile[0].h / 2;
	printf("xpm image dimansions are %d,%d\n", obj[i].tile[0].w, obj[i].tile[0].h);
	int k = 1;
	obj[i].tile[0].vectile = norm_tile(obj[i].tile[0].data_ptr, obj[i].tile[0].w, obj[i].tile[0].h, g);
	if (g->mip_map)
	{
	while (k < 10)
	{
//			printf("i is %d\n", k);
		obj[i].tile[k].w = obj[i].tile[k - 1].w;
		obj[i].tile[k].h = obj[i].tile[k - 1].h;

		obj[i].tile[k].data_ptr = (int *)malloc(sizeof(int) * obj[i].tile[0].w * obj[i].tile[0].h);

		alias(obj[i].tile[k].data_ptr,
		obj[i].tile[k - 1].data_ptr,
		obj[i].tile[k - 1].w, obj[i].tile[k - 1].h, obj[i].tile[k - 1].w, obj[i].tile[k - 1].h);

		obj[i].tile[k].w2 = obj[i].tile[k].w / 2;
		obj[i].tile[k].h2 = obj[i].tile[k].h / 2;
//		white(obj[i].tile[k].data_ptr, obj[i].tile[k].w ,obj[i].tile[k].h, k * 100000);
		k++;
	}
	k = 1;
	int s = 0;
	while (k < 10)
	{
//		printf("smothing k is %d\n", k);
		s = 0;
//		smooth(obj[i].tile[k].data_ptr, obj[i].tile[k].w,
//		obj[i].tile[k].h,obj[i].tile[k].w / exp2(k),
//		obj[i].tile[k].h / exp2(k), g);
		while (s++ < k)
			stretch(obj[i].tile[k].data_ptr, obj[i].tile[k].w , obj[i].tile[k].h);
		printf("obj i is %d tilek is %d\n", i, k);
		obj[i].tile[k].vectile = norm_tile(obj[i].tile[k].data_ptr, obj[i].tile[k].w, obj[i].tile[k].h, g);
	k++;
	}
	k = 0;
	obj[i].tile[0].mipq = fmin(log2(obj[i].tile[0].h), log2(obj[i].tile[0].w));
	printf("tile quant is %d\n", obj[i].tile[0].mipq);
	}
}

void		init_plane(t_vector *ctr, int i, t_global *g)
{
	t_object a;

	a = g->obj[i];
	g->obj[i].name = "plane";
	g->obj[i].id = i;
	g->obj[i].hit = &hit_plane;
	g->obj[i].bright = &bright_plane;
	g->obj[i].simple_bright = &simple_bright_plane;
	printf("here\n");
	g->obj[i].ctr = &ctr[i];
	printf("here2\n");
	g->obj[i].ctr->x = 0;
	g->obj[i].ctr->y = -200;
	g->obj[i].ctr->z = 0;
	g->obj[i].rd = 10;
	g->obj[i].color = rgb(0x010101);

	g->obj[i].ang.x = 0;
	g->obj[i].ang.y = 0;
	g->obj[i].ang.z = 0;
	g->obj[i].re = 0.7;
	g->obj[i].spec = 4;
	g->obj[i].soft = 0;
	init_vector(&g->obj[i].base[0], 1, 0, 0);
	init_vector(&g->obj[i].base[1], 0, 1, 0);
	init_vector(&g->obj[i].base[2], 0, 0, 1);
	printf("end init\n");
//	init_tile(i,"./tiles/brick.xpm", g->obj, g);
//	free(g->obj[i].tile[0].data_ptr);
	g->obj[i].tile[0].data_ptr = NULL;

	if (g->obj[i].trans)
		g->obj[i].simple_bright = bright_plane;
//	g->obj[i].simple_bright = bright_plane;
}

int		arrheight(void **a)
{
	int i;

	i = 0;
	while (*(a + i))
		i++;
	return (i);
}

t_object	*create_tris(t_vector **pts, t_object obj, t_global *g)
{
	printf("starting to create tri\n");
//	printf("line len is %d\n", (**pts).len);
	int j;
	int i;
	int h;
	int retc;
	t_vector rc;
	t_vector smallspace;
	t_vector pt;
	t_object *ret;
	int len;

	len = obj.ptdim.x / 20;
	h = arrheight((void **)pts);
	ret = (t_object *)malloc(sizeof(t_object) * (len * h * 2 + 1));
	j = 0;
	i = 0;
	retc = 0;
//	init_vector(&smallspace, 0.0001, 0.0001, 0.0001);
	init_vector(&smallspace, 0, 0, 0);
	while (*(pts + j + 1))
	{
		i = 0;
		while (i + 1 < len)
		{
			ret[retc].bd1 = sum(rotate(*(*(pts + j) + i), obj.ang), *obj.ctr);
			ret[retc].bd2 = sum(rotate(*(*(pts + j) + i + 1), obj.ang), *obj.ctr);
			ret[retc].bd3 = sum(rotate(*(*(pts + j + 1) + i), obj.ang), *obj.ctr);	


			ret[retc + 1].bd1 = sum(rotate(sum(*(*(pts + j + 1) + i + 1), smallspace), obj.ang), *obj.ctr);
			ret[retc + 1].bd2 = sum(rotate(sum(*(*(pts + j) + i + 1), smallspace), obj.ang), *obj.ctr);
			ret[retc + 1].bd3 = sum(rotate(sum(*(*(pts + j + 1) + i), smallspace), obj.ang), *obj.ctr);	

			ret[retc].hit = &hit_tri;
			ret[retc + 1].hit = &hit_tri;

			ret[retc].bright = &bright_tri;
			ret[retc].simple_bright = &bright_tri;
			ret[retc + 1].bright = &bright_tri;
//CHANGE BACK TO SIMPLE LATE
			ret[retc + 1].simple_bright = &bright_tri;

			ret[retc].id = g->argc + retc + 1;
			ret[retc + 1].id = g->argc + retc + 2;

			ret[retc].name = "tri";
			ret[retc + 1].name = "tri";
//			printf("just inited %s\n", ret[retc].name);
//		ret[retc].nr = norm(cross(diff(ret[retc].bd1, ret[retc].bd3), diff(ret[retc].bd2, ret[retc].bd3)));

		ret[retc].base[0] = norm(diff(ret[retc].bd1, ret[retc].bd3));
		ret[retc].base[2] = norm(diff(ret[retc].bd2, ret[retc].bd3));
		ret[retc].base[1] = norm(cross(diff(ret[retc].bd1, ret[retc].bd3), diff(ret[retc].bd2, ret[retc].bd3)));
//		ret[retc].base[1] = scale(-1, norm(cross(diff(ret[retc].bd1, ret[retc ].bd3), diff(ret[retc].bd2, ret[retc].bd3))));



//		ret[retc + 1].nr = norm(cross(diff(ret[retc + 1].bd1, ret[retc + 1].bd3), diff(ret[retc + 1].bd2, ret[retc + 1].bd3)));


		ret[retc + 1].base[0] = norm(diff(ret[retc + 1].bd1, ret[retc + 1].bd3));
		ret[retc + 1].base[2] = norm(diff(ret[retc + 1].bd2, ret[retc + 1].bd3));
		ret[retc + 1].base[1] = norm(cross(diff(ret[retc + 1].bd1, ret[retc + 1].bd3), diff(ret[retc + 1].bd2, ret[retc + 1].bd3)));
//	ret[retc + 1].base[1] = scale(-1, norm(cross(diff(ret[retc + 1].bd1, ret[retc + 1].bd3), diff(ret[retc + 1].bd2, ret[retc + 1].bd3))));


		ret[retc].ctr = obj.ctr;
		ret[retc + 1].ctr = obj.ctr;

//		init_tile(i, "./tiles/brick.xpm", ret + retc, g);
//		init_tile(i, "./tiles/brick.xpm", ret + retc + 1, g);

		ret[retc].tile[0] = obj.tile[0];
		ret[retc + 1].tile[0] = obj.tile[0];


//		ret[retc + 1].tile[0].data_ptr = NULL;
//		ret[retc].tile[0].data_ptr = NULL;

//		printf("end friing tile\n");
		ret[retc].color = obj.color;
		ret[retc + 1].color = rgb(0x010000);


		ret[retc].re = obj.re;
		ret[retc + 1].re = obj.re;

		ret[retc].spec = obj.spec;
		ret[retc + 1].spec = obj.spec;

		i++;
		retc = retc + 2;
		}
		j++;
	}
	(ret)->rd = (len - 1)* (h - 1)* 2 + 1;
//	printf("there are %d tris\n", ret->rd);
	return (ret);


}

t_object	*init_frame(t_object obj, t_global *g)
{
	t_object *ret;

	ret = (t_object *)malloc(sizeof(t_object));
	ret->name = "sphere";
	ret->hit = &hit_sphere;
	ret->ctr = obj.ctr;


        t_vector        bas[3];
        t_vector rc;
	bas[0] = scale(obj.ptdim.x, g->base[0]);
	bas[1] = scale(obj.ptdim.y, g->base[1]);
	bas[2] = scale(obj.ptdim.z, g->base[2]);

        printf("shift in init frame %f,%f,%f\n%f,%f,%f\n%f,%f,%f\n", bas[0].x, bas[0].y, bas[0].z, bas[1].x, bas[1].y, bas[1].z, bas[2].x, bas[2].y, bas[2].z);

	rc = scale(-0.5, sum(sum(bas[0], bas[1]), bas[2]));

	ret->rd2 = ceil(dot(rc, rc)) - 5000;
	printf("frame rd2 is %d\n", ret->rd2);
	ret->rd = sqrt(ret->rd2);
	return (ret);
}


void		init_complex(t_vector *ctr, int i, t_global *g)
{
//	printf("line len in init %d\n", (**(g->obj[i].pts)).len);
	g->obj[i].name = "complex";
	g->obj[i].id = i;
	g->obj[i].hit = &hit_complex;
	g->obj[i].bright = &bright_plane;
	g->obj[i].simple_bright = &simple_bright_plane;
	g->obj[i].ctr = &ctr[i];
	g->obj[i].ctr->x =/*270*/ 0;
	g->obj[i].ctr->y =/*270*/ 10;
	g->obj[i].ctr->z =/*270*/550;

	printf("center is %f\n", g->obj[i].ctr->z);
	g->obj[i].rd2 = g->obj[i].rd * g->obj[i].rd;
	g->obj[i].color = rgb(0x010000);
	g->obj[i].ang.x = /*-(-M_2_PI + 2) - 1 */-M_PI_2 /*- 0.7*/;
	g->obj[i].ang.y = 0;
	g->obj[i].ang.z = 0;

	init_vector(&g->obj[i].base[0], 1, 0, 0);
	init_vector(&g->obj[i].base[1], 0, 1, 0);
	init_vector(&g->obj[i].base[2], 0, 0, 1);	
//	init_vector(&g->obj[i].nr, 1, 0, 0);

	g->obj[i].frame = init_frame(g->obj[i], g);
	printf("frame name is %s\n", g->obj[i].frame->name);
	init_tile(i, "./tiles/brick.xpm", g->obj, g);
//	g->obj[i].tile[0].data_ptr = NULL;
	g->obj[i].re = 0;
	g->obj[i].spec = 4;
	g->obj[i].tris = create_tris(g->obj[i].pts, g->obj[i], g);
	g->obj[i].rd = g->obj[i].tris->rd - 1;
}


void		init_tri(t_vector *ctr, int i, t_global *g)
{
	g->obj[i].name = "tri";
	g->obj[i].id = i;
	g->obj[i].hit = &hit_tri;
	g->obj[i].bright = &bright_tri;
	g->obj[i].simple_bright = &simple_bright_tri;

	g->obj[i].ctr = &ctr[i];
	if (i == 1)
	{
		init_vector(&g->obj[i].bd1, -200, 0, 600);
		init_vector(&g->obj[i].bd2, 200, 0, 600);
		init_vector(&g->obj[i].bd3, 0, 100, 700);
	}
	if (i == 2)
	{
		init_vector(&g->obj[i].bd1, -200.0002, 0.0002, 600.0002);
		init_vector(&g->obj[i].bd2, 200.0002, 0.0002, 600.0002);
		init_vector(&g->obj[i].bd3, 0, -150, 800);
	}
	if (i == 3)
	{
		init_vector(&g->obj[i].bd1, -200.0004, 0.0002, 600.0004);
		init_vector(&g->obj[i].bd2, 0.0002, -150.0002, 800.0002);
		init_vector(&g->obj[i].bd3, 0.0002, 100.0002, 700.0002);
	}
	init_vector(g->obj[i].ctr, g->obj[i].bd1.x, g->obj[i].bd1.y, g->obj[i].bd1.z);	


	g->obj[i].rd = 10;
	g->obj[i].color = rgb(0x000101);
//	g->obj[i].nr = norm(cross(diff(g->obj[i].bd1, g->obj[i].bd3), diff(g->obj[i].bd2, g->obj[i].bd3)));

	g->obj[i].ang.x = 0;
	g->obj[i].ang.y = 0;
	g->obj[i].ang.z = 0;
//	init_vector(&g->obj[i].base[0], 1, 0, 0);
//	init_vector(&g->obj[i].base[1], 0, 1, 0);
//	init_vector(&g->obj[i].base[2], 0, 0, 1);
	g->obj[i].base[1] = norm(cross(diff(g->obj[i].bd1, g->obj[i].bd3), diff(g->obj[i].bd2, g->obj[i].bd3)));


	init_tile(i, "./tiles/brick.xpm", g->obj, g);
//	free(g->obj[i].tile[0].data_ptr);
//	g->obj[i].tile[0].data_ptr = NULL;

}


void		init_cylinder(t_vector *ctr, int i, t_global *g)
{
	g->obj[i].name = "cylinder";
	g->obj[i].id = i;
	g->obj[i].hit = &hit_cylinder;
	g->obj[i].bright = &bright_cylinder;
	g->obj[i].simple_bright = &simple_bright_cylinder;

	g->obj[i].ctr = &ctr[i];
	printf("center %p\n", g->obj[i].ctr);
	g->obj[i].ctr->x = 0;
	g->obj[i].ctr->y = 0;
	g->obj[i].ctr->z = 600;
	printf("center is %f\n", g->obj[i].ctr->z);
	g->obj[i].rd = 70;
	g->obj[i].rd2 = g->obj[i].rd * g->obj[i].rd;
	g->obj[i].color = rgb(0x010001);
	g->obj[i].ang.x = 0;
	g->obj[i].ang.y = 0;
	g->obj[i].ang.z = 0;
	init_vector(&g->obj[i].base[0], 1, 0, 0);
	init_vector(&g->obj[i].base[1], 0, 1, 0);
	init_vector(&g->obj[i].base[2], 0, 0, 1);	
	g->obj[i].spec = 4;
	g->obj[i].re = 0.5;
/*
	g->obj[i].tile[0].ptr = mlx_xpm_file_to_image
		(g->mlx_ptr,
		"./tiles/blank.xpm",
		&g->obj[i].tile[0].w,
		&g->obj[i].tile[0].h
		);
	printf("THIS IS CYLINDER COPPYING \n");
	printf("coppying xpm adress\n");
	g->obj[i].tile[0].data_ptr = (int *)mlx_get_data_addr
		(
		g->obj[i].tile[0].ptr,	
		&g->obj[i].tile[0].bpp,
		&g->obj[i].tile[0].sz_l,
		&g->obj[i].tile[0].e
		);
*/
//	g->obj[i].tile[0].data_ptr = NULL;
	init_tile(i,"./tiles/blank.xpm", g->obj, g);
	g->obj[i].tile[0].w2 = g->obj[i].tile[0].w / 2;
	g->obj[i].tile[0].h2 = g->obj[i].tile[0].h / 2;

	if (g->obj[i].tile[0].data_ptr || g->obj[i].re || g->obj[i].trans || g->obj[i].spec)
		g->obj[i].bright = &bright_cylinder;
	else
		g->obj[i].bright = &simple_bright_cylinder;
	if (g->obj[i].re || g->obj[i].trans)
		g->obj[i].simple_bright = &bright_cylinder;
	else
		g->obj[i].simple_bright = simple_bright_cylinder;

}

void		init_sphere(t_vector *ctr, int i, t_global *g)
{
	g->obj[i].name = "sphere";
	g->obj[i].id = i;
	g->obj[i].hit = &hit_sphere;
	g->obj[i].bright = &bright_sphere;
	g->obj[i].simple_bright = &simple_bright_sphere;
	g->obj[i].ctr = &ctr[i];
	printf("center %p\n", g->obj[i].ctr);
	g->obj[i].trans = 0;
	g->obj[i].re = 0.5;
	g->obj[i].spec = 4;
	init_vector(g->obj[i].ctr, 0, 0, 300);
	printf("center is %f\n", g->obj[i].ctr->z);
	g->obj[i].rd = 100;
	g->obj[i].rd2 = g->obj[i].rd * g->obj[i].rd;
	g->obj[i].color = rgb(0x000101);
	init_vector(&g->obj[i].ang, 0, 0, 0);
	init_vector(&g->obj[i].base[0], 1, 0, 0);
	init_vector(&g->obj[i].base[1], 0, 1, 0);
	init_vector(&g->obj[i].base[2], 0, 0, 1);
	printf("hello\n");
	init_tile(i,"./tiles/blank.xpm", g->obj, g);

//	g->obj[i].tile[0].data_ptr = NULL;
	if (g->obj[i].trans)
		g->obj[i].simple_bright = bright_sphere;
}

void		init_spheror(t_vector *ctr, int i, t_global *g)
{
	g->obj[i].name = "spheror";
	g->obj[i].id = i;
	g->obj[i].hit = &hit_sphere;
	g->obj[i].bright = &bright_spheror;
	g->obj[i].simple_bright = &simple_bright_spheror;
	g->obj[i].bright = &bright_spheror;
	g->obj[i].ctr = &ctr[i];
	printf("center %p\n", g->obj[i].ctr);
	g->obj[i].ctr->x = 0;
	g->obj[i].ctr->y = 0;
	g->obj[i].ctr->z = 300;
	printf("center is %f\n", g->obj[i].ctr->z);
	g->obj[i].rd = 100;
	g->obj[i].rd2 = g->obj[i].rd * g->obj[i].rd;
	g->obj[i].color = rgb(0x010000);
	g->obj[i].ang.x = 0;
	g->obj[i].ang.y = 0;
	g->obj[i].ang.z = 0;
	g->obj[i].re = 0;
	init_vector(&g->obj[i].base[0], 1, 0, 0);
	init_vector(&g->obj[i].base[1], 0, 1, 0);
	init_vector(&g->obj[i].base[2], 0, 0, 1);

//	init_tile(i,"./tiles/earth.xpm", g->obj, g);
//	g->obj[i].tile[0].data_ptr = NULL;
	if (g->obj[i].tile[0].data_ptr || g->obj[i].re || g->obj[i].trans)
		g->obj[i].bright = &bright_spheror;
	else
		g->obj[i].bright = &simple_bright_plane;

}

void		init_cone(t_vector *ctr, int i, t_global *g)
{
	g->obj[i].name = "cone";
	g->obj[i].id = i;
	g->obj[i].hit = &hit_cone;
	g->obj[i].bright = &bright_cone;
	g->obj[i].simple_bright = &simple_bright_cone;

	g->obj[i].ctr = &ctr[i];
	printf("center %p\n", g->obj[i].ctr);
	g->obj[i].ctr->x = 0;
	g->obj[i].ctr->y = 0;
	g->obj[i].ctr->z = 600;
	printf("center is %f\n", g->obj[i].ctr->z);
	g->obj[i].rd = 1;
	g->obj[i].rd2 = g->obj[i].rd * g->obj[i].rd;
	g->obj[i].color = rgb(0x000100);
	g->obj[i].ang.x = 0;
	g->obj[i].ang.y = 0;
	g->obj[i].ang.z = 0;
	init_vector(&g->obj[i].base[0], 1, 0, 0);
	init_vector(&g->obj[i].base[1], 0, 1, 0);
	init_vector(&g->obj[i].base[2], 0, 0, 1);	
	g->obj[i].re = 0.5;
	g->obj[i].spec = 0;
	g->obj[i].soft = 0;
	init_tile(i,"./tiles/blank.xpm", g->obj, g);

/*
	g->obj[i].tile[0].ptr = mlx_xpm_file_to_image
		(g->mlx_ptr,
		"./tiles/blank.xpm",
		&g->obj[i].tile[0].w,
		&g->obj[i].tile[0].h
		);
	printf("hello999\n");
	g->obj[i].tile[0].data_ptr = (int *)mlx_get_data_addr
		(
		g->obj[i].tile[0].ptr,	
		&g->obj[i].tile[0].bpp,
		&g->obj[i].tile[0].sz_l,
		&g->obj[i].tile[0].e
		);
	printf("finished getting addr\n");
	g->obj[i].tile[0].w2 = g->obj[i].tile[0].w / 2;
	g->obj[i].tile[0].h2 = g->obj[i].tile[0].h / 2;
*/

	if (g->obj[i].trans)
		g->obj[i].simple_bright = &bright_cone;
	g->obj[i].simple_bright = simple_bright_cone;
}

