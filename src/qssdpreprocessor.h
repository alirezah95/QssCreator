#ifndef QSSDPREPROCESSOR_H
#define QSSDPREPROCESSOR_H

#include <QObject>
#include <QRegularExpression>

class QTextEdit;

/*!
 * \brief The QssdPreprocessor class which is responsible for processing a
 * document inside a \ref QTextEdit and updating the variables in document
 * with a \ref IQssdVariableModel and vice versa. And also it is responsible for
 * replaced variables occurences in a document with the actual value of the
 * variable
 */
class QssdPreprocessor : public QObject
{
    Q_OBJECT
public:
    explicit QssdPreprocessor(QObject* parent = nullptr);
    QssdPreprocessor(QTextEdit* editor, QObject* parent = nullptr);
    ~QssdPreprocessor();

    /*!
     * \brief Sets the internal pointer to a \ref QTextEdit
     * \param editor A \ref QTextEdit
     */
    void setQssdEditor(QTextEdit* editor);

    /*!
     * \brief Processes the document, replaces all variables with theri values
     * and returning a \a\b QString as the result
     * \return \a\b QString as the result of processing
     */
    QString getProcessedDocumentContent();

private slots:
    void preProcessDocument(bool docIsModified);

private:
    QTextEdit* mEditor;

    QRegularExpression mVarDefineRegex; /*!< This regex is used to find the
                                         * variable definitions inside the
                                         * editor document
                                         */

    QRegularExpression mVarUsageRegex; /*!< This regex is used to find the usage
                                        * of variable inside the editor docuemnt
                                        * and replacing them with actual values.
                                        */
};

#endif // QSSDPREPROCESSOR_H
