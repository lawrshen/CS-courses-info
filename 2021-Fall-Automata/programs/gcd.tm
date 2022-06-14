; This program return gcd of two number.
; Input: a string of 1+01+ , eg '1111011111'

; the finite set of states
#Q = {0,cp,cp1,cmp,cmp1,mh,mhr,mhl,recover,accept,reject,reject1,reject2,reject3,halt_reject}

; the finite set of input symbols
#S = {0,1}

; the complete set of tape symbols
#G = {0,1,_,x}

; the start state
#q0 = 0

; the blank symbol
#B = _

; the set of final states
#F = {accept}

; the number of tapes
#N = 3 

; the transition functions

; State 0: begin
0 1** *** *** cp
0 0** *** *** reject
0 ___ ___ *** reject     ; Empty input

; State cp: copy 1+ before 0 to the 1nd tape 
cp 1__ 11* rr* cp
cp 0__ *_* rl* mh
cp ___ *** *** reject

; State mh: move 1st head to the right and copy 1+ after 0 to the 2nd tape
mh 0** *** rl* reject ; only one 0
mh _*_ _** l*l cmp
mh 1*_ **1 r*r mh 

; State cmp: compare 1+ after 0 with 1nd tape
cmp 11* __* ll* cmp
cmp 01* *** rr* recover
cmp 0_* _** l** accept
cmp 1_* *** l** mhl

; State recover: recover 1+ after 0 with 2nd tape
recover __1 111 rrl recover
recover ___ *** llr mhl

; State mhl: move 1st head to the left
mhl _** _** r** cp1
mhl _11 _** r** cmp1
mhl *** *** l** mhl

; State cmp1: compare 1+ before 0 with 2nd tape
cmp1 1*1 _*_ r*r cmp1
cmp1 111 ___ rlr cmp1
cmp1 **_ **_ *** mhr
cmp1 0** _** r** accept

; State mhr: move 1st head to the right
mhr _** _** l** cmp
mhr 1** *** r** mhr
mhr 0** *** r** mh

; State cp1: copy to 1nd tape 
cp1 1_* 11* rr* cp1
cp1 0_* *_* rl* mhr
cp1 1_1 11_ rrl cp1
cp1 11* *** *** reject

; State reject: halt
reject *** _** r** reject
reject _** *** l** reject1
reject1 *** _** l** reject1
reject1 _** *** l** reject2
reject2 *** _** l** reject2
reject2 _** *** *** reject3
reject3 _** x** *** halt_reject