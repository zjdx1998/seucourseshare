; This structure must match the corresponding structure definition
; in LinkedListPrefetch.h

LlNode  struct
ValA        real8 4 dup(?)
ValB        real8 4 dup(?)
ValC        real8 4 dup(?)
ValD        real8 4 dup(?)
FreeSpace   byte 376 dup(?)

IFDEF ASMX86_32
Link        dword ?
Pad         byte 4 dup(?)
ENDIF

IFDEF ASMX86_64
Link        qword ?
ENDIF

LlNode  ends
