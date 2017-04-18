#ifndef CPACKAGES_H
#define CPACKAGES_H

#include <QObject>

#include <QString>
#include <QStringList>

#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"

#include <iostream>
#include <math.h>
#include <string.h>

#include "cpackage.h"


using namespace cv;
using namespace std;

//holds master list of all packages
//has routines for loading packages from user and file
//has checks for acceptable nozzles

class cPackages : public QObject
{
    Q_OBJECT
public:
    explicit cPackages(QObject *parent = 0);

signals:

public slots:

public:

    //routine to load and create package list from data in csv file
    void load_packages_from_file();

    //void create_new_package_definition;

    vector<cPackage> package_list;


private:
};

#endif // CPACKAGES_H
