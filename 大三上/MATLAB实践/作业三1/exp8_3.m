clear;
t = 0:0.01:2;
%1
% Ns = [1, 3];
% Ds = [1, 3, 2];
% x0 = [4, -1]';
% u = exp(-3 * t) .* heaviside(t);
% 2
% Ns = [1, 3];
% Ds = [1, 2, 2];
% x0 = [1.6, -0.2]';
% u = heaviside(t);
%3
Ns = [1, 3];
Ds = [1, 2, 1];
x0 = [-3.5, 1.5]';
u = exp(-2 * t) .* heaviside(t);

[A, B, C, D] = tf2ss(Ns, Ds);
sys = ss(A, B, C, D);

lsim(sys, u, t);