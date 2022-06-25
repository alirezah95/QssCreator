#ifndef DOCUMENTOPERATIONS_H
#define DOCUMENTOPERATIONS_H

#include <QObject>

class QTextDocument;
class StyleSheetEditor;

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
    bool newFile(StyleSheetEditor* editor);

    /*!
     * \brief Opens a file and sets it as the text document of the \a editor
     * \param editor Pointer to a \ref StyleSheetEditor to set the opened
     * document on
     * \param filename Path to the file to open
     * \return True if successful and false otherwise
     */
    bool openFile(StyleSheetEditor* editor, const QString& filename);

    /*!
     * \brief Saves the document from \a doc in \a file name.
     * \details If file name is empty (default) the file name is retrieved from
     * the \a dco
     * \param editor Pointer to a \ref StyleSheetEditor that its document should
     * be saved
     * \param filename The file name to save the document in
     * \return True if successful and false other wise
     */
    bool save(
        const StyleSheetEditor* editor, const QString& filename = QString());
};

#endif // DOCUMENTOPERATIONS_H
