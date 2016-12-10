#include "mainwindow.h"
#include <QApplication>
#include "qcustomplot.h"
#include <QDebug>
#include <QtMath>

double f(double x) {
    return pow(x, 2.0) / 10.0;
}

double log2(double x) {
    return log(x) / log(2.0);
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
          i < N && interval_part <= B;
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

double get_simpson(double A,
                   double B,
                   unsigned int N) {
    double result = 0.0;
    double h = absolute(B - A) / (double) N;

    QVector<double> interval(N + 1);
    for ( double i = 0.0, interval_part = A;
          i < N && interval_part <= B;
          i++, interval_part += h ) {
        interval[(int)i] = interval_part;
    }

    result += f(interval[0]); //y0
    result += f(interval[N]); //yN

    for ( unsigned int i = 1; i < N; i += 2 ) {
        result += 4.0 * f(interval[i]);
    }

    for ( unsigned int i = 2; i < N - 1; i += 2 ) {
        result += 2.0 * f(interval[i]);
    }

    result *= (h / 3.0);
    return result;
}

double get_trapeze(double A,
                   double B,
                   unsigned int N) {
    double result = 0.0;
    double h = absolute(B - A) / (double) N;

    QVector<double> interval(N + 1);
    for ( double i = 0.0, interval_part = A;
          i < N && interval_part <= B;
          i++, interval_part += h ) {
        interval[(int)i] = interval_part;
    }
    for ( unsigned int i = 1; i < N; i++ ) {
        result += f(interval[i]);
    }
    result += (f((double) A) + f((double) B)) / 2.0;
    result *= h;
    return result;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QMainWindow window;
    QCustomPlot customPlot;
    window.setCentralWidget(&customPlot);

    double A = 3.0;
    double B = 6.0;
    double calculated_right_answer = 6.3;

    QVector<double> n_axis(20),
            delta_axis_trapeze(20),
            delta_axis_right_rectangle(20),
            delta_axis_simpson(20);

    for ( unsigned int i = 0;
          i < 20;
          i++) {
        n_axis[i] = i + 1;
        delta_axis_right_rectangle[i] = log2(absolute(calculated_right_answer - get_right_rectangle(A, B, pow(2, n_axis[i]))));
        delta_axis_trapeze[i] = log2(absolute(calculated_right_answer - get_trapeze(A, B, pow(2, n_axis[i]))));
        delta_axis_simpson[i] = log2(absolute(calculated_right_answer - get_simpson(A, B, pow(2, n_axis[i]))));
    }

    customPlot.addGraph();
    customPlot.graph(0) -> setData(n_axis, delta_axis_right_rectangle);
    customPlot.graph(0) -> setPen(QPen(Qt::red));
    customPlot.graph(0) -> setName("Right rectangle");

    customPlot.addGraph();
    customPlot.graph(1) -> setData(n_axis, delta_axis_trapeze);
    customPlot.graph(1) -> setPen(QPen(Qt::green));
    customPlot.graph(1) -> setName("Trapeze");

    customPlot.addGraph();
    customPlot.graph(2) -> setData(n_axis, delta_axis_simpson);
    customPlot.graph(2) -> setPen(QPen(Qt::cyan));
    customPlot.graph(2) -> setName("Simpson");


    qDebug() << "Right: " << qSetRealNumberPrecision(17) << get_right_rectangle(A, B, pow(2, 20));
    qDebug() << "Trapeze: " << qSetRealNumberPrecision(17) << get_trapeze(A, B, pow(2, 20));
    qDebug() << "Simpson: " << qSetRealNumberPrecision(17) << get_simpson(A, B, pow(2, 20));
    qDebug() << endl;
    qDebug() << qSetRealNumberPrecision(17) << calculated_right_answer << endl;

    customPlot.legend -> setVisible(true);
    customPlot.xAxis -> setLabel("N");
    customPlot.yAxis -> setLabel("Log2(Delta)");
    customPlot.rescaleAxes();
    window.setGeometry(100, 100, 1000, 600);
    window.show();
    return a.exec();
}
