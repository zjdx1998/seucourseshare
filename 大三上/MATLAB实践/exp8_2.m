% Ns = [0, 0, 0, 1];
% Ds = [1, 1, 1, 0];
% sys = tf(Ns, Ds);
% impulse(sys)
for n = 1:100
    t(n) = (n - 1) / 10;
    F = @(tao)(exp()