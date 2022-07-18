#ifndef DOCUMENTOPERATIONS_H
#define DOCUMENTOPERATIONS_H

#include <QObject>

#include "iqssdfileoperations.h"

class QTextDocument;
class QTextEdit;
class IDocumentFile;

/*!
 * \brief The DocumentOperations class reponsible for doing file operations
 * related to the style sheet document
 */
class QssdFileOperations : public IQssdFileOperations
{
    Q_OBJECT
public:
    explicit QssdFileOperations(QObject* parent = nullptr);
    virtual ~QssdFileOperations();

    /*!
     * \brief Implements \ref IQssdFileOperatios::newDocument
     * \param editor
     * \return
     */
    virtual bool newDocument(QTextEdit* editor) override;

    /*!
     * \brief Implements \ref IQssdFileOperatios::openDocument
     * \param editor
     * \param docFile
     * \return
     */
    virtual bool openDocument(
        QTextEdit* editor, IDocumentFile* docFile) override;

    /*!
     * \brief Implements \ref IQssdFileOperatios::saveDocument
     * \param editor
     * \param outFile
     * \return
     */
    virtual bool saveDocument(
        const QTextEdit* editor, IDocumentFile* outFile) override;

    /*!
     * \brief Implements \ref IQssdFileOperatios::exportDocument
     * \param editor
     * \param outFile
     * \return
     */
    virtual bool exportDocument(
        const QString& docContent, IDocumentFile* outFile) override;
};

#endif // DOCUMENTOPERATIONS_H
