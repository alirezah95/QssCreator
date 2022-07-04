#ifndef USERDIALOG_H
#define USERDIALOG_H

#include "iuserdialogs.h"

/*!
 * \brief The UserDialogs class which is a concrete derived class for the \ref
 * IUserDialogs class
 */
class UserDialogs : public IUserDialogs
{
    Q_OBJECT
public:
    explicit UserDialogs(QObject* parent = nullptr);
    virtual ~UserDialogs();

    virtual QString getOpenFileName(QWidget* parent, const QString& caption,
        const QString& dir, const QString& filter, QString* selectedFilter,
        Options options) override;

    virtual QString getSaveFileName(QWidget* parent, const QString& caption,
        const QString& dir, const QString& filter, QString* selectedFilter,
        Options options) override;

    virtual QString getExistingDirectory(QWidget* parent,
        const QString& caption, const QString& dir, Options options) override;

    virtual QStringList getOpenFileNames(QWidget* parent,
        const QString& caption, const QString& dir, const QString& filter,
        QString* selectedFilter, Options options) override;

    virtual StandardButton information(QWidget* parent, const QString& title,
        const QString& text, StandardButton button0,
        StandardButton button1) override;

    virtual StandardButton question(QWidget* parent, const QString& title,
        const QString& text, StandardButtons buttons,
        StandardButton defaultButton) override;

    virtual StandardButton warning(QWidget* parent, const QString& title,
        const QString& text, StandardButtons buttons,
        StandardButton defaultButton) override;

    virtual StandardButton critical(QWidget* parent, const QString& title,
        const QString& text, StandardButtons buttons,
        StandardButton defaultButton) override;

    virtual void about(
        QWidget* parent, const QString& title, const QString& text) override;

    virtual void aboutQt(QWidget* parent, const QString& title) override;
};

inline UserDialogs::UserDialogs(QObject* parent) : IUserDialogs { parent } { }

inline UserDialogs::~UserDialogs() { }

inline QString UserDialogs::getOpenFileName(QWidget* parent,
    const QString& caption, const QString& dir, const QString& filter,
    QString* selectedFilter, Options options)
{
    return QFileDialog::getOpenFileName(
        parent, caption, dir, filter, selectedFilter, options);
}

inline QString UserDialogs::getSaveFileName(QWidget* parent,
    const QString& caption, const QString& dir, const QString& filter,
    QString* selectedFilter, Options options)
{
    return QFileDialog::getSaveFileName(
        parent, caption, dir, filter, selectedFilter, options);
}

inline QString UserDialogs::getExistingDirectory(QWidget* parent,
    const QString& caption, const QString& dir, Options options)
{
    return QFileDialog::getExistingDirectory(parent, caption, dir, options);
}

inline QStringList UserDialogs::getOpenFileNames(QWidget* parent,
    const QString& caption, const QString& dir, const QString& filter,
    QString* selectedFilter, Options options)
{
    return QFileDialog::getOpenFileNames(
        parent, caption, dir, filter, selectedFilter, options);
}

inline IUserDialogs::StandardButton UserDialogs::information(QWidget* parent,
    const QString& title, const QString& text, StandardButton button0,
    StandardButton button1)
{
    return QMessageBox::information(parent, title, text, button0, button1);
}

inline IUserDialogs::StandardButton UserDialogs::question(QWidget* parent,
    const QString& title, const QString& text, StandardButtons buttons,
    StandardButton defaultButton)
{
    return QMessageBox::question(parent, title, text, buttons, defaultButton);
}

inline IUserDialogs::StandardButton UserDialogs::warning(QWidget* parent,
    const QString& title, const QString& text, StandardButtons buttons,
    StandardButton defaultButton)
{
    return QMessageBox::warning(parent, title, text, buttons, defaultButton);
}

inline IUserDialogs::StandardButton UserDialogs::critical(QWidget* parent,
    const QString& title, const QString& text, StandardButtons buttons,
    StandardButton defaultButton)
{
    return QMessageBox::critical(parent, title, text, buttons, defaultButton);
}

inline void UserDialogs::about(
    QWidget* parent, const QString& title, const QString& text)
{
    return QMessageBox::about(parent, title, text);
}

inline void UserDialogs::aboutQt(QWidget* parent, const QString& title)
{
    return QMessageBox::aboutQt(parent, title);
}

#endif // USERDIALOG_H
