#include "../include/Functions.h"



int main()
{
    //execution First(0.0, 1.0, 0.0, 0.1, 1.0);// ��� ������� (x0,y(x0),y`(x0),h,a-��������)
    //double maxX = 3.0;// �� ������ � ����� �������
    //ofstream out;
    //out.open("../Execution/ux2");//���� � ������� ������������, � ���� ��� ��������� ������ � ux2
    //if (out.is_open())
    //{
    //    First << out;//������ ���� ������� �������
    //    out << endl;
    //}
    // while (First.get_x() < maxX)
    //{
    //    First.RK4();
    //    if (out.is_open())
    //    {
    //        First << out;//����������� ��������
    //        out << endl;
    //    }
    //}
    //out.close();
    //cout << "File has been written" << endl;


    RK4(0.0,0.0,1.0,0.1,3.0,1);


}