#include <qwt_plot.h>
#include <qwt_interval.h>
#include <qwt_system_clock.h>

//
#include <qwt_legend.h>
#include<qwt_plot_textlabel.h>

class QwtPlotCurve;
class QwtPlotMarker;
class QwtPlotDirectPainter;

class Plot: public QwtPlot
{
    Q_OBJECT

public:
    Plot( QWidget * = NULL );
    virtual ~Plot();

    void start();
    virtual void replot();

    virtual bool eventFilter( QObject *, QEvent * );

public Q_SLOTS:
    void setIntervalLength( double );
    //
    void showLegendWithCheckbox(int state);
    void changeAmplitudeLab(double val);
    void changeFrequencyLab(double val);
    void changeColorCurve();
    void changeVerticalShift(double val);
    void stopUpdatingCurve();

protected:
    virtual void showEvent( QShowEvent * );
    virtual void resizeEvent( QResizeEvent * );
    virtual void timerEvent( QTimerEvent * );

private:
    void updateCurve();
    void incrementInterval();

    QwtPlotMarker *d_origin;
    QwtPlotCurve *d_curve;
    int d_paintedPoints;

    QwtPlotDirectPainter *d_directPainter;

    QwtInterval d_interval;
    int d_timerId;

    QwtSystemClock d_clock;
    //
    QwtPlotTextLabel *legendAmpitude;
    QwtPlotTextLabel *legendFrequency;
    QwtText *textAmplitude;
    QwtText *textFrecuencia;
    bool stopped=false;
};
