n = 0:128;
f = 0.4375;
x1 = exp(-0.1 * n);
x2 = exp(-0.1 * n) .* sin(2 * pi * f * n);

L = 128;
w = 0:2*pi/L:2*pi/L*(L-1);

F1 = fft(x1, L);
F2 = fft(x2, L);

figure;
subplot(2, 2, 1);
stem(n, x1); title('de-amplification')
subplot(2, 2, 2);
plot(w, abs(F1)); title('de-amplification')
subplot(2, 2, 3);
stem(n, x2); title('sin de-amplification')
subplot(2, 2, 4);
plot(w, abs(F2)); title('sin de-amplification')