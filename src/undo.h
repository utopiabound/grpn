#ifndef __UNDO_H
#define __UNDO_H


void initUndoSystem();

void UndoStack();

void CancelStackState();

void SaveStackState(int);
void AddStackState(int);

void UndoStackState(int);

#endif
