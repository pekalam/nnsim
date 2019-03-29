#include "terminalwidget.h"
#include <QKeyEvent>
#include <QSizePolicy>

TerminalWidget::TerminalWidget(QWidget *parent) : QTextEdit(parent)
{
	setAutoFillBackground(true);
	setStyleSheet("background-color:black;");
	setTextColor(QColor::fromRgb(255, 255, 255));
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	setContextMenuPolicy(Qt::NoContextMenu);
	setUndoRedoEnabled(false);
	setFontFamily("Consolas");
	setFontPointSize(11);
	// setLineWrapMode(QTextEdit::LineWrapMode::NoWrap);
	// setWordWrapMode(QTextOption::NoWrap);
	 //clear();
	append("> ");
}

void TerminalWidget::addAutoCompletionCommand(QString s)
{
	autoCompletionCommands.append(s.toLower());
}

int TerminalWidget::isInAutoCompletionList(QString cmd)
{
	int ind = -1;
	QString cmd_l = cmd.toLower();
	for (int i = 0; i < autoCompletionCommands.size(); i++) {
		if (autoCompletionCommands[i].indexOf(cmd_l) == 0) {
			ind = i;
			break;
		}
	}
	return ind;
}

void TerminalWidget::printLine(QString line)
{
	insertPlainText(QString("%1\n%2").arg(line).arg("> "));
	moveCursor(QTextCursor::End);
	currentCommandPos = textCursor().position();
}

void TerminalWidget::keyPressEvent(QKeyEvent *e)
{

	switch (e->key()) {
	case Qt::Key_Up: { //TODO: bug kursor przechodzi w gore
		if (!cursorInEditionZone()) {
			break;
		}
		if ((cursorInEditionZone()) && (history.size() > 0 && historyPositon >= 0)) {
			QString s = toPlainText();
			s.chop(currentCommand.length());
			s.append(history[historyPositon]);
			currentCommand = history[historyPositon];
			historyPositon--;
			setText(s);

			moveCursor(QTextCursor::End);
			break;
		}
		break;
	}
	case Qt::Key_Tab: {
		int pos = isInAutoCompletionList(currentCommand);
		if (pos != -1 && currentCommand != autoCompletionCommands[pos]) {
			QString s = toPlainText();
			s.chop(currentCommand.length());
			s.append(autoCompletionCommands[pos]);
			currentCommand = autoCompletionCommands[pos];
			setText(s);

			moveCursor(QTextCursor::End);

		}

		break;
	}
	case Qt::Key_Down:
	case Qt::Key_Left: //TODO: bug kursor nie idzie w lewo po history
		if (!cursorInEditionZone()) {
			break;
		}
		else
			QTextEdit::keyPressEvent(e);
		break;
	case Qt::Key_Backspace:
		if (!(textCursor().position() > currentCommandPos))
			break;
		else {
			int cp = textCursor().position();
			int pos = cp - currentCommandPos - 1;
			if (pos >= 0 && pos < currentCommand.length())
				currentCommand = currentCommand.remove(pos, 1);
			QTextEdit::keyPressEvent(e);

		}
		break;
	case Qt::Key_Return:
		append("> ");
		moveCursor(QTextCursor::End);
		currentCommandPos = textCursor().position();
		emit commandExec(currentCommand);
		if (!currentCommand.isEmpty()) {
			history.append(currentCommand);
			historyPositon = history.size() - 1;
		}
		currentCommand.clear();
		break;
	default:
		if ((e->key() >= 0x30 && e->key() <= 0x57) || // 0-9
			(e->key() == 0x3B || e->key() == 0x7B || e->key() == 0x7D || e->key() == 0x5D || e->key() == 0x2E) || // ; { } ]
			(e->key() == 0x2C || e->key() == 0x28 || e->key() == 0x29 || e->key() == 0x5B) || // , ( ) [
			(e->key() >= 0x41 && e->key() <= 0x5A) || // A-Z
			(e->key() >= 0x61 && e->key() <= 0x7A)) // a-z
		{
			int cp = textCursor().position();
			int pos = cp - currentCommandPos;
			currentCommand.insert(pos, e->text());
			historyPositon = history.size() - 1;
		}
		QTextEdit::keyPressEvent(e);
	}

}

bool TerminalWidget::cursorInEditionZone()
{
	return (textCursor().position() >= currentCommandPos);
}



