#   Les échiquiers initiaux BI ou BPI (toutes les pieces distinguées 1 à 16)    #
#   sont des tores 10x10 sur lesquelles vivent les pieces.                      #
#   P contient les positions sur l'echiquier des 32 pieces :                    #
#   colonne 1 pieces blanches, colonne 2 pieces noires.                         #

#   Codage des types de piéces Pion:1, Cavalier:2, Fou:3, Tour:4, Reine:5, Roi:6   #
T=zeros(Int8,16); T[9:16]=1; T[1]=T[8]=4; T[2]=T[7]=2; T[3]=T[6]=3; T[4]=5; T[5]=6

#   Valeur des pieces #
V=zeros(Float64,6); V=[10 30 30 50 90 Inf]

#   Echiquier initial précis BPI   #
BPI=zeros(Int8,10,10); BPI[9,2:9]=1:8; BPI[8,2:9]=9:16; BPI[2,2:9]=-(1:8); BPI[3,2:9]=-(9:16)

#   Conversion des codages #
BPtoB!(BP,B)=for i=1:100 a=BP[i]; a==0 ? B[i]=0 : B[i]=sign(a)*T[abs(a)] end
BPtoP!(BP,P)=for i=1:100 BP[i]!=0 && (BP[i]>0 ? P[BP[i],1]=i : P[-BP[i],2]=i) end
PtoBP!(P,BP)=for i=1:16 a=P[i,1]; a!=0 && (BP[a]=i); a=P[i,2]; a!=0 && (BP[a]=-i) end

#   Mvmt de x à x+y en restant dans l'échiquier
Inside(x,y) = x==0 ? 0 : (a=(x+y-1)%100; ((a%10 in (0,9))|(div(a,10) in (0,9)) ? 0 : a+1))

#   Echiquier initial normal BI et position des pieces PI #
BI=zeros(Int8,10,10); PI=zeros(Int8,16,2); BPtoP!(BPI,PI); BPtoB!(BPI,BI)

#    Def des Mvmt Semi-Legaux par type de Piece : Dr #
Dr=zeros(Int8,6,8); Dr[1,1:3]=[-1 -11 9]; Dr[2,1:8]=[12 21 19 8 -12 -21 -19 -8]
Dr[3,1:4]=[-11 9 -9 11]; Dr[4,1:4]=[1 -1 10 -10]; Dr[5,1:8]=[Dr[4,1:4] Dr[3,1:4]]; Dr[6,1:8]=Dr[5,1:8]

#   Def des Mvmt Semi-Legaux par piece : DrP  #
DrP=zeros(Int8,16,8);
DrP[1:8,:]=[Dr[4,:]';Dr[2,:]';Dr[3,:]';Dr[5,:]';Dr[6,:]';Dr[3,:]';Dr[2,:]';Dr[4,:]']
DrP[9:16,:]=[Dr[1,:]';Dr[1,:]';Dr[1,:]';Dr[1,:]';Dr[1,:]';Dr[1,:]';Dr[1,:]';Dr[1,:]']

#   Mvts (MS) semi-legaux non iterables restant dans l'echiquier  #
MVSL!(c,MS) = (co=(c==1 ? 1 : -1); for p in (2,5,7,9,10,11,12,13,14,15,16)
    P[p,c]!=0 && for m=1:8 MS[p,m,c]=Inside(co*DrP[p,m],P[p,c]) end end )

#   Couts (Ct) des Mvts semi-legaux des pieces non iterable restant dans l'echiquier #
CtSL!(c,Ct) = (co=(c==1 ? 1 : -1); for p in (2,5,7) P[p,c]!=0 && for m=1:27  mp=MS[p,m,c]
    mp!=0 && ( Ct[p,m,c] = (BP[mp]==0 ? 0.0 : co*BP[mp]>0 ? -Inf : V[-co*B[mp]])) end end )

#   Couts (Ct) des Mvts semi-legaux des pions restant dans l'echiquier #
CtSLP!(c,Ct) = (co=(c==1 ? 1 : -1); for p=9:16 P[p,c]!=0 && for m=1:3 mp=MS[p,m,c]
    mp!=0 && ( if m==1 Ct[p,1,c] = (BP[mp] == 0 ? 0.0 : -Inf)
    else Ct[p,m,c] = (B[mp] == 0 ? -Inf : co*BP[mp]>0 ? -Inf : V[-co*B[mp]]) end ) end end )

#    Mvts (MS) Et Couts (Ct) semi-legaux iterables restant dans l'echiquier #
MVCtSLI!(c,MS,Ct) = (co=(c==1 ? 1 : -1); for p in (1,3,4,6,8) m=0
    P[p,c]!=0 && for b in DrP[p,:]; a=P[p,c]; Om=0
    b!=0 &&  while true; d=Inside(a,b); d==0 && break; m=m+1; MS[p,m,c]=d; a=d
    if Om == 0  BP[d]!=0 && (co*BP[d]>0 ? Ct[p,m,c]=-Inf : Ct[p,m,c]=V[-co*B[d]]; Om=1)
    else  Ct[p,m,c]=-Inf end end end end )

#   Liste creuse des mvts et couts légaux  #
function MvCt!(c)
    global(CL); CL=Array{Any}(0,4); for p=1:16 for m=1:27
    if (MS[p,m,c]!=0)  & (Ct[p,m,c]!=-Inf) CL=[CL; P[p,c] MS[p,m,c] Ct[p,m,c] 0.0] end end end ; CL
end

Coup(c,CL) = floor.(Int,size(CL,1)*rand(1)[1]+1)

#   Boucle read eval print Blanc #
#   Rentrer au prompt / case-départ case-d'arrivée retour-chariot (par ex : e2e4CR) ou QCR pour sortir #
#   Reponse des noirs : tirage au hasard sur les coups légaux sans s'occuper des echecs #
function REVP()
    global P,MS,Ct,CL,BP,B
    #   Initialisation #
    BP=copy(BPI); B=copy(BI); P=copy(PI); display(BI[2:9,2:9])
    #   Boucle read eval print  #
 while true
        c=1; print("\n==>"); IN=readline()           # case-initiale case-finale CR      #
    !ismatch(r"([a-h][1-8]){2}|Q", IN) && continue
    IN[1]=='Q' && break                              # QCR pour sortir de la boucle      #
    Pi=Int8(11+(IN[1]-'a')*10+('9'-IN[2]))           # Numero de la case de départ       #
    Pf=Int8(11+(IN[3]-'a')*10+('9'-IN[4]))           # Numero de la case d'arrivée       #

     a=BP[Pi]; a==0 ? print("\nerr1\n\n") : a>0 ? P[a,1]=Pf : P[abs(a),2]=Pf
     b=abs(BP[Pf]); b!=0 && (P[b,2]=0)
     BP=zeros(Int8,10,10);PtoBP!(P,BP);BPtoB!(BP,B)
    #display(B[2:9,2:9]); display(P')
    #   Coup Noir   #
        c=2; MS=zeros(Int8,16,27,2); MVSL!(c,MS); Ct=zeros(Float64,16,27,2)
    CtSL!(c,Ct); CtSLP!(c,Ct); MVCtSLI!(c,MS,Ct); CL=MvCt!(c); cpn=Coup(c,CL)
    a=abs(BP[Int(CL[cpn,1])]); b=abs(BP[Int(CL[cpn,2])])
    #display(Int.((CL-1.0)'));display(cpn);
    a==0 ? display(CL[cpn,:]) : P[a,2]=Int(CL[cpn,2])
    CL[cpn,3]!=0.0 && (P[b,1]=0)
    BP=zeros(Int8,10,10); PtoBP!(P,BP); BPtoB!(BP,B); display(B[2:9,2:9])
 end
end


#--------------------Debug ---------------------------------------------------------#

#   Utilitaire pour rendre plus lisible les tableaux de Couts #
beau(A::Array{Float64,3})=map(x->x==-Inf ?  -1 : x==0.0 ? 0: x,A)

#------------------------------Tests-------------------------------------------------#
#   Test1   #
BP=copy(BPI); B=copy(BI); P=copy(PI); display(B[2:9,2:9]);
c=2; MS=zeros(Int8,16,27,2); MVSL!(c,MS); Ct=zeros(Float64,16,27,2);
CtSL!(c,Ct); CtSLP!(c,Ct); MVCtSLI!(c,MS,Ct); CL=MvCt!(c);display(Int.((CL-1)'));
P[16,2]=85;BP=zeros(Int8,10,10);PtoBP!(P,BP); BPtoB!(BP,B); display(B[2:9,2:9])
#   Test2   #
BP=copy(BPI);BP[3,6]=0;P=zeros(Int8,16,2);BPtoP!(BP,P);BPtoB!(BP,B);display(B[2:9,2:9]);
#-------------------------------------------------------------------------------------#
