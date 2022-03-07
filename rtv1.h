/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtv1.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpokalch <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/03 12:37:33 by tpokalch          #+#    #+#             */
/*   Updated: 2019/09/30 06:25:27 by tpokalch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <mlx.h>
#include <stdio.h>
#include <stdlib.h>
#include "libft/libft.h"
#include <pthread.h>
#include <fcntl.h>
#include <time.h>
/*
#define WIDTH 800
#define HEIGHT 400
#define HEIGHT_2 200
#define WIDTH_2 400

*/
#define WIDTH 400
#define HEIGHT 200
#define HEIGHT_2 100
#define WIDTH_2 200

#define TASK 20
#define STRIPS HEIGHT / TASK
#define CORES 4
#define M_T 6.28318530718
#define MAX_REC 4
#define RECORD_VIDEO 0

// these are keycodes for the project onmy old mac,
// i have to rewrite them for my 2017 mac

/*
#define A_KEY 0
#define S_KEY 1
#define D_KEY 2
#define F_KEY 3
#define W_KEY 13
#define SPACE_KEY 49
#define ESC_KEY 53
#define J_KEY 38
#define K_KEY 40
#define L_KEY 37
#define I_KEY 34
#define O_KEY 31
#define BRA_KEY 43
#define KET_KEY 47
#define UP_KEY 126
#define DOWN_KEY 125
#define LEFT_KEY 123
#define RIGHT_KEY 124
#define H_KEY 4
#define G_KEY 5
#define R_KEY 15
#define T_KEY 17
#define Y_KEY 16

*/

#define A_KEY 97 
#define S_KEY 115
#define D_KEY 100
#define F_KEY 102
#define W_KEY 119
#define SPACE_KEY 32
#define ESC_KEY 65307
#define J_KEY 106
#define K_KEY 107
#define L_KEY 108
#define I_KEY 105
#define O_KEY 111
#define BRA_KEY 44
#define KET_KEY 46
#define UP_KEY 65362
#define DOWN_KEY 65364
#define LEFT_KEY 65361
#define RIGHT_KEY 65363
#define H_KEY 104
#define G_KEY 103
#define R_KEY 114
#define T_KEY 116
#define Y_KEY 121



















typedef enum e_name
{
	plane,		// 0
	sphere,		// 1
	cylinder,	// 2
	cone,		// 3
	complex,	// 4
	tri,		// 5
	nothing		// 6
}	t_name;


typedef	struct	s_vector t_vector;

struct s_vector
{
	double x;
	double y;
	double z;
//	int	len;
};


// global varible for debugging
t_vector		shot;
//int			mousex;



typedef struct		s_global t_global;
typedef	struct		s_object t_object;
typedef	struct		s_objecthit t_objecthit;
typedef struct		s_dstpst	t_dstpst;

typedef	struct		s_colbri
{
	t_vector col;
	int	bri;
}			t_colbri;


int				brg(t_vector rgb);
int				inside_cone(t_vector p, t_object o, t_global *g);
double				dot(t_vector a, t_vector b);
t_vector			diff(t_vector a, t_vector b);
t_vector			sum(t_vector a, t_vector b);
t_vector			norm(t_vector a);
int					color(int b, t_vector c);
t_vector			scale(double a, t_vector b);
void				ginit(t_global *g);
void				init_plane(t_vector *ctr, int i, t_global *g);
void				init_cylinder(t_vector *ctr, int i, t_global *g);
void				init_sphere(t_vector *ctr, int i, t_global *g);
void				init_spheror(t_vector *ctr, int i, t_global *g);
void				init_cone(t_vector *ctr, int i, t_global *g);
void				init_tri(t_vector *ctr, int i, t_global *g);
void				init_complex(t_vector *ctr, int i, t_global *g);


int					check_arg(char **argv, int argc, t_global *g, t_vector *ctr);
int					usage(void);
int					arg_valid(char **argv);
int					fill_objects(t_vector *ctr, char **argv, t_global *g);
void				fill_obj(char **argv, int n, t_global *g);
void				vectorify(void *obj_coord, char **argv);
int					is_coords(char *argv);
int					putstr(char *s, int ret);
void				free_arr(char **arr);
int					init_objects(t_vector *ctr, char **argv, t_global *g);
int					obj_traver(char **argv, char *c);
void				init_rays(t_vector ****ray);
void				init_hits(t_objecthit ***hits);
int					key_press(int kk, void *param);
void				copy_tcps(t_global *g);
void				copy(t_global *tcps, t_global *g);
int					move_obj(int kk, t_global *g);
int					move_phys(int keycode, t_global *g);
void				move_cam(char s, t_global *g);
int				campos(t_global *g);
int					free_hits(t_global *g);
int					start_threads(void *f, t_global *g);
int					mouse_move(int x, int y, void *param);
void				debug(t_global *g);
void				*recalc(void *p);
void				*move(void *p);
void				*toimg(void *tcp);

void				objecthit(t_dstpst *ret, t_vector st, t_vector end,  t_object *obj, int objc, t_global *g);

t_dstpst			hit_plane(t_vector st, t_vector end,  t_vector ray, t_object obj, t_global *g);
t_dstpst			hit_sphere(t_vector st, t_vector end,  t_vector ray, t_object obj, t_global *g);
t_dstpst			hit_cylinder(t_vector st, t_vector end,  t_vector ray, t_object obj, t_global *g);
t_dstpst			hit_cone(t_vector st, t_vector end,  t_vector ray, t_object obj, t_global *g);
t_dstpst			hit_tri(t_vector st, t_vector end,  t_vector ray, t_object obj, t_global *g);
t_dstpst			hit_complex(t_vector st, t_vector end,  t_vector ray, t_object obj, t_global *g);

t_colbri				simple_bright_plane(t_vector st, t_vector hit, t_object *obj, t_global *g);
t_colbri				bright_plane(t_vector st, t_vector hit, t_object *obj, t_global *g);
t_colbri					bright_sphere(t_vector st, t_vector hit, t_object *obj, t_global *g);
t_colbri					simple_bright_sphere(t_vector st, t_vector hit, t_object *obj, t_global *g);

t_colbri					bright_cylinder(t_vector st, t_vector hit, t_object *obj, t_global *g);
t_colbri					simple_bright_cylinder(t_vector st, t_vector hit, t_object *obj, t_global *g);
t_colbri					bright_cone(t_vector st, t_vector hit, t_object *obj, t_global *g);
t_colbri					simple_bright_cone(t_vector st, t_vector hit, t_object *obj, t_global *g);
t_colbri					bright_tri(t_vector st, t_vector hit, t_object *obj, t_global *g);
t_colbri					simple_bright_tri(t_vector st, t_vector hit, t_object *obj, t_global *g);


t_vector			get_normal_sphere(t_vector point, t_object *obj);
t_vector			get_normal_plane(t_vector point, t_object *obj);
t_vector			get_normal_cyl(t_vector point, t_object *obj);
t_vector			get_normal_cone(t_vector point, t_object *obj);




void				alias(int *dst, int *a, int w, int h, int xmax, int ymax);

t_dstpst			*NANI(t_dstpst *t);
void					obstructed(t_colbri *i, t_vector hit, t_vector *hitli, t_vector reflrayv, t_object obj, t_global *g);
t_vector			rotate(t_vector ray, t_vector angle);
void				init_vector(t_vector *current, double x, double y, double z);
int				con(t_global *g);
t_vector			rgb(int c);
t_vector			base(t_vector c);
int				pinside(t_vector p, t_vector bd1, t_vector bd2, t_vector bd3, t_vector nr, t_global *g);
void				screen(int *a, int w, int h, t_global *g);

t_vector			cross(t_vector a, t_vector b);
double				det(t_vector a, t_vector b);
void				stretch(int *a, int d, int h);
void				smooth(int *a, int w, int h, int xmax, int ymax, t_global *g);
void				save_im(int *a, int *b, int w, int h);
t_vector			base255(t_vector);
void				white(int *a, int w, int h, int c);
t_vector			**initialize_points(int height);
t_vector			**create_points(char *filename, t_vector *ptdim, t_global *g);
void				free_points(t_vector **pts);
double				mymod(double x, int m);
double				myacos(t_vector ax, t_vector v, t_vector nrm, t_global *g);
int				myintmod(int x, int m);
int				left(t_vector a, t_vector b, t_vector nrm, t_global *g);
double				tothe2(double x, int e);

void		do_tile_sphere(t_vector hit, t_object *obj, t_global *g);
void		do_re(t_vector refl, t_vector hit, t_vector *retcol, t_object obj, t_global *g);
void		do_trans(t_vector st, t_vector hit, t_colbri *ret, t_object obj, t_global *g);


void		do_spec(t_colbri *ret, t_vector hit, t_vector nrm, t_vector reflrayv, t_object obj, t_global *g);

void		do_1_spec(t_colbri *tmp, t_colbri *ret, t_vector *hitli, t_vector reflrayv, t_object obj, int i, t_global *g);
double		len2(t_vector a);

typedef	struct		s_tile
{
	int bpp;
	int sz_l;
	int e;
	int w;
	int h;
	int w2;
	int h2;
	int mipq;
	int *data_ptr;
	t_vector *vectile;
	void *ptr;
}			t_tile;

typedef struct		s_object
{
//	char			*name;
	t_name			name;
	int				id;
	int				cam_pos;
	t_dstpst		(*hit)(t_vector, t_vector, t_vector, t_object, t_global *g);
	t_colbri				(*bright)(t_vector, t_vector, t_object *, struct s_global *);
	t_colbri				(*simple_bright)(t_vector, t_vector, t_object *, struct s_global *);
	t_vector				(*get_normal)(t_vector, t_object *);


	t_vector		bd1;
	t_vector		bd2;
	t_vector		bd3;
	t_vector		base[3];
	t_vector		nr;
	t_vector		real_nr;
	t_tile			tile[15];
	t_tile			normal_map;
	t_vector		*ctr;
	t_vector		ang;
	t_vector		color;
	int				rd;
	int				rd2;
	double				rd_1; // 1 / rd

	void			(*prop[3])();
	t_vector		**pts;
	t_object		*tris;
	double			re;
	double			trans;
	int			spec;
	int			soft;
	t_vector		ptdim;
	t_object		*frame;
	t_vector		box[8];
}		t_object;

struct		s_dstpst
{
	double	dst;
	double	pst;
	t_object obj;
};

typedef	struct		s_objecthit
{
	t_object obj;
	t_vector hit;
}				t_objecthit;

typedef struct		s_global
{
	void 			*mlx_ptr;
	void			*win_ptr;
	void			*img_ptr;
	int				*data_ptr;
	int				bpp;
	int				sz_l;
	int				e;
	int				light_switch;
	t_dstpst		cone[2];
	t_vector		_0015;
	t_vector		_1500;
	t_vector		white;
	t_vector		base[3];
	t_vector		*ray;
	t_vector		*li;
	t_vector		*cam_pos;
	double			*liz;
	t_vector		*angle;
	t_vector		*normal;
	t_vector		*right;

	t_object		*obj;
	t_object		*all_obj;
	t_objecthit		***hits;
	t_vector		***rays;
	int				objn;
	int				argc;
	pthread_t		tid[CORES];
	int				core;
	int				prim;
	int				ambient;
	int				my_line;
//	int				*line_taken;//size of number of tasks
	int				mip_map;
	pthread_mutex_t			mutex;
	int				lights;
	t_vector			spec_con;
//	t_vector			*hitli;
//	t_vector			*savehitli;
	t_vector			prev;
	double				*cosa;
	t_vector			*ctrli;
	int					*recursion;
	t_global		*tcps[CORES];
}				t_global;
