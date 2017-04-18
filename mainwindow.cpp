#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QFile>
#include <QDir>
#include <QFileDialog>
#include <QSettings>

#include <QMessageBox>
#include <QDesktopServices>
#include <QStandardPaths>

#include <QLabel>
#include <QPainter>

#include "my_qlabel.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    load_mainWindow_icons();


    cocv = new cOpenCv_Utils();

    cgc1 = new cGCode();  //class to create and keep track of controller-1's position
    cgc2 = new cGCode();  //class to create and keep track of controller-2's position
    cgc3 = new cGCode();  //class to create and keep track of controller-3's position

    cfc = new cFlyingCamera();



    img_mouse_captured = false;
    mouse_capture_count=0;


//setup settings for loading
    QString m_path ;
    QString m_filename;
    //QSettings * p_settings;

    m_path = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) ;
    m_filename = "config.ini" ;

    p_settings = new QSettings(m_path + "/"+ m_filename,QSettings::IniFormat);

    //saveSettings();

    //load from resource file if possible (if not fall back to already loaded values
    loadSettings();  //load up settings from ini file in config.qrc



    serial = new QSerialPort(this);
    serial2 = new QSerialPort(this);
    serial3 = new QSerialPort(this);


    //connect(serial, &QSerialPort::readyRead, this, &MainWindow::serialRead);
    connect(serial,SIGNAL(readyRead()),this,SLOT(serialRead()));
    connect(serial2,SIGNAL(readyRead()),this,SLOT(serialRead_com2()));
    connect(serial3,SIGNAL(readyRead()),this,SLOT(serialRead_com3()));

    initSerialPort();
    initSerialPort_com2();
    initSerialPort_com3();



    //"Display window"
    //cv::namedWindow("Display_Image",WINDOW_NORMAL);
    //cv::resizeWindow("Display_Image", 800,600);

    //need to shift focus back on us
    //this->activateWindow();

    //setup mouse slots
    connect(ui->label_imageShow,SIGNAL(Mouse_Pos()),this,SLOT(Mouse_current_pos()));
    connect(ui->label_imageShow,SIGNAL(Mouse_Pressed()),this,SLOT(Mouse_Pressed()));
    connect(ui->label_imageShow,SIGNAL(Mouse_Right_Pressed()),this,SLOT(Mouse_Right_Pressed()));
    connect(ui->label_imageShow,SIGNAL(Mouse_Left()),this,SLOT(Mouse_left()));
    connect(ui->label_imageShow,SIGNAL(Mouse_Released()),this,SLOT(Mouse_Released()));


    load_image();

    //setup gcode timer call-back
    gcodeTimer_com1 = new QTimer();
    gcodeTimer_com2 = new QTimer();
    gcodeTimer_com3 = new QTimer();

    connect(gcodeTimer_com1,SIGNAL(timeout()),this,SLOT(gcode_timer_callback_com1()));
    connect(gcodeTimer_com2,SIGNAL(timeout()),this,SLOT(gcode_timer_callback_com2()));
    connect(gcodeTimer_com3,SIGNAL(timeout()),this,SLOT(gcode_timer_callback_com3()));

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::loadSettings()
{

    //QSettings settings(sInit_fileName, QSettings::IniFormat);

    //check that we can read it (that it's not missing)
    p_settings->beginGroup("GROUP_TEST");
    int param1 = p_settings->value("PARAM1").toInt();
    p_settings->endGroup();

    if(param1 == 99)  //should read 99 if it's there and a correct file
    {
/*
        p_settings->beginGroup("GROUP_SaturValue");
        //QString param3b = settings.value("PARAM3").toString();  //example on how to read in string
        low_satVal = p_settings->value("low_satVal").toInt();
        high_satVal = p_settings->value("high_satVal").toInt();
        low_val = p_settings->value("low_val").toInt();
        high_val = p_settings->value("high_val").toInt();
        low_blue_satVal = p_settings->value("low_blue_satVal").toInt();
        high_blue_satVal = p_settings->value("high_blue_satVal").toInt();
        low_grn_satVal = p_settings->value("low_grn_satVal").toInt();
        high_grn_satVal = p_settings->value("high_grn_satVal").toInt();
        low_red_satVal = p_settings->value("low_red_satVal").toInt();
        high_red_satVal = p_settings->value("high_red_satVal").toInt();
        low_red2_satVal = p_settings->value("low_red2_satVal").toInt();
        high_red2_satVal = p_settings->value("high_red2_satVal").toInt();
        low_yellow_satVal = p_settings->value("low_yellow_satVal").toInt();
        high_yellow_satVal = p_settings->value("high_yellow_satVal").toInt();
        low_cyan_satVal = p_settings->value("low_cyan_satVal").toInt();
        high_cyan_satVal = p_settings->value("high_cyan_satVal").toInt();
        p_settings->endGroup();

        p_settings->beginGroup("GROUP_HUES");
        low_blue_h = p_settings->value("low_blue_h").toInt();
        high_blue_h = p_settings->value("high_blue_h").toInt();
        low_grn_h = p_settings->value("low_grn_h").toInt();
        high_grn_h = p_settings->value("high_grn_h").toInt();
        low_red_h = p_settings->value("low_red_h").toInt();
        high_red_h = p_settings->value("high_red_h").toInt();
        low_red2_h = p_settings->value("low_red2_h").toInt();
        high_red2_h = p_settings->value("high_red2_h").toInt();
        low_yellow_h = p_settings->value("low_yellow_h").toInt();
        high_yellow_h = p_settings->value("high_yellow_h").toInt();
        low_cyan_h = p_settings->value("low_cyan_h").toInt();
        high_cyan_h = p_settings->value("high_cyan_h").toInt();
        p_settings->endGroup();
*/
    }


}

void MainWindow::saveSettings()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Save Configuration", "Are you sure you want to Save Pixel Classifier Values?", QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        //qDebug() << "Yes was clicked";
        //QApplication::quit();

        //QSettings settings(sInit_fileName, QSettings::IniFormat);

        p_settings->beginGroup("GROUP_TEST");
        p_settings->setValue("PARAM1", 99);
        p_settings->endGroup();
/*
        p_settings->beginGroup("GROUP_SaturValue");
        p_settings->setValue("low_satVal", low_satVal);
        p_settings->setValue("high_satVal", high_satVal);
        p_settings->setValue("low_val", low_val);
        p_settings->setValue("high_val", high_val);
        p_settings->setValue("low_blue_satVal", low_blue_satVal);
        p_settings->setValue("high_blue_satVal", high_blue_satVal);
        p_settings->setValue("low_grn_satVal", low_grn_satVal);
        p_settings->setValue("high_grn_satVal", high_grn_satVal);
        p_settings->setValue("low_red_satVal", low_red_satVal);
        p_settings->setValue("high_red_satVal", high_red_satVal);
        p_settings->setValue("low_red2_satVal", low_red2_satVal);
        p_settings->setValue("high_red2_satVal", high_red2_satVal);
        p_settings->setValue("low_yellow_satVal", low_yellow_satVal);
        p_settings->setValue("high_yellow_satVal", high_yellow_satVal);
        p_settings->setValue("low_cyan_satVal", low_cyan_satVal);
        p_settings->setValue("high_cyan_satVal", high_cyan_satVal);
        p_settings->endGroup();

int lbh = low_blue_h;
int hbh = high_blue_h;



        p_settings->beginGroup("GROUP_HUES");
        p_settings->setValue("low_blue_h", low_blue_h);
        p_settings->setValue("high_blue_h", high_blue_h);
        p_settings->setValue("low_grn_h", low_grn_h);
        p_settings->setValue("high_grn_h", high_grn_h);
        p_settings->setValue("low_red_h", low_red_h);
        p_settings->setValue("high_red_h", high_red_h);
        p_settings->setValue("low_red2_h", low_red2_h);
        p_settings->setValue("high_red2_h", high_red2_h);
        p_settings->setValue("low_yellow_h", low_yellow_h);
        p_settings->setValue("high_yellow_h", high_yellow_h);
        p_settings->setValue("low_cyan_h", low_cyan_h);
        p_settings->setValue("high_cyan_h", high_cyan_h);
        p_settings->endGroup();
*/
    }
    else
    {
        //qDebug() << "No was clicked";
        return;
    }




}



void MainWindow::load_mainWindow_icons()
{
    QIcon icon_forward, icon_back, icon_right, icon_left, icon_up, icon_down, icon_ccw, icon_cw, icon_run, icon_stop;

    QPixmap qpm_forward, qpm_back, qpm_right, qpm_left, qpm_up, qpm_down, qpm_cw, qpm_ccw, qpm_run, qpm_stop;


    if(qpm_run.load(":/Run.png"))
    {
        icon_run.addPixmap(qpm_run);
        ui->pushButton_Run_Stop->setIcon(icon_run);
        ui->pushButton_Run_Stop->setIconSize(QSize(30,30));
    }

    if(qpm_left.load(":/Left_Arrow.png"))
    {
        icon_left.addPixmap(qpm_left);
        ui->pushButton_Left->setIcon(icon_left);
        ui->pushButton_Left->setIconSize(QSize(30,30));
    }

    if(qpm_right.load(":/Right_Arrow.png"))
    {
        icon_right.addPixmap(qpm_right);
        ui->pushButton_Right->setIcon(icon_right);
        ui->pushButton_Right->setIconSize(QSize(30,30));
    }

    if(qpm_forward.load(":/Forward_Arrow.png"))
    {
        icon_forward.addPixmap(qpm_forward);
        ui->pushButton_Forward->setIcon(icon_forward);
        ui->pushButton_Forward->setIconSize(QSize(30,30));
    }

    if(qpm_back.load(":/Backward_Arrow.png"))
    {
        icon_back.addPixmap(qpm_back);
        ui->pushButton_Reverse->setIcon(icon_back);
        ui->pushButton_Reverse->setIconSize(QSize(30,30));
    }


    if(qpm_up.load(":/Up_Arrow.png"))
    {
        icon_up.addPixmap(qpm_up);

        ui->pushButton_Up_1->setIcon(icon_up);
        ui->pushButton_Up_1->setIconSize(QSize(30,30));
        ui->pushButton_Up_2->setIcon(icon_up);
        ui->pushButton_Up_2->setIconSize(QSize(30,30));
        ui->pushButton_Up_3->setIcon(icon_up);
        ui->pushButton_Up_3->setIconSize(QSize(30,30));
        ui->pushButton_Up_4->setIcon(icon_up);
        ui->pushButton_Up_4->setIconSize(QSize(30,30));
    }

    if(qpm_down.load(":/Down_Arrow.png"))
    {
        icon_down.addPixmap(qpm_down);
        ui->pushButton_Down_1->setIcon(icon_down);
        ui->pushButton_Down_1->setIconSize(QSize(30,30));
        ui->pushButton_Down_2->setIcon(icon_down);
        ui->pushButton_Down_2->setIconSize(QSize(30,30));
        ui->pushButton_Down_3->setIcon(icon_down);
        ui->pushButton_Down_3->setIconSize(QSize(30,30));
        ui->pushButton_Down_4->setIcon(icon_down);
        ui->pushButton_Down_4->setIconSize(QSize(30,30));
    }

    if(qpm_cw.load(":/Rotate_CW.png"))
    {
        icon_cw.addPixmap(qpm_cw);
        ui->pushButton_CW_1->setIcon(icon_cw);
        ui->pushButton_CW_1->setIconSize(QSize(30,30));
        ui->pushButton_CW_2->setIcon(icon_cw);
        ui->pushButton_CW_2->setIconSize(QSize(30,30));
        ui->pushButton_CW_3->setIcon(icon_cw);
        ui->pushButton_CW_3->setIconSize(QSize(30,30));
        ui->pushButton_CW_4->setIcon(icon_cw);
        ui->pushButton_CW_4->setIconSize(QSize(30,30));
    }

    if(qpm_ccw.load(":/Rotate_CCW.png"))
    {
        icon_ccw.addPixmap(qpm_ccw);
        ui->pushButton_CCW_1->setIcon(icon_ccw);
        ui->pushButton_CCW_1->setIconSize(QSize(30,30));
        ui->pushButton_CCW_2->setIcon(icon_ccw);
        ui->pushButton_CCW_2->setIconSize(QSize(30,30));
        ui->pushButton_CCW_3->setIcon(icon_ccw);
        ui->pushButton_CCW_3->setIconSize(QSize(30,30));
        ui->pushButton_CCW_4->setIcon(icon_ccw);
        ui->pushButton_CCW_4->setIconSize(QSize(30,30));
    }

}

//file open
void MainWindow::on_actionOpen_triggered()
{

    QString jobFileName = job_directory + job_fileName;

    load_job(jobFileName);


}

//load a feeder configuration file
void MainWindow::on_actionLoad_Feeders_triggered()
{
    feeder_list.clear();

    QString fName = job_directory + feeders_fileName;

    load_feeders(fName);
}


//load nozzle configuration
void MainWindow::on_actionLoad_Nozzles_triggered()
{
    //clear out our list
    nozzle_fileNames_list.clear();

    QString fName1 = job_directory + nozzle_defs1_fileName;
    QString fName2 = job_directory + nozzle_defs2_fileName;


    //build list of nozzle definition fileNames
    nozzle_fileNames_list.append(fName1);
    nozzle_fileNames_list.append(fName2);

    load_nozzles(nozzle_fileNames_list);

}


//compile feeder info with board info to build a working job
void MainWindow::on_actionCompile_Job_triggered()
{
    void compile_job();
}


//file save
void MainWindow::on_actionSave_triggered()
{

}

//file close
void MainWindow::on_actionClose_triggered()
{

}
//program exit
void MainWindow::on_actionExit_triggered()
{

}

//config cnc
void MainWindow::on_actionCnc_triggered()
{

}


//Test machine; Jog
void MainWindow::on_actionJog_triggered()
{

}

void MainWindow::on_actionTeach_triggered()
{

}


void MainWindow::on_actionAbout_triggered()
{

}

void MainWindow::on_actionTutorial_triggered()
{

}

void MainWindow::on_actionManual_triggered()
{

}

void MainWindow::on_pushButton_Up_clicked()
{

}


#define Serial_Port_Start {

void MainWindow::initSerialPort()
{
    QString er_msg;


    //connect(serial,SIGNAL(readyRead()),this,SLOT(serialRead()));
    //connect(serial,SIGNAL(readyRead()),this,SLOT(serialRead()));

    //connect(serial, &QSerialPort::readyRead, this, &MainWindow::readData);


    //get name for choose
    QList<QSerialPortInfo>  infos = QSerialPortInfo::availablePorts();
    if(infos.isEmpty())
    {
        er_msg = er_msg + " No Serial Ports Available...";
        //ui->textEdit->setText(er_msg);
        ui->label_comm->setText(er_msg);
        //ui->comboBox->addItem("Invalid");
        return;
    }


    //serial = new QSerialPort("COM4");       //Serial number, must match up, capital!!! Serial number to modify.

    serial->setPortName("/dev/ttyUSB0");
    serial->setBaudRate(QSerialPort::Baud9600);         //Baud rate
    serial->setDataBits(QSerialPort::Data8);            //Data bits
    serial->setParity(QSerialPort::NoParity);           //No parity
    serial->setStopBits(QSerialPort::OneStop);          //No stop bit
    serial->setFlowControl(QSerialPort::NoFlowControl); //No control

    if(serial->open(QIODevice::ReadWrite) )                 //Read write open
    {
        er_msg += ": Connected to ttyUSB0";
        //ui->textEdit->setText(er_msg);
        ui->label_comm->setText(er_msg);
    }
    else
    {
        er_msg += ": NOT Connected to ttyUSB0";
        //ui->textEdit->setText(er_msg);
        ui->label_comm->setText(er_msg);
    }

}

void MainWindow::initSerialPort_com2()
{
    QString er_msg;


    //connect(serial,SIGNAL(readyRead()),this,SLOT(serialRead()));
    //connect(serial,SIGNAL(readyRead()),this,SLOT(serialRead()));

    //connect(serial, &QSerialPort::readyRead, this, &MainWindow::readData);


    //get name for choose
    QList<QSerialPortInfo>  infos = QSerialPortInfo::availablePorts();
    if(infos.isEmpty())
    {
        er_msg = er_msg + " No Serial Ports Available...";
        //ui->textEdit->setText(er_msg);
        ui->label_comm->setText(er_msg);
        //ui->comboBox->addItem("Invalid");
        return;
    }


    //serial = new QSerialPort("COM4");       //Serial number, must match up, capital!!! Serial number to modify.

    serial2->setPortName("/dev/ttyUSB1");
    serial2->setBaudRate(QSerialPort::Baud9600);         //Baud rate
    serial2->setDataBits(QSerialPort::Data8);            //Data bits
    serial2->setParity(QSerialPort::NoParity);           //No parity
    serial2->setStopBits(QSerialPort::OneStop);          //No stop bit
    serial2->setFlowControl(QSerialPort::NoFlowControl); //No control

    if(serial2->open(QIODevice::ReadWrite) )                 //Read write open
    {
        er_msg += ": Connected to ttyUSB0";
        //ui->textEdit->setText(er_msg);
        ui->label_comm->setText(er_msg);
    }
    else
    {
        er_msg += ": NOT Connected to ttyUSB0";
        //ui->textEdit->setText(er_msg);
        ui->label_comm->setText(er_msg);
    }

}

void MainWindow::initSerialPort_com3()
{
    QString er_msg;


    //connect(serial,SIGNAL(readyRead()),this,SLOT(serialRead()));
    //connect(serial,SIGNAL(readyRead()),this,SLOT(serialRead()));

    //connect(serial, &QSerialPort::readyRead, this, &MainWindow::readData);


    //get name for choose
    QList<QSerialPortInfo>  infos = QSerialPortInfo::availablePorts();
    if(infos.isEmpty())
    {
        er_msg = er_msg + " No Serial Ports Available...";
        //ui->textEdit->setText(er_msg);
        ui->label_comm->setText(er_msg);
        //ui->comboBox->addItem("Invalid");
        return;
    }


    //serial = new QSerialPort("COM4");       //Serial number, must match up, capital!!! Serial number to modify.

    serial3->setPortName("/dev/ttyUSB2");
    serial3->setBaudRate(QSerialPort::Baud9600);         //Baud rate
    serial3->setDataBits(QSerialPort::Data8);            //Data bits
    serial3->setParity(QSerialPort::NoParity);           //No parity
    serial3->setStopBits(QSerialPort::OneStop);          //No stop bit
    serial3->setFlowControl(QSerialPort::NoFlowControl); //No control

    if(serial3->open(QIODevice::ReadWrite) )                 //Read write open
    {
        er_msg += ": Connected to ttyUSB0";
        //ui->textEdit->setText(er_msg);
        ui->label_comm->setText(er_msg);
    }
    else
    {
        er_msg += ": NOT Connected to ttyUSB0";
        //ui->textEdit->setText(er_msg);
        ui->label_comm->setText(er_msg);
    }

}


void MainWindow::sendSerialMsg(QString msg, int com_id)
{
    if(msg.isEmpty())
    {
        return;
    }

    if(com_id == 1)
    {
        serial->write(msg.toLatin1());
    }
    else if(com_id == 2)
    {
        serial2->write(msg.toLatin1());
    }
    else if(com_id == 3)
    {
        serial3->write(msg.toLatin1());
    }
}

//more complete read example here: http://www.programering.com/a/MTO2YjNwATM.html
void MainWindow::serialRead()
{
    //ui->textEdit->setText("");  //clear Text display
    ui->label_comm->setText("");

    QByteArray qba = serial->readAll();
    //ui->textEdit->append(qba);
    ui->label_comm->setText(qba);


    QString sVal(qba);

    cgc1->update_rxd_buffer(sVal);
}

//more complete read example here: http://www.programering.com/a/MTO2YjNwATM.html
void MainWindow::serialRead_com2()
{
    QByteArray qba = serial2->readAll();

    QString sVal(qba);

    cgc2->update_rxd_buffer(sVal);
}

//more complete read example here: http://www.programering.com/a/MTO2YjNwATM.html
void MainWindow::serialRead_com3()
{
    QByteArray qba = serial3->readAll();

    QString sVal(qba);

    cgc3->update_rxd_buffer(sVal);
}

#define Serial_Port_End }


//key capture for image zoom and pan
void MainWindow::keyPressEvent( QKeyEvent* event )
{
    if(!event->isAutoRepeat())  //only adjust on 1st press event; ignore
    {
        switch ( event->key() )
        {
            case Qt::Key_Plus:
                //act on '+' (zoom In)
                zoom_factor++;
                break;
            case Qt::Key_Minus:
                //act on '-' (zoom Out)
                zoom_factor--;
                break;
            case Qt::Key_Escape:
                //act on 'Escape' (reset mouse capture classifier region)
                init_capture();  //needed so we can reset between zoom in and out
                break;
            default:
                event->ignore();
                break;
        }

        if(zoom_factor>1)
        {
            isZoomed = true;
        }
        else
        {
            zoom_factor = 1;
            isZoomed = false;
        }



    }
    else
    {
        switch ( event->key() )
        {
            case Qt::Key_4:
                //act on '4' (pan Left)
                pan_topLeft.x++;
                break;
            case Qt::Key_6:
                //act on '6' (pan Right)
                pan_topLeft.x--;
                break;
            case Qt::Key_8:
                //act on '8' (pan Up)
                pan_topLeft.y++;
                break;
            case Qt::Key_2:
                //act on '2' (pan Down)
                pan_topLeft.y--;
                break;
            default:
                event->ignore();
                break;
        }


        if(pan_topLeft.x < 0)
        {
            pan_topLeft.x = 0;
        }
        if(pan_topLeft.y < 0)
        {
            pan_topLeft.y = 0;
        }
    }

}

// ***** BE SURE TO SET MOUSE-TRACKING ON IN USER-INTERFACE:DESIGN FOR label_imageShow
// will also have to promote label_imageShow to use sub-class my_qlabel
// watch youtube: https://www.youtube.com/watch?v=d0CDMtfefB4
void MainWindow::Mouse_current_pos()
{
    int x = (int)ui->label_imageShow->x;
    int y = (int)ui->label_imageShow->y;


    img_mouse_pos.x = x;
    img_mouse_pos.y = y;
}

void MainWindow::Mouse_Pressed()
{
    if(mouse_firstPress)
    {
        img_mouse_captured = true;
        img_mouse_lastCaptured = img_mouse_pos;
        mouse_firstPress = false;
    }
    else
    {
        img_mouse_captured = false;
    }
}
void MainWindow::Mouse_Right_Pressed()
{
    if(mouse_right_firstPress)
    {
        img_mouse_captured = true;
        mouse_right_firstPress = false;
        img_mouse_right_captured = true;
    }
    else
    {
        img_mouse_right_captured = false;
    }
}

void MainWindow::Mouse_Released()
{
    mouse_firstPress = true;
}

void MainWindow::Mouse_Right_Released()
{
    mouse_right_firstPress = true;
}


void MainWindow::Mouse_left()
{

}

void MainWindow::clear_mouse_capture_buffer()
{
    mouse_capture_count = -1;
    for(int i=0;i<MOUSE_CAPTURE_BUFFER_SIZE;i++)
    {
        img_mousePts_captured[i].x = 0;
        img_mousePts_captured[i].y = 0;

    }

}

void MainWindow::init_capture()
{
    clear_mouse_capture_buffer();
    mouse_capture_count=-1;
    rubber_band_on = true;
    mouse_capture_complete = false;

    img_mouse_right_captured = false;

    mouse_right_firstPress = true;  //reset right-click close polygon mode

    img_mouse_captured = false; //start with mouse capture off

}



//QImage qImg = Mat2QImage(image);
QImage MainWindow::Mat2QImage(const cv::Mat3b &src)
{
    QImage dest(src.cols, src.rows, QImage::Format_ARGB32);
    for (int y = 0; y < src.rows; ++y)
    {
            const cv::Vec3b *srcrow = src[y];
            QRgb *destrow = (QRgb*)dest.scanLine(y);
            for (int x = 0; x < src.cols; ++x)
            {
                    destrow[x] = qRgba(srcrow[x][2], srcrow[x][1], srcrow[x][0], 255);
            }
    }
    return dest;
}

void MainWindow::draw_region_lines(cv::Mat &img)
{
    for(int i=0;i<=mouse_capture_count;i++)
    {
        Point center = img_mousePts_captured[i];
        cocv->drawPoint(&img, center, cv::Scalar(0,0,255));

        if(i>0)
        {
            Point pt2 = img_mousePts_captured[i-1];
            cocv->drawLine(img,center,pt2,cv::Scalar(0,255,0));
        }
    }

}

void MainWindow::draw_rubber_band_lines(cv::Mat &img)
{
    if(rubber_band_on)
    {
        if(mouse_capture_count >= 0)
        {
            //draw rubber-band from last point to where mouse currently is
            Point last_pt = img_mousePts_captured[mouse_capture_count];
            cocv->drawLine(img,img_mouse_pos,last_pt,cv::Scalar(0,0,255));
        }
    }

}

void MainWindow::draw_image_region_lines(cv::Mat img)
{
    draw_region_lines(img);

    //convert image to QImage so we can display normally

    QImage qImg = Mat2QImage(img);
    ui->label_imageShow->setPixmap(QPixmap::fromImage(qImg));

    Rect rect;
    get_rect_from_region(rect);

}

void MainWindow::get_rect_from_region(Rect &rect)
{
    int left=99999,bottom=99999,right=0,top=0,height,width;

    for(int i=0;i<=mouse_capture_count;i++)
    {
        Point pt = img_mousePts_captured[i];
        int x = pt.x;
        int y = pt.y;

        if(left>x)
        {
            left = x;
        }
        if(bottom>y)
        {
            bottom = y;
        }
        if(right<x)
        {
            right = x;
        }
        if(top<y)
        {
            top = y;
        }

    }

    width = right - left;
    height = top - bottom;

    rect = Rect(left,bottom,width,height);

    selection_rect = rect;  //save it for processing

    //zoom-in while preserving aspect ratio
    //expand_rect_to_preserve_aspectRatio(rect);


}

void MainWindow::expand_rect_to_preserve_aspectRatio(Rect &rect)
{
    double desired_aspectRatio = 640.0/480.0;

    const int MAX_WIDTH = 640;
    const int MAX_HEIGHT = 480;

    int width, height, dx, dy;

    //see if we need to expand width or height
    double aspectRatio = (double)rect.width/(double)rect.height;

    if(aspectRatio < desired_aspectRatio)
    {
        height = rect.height;

        //need to expand width
        //compute correct size expansion
        double exp_factor = desired_aspectRatio/aspectRatio;

        width = (double)rect.width * exp_factor;

        dx = width - rect.width;

        dx = dx/2;

        rect.x = rect.x - dx;
        if(rect.x < 0)
            rect.x = 0;

        int max_x = rect.x + width;

        if(max_x < MAX_WIDTH)
        {
            rect.width = max_x;
        }
        else
        {
            MAX_WIDTH - rect.y;
        }

    }
    else
    {
        width = rect.width;

        //need to expand height
        double exp_factor = aspectRatio/desired_aspectRatio;

        int height = (double)rect.width * exp_factor;

        dy = height - rect.height;
        dy = dy/2;

        rect.y = rect.y - dy;

        if(rect.y < 0)
            rect.y = 0;

        int max_y = rect.y + height;

        if(max_y < MAX_HEIGHT)
        {
            rect.height = max_y;
        }
        else
        {
            rect.height = MAX_HEIGHT - rect.y;
        }

    }

    double new_aspectRatio = (double)rect.width/(double)rect.height;

    double z = new_aspectRatio;

}

//modify input image to reflect current zoom and pan settings
//output will be a 640x480 image
void MainWindow::process_zoomPan(Mat &img)
{
  Mat imageROI;

    Point output_imageSize;
    output_imageSize.x = DISPLAY_PIXEL_WIDTH;
    output_imageSize.y = DISPLAY_PIXEL_HEIGHT;

    cocv->process_zoomPan(img, zoom_factor, pan_topLeft, output_imageSize);

    zoom_image = img.clone();
    processing_image = img.clone();
}

void MainWindow::load_image_from_file(QString filePathName)
{
    //need to test ext to make sure it's png or jpg (valid image extension
    QList<QString> ext_list;
    ext_list<<"bmp"<<"jpg"<<"png";
    QFileInfo fi(filePathName);
    QString ext = fi.suffix();
    if (ext_list.contains(ext))
    {
        read_cvImage(image_copy, filePathName);  //so, we're going to read this into image_copy; how to let process know to use it?

        bProcessImage = true;

        //ui->action_Process_Image_as_Live->setChecked(true);
        action_Process_Image_as_Live = true; //replaces action menu check-box

        //ui->action_Process_Image_as_Live->setEnabled(true);


        if(!loopTimerStarted)
        {
            loopTimerStarted = true;
            startTimer(10); //want to be able to treat image as live-video; question will be how to switch to live video from still image...?
        }
    }
    else
    {
        QMessageBox msgBox;
        msgBox.critical(0, "Error", "You must select a valid image file (ensure extension is {*.png, *.jpg, *.bmp}");
        return;
    }

}

void MainWindow::load_image()
{
    //cclass->test5();
    //cclass->test4();    //chk 3 color classifier
    //cclass->test6();  //quad six-color_image and save
//    cclass->test7();    //mlp example
    //cclass->build_classifier();
//return;

    bLiveVideo = false;
    //ui->action_Live_Video->setChecked(false);
    action_Live_Video = false;  //replace action menu checkbox

    QString pics_path = QStandardPaths::locate(QStandardPaths::PicturesLocation, QString(), QStandardPaths::LocateDirectory);

    QFileDialog qfd;

    qfd.setDirectory(pics_path);

    //get save path
    QString filePathName = qfd.getOpenFileName(this,tr("Open Image"),"",tr("Image (*.png);Image (*.jpg);All Files (*)"));
    //QString filePathName = qfd.getSaveFileName(this, tr("Save Image"), "", tr("Image (*.png);Image (*.jpg);All Files (*)"));


    if (filePathName.isEmpty())
        return;

    load_image_from_file(filePathName);
}

void MainWindow::write_cvImage(cv::Mat &img, QString filePathName)
{
    QByteArray ba = filePathName.toLatin1();

    char* imageName = ba.data();

    int nrows = img.rows;

    if(nrows > 0)
        imwrite( imageName, img );
    else
    {
        QMessageBox msgBox;
        msgBox.critical(0, "Error", "Error writing Image-File; Empty picture... ");
        return;
    }

}

void MainWindow::read_cvImage(cv::Mat &img, QString filePathName)
{
    QByteArray ba = filePathName.toLatin1();

    char* imageName = ba.data();

    //Mat img;
    img = imread( imageName, 1 );

}


//should only be here if region has been captured and confirmed via ok on dialog box
void MainWindow::process_capture_region(Mat &img)
{
/*
    run_histogram = true;

    vector<Point> captured_pts;
    int npts = mouse_capture_count;

    for(int i=0;i<npts;i++)
    {
        captured_pts.push_back(img_mousePts_captured[i]);
    }

//test ok
//    imshow("test image in MainWin",img);
//return;

    //really should add a bool ok here
    //selection_rect
    cclass->add_trainingData_to_dataSet(img, captured_pts, cur_classId, run_histogram, selection_rect);

    //auto reload dialog box
    cur_classId++;
    on_actionCreate_Classifier_triggered();
*/
}



#define Main_Timer_Loop_Start {
void MainWindow::timerEvent(QTimerEvent *event)
{
    static int frame_cnt = 0;

    cv::Mat image;

    if(ui->checkBox_runVideo->isChecked())
    {
        bLiveVideo = true;

        if(first_time_video)
        {
            first_time_video = false;
            init_Video();
            return; //return to give it some time to startup
        }
    }
    else
    {
        bLiveVideo = false;
    }


    //test is we're want live video
    if(!bLiveVideo)
    {
        if(!bProcessImage)  //process still image
        {
            return;
        }
    }

    myTimer.start();



//capture usb camera image
    if(bLiveVideo)
    {
        mCapture >> image;

        //chk if valid image

        int nrows = image.rows;
        int ncols = image.cols;
        int nchans = image.channels();

        if(nrows!=480)
        {
            return;
        }
        else if(ncols != 640)
        {
            return;
        }
        else if(nchans != 3)
        {
            return;
        }

    }
    else if(bProcessImage)
        image = image_copy.clone();



    //debate is should this come first or after previous image processing; like CLAHE histogram stretch...
    //for now after

     //modify image to reflect current zoom and pan settings
     process_zoomPan(image);  //this makes a copy "zoom_image" case we want to create a classifier from it that is free from point capture lines and painted pts


     //save live-Video
     if(bSaveVideo)
     {
        //save frame to file
        video_writer.write(image);
     }



    num_objects_found = 0;
    num_pixels_classified = 0;

    if( bProcessPixels)  //process image check_box
    {
/*
        //chk if we should be in HSV space
        if(bUseHSV)
        {
            process_image_HSV(image);
        }

        if(bUseBayes)
        {
            // use Bayes classifier, if it's built, loaded and ready to use
            if(classifier_ready_for_use)
            {
                use_classifier(image);
            }
            else
            {
                //flag error

            }

        }
*/
    }


    //save a copy case we want to save it
    if(bLiveVideo)
        image_copy = image.clone();



    //if(createClassifier_start)
    if(captureRegion_start)
    {
        //draw lines and points for current selector lines
        draw_region_lines(image);

        //draw rubber-band
        draw_rubber_band_lines(image);

        if(img_mouse_captured)
        {
            img_mouse_captured = false; //turn it back off


            if(img_mouse_right_captured)
            {
                img_mouse_right_captured = false;

                //we're done; turn this off
                rubber_band_on = false;
                mouse_capture_complete = true;

                img_mousePts_captured[++mouse_capture_count] = img_mousePts_captured[0];

                Point pt1,pt2;
                if(mouse_capture_count > 2)
                {
                    //use clean copy of image for final drawing
                    draw_image_region_lines(zoom_image);

                    //call next stage; i.e. build this region into trainingSet
                    //check if we really want to add this region,  via dialog box
                    //if so then;

                    //was what we were doing in Plastic Detection Program
                    //process_classifier_capture_region(processing_image);  //need to use zoom_image (copy), so it doesn't have painted lines and points in image
                                                                    //zoom_image created by pan_zoom
                    //now call process capture region
                    process_capture_region(processing_image);  //need to use zoom_image (copy), so it doesn't have painted lines and points in image

                }
                else
                {
                    //skip processing and restart
                    //auto reload dialog box
                    //cur_classId++;
                    //on_actionCreate_Classifier_triggered();
                }

            }
            else
            {
                if(mouse_capture_complete == false)
                {
                    img_mousePts_captured[++mouse_capture_count] = img_mouse_lastCaptured;
                }
            }

            if(mouse_capture_count > 19)
            {
                QMessageBox msgBox;
                msgBox.critical(0, "Error", "Error to large a polygon; please restart... ");

                mouse_capture_count=0;
                return;

            }

        }

    }


    //convert image to QImage so we can display normally
    QImage qImg = Mat2QImage(image);


    ui->label_imageShow->setPixmap(QPixmap::fromImage(qImg));


     if(num_pixels_classified > 0)
    {
        //QString msg = "$" + QString::number(num_objects_found) + "," + QString::number(num_pixels_classified) + "\n";
        //sendSerialMsg(msg);
    }


    int nMilliseconds = myTimer.elapsed();

    skipCount++;
    if(skipCount > 10)
    {
        double fps = 0.0;
        if(nMilliseconds > 0)
        {
            fps = 1000.0/(double)nMilliseconds;
        }

        QString sInfo = "Mouse: (" + QString::number(img_mouse_pos.x) + ", " + QString::number(img_mouse_pos.y)  + "): Capture_time + Processing at (FPS): " + QString::number(fps,'f',1) + "  : Number Objects Found: " + QString::number(num_objects_found);

        ui->label_info->setText(sInfo);
        skipCount = 0;
    }
}

#define Main_Timer_Loop_End }

//Assumes Fiducial is within target rect in center of image
void MainWindow::on_pushButton_Find_Nearest_Fiducial_clicked()
{
    bool fastMove = true;

    //make copy of current image
    Mat image_to_process = image_copy;

    //vector<Vec3f> circles = cocv->get_circles(image_to_process);  //see code inside this func for example on how to extract circle info

    //this would be a better choice as it allows us to screen out circles of wrong size
    float min_size = 5;
    float max_size = 25;

    //current fudicial on Sparkfun board is 11
    vector<Vec3f> circles = cocv->get_circles(image_to_process, min_size, max_size);

    //now use location to move head so we're centered
    Point2f pixel_offset;
    bool fastMov = true;;

    size_t n_circles = circles.size();
    if(n_circles == 1)
    {
        Vec3f center_point = circles[0];
        pixel_offset.x = center_point[0];
        pixel_offset.y = center_point[1];

        //check if reasonable move size

        //get gcode line to move-to
        QString gcode_move_line = cfc->get_gcode_moveTo(pixel_offset, fastMove);

        //run move


    }
    else
    {
        //flag error didn't find it
    }



}


void MainWindow::init_Video()
{
    /// *******************************************
    ///   IMPORTANT; USE USB 3.0 INPUT EVEN FOR USB 2.0 CAMERAS!
    ///              otherwise you'll get bad frames periodically
    /// *******************************************

    //open USB Camera
    if( !mCapture.isOpened() )
        if( !mCapture.open( 0 ) )
            return;

    //set resolution here
    //cap.set(CV_CAP_PROP_FRAME_WIDTH,640);
    //cap.set(CV_CAP_PROP_FRAME_HEIGHT,480);

    bSaveVideoFileNameLoaded = false;

    bLiveVideo = true;
    //ui->action_Live_Video->setChecked(true);
    ui->checkBox_runVideo->setChecked(true);

    bProcessImage = false;
    //ui->action_Process_Image_as_Live->setChecked(false);

    if(!loopTimerStarted)
    {
        loopTimerStarted = true;
        startTimer(10);  //set for 50ms timer rate
    }
}


void MainWindow::save_Video()
{

    if((!bSaveVideo)&&(bLiveVideo))
    {
        //need to pause live video so we can get reasonable access to the gui
        bLiveVideo = false;

        int ex = static_cast<int>(mCapture.get(CV_CAP_PROP_FOURCC));
        int codec = CV_FOURCC('M','J','P','G');
        int fps = mCapture.get(CV_CAP_PROP_FPS);

        //Size s = Size((int)mCapture.get(CV_CAP_PROP_FRAME_WIDTH),(int)mCapture.get(CV_CAP_PROP_FRAME_HEIGHT));

        Size s = image_copy.size();

        bool isColor = (image_copy.type() == CV_8UC3);

        //get file for saving
        cUtils cutls;

        QString filePathName = cutls.get_saveFileName();
        //QString filePathName = get_saveFileName();

        //turn video back on
        bLiveVideo = true;

        int n = filePathName.length();

        if(n>0)
        {
            //convert QString to string
            string str_filePathName = cutls.convert_QString_to_stdString(filePathName);


            //string str_filePathName = "./live.avi";
            //string str_filePathName = "/home/cfd0/Pictures/live.avi";

            //bool isOpened = video_writer.open(str_filePathName,ex,fps,s,true);
            bool isOpened = video_writer.open(str_filePathName,codec,fps,s,isColor);

            if(isOpened)
            {
                bSaveVideo = true;
                bSaveVideoFileNameLoaded = true;
                sSaveVideo_FileName = filePathName;
            }
            else
            {
                //raise error; couldn't open Video-Save File
                QMessageBox msgBox;
                msgBox.critical(0, "Error", "Couldn't Open Video-Save File; saving video disabled; please try again");
                bSaveVideo = false;
                //turn off menu check-box
                //ui->actionSave_Video->setChecked(false);

            }
        }
        else
        {
            //raise error; couldn't open Video-Save File
            QMessageBox msgBox;
            msgBox.critical(0, "Error", "Couldn't Open Video-Save File; saving video disabled; please try again");
            bSaveVideo = false;
            //turn off menu check-box
            //ui->actionSave_Video->setChecked(false);
        }

    }
    else
    {
        if(bSaveVideo)  //if true; means we've been recording
        {
            bSaveVideo = false;

           //save frame to file
           video_writer.release();  //let it go so it'll close the file out
        }
    }

}


void MainWindow::save_image()
{
    //check if we have a valid image (i.e. camera is capturing
    if( !mCapture.isOpened() )
    {
        QMessageBox::information(this, tr("Error"),tr("Unable to save Image File; Camera needs to be Started"));
        return;
    }

    //check if live video is off
    if (bLiveVideo)
    {
        QMessageBox::information(this, tr("Error"),tr("Unable to save Image File while Video is running"));
        return;
    }



    QString homeLocation = QStandardPaths::locate(QStandardPaths::HomeLocation, QString(), QStandardPaths::LocateDirectory);
    QString pics_path = QStandardPaths::locate(QStandardPaths::PicturesLocation, QString(), QStandardPaths::LocateDirectory);

    QFileDialog qfd;

    qfd.setDirectory(pics_path);

    //get save path
    QString filePathName = qfd.getSaveFileName(this, tr("Save Image"), "", tr("Image (*.png);Image (*.jpg);All Files (*)"));


    if (filePathName.isEmpty())
        return;
    else
    {
        //need to test ext to make sure it's png or jpg (valid image extension
        QList<QString> ext_list;
        ext_list<<"bmp"<<"jpg"<<"png";
        QFileInfo fi(filePathName);
        QString ext = fi.suffix();
        if (ext_list.contains(ext))
        {
            //will write pic type based on provided extension?
            write_cvImage(image_copy, filePathName);
        }
        else
        {
            QMessageBox msgBox;
            msgBox.critical(0, "Error", "You must select a valid image file (ensure extension is {*.png, *.jpg, *.bmp}");
            return;
        }

    }

}

//need a way to start, periodically check to see if done and then recognize it is done and stop this (also time-out function)
void MainWindow::run_gcode_line(QString gcode_line, int com_id)
{
    //send gcode line to GRBL controller on COM_x
    if(com_id == 1)
    {
        cur_move_com_id = com_id;
        bGcodeMove_inProgress_com1 = true;
        sendSerialMsg(gcode_line,1);

        //start gcode callback timer so it'll keep checking to see if we're there and line has completed
        gCode_move_timeOut_count_com1 = 0;
        gcodeTimer_com1->start(ms_timeOut_inteval);
    }
    else if(com_id == 2) //use 2nd comm port
    {
        cur_move_com_id = com_id;
        bGcodeMove_inProgress_com1 = true;
        sendSerialMsg(gcode_line,2);

        //start gcode callback timer so it'll keep checking to see if we're there and line has completed
        gCode_move_timeOut_count_com2 = 0;
        gcodeTimer_com2->start(ms_timeOut_inteval);
    }
    else if(com_id == 3) //use 3rd comm port
    {
        cur_move_com_id = com_id;
        bGcodeMove_inProgress_com1 = true;
        sendSerialMsg(gcode_line,2);

        //start gcode callback timer so it'll keep checking to see if we're there and line has completed
        gCode_move_timeOut_count_com3 = 0;
        gcodeTimer_com3->start(ms_timeOut_inteval);
    }

}

void MainWindow::gcode_timer_callback_com1()
{
    //need to keep check with GRBL to see if we've arrived; means we need to send '?'

    //check if we're over time (never arrived...)
    gCode_move_timeOut_count_com1++;
    if(gCode_move_timeOut_count_com1>20)
    {
        //flag error and stop
        //raise error; couldn't open Video-Save File
        QMessageBox msgBox;
        msgBox.critical(0, "Error", "Timeout running GCode in gcode_timer_callback_com1");

        bGcodeMove_inProgress_com1 = false;
        gcodeTimer_com1->stop();
    }

    //if done
    bool still_running = cgc1->cmd_run_status;
    if(!still_running)
    {
        bGcodeMove_inProgress_com1 = false;
        gcodeTimer_com1->stop();
    }
}

void MainWindow::gcode_timer_callback_com2()
{
    //need to keep check with GRBL to see if we've arrived

    //check if we're over time (never arrived...)
    gCode_move_timeOut_count_com2++;
    if(gCode_move_timeOut_count_com2>20)
    {
        //flag error and stop
        //raise error; couldn't open Video-Save File
        QMessageBox msgBox;
        msgBox.critical(0, "Error", "Timeout running GCode in gcode_timer_callback_com1");

        bGcodeMove_inProgress_com2 = false;
        gcodeTimer_com2->stop();
    }

    //if done
    bool still_running = cgc2->cmd_run_status;
    if(!still_running)
    {
        bGcodeMove_inProgress_com2 = false;
        gcodeTimer_com2->stop();
    }
}

void MainWindow::gcode_timer_callback_com3()
{
    //need to keep check with GRBL to see if we've arrived

    //check if we're over time (never arrived...)
    gCode_move_timeOut_count_com3++;
    if(gCode_move_timeOut_count_com3>20)
    {
        //flag error and stop
        //raise error; couldn't open Video-Save File
        QMessageBox msgBox;
        msgBox.critical(0, "Error", "Timeout running GCode in gcode_timer_callback_com1");

        bGcodeMove_inProgress_com3 = false;
        gcodeTimer_com3->stop();
    }

    //if done
    bool still_running = cgc3->cmd_run_status;
    if(!still_running)
    {
        bGcodeMove_inProgress_com3 = false;
        gcodeTimer_com3->stop();
    }
 }


//gets the list of all the gcode lines for the series of moves that comprise an operation; like pick or place or move to feeder ect
QStringList MainWindow::get_gcode_lines_next_operation()
{
    QString gcode_line1 = "G0 X200 Y100";
    QString gcode_line2 = "G0 Z100";
    QString gcode_line3 = "G4 U500"; //dwell (pause) for 500ms (might be X?)
    QString gcode_line4 = "G0 Z-100";

    QStringList gcode_line_list;

    gcode_line_list.append(gcode_line1);
    gcode_line_list.append(gcode_line2);
    gcode_line_list.append(gcode_line3);
    gcode_line_list.append(gcode_line4);

    return gcode_line_list;
}


//chks if we have any lines left to run; if so, loads it and fires it off
bool MainWindow::process_gcode_lines()
{
    bool ok = false;;

    gcode_run_line_indx++;

    int N = gcode_lines_to_run.count();

    if(gcode_run_line_indx <= N)
    {
        ok = true;

        //get next line to run
        QString gcode_line = gcode_lines_to_run[gcode_run_line_indx];

        //fire off gcode_line
        int com_id = 1;
        run_gcode_line(gcode_line, com_id);
    }
    else
    {
        ok = false;

        //we're done; update state
        GCODE_PROCESS_STATE process_state = static_cast<GCODE_PROCESS_STATE>(static_cast<int>(gcode_process_state)+1);
        gcode_process_state = process_state;
    }

    return ok;
}

//loads file that includes list of boards to build (filenames) and their location/s on the machine
//when this func is done; all the boards have been loaded
void MainWindow::load_job(QString jobFileName)
{
    cUtils cutils;

    //read in job from file;  //has filenames for {feeders, nozzles, currently_loaded_nozzle_list}
    QList<QStringList> board_info_list = cutils.read_csv_file(jobFileName);

    int N = board_info_list.count();

    if(N < 1)
    {
        //send er msg; invalid Job File
        QMessageBox msgBox;
        msgBox.critical(0, "Error", "Invalid Job File, please select another Job-File");

        return;
    }

    //load board fileNames and locations
    for(int i=0;i<N;i++)
    {
        cBoard cbd;
        QStringList sBoard_info = board_info_list[i];
        int M = sBoard_info.length();

        if(M>=4)
        {
            QString bd_fName = sBoard_info[0];
            QString sX_loc = sBoard_info[1];
            QString sY_loc = sBoard_info[2];
            QString sRot = sBoard_info[3];

            Point2f bd_loc;

            int L = bd_fName.count();

            if(L>1)
            {
                try
                {
                    float rot = sRot.toFloat();
                    float x = sX_loc.toFloat();
                    float y = sY_loc.toFloat();
                    bd_loc.x = x;
                    bd_loc.y = y;


                    //add to list
                    cbd.board_fileName = bd_fName;
                    cbd.board_id = i;
                    cbd.board_location = bd_loc;
                    cbd.board_rotation = rot;


                    //add it to list
                    board_list.push_back(cbd);


                }
                catch(const char* e)
                {
                    //exception in e
                }
            }


        }
    }

    //load_partValues();  //loaded during load_feeders()
    load_packageTypes(sPackageTypes_fileName);
    load_partTypes(sPartTypes_fileName);
    load_feederTypes(sFeederTypes_fileName);
    load_nozzleTypes(sNozzleTypes_fileName);

    //create boards list

    //load feeders from file

    //load nozzles from file
}



bool MainWindow::is_valid_package_type(QString sPackageType)
{
    cUtils cutils;

    bool ok = cutils.test_exact_stringMatch_in_list(sPackageType, sPackageTypes_list);

    return ok;
}

bool MainWindow::is_valid_part_value(QString sPartValue)
{
    cUtils cutils;

    bool ok = cutils.test_exact_stringMatch_in_list(sPartValue, sPartValues_list);

    return ok;
}

bool MainWindow::is_valid_part_type(QString sPartType)
{
    cUtils cutils;

    bool ok = cutils.test_exact_stringMatch_in_list(sPartType, sPartTypes_list);

    return ok;
}

bool MainWindow::is_valid_feeder_type(QString sFeederType)
{
    cUtils cutils;

    bool ok = cutils.test_exact_stringMatch_in_list(sFeederType, sFeederTypes_list);

    return ok;
}



void MainWindow::load_packageTypes(QString fileName)
{
    cUtils cutils;

    sPackageTypes_list.clear();
    QString sfileName = job_directory + fileName;

    QList<QStringList> info_list = cutils.read_csv_file(sfileName);

    int N = info_list.count();

    if(N>=1)
    {
        for(int i=0;i<N;i++)
        {
            QStringList str_list = info_list[i];
            int cnt = str_list.count();
            if(cnt > 0)
            {
                sPackageTypes_list.append(str_list);
            }
        }
    }
    else
    {
        //flag error
        QMessageBox msgBox;
        msgBox.critical(0, "Error", "Invalid PART_TYPES File, please fix");

        return;
    }

}

void MainWindow::load_partTypes(QString fileName)
{
    cUtils cutils;

    sPartTypes_list.clear();
    QString sfileName = job_directory + fileName;

    QList<QStringList> info_list = cutils.read_csv_file(sfileName);

    int N = info_list.count();

    if(N>=1)
    {
        for(int i=0;i<N;i++)
        {
            QStringList str_list = info_list[i];
            int cnt = str_list.count();
            if(cnt > 0)
            {
                sPartTypes_list.append(str_list);
            }
        }
    }
    else
    {
        //flag error
        QMessageBox msgBox;
        msgBox.critical(0, "Error", "Invalid PART_TYPES File, please fix");

        return;
    }

}

//partValues loaded implicitly with load_feeders (builds our parts into system;
//then load boards chks the parts on board against what's loaded in feeders
//void MainWindow::load_partValues()


void MainWindow::load_feederTypes(QString fileName)
{
    cUtils cutils;

    sFeederTypes_list.clear();

    QString sfileName = job_directory + fileName;

    QList<QStringList> info_list = cutils.read_csv_file(sfileName);

    int N = info_list.count();

    if(N>=1)
    {
        for(int i=0;i<N;i++)
        {
            QStringList str_list = info_list[i];
            int cnt = str_list.count();
            if(cnt > 0)
            {
                sFeederTypes_list.append(str_list);
            }
        }
    }
    else
    {
        //flag error
        QMessageBox msgBox;
        msgBox.critical(0, "Error", "Invalid FEEDER_TYPES File, please fix");

        return;
    }

}

void MainWindow::load_nozzleTypes(QString fileName)
{
    cUtils cutils;

    sNozzleTypes_list.clear();

    QString sfileName = job_directory + fileName;

    QList<QStringList> info_list = cutils.read_csv_file(sfileName);

    int N = info_list.count();

    if(N>=1)
    {
        for(int i=0;i<N;i++)
        {
            QStringList str_list = info_list[i];
            int cnt = str_list.count();
            if(cnt > 0)
            {
                sNozzleTypes_list.append(str_list);
            }
        }
    }
    else
    {
        //flag error
        QMessageBox msgBox;
        msgBox.critical(0, "Error", "Invalid NOZZLE_TYPES File, please fix");

        return;
    }

}


//each QStringList line holds all info for a single feeder
bool MainWindow::load_feeder(QStringList feeder_info_list, cFeeder &feederDefs)
{

    bool ok = false;

    int N = feeder_info_list.count();
    if(N < 8)
    {
        //send er msg; invalid Job File
        QMessageBox msgBox;
        msgBox.critical(0, "Error", "Invalid Feeder File, please select another Feeder-File");

        return ok;
    }

    try
    {
        //extract data from QStringList and load into cFeeder::feederDefs

        //Feeder_id,Feeder_loc_id,Feeder_type,qty,part_package,part_name,part_value,orientation,locX,locY
        //1,1,STRIP,50,C0805,CAP,0.01uF,90

        QString sFeederId = feeder_info_list[0];
        QString sFeederLocId = feeder_info_list[1];
        QString sFeederType = feeder_info_list[2];
        QString sFeederQty = feeder_info_list[3];
        QString sPackageType = feeder_info_list[4];
        QString sPartType = feeder_info_list[5];
        QString sPartValue = feeder_info_list[6];
        QString sRotation = feeder_info_list[7];

        QString sLocX="0.0", sLocY="0.0";

        //SHOULD TEST HERE THAT WE HAVE A VALID FEEDER_TYPE, PACKAGE_TYPE, PACKAGE_VALUE
        ok = is_valid_package_type(sPackageType);
        if(!ok)
        {
            QMessageBox msgBox;
            msgBox.critical(0, "Error", "Invalid Feeder File, bad Package_Type");

            return ok;
        }

        //don't chk is part is good as this is where we bring it in
        int cnt = sPartValue.count();
        if(cnt > 0)
        {
           //add partValue to our list
            sPartValues_list.append(sPartValue);
        }
        else
        {
            QMessageBox msgBox;
            msgBox.critical(0, "Error", "Invalid Feeder File, bad Part_Value");

            return ok;
        }

        ok = is_valid_part_type(sPartType);
        if(!ok)
        {
            QMessageBox msgBox;
            msgBox.critical(0, "Error", "Invalid Feeder File, bad Part_Type");

            return ok;
        }

        ok = is_valid_feeder_type(sFeederType);
        if(!ok)
        {
            QMessageBox msgBox;
            msgBox.critical(0, "Error", "Invalid Feeder File, bad Feeder_Type");

            return ok;
        }

        if(N>=10)
        {
            sLocX = feeder_info_list[8];
            sLocX = feeder_info_list[9];
        }

        int feederId = sFeederId.toInt();
        int feederLocId = sFeederLocId.toInt();
        int qty = sFeederQty.toInt();
        int rotation = sRotation.toInt();
        float locX = sLocX.toFloat();
        float locY = sLocY.toFloat();


        feederDefs.feeder_id = feederId;
        feederDefs.location_id = feederLocId;
        feederDefs.sFeederType = sFeederType;
        feederDefs.num_parts_left = qty;
        feederDefs.package.sPackageName = sPackageType;
        feederDefs.sPackage = sPackageType;
        feederDefs.sPartType = sPartType;
        feederDefs.sPartValue = sPartValue;
        feederDefs.rotation_angle = rotation;
        feederDefs.location.x = locX;
        feederDefs.location.y = locY;

    }
    catch(const char* e)
    {
        //exception in e
        QMessageBox msgBox;
        msgBox.critical(0, "Error", "Invalid Feeder File");

        ok = false;
        return ok;
    }

    return ok;
}

void MainWindow::load_feeders(QString feeder_definitions_fileName)
{
    sPartValues_list.clear();  //clear out all parts, only want parts that are actually loaded in feeders
    feeder_list.clear(); //clear out feeder_list so we only have what's in the list we're loading

    cUtils cutils;
    //each QStringList line holds all info for a single feeder
    //# of lines equal to # of feeders
    QList<QStringList> all_feeders_info_list = cutils.read_csv_file(feeder_definitions_fileName);

    int N = all_feeders_info_list.count();
    if(N < 1)
    {
        //send er msg; invalid Job File
        QMessageBox msgBox;
        msgBox.critical(0, "Error", "Invalid Feeders File, please select another Job-File");

        return;
    }

    for(int i=0;i<N;i++)
    {
        //parse and extract each nozzle info
        cFeeder cfeeder;

        QStringList feeder_info_list = all_feeders_info_list[i];

        //extract info for each nozzle from each QStringList (each list has all info for 1 nozzle)
        load_feeder(feeder_info_list, cfeeder);

        feeder_list.push_back(cfeeder);
    }

}


//QStringList line will have all req'd info for a single nozzle
void MainWindow::load_nozzle(QStringList nozzle_info_list, cNozzle &nozzleDefs)
{
    //extract info for each nozzle from each QStringList (each list has all info for 1 nozzle)

    //nozzle_loc_id, nozzle_type, z_height, n_packages_it_can_place
    //1, N503, -1.10, C0603, R0603, L0603, C0805, R0805, L0805

    int N = nozzle_info_list.count();

    if(N >= 4)
    {
        QString sNozzle_loc_id = nozzle_info_list[0];
        QString sNozzleType = nozzle_info_list[1];
        QString sZHeight = nozzle_info_list[2];

        sNozzle_loc_id = sNozzle_loc_id.trimmed();
        sNozzleType = sNozzleType.trimmed();
        sZHeight = sZHeight.trimmed();

        try
        {
            int nozzle_loc_id = sNozzle_loc_id.toInt();
            double z_height = sZHeight.toDouble();

            nozzleDefs.nozzle_loc_id = nozzle_loc_id;
            nozzleDefs.zHeight = z_height;
            nozzleDefs.sNozzleType = sNozzleType;

            QStringList sCompatiblePackages;

            for(int i=3;i<N;i++)
            {
                QString sPackage = nozzle_info_list[i];
                int cnt = sPackage.count();
                if(cnt > 0)
                {
                    sCompatiblePackages.append(sPackage);
                }
            }
            nozzleDefs.sPackages = sCompatiblePackages;

        }
        catch(const char* e)
        {
            //exception in e
            QMessageBox msgBox;
            msgBox.critical(0, "Error", "Invalid Nozzle File");

            return;
        }

    }
    else
    {
        QMessageBox msgBox;
        msgBox.critical(0, "Error", "Invalid Nozzles File, please select another Nozzle-Defs-File");

        return;
    }

}

//arg passed in is a list of fileNames; where each file holds all defs for a set of feeders
//if we swap out nozzles; we'll need more than 1 file (1 for each set of nozzles)
void MainWindow::load_nozzles(QStringList nozzle_defs_fileNames_list)
{
    //clear out our nozzle_defs_list as we're re-defining the configuration
    nozzles_defs_list.clear();

    cUtils cutils;
    int N = nozzle_defs_fileNames_list.count();
    if(N < 1)
    {
        //send er msg; invalid Job File
        QMessageBox msgBox;
        msgBox.critical(0, "Error", "Invalid Nozzles File, please select another Job-File");

        return;
    }

    for(int i=0;i<N;i++)
    {
        //this list will have 2-6 lines; where each line is a single nozzle def; includes id(which is install location)
        QList<QStringList> all_nozzles_info_list = cutils.read_csv_file(nozzle_defs_fileNames_list[i]);

        int M = all_nozzles_info_list.count();

        //create a nozzle list from each of the nozzle defs classes
        cNozzles cnozzle_list;

        for(int j=0;j<M;j++)
        {
            cNozzle nozzle_defs;
            QStringList nozzle_list = all_nozzles_info_list[j];

            load_nozzle(nozzle_list, nozzle_defs);

            cnozzle_list.nozzle_list.push_back(nozzle_defs);
        }

        //add it to list of nozzles defs list
        nozzles_defs_list.push_back(cnozzle_list);
    }

}

void MainWindow::compile_job()
{
    //using board list; load each board and in combo with feeders, build out a full working job
    QString fileName = job_directory + board_defs_fileName;

    load_boards(fileName);

    //bds are now at a point where each part has been assigned to a feeder and a nozzle
    //there is also a mark in each part that tells if we need to stop loading nozzles and run place op

}

bool MainWindow::find_compatible_feeders(cPart part, vector<int> &compatibleFeeders_list)
{
    bool found = false;

    //need to iterate through all the feeders and build list of compatibleFeeders for this part
    int N = static_cast<int>(feeder_list.size());

    for(int i=0;i<N;i++)
    {
        cFeeder fdr = feeder_list[i];

        int fdr_id = fdr.feeder_id;

        QString sPackage_part = part.sPackage;
        QString sPartType_part = part.part_type;
        QString sPartValue_part = part.part_value;

        QString sPackage_fdr = fdr.sPackage;
        QString sPartType_fdr = fdr.sPartType;
        QString sPartValue_fdr = fdr.sPartValue;



        if( (sPackage_part == sPackage_fdr) && (sPartType_part == sPartType_fdr) && (sPartValue_part==sPartValue_fdr) )
        {
            found = true;
            compatibleFeeders_list.push_back(fdr_id);
        }

    }
    return found;
}

bool MainWindow::find_feeder(cPart part, int last_feeder_used, int &feeder_Id_to_use)
{
    //need some rules;

    // 1) find list of compatible feeders
    vector<int> compatibleFeeders_list;
    bool found = find_compatible_feeders(part, compatibleFeeders_list);

    if(found)
    {
        int cnt = static_cast<int>(compatibleFeeders_list.size());

        if(cnt == 1)
        {
            feeder_Id_to_use = compatibleFeeders_list[0];
            return found;
        }
        else
        {
            //
            int found_indx = -1;

            //now need to iterate till we pass last_feeder_used; if we loop back around, then use first feeder
            for(int i=0;i<cnt;i++)
            {
                int cfder_id = compatibleFeeders_list[1];

                if(cfder_id == last_feeder_used)
                {
                    found_indx = cfder_id;
                }

                //don't look till we past our last indx used
                if(found_indx > 0)
                {
                    if(cfder_id != last_feeder_used)
                    {
                        feeder_Id_to_use = cfder_id;
                        return found;
                    }
                }
            }

            //if we went past end; then we need to start over
            feeder_Id_to_use = compatibleFeeders_list[0];
            return found;

        }
    }

    return found;
}

bool MainWindow::find_compatible_nozzles(cPart part, vector<int> &compatibleNozzles_list, cNozzles nozzle_list)
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

void MainWindow::init_nozzle_fill_status(QStringList &nozzle_fill_status)
{
    nozzle_fill_status.clear();

    for(int i=0;i<MAX_NUM_NOZZLES;i++)
    {
        nozzle_fill_status << "EMPTY";
    }
}

//start_fresh_fill means go do a place op and then this part will start over filling up nozzles
bool MainWindow::assign_nozzle(cPart part, QStringList &nozzle_fill_status, int &nozzle_Id_to_use, int &nozzle_def_list_to_use, bool &start_fresh_fill)
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
            init_nozzle_fill_status(nozzle_fill_status);

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
            QMessageBox msgBox;
            msgBox.critical(0, "Error", "coding error; shouldn't be here; inside assign nozzle");

            return found;


        }

    }

    return found;
}


void MainWindow::load_board(QString board_fileName, cBoard &board)
{
    //go through board file line by line; linking each item to a feeder and a nozzle (what to do if nozzle isn't loaded?

    //Be sure to check for additional adjacent feeders holding same part so you can move faster to load 2nd, 3rd nozzles
    //with same part (more of an issue with automatic feeders, but good practice, so we're going to do it anyways

    cUtils cutils;

    QList<QStringList> all_board_info_list = cutils.read_csv_file(board_fileName);

    int N = all_board_info_list.count();
    if(N < 2)
    {
        //flag error
        QMessageBox msgBox;
        msgBox.critical(0, "Error", "Invalid Boards_List File, please select another File");

        return;
    }

    int nozzle_id = 1;  //start out at 1
    int last_feeder_used = -1;  //used to scatter out picks across feeders for same valued parts (if possible)

    QStringList nozzle_fill_status;  //holds our current nozzle part_ids assignment
    init_nozzle_fill_status(nozzle_fill_status);

    for(int i=0;i<N;i++)
    {
        QStringList board_line_list = all_board_info_list[i];

        int cnt = board_line_list.count();
        if(cnt < 6)
        {
            //flag error
            QMessageBox msgBox;
            msgBox.critical(0, "Error", "Invalid Boards_List File, please select another File");

            return;
        }

        try
        {
            cPart part;

            QString sPart_ID = board_line_list[0];
            QString sCentroid_X = board_line_list[1];
            QString sCentroid_Y = board_line_list[2];
            QString sRotation = board_line_list[3];
            QString sPartValue = board_line_list[4];
            QString sPackage = board_line_list[5];
            QString sPartType = board_line_list[6];

            float centroid_X = sCentroid_X.toFloat();
            float centroid_Y = sCentroid_X.toFloat();
            float rotation = sRotation.toFloat();

            Point2f centroid;
            centroid.x = centroid_X;
            centroid.y = centroid_Y;

            part.part_Id = sPart_ID;
            part.sPackage = sPackage;
            part.part_type = sPartType;
            part.part_value = sPartValue;
            part.part_centroid = centroid;
            part.part_place_rotation = rotation;

            if(sPartType == "FIDUC")
            {
                board.relative_fiducial_locations.push_back(centroid);
            }

            //now we need to find a feeder for this part and assign a nozzle
            int feeder_Id_to_use;

            bool found = find_feeder(part, last_feeder_used, feeder_Id_to_use);

            if(!found)
            {
                //flag error
                QMessageBox msgBox;
                msgBox.critical(0, "Error", "Couldn't Match Feeder to Part; please check files");

                return;
            }

            part.feeder_id = feeder_Id_to_use;

            //save last feeder used
            last_feeder_used = feeder_Id_to_use;


            //Now need to assign this part to a nozzle
            int nozzle_Id_to_use;
            bool start_fresh_fill;
            int  nozzle_def_list_to_use;

            bool bAssigned_ok = assign_nozzle(part, nozzle_fill_status, nozzle_Id_to_use, nozzle_def_list_to_use, start_fresh_fill);

            if(!bAssigned_ok)
            {
                //flag error
                QMessageBox msgBox;
                msgBox.critical(0, "Error", "Couldn't Assign Nozzle...");

                return;

            }

            part.start_fresh_nozzle_fill = start_fresh_fill;
            part.nozzle_id = nozzle_Id_to_use;
            part.nozzle_def_list_to_use = nozzle_def_list_to_use;

            //get nozzleType (used for sorting cParts in a board by nozzle-size)
            cNozzles cNozzle_list = nozzles_defs_list[nozzle_def_list_to_use];
            cNozzle nozzle = cNozzle_list.nozzle_list[nozzle_Id_to_use];
            QString sNozzleType = nozzle.sNozzleType;
            part.sNozzleType = sNozzleType;


            //add part to board's part_list
            board.part_list.push_back(part);
        }
        catch(const char* e)
        {
            //exception in e
            QMessageBox msgBox;
            msgBox.critical(0, "Error", "Invalid Nozzle File");

            return;
        }

    }


}

void MainWindow::load_boards(QString boards_list_fileName)
{
    board_list.clear();  //clear out all boards,
    cUtils cutils;

    //each QStringList line holds all info for a single board
    //# of lines equal to # of boards
    QList<QStringList> all_boards_info_list = cutils.read_csv_file(boards_list_fileName);

    int N = all_boards_info_list.count();
    if(N < 1)
    {
        //send er msg; invalid Job File
        QMessageBox msgBox;
        msgBox.critical(0, "Error", "Invalid Boards File, please select another File");

        return;
    }

    for(int i=0;i<N;i++)
    {
        //parse and extract each nozzle info
        cBoard cboard;

        QStringList board_info_list = all_boards_info_list[i];

        int P = board_info_list.count();

        if(P<3)
        {
            //flag error
            QMessageBox msgBox;
            msgBox.critical(0, "Error", "Invalid Boards_List File, please select another File");

            return;
        }



        try
        {
            QString sBoard_fileName = board_info_list[0];
            QString sBoard_locMinX = board_info_list[1];
            QString sBoard_locMinY = board_info_list[2];
            QString sBoard_rotation = board_info_list[3];

            float board_locMinX = sBoard_locMinX.toFloat();
            float board_locMinY = sBoard_locMinY.toFloat();
            float board_rotation = sBoard_rotation.toFloat();

            cboard.board_fileName = sBoard_fileName;
            cboard.board_location.x = board_locMinX;
            cboard.board_location.y = board_locMinY;
            cboard.board_rotation = board_rotation;

            //extract info for each nozzle from each QStringList (each list has all info for 1 nozzle)
            load_board(sBoard_fileName, cboard);

            board_list.push_back(cboard);
        }
        catch(const char* e)
        {
            //exception in e
            QMessageBox msgBox;
            msgBox.critical(0, "Error", "Invalid Nozzle File");

            return;
        }


    }

}

void MainWindow::find_fiducials()
{

}

void MainWindow::find_fiducial(Point2f approx_location, Point2f &found_location)
{

}


bool MainWindow::pick_part(Point3f feeder_location)
{

}

bool MainWindow::rotate_part(float target_angle)
{

}

bool MainWindow::upVision_angle_correct()
{

}

bool MainWindow::place_part(int nozzle_id, float part_orientation, float location_orientation, Point3f place_location)
{

}

void MainWindow::pnp_state_machine()
{
/* //GCODE_PROCESS_STATE::
        LOAD_BOARDS,
        FIND_FIDUCIALS,  //for a given board
        LOAD_NOZZLE_x,
        UPVISION_ANGLE_CORRECT_NOZZLE_x,
        PLACE_NOZZLE_x,
        OPERATION_COMPLETE,
        ALL_DONE
 */

    if(gcode_process_state==GCODE_PROCESS_STATE::LOAD_BOARDS)
    {
        //for now skip all this so we can create basic interfacing code

        //load some lines
        gcode_lines_to_run = get_gcode_lines_next_operation();

        GCODE_PROCESS_STATE process_state = static_cast<GCODE_PROCESS_STATE>(static_cast<int>(gcode_process_state)+1);
        gcode_process_state = process_state;
        return;
    }
    else if(gcode_process_state==GCODE_PROCESS_STATE::FIND_FIDUCIALS)
    {
        GCODE_PROCESS_STATE process_state = static_cast<GCODE_PROCESS_STATE>(static_cast<int>(gcode_process_state)+1);
        gcode_process_state = process_state;
        return;

        //do we have another Fiducial to find? if yes; get it

        //move to fiducial neighborhood
        //get image; find fiducial in pixels
        //move to center of fiducial
        //chk if we're good; if so, update location of fiducial

    }
    else if(gcode_process_state==GCODE_PROCESS_STATE::LOAD_NOZZLE_x)
    {
        GCODE_PROCESS_STATE process_state = static_cast<GCODE_PROCESS_STATE>(static_cast<int>(gcode_process_state)+1);
        gcode_process_state = process_state;
        return;

        //do we need to load another nozzle? if so load specifics about nozzle {which feeder, loc of feeder, rotation of part, desired rotation
        //if not; update gcode_process_state to next state

        //move to feeder
        //pick part
        //chk if we should adjust location of feeder (for strip-line feeders)
        //if so; use flying-vision to adjust feeder location


    }
    else if(gcode_process_state==GCODE_PROCESS_STATE::UPVISION_ANGLE_CORRECT_NOZZLE_x)
    {
        GCODE_PROCESS_STATE process_state = static_cast<GCODE_PROCESS_STATE>(static_cast<int>(gcode_process_state)+1);
        gcode_process_state = process_state;
        return;


    }
    else if(gcode_process_state==GCODE_PROCESS_STATE::PLACE_NOZZLE_x)
    {
        GCODE_PROCESS_STATE process_state = static_cast<GCODE_PROCESS_STATE>(static_cast<int>(gcode_process_state)+1);
        gcode_process_state = process_state;
        return;


    }
    else if(gcode_process_state==GCODE_PROCESS_STATE::OPERATION_COMPLETE)
    {
        //clean up
        gcode_lines_to_run.clear();
        gcode_run_line_indx = -1;

        //set back to start
        gcode_process_state=GCODE_PROCESS_STATE::LOAD_NOZZLE_x;
        return;
    }
    else if(gcode_process_state==GCODE_PROCESS_STATE::ALL_DONE)
    {
        return;
    }
    else
    {
        return;
    }



}


void MainWindow::calibrate_feeder()
{

}

//assumes we're positioned over a calibration point on a feeder
//this will open up a dialog that will allow us to select feeder id and which calibration point we want to enter
void MainWindow::on_pushButton_Calibrate_Feeder_clicked()
{

    //open dialog we can select the feeder we're trying to cal
    Dialog_feederCalibrate *dialog_fdr_cal;
    dialog_fdr_cal = new Dialog_feederCalibrate();

    dialog_fdr_cal->exec();

    //extract data from dialog



    calibrate_feeder();

}


/*
 * To Do:
 * 1) figure out how to combine boards into a single master-part list (that keeps board id for placing)
 * so that Parts can be sorted and placed togeather ...
 *
 *
 */
