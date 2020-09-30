% 1
% Ns = [1];
% Ds = [1, 1];
% 2
% Ns = [10, 10];
% Ds = [1];
% 3
Ns = [10, 0, 0];
Ds = [-5, 1];

sys1 = tf(Ns, Ds)
disp('zeros:');disp(z);
disp('poles:');disp(p);