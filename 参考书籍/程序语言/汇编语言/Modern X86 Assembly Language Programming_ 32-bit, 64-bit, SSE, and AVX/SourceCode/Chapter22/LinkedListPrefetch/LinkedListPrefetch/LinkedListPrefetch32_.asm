IFDEF ASMX86_32
        include <LinkedListPrefetch.inc>
        .model flat,c
        .code

; Macro _LlTraverse32
;
; The following macro generates linked list traversal code using the
; prefetchnta instruction if UsePrefetch is equal to 'Y'.

_LlTraverse32 macro UsePrefetch
        mov eax,[esp+4]                             ;eax = ptr to 1st node
        test eax,eax
        jz Done                                     ;jump if end-of-list

        align 16
@@:     mov ecx,[eax+LlNode.Link]                   ;ecx = next node
        vmovapd ymm0,ymmword ptr [eax+LlNode.ValA]  ;ymm0 = ValA
        vmovapd ymm1,ymmword ptr [eax+LlNode.ValB]  ;ymm1 = ValB

IFIDNI <UsePrefetch>,<Y>
        mov edx,ecx
        test edx,edx                        ;is there another node?
        cmovz edx,eax                       ;avoid prefetch of NULL
        prefetchnta [edx]                   ;prefetch start of next node
ENDIF

; Calculate ValC[i] = sqrt(ValA[i] * ValA[i] + ValB[i] * ValB[i])
        vmulpd ymm2,ymm0,ymm0                       ;ymm2 = ValA * ValA
        vmulpd ymm3,ymm1,ymm1                       ;ymm3 = ValB * ValB
        vaddpd ymm4,ymm2,ymm3                       ;ymm4 = sums
        vsqrtpd ymm5,ymm4                           ;ymm5 = square roots
        vmovntpd ymmword ptr [eax+LlNode.ValC],ymm5 ;save result

; Calculate ValD[i] = sqrt(ValA[i] / ValB[i] + ValB[i] / ValA[i]);
        vdivpd ymm2,ymm0,ymm1                       ;ymm2 = ValA / ValB
        vdivpd ymm3,ymm1,ymm0                       ;ymm3 = ValB / ValA
        vaddpd ymm4,ymm2,ymm3                       ;ymm4 = sums
        vsqrtpd ymm5,ymm4                           ;ymm5 = square roots
        vmovntpd ymmword ptr [eax+LlNode.ValD],ymm5 ;save result

        mov eax,ecx                         ;eax = ptr to next node
        test eax,eax
        jnz @B
        vzeroupper

Done:   ret
        endm

; extern "C" void LlTraverseA_(LlNode* first);
LlTraverseA_ proc
        _LlTraverse32 n
LlTraverseA_ endp

; extern "C" void LlTraverseB_(LlNode* first);
LlTraverseB_ proc
        _LlTraverse32 y
LlTraverseB_ endp

ENDIF
        end
