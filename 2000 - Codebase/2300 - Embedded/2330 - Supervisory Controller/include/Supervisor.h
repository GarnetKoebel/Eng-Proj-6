#ifndef SUPERVISOR_H
#define SUPERVISOR_H

int deDuplicateCommandQueue(void);
int receiveMsg(void);
void stateMachine(void);
int nextCommand();
int executeCommand(int cmd);
//int processMsg(void);


#endif