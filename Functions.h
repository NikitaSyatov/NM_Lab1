
#pragma once
#include "class.h"


void RK4(double xstart, double y1start, double y2start, double stepstart,double Xmax, double a = 1.0, int maxSteps = 100, double tolerance = 0.001, double edge = 0.0)
{
	execution a1(xstart, y1start, y2start, stepstart, a),tmp(a1);
	execution a05(xstart, y1start, y2start, stepstart/2.0, a);

    int p = 4, C11 = 0, C12 = 0,C21 = 0, C22 = 0;
    double localerase1,localerase2,S1,S2,Epsilon = tolerance;

    ofstream out1,out2;

    out1.open("/home/syatov430/VAZHNO/NM_Lab1/ux1.txt");
    out2.open("/home/syatov430/VAZHNO/NM_Lab1/ux2.txt");


    if (out1.is_open())
    {
        a1 << out1;
        out1 << endl;
    }
    if (out2.is_open())
    {
        a1 << out2;
        out2 << endl;
    }


    while (a1.get_x() < Xmax && a1.get_iteration() < maxSteps)
    {
        tmp = a1;
        a05 = a1;
        a05.delstep2();
        a1.RK4();
        a05.RK4();
        a05.RK4();

        localerase1 = fabs(a05.get_y1() - a1.get_y1());
        localerase2 = fabs(a05.get_y2() - a1.get_y2());

        S1 = fabs(a05.get_y1() - a1.get_y1()) / (pow(2, p) - 1);
        S2 = fabs(a05.get_y2() - a1.get_y2()) / (pow(2, p) - 1);

        if (S1 < Epsilon / pow(2, p + 1))
        {
            a1.addstep2();
            C12++;
        }
        else if (S2 < Epsilon / pow(2, p + 1))
        {
            a1.addstep2();
            C22++;
        }
        else if (S1 > Epsilon)
        {
            a1 = tmp;
            a05 = a1;
            do
            {
                a1.delstep2();
                a05 = a1;
                a05.delstep2();
                C11++;
                a1.RK4();
                a05.RK4();
                a05.RK4();
            } while (fabs(a05.get_y1() - a1.get_y1()) / (pow(2, p) - 1) > Epsilon);

        }
        else if (S2 > Epsilon)
        {
            a1 = tmp;
            a05 = a1;
            do
            {
                a1.delstep2();
                a05 = a1;
                a05.delstep2();
                C21++;
                a1.RK4();
                a05.RK4();
                a05.RK4();
            } while (fabs(a05.get_y2() - a1.get_y2()) / (pow(2, p) - 1) > Epsilon);
        }




        if (out1.is_open())
        {
            a1 << out1;
            out1 << "\t\t" << localerase1 << "\t\t" << C11 << "\t\t" << C12 << endl;
        }
        if (out2.is_open())
        {
            a1 << out2;
            out2 << "\t\t" << localerase2 << "\t\t" << C21 << "\t\t" << C22 << endl;
        }
    }
    out1.close();
    out2.close();
}