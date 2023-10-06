#pragma once
#include "Header.h"



class execution
{
private:
	double x;
	double y1;
	double y2;
	double step;
	double a;
	int iteration = 0;
public:
	execution(double xstart, double y1start, double y2start, double stepstart, double a):x(xstart),y1(y1start), y2(y2start), step(stepstart),a(a){}

	double f1(double x, double y1, double y2)
	{
		return y2;
	}
	double f2(double x, double y1, double y2)
	{
		return -a*sin(y1);
	}

	void RK4()// пока шаг не меняется
	{
		double xlast = x;
		double y1last = y1;
		double y2last = y2;
		double steplast = step;
		double h = step / 2.0;



		double ky11 = f1(x, y1, y2);
		double ky21 = f2(x, y1, y2);

		double ky12 = f1(x + h, y1 + h * ky11, y2 + h * ky21);
		double ky22 = f2(x + h, y1 + h * ky11, y2 + h * ky21);

		double ky13 = f1(x + h, y1 + h * ky12, y2 + h * ky22);
		double ky23 = f2(x + h, y1 + h * ky12, y2 + h * ky22);

		double ky14 = f1(x + h, y1 + h * ky13, y2 + h * ky23);
		double ky24 = f2(x + h, y1 + h * ky13, y2 + h * ky23);

		y1 = y1 + h * (ky11 + 2 * ky12 + 2 * ky13 + ky14);
		y2 = y2 + h * (ky21 + 2 * ky22 + 2 * ky23 + ky24);

		x = xlast + steplast;

		iteration++;
	}
	double get_x() { return x; }
	double get_y1() { return y1; }
	double get_y2() { return y2; }

	void delstep2() { step /= 2.0; }
	void addstep2() { step *= 2.0; }

	int get_iteration() { return iteration; }
	ostream& operator<<(ostream& os)
	{
		string tab = "	";
		return os << iteration << tab << x << tab << y1 << tab << y2 << tab << step;
	}
};