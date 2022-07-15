#ifndef QSSDPROCESSOR_H
#define QSSDPROCESSOR_H

#include <QObject>
#include <QRegularExpression>

#include "iqssdprocessor.h"

class QTextEdit;
class IQssdVariablesModel;

/*!
 * \brief The QssdProcessor class which is responsible for processing a
 * document inside a \ref QTextEdit and updating the variables in document
 * with a \ref IQssdVariableModel and vice versa. And also it is responsible for
 * replaced variables occurences in a document with the actual value of the
 * variable
 */
class QssdProcessor : public IQssdProcessor
{
    Q_OBJECT
public:
    QssdProcessor(IQssdVariablesModel* varsModel, QObject* parent = nullptr);
    virtual ~QssdProcessor();

    /*!
     * \brief Implements \ref IQssdProcessor::setVariablesModel to set the
     * list models of the variables
     * \param varsModel
     */
    virtual void setVariablesModel(IQssdVariablesModel* varsModel) override;

    /*!
     * \brief Implements \ref IQssdProcessor::processDocument() to
     * searches for the variables in the document and updates the vars model
     */
    virtual void processDocument(QTextDocument* doc, bool updateModel) override;

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
