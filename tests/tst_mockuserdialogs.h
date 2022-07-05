#include "iuserdialogs.h"

#include <gmock/gmock.h>

class MockUserDialogs : public IUserDialogs
{
public:
    explicit MockUserDialogs() : IUserDialogs(nullptr) {};
    virtual ~MockUserDialogs() {};

    MOCK_METHOD(QString, getOpenFileName,
        (QWidget * parent, const QString& caption, const QString& dir,
            const QString& filter, QString* selectedFilter, Options options),
        (override));

    MOCK_METHOD(QString, getSaveFileName,
        (QWidget * parent, const QString& caption, const QString& dir,
            const QString& filter, QString* selectedFilter, Options options),
        (override));

    MOCK_METHOD(QString, getExistingDirectory,
        (QWidget * parent, const QString& caption, const QString& dir,
            Options options),
        (override));

    MOCK_METHOD(QStringList, getOpenFileNames,
        (QWidget * parent, const QString& caption, const QString& dir,
            const QString& filter, QString* selectedFilter, Options options),
        (override));

    MOCK_METHOD(StandardButton, information,
        (QWidget * parent, const QString& title, const QString& text,
            StandardButton button0, StandardButton button1),
        (override));

    MOCK_METHOD(StandardButton, question,
        (QWidget * parent, const QString& title, const QString& text,
            StandardButtons buttons, StandardButton defaultButton),
        (override));

    MOCK_METHOD(StandardButton, warning,
        (QWidget * parent, const QString& title, const QString& text,
            StandardButtons buttons, StandardButton defaultButton),
        (override));

    MOCK_METHOD(StandardButton, critical,
        (QWidget * parent, const QString& title, const QString& text,
            StandardButtons buttons, StandardButton defaultButton),
        (override));

    MOCK_METHOD(void, about,
        (QWidget * parent, const QString& title, const QString& text),
        (override));

    MOCK_METHOD(
        void, aboutQt, (QWidget * parent, const QString& title), (override));
};
