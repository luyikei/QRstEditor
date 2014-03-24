#include "edit.h"
#include <QDebug>
#include <QRegExp>

Edit::Edit(QWidget *parent) :
    QPlainTextEdit(parent),
    completionPopup(new CompletionPopup(this))
{
    setCompleter();
}

void Edit::insertCompletion(const QString &completion)
{
    if (completionPopup->widget() != this)
        return;
    QTextCursor tc = textCursor();
    int extra = completion.length() - completionPopup->completionPrefix().length();
    tc.movePosition(QTextCursor::Left);
    tc.movePosition(QTextCursor::EndOfWord);
    tc.insertText(completion.right(extra));
    setTextCursor(tc);

}

void Edit::toEmphasis()
{
    QRegExp r("^[^\\*]*$");
    QRegExp r2("\\*(.*)\\*");
    QString baseString = textMaybe();
    QString blockString = textOnBlock();
    blockString.replace(r2,"");
    if (r.exactMatch(baseString) && blockString.contains(baseString)){
        baseString.replace("*","");
        replaceTextMaybe(QString("*%1*").arg(baseString));
    }else{
        baseString.replace("*","");
        replaceTextMaybe(baseString);
    }
}

void Edit::toStrong()
{
    QRegExp r("^[^(\\*\\*)]*$");
    QRegExp r2("\\*\\*(.*)\\*\\*");
    QString baseString = textMaybe();
    QString blockString = textOnBlock();
    blockString.replace(r2,"");
    if (r.exactMatch(baseString) && blockString.contains(baseString)){
        baseString.replace("**","");
        replaceTextMaybe(QString("**%1**").arg(baseString));
    }else{
        baseString.replace("**","");
        replaceTextMaybe(baseString);
    }

}

void Edit::toH1()
{

}

void Edit::toH2()
{

}

void Edit::toH3()
{

}

QString Edit::textUnderCursor() const
{
    QTextCursor tc = textCursor();
    tc.select(QTextCursor::WordUnderCursor);
    return tc.selectedText();
}

QString Edit::textSelected() const
{
    QTextCursor tc = textCursor();
    return tc.selectedText();
}

QString Edit::textOnLine() const
{
    QTextCursor tc = textCursor();
    tc.select(QTextCursor::LineUnderCursor);
    return tc.selectedText();
}

QString Edit::textOnBlock() const
{

    QTextCursor tc = textCursor();
    tc.select(QTextCursor::BlockUnderCursor);
    return tc.selectedText();
}

QString Edit::textMaybe() const
{
    if(textSelected().isEmpty()){
        return textOnLine();
    }else{
        return textSelected();
    }
}

void Edit::replaceTextMaybe(const QString &text)
{
    QTextCursor tc = textCursor();
    if(tc.selectedText().isEmpty())
        tc.select(QTextCursor::LineUnderCursor);
    tc.removeSelectedText();
    tc.insertText(text);
}

void Edit::setCompleter()
{
    completionPopup->setWidget(this);
    completionPopup->setCompletionMode(QCompleter::PopupCompletion);
    completionPopup->setCaseSensitivity(Qt::CaseInsensitive);
    connect(completionPopup, SIGNAL(activated(QString)),
            this, SLOT(insertCompletion(QString)));
}

void Edit::keyPressEvent(QKeyEvent *e)
{
    if(e->modifiers() & Qt::ControlModifier && e->key() == Qt::Key_Plus){
        QTextCursor tc = textCursor();
        QString baseString = textOnLine();
        tc.select(QTextCursor::LineUnderCursor);
        tc.removeSelectedText();
        QString toAdd = "";
        for(int i=0;i < baseString.length(); i++){
            toAdd.append("=");
        }
        tc.insertText(QString("%1\n%2\n%3").arg(toAdd).arg(baseString).arg(toAdd));
        return;
    }

    if (completionPopup && completionPopup->popup()->isVisible()) {
        // The following keys are forwarded by the completer to the widget
       switch (e->key()) {
       case Qt::Key_Enter:
       case Qt::Key_Return:
       case Qt::Key_Escape:
       case Qt::Key_Tab:
       case Qt::Key_Backtab:
            e->ignore();
            return; // let the completer do default behavior
       default:
           break;
       }
    }

    bool isShortcut = ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_Space); // CTRL+SPACE
    if (!completionPopup || !isShortcut) // do not process the shortcut when we have a completer
        QPlainTextEdit::keyPressEvent(e);
    const bool ctrlOrShift = e->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);
    if (!completionPopup || (ctrlOrShift && e->text().isEmpty()))
        return;

    static QString eow("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-="); // end of word
    bool hasModifier = (e->modifiers() != Qt::NoModifier) && !ctrlOrShift;
    QString completionPrefix = textUnderCursor();

    if (!isShortcut && (hasModifier || e->text().isEmpty()|| completionPrefix.length() < 3
                      || eow.contains(e->text().right(1)))) {
        completionPopup->popup()->hide();
        return;
    }

    if (completionPrefix != completionPopup->completionPrefix()) {
        completionPopup->setCompletionPrefix(completionPrefix);
        completionPopup->popup()->setCurrentIndex(completionPopup->completionModel()->index(0, 0));
    }
    QRect cr = cursorRect();
    cr.setWidth(completionPopup->popup()->sizeHintForColumn(0)
                + completionPopup->popup()->verticalScrollBar()->sizeHint().width());
    completionPopup->complete(cr); // popup it up!
}

void Edit::focusInEvent(QFocusEvent *e)
{

    if (completionPopup)
        completionPopup->setWidget(this);
    QPlainTextEdit::focusInEvent(e);
}
