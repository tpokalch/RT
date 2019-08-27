
#include "rtv1.h"

void		init_vector(t_vector *i, double x, double y, double z)
{
	i->x = x;
	i->y = y;
	i->z = z;
}

t_vector		cross(t_vector a, t_vector b)
{
	t_vector ret;

	ret.x = -a.y * b.z + a.z * b.y;
	ret.y = -a.z * b.x + a.x * b.z;
	ret.z = -a.x * b.y + a.y * b.x;

//	ret = scale(-1, ret);
//	if (con(g))
//	printf("returning nrm of tri %f,%f,%f\n", ret.x, ret.y, ret.z);
	return (ret);
}

int			left(t_vector a, t_vector b, t_vector nr, t_global *g)
{
/*	t_vector norma;
	t_vector b_p;
	t_vector newbas[2];

	a = norm(a);
	b = norm(b);
	newbas[0] = a;
	b_p.y = dot(a, b);
	newbas[1] = norm(diff(b, scale(b_p.y, a)));
	b_p.x = dot(newbas[1], b);
	a.y = dot(newbas[0], a);
	a.x = dot(newbas[1], a);
	if (con(g))
	{
		printf("a is %f,%f,%f\n", a.x, a.y, a.z);
		printf("b is %f,%f,%f\n", b.x, b.y, b.z);
	}
#if0
	b_p.y = dot(norm(a), b);
	b_p.x = dot(b, norm(diff(b, scale(b_p.y, norm(a)))));
	b_p.x = dot(b, diff(b, scale(b_p.y, norm(a))));
	b_p.x = dot(b, diff(b, scale(dot(b, norm(a)), a)));
	b = b_p;
#endif
	if (con(g))
		printf("b_p is %f,%f\n", b.x, b.y);
//	a.x = 0;
//	a.y = dot(a, norm(a));
	if (con(g))
		printf("a_p is %f, %f", a.x, a.y);
	if (con(g))
		printf("left %f\n", a.x * b.y - a.y * b.x); 
*/
	if (con(g))
	{
//		printf("bound is %f, %f, %f\n", b.x, b.y, b.z);
//		printf("point is %f, %f, %f\n", a.x, a.y, a.z);
//		printf("x y %f\n", a.x * b.y - a.y * b.x);
//		printf("y z %f\n", a.y * b.z - a.z * b.y);
//		printf("z x %f\n",  a.x * b.z - a.z * b.x);
	}
	return (dot(cross(b, a), nr) >= 0);
}

int			pinside(t_vector p, t_vector bd1, t_vector bd2, t_vector bd3, t_vector nr, t_global *g)
{
	t_vector bd[3];
	t_vector pt[3];

	bd[0] = diff(bd2, bd1);
	bd[1] = diff(bd3, bd2);
	bd[2] = diff(bd1, bd3);
	pt[0] = diff(p, bd1);
	pt[1] = diff(p, bd2);
	pt[2] = diff(p, bd3);

	if (con(g))
	{
//		printf("point is %f,%f,%f\n", p.x, p.y, p.z);
//		printf("bounds are %f,%f,%f\n%f,%f,%f\n%f,%f,%f\n", bd1.x, bd1.y, bd1.z, bd2.x, bd2.y, bd2.z, bd3.x, bd3.y, bd3.z);
//		printf("vector bounds are %f,%f,%f\n%f,%f,%f\n%f,%f,%f\n", bd[0].x, bd[0].y, bd[0].z, bd[1].x, bd[1].y, bd[1].z, bd[2].x, bd[2].y, bd[2].z);

/*		printf("left1 %d\n", left(diff(p, bd3), diff(bd1, bd3), g));
		printf("left2 %d\n", left(diff(p, bd2), diff(bd3, bd2), g));
		printf("left3 %d\n", left(diff(p, bd1), diff(bd2, bd1), g));
*/		t_vector a[2];
		init_vector(&a[0], 5, 7, 0);
		init_vector(&a[1], 7, 3, 0);
//		printf("left custom %d\n", left(a[0], a[1], g));
	}
	return (left(pt[0], bd[0], nr, g)
		&& left(pt[2], bd[2], nr, g)
		&& left(pt[1], bd[1], nr, g));
}

double			det(t_vector a, t_vector b)
{
	return (a.x * b.z - a.z * b.x);
}

t_vector		rotate(t_vector ray, t_vector angle)
{
	t_vector ret;
	t_vector cxcycz;
	t_vector sxsysz;
	t_vector opt;
	t_vector row[3];

	if (angle.x == 0 && angle.y == 0 && angle.z == 0)
		return (ray);
	init_vector(&cxcycz, cos(angle.x), cos(angle.y), cos(angle.z));
	init_vector(&sxsysz, sin(angle.x), sin(angle.y), sin(angle.z));
	init_vector(&opt, cxcycz.z * sxsysz.x,
	cxcycz.z * cxcycz.x, sxsysz.z * sxsysz.y);
	init_vector(&row[0], cxcycz.z * cxcycz.y,  -sxsysz.z * cxcycz.x
	+ opt.x * sxsysz.y, sxsysz.z * sxsysz.x + opt.y * sxsysz.y);
	init_vector(&row[1], sxsysz.z * cxcycz.y, opt.y
	+ opt.z * sxsysz.x, -opt.x + opt.z * cxcycz.x);
	init_vector(&row[2], -sxsysz.y, cxcycz.y * sxsysz.x, cxcycz.y * cxcycz.x);
	init_vector(&ret, dot(row[0], ray), dot(row[1], ray), dot(row[2], ray));
	return (ret);
}
/*
t_vector			rotate3(t_vector base[3], t_vector ang)
{
	t_vector ret[3];

	ret[0] = rotate(base[0], ang);
	ret[0] = rotate(base[0], ang);
	ret[0] = rotate(base[0], ang);
	return (ret)
*/

t_dstpst			*NANI(t_dstpst *t)
{
	t->dst = NAN;
	t->obj.name = NULL;
	return (t);
}
