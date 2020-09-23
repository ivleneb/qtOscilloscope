#include "mainwindow.h"
#include "plot.h"
#include "knob.h"
#include "wheelbox.h"
#include <qwt_scale_engine.h>
#include <qlabel.h>
#include <qlayout.h>
//
#include<QPushButton>
#include<QCheckBox>
#include<QDebug>
#include<QFileDialog>
#include "screenshot.h"

MainWindow::MainWindow( QWidget *parent ):
    QWidget( parent )
{
    const double intervalLength = 10.0; // seconds

    d_plot = new Plot( this );
    d_plot->setIntervalLength( intervalLength );

    d_amplitudeKnob = new Knob( "Amplitude", 0.0, 200.0, this );
    d_amplitudeKnob->setValue( 160.0 );

    d_frequencyKnob = new Knob( "Frequency [Hz]", 0.1, 20.0, this );
    d_frequencyKnob->setValue( 17.8 );

    d_intervalWheel = new WheelBox( "Displayed [s]", 1.0, 100.0, 1.0, this );
    d_intervalWheel->setValue( intervalLength );

    d_timerWheel = new WheelBox( "Sample Interval [ms]", 0.0, 20.0, 0.1, this );
    d_timerWheel->setValue( 10.0 );

    /*extra*/
    btn_stop=new QPushButton("Stop");
    btn_fotoActual=new QPushButton("Foto Actual");
    btn_changeColor=new QPushButton("Change Color");
    btn_save=new QPushButton("Save");
    cb_showLegend=new QCheckBox("Show Legend");
    d_verticalWheel = new WheelBox( "Vertical shift", -100.0, 100.0, 0.1, this );
    //container
    d_container=new QLabel(this);
    map_list=new QListWidget(this);
    map_list->setStyleSheet("background-color:#A6A6A6;color:black;");
    tHeader = new QLabel("Screenshot");
    tHeader->setFont(QFont("FontAwesome"));
    tHeader->setVisible(false);
    currentScreen=new QLabel(this);

    //format extra
    cb_showLegend->setChecked(false);
    btn_stop->setStyleSheet("background-color:blue");
    btn_fotoActual->setStyleSheet("background-color:blue");
    btn_changeColor->setStyleSheet("background-color:blue");
    btn_save->setStyleSheet("background-color:blue");
    d_verticalWheel->setValue( 0 );
    currentScreen->setVisible(false);
    map_list->setVisible(false);
    btn_save->setVisible(false);
    //
    tHeader->setVisible(true);
    map_list->setVisible(true);
    currentScreen->setVisible(true);
    btn_save->setVisible(true);

    QVBoxLayout* vLayout1 = new QVBoxLayout();
    vLayout1->addWidget( d_intervalWheel );
    vLayout1->addWidget( d_timerWheel );
    vLayout1->addStretch( 10 );
    vLayout1->addWidget( d_amplitudeKnob );
    vLayout1->addWidget( d_frequencyKnob );

    QVBoxLayout* vLayout2=new QVBoxLayout();
    vLayout2->addWidget(d_verticalWheel);
    vLayout2->addWidget(btn_stop);
    vLayout2->addWidget(btn_fotoActual);
    vLayout2->addWidget(btn_changeColor);
    vLayout2->addWidget(cb_showLegend);

    QVBoxLayout* vLayout3=new QVBoxLayout();
    QHBoxLayout* hLayout3=new QHBoxLayout();
    hLayout3->addWidget(currentScreen);
    hLayout3->addWidget(btn_save);
    vLayout3->addLayout(hLayout3);
    vLayout3->addWidget(d_container);

    QVBoxLayout* vLayout4=new QVBoxLayout();
    vLayout4->addWidget(tHeader);
    vLayout4->addWidget(map_list);

    QHBoxLayout *layout = new QHBoxLayout( this );
    layout->addWidget( d_plot, 10 );
    layout->addLayout( vLayout1 );
    layout->addLayout(vLayout2);
    layout->addLayout(vLayout4);
    layout->addLayout(vLayout3);

    //d_container->setPixmap(d_plot->grab().scaled(500,500));

    connect( d_amplitudeKnob, SIGNAL( valueChanged( double ) ),
        SIGNAL( amplitudeChanged( double ) ) );
    connect( d_frequencyKnob, SIGNAL( valueChanged( double ) ),
        SIGNAL( frequencyChanged( double ) ) );
    connect( d_timerWheel, SIGNAL( valueChanged( double ) ),
        SIGNAL( signalIntervalChanged( double ) ) );

    connect( d_intervalWheel, SIGNAL( valueChanged( double ) ),
        d_plot, SLOT( setIntervalLength( double ) ) );
    //
    connect(cb_showLegend, SIGNAL(stateChanged(int)), d_plot, SLOT(showLegendWithCheckbox(int)));
    connect(d_amplitudeKnob, SIGNAL(valueChanged(double)),d_plot,SLOT(changeAmplitudeLab(double)));
    connect(d_frequencyKnob, SIGNAL(valueChanged(double)),d_plot,SLOT(changeFrequencyLab(double)));
    connect(btn_changeColor, SIGNAL(clicked()),d_plot,SLOT(changeColorCurve()));
    connect(d_verticalWheel,SIGNAL(valueChanged(double)),d_plot,SLOT(changeVerticalShift(double)));
    connect(btn_stop,SIGNAL(clicked()),this,SLOT(changeStopBtn()));
    connect(btn_stop,SIGNAL(clicked()),d_plot,SLOT(stopUpdatingCurve()));
    //connect(btn_stop,SIGNAL(clicked()),this,SIGNAL(stopSignal()));
    connect(btn_fotoActual,SIGNAL(clicked()),this,SLOT(addScreenshot()));
    connect(map_list,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(updateScreenItem(QListWidgetItem*)));
    connect(btn_save,SIGNAL(clicked()),this,SLOT(saveScreen()));
}

void MainWindow::start()
{
    d_plot->start();
}

double MainWindow::frequency() const
{
    return d_frequencyKnob->value();
}

double MainWindow::amplitude() const
{
    return d_amplitudeKnob->value();
}

double MainWindow::signalInterval() const
{
    return d_timerWheel->value();
}

void MainWindow::changeStopBtn(){
    if(stopped){
        stopped=false;
        btn_stop->setText("Stop");
    } else {
        stopped=true;
        btn_stop->setText("Resume");
    }
}

void MainWindow::addScreenshot(){
    // if 0 start showing
    Screenshot m;
    if(map_count==0){
        /*tHeader->setVisible(true);
        map_list->setVisible(true);
        currentScreen->setVisible(true);
        btn_save->setVisible(true);
        m.map=d_plot->grab().scaled(500,500);
        d_container->setPixmap(m.map);*/
    }
    m.map=d_plot->grab().scaled(410,390/*,Qt::KeepAspectRatio*/);
    m.frequency=d_frequencyKnob->value();
    m.amplitude=d_amplitudeKnob->value();
    QString itemtext="Item "+QString::number(map_count)+"\n";
    m.item_name=itemtext;
    m.item_name+=" Frecuencia:"+QString::number(m.frequency);
    m.item_name+=" Amplitud:"+QString::number(m.amplitude)+"\n";
    map_list->addItem(itemtext);
    currentScreen->setText(m.item_name);
    d_screen[map_count]=m;
    map_count++;
    d_container->setPixmap(m.map);
    m.map=d_plot->grab().scaled(410,390);

}

void MainWindow::updateScreenItem(QListWidgetItem* item){
    QStringList ls=item->text().split(" ");
    int index= ls[1].toInt();
    d_container->setPixmap(d_screen[index].map);
    currentScreen->setText(d_screen[index].item_name);
}

void MainWindow::saveScreen(){
    qDebug()<<"Save screen";
    QStringList ls=currentScreen->text().split("\n");
    QString fileStr=ls[0];
    QString numIndex=fileStr.split(" ")[1];
    qDebug()<<numIndex;
    QString fileName = QFileDialog::getSaveFileName(this, tr("File name"), "C:/Users/user/Desktop/"+fileStr+".png","Graphic files (*png)");
    qDebug()<<fileName;
    if(d_screen[numIndex.toInt()].map.save(fileName,"PNG")){
        qDebug()<<"Saved";
    }else qDebug()<<"Faile save";
}
