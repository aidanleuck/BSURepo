#!/bin/gprolog --consult-file

:- include('data.pl').
:- include('uniq.pl').

lte(time(T1H, _),  time(T2H, _))  :- T1H < T2H.
lte(time(TH, T1M), time(TH, T2M)) :- T1M =< T2M.

%                    |---------- PT ----------|
%               |-- MT --|
overlap(slot(PTbeg,PTend),slot(PTbeg,MTend)) :- 
free(_,slot(PTbeg,PTend)), free(_,slot(MTbeg,MTend)),
lte(PTbeg,MTend), lte(MTend, PTend), lte(MTbeg, PTbeg),
PTbeg \== MTend.


%                    |---------- PT ----------|
%                            |-- MT --|
overlap(slot(PTbeg,PTend),slot(MTbeg,MTend)) :- 
free(_,slot(PTbeg,PTend)), free(_,slot(MTbeg,MTend)),
lte(PTbeg, MTbeg), lte(MTend, PTend),
MTbeg \== MTend.



   

%                    |---------- PT ----------|
%                                         |-- MT --|
overlap(slot(PTbeg,PTend),slot(MTbeg,PTend)) :-
free(_,slot(PTbeg,PTend)), free(_,slot(MTbeg,_)),
lte(PTbeg, MTbeg), lte(MTbeg, PTend),
PTend \== MTbeg.


meetone(Person, MeetSlot) :-
    free(Person, PersSlot),
    overlap(PersSlot, MeetSlot).

meetall([Head|Tail], Slot) :- meetone(Head, Slot), meetall(Tail, Slot).
meetall([], _).

meet(Slot) :- people(P), meetall(P, Slot).

people([ann,bob, carla]).

main :- findall(Slot, meet(Slot), Slots),
        uniq(Slots, Uniq),
        write(Uniq), nl, halt.

:- initialization(main).
