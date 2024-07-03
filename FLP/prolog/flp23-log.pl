% Název: FLP Prolog projekt (Rubikova kostka)
% Autor: Petr Kaška (xkaska01)

% MAIN PROGRAMU

/** cte radky ze standardniho vstupu, konci na LF nebo EOF */
read_line(L,C) :-
	get_char(C),
	(isEOFEOL(C), L = [], !;
		read_line(LL,_),% atom_codes(C,[Cd]),
		[C|LL] = L).


/** testuje znak na EOF nebo LF */
isEOFEOL(C) :-
	C == end_of_file;
	(char_code(C,Code), Code==10).


read_lines(Ls) :-
	read_line(L,C),
	( C == end_of_file, Ls = [] ;
	  read_lines(LLs), Ls = [L|LLs]
	).


/** rozdeli radek na podseznamy */
split_line([],[[]]) :- !.
split_line([' '|T], [[]|S1]) :- !, split_line(T,S1).
split_line([32|T], [[]|S1]) :- !, split_line(T,S1).    % aby to fungovalo i s retezcem na miste seznamu
split_line([H|T], [[H|G]|S1]) :- split_line(T,[G|S1]). % G je prvni seznam ze seznamu seznamu G|S1


/** vstupem je seznam radku (kazdy radek je seznam znaku) */
split_lines([],[]).
split_lines([L|Ls],[H|T]) :- split_lines(Ls,T), split_line(L,H).



parseTopAndBot(R1,R2,R3, R123) :- append([[R1],[R2],[R3]], R123).

/**
 * @brief Funkce pro parsování bočních stran kostky.
 *
 * Tato funkce parsování bočních stran kostky ze vstupních dat do seznamu.
 *
 * @param R1 První řádek boční strany.
 * @param R2 Druhý řádek boční strany.
 * @param R3 Třetí řádek boční strany.
 * @param Empty Prázdný seznam pro ukládání bočních stran.
 * @param Sides Seznam bočních stran.
 *
 * @note Příklad dat: R1 = [a, b, c], R2 = [d, e, f], R3 = [g, h, i]
 */
parseSide([], [], [], Sides, Sides).
parseSide([R1|T1],[R2|T2],[R3|T3], Empty, Sides) :-
    parseTopAndBot(R1, R2, R3, Side),
    append(Empty, [Side], Tmp),
    parseSide(T1, T2, T3, Tmp, Sides).
    

/**
 * @brief Funkce pro parsování celé kostky.
 *
 * Tato funkce parsování vstupních dat reprezentujících celou kostku.
 *
 * @param Input Vstupní kostka.
 * @param RC Výstupní seznam s reprezentací kostky.
 *
 * @note Příklad dat:
 * Input = [[[5,5,3]],[[5,5,3]],[[5,5,4]],[[2,2,5],[3,2,2],[6,4,4],[1,1,1]],[[4,4,4],[1,1,5],[3,2,2],[6,3,3]],[[1,1,5],[3,2,2],[6,3,3],[4,4,4]],[[6,6,2]],[[6,6,1]],[[6,6,1]]]
 * RC = [[[5,5,3],[5,5,3],[5,5,4]],[[2,2,5],[4,4,4],[1,1,5]],[[3,2,2],[1,1,5],[3,2,2]],[[6,4,4],[3,2,2],[6,3,3]],[[1,1,1],[6,3,3],[4,4,4]],[[6,6,2],[6,6,1],[6,6,1]]]
 */
parse([[InputRow1],[InputRow2],[InputRow3],InputRow4,InputRow5,InputRow6,[InputRow7],[InputRow8],[InputRow9]], RC) :- 
    parseTopAndBot(InputRow1,InputRow2,InputRow3, TOP),
    parseSide(InputRow4,InputRow5,InputRow6, [], SIDES),
    parseTopAndBot(InputRow7,InputRow8,InputRow9, BOT),
    append([[TOP],SIDES,[BOT]], RC).


/**
 * @brief Hlavní funkce programu.
 *
 * Tato funkce řídí celý průběh programu. Načte vstupní data, provede parsování kostky, vyřeší kostku pomocí DFS algoritmu,
 * vypíše výsledky a ukončí program.
 */
main :-
    prompt(_, ''),
    read_lines(LL),
    split_lines(LL, Input),
    parse(Input,RC),
    print_RC(RC),

    solve_DFS(RC, Result, Step),
    print_all_rubics_steps(Result),
    % write_steps(Step),
    
    halt.


/**
 * @brief Funkce pro vypsání všech kroků řešení.
 *
 * Tato funkce vypíše všechny kroky, které byly použity k vyřešení kostky.
 *
 * @param Steps Seznam kroků.
 */
write_steps([]) :- write("konec\n").
write_steps([Step| Steps]) :-
    write(Step), write(" -> ") , write_steps(Steps).

/**
 * @brief Funkce pro vypsání všech kostek.
 *
 * Tato funkce vypíše všechny kostky v seznamu.
 *
 * @param Head_Cube Hlavní kostka.
 * @param Rest_Cubes Zbývající kostky.
 */
print_all_rubics_steps([]).
print_all_rubics_steps([Head_Cube | Rest_Cubes]) :-
	nl, print_RC(Head_Cube), print_all_rubics_steps(Rest_Cubes).


/**
 * @brief Funkce pro vypsání jedné kostky.
 *
 * Tato funkce vypíše jednu kostku reprezentovanou maticí.
 *
 * @param E1..F9 Jednotlivé prvky kostky.
 */
print_RC([
        [[E1, E2, E3], [E4, E5, E6], [E7, E8, E9]],
        [[A1, A2, A3], [A4, A5, A6], [A7, A8, A9]],
        [[B1, B2, B3], [B4, B5, B6], [B7, B8, B9]],
        [[C1, C2, C3], [C4, C5, C6], [C7, C8, C9]],
        [[D1, D2, D3], [D4, D5, D6], [D7, D8, D9]],
        [[F1, F2, F3], [F4, F5, F6], [F7, F8, F9]]
    ]) :-
        format("~w~w~w\n", [E1, E2, E3]),
        format("~w~w~w\n", [E4, E5, E6]),
        format("~w~w~w\n", [E7, E8, E9]),
        format("~w~w~w ~w~w~w ~w~w~w ~w~w~w\n", [A1, A2, A3, B1, B2, B3, C1, C2, C3, D1, D2, D3]),
        format("~w~w~w ~w~w~w ~w~w~w ~w~w~w\n", [A4, A5, A6, B4, B5, B6, C4, C5, C6, D4, D5, D6]),
        format("~w~w~w ~w~w~w ~w~w~w ~w~w~w\n", [A7, A8, A9, B7, B8, B9, C7, C8, C9, D7, D8, D9]),
        format("~w~w~w\n", [F1, F2, F3]),
        format("~w~w~w\n", [F4, F5, F6]),
        format("~w~w~w\n", [F7, F8, F9]).