/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtv1.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpokalch <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/03 12:37:33 by tpokalch          #+#    #+#             */
/*   Updated: 2019/07/10 20:44:48 by tpokalch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RTV1_H
# define RTV1_H
# include <math.h>
# include <mlx.h>
# include "libft/libft.h"
# include <pthread.h>

# define WIDTH 400
# define HEIGHT 400
# define CORES 1

typedef	struct			s_vector
{
	double x;
	double y;
	double z;
}						t_vector;

typedef	struct			s_dstpst
{
	double	dst;
	double	pst;
}						t_dstpst;

typedef struct			s_global
{
	void				*mlx_ptr;
	void				*win_ptr;
	void				*img_ptr;
	int					*data_ptr;
	int					bpp;
	int					sz_l;
	int					e;
	int					light_switch;
	t_dstpst			cone[2];
	t_vector			o01;
	t_vector			o10;
	t_vector			*ray;
	t_vector			*li;
	t_vector			*cam_pos;
	double				liz;
	t_vector			*angle;
	t_vector			*normal;
	struct s_object		*obj;
	struct s_objecthit	***hits;
	t_vector			***rays;
	int					objn;
	int					argc;
	pthread_t			tid[CORES];
	int					core;
	int					prim;
	struct s_global		*tcps[CORES];
}						t_global;

typedef	struct			s_object
{
	char				*name;
	int					id;
	t_vector			hitnow;
	int					cam_pos;
	t_dstpst			(*hit)(t_vector, t_vector, struct s_object,
						t_global *g);
	int					(*bright)(t_vector, struct s_object, t_global *);
	t_vector			nr;
	t_vector			*ctr;
	t_vector			ang;
	int					color;
	int					rd;
	int					rd2;
}						t_object;

typedef	struct			s_objecthit
{
	t_object			obj;
	t_vector			hit;
}						t_objecthit;

t_vector				shot;
int						inside_cone(t_vector p, t_object o);
double					dot(t_vector a, t_vector b);
t_vector				diff(t_vector a, t_vector b);
t_vector				sum(t_vector a, t_vector b);
t_vector				norm(t_vector a);
int						color(int b, int c);
t_vector				scale(double a, t_vector b);
void					ginit(t_global *g);
void					init_plane(t_vector *ctr, int i, t_global *g);
void					init_cylinder(t_vector *ctr, int i, t_global *g);
void					init_sphere(t_vector *ctr, int i, t_global *g);
void					init_cone(t_vector *ctr, int i, t_global *g);
int						check_arg(char **argv, int argc, t_global *g,
						t_vector *ctr);
int						usage(void);
int						arg_valid(char **argv);
int						fill_objects(t_vector *ctr, char **argv, t_global *g);
void					fill_obj(char **argv, int n, t_global *g);
void					vectorify(void *obj_coord, char **argv);
int						is_coords(char *argv);
int						putstr(char *s, int ret);
void					free_arr(char **arr);
int						init_objects(t_vector *ctr, char **argv, t_global *g);
int						obj_traver(char **argv, char *c);
void					init_rays(t_vector ****ray);
void					init_hits(t_objecthit ***hits);
int						key_press(int kk, void *param);
void					copy_tcps(t_global *g);
void					copy(t_global *tcps, t_global *g);
int						move_obj(int kk, t_global *g);
int						move_phys(int keycode, t_global *g);
void					move_cam(char s, t_global *g);
int						free_hits(t_global *g);
int						start_threads(void *f, t_global *g);
int						mouse_move(int x, int y, void *param);
void					debug(t_global *g);
void					*recalc(void *p);
void					*move(void *p);
void					*toimg(void *tcp);
void					objecthit(t_objecthit *ret, t_vector ray, t_global *g);
int						bright_plane(t_vector hit, t_object obj,
						t_global *g);
t_dstpst				hit_plane(t_vector st, t_vector end, t_object obj,
						t_global *g);
int						bright_sphere(t_vector hit, t_object obj, t_global *g);
t_dstpst				hit_sphere(t_vector st, t_vector end, t_object obj,
						t_global *g);
t_dstpst				hit_cylinder(t_vector st, t_vector end, t_object obj,
						t_global *g);
int						bright_cylinder(t_vector hit, t_object obj,
						t_global *g);
int						bright_cone(t_vector hit, t_object obj, t_global *g);
t_dstpst				hit_cone(t_vector st, t_vector end, t_object obj,
						t_global *g);
t_dstpst				*nani(t_dstpst *t);
int						obstructed(t_vector hit, t_object obj, t_global *g);
t_vector				rotate(t_vector ray, t_vector angle);
void					init_vector(t_vector *i, double x, double y, double z);
int						con(t_global *g);
#endif
