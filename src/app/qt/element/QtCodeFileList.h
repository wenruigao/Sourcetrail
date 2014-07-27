#ifndef QT_CODE_FILE_LIST
#define QT_CODE_FILE_LIST

#include <memory>
#include <vector>

#include <QFrame>
#include <QScrollArea>

#include "utility/types.h"

class QtCodeFile;
class TokenLocationFile;

class QtCodeFileList: public QScrollArea
{
	Q_OBJECT

public:
	QtCodeFileList(QWidget* parent = 0);
	virtual ~QtCodeFileList();

	void addCodeSnippet(
		int startLineNumber,
		const std::string& code,
		const TokenLocationFile& locationFile,
		const std::vector<Id>& activeTokenIds
	);

	void clearCodeSnippets();

private:
	std::shared_ptr<QFrame> m_frame;
	std::vector<std::shared_ptr<QtCodeFile> > m_files;
};

#endif // QT_CODE_FILE_LIST
