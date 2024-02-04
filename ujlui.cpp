#include "ujlui.h"
#include "ujlparser.h"
#include "ui_ujlui.h"
#include "ujldata.h"
#include "ujlpatcher.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTableWidget>
#include <QInputDialog>
#include <QCloseEvent>

// same UI for the same 6 SDE

ujlparser Parser;
bool hideBlanks = true;
bool isFiltered = false;
bool filteredLangs[5] = { false, false, false, false, false };
bool uiReady = false;

int findedCurrentL = -1;
QList<QTableWidgetItem *> findedItems;

QString langsTP[5] = { "English", "German", "French", "Italian", "Spanish" };
QString langsT[2] = { "English", "Japanese" };
QString ownerNames[2] = { "Teacher", "Lammy/Parappa" };

ujlui::ujlui(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ujlui)
{
    ui->setupUi(this);
    statusBar()->showMessage(tr("Welcome to UJLSDE!"));

    ui->cListWidget->setColumnHidden(0, true);
    ui->gListWidget->setColumnHidden(0, true);
    ui->gListWidget->setColumnHidden(2, true);
    ui->gListWidget->setColumnHidden(3, true);
    ui->cListWidget->setColumnHidden(4, true);
    ui->actionImportP->QAction::setVisible(false);
    ui->actionExportP->QAction::setVisible(false);
    ui->actionExportR->QAction::setVisible(false);
    ui->actionImportR->QAction::setVisible(false);
    ui->menuTools->menuAction()->setVisible(false);


    icParappa = QIcon(":/tableIcons/icParappa");
    icTeacher = QIcon(":/tableIcons/icTeacher");

    langs[0] = QIcon(":/tableIcons/language/icLangEn");
    langs[1] = QIcon(":/tableIcons/language/icLangJp");

    langsP[0] = langs[0];
    langsP[1] = QIcon(":/tableIcons/language/icLangGr");
    langsP[2] = QIcon(":/tableIcons/language/icLangFr");
    langsP[3] = QIcon(":/tableIcons/language/icLangIt");
    langsP[4] = QIcon(":/tableIcons/language/icLangEs");
}

ujlui::~ujlui()
{
    delete ui;
}

void ujlui::closeEvent(QCloseEvent *event) {
    if(fileInfo.hasEdited) {
        int result = displaySaveDlg();
        switch(result) {
        case QMessageBox::Save:
            on_actionSave_triggered();
            break;
        case QMessageBox::Cancel:
            event->ignore();
            break;
        }
    }
}

void ujlui::dragEnterEvent(QDragEnterEvent *event) {
    event->acceptProposedAction();
}

void ujlui::dragMoveEvent(QDragMoveEvent *event) {
    event->acceptProposedAction();
}

void ujlui::dragLeaveEvent(QDragLeaveEvent *event) {
    event->accept();
}

void ujlui::dropEvent(QDropEvent *event) {
    const QMimeData *mime(event->mimeData());
    QString fileName = QDir::toNativeSeparators(QUrl(mime->urls().at(0).toString()).toLocalFile());

    qDebug() << "Loading from drag & drop: " << fileName;
    int loadr = Parser.loadFile(fileName);
    if(loadr == 0) {
        startFileInfos(fileName);
        clearGUI();
        startGUI();
        statusBar()->showMessage(tr("Loaded SNG file successfully!"));
    } else {
        statusBar()->showMessage(tr("Couldn't find a file! Check naming scheme"));
    }

}

void ujlui::on_actionOpen_triggered() {
    QString fileName =QFileDialog::getOpenFileName(this, tr("Open SNG, COP or CMP File!"), "", tr("COMODX.SNG File (*.SNG);;COMODX.COP File (*.COP);;COMODX.CMP File (*.CMP);;COMODX.MEN File (*.MEN);;Any File (*.*)"));

    int loadr = Parser.loadFile(fileName);
    if(loadr == 3) return;
    if(loadr == 0) {
        startFileInfos(fileName);
        clearGUI();
        startGUI();
        statusBar()->showMessage(tr("Loaded SNG file successfully!"));
    } else {
        statusBar()->showMessage(tr("Couldn't find a file! Check naming scheme"));
    }
}

void ujlui::startFileInfos(QString fileName) {
    QFileInfo info(fileName);
    QWidget::setWindowTitle(info.completeBaseName() +" - UJLSDE");

    QString i;
    i = fileInfo.Name + "\n";
    i += fileInfo.Description;
    i += "\n";
    i += "\nGame region: ";
    i += (fileInfo.languageCount == 1) ? "USA" : "NTSC-J"; // FIXME
    i += "\nAvailable languages: ";
    i += (fileInfo.languageCount == 1) ? "English" : "English, Japanese";
    i += "\nSubtitle count: ";
    i += QString::number(Subtitles.size());
    i += "\nFile path: ";
    i += info.filePath();
    ui->infoDeb->setPlainText(i);
}


void ujlui::on_actionSave_triggered()
{
    QString fileName = QDir::toNativeSeparators(QFileDialog::getSaveFileName(this, tr("Save SNG File"), "", tr("COMODX.SNG File (*.SNG);;COMODX.COP File (*.COP);;COMODX.CMP File (*.CMP);;All Files (*.)")));
    int savr = Patcher.PatchFile(fileName);
    if(savr == 0) statusBar()->showMessage(tr("Saved file successfully"));
    else statusBar()->showMessage(tr("Couldn't find ELF file! Check naming scheme"));
}

void ujlui::startGUI() {
    uiReady = false;
    statusBar()->showMessage(tr("Populating UI..."));
    // Enable buttons
    ui->actionSave->setEnabled(true);
    ui->setAll->setEnabled(true);
    ui->actionHideLink->setEnabled(true);
    updateLanguages(false);

    countGUI();
    updateGUI();
}

void ujlui::countGUI() {
    fileInfo.subCount = Subtitles.size() - 1;
    fileInfo.cutCount = 0;
    fileInfo.gamCount = 0;

    for(CTable const &ctab : Cutscenes) {
        if(isFiltered && !filteredLangs[ctab.Language]) continue;
        for(cLine const &subs : ctab.Subtitles) {
            if(Subtitles[subs.localisation].Text == "" && hideBlanks) continue;
            if(!ui->actionHideLink->isChecked() && subs.localelinked) continue;
            fileInfo.cutCount++;
        }
    }

    for(GTable const &gtab : Gameplays) {
        if(isFiltered && !filteredLangs[gtab.Language]) continue;
        for(gLine const &subs : gtab.Subtitles) {
            if(Subtitles[subs.localisation].Text == "" && hideBlanks) continue;
            if(!ui->actionHideLink->isChecked() && subs.localelinked) continue;
            fileInfo.gamCount++;
        }
    }

    ui->aListWidget->setRowCount(fileInfo.subCount);
    ui->cListWidget->setRowCount(fileInfo.cutCount);
    ui->gListWidget->setRowCount(fileInfo.gamCount);
}

void ujlui::updateGUI() {
    int rowCount = 0;
    int cutCount = 0;
    int subCount = 0;
    uiReady = false;

    // Populate ALl Subtitles tab
    for(Subtitle const &sub : Subtitles) {
        QTableWidgetItem *iPosi = new QTableWidgetItem(QString::number(sub.Position - fileInfo.binPositionStart));
        QTableWidgetItem *iText = new QTableWidgetItem(sub.Text);
        iPosi->setFlags(iPosi->flags() & ~Qt::ItemIsEditable);
        ui->aListWidget->setItem(rowCount, 0, iPosi);
        ui->aListWidget->setItem(rowCount, 1, iText);
        rowCount++;
    }
    rowCount = 0;

    // Populate Cutscene tab
    for(CTable const &ctab : Cutscenes) {
        if(isFiltered && !filteredLangs[ctab.Language]) continue;
        for(cLine const &subs : ctab.Subtitles) {
            if(Subtitles[subs.localisation].Text == "" && hideBlanks) continue;
            if(!ui->actionHideLink->isChecked() && subs.localelinked) continue;
            QTableWidgetItem *ID = new QTableWidgetItem(QString::number(subCount));
            QTableWidgetItem *iCutID = new QTableWidgetItem(QString::number(cutCount));
            QTableWidgetItem *iTimST = new QTableWidgetItem(QString::number(subs.timestart));
            QTableWidgetItem *iTimEN = new QTableWidgetItem(QString::number(subs.timeend));
            QTableWidgetItem *iLang = new QTableWidgetItem(QString::number(ctab.Language));
            QTableWidgetItem *iText = new QTableWidgetItem(Subtitles[subs.localisation].Text);
            if(subs.localelinked) {iText->setText("(Linked to " + QString::number(subs.localisation+1) +") " + iText->text()); iText->setFlags(Qt::ItemIsEditable);}
            iCutID->setFlags(iCutID->flags() & ~Qt::ItemIsEditable);
            iLang->setFlags(iLang->flags() & ~Qt::ItemIsEditable);
            iLang->setIcon((fileInfo.languageCount == 5) ? langsP[ctab.Language] : langs[ctab.Language]);
            iLang->setText((fileInfo.languageCount == 5) ? langsTP[ctab.Language] : langsT[ctab.Language]);
            ui->cListWidget->setItem(rowCount, 0, ID);
            ui->cListWidget->setItem(rowCount, 1, iCutID);
            ui->cListWidget->setItem(rowCount, 2, iTimST);
            ui->cListWidget->setItem(rowCount, 3, iTimEN);
            ui->cListWidget->setItem(rowCount, 4, iLang);
            ui->cListWidget->setItem(rowCount, 5, iText);
            rowCount++;
            subCount++;
        }
        subCount = 0;
        cutCount++;
    }
    rowCount = 0;
    cutCount = 0;

    // Populate Gameplay tab
    for(GTable const &gtab : Gameplays) {
        if(isFiltered && !filteredLangs[gtab.Language]) continue;
        for(gLine const &gpt : gtab.Subtitles) {
            if(Subtitles[gpt.localisation].Text == "" && hideBlanks) continue;
            if(!ui->actionHideLink->isChecked() && gpt.localelinked) continue;
            QTableWidgetItem *ID = new QTableWidgetItem(QString::number(subCount));
            QTableWidgetItem *iGamID = new QTableWidgetItem(QString::number(cutCount));
            QTableWidgetItem *iOwner = new QTableWidgetItem(ownerNames[gpt.owner]);
            QTableWidgetItem *iLang = new QTableWidgetItem(QString::number(gtab.Language));
            QTableWidgetItem *iUnk = new QTableWidgetItem(QString::number(gpt.unknown));
            QTableWidgetItem *iText = new QTableWidgetItem(Subtitles[gpt.localisation].Text);
            if(gpt.localelinked) {iText->setText("(Linked to " + QString::number(gpt.localisation+1)  +") " + iText->text()); iText->setFlags(Qt::ItemIsEditable);}
            iOwner->setIcon((gpt.owner == 0) ? icTeacher : icParappa);
            iLang->setIcon((fileInfo.languageCount == 5) ? langsP[gtab.Language] : langs[gtab.Language]);
            iLang->setText((fileInfo.languageCount == 5) ? langsTP[gtab.Language] : langsT[gtab.Language]);
            iGamID->setFlags(iGamID->flags() & ~Qt::ItemIsEditable);
            iOwner->setFlags(iOwner->flags() & ~Qt::ItemIsEditable);
            iLang->setFlags(iLang->flags() & ~Qt::ItemIsEditable);
            ui->gListWidget->setItem(rowCount, 0, ID);
            ui->gListWidget->setItem(rowCount, 1, iGamID);
            ui->gListWidget->setItem(rowCount, 2, iLang);
            ui->gListWidget->setItem(rowCount, 3, iOwner);
            ui->gListWidget->setItem(rowCount, 4, iUnk);
            ui->gListWidget->setItem(rowCount, 5, iText);
            rowCount++;
            subCount++;
        }
        subCount = 0;
        cutCount++;
    }
    uiReady = true;
}

void ujlui::updateLanguages(bool isClear) {
    bool isPal = fileInfo.languageCount == 5;
    // Enabled or not
    ui->setEn->setEnabled(isPal && !isClear && isFiltered);
    ui->setDe->setEnabled(isPal && !isClear && isFiltered);
    ui->setFr->setEnabled(isPal && !isClear && isFiltered);
    ui->setIt->setEnabled(isPal && !isClear && isFiltered);
    ui->setSp->setEnabled(isPal && !isClear && isFiltered);
    ui->setJp->setEnabled(!isPal && !isClear && isFiltered);
    // Filtered or not
    ui->setEn->setChecked(filteredLangs[0] && isFiltered);
    if(isPal) {
        ui->setDe->setChecked(filteredLangs[1] && isFiltered);
        ui->setFr->setChecked(filteredLangs[2] && isFiltered);
        ui->setIt->setChecked(filteredLangs[3] && isFiltered);
        ui->setSp->setChecked(filteredLangs[4] && isFiltered);
    } //else ui->setJp->setChecked(filteredLangs[1] && isFiltered); NO Japanese LMAO
}

void ujlui::clearGUI() {
    ui->actionSave->setEnabled(false);
    ui->setAll->setEnabled(false);
    ui->actionHideLink->setEnabled(false);
    updateLanguages(true);

    uiReady = false;
    while(ui->cListWidget->rowCount() > 0) { ui->cListWidget->removeRow(0); }
    while(ui->gListWidget->rowCount() > 0) { ui->gListWidget->removeRow(0); }
    while(ui->aListWidget->rowCount() > 0) { ui->aListWidget->removeRow(0); }
    uiReady = true;
}

int ujlui::displaySaveDlg() {
    QMessageBox msgBox;
    msgBox.setText("The SNG file has been modified.");
    msgBox.setInformativeText("Do you want to save your changes?");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    return msgBox.exec();
}

void ujlui::editSubtitleText(int subId, QString text) {
    QByteArray oTConv = Subtitles[subId].isEucJP ? Subtitles[subId].Text.toUtf8() : Subtitles[subId].Text.toLatin1();
    QByteArray mTConv = Subtitles[subId].isEucJP ? text.toUtf8() : text.toLatin1();
    int diff = mTConv.size() - oTConv.size();
    Patcher.ApplyChanges(Subtitles[subId].Position, mTConv.size(), diff, 0);
    Subtitles[subId].Text = text;
}

// Filtering

void ujlui::on_setAll_toggled(bool isChecked) {
    isFiltered = !isChecked;
    clearGUI();
    startGUI();
}

void ujlui::setFiltering(int input, bool value) {
    if(input != 99) filteredLangs[input] = value;

    updateLanguages(false);
    clearGUI();
    startGUI();
    statusBar()->showMessage(tr("Changed view options"));
}

void ujlui::on_setEn_toggled(bool isTrue) { setFiltering(0, isTrue); }
void ujlui::on_setJp_toggled(bool isTrue) { setFiltering(1, isTrue); }
void ujlui::on_setDe_toggled(bool isTrue) { setFiltering(1, isTrue); }
void ujlui::on_setFr_toggled(bool isTrue) { setFiltering(2, isTrue); }
void ujlui::on_setIt_toggled(bool isTrue) { setFiltering(3, isTrue); }
void ujlui::on_setSp_toggled(bool isTrue) { setFiltering(4, isTrue); }

// Tables Item Change

void ujlui::on_cListWidget_itemChanged(QTableWidgetItem *item) { // Cutscene
    if(!uiReady) return;
    int row = item->row();
    int column = item->column();

    int subNum = ui->cListWidget->model()->data(ui->cListWidget->model()->index(row,0)).toInt();
    int cutId = ui->cListWidget->model()->data(ui->cListWidget->model()->index(row,1)).toInt();
    int subId = Cutscenes[cutId].Subtitles[subNum].localisation;

    int subSta = ui->cListWidget->model()->data(ui->cListWidget->model()->index(row, 2)).toInt();
    int subEnd = ui->cListWidget->model()->data(ui->cListWidget->model()->index(row, 3)).toInt();
    QString subStr = ui->cListWidget->model()->data(ui->cListWidget->model()->index(row, 5)).toString();

    switch(column) {
    case 2:
        Cutscenes[cutId].Subtitles[subNum].timestart = subSta;
        break;
    case 3:
        Cutscenes[cutId].Subtitles[subNum].timeend = subEnd;
        break;
    case 5:
        editSubtitleText(subId, subStr);
        updateGUI();
        break;
    }
}

void ujlui::on_gListWidget_itemChanged(QTableWidgetItem *item) { // Gameplay
    if(!uiReady) return;
    int row = item->row();
    int column = item->column();

    int subNum = ui->gListWidget->model()->data(ui->gListWidget->model()->index(row,0)).toInt();
    int gamId = ui->gListWidget->model()->data(ui->gListWidget->model()->index(row,1)).toInt();
    int subId = Gameplays[gamId].Subtitles[subNum].localisation;

    int unknown = ui->cListWidget->model()->data(ui->cListWidget->model()->index(row, 4)).toInt();
    QString subStr = ui->gListWidget->model()->data(ui->gListWidget->model()->index(row, 5)).toString();

    switch(column) {
    case 4:
        Gameplays[gamId].Subtitles[subNum].unknown = unknown;
        break;
    case 5:
        editSubtitleText(subId, subStr);
        updateGUI();
        break;
    }
}

void ujlui::on_aListWidget_itemChanged(QTableWidgetItem *item) { // All
    if(!uiReady) return;
    int row = item->row();

    QString subStr = ui->aListWidget->model()->data(ui->aListWidget->model()->index(row, 1)).toString();
    editSubtitleText(row, subStr);
    updateGUI();
}

void ujlui::on_rListWidget_itemChanged(QTableWidgetItem *item) {
    if(!uiReady) return;
    int row = item->row();

    QString charOr = ui->rListWidget->model()->data(ui->rListWidget->model()->index(row,0)).toString();
    QString charRe = ui->rListWidget->model()->data(ui->rListWidget->model()->index(row,1)).toString();

    ReplacementTable[item->row()].Character = charOr;
    ReplacementTable[item->row()].Replacement = charRe;
}

// Actions

void ujlui::on_actionLink_triggered()
{
    int result = QInputDialog::getInt(0, "Link a subtitle", "Insert the number of the subtitle to link:", 0, 0, Subtitles.size() - 1);
    int curTab = ui->tabWidget->currentIndex();
    int curSub, curCut, curGam;
    bool unLi = false;
    QMessageBox msgBox;
    if(result == 0) {
        Subtitle newSub = { "", (char)-1, false }; // TODO: JP Region
        Subtitles.push_back(newSub);
        Patcher.ApplyChanges(newSub.Position, 0, 0, 0);
        result = Subtitles.size() - 1;
        unLi = true;
    }

    switch(curTab) {
    case 1: // All
        msgBox.setText("You can't link a raw text (subtitle from \"All Subtitles\" tab)");
        msgBox.exec();
        break;
    case 2: // Cutscenes
        curSub = ui->cListWidget->model()->data(ui->cListWidget->model()->index(ui->cListWidget->currentRow(), 0)).toInt();
        curCut = ui->cListWidget->model()->data(ui->cListWidget->model()->index(ui->cListWidget->currentRow(), 1)).toInt();
        Cutscenes[curCut].Subtitles[curSub].localisation = result - 1;
        if(!unLi) Cutscenes[curCut].Subtitles[curSub].localelinked = true;
        break;
    case 3: // Gameplay
        curSub = ui->gListWidget->model()->data(ui->gListWidget->model()->index(ui->gListWidget->currentRow(), 0)).toInt();
        curGam = ui->gListWidget->model()->data(ui->gListWidget->model()->index(ui->gListWidget->currentRow(), 1)).toInt();
        Gameplays[curGam].Subtitles[curSub].localisation = result - 1;
        if(!unLi) Gameplays[curGam].Subtitles[curSub].localelinked = true;
        break;
    }
    updateGUI();
}


void ujlui::on_actionHideBlank_triggered() {
    hideBlanks = !hideBlanks;
    clearGUI();
    startGUI();
}

void ujlui::on_actionHideLink_triggered() {
    clearGUI();
    startGUI();
}

void ujlui::on_actionCheckRight_triggered() {
    QString a;
    bool isDamaged = false;
    int counter = 0;
    for(Subtitle sub : Subtitles) {
        int subSize = sub.Text.toLatin1().size();
        for(Subtitle tub : Subtitles) {
            int tubSize = tub.Text.toLatin1().size();

            if(sub.Position == tub.Position) continue;
            bool damageBehind = (sub.Position > tub.Position) && sub.Position < tub.Position + tubSize;
            bool damageFront  = (sub.Position < tub.Position) && tub.Position < sub.Position + subSize;
            if((damageBehind || damageFront) && counter < 5) {
                isDamaged = true;
                a += sub.Text + "\n";
                a += QString::number(sub.Position) + "\n";
                a += tub.Text + "\n";
                a += QString::number(tub.Position);
                counter++;
            }
        }
    }

    QMessageBox msgBox;
    if(isDamaged) {
        msgBox.setText("A subtitle is replacing another subtitle.\n\n" + a);
    } else {
        msgBox.setText("Nothing seems wrong with the file");
    }
    msgBox.exec();
}

void ujlui::on_actionFind_triggered() {
    QString text = QInputDialog::getText(this, "Find", "Type the text you want to find:");
    findedCurrentL = ui->tabWidget->currentIndex();

    switch(findedCurrentL) {
    case 1:
        findedItems = ui->aListWidget->findItems(text, Qt::MatchContains);
        ui->aListWidget->setCurrentCell(findedItems[0]->row(), findedItems[0]->column());
        break;
    case 2:
        findedItems = ui->cListWidget->findItems(text, Qt::MatchContains);
        ui->cListWidget->setCurrentCell(findedItems[0]->row(), findedItems[0]->column());
        break;
    case 3:
        findedItems = ui->gListWidget->findItems(text, Qt::MatchContains);
        ui->gListWidget->setCurrentCell(findedItems[0]->row(), findedItems[0]->column());
        break;
    }
}

void ujlui::on_actionAbout_triggered() {
    QMessageBox::about(0, "About UJLSDE", "UJLSDE C++\nVersion Nightly\nProgrammed by Zungle & Joo\nTested by Joo"); // damm zungle what i did to you :sob:
}
// Replacement Table Action Buttons

void ujlui::on_addRepButton_clicked()
{
    Replace newRep = {"", ""};
    ReplacementTable.push_back(newRep);
    ui->rListWidget->insertRow(ReplacementTable.size() - 1);
}

void ujlui::on_removeRepButton_clicked()
{
    if(ReplacementTable.size() == 0) return;
    ReplacementTable.erase(ReplacementTable.end());
    ui->rListWidget->removeRow(ui->rListWidget->rowCount() -1);
}

// Export Import
// zungle broke the import export sde project file, and the sder

void ujlui::on_actionImportP_triggered() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open SDE Project File"), "", tr("SDE Project File (*.sde1);;Any File (*.*)"));
    Importer.LoadProject(fileName);
}

void ujlui::on_actionImportR_triggered() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open SDE Char. Replace File"), "", tr("SDE Char. Replace File (*.sder);;Any File (*.*)"));
    Importer.LoadReplaceTable(fileName);
}

void ujlui::on_actionExportP_triggered() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save SDE Project File"), "", tr("SDE Project File (*.sde1);;Any File (*.*)"));
    Exporter.SaveProject(fileName+".sde1");
}

void ujlui::on_actionExportR_triggered() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save SDE Char. Replace File"), "", tr("SDE Char. Replace File (*.sder);;Any File (*.*)"));
    Exporter.SaveReplaceTable(fileName+".sder");
}
