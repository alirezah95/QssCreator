#ifndef DOCUMENTOPERATIONS_H
#define DOCUMENTOPERATIONS_H

#include <QObject>

class QTextDocument;
class QTextEdit;
class IDocumentFile;

/*!
 * \brief The DocumentOperations class reponsible for doing file operations
 * related to the style sheet document
 */
class DocumentOperations : public QObject
{
    Q_OBJECT
public:
    explicit DocumentOperations(QObject* parent = nullptr);

    /*!
     * \brief Sets a new empty, untitled document to the \a editor
     * \param editor Pointer to a \ref StyleSheetEditor to set the new document
     * on
     * \return True if successful and false otherwise
     */
    bool newFile(QTextEdit* editor);

    /*!
     * \brief Opens a file and sets it as the text document of the \a editor
     * \param editor Pointer to a \ref StyleSheetEditor to set the opened
     * document on
     * \param docFile A pointer to an \ref IDocumentFile
     * \return True if successful and false otherwise
     */
    bool openFile(QTextEdit* editor, IDocumentFile* docFile);

    /*!
     * \brief Saves the document from \a doc in \a file name.
     * \param editor Pointer to a \ref StyleSheetEditor that its document should
     * be saved
     * \param docFile A pointer to an \ref IDocumentFile
     * \return True if successful and false other wise
     */
    bool save(const QTextEdit* editor, IDocumentFile* outFile);
};

#endif // DOCUMENTOPERATIONS_H
