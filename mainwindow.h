#include <qwidget.h>
//
#include<QPushButton>
#include<QCheckBox>
#include<QLabel>
#include<QListWidget>
#include<QListWidgetItem>
#include "screenshot.h"
class Plot;
class Knob;
class WheelBox;

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow( QWidget * = NULL );

    void start();

    double amplitude() const;
    double frequency() const;
    double signalInterval() const;

public slots:
    void changeStopBtn(void);
    void addScreenshot();
    void updateScreenItem(QListWidgetItem* item);
    void saveScreen();

Q_SIGNALS:
    void amplitudeChanged( double );
    void frequencyChanged( double );
    void signalIntervalChanged( double );
    void stopSignal();

private:
    Knob *d_frequencyKnob;
    Knob *d_amplitudeKnob;
    WheelBox *d_timerWheel;
    WheelBox *d_intervalWheel;

    Plot *d_plot;
    //extra
    QPushButton* btn_stop;
    QPushButton* btn_fotoActual;
    QPushButton* btn_changeColor;
    QPushButton* btn_save;
    QCheckBox* cb_showLegend;
    WheelBox *d_verticalWheel;
    bool stopped=false;
    QLabel *d_container;
    QListWidget *map_list;
    QLabel *tHeader;
    QLabel *currentScreen;
    //QPixmap map[100];
    Screenshot d_screen[100];
    int map_count=0;
};
