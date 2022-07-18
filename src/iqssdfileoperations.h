#ifndef IQSSDOPERATIONS_H
#define IQSSDOPERATIONS_H

#include <QObject>

class QTextEdit;
class IDocumentFile;

class IQssdFileOperations : public QObject
{
    Q_OBJECT
public:
    explicit IQssdFileOperations(QObject* parent = nullptr);
    virtual ~IQssdFileOperations();

    /*!
     * \brief Sets a new empty, untitled document to the \a editor
     * \param editor Pointer to a \ref StyleSheetEditor to set the new document
     * on
     * \return True if successful and false otherwise
     */
    virtual bool newDocument(QTextEdit* editor) = 0;

    /*!
     * \brief Opens a file and sets it as the text document of the \a editor
     * \param editor Pointer to a \ref StyleSheetEditor to set the opened
     * document on
     * \param docFile A pointer to an \ref IDocumentFile
     * \return True if successful and false otherwise
     */
    virtual bool openDocument(QTextEdit* editor, IDocumentFile* docFile) = 0;

    /*!
     * \brief Saves the document from \a doc in \a file name.
     * \param editor Pointer to a \ref StyleSheetEditor that its document should
     * be saved
     * \param outFile A pointer to an \ref IDocumentFile which is used as the
     * output file (path)
     * \return True if successful and false other wise
     */
    virtual bool saveDocument(const QTextEdit* editor, IDocumentFile* outFile)
        = 0;

    /*!
     * \brief Export the current document as a <em>Qt style sheet</em> document
     * to the file specified in \ref IDocumentFile
     * \param doc A \a\b QString containing the export content
     * \param outFile An \ref IDocumentFile used to export the document to file
     * storage
     * \return True if successfull and false otherwise
     */
    virtual bool exportDocument(const QString& doc, IDocumentFile* outFile) = 0;
};

#endif
