#include "colorconverter.h"
#include <algorithm>
#include <cmath>

ColorConverter::ColorConverter(QWidget *parent) :
    QWidget(parent), updating(false) {
    setupUI();
}

void ColorConverter::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QHBoxLayout *rgbLayout = new QHBoxLayout();
    QLabel *rLabel = new QLabel("R:", this);
    rInput = new QLineEdit(this);
    rSlider = new QSlider(Qt::Horizontal, this);
    rSlider->setRange(0, 255);
    QLabel *gLabel = new QLabel("G:", this);
    gInput = new QLineEdit(this);
    gSlider = new QSlider(Qt::Horizontal, this);
    gSlider->setRange(0, 255);
    QLabel *bLabel = new QLabel("B:", this);
    bInput = new QLineEdit(this);
    bSlider = new QSlider(Qt::Horizontal, this);
    bSlider->setRange(0, 255);
    rgbLayout->addWidget(rLabel);
    rgbLayout->addWidget(rInput);
    rgbLayout->addWidget(rSlider);
    rgbLayout->addWidget(gLabel);
    rgbLayout->addWidget(gInput);
    rgbLayout->addWidget(gSlider);
    rgbLayout->addWidget(bLabel);
    rgbLayout->addWidget(bInput);
    rgbLayout->addWidget(bSlider);

    QHBoxLayout *cmykLayout = new QHBoxLayout();
    QLabel *cLabel = new QLabel("C:", this);
    cInput = new QLineEdit(this);
    QLabel *mLabel = new QLabel("M:", this);
    mInput = new QLineEdit(this);
    QLabel *yLabel = new QLabel("Y:", this);
    yInput = new QLineEdit(this);
    QLabel *kLabel = new QLabel("K:", this);
    kInput = new QLineEdit(this);
    cmykLayout->addWidget(cLabel);
    cmykLayout->addWidget(cInput);
    cmykLayout->addWidget(mLabel);
    cmykLayout->addWidget(mInput);
    cmykLayout->addWidget(yLabel);
    cmykLayout->addWidget(yInput);
    cmykLayout->addWidget(kLabel);
    cmykLayout->addWidget(kInput);

    QHBoxLayout *labLayout = new QHBoxLayout();
    QLabel *lLabel = new QLabel("L:", this);
    lInput = new QLineEdit(this);
    QLabel *aLabel = new QLabel("A:", this);
    aInput = new QLineEdit(this);
    QLabel *bLabLabel = new QLabel("B:", this);
    bLabInput = new QLineEdit(this);
    labLayout->addWidget(lLabel);
    labLayout->addWidget(lInput);
    labLayout->addWidget(aLabel);
    labLayout->addWidget(aInput);
    labLayout->addWidget(bLabLabel);
    labLayout->addWidget(bLabInput);

    colorDisplay = new QLabel(this);
    colorDisplay->setFixedSize(100, 100);
    colorDisplay->setStyleSheet("background-color: white;");

    QPushButton *colorDialogButton = new QPushButton("选择颜色", this);
    connect(colorDialogButton, &QPushButton::clicked, this, &ColorConverter::onColorDialog);

    mainLayout->addLayout(rgbLayout);
    mainLayout->addLayout(cmykLayout);
    mainLayout->addLayout(labLayout);
    mainLayout->addWidget(colorDisplay);
    mainLayout->addWidget(colorDialogButton);

    setLayout(mainLayout);

    connect(rInput, &QLineEdit::textChanged, this, &ColorConverter::onRgbChanged);
    connect(gInput, &QLineEdit::textChanged, this, &ColorConverter::onRgbChanged);
    connect(bInput, &QLineEdit::textChanged, this, &ColorConverter::onRgbChanged);

    connect(rSlider, &QSlider::valueChanged, [this](int value) {
        if (!updating) {
            rInput->setText(QString::number(value));
            onRgbChanged();
        }
    });
    connect(gSlider, &QSlider::valueChanged, [this](int value) {
        if (!updating) {
            gInput->setText(QString::number(value));
            onRgbChanged();
        }
    });
    connect(bSlider, &QSlider::valueChanged, [this](int value) {
        if (!updating) {
            bInput->setText(QString::number(value));
            onRgbChanged();
        }
    });

    connect(cInput, &QLineEdit::textChanged, this, &ColorConverter::onCmykChanged);
    connect(mInput, &QLineEdit::textChanged, this, &ColorConverter::onCmykChanged);
    connect(yInput, &QLineEdit::textChanged, this, &ColorConverter::onCmykChanged);
    connect(kInput, &QLineEdit::textChanged, this, &ColorConverter::onCmykChanged);

    connect(lInput, &QLineEdit::textChanged, this, &ColorConverter::onLabChanged);
    connect(aInput, &QLineEdit::textChanged, this, &ColorConverter::onLabChanged);
    connect(bLabInput, &QLineEdit::textChanged, this, &ColorConverter::onLabChanged);
}

void ColorConverter::onRgbChanged() {
    if (updating) return;

    updating = true;

    int r = rInput->text().toInt();
    int g = gInput->text().toInt();
    int b = bInput->text().toInt();

    rSlider->setValue(r);
    gSlider->setValue(g);
    bSlider->setValue(b);

    updateCmykLabFromRgb(r, g, b);
    updateColorDisplay();  // 更新颜色显示

    updating = false;
}

void ColorConverter::onCmykChanged() {
    if (updating) return;

    updating = true;

    int c = cInput->text().toInt();
    int m = mInput->text().toInt();
    int y = yInput->text().toInt();
    int k = kInput->text().toInt();

    int r, g, b;
    cmykToRgb(c, m, y, k, r, g, b);
    rInput->setText(QString::number(r));
    gInput->setText(QString::number(g));
    bInput->setText(QString::number(b));

    rSlider->setValue(r);
    gSlider->setValue(g);
    bSlider->setValue(b);

    double l, a, bLab;
    rgbToLab(r, g, b, l, a, bLab);
    lInput->setText(QString::number(l));
    aInput->setText(QString::number(a));
    bLabInput->setText(QString::number(bLab));

    updateColorDisplay();  // 更新颜色显示

    updating = false;
}

void ColorConverter::onLabChanged() {
    if (updating) return;

    updating = true;

    double l = lInput->text().toDouble();
    double a = aInput->text().toDouble();
    double bLab = bLabInput->text().toDouble();

    int r, g, b;
    labToRgb(l, a, bLab, r, g, b);
    rInput->setText(QString::number(r));
    gInput->setText(QString::number(g));
    bInput->setText(QString::number(b));

    rSlider->setValue(r);
    gSlider->setValue(g);
    bSlider->setValue(b);

    int c, m, y, k;
    rgbToCmyk(r, g, b, c, m, y, k);
    cInput->setText(QString::number(c));
    mInput->setText(QString::number(m));
    yInput->setText(QString::number(y));
    kInput->setText(QString::number(k));

    updateColorDisplay();  // 更新颜色显示

    updating = false;
}

void ColorConverter::onColorDialog() {
    if (updating) return;

    updating = true;

    QColor color = QColorDialog::getColor(Qt::white, this, "选择颜色");
    if (color.isValid()) {
        int r = color.red();
        int g = color.green();
        int b = color.blue();

        rInput->setText(QString::number(r));
        gInput->setText(QString::number(g));
        bInput->setText(QString::number(b));

        rSlider->setValue(r);
        gSlider->setValue(g);
        bSlider->setValue(b);

        updateCmykLabFromRgb(r, g, b);
        updateColorDisplay();  // 更新颜色显示
    }

    updating = false;
}

void ColorConverter::updateCmykLabFromRgb(int r, int g, int b) {
    int c, m, y, k;
    rgbToCmyk(r, g, b, c, m, y, k);
    cInput->setText(QString::number(c));
    mInput->setText(QString::number(m));
    yInput->setText(QString::number(y));
    kInput->setText(QString::number(k));

    double l, a, bLab;
    rgbToLab(r, g, b, l, a, bLab);
    lInput->setText(QString::number(l));
    aInput->setText(QString::number(a));
    bLabInput->setText(QString::number(bLab));
}

void ColorConverter::updateColorDisplay() {
    int r = rInput->text().toInt();
    int g = gInput->text().toInt();
    int b = bInput->text().toInt();

    QColor color(r, g, b);
    QString colorStyle = QString("background-color: %1").arg(color.name());
    colorDisplay->setStyleSheet(colorStyle);
}

void ColorConverter::rgbToCmyk(int r, int g, int b, int &c, int &m, int &y, int &k) {
    float rf = r / 255.0f;
    float gf = g / 255.0f;
    float bf = b / 255.0f;

    k = 100 * (1 - std::max(rf, std::max(gf, bf)));
    if (k != 100) {
        c = 100 * (1 - rf - k / 100.0f) / (1 - k / 100.0f);
        m = 100 * (1 - gf - k / 100.0f) / (1 - k / 100.0f);
        y = 100 * (1 - bf - k / 100.0f) / (1 - k / 100.0f);
    } else {
        c = m = y = 0;
    }
}

void ColorConverter::cmykToRgb(int c, int m, int y, int k, int &r, int &g, int &b) {
    float cf = c / 100.0f;
    float mf = m / 100.0f;
    float yf = y / 100.0f;
    float kf = k / 100.0f;

    r = 255 * (1 - cf) * (1 - kf);
    g = 255 * (1 - mf) * (1 - kf);
    b = 255 * (1 - yf) * (1 - kf);
}

void ColorConverter::rgbToLab(int r, int g, int b, double &l, double &a, double &bLab) {
    double x, y, z;
    rgbToXyz(r, g, b, x, y, z);
    xyzToLab(x, y, z, l, a, bLab);
}

void ColorConverter::labToRgb(double l, double a, double bLab, int &r, int &g, int &b) {
    double x, y, z;
    labToXyz(l, a, bLab, x, y, z);
    xyzToRgb(x, y, z, r, g, b);
}

void ColorConverter::rgbToXyz(int r, int g, int b, double &x, double &y, double &z) {
    double rf = r / 255.0;
    double gf = g / 255.0;
    double bf = b / 255.0;

    if (rf > 0.04045) rf = std::pow((rf + 0.055) / 1.055, 2.4);
    else rf /= 12.92;

    if (gf > 0.04045) gf = std::pow((gf + 0.055) / 1.055, 2.4);
    else gf /= 12.92;

    if (bf > 0.04045) bf = std::pow((bf + 0.055) / 1.055, 2.4);
    else bf /= 12.92;

    rf *= 100.0;
    gf *= 100.0;
    bf *= 100.0;

    x = rf * 0.4124564 + gf * 0.3575761 + bf * 0.1804375;
    y = rf * 0.2126729 + gf * 0.7151522 + bf * 0.0721750;
    z = rf * 0.0193339 + gf * 0.1191920 + bf * 0.9503041;
}

void ColorConverter::xyzToLab(double x, double y, double z, double &l, double &a, double &bLab) {
    x /= 95.047;
    y /= 100.000;
    z /= 108.883;

    if (x > 0.008856) x = std::pow(x, 1.0 / 3.0);
    else x = (7.787 * x) + (16.0 / 116.0);

    if (y > 0.008856) y = std::pow(y, 1.0 / 3.0);
    else y = (7.787 * y) + (16.0 / 116.0);

    if (z > 0.008856) z = std::pow(z, 1.0 / 3.0);
    else z = (7.787 * z) + (16.0 / 116.0);

    l = (116.0 * y) - 16.0;
    a = 500.0 * (x - y);
    bLab = 200.0 * (y - z);
}

void ColorConverter::labToXyz(double l, double a, double bLab, double &x, double &y, double &z) {
    y = (l + 16.0) / 116.0;
    x = a / 500.0 + y;
    z = y - bLab / 200.0;

    if (std::pow(y, 3) > 0.008856) y = std::pow(y, 3);
    else y = (y - 16.0 / 116.0) / 7.787;

    if (std::pow(x, 3) > 0.008856) x = std::pow(x, 3);
    else x = (x - 16.0 / 116.0) / 7.787;

    if (std::pow(z, 3) > 0.008856) z = std::pow(z, 3);
    else z = (z - 16.0 / 116.0) / 7.787;

    x *= 95.047;
    y *= 100.000;
    z *= 108.883;
}

void ColorConverter::xyzToRgb(double x, double y, double z, int &r, int &g, int &b) {
    x /= 100.0;
    y /= 100.0;
    z /= 100.0;

    double rf = x *  3.2404542 + y * -1.5371385 + z * -0.4985314;
    double gf = x * -0.9692660 + y *  1.8760108 + z *  0.0415560;
    double bf = x *  0.0556434 + y * -0.2040259 + z *  1.0572252;

    if (rf > 0.0031308) rf = 1.055 * std::pow(rf, 1.0 / 2.4) - 0.055;
    else rf *= 12.92;

    if (gf > 0.0031308) gf = 1.055 * std::pow(gf, 1.0 / 2.4) - 0.055;
    else gf *= 12.92;

    if (bf > 0.0031308) bf = 1.055 * std::pow(bf, 1.0 / 2.4) - 0.055;
    else bf *= 12.92;

    r = std::max(0, std::min(255, static_cast<int>(rf * 255.0)));
    g = std::max(0, std::min(255, static_cast<int>(gf * 255.0)));
    b = std::max(0, std::min(255, static_cast<int>(bf * 255.0)));
}
