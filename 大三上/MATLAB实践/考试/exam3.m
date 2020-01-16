clear;
Ns = [0, 0, 1, 4];
Ds = [1, 3, 12, 20];
[A, B, C, D] = tf2ss(Ns, Ds);
sys = ss(A, B, C, D);

x0 = [2, -1/2, 3/8]';

figure;
t = 0:0.01:5;
u0 = 0 * t;
r1 = lsim(sys, u0, t, x0);
plot(r1); title('zero-input response');

figure;
u = sin(0.5 * pi * t);
r2 = lsim(sys, u, t);
plot(t, r2); title('zero-state response');

figure;
[r3, t, x] = lsim(sys, u, t, x0);
subplot(2, 1, 1);
plot(t, u); title('excitation');
subplot(2, 1, 2);
plot(t, r3); title('complete response');

figure;
plot(t, r1 + r2, '-r*');
hold;
plot(t, r3, '-.bo');
legend('zeroinput + zerostate', 'complete');