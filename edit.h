#ifndef EDITOR_H
#define EDITOR_H

#include "completionpopup.h"
#include <QPlainTextEdit>

class Edit : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit Edit(QWidget *parent = 0);
    void setCompleter();

protected:
    void keyPressEvent(QKeyEvent *e);
    void focusInEvent(QFocusEvent *e);

public slots:
    void insertCompletion(const QString &completion);
    void toEmphasis();
    void toStrong();
    void toH1();
    void toH2();
    void toH3();

private:
    QString textUnderCursor() const;
    QString textSelected() const;
    QString textOnLine() const;
    QString textOnBlock() const;
    QString textMaybe() const;
    void replaceTextMaybe(const QString &text);

    CompletionPopup *completionPopup;

};

#endif // EDITOR_H
