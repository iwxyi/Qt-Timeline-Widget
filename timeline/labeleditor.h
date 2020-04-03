#ifndef LABELEDITOR_H
#define LABELEDITOR_H

#include <QObject>
#include <QPlainTextEdit>
#include <QLabel>

class LabelEditor : public QPlainTextEdit
{
public:
    LabelEditor(QWidget* parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent *event) override;
};

#endif // LABELEDITOR_H
