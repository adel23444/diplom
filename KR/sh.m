A1= 1;
A2 = 0;
alpha1 = 0.14;
alpha2 = 0;
betta = 0;
t = 0:1:200;
V = 2*pi*rad_wheel*0.1*n;
V_delt = V/200;

V_mass(1) = 0;

for i=2:50
    V_mass(i) = V_mass(i-1)+V_delt;
end;
   
for i=51:80
    V_mass(i) = V_mass(i-1)-0.5*V_delt;
end;
  
for i=81:120
    V_mass(i) = V_mass(i-1)+1.5*V_delt;
end;
 
for i=121:201
    V_mass(i) = V_mass(i-1);
end;
 


figure(1);
R = A1*exp(-alpha1.*V_mass.*t)+A2*exp(-alpha2.*V_mass.*t).*cos(V_mass*betta.*t);

figure(2);
plot(t, R, t, V_mass);
