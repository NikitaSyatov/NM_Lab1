#pragma once
#include "Class.h"


void RK4(double xstart, double y1start, double y2start, double stepstart,double Xmax, double a = 1.0, int maxSteps = 9999, double tolerance = 0.00001, double edge = 0.0000001)
{
	execution a1(xstart, y1start, y2start, stepstart, a),tmp(a1);
	execution a05(xstart, y1start, y2start, stepstart/2.0, a);

    int iter = 0,m = maxSteps;
    while (m)
    {
        iter++;
        m /= 10;
    }

    int p = 4, C1 = 0, C2 = 0,flg1 = 0,flg2 = 0;
    double Normal = 0.0,S1=0.0,S2=0.0,Epsilon = tolerance;

    ofstream out1;

    out1.open("../Execution/ux1");//файл в который записывается решение u



    if (out1.is_open())
    {
        out1 << setfill(' ') << setw(4) << "i";
        out1 << setfill(' ') << setw(20) << "x";
        out1 << setfill(' ') << setw(20) << "u";
        out1 << setfill(' ') << setw(20) << "u^";
        out1 << setfill(' ') << setw(20) << "u'";
        out1 << setfill(' ') << setw(20) << "u'^";
        out1 << setfill(' ') << setw(20) << "h";
        out1 << setfill(' ') << setw(20) << "Error";
        out1 << setfill(' ') << setw(10) << "C1";
        out1 << setfill(' ') << setw(8) << "C2";
        out1 << endl;

        out1 << setfill(' ') << setw(4) << 0;
        out1 << setfill(' ') << setw(20) << xstart;
        out1 << setfill(' ') << setw(20) << y1start;
        out1 << setfill(' ') << setw(20) << "-";
        out1 << setfill(' ') << setw(20) << y2start;
        out1 << setfill(' ') << setw(20) << "-";
        out1 << setfill(' ') << setw(20) << "-";
        out1 << setfill(' ') << setw(20) << "-";
        out1 << setfill(' ') << setw(10) << 0;
        out1 << setfill(' ') << setw(8) << 0;
        out1 << endl;
    }

    while (maxSteps > a1.get_iteration() && a1.get_x()<Xmax)
    {

        if (fabs(Xmax -(a1.get_x() + a1.get_h())) < edge)
        {
            break;
        }
        else
        {
            while (a1.get_x() + a1.get_h() > Xmax)
            {
                a1.delstep2();
            }
        }

        flg1 = 0;

        tmp = a1;
        a05 = a1;
        a1.RK4();
        a05.delstep2();
        a05.RK4();
        a05.RK4();

        S1 = fabs(a1.get_y1() - a05.get_y1()) / (pow(2, p) - 1);
        S2 = fabs(a1.get_y2() - a05.get_y2()) / (pow(2, p) - 1);

        Normal = sqrt(pow(S1,2) + pow(S2,2));

        if (Normal < Epsilon / (pow(2, p + 1)))
        {
            C2++;
            flg1 = 1;
        }

        while (Normal > Epsilon)
        {
            flg2++;
            C1++;
            a1 = tmp;
            a1.delstep2();
            tmp = a1;
            a05 = a1;
            a05.delstep2();

            a1.RK4();
            a05.RK4();
            a05.RK4();

            S1 = fabs(a1.get_y1() - a05.get_y1()) / (pow(2, p) - 1);
            S2 = fabs(a1.get_y2() - a05.get_y2()) / (pow(2, p) - 1);

            Normal = sqrt(pow(S1, 2) + pow(S2, 2));
        }








        if (out1.is_open())
        {
            out1 << setfill(' ') << setw(4) << a1.get_iteration();
            out1 << setfill(' ') << setw(20) << a1.get_x();
            out1 << setfill(' ') << setw(20) << a1.get_y1();
            out1 << setfill(' ') << setw(20) << a05.get_y1();
            out1 << setfill(' ') << setw(20) << a1.get_y2();
            out1 << setfill(' ') << setw(20) << a05.get_y2();
            out1 << setfill(' ') << setw(20) << a1.get_h();
            out1 << setfill(' ') << setw(20) << Normal;
            out1 << setfill(' ') << setw(10) << C1;
            out1 << setfill(' ') << setw(8) << C2;
            out1 << endl;

        }

        if (flg1)
        {
            a1.addstep2();
        }


    }
    out1.close();

}