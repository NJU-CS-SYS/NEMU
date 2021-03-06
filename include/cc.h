#ifndef __CC_H__
#define __CC_H__

#define CC_AE   (FLAG_VAL(CF) == 0)
#define CC_A    ((FLAG_VAL(CF) == 0) && (FLAG_VAL(ZF) == 0))
#define CC_BE   (FLAG_VAL(CF) || FLAG_VAL(ZF))
#define CC_B    (FLAG_VAL(CF)==1)
#define CC_CXZ  (reg_w(R_CX)==0)
#define CC_ECXZ (reg_l(R_ECX)==0)
#define CC_E    (FLAG_VAL(ZF))
#define CC_GE   (FLAG_VAL(OF)==FLAG_VAL(SF))
#define CC_G    ((!FLAG_VAL(ZF))&&(FLAG_VAL(SF)==FLAG_VAL(OF)))
#define CC_LE   ((FLAG_VAL(OF) != FLAG_VAL(SF)) || FLAG_VAL(ZF))
#define CC_L    (FLAG_VAL(OF) != FLAG_VAL(SF))
#define CC_NAE  (FLAG_VAL(CF))
#define CC_NA   (FLAG_VAL(CF) || FLAG_VAL(ZF))
#define CC_NBE  ((FLAG_VAL(CF)==0)&&(FLAG_VAL(ZF)==0))
#define CC_NB   (FLAG_VAL(CF==0))
#define CC_NE   (FLAG_VAL(ZF) == 0)
#define CC_NEL  ((FLAG_VAL(SF)==FLAG_VAL(OF)) && (FLAG_VAL(ZF)==0))
#define CC_NGE  (FLAG_VAL(SF) != FLAG_VAL(OF))
#define CC_NG   (FLAG_VAL(ZF) || (FLAG_VAL(SF) != FLAG_VAL(OF)))
#define CC_NL   (FLAG_VAL(SF)==FLAG_VAL(OF))
#define CC_NO   (!FLAG_VAL(OF))
#define CC_NP   (!FLAG_VAL(PF))
#define CC_NS   (!FLAG_VAL(SF))
#define CC_NZ   (!FLAG_VAL(ZF))
#define CC_O    (FLAG_VAL(OF))
#define CC_P    (FLAG_VAL(PF))
#define CC_S    (FLAG_VAL(SF))

#endif
