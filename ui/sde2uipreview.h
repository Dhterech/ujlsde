#ifndef SDE2UIPREVIEW_H
#define SDE2UIPREVIEW_H

#include <QWidget>
#include <QFontDatabase>
#include "types.h"

namespace Ui {
class sde2uipreview;
}

class sde2uipreview : public QWidget
{
    Q_OBJECT

public:
    explicit sde2uipreview(QWidget *parent = nullptr);
    ~sde2uipreview();
    void SetupPreview(Subtitle subtitle);

private:
    Ui::sde2uipreview *ui;
};

#endif // SDE2UIPREVIEW_H
