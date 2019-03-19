# Prédit le nombre de cases de déplacement (m) à droite
# d'une pièce dans une rangée de 8 cases horizontales
# occupées par un nombre quelconque de pièces de même
# couleur. La sortie met un 1 sur une case m+1 0 ailleurs
# d'une rangée de 8 cases.
using Knet

function predict(w,x0)
    x1 = pool(max.(0, conv4(w[1],x0;padding=(7,0)) );window=8)
end

function predict2(w,x0)
    x1 = pool(max.(0, conv4(w[1],x0;padding=(7,0)) );window=8)
    x2= exp.(x1)
    x2/sum(x2)
end

w=Any[ones(15,1,2,8)]
w[1][1:7,1,2,:]=0
w[1][9:15,1,2,:]=0


function loss(w,x,y)
    ypred = predict(w,x)
    ynorm = ypred .- log.(sum(exp.(ypred),3))
    -sum(y .* ynorm[1,1,:,1]) / size(y,1)
end

lossgradient = grad(loss)

function train(w, n; lr=10.0)
    for j=1:n
        x=zeros(8,1,2,1)
        x[:,1,1,1]=Int.(floor.(0.5+rand(8)))
        p=Int.(floor.(1+8*rand(1)[1]))
        x[p,1,2,1]=1
        y=calcy3(x[:,1,1,1],p)
        dw = lossgradient(w, x, y)
        for i in 1:length(w)
            w[i] -= lr * dw[i]
        end
        (j%100==0) && display(loss(w,x,y))
    end
    return w
end

n=10000

function calcy3(x,p)
    z=zeros(Int64,8)
    y=zeros(Int64,8)
        z[p]=1
        j=p+1
        while j<=8
            x[j]==1 ? z[j]=1 : break
            j=j+1
        end
        u=sum(z)
        y[u]=1
        y
end

function test(w,nt)
    er=0.0
    for l=1:nt
        x=zeros(8,1,2,1)
        x[:,1,1,1]=Int.(floor.(0.5+rand(8)))
        p=Int.(floor.(1+8*rand(1)[1]))
        x[p,1,2,1]=1
        y=calcy3(x[:,1,1,1],p)
        ypred=predict2(w,x)[1,1,:,1]
        er+=sum(abs2,y .- ypred)
    end
    er
end

train(w,10000);
test(w,1000)
