// Copyright (C) 2010-2011 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.


#include "PythonCodeEditor.hpp"
#include "PythonConsole.hpp"
#include "ui/core/NScriptEngine.hpp"
#include "ui/core/NLog.hpp"
#include "common/Log.hpp"
#include <QStringListModel>
#include <QMessageBox>
#include <QTextBlock>
#include <QScrollBar>
#include <QToolBar>
#include <QAction>
#include <QWidget>
#include <QFileDialog>

////////////////////////////////////////////////////////////////////////////

namespace cf3 {
namespace ui {
namespace graphics {

//////////////////////////////////////////////////////////////////////////

PythonCodeEditor::PythonCodeEditor(QWidget *parent) :
  PythonCodeContainer(parent)
{
  setUndoRedoEnabled(true);
  //Toolbar
  connect(tool_bar->addAction("Execute all"),SIGNAL(triggered()),this,SLOT(execute_immediat()));
  connect(tool_bar->addAction("Execute statement by statement"),SIGNAL(triggered()),this,SLOT(execute_stepped()));
  connect(tool_bar->addAction(QIcon(":/Icons/action_open.png"),"Open"),SIGNAL(triggered()),this,SLOT(open()));
  connect(tool_bar->addAction(QIcon(":/Icons/action_save.png"),"Save"),SIGNAL(triggered()),this,SLOT(save()));
  setViewportMargins(border_width,tool_bar->height(),0,0);
  offset_border.setX(border_width);
  offset_border.setY(tool_bar->height());
}

////////////////////////////////////////////////////////////////////////////

PythonCodeEditor::~PythonCodeEditor(){

}

////////////////////////////////////////////////////////////////////////////

void PythonCodeEditor::key_press_event(QKeyEvent *e){
  QTextCursor c=textCursor();
  switch(e->key()){
  case Qt::Key_Home:
    c.movePosition(QTextCursor::StartOfBlock);
    c.movePosition(QTextCursor::WordRight);
    setTextCursor(c);
    break;
  default:
    QPlainTextEdit::keyPressEvent(e);
  }
}

////////////////////////////////////////////////////////////////////////////

void PythonCodeEditor::new_line(int indent_number){

}

////////////////////////////////////////////////////////////////////////////

void PythonCodeEditor::border_click(const QPoint &pos){
  QTextBlock b=cursorForPosition(pos-offset_border).block();
  int current_block=b.blockNumber();
  document()->markContentsDirty(b.position(),1);
  toggle_break_point(0,current_block,false);
}

////////////////////////////////////////////////////////////////////////////

bool PythonCodeEditor::editable_zone(const QTextCursor &cursor){
  return true;
}

////////////////////////////////////////////////////////////////////////////

void PythonCodeEditor::execute_immediat(){
  python_console->execute_code(toPlainText(),true,break_points);
}

////////////////////////////////////////////////////////////////////////////

void PythonCodeEditor::execute_stepped(){
  python_console->execute_code(toPlainText(),false,break_points);
}

////////////////////////////////////////////////////////////////////////////

void PythonCodeEditor::open(){
  QFileDialog dlg;

#ifndef Q_WS_MAC
  dlg.setOption(QFileDialog::DontUseNativeDialog);
#endif

  dlg.setAcceptMode(QFileDialog::AcceptOpen);
  dlg.setNameFilters( QStringList() << "Python script (*.py)" << "All files (*.*)" );
  dlg.setDirectory( QDir::home() );
  dlg.setFileMode(QFileDialog::ExistingFile);
  if( dlg.exec() == QFileDialog::Accepted ){
    QFile f(dlg.selectedFiles().first());
    f.open(QFile::ReadOnly);
    clear();
    insertPlainText(f.readAll());
    f.close();
  }
}

////////////////////////////////////////////////////////////////////////////

void PythonCodeEditor::save(){
  QFileDialog dlg;

#ifndef Q_WS_MAC
  dlg.setOption(QFileDialog::DontUseNativeDialog);
#endif

  dlg.setAcceptMode(QFileDialog::AcceptSave);
  dlg.setNameFilters( QStringList() << "Python script (*.py)" << "All files (*.*)" );
  dlg.setDirectory( QDir::home() );
  dlg.setFileMode(QFileDialog::AnyFile);

  if( dlg.exec() == QFileDialog::Accepted ){
    QFile f(dlg.selectedFiles().first());
    f.open(QFile::WriteOnly);
    f.write(toPlainText().toStdString().c_str());
    f.close();
  }
}

//////////////////////////////////////////////////////////////////////////

} // Graphics
} // ui
} // cf3