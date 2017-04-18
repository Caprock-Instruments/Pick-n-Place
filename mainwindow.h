#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QString>
#include <QStringList>

#include <QTime>
#include <QTimer>


#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"

#include <iostream>
#include <math.h>
#include <string.h>

#include <QtSerialPort/QtSerialPort>
#include <QSerialPortInfo>


#include "copencv_utils.h"
#include "cgcode.h"
#include "cflyingcamera.h"
#include "cboard.h"
#include "cfeeder.h"

#include "dialog_feedercalibrate.h"


using namespace cv;
using namespace std;


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    enum GCODE_PROCESS_STATE
    {
        LOAD_BOARDS,
        FIND_FIDUCIALS,  //for a given board
        LOAD_NOZZLE_x,
        UPVISION_ANGLE_CORRECT_NOZZLE_x,
        PLACE_NOZZLE_x,
        OPERATION_COMPLETE,
        ALL_DONE
    };



private slots:
    void on_actionOpen_triggered();

    void on_actionClose_triggered();

    void on_actionExit_triggered();

    void on_actionCnc_triggered();

    void on_actionJog_triggered();

    void on_actionTeach_triggered();

    void on_actionSave_triggered();

    void on_actionAbout_triggered();

    void on_actionTutorial_triggered();

    void on_actionManual_triggered();

    void on_pushButton_Up_clicked();

    void initSerialPort();
    void initSerialPort_com2();
    void initSerialPort_com3();

    void serialRead();
    void serialRead_com2();
    void serialRead_com3();

    void loadSettings();
    void saveSettings();

    void Mouse_current_pos();
    void Mouse_Pressed();
    void Mouse_Right_Pressed();
    void Mouse_left();
    void Mouse_Released();
    void Mouse_Right_Released();

    void on_pushButton_Find_Nearest_Fiducial_clicked();

    void gcode_timer_callback_com1();
    void gcode_timer_callback_com2();
    void gcode_timer_callback_com3();

    void on_actionLoad_Feeders_triggered();

    void on_actionCompile_Job_triggered();

    void on_actionLoad_Nozzles_triggered();

    void on_pushButton_Calibrate_Feeder_clicked();

protected:
    void timerEvent(QTimerEvent *event);
    void keyPressEvent( QKeyEvent* event ) ;

private:
    void load_mainWindow_icons();

    void sendSerialMsg(QString msg, int com_id);

    void init_Video();
    void save_Video();
    void save_image();



    //QImage Mat2QImage(const cv::Mat_<double> &src);
    QImage Mat2QImage(const cv::Mat3b &src);


    void draw_image_region_lines(cv::Mat img);

    void draw_region_lines(cv::Mat &img);
    void draw_rubber_band_lines(cv::Mat &img);

    void get_rect_from_region(Rect &rect);
    void expand_rect_to_preserve_aspectRatio(Rect &rect);

    void init_capture();
    void clear_mouse_capture_buffer();

    void load_image();
    void load_image_from_file(QString filePathName);

    void read_cvImage(cv::Mat &img, QString filePathName);
    void write_cvImage(cv::Mat &img, QString filePathName);

    void process_zoomPan(Mat &img);  //modify input image to reflect current zoom and pan settings

    void process_capture_region(Mat &img);

    void run_gcode_line(QString gcode_line, int com_id);

    bool process_gcode_lines();  //chks if we have any lines left to run; if so, loads it and fires it off

    void pnp_state_machine();


    bool is_valid_package_type(QString sPackageType);
    bool is_valid_part_name(QString sPartName);
    bool is_valid_part_value(QString sPartName);
    bool is_valid_part_type(QString sPartName);
    bool is_valid_feeder_type(QString sPartName);

    void load_job(QString jobFileName);

    void load_packageTypes(QString fileName);
    void load_partTypes(QString fileName);
    //void load_partValues(); //done implicitly with load_feeders (that will be chk'd later with load board)
    void load_feederTypes(QString fileName);
    void load_nozzleTypes(QString fileName);


    void load_feeders(QString feeder_definitions_fileName);
    bool load_feeder(QStringList feeder_info_list, cFeeder &feederDefs);


    void load_nozzles(QStringList nozzle_defs_fileNames_list);
    void load_nozzle(QStringList nozzle_info_list, cNozzle &nozzleDefs);

    void load_boards(QString fileName);
    void load_board(QString board_fileName, cBoard &board);

    bool find_feeder(cPart part, int last_feeder_used, int &feeder_Id_to_use);
    bool assign_nozzle(cPart part, QStringList &nozzle_fill_status, int &nozzle_Id_to_use, int &nozzle_list_to_use, bool &start_fresh_fill);
    void init_nozzle_fill_status(QStringList &nozzle_fill_status);

    bool find_compatible_feeders(cPart part, vector<int> &compatibleFeeders_list);
    bool find_compatible_nozzles(cPart part, vector<int> &compatibleNozzles_list, cNozzles nozzle_list);


    void compile_job();  //combines feeder info with board info and nozzle into to build working job

    void calibrate_feeder();

    void find_fiducials();
    void find_fiducial(Point2f approx_location, Point2f &found_location);
    bool pick_part(Point3f feeder_location);
    bool rotate_part(float target_angle);
    bool upVision_angle_correct();
    bool place_part(int nozzle_id, float part_orientation, float location_orientation, Point3f place_location);

    QStringList get_gcode_lines_next_operation();


private:
    Ui::MainWindow *ui;

    QTime myTimer;
    QTimer *gcodeTimer_com1;
    QTimer *gcodeTimer_com2;
    QTimer *gcodeTimer_com3;
    const int ms_timeOut_inteval = 20;  //time to txd update msg is faster than 7ms; + need to allow some reaction time for controller;

    QSerialPort *serial;
    QSerialPort *serial2;
    QSerialPort *serial3;

    QSettings * p_settings;

    cOpenCv_Utils *cocv;
    cGCode *cgc1;
    cGCode *cgc2;
    cGCode *cgc3;
    cFlyingCamera *cfc;

    cv::VideoCapture mCapture;  //standard usb camera
    cv::VideoWriter video_writer;
    cv::Mat image_copy;
    cv::Mat zoom_image;
    cv::Mat processing_image;

    bool bSaveVideo=false;
    bool bSaveVideoFileNameLoaded = false;
    QString sSaveVideo_FileName;

    bool bLiveVideo;
    bool bClearBackground;

    bool first_time;
    bool first_time_video=true;

    bool bProcessImage;
    bool loopTimerStarted;

    bool bProcessPixels;
    bool bObjectProcess;
    bool bUseHSV;


    bool action_Process_Image_as_Live;  //replaces menu action item checkbox
    bool action_Live_Video;             //replaces menu action item checkbox

    bool captureRegion_start = false;
    Point img_mouse_pos;
    Point img_mouse_lastCaptured;
    Point img_mouse_last1Captured;
    Point img_mouse_last2Captured;
    static const int MOUSE_CAPTURE_BUFFER_SIZE = 20;
    Point img_mousePts_captured[MOUSE_CAPTURE_BUFFER_SIZE];
    Rect selection_rect;
    bool img_mouse_captured;
    bool img_mouse_right_captured;
    int mouse_capture_count;
    bool mouse_firstPress = true;
    bool mouse_right_firstPress = true;
    bool rubber_band_on = true;
    bool mouse_capture_complete = false;


    // PAN/ZOOM
    Point pan_center, pan_topLeft;
    int zoom_factor=1;
    bool isZoomed=false;
    bool use_panCenter = false;

    static const int DISPLAY_PIXEL_WIDTH = 640;
    static const int DISPLAY_PIXEL_HEIGHT = 480;


    int skipCount;

    int num_objects_found;
    int num_pixels_classified;

    bool bGcodeMove_inProgress_com1 = false;
    bool bGcodeMove_inProgress_com2 = false;
    bool bGcodeMove_inProgress_com3 = false;
    int gCode_move_timeOut_count_com1;
    int gCode_move_timeOut_count_com2;
    int gCode_move_timeOut_count_com3;
    int cur_move_com_id = 1;

    GCODE_PROCESS_STATE gcode_process_state = GCODE_PROCESS_STATE::LOAD_BOARDS;
    int gcode_run_line_indx = -1;  //when running a pre-filled list of gcode lines; this holds our current index
    QStringList gcode_lines_to_run;

    vector<cBoard> board_list;
    vector<cFeeder> feeder_list;
    vector<cNozzles> nozzles_defs_list;

    const int MAX_NUM_NOZZLES = 4;

    QStringList nozzle_fileNames_list;
\
    vector<QString> board_fileName_list;    //list of fileName/s for each board to build [board_id is implicit in the indx of this list
    QString job_fileName = "job_example1.csv";  //file that includes list of boards to build (filenames) and their location/s on the machine
    QString feeders_fileName = "feeders.csv";  //file that includes all info. for all the feeders and the parts they hold
    QString nozzle_defs1_fileName = "nozzles_config_defs_1.csv";  //file that includes all info. for each of the loaded nozzles and the parts they can handle
    QString nozzle_defs2_fileName = "nozzles_config_defs_2.csv";  //file that includes all info. for each of the loaded nozzles and the parts they can handle
    QString nozzle_defs3_fileName = "nozzles_config_defs_3.csv";  //file that includes all info. for each of the loaded nozzles and the parts they can handle
    QString board_defs_fileName = "boards_list.csv";

    QString job_directory = "/home/cfd0/PnP/job_example1/";

    //don't load sPartValues from file; loading with _load_Feeders()
    //QString sPartValues_fileName = "PART_VALUES.csv";
    QString sPackageTypes_fileName = "PACKAGE_TYPES.csv";
    QString sPartTypes_fileName = "PART_TYPES.csv";
    QString sFeederTypes_fileName = "FEEDER_TYPES.csv";
    QString sNozzleTypes_fileName = "NOZZLE_TYPES.csv";

    QStringList sPackageTypes_list;
    QStringList sPartTypes_list;
    QStringList sPartValues_list;
    QStringList sFeederTypes_list;
    QStringList sNozzleTypes_list;




};

#endif // MAINWINDOW_H
