#include "completionpopup.h"
#include <QStringList>
#include <QStringListModel>
CompletionPopup::CompletionPopup(QWidget *parent) :
    QCompleter(parent)
{
    setModel(modelFromFile());
    setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    setCaseSensitivity(Qt::CaseInsensitive);
    setWrapAround(false);
}

QAbstractItemModel *CompletionPopup::modelFromFile(/*const QString &fileName*/)
{

//    QFile file(fileName);
//    if (!file.open(QFile::ReadOnly))
//        return new QStringListModel(completer);

//#ifndef QT_NO_CURSOR
//    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
//#endif
//    QStringList words;

//    while (!file.atEnd()) {
//        QByteArray line = file.readLine();
//        if (!line.isEmpty())
//            words << line.trimmed();
//    }

//#ifndef QT_NO_CURSOR
//    QApplication::restoreOverrideCursor();
//#endif
//    return new QStringListModel(words, completer);

    QStringList wordList;
    wordList << "zasa"
             << "zbasf"
             << "zcasf"
             << "zeta";
    return new QStringListModel(wordList, this);
}
