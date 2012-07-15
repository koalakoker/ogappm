#include "cnoteindex.h"

CNoteIndex::CNoteIndex(QWidget *parent)
    : QDialog(parent)
{
	ui.setupUi(this);

	// Connections
	connect(ui.noteIndex , SIGNAL(itemSelectionChanged()), this, SLOT(noteIndexChange()));
	connect(ui.noteIndex , SIGNAL(itemDoubleClicked ( QListWidgetItem * )), this, SLOT(accept ()));
}

CNoteIndex::~CNoteIndex()
{

}

void CNoteIndex::noteIndexAdd(QString note)
{
	ui.noteIndex->addItem(note);
}

void CNoteIndex::setCurSelected(int sel)
{
	ui.noteIndex->setCurrentRow(sel);
}

void CNoteIndex::noteIndexChange()
{
	emit noteIndexHasChanged(ui.noteIndex->currentRow());
}
