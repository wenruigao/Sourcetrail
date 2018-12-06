#include "QtProjectWizzardContent.h"

#include <thread>

#include "QtTextEditDialog.h"
#include "utility.h"
#include "utilityString.h"

QtProjectWizzardContent::QtProjectWizzardContent(QtProjectWizzardWindow* window)
	: QWidget(window)
	, m_window(window)
	, m_isInForm(false)
	, m_showFilesFunctor(std::bind(&QtProjectWizzardContent::showFilesDialog, this, std::placeholders::_1))
{
}

void QtProjectWizzardContent::populate(QGridLayout* layout, int& row)
{
}

void QtProjectWizzardContent::windowReady()
{
}

void QtProjectWizzardContent::load()
{
}

void QtProjectWizzardContent::save()
{
}

bool QtProjectWizzardContent::check()
{
	return true;
}

bool QtProjectWizzardContent::isScrollAble() const
{
	return false;
}

std::vector<FilePath> QtProjectWizzardContent::getFilePaths() const
{
	return {};
}

QString QtProjectWizzardContent::getFileNamesTitle() const
{
	return "File List";
}

QString QtProjectWizzardContent::getFileNamesDescription() const
{
	return "files";
}

bool QtProjectWizzardContent::isInForm() const
{
	return m_isInForm;
}

void QtProjectWizzardContent::setIsInForm(bool isInForm)
{
	m_isInForm = isInForm;
}

QLabel* QtProjectWizzardContent::createFormLabel(QString name) const
{
	QLabel* label = new QLabel(name);
	label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	label->setObjectName("label");
	label->setWordWrap(true);
	return label;
}

QLabel* QtProjectWizzardContent::createFormTitle(QString name) const
{
	QLabel* label = new QLabel(name);
	label->setObjectName("titleLabel");
	label->setWordWrap(true);
	return label;
}

QToolButton* QtProjectWizzardContent::createSourceGroupButton(QString name, QString iconPath) const
{
	QToolButton* button = new QToolButton();
	button->setObjectName("sourceGroupButton");
	button->setText(name);
	button->setIcon(QPixmap(iconPath));
	button->setIconSize(QSize(64, 64));
	button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	button->setCheckable(true);
	return button;
}

QtHelpButton* QtProjectWizzardContent::addHelpButton(const QString& helpTitle, const QString& helpText, QGridLayout* layout, int row) const
{
	QtHelpButton* button = new QtHelpButton(helpTitle, helpText);
	layout->addWidget(button, row, QtProjectWizzardWindow::HELP_COL, Qt::AlignTop);
	return button;
}

QPushButton* QtProjectWizzardContent::addFilesButton(QString name, QGridLayout* layout, int row) const
{
	QPushButton* button = new QPushButton(name);
	button->setObjectName("windowButton");
	button->setAttribute(Qt::WA_LayoutUsesWidgetRect); // fixes layouting on Mac
	if (layout)
	{
		layout->addWidget(button, row, QtProjectWizzardWindow::BACK_COL, Qt::AlignRight | Qt::AlignTop);
	}
	connect(button, &QPushButton::clicked, this, &QtProjectWizzardContent::filesButtonClicked);

	return button;
}

QFrame* QtProjectWizzardContent::addSeparator(QGridLayout* layout, int row) const
{
	QFrame* separator = new QFrame();
	separator->setFrameShape(QFrame::HLine);

	QPalette palette = separator->palette();
	palette.setColor(QPalette::WindowText, Qt::lightGray);
	separator->setPalette(palette);

	layout->addWidget(separator, row, 0, 1, -1);
	return separator;
}

void QtProjectWizzardContent::filesButtonClicked()
{
	m_window->saveContent();
	m_window->loadContent();

	std::thread([&](){
		const std::vector<FilePath> filePaths = getFilePaths();
		m_showFilesFunctor(filePaths);
	}).detach();
}

void QtProjectWizzardContent::showFilesDialog(const std::vector<FilePath>& filePaths)
{
	if (!m_filesDialog)
	{
		m_filesDialog = std::make_shared<QtTextEditDialog>(
			getFileNamesTitle(), QString::number(filePaths.size()) + " " + getFileNamesDescription());
		m_filesDialog->setup();

		m_filesDialog->setText(utility::join(utility::toWStrings(filePaths), L"\n"));
		m_filesDialog->setCloseVisible(false);
		m_filesDialog->setReadOnly(true);

		connect(m_filesDialog.get(), &QtTextEditDialog::finished, this, &QtProjectWizzardContent::closedFilesDialog);
		connect(m_filesDialog.get(), &QtTextEditDialog::canceled, this, &QtProjectWizzardContent::closedFilesDialog);
	}

	m_filesDialog->showWindow();
	m_filesDialog->raise();
}

void QtProjectWizzardContent::closedFilesDialog()
{
	m_filesDialog->hide();
	m_filesDialog.reset();

	window()->raise();
}
