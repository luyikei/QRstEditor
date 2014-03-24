#ifndef CANDIDATEPOPUP_H
#define CANDIDATEPOPUP_H

#include <QCompleter>
#include <QScrollBar>
#include <QAbstractItemView>
class CompletionPopup : public QCompleter
{
    Q_OBJECT
public:
    explicit CompletionPopup(QWidget *parent = 0);
signals:

public slots:

private:

    QAbstractItemModel *modelFromFile(/*const QString& fileName*/);

};

#endif // CANDIDATEPOPUP_H
