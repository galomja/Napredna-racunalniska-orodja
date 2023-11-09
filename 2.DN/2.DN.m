(* ::Package:: *)

(* ::Input:: *)
(*n= 800 ;(*\[SHacek]tevilo naklju\[CHacek]nih to\[CHacek]k*)*)
(*r=100;(*radij kro\[ZHacek]nice*)*)


(* ::Input:: *)
(*Get["C:\\Users\\Uporabnik\\OneDrive - Univerza v Ljubljani\\Gal faks\\3.letnik\\1.semester\\Napredna ra\[CHacek]unalni\[SHacek]ka orodja\\2.DN\\Funkcije.m"]*)


(* ::Input:: *)
(*Matrika=Transpose[{RandomInteger[{-r,r},n],RandomInteger[{-100,100},n]}];*)
(*{Mat1,Mat2}=Module[{Matvn={},Matnot={}},*)
(*For[i=1,i<n,i++,*)
(*x=Matrika[[i]][[1]];*)
(*y=Matrika[[i]][[2]];*)
(*If[x^2+y^2<r^2,*)
(*AppendTo[Matnot,{x,y}],*)
(*AppendTo[Matvn,{x,y}]]];*)
(*{Matnot,Matvn}];*)
(*\[Pi]apr=aproksimirajpi[Mat1,n]*)


(* ::Input:: *)
(*vn=ListPlot[Mat2,PlotStyle->Red];*)
(*not=ListPlot[Mat1,PlotStyle->Green];*)
(*c=ParametricPlot[{r*Cos[\[Phi]],r*Sin[\[Phi]]},{\[Phi],0,2\[Pi]}];*)
(*Show[{vn,not,c},AspectRatio->Automatic]*)


(* ::Input:: *)
(*Clear[f,n,t,x]*)


(* ::Input:: *)
(*izrisi[ft_]:=Module[{f=ft},*)
(*d=Series[f,{t,t0,4}];*)
(*p2=Plot[f,{t,0,4},PlotStyle->Red,PlotLegends->{"To\[CHacek]na funkcija"}];*)
(*Manipulate[*)
(*Show[*)
(*{Plot[Evaluate[Normal[Series[f,{t,t0,n}]]],{t,0,4},PlotStyle->Blue,PlotLegends->{"Pribli\[ZHacek]ek"}],p2},*)
(*PlotRange->Automatic*)
(*],*)
(*{n,1,10,1}]];*)


(* ::Input:: *)
(*t0=2;*)
(*izrisi[Sin[t]*t^2*Exp[-t]]*)
