(define (replace source search-for replace-with)
        
        (cond ((equal? source search-for)   ; Checks if source is equal to search-for
            (copy replace-with))            ; If source is equal to search-for then return the replaced value
            ((and(isPair source) (not (null? search-for))) ; If source is a pair and search-for is not null
                (cond((null? (cdr source))                  ; If cdr is null
                    (list(replace (car source) (copy search-for) (copy replace-with)))) ; call replace on car of source and put that value back into a list
                    (else
                        (cons (replace (car source) (copy search-for) (copy replace-with)) ; If it has a cdr then do the same thing but append car to cdr.
                        (replace (cdr source) (copy search-for) (copy replace-with)))
                    )

                )
            )
            (else                           ; If the two are not equal to one another
                (cond((null? search-for)    ; Special case if search for operator is null. We append the two values together
                
                    (cons (car source) replace-with)) ; Special case to combine the two values

                    (else (copy source))               ; Just return the copy of source otherwise
                
                )   
           
            )
        )
)
        
        
       
       

; Copies list
(define (copy listCopy)
    listCopy
)
; Checks if a list is a pair returns true or false
(define (isPair list)
    (if (pair? list)
        #t
        #f
    )
    
)
   ; TEST CASES
   ;(display(replace 1 2 3))
   ;(display(replace 1 1 2))
   ;(display(replace '() 1 2))
   ;(display(replace '() '() 1))
   ;(display(replace '() '() '(1)))
   ;(display(replace '(1) '() 2))
   ;(display(replace '(1) 1 2))
   ;(display(replace '((1) (1)) '(1) '(2) ))
   ;(display(replace '(a (b c) d) '(b c) '(x y)))
   ;(display(replace '(a (b c) (d (b c))) '(b c) '(x y)))
   ;(display(replace '(a b c) '(a b) '(x y)))
   ;(display(replace '(a b c) '(b c) '(x y)))

   
   
