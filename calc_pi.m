priblizki_pi = [];
napake=[];
for b = 1:12
    n=2^b;
    [koor1,koor2]=mcc_pi(n);
    [zadnji_prib,zadnja_nap] = area_pi(koor1,koor2);
    priblizki_pi=[priblizki_pi;zadnji_prib];
    napake = [napake;zadnja_nap];
end

x=linspace(-1,1,1000);
yy=@(x)[sqrt(1-x.^2);-sqrt(1-x.^2)];
mat_y = yy(x);
y1=mat_y(1,:);
y2=mat_y(2,:);
plot(koor1(:,1),koor1(:,2),"cdiamond")
hold on
plot(koor2(:,1),koor2(:,2),"r*")
plot(x,y1,"b.")
plot(x,y2,"b.")
legend('Notranje točke','Zunanje točke','Krožnica');
xlabel('x');
ylabel('y');
title('Metoda Monte Carlo');
hold off

function [priblizek,napaka] = area_pi(v1,v2)
%Funkcija sprejme vektorja v1(vektor koordinat tock znotraj korga) in
%vektor v2(vektor koordinat tock zunaj kroga). Iz njunih dolzin izracuna priblizek
%števila pi in kot odgovor vrne vektor priblizkov in vektor napak
l1=length(v1);
l2=length(v1)+length(v2);
priblizek=4* l1/l2;
napaka = priblizek-pi;
end