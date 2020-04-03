#include "labeleditor.h"

LabelEditor::LabelEditor(QWidget *parent) : QPlainTextEdit(parent)
{
    setObjectName("TimelineEdit");
    setMaximumBlockCount(1);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void LabelEditor::setOriginText(QString text)
{
    origin_text = text;
}

QString LabelEditor::getOriginText()
{
    return origin_text;
}

void LabelEditor::keyPressEvent(QKeyEvent *event)
{
    auto key = event->key();
    auto modifiers = event->modifiers();

    if (key == Qt::Key_Escape)
    {
        emit signalEditCanceled(origin_text);
        this->hide();
        return ;
    }
    else if (key == Qt::Key_Return || key == Qt::Key_Enter)
    {
        emit signalEditFinished(toPlainText());
        hide();
        return ;
    }

    return QPlainTextEdit::keyPressEvent(event);
}
