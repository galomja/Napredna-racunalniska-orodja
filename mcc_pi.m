function [koor1, koor2] = mcc_pi(n)
%Kot argument podamo n - število naključnih točk, ki jih želimo generirati.
%Funkcija vrne koor1, ki so koordniate točk znotraj enotskeda kroga in
%koor2, ki so koordinate točk izven kroga, a znotraj kvadrata [-1,1]x[-1,1].
matrika_koor=-1+2*rand(n,2);%generiramo koordinate točk na kvadtratu [-1,1]x[-1,1].
koor1=[];%tocke, ki so znortaj kroga
koor2=[];%tocke, ki niso znotraj kroga
for i = 1:length(matrika_koor)
    vec=matrika_koor(i,:);
    abs_vrednost=dot(vec, vec);
    if abs_vrednost < 1
    koor1=[koor1;vec];
    else
        koor2=[koor2;vec];
    end
end  