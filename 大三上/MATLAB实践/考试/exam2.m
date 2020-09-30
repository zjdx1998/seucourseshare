Nz = [1, 0, 0];
Dz = [1, 0.8, 0.5];
sys = tf(Nz, Dz, 1000);

figure;
freqz(Nz, Dz, 128);

figure;
pzmap(sys); % stable

figure;
n = 0:200;
x = 5 + cos(0.2 * pi * n) + 2 * sin(0.7 * pi * n);
y = filter(Nz, Dz, x);
stem(n, y);
