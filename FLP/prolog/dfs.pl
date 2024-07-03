% Název: FLP Prolog projekt (Rubikova kostka)
% Autor: Petr Kaška (xkaska01)

% Algoritmus IDS


% předlohou tohoto algoritmu byla opora IZU.

solve_DFS(Start, Result, Steps) :-
    find_solution(
        Start,
        [[[E5, E5, E5], [E5, E5, E5], [E5, E5, E5]],
         [[A5, A5, A5], [A5, A5, A5], [A5, A5, A5]],
         [[B5, B5, B5], [B5, B5, B5], [B5, B5, B5]],
         [[C5, C5, C5], [C5, C5, C5], [C5, C5, C5]],
         [[D5, D5, D5], [D5, D5, D5], [D5, D5, D5]],
         [[F5, F5, F5], [F5, F5, F5], [F5, F5, F5]]],
         Result,
         Steps,
        30,
        0).

find_solution(Actual_State, Desired_Result, Result, Steps, Max_Alowed_Depth, Actual_Depth) :-
    Max_Alowed_Depth >= Actual_Depth,
    (
        (
            steps(Actual_State, Desired_Result, [], Result, Steps, [], Actual_Depth, ReachedDepth),
            ReachedDepth == 0
        ) 
        ;
        (   
            TmpDepth is Actual_Depth + 1,
            find_solution(Actual_State, Desired_Result, Result, Steps, Max_Alowed_Depth, TmpDepth)
        )
    ).
        

steps(Actual_State, Desired_Result, PreviousSteps, Result, Steps, PreviousMoves, Depth, ReachedDepth) :-
    Depth > 0,
    move(Actual_State, Next_State, Move),
    append(PreviousMoves, [Move], UpdatedSteps), 
    append(PreviousSteps, [Next_State], TmpSteps),
    TmpDepth is Depth - 1,
    steps(Next_State, Desired_Result, TmpSteps, Result, Steps, UpdatedSteps, TmpDepth, ReachedDepth).
steps(Actual_State, Actual_State, Result, Result, Steps, Steps, _, ReachedDepth) :- ReachedDepth is 0, !.
steps(_, _, _, _, _, _,0, 1).

move(Actual_State, Next_State, Move) :- u(Actual_State, Next_State), Move = u.
move(Actual_State, Next_State, Move) :- u_prime(Actual_State, Next_State), Move = u_.
move(Actual_State, Next_State, Move) :- d(Actual_State, Next_State), Move = d.
move(Actual_State, Next_State, Move) :- d_prime(Actual_State, Next_State), Move = d_.
move(Actual_State, Next_State, Move) :- r(Actual_State, Next_State), Move = r.
move(Actual_State, Next_State, Move) :- r_prime(Actual_State, Next_State), Move = r_.
move(Actual_State, Next_State, Move) :- l(Actual_State, Next_State), Move = l.
move(Actual_State, Next_State, Move) :- l_prime(Actual_State, Next_State), Move = l_.
move(Actual_State, Next_State, Move) :- f(Actual_State, Next_State), Move = f.
move(Actual_State, Next_State, Move) :- f_prime(Actual_State, Next_State), Move = f_.
move(Actual_State, Next_State, Move) :- b(Actual_State, Next_State), Move = b.
move(Actual_State, Next_State, Move) :- b_prime(Actual_State, Next_State), Move = b_.
move(Actual_State, Next_State, Move) :- m(Actual_State, Next_State), Move = m.
move(Actual_State, Next_State, Move) :- m_prime(Actual_State, Next_State), Move = m_.
move(Actual_State, Next_State, Move) :- e(Actual_State, Next_State), Move = e.
move(Actual_State, Next_State, Move) :- e_prime(Actual_State, Next_State), Move = e_.
move(Actual_State, Next_State, Move) :- s(Actual_State, Next_State), Move = s.
move(Actual_State, Next_State, Move) :- s_prime(Actual_State, Next_State), Move = s_.