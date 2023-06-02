function y = fcn(u)
    u_bl1 = sqrt(2*alpha1*u);
    u_bl2 = 2*alpha2*u;
    a_b = sqrt(alpha2^2+betta^2);
    block1 = c2dm(sqrt(a1)*tf([u_bl1], [1 alpha1*u]));
    block2 = c2dm(sqrt(a2)*tf([sqrt(u_bl2) u*a_b], [1 u_bl u^2*a_b]));
y = block1+block2;
