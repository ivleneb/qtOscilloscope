#ifndef SCREENSHOT_H
#define SCREENSHOT_H
#include<QtCore>
#include<QPixmap>

class Screenshot
{
public:
    Screenshot();
    QPixmap map;
    double amplitude;
    double frequency;
    QString item_name;
};

#endif // SCREENSHOT_H
