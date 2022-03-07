/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpokalch <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/03 15:51:47 by tpokalch          #+#    #+#             */
/*   Updated: 2019/09/30 05:36:28 by tpokalch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void		ginit(t_global *g)
{
//	NOTE: if you add a field to a g structure make shure
//	to copy teh field into a tcp in memory.c!!!
	int i;
	t_objecthit ***hits;

	g->ray->z = lround(WIDTH / (double)2000 * 1600);
	g->lights = 1; //if more than 1, disable specular on plane!
	g->li = (t_vector *)malloc(sizeof(t_vector) * g->lights);
	shot.x = -1;

//	init_vector(&g->li[0], 300, 300, -100); // for many li

//	init_vector(&g->li[0], -50, 187, 450);
//	init_vector(&g->li[0], -165,82,13);
//	init_vector(&g->li[0], -712, 312, 609);
//	init_vector(&g->li[0], -181, 150, 246);
//	init_vector(&g->li[0], 376,365,227);
	init_vector(&g->li[0], 117,152,410);
	init_vector(&g->li[0], 62,118,297);
	init_vector(&g->li[0], 103,10,269);
	init_vector(&g->li[0], 48.362500,90.012548,243.742205);
	init_vector(&g->li[0], 258.415865,-80.441955,176.892432);
	init_vector(&g->li[0], 154,11,420);

	init_vector(&g->li[0], 183.788427,24.676411,161.037996);
	init_vector(&g->li[0], 2053.659911,-139.807833,726.474466);
	init_vector(&g->li[0], -137.162366,586.490258,688.022130);
	if (g->lights > 1)
		init_vector(&g->li[1], -360,-47,267);

//# if 0	
	i = 0;
	while(++i < g->lights)
		init_vector(&g->li[i], g->li[0].x -((10 * i) % lround(sqrt(g->lights))  ), g->li[0].y +  i * 10 / lround(sqrt(g->lights))  /*((10 * i) % lround(sqrt(g->lights))  )*/, g->li[0].z /*+  (i * 10) / lround(sqrt(g->lights )) */);

//#endif 
//	init_vector(&g->li[0], -800,-400,300);
//	init_vector(&g->li[1], -200, 400, 100);
//	init_vector(&g->li[0], -600, 100, 500);
//	init_vector(&g->li[0], -600, 500, 800);
//	init_vector(&g->li[0], -576, 211, 230);
//	init_vector(&g->li[0], -446, -10, 1320);
//	init_vector(&g->li[0], -667, -13, 1800);

	g->liz = (double *)malloc(sizeof(double) * g->lights); 	
	i = -1;
	while(++i < g->lights)
		g->liz[i] = g->li[i].z;
//	g->liz[1] = g->li[1].z;
//	g->liz[2] = g->li[2].z;


//	g->ambient = 255;
	g->ambient = 80;


//	g->step_bri = (255 - g->ambient) / (double)g->lights;
	g->mip_map = 0;
//	g->mutex = PTHREAD_MUTEX_INITIALIZER;
	init_vector(&g->_0015, 0, 0, 15);
	init_vector(&g->_1500, 15, 0, 0);

	init_vector(&g->base[0], 1, 0, 0);
	init_vector(&g->base[1], 0, 1, 0);
	init_vector(&g->base[2], 0, 0, 1);

//	init_vector(g->angle, 0.3, -0.6, 0);
	init_vector(g->angle, 0.45, -0.2, 0); //look down
//	init_vector(g->angle, 0.15, 0, 0);
//	init_vector(g->angle, 0, 0, 0);


	init_vector(g->normal, 0, 0, 7);
	init_vector(g->right, 7, 0, 0);

	*g->normal = rotate(*g->normal, *g->angle);
//	init_vector(g->cam_pos, 0.135995, 100, 100.919620);
//	init_vector(g->cam_pos, 109.86, -6.69, 33.44);
	init_vector(g->cam_pos, 0, 100, -400);

	init_vector(&g->white, 1, 1, 1);
	g->spec_con = scale(1 / (float)g->lights, g->white);
	g->light_switch = 0;
	g->objn = 0;
	g->prim = 0;
//	g->obj = (t_object *)malloc(sizeof(t_object) * (g->argc + 1));
	(hits) = (t_objecthit ***)malloc(sizeof(t_objecthit **) * HEIGHT + 1);
	init_hits(hits);
	g->hits = hits;
	init_rays(&g->rays);
//	g->line_taken = (int *)malloc(sizeof(int) * HEIGHT);
//	ft_bzero(g->line_taken, 4 * WIDTH);
	g->recursion = 0;
	i = -1;
	while (++i < CORES)
		g->tcps[i] = (t_global *)malloc(sizeof(t_global));
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
t_vector		*norm_normal_map(int *tile, int w, int h, t_global *g)
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

//			*(ret + j * h + i) = base(rgb(*(tile + j * h + i)));
//			*(ret + j * h + i).x = (base255(rgb(*(tile + j * h + i))));

			(ret + j * h + i)->x =  (base255(rgb(*(tile + j * h + i)))).x - 0.5;
			(ret + j * h + i)->y =  ((base255(rgb(*(tile + j * h + i)))).z - 0.5);
			(ret + j * h + i)->z =  (base255(rgb(*(tile + j * h + i)))).y - 0.5;
			*(ret + j * h + i) = norm(*(ret + j * h + i));
			i++;
		}
		j++;
	}
	return (ret);
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

//			*(ret + j * h + i) = base(rgb(*(tile + j * h + i)));
			*(ret + j * h + i) = (base255(rgb(*(tile + j * h + i))));
			i++;
		}
		j++;
	}
	return (ret);
}



//		0 for tile, 1 for normal map
void		init_tile(int type, char *tilename, t_tile *tile, t_global *g)
{
//	printf("writing xpm file\n");
	tile[0].ptr = mlx_xpm_file_to_image
		(g->mlx_ptr,
		tilename,
		&tile[0].w,
		&tile[0].h
		);
//	printf("coppying xpm adress\n");
	if (tile[0].ptr == NULL)
	{
		printf("ERROR LOADING TILE\n");
		return ;
	}
	tile[0].data_ptr = (int *)mlx_get_data_addr
		(
		tile[0].ptr,	
		&tile[0].bpp,
		&tile[0].sz_l,
		&tile[0].e
		);
	if (tile[0].data_ptr == NULL)
	{
		printf("ERROR LOADING TILE 2\n");
		return ;
	}
	
	tile[0].w2 = tile[0].w / 2;
	tile[0].h2 = tile[0].h / 2;
	printf("xpm image dimansions are %d,%d\n", tile[0].w, tile[0].h);
	int k = 1;
	if (type == 0)
		tile[0].vectile = norm_tile(tile[0].data_ptr, tile[0].w, tile[0].h, g);
	if (type == 1)
		tile[0].vectile = norm_normal_map(tile[0].data_ptr, tile[0].w, tile[0].h, g);
////////////////////
//	tile[0].w2 *= 2;
//	tile[0].w *= 2;



/*	if (g->mip_map)
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
*/
}

void		init_plane(t_vector *ctr, int i, t_global *g)
{
	t_object a;

	a = g->obj[i];
	g->obj[i].name = plane/*1*/;
	g->obj[i].id = i;
	g->obj[i].hit = &hit_plane;
	g->obj[i].bright = &bright_plane;
	g->obj[i].simple_bright = &simple_bright_plane;
	g->obj[i].get_normal = &get_normal_plane;

	printf("here\n");
	g->obj[i].ctr = &ctr[i];
	printf("here2\n");
	g->obj[i].ctr->x = 0;
	g->obj[i].ctr->y = -200;
	g->obj[i].ctr->z = 0;
	g->obj[i].rd = 10;
//	g->obj[i].color = scale(0.7, rgb(0x010100));
	g->obj[i].color = rgb(0x010101);


	g->obj[i].ang.x = 0;
	g->obj[i].ang.y = 0;
	g->obj[i].ang.z = 0;
	g->obj[i].re = 0; //0.6 is good
	g->obj[i].spec = 4;
	g->obj[i].soft = 0;
	g->obj[i].trans = 0;
	init_vector(&g->obj[i].base[0], 1, 0, 0);
	init_vector(&g->obj[i].base[1], 0, 1, 0);
	init_vector(&g->obj[i].base[2], 0, 0, 1);
	printf("end init\n");

	g->obj[i].tile[0].data_ptr = NULL;

//	init_tile(0,"./tiles/brick.xpm", g->obj[i].tile, g);
//	init_tile(0,"./tiles/normalmap.xpm", g->obj[i].tile, g);
//	init_tile(0,"./tiles/wheelnormal.xpm", g->obj[i].tile, g);
//	init_tile(0,"./tiles/glass_base.xpm", g->obj[i].tile, g);


//	init_tile(0,"./tiles/basecolor.xpm", g->obj[i].tile, g);
	g->obj[i].normal_map.data_ptr = NULL;
//	init_tile(1,"./tiles/basecolornormal.xpm", &g->obj[i].normal_map, g);
//	init_tile(1,"./tiles/wheelnormal.xpm", &g->obj[i].normal_map, g);
//	init_tile(1,"./tiles/normalmap.xpm", &g->obj[i].normal_map, g);



//	init_tile(1,"./tiles/glass_normal.xpm", &g->obj[i].normal_map, g);
//	if (g->obj[i].trans)
//		g->obj[i].simple_bright = bright_plane;
//	g->obj[i].simple_bright = bright_plane;

//	campos(g);
	printf("ctr is %f,%f,%f\n", g->obj[i].ctr->x, g->obj[i].ctr->y, g->obj[i].ctr->z); 
	printf("cam_pos is %f,%f,%f\n", g->cam_pos->x, g->cam_pos->y, g->cam_pos->z); 

	if (dot(diff(*g->obj[i].ctr, *g->cam_pos), g->obj[i].base[1]) > 0)
	{
		printf("_______CHANGE NORMALS\n");
		g->obj[i].base[1] = scale(-1, g->obj[i].base[1]);
//			      1==inside
//		g->obj[i].cam_pos = 1;
	}

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

			ret[retc].name = tri/*5*/;
			ret[retc + 1].name = tri/*5*/;
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
	//	ret[retc].base[1] = scale(-1, ret[retc].base[1]);
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

		ret[retc].trans = obj.trans;
		ret[retc + 1].trans = obj.trans;

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
	ret->name = sphere/*0*/;
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
	g->obj[i].name = complex;
	g->obj[i].id = i;
	g->obj[i].hit = &hit_complex;
	g->obj[i].bright = &bright_plane;
	g->obj[i].simple_bright = &simple_bright_plane;
	g->obj[i].get_normal = &get_normal_plane;
	g->obj[i].ctr = &ctr[i];
	g->obj[i].ctr->x = 0;
	g->obj[i].ctr->y = 0;
	g->obj[i].ctr->z = 1200;

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
	printf("frame name is %u\n", g->obj[i].frame->name);
//	init_tile(i, "./tiles/brick.xpm", g->obj, g);
	g->obj[i].tile[0].data_ptr = NULL;
	g->obj[i].re = 0;
	g->obj[i].spec = 5;
	g->obj[i].trans = 0;
	g->obj[i].tris = create_tris(g->obj[i].pts, g->obj[i], g);
	g->obj[i].rd = g->obj[i].tris->rd - 1;
}


void		init_tri(t_vector *ctr, int i, t_global *g)
{
	g->obj[i].name = tri;
	g->obj[i].id = i;
	g->obj[i].hit = &hit_tri;
	g->obj[i].bright = &bright_tri;
	g->obj[i].simple_bright = &simple_bright_tri;
	g->obj[i].get_normal = &get_normal_plane;

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


	init_tile(i, "./tiles/brick.xpm", g->obj[i].tile, g);
//	free(g->obj[i].tile[0].data_ptr);
//	g->obj[i].tile[0].data_ptr = NULL;

}


void		init_cylinder(t_vector *ctr, int i, t_global *g)
{
	g->obj[i].name = cylinder;
	g->obj[i].id = i;
	g->obj[i].hit = &hit_cylinder;
	g->obj[i].bright = &bright_cylinder;
	g->obj[i].simple_bright = &simple_bright_cylinder;
	g->obj[i].get_normal = &get_normal_cyl;

	g->obj[i].ctr = &ctr[i];
	printf("center %p\n", g->obj[i].ctr);
	g->obj[i].ctr->x = 0;
	g->obj[i].ctr->y = 0;
	g->obj[i].ctr->z = 600;
	printf("center is %f\n", g->obj[i].ctr->z);
	g->obj[i].rd = 70;
	g->obj[i].rd2 = g->obj[i].rd * g->obj[i].rd;
	g->obj[i].color = rgb(0x000101);
	g->obj[i].ang.x = 0;
	g->obj[i].ang.y = 0;
	g->obj[i].ang.z = 0;
	init_vector(&g->obj[i].base[0], 1, 0, 0);
	init_vector(&g->obj[i].base[1], 0, 1, 0);
	init_vector(&g->obj[i].base[2], 0, 0, 1);
	g->obj[i].spec = 2/*4*/;
	g->obj[i].re = 0.6;
	g->obj[i].trans = 0;
	g->obj[i].soft = 0;

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
	g->obj[i].tile[0].data_ptr = NULL;



	init_tile(0,"./tiles/blank.xpm", g->obj[i].tile, g);
	g->obj[i].tile[0].w2 = g->obj[i].tile[0].w / 2;
	g->obj[i].tile[0].h2 = g->obj[i].tile[0].h / 2;

}

void		rotate_normal_map_sphere(t_tile *normalmap)
{
	t_vector angle;
	init_vector(&angle, 0, 0, 0);
	int x = 0;
	int y = 0;

	while (y < normalmap->h)
	{
		x = 0;
		while (x < normalmap->w)
		{
			angle.x = y / (double)(normalmap->h * M_1_PI);
			angle.y = x / (double)(normalmap->w2 * M_1_PI);

			*(normalmap->vectile + y * normalmap->w + x) = 
			rotate(*(normalmap->vectile + y * normalmap->w + x), angle);
			x++;
		}
		y++;
	}
}

void		init_sphere(t_vector *ctr, int i, t_global *g)
{
	g->obj[i].name = sphere/*0*/;
	g->obj[i].id = i;
	g->obj[i].hit = &hit_sphere;
	g->obj[i].bright = &bright_sphere;
	g->obj[i].simple_bright = &simple_bright_sphere;
	g->obj[i].get_normal = &get_normal_sphere;


	g->obj[i].ctr = &ctr[i];
	printf("center %p\n", g->obj[i].ctr);
	g->obj[i].trans = 0;
	g->obj[i].re = 0;
	g->obj[i].spec = 4;
	g->obj[i].soft = 0;

	g->obj[i].prop[0] = do_tile_sphere;
	g->obj[i].prop[1] = do_re;
	g->obj[i].prop[2] = do_trans;

//(t_vector *)do_tile_sphere(t_vector, t_object *, t_global *);

	init_vector(g->obj[i].ctr, 0, -100, 200);
	printf("center is %f\n", g->obj[i].ctr->z);
	g->obj[i].rd = 100;
	g->obj[i].rd2 = g->obj[i].rd * g->obj[i].rd;
	g->obj[i].rd_1 = 1 / (double)g->obj[i].rd;
	g->obj[i].color = rgb(0x010100);
	init_vector(&g->obj[i].ang, 0, 0, 0);
	init_vector(&g->obj[i].base[0], 1, 0, 0);
	init_vector(&g->obj[i].base[1], 0, 1, 0);
	init_vector(&g->obj[i].base[2], 0, 0, 1);
	g->obj[i].tile[0].data_ptr = NULL;

//	printf("init tile\n");
//	init_tile(0,"./tiles/jupiter.xpm", g->obj[i].tile, g);
//	init_tile(0,"./tiles/basecolor.xpm", g->obj[i].tile, g);
//	init_tile(0,"./tiles/blank.xpm", g->obj[i].tile, g);


//	init_tile(0,"./tiles/z1.xpm", g->obj[i].tile, g);	
	g->obj[i].normal_map.data_ptr = NULL;
//	init_tile(1,"./tiles/basecolornormal.xpm", &g->obj[i].normal_map, g);
//	init_tile(1,"./tiles/z1.xpm", &g->obj[i].normal_map, g);
	printf("init normal map\n");
//	init_tile(1,"./tiles/normalmap.xpm", &g->obj[i].normal_map, g);
	if (g->obj[i].normal_map.data_ptr)
		rotate_normal_map_sphere(&g->obj[i].normal_map);
}

void		init_cone(t_vector *ctr, int i, t_global *g)
{
	g->obj[i].name = cone;
	g->obj[i].id = i;
	g->obj[i].hit = &hit_cone;
	g->obj[i].bright = &bright_cone;
	g->obj[i].simple_bright = &simple_bright_cone;
	g->obj[i].get_normal = &get_normal_cone;

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
	g->obj[i].re = 0;
	g->obj[i].spec = 5;
	g->obj[i].trans = 0;
	g->obj[i].soft = 1;
	init_tile(i,"./tiles/blank.xpm", g->obj[i].tile, g);

//	g->obj[i].tile[0].data_ptr = NULL;
	
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
}
