#ifndef CPARTS_H
#define CPARTS_H

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

#include "cpart.h"

using namespace cv;
using namespace std;

class cParts : public QObject
{
    Q_OBJECT
public:
    explicit cParts(QObject *parent = 0);

signals:

public slots:

public:

    vector<cPart> part_list;



private:

};

#endif // CPARTS_H
