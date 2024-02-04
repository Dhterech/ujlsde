#include "sde2uipreview.h"
#include "ui_sde2uipreview.h"

sde2uipreview::sde2uipreview(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::sde2uipreview)
{
    ui->setupUi(this);
}

sde2uipreview::~sde2uipreview()
{
    delete ui;
}

void sde2uipreview::SetupPreview(Subtitle subtitle) {
    QString fontPath = subtitle.isEucJP ? ":/previewRes/pvSubJapanFont" : ":/previewRes/pvSubLatinFont";
    int id = QFontDatabase::addApplicationFont(fontPath);
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont subFont(family);
    subFont.setPointSize(12);

    ui->prvSubtitle->setFont(subFont);
    ui->prvSubtitle->setText(subtitle.Text);
    ui->prvSubtitle->setStyleSheet("QLabel { color : white; }");
    return;
}
