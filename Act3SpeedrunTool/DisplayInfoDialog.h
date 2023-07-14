#ifndef SCREENSHOTDISPLAYDIALOG_H
#define SCREENSHOTDISPLAYDIALOG_H

#include "GlobalData.h"
#include <QDialog>

namespace Ui {
class DisplayInfoDialog;
}

class DisplayInfoDialog : public QDialog {
    Q_OBJECT

public:
    explicit DisplayInfoDialog(QWidget* parent = nullptr);
    ~DisplayInfoDialog();

    void setDialogBackground(QColor color = GlobalData::displayInfoBackground);

    void setDisplay();

    void setTextAlignment();

    void setFont();

    void setTextStyle();

    void setHeadShotCount(short count);

    void setTime(int m, int s, int ms);

    static const QString timePattern;

    static const QString textQssPattern;

private:
    void setTouchable(bool touchable = GlobalData::displayInfoTouchable);

    void setChildrenTransparentForMouseEvents(bool transparent = true);

    Ui::DisplayInfoDialog* ui;

    QPointF mousePressedPos;

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
};

#endif // SCREENSHOTDISPLAYDIALOG_H
