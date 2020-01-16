z = [1, 2]';
p = [-1, -2]';
k = 3;
[Ns, Ds] = zp2tf(z, p, k);
freqs(Ns, Ds, 0:0.05:20);