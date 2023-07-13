#ifndef SCREENSHOTDISPLAYDIALOG_H
#define SCREENSHOTDISPLAYDIALOG_H

#include <QDialog>
#include <QGraphicsScene>

namespace Ui {
class DisplayInfoDialog;
}

class DisplayInfoDialog : public QDialog {
    Q_OBJECT

public:
    explicit DisplayInfoDialog(QWidget* parent = nullptr);
    ~DisplayInfoDialog();

    void setDialogBackground(QColor color);

    void setTextAlignment(Qt::Alignment alignment);

    void setFont(QString family, int size);

    void setTextColor(QColor color);

    void setHeadShotCount(short count);

    void setTime(int m, int s, int ms);

    static const QString timePattern;

private:
    void setTouchable(bool touchable);

    void setChildrenTransparentForMouseEvents(bool transparent = true);

    Ui::DisplayInfoDialog* ui;

    QPointF mousePressedPos;

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
};

#endif // SCREENSHOTDISPLAYDIALOG_H
