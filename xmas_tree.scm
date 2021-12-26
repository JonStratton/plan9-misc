; xmas tree layer star counts !
(define clevel (lambda (start steps rows)
	(let ((levels `()))
		(set! levels (append levels (list start)))
		(if (> rows 1)
			(set! levels (append levels (clevel (+ start steps) steps (- rows 1)))))
		levels)))

; Pull the first two out, get the greatest and create a new list with that plus the rest of the list. Recurs do that with rest until one is left!
(define maxright (lambda (inlist)
	(if (> (length inlist) 1)
		(let ((newlist `()))
			(if (> (car inlist) (car (cdr inlist)))
				(set! newlist (append newlist (list (car inlist))))
				(set! newlist (append newlist (list (car (cdr inlist))))))
			(set! newlist (append newlist (cdr (cdr inlist))))
			(maxright newlist))
		inlist)))

; Display one char at a time!
(define dischar (lambda (char count)
	(display char)
	(if (> count 1)
		(dischar char (- count 1)))))

; Display each layer
(define disptree (lambda (fold stars)
	(let ((star (car stars)) (spaces 0))
		(set! spaces (- fold (/ (- star (modulo star 2)) 2)))
		(dischar " " spaces)
		(dischar "*" star)
		(newline))
	(if (> (length stars) 1)
		(disptree fold (cdr stars)))))

(let ((stars '()) (maxwidth 0) (foldpoint 0))
	; array of stars for each layer, top down
	(set! stars (append stars (clevel 1 2 4)))
	(set! stars (append stars (clevel 3 4 4)))
	(set! stars (append stars (clevel 5 6 4)))
	(set! stars (append stars (clevel 3 0 1)))
	(set! stars (append stars (clevel 3 0 1)))

	; the halfway point from widest layer
	(set! maxwidth (car (maxright stars)))
	(set! foldpoint (/ (- maxwidth (modulo maxwidth 2)) 2))

	(disptree foldpoint stars)
)
