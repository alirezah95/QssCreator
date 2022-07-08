#ifndef QSSDPREPROCESSOR_H
#define QSSDPREPROCESSOR_H

#include <QObject>
#include <QRegularExpression>

#include "iqssdpreprocessor.h"

class QTextEdit;
class IQssdVariablesModel;

/*!
 * \brief The QssdPreprocessor class which is responsible for processing a
 * document inside a \ref QTextEdit and updating the variables in document
 * with a \ref IQssdVariableModel and vice versa. And also it is responsible for
 * replaced variables occurences in a document with the actual value of the
 * variable
 */
class QssdPreprocessor : public IQssdPreprocessor
{
    Q_OBJECT
public:
    explicit QssdPreprocessor(QObject* parent = nullptr);
    QssdPreprocessor(IQssdVariablesModel* varsModel, QObject* parent = nullptr);
    ~QssdPreprocessor();

    /*!
     * \brief Implements \ref IQssdPreprocessor::setVariablesModel to set the
     * list models of the variables
     * \param varsModel
     */
    void setVariablesModel(IQssdVariablesModel* varsModel);

    /*!
     * \brief Implements \ref IQssdPreprocessor::getProcessedDocumentContent()
     * to processes the document, replaces all variables with theri values and
     * returning a \a\b QString as the result
     * \return \a\b QString as the result of processing
     */
    QString getProcessedDocumentContent(QTextEdit* editor);

    /*!
     * \brief Implements \ref IQssdPreprocessor::processDocumentVariables() to
     * searches for the variables in the document and updates the vars model
     */
    void processDocumentVariables(QTextEdit* editor);

private:
    QRegularExpression mVarDefineRegex; /*!< This regex is used to find the
                                         * variable definitions inside the
                                         * editor document
                                         */

    QRegularExpression mVarUsageRegex; /*!< This regex is used to find the usage
                                        * of variable inside the editor docuemnt
                                        * and replacing them with actual values.
                                        */

    IQssdVariablesModel* mVarsModel; /*!< A list model used to store the
                                      * variables of a qssd document.
                                      */
};

#endif // QSSDPREPROCESSOR_H
