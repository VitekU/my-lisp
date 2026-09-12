(def gcd
  (fn (x y)
    (if (== y 0)
        x
        (gcd y (- x (* (/ x y) y))))))

(gcd 32 48)
