#ifndef IQSSDPROCESSOR_H
#define IQSSDPROCESSOR_H

#include <QObject>

class QTextDocument;
class IQssdVariablesModel;

/*!
 * \brief The IQssdProcessor class providing the interface of a document
 * processor which is used with a \a\b QTextDocument and \ref IQssdEditor
 */
class IQssdProcessor : public QObject
{
    Q_OBJECT
public:
    explicit IQssdProcessor(QObject* parent = nullptr);
    virtual ~IQssdProcessor();

    /*!
     * \brief Set the list models of the variables
     * \param varsModel
     */
    virtual void setVariablesModel(IQssdVariablesModel* varsModel) = 0;

    /*!
     * \brief getVariablesModel
     * \return
     */
    virtual IQssdVariablesModel* getVariablesModel() const = 0;

    /*!
     * \brief Searches for the variables in the document and updates the vars
     * model
     * \return A \a\b QString containing the processed content of the document
     */
    virtual QString processDocument(
        QTextDocument* editor, bool updateModel = true)
        = 0;
};

#endif // IQSSDPREPROCESSOR_H
