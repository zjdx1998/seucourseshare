for k = -10:10
    if k >= 0
        s = 1;
    else
        s = 0;
    end
    f = sin(t) * s;
    stem(t,f)
    hold
end
