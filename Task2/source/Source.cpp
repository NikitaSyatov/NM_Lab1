#include "../include/Functions.h"



int main()
{
    //execution First(0.0, 1.0, 0.0, 0.1, 1.0);// нач условия (x0,y(x0),y`(x0),h,a-параметр)
    //double maxX = 3.0;// до какого Х будем считать
    //ofstream out;
    //out.open("../Execution/ux2");//файл в который записывается, я пока что записываю только в ux2
    //if (out.is_open())
    //{
    //    First << out;//Первый ввод нулевых условий
    //    out << endl;
    //}
    // while (First.get_x() < maxX)
    //{
    //    First.RK4();
    //    if (out.is_open())
    //    {
    //        First << out;//Последующие итерации
    //        out << endl;
    //    }
    //}
    //out.close();
    //cout << "File has been written" << endl;


    RK4(0.0,0.0,1.0,0.1,3.0,1);


}