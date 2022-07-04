#ifndef IUSERDIALOGS_H
#define IUSERDIALOGS_H

#include <QFileDialog>
#include <QMessageBox>
#include <QObject>

/*!
 * \brief The IUserDialogs class is an interface for classes which provide user
 * dialogs, like save file, open file, user input and etc dialogs. This is
 * required to be able to fake them out in unit tests
 */
class IUserDialogs : public QObject
{
    Q_OBJECT
public:
    explicit IUserDialogs(QObject* parent = nullptr);
    virtual ~IUserDialogs();

    // File dialogs
    using Options = QFileDialog::Options;

    virtual QString getOpenFileName(QWidget* parent = nullptr,
        const QString& caption = QString(), const QString& dir = QString(),
        const QString& filter = QString(), QString* selectedFilter = nullptr,
        Options options = Options())
        = 0;

    virtual QString getSaveFileName(QWidget* parent = nullptr,
        const QString& caption = QString(), const QString& dir = QString(),
        const QString& filter = QString(), QString* selectedFilter = nullptr,
        Options options = Options())
        = 0;

    virtual QString getExistingDirectory(QWidget* parent = nullptr,
        const QString& caption = QString(), const QString& dir = QString(),
        Options options = QFileDialog::ShowDirsOnly)
        = 0;

    virtual QStringList getOpenFileNames(QWidget* parent = nullptr,
        const QString& caption = QString(), const QString& dir = QString(),
        const QString& filter = QString(), QString* selectedFilter = nullptr,
        Options options = Options())
        = 0;

    // Message box dialogs
    using StandardButton = QMessageBox::StandardButton;
    using StandardButtons = QMessageBox::StandardButtons;

    virtual StandardButton information(QWidget* parent, const QString& title,
        const QString& text, StandardButton button0,
        StandardButton button1 = QMessageBox::NoButton)
        = 0;

    virtual StandardButton question(QWidget* parent, const QString& title,
        const QString& text,
        StandardButtons buttons
        = StandardButtons(QMessageBox::Yes | QMessageBox::No),
        StandardButton defaultButton = QMessageBox::NoButton)
        = 0;

    virtual StandardButton warning(QWidget* parent, const QString& title,
        const QString& text, StandardButtons buttons = QMessageBox::Ok,
        StandardButton defaultButton = QMessageBox::NoButton)
        = 0;

    virtual StandardButton critical(QWidget* parent, const QString& title,
        const QString& text, StandardButtons buttons = QMessageBox::Ok,
        StandardButton defaultButton = QMessageBox::NoButton)
        = 0;

    virtual void about(
        QWidget* parent, const QString& title, const QString& text)
        = 0;
    virtual void aboutQt(QWidget* parent, const QString& title = QString()) = 0;
};

#endif // IUSERDIALOGS_H
