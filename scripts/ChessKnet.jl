# Prédit le nombre de cases de déplacement (m) à droite
# d'une pièce dans une rangée de 8 cases horizontales
# occupées par un nombre quelconque de pièces de même
# couleur. La sortie met un 1 sur une case m+1 0 ailleurs
# d'une rangée de 8 cases.
using Knet

# Calcul imprecis de la prédiction mais plus rapide.
# le padding permet de completer la convolution sur les bords
# de l'echiquer.
function predict(w, x)
    pool(max.(0, conv4(w[1], x; padding=(7,7))); window=8)
end

# Calcul precis de la prédiction mais plus lent.
function predict2(w, x)
    x1 = predict(w, x)
    x2 = exp.(x1)
    x2 / sum(x2)
end

# Defini le réseau de convolution (CNN):
# (2, 8): 2 couches d'entrees à 8 positions:
#   couche1: 8 pour les pièces qui bloquent.
#   couche2: 8 pour la pièce à déplacer.
# (15, 1): 15 poids image en 1 dimension.
w=Any[ones(15,1,2,8)]
w[1][1:7,1,2,:]  .= 0.0
w[1][9:15,1,2,:] .= 0.0

# Cross entropy loss function
function loss(w, x, y)
    ypred = predict(w, x)
    ynorm = ypred .- log.(sum(exp.(ypred)))
    -sum(y .* ynorm[1,1,:,1]) / size(y,1)
end

lossgradient = grad(loss)

# Calcule la distance entre la pièce a déplacer et la pièce la plus proche.
function distance_plus_proche(x, p)
    z = zeros(Int64,8)
    y = zeros(Int64,8)
    z[p] = 1
    j = p + 1
    while j <= 8
        x[j] == 1 ? z[j] = 1 : break
        j = j + 1
    end
    u = sum(z)
    y[u] = 1
    y
end

# Entrainement du réseau
function train(w, iter; lr=10.0)
    all=[]
    for j = 1:iter
        x = zeros(8,1,2,1)
        x[:,1,1,1] = Int.(floor.(0.5 .+ rand(8)))
        p = Int.(floor.(1 + 8 * rand(1)[1]))
        x[p,1,2,1] = 1
        y = distance_plus_proche(x[:,1,1,1], p)
        all=[all; y]
        dw = lossgradient(w, x, y)
        for i in 1:length(w)
            w[i] -= lr * dw[i]
        end
        (j%100==0) && display(loss(w,x,y))
    end;
    gg=reshape(all, 8, :)
    display(sum(gg, dims=2))
    return w
end

# Validation du réseau
function test(w, iter)
    err = 0.0
    for l = 1:iter
        x = zeros(8,1,2,1)
        x[:,1,1,1] = Int.(floor.(0.5 .+ rand(8)))
        p = Int.(floor.(1 + 8 * rand(1)[1]))
        x[p,1,2,1] = 1
        y = distance_plus_proche(x[:,1,1,1], p)
        ypred = predict2(w,x)[1,1,:,1]
        err += sum(abs2, y .- ypred)
    end
    err # /iter
end 

train(w, 10000)
test(w, 1000)
