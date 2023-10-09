#include "/home/syatov430/VAZHNO/NM_Lab1/Functions.h"

int main()
{
    //execution First(0.0, 1.0, 0.0, 0.1, 1.0);// íà÷ óñëîâèÿ (x0,y(x0),y`(x0),h,a-ïàðàìåòð)
    //double maxX = 3.0;// äî êàêîãî Õ áóäåì ñ÷èòàòü
    //ofstream out;
    //out.open("../Execution/ux2");//ôàéë â êîòîðûé çàïèñûâàåòñÿ, ÿ ïîêà ÷òî çàïèñûâàþ òîëüêî â ux2
    //if (out.is_open())
    //{
    //    First << out;//Ïåðâûé ââîä íóëåâûõ óñëîâèé
    //    out << endl;
    //}
    // while (First.get_x() < maxX)
    //{
    //    First.RK4();
    //    if (out.is_open())
    //    {
    //        First << out;//Ïîñëåäóþùèå èòåðàöèè
    //        out << endl;
    //    }
    //}
    //out.close();
    //cout << "File has been written" << endl;


    RK4(0.0,0.0,1.0,0.1,3.0,1);
}