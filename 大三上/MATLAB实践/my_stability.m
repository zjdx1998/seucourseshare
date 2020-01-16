function my_stability(a, b)
    p = roots(a);
    if sum(p < 0) == len(p)
        disp('稳定');
    else
        if sum(p == 0) == 1 && sum(abs(p) == 1) == 1
            disp('临界稳定');
        else
            disp('稳定');
        end
    end
end