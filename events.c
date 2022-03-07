/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpokalch <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/03 16:10:51 by tpokalch          #+#    #+#             */
/*   Updated: 2019/07/17 21:25:14 by tpokalch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

//	this function is needed to check for the direction of the plane/tri
//	normals only once after the movement of the object, or movement of
//	the camera
int		campos(t_global *g)
{
	int i;

	i = 1;
	while (i < g->argc + 1)
	{
		printf("argc = %d\n", g->argc);
		if (g->obj[i].name == complex)
			printf("rd is %d\n", g->obj[i].rd);
		printf("i is %d\n", i);
		if ((g->obj[i].name == plane || g->obj[i].name == tri/*no tri in the object list*/)  && dot(diff(*g->obj[i].ctr, *g->cam_pos), g->obj[i].base[1]) > 0)
		{
/*			if (g->obj[i].normal_map.data_ptr)
			{
				for (int i = 0; i < g->obj[i].normal_map.w * g->obj[i].normal_map.h; i++)
				{
					printf("changing normal\n");
					g->obj[i].normal_map.vectile[i] = scale(-1, g->obj[i].normal_map.vectile[i]);
				}
			}
*/
//			g->obj[i].base[1] = scale(-1, g->obj[i].base[1]);
//			g->obj[i].nr = scale(-1, g->obj[i].nr);
			
//					1==inside
			g->obj[i].cam_pos = 1;
			printf("we are in %d %d enum\n", i, g->obj[i].name);
		}
		else
			g->obj[i].cam_pos = 0;
		i++;
	}
	return (1);
}

extern int fd;

int		key_press(int kk, void *param)
{
	t_global *g;

	g = param;
	printf("keycode is %d\n", kk);
//	if (kk == H_KEY7)
//	comment this to keep shot constant
//	uncomment to keep shot only 1 frame
//		shot.x = -WIDTH;
//	ft_bzero((int *)g->data_ptr, g->sz_l * HEIGHT);
	if (kk == ESC_KEY)
	{//	system("leaks -s rtv1");
//		system("leaks -s rtv1");	
		printf("freeing\n");
		if (RECORD_VIDEO)
		{
			close(fd);
			printf("closing file\n");
		}
		exit(free_hits(g));
	}
	else if (kk == W_KEY || kk == S_KEY || kk == A_KEY || kk == D_KEY || kk == F_KEY || kk == DOWN_KEY
		|| kk == UP_KEY || kk == J_KEY || kk == K_KEY || kk == L_KEY || kk == I_KEY
		|| kk == LEFT_KEY || kk == RIGHT_KEY)
		return(move_obj(kk, g));
	else if (kk == Y_KEY)
		g->objn = (g->objn + 1) % (g->argc + 1);
	else if (kk == H_KEY || kk == G_KEY) //camera perspective
	{
		g->ray->z = fabs(g->ray->z + 10 * (2 * (kk == H_KEY) - 1));
		copy_tcps(g);
		return (start_threads(recalc, g));
	}
	else if ((kk == R_KEY || kk == T_KEY) && g->light_switch > 0 && g->light_switch <= g->lights)
	{
		g->liz[g->light_switch - 1] += 15 * (2 * (kk == R_KEY) - 1);

//		*g->li = sum(*g->li, scale((2 * (kk == S_KEY) - 1), *g->normal));
//		g->liz = g->li->z;
//		return (start_threads(recalc, g));	
	}
	else if (kk == SPACE_KEY)
	{
		g->light_switch = (g->light_switch + 1) % (2 + g->lights);
		return (1);
	}
	return (start_threads(toimg, g));
}

int		move_phys(int keycode, t_global *g)
{
	if (keycode == S_KEY || keycode == W_KEY)
		g->obj[g->objn].ctr->z += 5 * (2 * (keycode == W_KEY) - 1);
	else if	(keycode == UP_KEY || keycode == DOWN_KEY)
		g->obj[g->objn].ctr->y += 5 * (2 * (keycode == UP_KEY) - 1);
	else if	(keycode == A_KEY)
		g->obj[g->objn].ctr->x += -5;
	else if	(keycode == D_KEY)
		g->obj[g->objn].ctr->x += 5;
	else if (keycode == I_KEY)
		g->obj[g->objn].ang.x -= 0.05;
	else if (keycode == K_KEY)
		g->obj[g->objn].ang.x += 0.05;
	else if (keycode == J_KEY)
		g->obj[g->objn].ang.y -= 0.05;
	else if (keycode == L_KEY)
		g->obj[g->objn].ang.y += 0.05;
	else if (keycode == BRA_KEY || keycode == S_KEY)
		g->obj[g->objn].ang.z -= 0.05;
	else if (keycode == KET_KEY || keycode == S_KEY)
		g->obj[g->objn].ang.z += 0.05;
	if (keycode == I_KEY || keycode == K_KEY || keycode == J_KEY || keycode == L_KEY
	|| keycode == BRA_KEY || keycode == KET_KEY || keycode == S_KEY || keycode == S_KEY)
	{
		g->obj[g->objn].base[0] = rotate(g->base[0], g->obj[g->objn].ang);
		g->obj[g->objn].base[1] = rotate(g->base[1], g->obj[g->objn].ang);
		g->obj[g->objn].base[2] = rotate(g->base[2], g->obj[g->objn].ang);
//		printf("base is %f,%f,%f\n", g->obj[g->objn].base[0].x, g->obj[g->objn].base[1].y, g->obj[g->objn].base[2].z);
	}
	campos(g);	
	return (start_threads(move, g));
}

int	move_obj(int kk, t_global *g)
{
	if (g->objn != 0) // if something else than screen
		return (move_phys(kk, g));
	if (kk == UP_KEY || kk == DOWN_KEY || kk == J_KEY || kk == L_KEY
		|| kk == A_KEY || kk == D_KEY || kk == RIGHT_KEY || kk == LEFT_KEY)
	{
		if (kk == DOWN_KEY)
			g->angle->x += 0.05;
		else if (kk == UP_KEY)
			g->angle->x -= 0.05;
		else if (kk == J_KEY)
			g->angle->z -= 0.05;
		else if (kk == L_KEY)
			g->angle->z += 0.05;
		else if (kk == A_KEY || kk == LEFT_KEY)
			g->angle->y -= 0.05;
		else if (kk == D_KEY || kk == RIGHT_KEY)
			g->angle->y += 0.05;
		*g->normal = rotate(g->_0015, *g->angle);
		campos(g);
		return (start_threads(recalc, g));
	}
	else if (kk == S_KEY)
		*g->cam_pos = diff(*g->cam_pos, *g->normal);
	else if (kk == W_KEY)
		*g->cam_pos = sum(*g->cam_pos, *g->normal);
	campos(g);
	return (start_threads(move, g));
}

//		not in use
void		move_cam(char s, t_global *g)
{
	int i;

	i = 0;
	while (i < g->argc)
	{
		if (s == '+')
			*g->obj[i].ctr = sum(*g->obj[i].ctr, *g->normal);
		else
			*g->obj[i].ctr = diff(*g->obj[i].ctr, *g->normal);
		i++;
	}
}

int		mouse_move(int x, int y, void *param)
{
	t_global *g;
	int	i;
	t_vector p;
	double block;

	i = -1;
	g = param;
//	mousex = x; // probably delete later
//	printf("mouse is at %d, %d\n", x, y);
//	printf("shit is %f,%f\n", shot.x, shot.y);
	if (g->light_switch >= 1 && g->light_switch <= g->lights) //move light
	{
//			printf("hello\n");
//			ft_bzero((int *)g->data_ptr, g->sz_l * HEIGHT);
			block = g->liz[g->light_switch - 1] / g->ray->z;
			p.x = (x - WIDTH_2) * block;
			p.y = (HEIGHT_2 - y) * block;
			p.z = g->liz[g->light_switch - 1];
			g->li[g->light_switch - 1] = sum(*g->cam_pos, rotate(p, *g->angle));
			start_threads(toimg, g);
	}
	else if (g->light_switch > g->lights) //move cam
	{
//		ft_bzero((int *)g->data_ptr, g->sz_l * HEIGHT);
		p.y = /*sin*/((x - WIDTH_2)* 0.0045);
		p.x = /*sin*/(0.0045 * (y - HEIGHT_2));
		p.z = 0;
//		*g->normal = rotate(g->_0015, p);
//		*g->angle = sum(p, *g->angle);
		*g->angle = p;

//		ideally normal changes only when the key is pressed
//		and doesn't change untill mouse_move->key_press
//		muse move should set int can_change to 1
//		key press should change normal and
//		set int can change to 0

		*g->normal = rotate(g->_0015, p);	
		*g->right = rotate(g->_1500, p);
		start_threads(recalc, g);
	}
	return (1);
}

