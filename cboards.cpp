#include "cboards.h"

cBoards::cBoards()
{

}

void cBoards::sort_parts_by_nozzle_size()
{
    //this requires a bool comparison operator be built into class cPart
    std::sort(master_parts_list.begin(), master_parts_list.end());
}

void cBoards::append_part_to_master_list(cBoard board)
{

    master_parts_list.insert(master_parts_list.end(), board.part_list.begin(), board.part_list.end());
}

void cBoards::clear_master_list()
{

}

bool cBoards::assign_all_parts_to_nozzles(vector<cNozzles> nozzles_defs_list, const int MAX_NUM_NOZZLES)
{
    bool ok = false;

    int N = static_cast<int>(master_parts_list.size());

    if(N==0)
        return false;

    QStringList nozzle_fill_status;

    init_nozzle_fill_status(nozzle_fill_status, MAX_NUM_NOZZLES);

    for(int i=0;i<N;i++)
    {
        cPart part = master_parts_list[i];

        ok = assign_nozzle(part, nozzle_fill_status, nozzles_defs_list, MAX_NUM_NOZZLES);

        if(!ok)
            return false;
    }


}



void cBoards::init_nozzle_fill_status(QStringList &nozzle_fill_status, const int MAX_NUM_NOZZLES)
{
    nozzle_fill_status.clear();

    for(int i=0;i<MAX_NUM_NOZZLES;i++)
    {
        nozzle_fill_status << "EMPTY";
    }
}

bool cBoards::find_compatible_nozzles(cPart part, vector<int> &compatibleNozzles_list, cNozzles nozzle_list)
{
    bool found = false;

    int cnt = static_cast<int>(nozzle_list.nozzle_list.size());

    QString sPart_package = part.sPackage;

    for(int i=0;i<cnt;i++)
    {
        cNozzle nozzle = nozzle_list.nozzle_list[i];

        QStringList comp_nozzle_packages = nozzle.sPackages;

        int N = comp_nozzle_packages.count();

        for(int j=0;j<N;j++)
        {
            QString sPackage = comp_nozzle_packages[j];

            if(sPart_package == sPackage)
            {
                //found compatible nozzle
                found = true;
                compatibleNozzles_list.push_back(i);  //save nozzle_location in nozzle_list
            }
        }

    }

    return found;
}


//start_fresh_fill means go do a place op and then this part will start over filling up nozzles
bool cBoards::assign_nozzle(cPart part, QStringList &nozzle_fill_status, vector<cNozzles> nozzles_defs_list, const int MAX_NUM_NOZZLES)
{
    bool found = false;

    //find list of compatible nozzles

    QString sPackage = part.sPackage;

    //need active nozzle list (what do we do when we switch 1/2 way through to new set of nozzles?
    //how to know if we should just start a fresh fill or switch to a new rack?

    int cnt = static_cast<int>(nozzles_defs_list.size());



    for(int i=0;i<cnt;i++)
    {
        cNozzles nozzle_list = nozzles_defs_list[i];

        vector<int> compatibleNozzles_list;

        bool found = find_compatible_nozzles(part, compatibleNozzles_list, nozzle_list);

        if(found)  //found so it we don't find an EMPTY NOZZLE; DO REFILL
        {
            int N = static_cast<int>(compatibleNozzles_list.size());

            for(int j=0;j<N;j++)
            {
                int nozzle_id = compatibleNozzles_list[j];

                if(nozzle_id < MAX_NUM_NOZZLES)
                {
                    //need to check if nozzle is in use
                    if(nozzle_fill_status[nozzle_id]=="EMPTY")
                    {
                        //FOUND OUR NOZZLE
                        found = true;
                        nozzle_Id_to_use = nozzle_id;
                        nozzle_def_list_to_use = i;

                        nozzle_fill_status[nozzle_id] = sPackage;
                        return found;
                    }
                }
            }

            //DIDN'T FIND IT SO START FRESH FILL
            start_fresh_fill = true;

            //reset nozzle_fill_status to back all empty
            init_nozzle_fill_status(nozzle_fill_status, MAX_NUM_NOZZLES);

            for(int j=0;j<N;j++)
            {
                int nozzle_id = compatibleNozzles_list[j];

                if(nozzle_id < MAX_NUM_NOZZLES)
                {
                    //need to check if nozzle is in use
                    if(nozzle_fill_status[nozzle_id]=="EMPTY")
                    {
                        //FOUND OUR NOZZLE
                        nozzle_Id_to_use = nozzle_id;
                        nozzle_def_list_to_use = i;

                        nozzle_fill_status[nozzle_id] = sPackage;
                        return found;
                    }
                }
            }

            //if were here; we have an error
            //flag error
            //QMessageBox msgBox;
            //msgBox.critical(0, "Error", "coding error; shouldn't be here; inside assign nozzle");

            return found;


        }

    }

    if(!found)
    {
        //flag error
        //QMessageBox msgBox;
        //msgBox.critical(0, "Error", "Couldn't Assign Nozzle...");

        return found;
    }

    part.start_fresh_nozzle_fill = start_fresh_fill;
    part.nozzle_id = nozzle_Id_to_use;
    part.nozzle_def_list_to_use = nozzle_def_list_to_use;

    //get nozzleType (used for sorting cParts in a board by nozzle-size)
    cNozzles cNozzle_list = nozzles_defs_list[nozzle_def_list_to_use];
    cNozzle nozzle = cNozzle_list.nozzle_list[nozzle_Id_to_use];
    QString sNozzleType = nozzle.sNozzleType;
    part.sNozzleType = sNozzleType;


    return found;
}
