Ns = [2, 0, 8, 10];
Ds = [1, 3, 6, 5];
sys = tf(Ns, Ds);

figure;
freqs(Ns, Ds, 0:0.1:10);
figure;
pzmap(sys); % stable