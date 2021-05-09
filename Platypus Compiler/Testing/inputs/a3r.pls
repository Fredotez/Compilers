!!This is a syntactically correct PLATYPUS program
!!Weiler's law:
!!"Nothing is impossible for the man who doesn't have to do it himself."
!!"Parsing is passing." S^R & Compilers' law

PROGRAM{
 a=-000;
 b=+0.;
 INPUT(c);
 INPUT(d,e,f);
 c=((d+e)/a)*f-(((d-e)*a)/f);
 WHILE TRUE(a<>b _OR_ c==d _AND_ e<f _OR_ a>0)DO{
   IF TRUE(a==1 _AND_ b==0.0)THEN{
    c=-(5.9);
   }ELSE {c=-c;};  
 };   
 OUTPUT();
 OUTPUT("Results: ");
 OUTPUT(d,e,f,a);
}