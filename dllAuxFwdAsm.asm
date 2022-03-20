.data
EXTERN FcnPtrOrg:QWORD

.code
OPTION PROLOGUE:NONE
OPTION EPILOGUE:NONE

Proxy_CallOrgFcnAsm proc
	jmp qword ptr [FcnPtrOrg]
Proxy_CallOrgFcnAsm endp

end

OPTION PROLOGUE:PrologueDef
OPTION EPILOGUE:EpilogueDef