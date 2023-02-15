
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
//	the point is obstructed. // fixed?
//
//	light ambient at distance // fixed?

int			con(t_global *g)
{
	return (0);
	//i forgot how to make it stop((
	return (shot.x == g->ray->x && shot.y == g->ray->y);
}

// moved to events
/*
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
*/

void	integrator(t_global *g)
{
	static t_vector s1;
	static t_vector s2;
	static t_vector s3;

	double dt = 0.001;

	double m1 = g->obj[2].rd ;
	double m2 = g->obj[3].rd ;
	double m3 = g->obj[4].rd ;


	double G = 0.01;


	static int count = 0;


	if (count == 0)
	{

		init_vector(&s1, 600, 0, 1800);
		init_vector(&s2, 0, 0, -m1 * 1800 / m2);
		init_vector(&s3, -m1 * 600 / m3, 0, 0);



/*		init_vector(&s1, 0, 0, 0);
		init_vector(&s2, 0, 0, 0);
		init_vector(&s3, 0, 0, 0);
*/
	}
//	s2 = scale(0.99, s2);
	count++;	

	t_vector *p1 = (g->obj[2].ctr);
	t_vector *p2 = (g->obj[3].ctr);
	t_vector *p3 = (g->obj[4].ctr);

	t_vector v = diff(*p2, *p1);
	double r12 = sqrt(dot(v, v));

	t_vector h = diff(*p3, *p1);
	double r13 = sqrt(dot(h, h));

	t_vector d = diff(*p3, *p2);
	double r23 = sqrt(dot(d, d));



	t_vector force12;
	t_vector force13;
	t_vector force23;




	if (r12 < 0.01)
		init_vector(&force12, 0, 0, 0);
	else
		force12 = scale(G * m1 * m2 / r12 * r12 * r12, v);

	if (r13 < 0.01)
		init_vector(&force13, 0, 0, 0);
	else
		force13 = scale(G * m1 * m2 / r13 * r13 * r13, h);

	if (r23 < 0.01)
		init_vector(&force23, 0, 0, 0);
	else	
		force23 = scale(G * m1 * m2 / r23 * r23 * r23, d);


	t_vector a1 = scale( 1/m1 , sum(force12, force13));

	t_vector a2 = scale( 1/m2, sum(scale(-1, force12), force23));

	t_vector a3 = scale( 1/m3, sum(scale(-1, force13), scale(-1, force23)));

	s1 = sum(scale( dt, a1), s1);
	s2 = sum(scale( dt, a2), s2);
	s3 = sum(scale( dt, a3), s3);



	t_vector newp1 = sum(*p1, scale(dt, s1)); 
	t_vector newp2 = sum(*p2, scale(dt, s2)); 
	t_vector newp3 = sum(*p3, scale(dt, s3)); 


	v = diff(newp1, newp2);
	h =  diff(newp3, newp1);
	d =  diff(newp3, newp2);


	r12 = sqrt(dot(v, v));
	double r31 = sqrt(dot(h, h));
	double r32 = sqrt(dot(d, d));
//	collision

//#if 0


////////////////////// r12 number 1

	if (r12 -  (g->obj[2].rd + g->obj[3].rd) < 0.1)
	{
		printf("____________collision\n");

/*		t_vector temp;
		temp = s1;
		s1 = s2;
		s2 = temp;
*/
		t_vector rad21 = norm(diff(*(g->obj[2].ctr), *(g->obj[3].ctr)));
		double speed2 = dot(s2, rad21);


		t_vector rad12 = scale(1, rad21);
		double speed1 = dot(s1, rad12);


//		printf("speed1 projection is %f\n", speed1);
//		printf("speed2 projection is %f\n", speed2);


		double newspeed2 = ((m2 - m1) *speed2 + m1 * 2 * speed1) / (m1 + m2);
		printf("newspeed2 is %f\n", newspeed2);

		s2 = sum(diff(s2, scale(speed2, rad21)), scale(newspeed2, rad21));


		double newspeed1 = ((m1 - m2) *speed1 + m2 * 2 * speed2) / (m1 + m2);
		printf("newspeed1 is %f\n", newspeed1);


		s1 = sum(diff(s1, scale(speed1, rad12)), scale(newspeed1, rad12));


		t_vector newp1 = sum(*p1, scale(dt, s1)); 
		t_vector newp2 = sum(*p2, scale(dt, s2)); 

			v = diff(newp1, newp2);
			r12 = sqrt(dot(v, v));

		while (r12 -  (g->obj[2].rd + g->obj[3].rd) < 1)
		{
			printf("closeness12 %f\n", r12 -  (g->obj[2].rd + g->obj[3].rd));
			t_vector getout2 = norm(diff(newp2, newp1));
			newp1 = sum(newp1, scale(0.1, scale(-1, getout2)));
			newp2 = sum(newp2, scale(0.1,  getout2)); 
			v = diff(newp1, newp2);
			r12 = sqrt(dot(v, v));
			printf("newp1 is %f,%f,%f\n", newp1.x,newp1.y,newp1.z);
			printf("getout2 is  %f,%f,%f\n", getout2.x,getout2.y,getout2.z);
		}
	{
		*p1 = newp1;
		*p2 = newp2;
	}


	}
	else
//#endif
	{
		*p1 = newp1;
 		*p2 = newp2;
//		*p3 = newp3;
	}

/////////////////////////// r32 number 3
//#if 0
	if (r32 -  (g->obj[4].rd + g->obj[3].rd) < 0.1)
	{
		printf("collision\n");

/*		t_vector temp;
		temp = s1;
		s1 = s2;
		s2 = temp;
*/
		t_vector rad23 = norm(diff(*(g->obj[4].ctr), *(g->obj[3].ctr)));
		double speed2 = dot(s2, rad23);


		t_vector rad32 = scale(1, rad23);
		double speed3 = dot(s3, rad32);


//		printf("speed1 projection is %f\n", speed3);
//		printf("speed2 projection is %f\n", speed2);


		double newspeed2 = ((m2 - m3) *speed2 + m3 * 2 * speed3) / (m3 + m2);
		printf("newspeed2 is %f\n", newspeed2);

		s2 = sum(diff(s2, scale(speed2, rad23)), scale(newspeed2, rad23));


		double newspeed3 = ((m3 - m2) *speed3 + m2 * 2 * speed2) / (m3 + m2);
		printf("newspeed1 is %f\n", newspeed3);


		s3 = sum(diff(s3, scale(speed3, rad32)), scale(newspeed3, rad32));


		t_vector newp3 = sum(*p3, scale(dt, s3)); 
		t_vector newp2 = sum(*p2, scale(dt, s2)); 

			h = diff(newp3, newp2);
			r32 = sqrt(dot(h, h));

		while (r32 -  (g->obj[4].rd + g->obj[3].rd) < 1)
		{
			printf("closeness32 %f\n", r32 -  (g->obj[4].rd + g->obj[3].rd));
			t_vector getout2 = norm(diff(newp2, newp3));
			newp3 = sum(newp3, scale(0.1, scale(-1, getout2)));
			newp2 = sum(newp2, scale(0.1,  getout2)); 
			v = diff(newp2, newp3);
			r32 = sqrt(dot(v, v));
			printf("newp1 is %f,%f,%f\n", newp3.x,newp3.y,newp3.z);
			printf("getout2 is  %f,%f,%f\n", getout2.x,getout2.y,getout2.z);
//			s2 = scale(-1, s2);
//			s3 = scale(-1, s3);


		}
		*p2 = newp2;
		*p3 = newp3;


	}
	else
//#endif
	{
//		*p1 = newp1;
 		*p2 = newp2;
		*p3 = newp3;
	}



//////////////////////////////////////////// r31 number 2

//#if 0
	if (r31 -  (g->obj[4].rd + g->obj[2].rd) < 0.1)
	{
		printf("collision\n");

/*		t_vector temp;
		temp = s1;
		s1 = s2;
		s2 = temp;
*/
		t_vector rad13 = norm(diff(*(g->obj[2].ctr), *(g->obj[4].ctr)));
		double speed1 = dot(s1, rad13);


		t_vector rad31 = scale(1, rad13);
		double speed3 = dot(s3, rad31);


//		printf("speed1 projection is %f\n", speed3);
//		printf("speed2 projection is %f\n", speed2);


		double newspeed1 = ((m1 - m3) *speed1 + m3 * 2 * speed3) / (m3 + m1);
		printf("newspeed2 is %f\n", newspeed1);

		s1 = sum(diff(s1, scale(speed1, rad13)), scale(newspeed1, rad13));


		double newspeed3 = ((m3 - m1) *speed3 + m1 * 2 * speed1) / (m3 + m1);
		printf("newspeed1 is %f\n", newspeed3);


		s3 = sum(diff(s3, scale(speed3, rad31)), scale(newspeed3, rad31));


		t_vector newp3 = sum(*p3, scale(dt, s3)); 
		t_vector newp1 = sum(*p1, scale(dt, s1)); 

			h = diff(newp3, newp1);
			r31 = sqrt(dot(h, h));

		while (r31 -  (g->obj[4].rd + g->obj[2].rd) < 1)
		{
			printf("closeness31 %f\n", r31 -  (g->obj[4].rd + g->obj[2].rd));
			t_vector getout1 = norm(diff(newp1, newp3));
			newp3 = sum(newp3, scale(0.1, scale(-1, getout1)));
			newp1 = sum(newp1, scale(0.1,  getout1)); 
			v = diff(newp1, newp3);
			r31 = sqrt(dot(v, v));
			printf("newp1 is %f,%f,%f\n", newp3.x,newp3.y,newp3.z);
			printf("getout1 is  %f,%f,%f\n", getout1.x,getout1.y,getout1.z);
		}
		*p1 = newp1;
		*p3 = newp3;
	}
	else
//#endif
	{
		*p1 = newp1;
// 		*p2 = newp2;
		*p3 = newp3;
	}

//#endif





//	printf("speed 1 is %f,%f,%f\n", s1.x, s1.y, s1.z);
//	printf("speed 2 is %f,%f,%f\n", s2.x, s2.y, s2.z);
//#endif

}


void	update_camera(t_global *g)
{
	static float t = 0;
	t+=0.01;
	float w = 1;
	float R = -sqrt(g->cam_pos->x * g->cam_pos->x + g->cam_pos->z * g->cam_pos->z);
	float newz = R * cos(w * t);
	float newx = R * sin(w * t);

	g->angle->y = w * t;
//	*g->normal = rotate(g->_0015, *g->angle);

	g->cam_pos->z = newz;
	g->cam_pos->x = newx;


}

int	loop(void *p)
{
//	printf("start loop function callback\n");
	t_global *g = (t_global *)p;

//	the light moves lefr to righ and the camera moves back
//	g->li->x += 5;
//	*g->cam_pos = diff(*g->cam_pos, scale(0.1, *g->normal));
	update_camera(g);
//	printf("integtator call\n");
	integrator(g);
	
	printf("______loop starting threads\n");
	start_threads(recalc, g);
	return (1);
}

void		free_arr(char **arr)
{
	int i;

	i = 0;
	while (*(arr + i) != NULL)
	{
//		printf("now freeing %s\n", *(arr + i));
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
//	t_vector *ctr = (t_vector *)malloc(sizeof(t_vector) * argc);
//	t_vector ctr[argc];
	t_vector kenobi[6];
	int h;
	int w;


	g.mlx_ptr = mlx_init();
//	if (RECORD_VIDEO)
//		fd = open("./video.sh", O_WRONLY | O_CREAT | O_APPEND, S_IRWXU);
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
//	check arg allocates memory for g->obj and their centers
	if (!check_arg(argv, argc, &g/*, ctr*/))
		return (0);
	printf("new image\n");
	
//	g.win_ptr = mlx_new_window(g.mlx_ptr, WIDTH, HEIGHT, "window1");
//	g.mlx_ptr = mlx_init();
	// with mymlx window should be created always after mlx_init
	// not necessery in this version of rtv1, here only for homogenity between versions
	// of rtv1
	g.win_ptr = mlx_new_window(g.mlx_ptr, WIDTH, HEIGHT, "window1");



	g.img_ptr = mlx_new_image(g.mlx_ptr, WIDTH, HEIGHT);
	g.data_ptr = (int *)mlx_get_data_addr(g.img_ptr, &g.bpp, &g.sz_l, &g.e);

//	testtest
	int local_endian;
	  int   a;

	  printf("MinilibX Test Program\n");
	  a = 0x11223344;

    	  if (((unsigned char *)&a)[0] == 0x11)
                local_endian = 1;
        else
                local_endian = 0;
	printf("bpp: %d, sz_l = %d, endian = %d\n", g.bpp, g.sz_l, g.e);
	printf("local endian: %d\n", local_endian);

//	testend testend

	if (!g.data_ptr || !g.img_ptr || !g.mlx_ptr)
	{
		printf("mlx_get_data_addr or img_ptr or mlx_ptr returned NULL");
		return (0);
	}
	printf("data_ptr is %p\n", g.data_ptr);
	printf("img_ptr is %p\n", g.img_ptr);
	printf("mlx_ptr is %p\n", g.mlx_ptr);


//	g.win_ptr = mlx_new_window(g.mlx_ptr, WIDTH, HEIGHT, "window1");
	copy_tcps(&g);

	printf("starting threads\n");
	start_threads(recalc, &g);
	mlx_hook(g.win_ptr, 4, 4, mouse_press, &g);
	mlx_hook(g.win_ptr, 2, 1L<<0, key_press, &g);
	mlx_hook(g.win_ptr, 6, 1L<<6, mouse_move, &g);
//	mlx_mouse_hook(g.win_ptr, mouse_move, &g);
//	mlx_hook(g.win_ptr, 2, 2, key_press, &g);
//	mlx_hook(g.win_ptr, 6, 6, mouse_move, &g);


//	mlx_loop_hook(g.mlx_ptr, loop, &g);
//	printf("doing mlx_loop\n");
	mlx_loop(g.mlx_ptr);
}
