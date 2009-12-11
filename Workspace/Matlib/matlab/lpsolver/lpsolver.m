function [s]=lpsolver(a,b)
    %a=[1,2;2,1];
    %b=[3,1];
    %a=[5,2,3,4;2,1,5,5;4,2,1,3;2,6,3,1];
    %b=[7,4,2,10];
    %a=rand(100,100);
    %b=rand(1,100);
    a=[1,2,3;2,1,5;4,2,3];
    b=[7,4,2];
    aold=a;
    bold=b;
    n=size(a,1);
    for i=1:n
        t=a(i,i);
        for j=1:n
            a(i,j)=a(i,j)/t;
        end
        b(i)=b(i)/t;    
        if(i==n) break; end
        for j=(i+1):n
            for k=1:n
                tmp(k)=a(j,k)/a(j,i)-a(i,k);
            end
            b(j)=b(j)/a(j,i)-b(i);
            a(j,:)=tmp(:);    
        end
        b
    end
    for i=n:-1:1
        s(i)=b(i);
        for k=n:-1:(i+1)
            s(i)=s(i)-a(i,k)*s(k);
        end
    end
    s
end

    