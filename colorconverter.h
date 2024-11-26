#ifndef COLORCONVERTER_H
#define COLORCONVERTER_H

#include <QWidget>
#include <QLineEdit>
#include <QSlider>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QColorDialog>
#include <QPushButton>

class ColorConverter : public QWidget {
    Q_OBJECT

public:
    explicit ColorConverter(QWidget *parent = nullptr);

private slots:
    void onRgbChanged();
    void onCmykChanged();
    void onLabChanged();
    void onColorDialog();
    void updateColorDisplay();

private:
    QLineEdit *rInput, *gInput, *bInput;
    QLineEdit *cInput, *mInput, *yInput, *kInput;
    QLineEdit *lInput, *aInput, *bLabInput;
    QSlider *rSlider, *gSlider, *bSlider;
    QLabel *colorDisplay;

    bool updating; // 用于避免递归更新的标志

    void setupUI();
    void updateCmykLabFromRgb(int r, int g, int b);

    void rgbToCmyk(int r, int g, int b, int &c, int &m, int &y, int &k);
    void cmykToRgb(int c, int m, int y, int k, int &r, int &g, int &b);
    void rgbToLab(int r, int g, int b, double &l, double &a, double &bLab);
    void labToRgb(double l, double a, double bLab, int &r, int &g, int &b);
    void rgbToXyz(int r, int g, int b, double &x, double &y, double &z);
    void xyzToLab(double x, double y, double z, double &l, double &a, double &bLab);
    void labToXyz(double l, double a, double bLab, double &x, double &y, double &z);
    void xyzToRgb(double x, double y, double z, int &r, int &g, int &b);
};

#endif // COLORCONVERTER_H
