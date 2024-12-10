#ifdef INFICS_OPER
GENER(SUM, "+");
GENER(SUB, "-");
GENER(MUL, "*");
#endif

#ifdef PREFICS_OPER
GENER(DIV, "\\frak{");
#endif

#ifdef GEOM
GENER(SIN, "sin");
GENER(COS, "cos");
GENER(TNG, "tan");
GENER(CTG, "ctg");
#endif

#ifdef A_GEOM
GENER(ASIN, "asin");
GENER(ACOS, "acos");
GENER(ATNG, "atan");
GENER(ACTG, "actg");
#endif

#ifdef OTHER
GENER(LN, "ln");
#endif

#ifdef ALL_FUNKS
GENER(SIN, "sin");
GENER(COS, "cos");
GENER(TNG, "tan");
GENER(CTG, "ctg");

GENER(ASIN, "asin");
GENER(ACOS, "acos");
GENER(ATNG, "atan");
GENER(ACTG, "actg");

GENER(LN, "ln");
#endif


