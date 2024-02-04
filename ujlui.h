#ifndef UJLUI_H
#define UJLUI_H

#include "ujlparser.h"
#include "ujlpatcher.h"

#include "ujlexporter.h"
#include "ujlimporter.h"

#include <QMainWindow>

#include <QTableWidgetItem>
#include <QDragEnterEvent>
#include <QMimeData>

QT_BEGIN_NAMESPACE
namespace Ui { class ujlui; }
QT_END_NAMESPACE

class ujlui : public QMainWindow
{
    Q_OBJECT

public:
    ujlparser Parser;
    ujlpatcher Patcher;
    ujlexporter Exporter;
    ujlimporter Importer;
    ujlui(QWidget *parent = nullptr);
    ~ujlui();

private:
    int displaySaveDlg();
    Ui::ujlui *ui;

    QIcon icParappa;
    QIcon icTeacher;

    QIcon icLangEn;
    QIcon icLangJp;
    QIcon icLangGr;
    QIcon icLangFr;
    QIcon icLangIt;
    QIcon icLangEs;

    QIcon langsP[5];
    QIcon langs[2];

public slots:
    void setFiltering(int input, bool value);
    void editSubtitleText(int subId, QString text);

private slots:
    void startGUI();
    void clearGUI();
    void countGUI();
    void updateGUI();
    void updateLanguages(bool isClear);
    void startFileInfos(QString fileName);

    void on_actionSave_triggered();
    void on_actionOpen_triggered();

    void on_setAll_toggled(bool isChecked);
    void on_setEn_toggled(bool isChecked);
    void on_setJp_toggled(bool isChecked);
    void on_setDe_toggled(bool isChecked);
    void on_setFr_toggled(bool isChecked);
    void on_setIt_toggled(bool isChecked);
    void on_setSp_toggled(bool isChecked);

    void on_cListWidget_itemChanged(QTableWidgetItem *item);
    void on_gListWidget_itemChanged(QTableWidgetItem *item);
    void on_aListWidget_itemChanged(QTableWidgetItem *item);
    void on_rListWidget_itemChanged(QTableWidgetItem *item);

    void on_actionHideBlank_triggered();
    void on_actionHideLink_triggered();
    void on_actionLink_triggered();
    void on_actionFind_triggered();
    void on_actionAbout_triggered();

    void on_addRepButton_clicked();
    void on_removeRepButton_clicked();

    void on_actionImportP_triggered();
    void on_actionExportP_triggered();
    void on_actionImportR_triggered();
    void on_actionExportR_triggered();
    void on_actionCheckRight_triggered();

protected:
    void closeEvent(QCloseEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dropEvent(QDropEvent *event);
};
#endif // UJLUI_H
