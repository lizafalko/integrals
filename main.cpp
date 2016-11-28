#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QtMath>

double f(double x) {
    return pow(x, 2.0) / 10.0;
}

double absolute(double value) {
    if ( value > 0.0 ) {
        return value;
    } else {
        return -1.0 * value;
    }
}


double get_right_rectangle(double A,
                          double B,
                          unsigned int N) {
    double result = 0.0;
    double delta = absolute(B - A) / N;

    QVector<double> interval(N + 1);

    for ( double i = 0.0, interval_part = A;
          i < N, interval_part <= B;
          i++, interval_part += delta ) {
        interval[(int)i] = interval_part;
    }

    for ( unsigned int i = 1; i < N; i++ ) {
        double h = interval[(int) i] - interval[(int) i - 1];
        double y = absolute(f(interval[i]));
        result += h * y;
    }

    return result;
}

double get_center_rectangle(double A,
                          double B,
                          unsigned int N) {
    double result = 0.0;
    double delta = absolute(B - A) / N;

    QVector<double> interval(N + 1);

    for ( double i = 0.0, interval_part = A;
          i < N, interval_part <= B;
          i++, interval_part += delta ) {
        interval[(int)i] = interval_part;
    }

    for ( unsigned int i = 1; i < N; i++ ) {
        double h = interval[(int) i] - interval[(int) i - 1];
        double y_right = absolute(f(interval[i]));
        double y_left = absolute(f(interval[i - 1]));
        double y = (y_right + y_left) / 2.0;
        result += h * y;
    }

    return result;
}

double get_trapeze(double A,
                   double B,
                   unsigned int N) {
    double result = 0.0;
    double delta = absolute(B - A) / N;

    QVector<double> interval(N + 1);

    for ( double i = 0.0, interval_part = A;
          i < N, interval_part <= B;
          i++, interval_part += delta ) {
        interval[(int)i] = interval_part;
    }

    for ( unsigned int i = 1; i < N; i++ ) {
        double h = interval[(int) i] - interval[(int) i - 1];
        double y_right = absolute(f(interval[i]));
        double y_left = absolute(f(interval[i - 1]));
        double y = (y_right + y_left) / 2.0;
        result += h * y;
    }

    return result;
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;

    qDebug() << get_right_rectangle(3.0, 6.0, 10000) << get_center_rectangle(3.0, 6.0, 10000);

    return a.exec();
}
