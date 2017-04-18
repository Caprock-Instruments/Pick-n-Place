#include "cpart.h"

cPart::cPart()
{

}

//comparator symbol
//note: needs to be a const function as well [which means we won't change values of class variables inside class object that is being pass-in
bool cPart::operator<(cPart &rhs) const
{
    QString lhs_sNozzleType = sNozzleType;
    QString rhs_sNozzleType = rhs.sNozzleType;

    bool is_lessThan = compare_for_sort(lhs_sNozzleType, rhs_sNozzleType);

    if(is_lessThan)
        return true;
    else
        return false;

}


//returns bool true is lhs < rhs
//note: needs to be a const function as well [which means we won't change values of variables that were passed in
bool cPart::compare_for_sort(QString lhs_sNozzleType, QString rhs_sNozzleType) const
{
    bool is_lessThan = false;

    int cnt_lhs = lhs_sNozzleType.count();
    int cnt_rhs = rhs_sNozzleType.count();

    if((cnt_lhs > 1)&&(cnt_rhs > 1))
    {
        QString lhs_sNT = lhs_sNozzleType.mid(1);  //return substring less first char
        QString rhs_sNT = rhs_sNozzleType.mid(1);  //return substring less first char

        int lhs_NT = lhs_sNT.toInt();
        int rhs_NT = rhs_sNT.toInt();

        if(lhs_NT < rhs_NT)
        {
            is_lessThan = true;
        }
    }

    return is_lessThan;
}
