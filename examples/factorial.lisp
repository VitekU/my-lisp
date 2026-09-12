(def fact
    (fn (x)
        (if (== x 0)
        1
        (* x (fact (- x 1))))))

(fact 5)
