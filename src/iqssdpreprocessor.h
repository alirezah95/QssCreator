#ifndef IQSSDPREPROCESSOR_H
#define IQSSDPREPROCESSOR_H

#include <QObject>

class QTextEdit;
class IQssdVariablesModel;

class IQssdPreprocessor : public QObject
{
    Q_OBJECT
public:
    explicit IQssdPreprocessor(QObject* parent = nullptr);
    virtual ~IQssdPreprocessor();

    /*!
     * \brief Set the list models of the variables
     * \param varsModel
     */
    virtual void setVariablesModel(IQssdVariablesModel* varsModel) = 0;

    /*!
     * \brief Processes the document, replaces all variables with theri values
     * and returning a \a\b QString as the result
     * \return \a\b QString as the result of processing
     */
    virtual QString getProcessedDocumentContent(QTextEdit* editor) = 0;

    /*!
     * \brief Searches for the variables in the document and updates the vars
     * model
     */
    virtual void processDocumentVariables(QTextEdit* editor) = 0;
};

#endif // IQSSDPREPROCESSOR_H
