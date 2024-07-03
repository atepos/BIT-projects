% Název: FLP Prolog projekt (Rubikova kostka)
% Autor: Petr Kaška (xkaska01)

% Vsechny mozne pohyby kostky


u([
    [[E1, E2, E3], [E4, E5, E6], [E7, E8, E9]],
    [[A1, A2, A3], [A4, A5, A6], [A7, A8, A9]],
    [[B1, B2, B3], [B4, B5, B6], [B7, B8, B9]],
    [[C1, C2, C3], [C4, C5, C6], [C7, C8, C9]],
    [[D1, D2, D3], [D4, D5, D6], [D7, D8, D9]],
    [[F1, F2, F3], [F4, F5, F6], [F7, F8, F9]]
], TransformedRC) :- 
    TransformedRC = [
        [[E3, E6, E9], [E2, E5, E8], [E1, E4, E7]],
        [[D1, D2, D3], [A4, A5, A6], [A7, A8, A9]],
        [[A1, A2, A3], [B4, B5, B6], [B7, B8, B9]],
        [[B1, B2, B3], [C4, C5, C6], [C7, C8, C9]],
        [[C1, C2, C3], [D4, D5, D6], [D7, D8, D9]],
        [[F1, F2, F3], [F4, F5, F6], [F7, F8, F9]]
    ].

u_prime([
    [[E1, E2, E3], [E4, E5, E6], [E7, E8, E9]],
    [[A1, A2, A3], [A4, A5, A6], [A7, A8, A9]],
    [[B1, B2, B3], [B4, B5, B6], [B7, B8, B9]],
    [[C1, C2, C3], [C4, C5, C6], [C7, C8, C9]],
    [[D1, D2, D3], [D4, D5, D6], [D7, D8, D9]],
    [[F1, F2, F3], [F4, F5, F6], [F7, F8, F9]]
], TransformedRC) :- 
    TransformedRC = [
        [[E7, E4, E1], [E8, E5, E2], [E9, E6, E3]],
        [[B1, B2, B3], [A4, A5, A6], [A7, A8, A9]],
        [[C1, C2, C3], [B4, B5, B6], [B7, B8, B9]],
        [[D1, D2, D3], [C4, C5, C6], [C7, C8, C9]],
        [[A1, A2, A3], [D4, D5, D6], [D7, D8, D9]],
        [[F1, F2, F3], [F4, F5, F6], [F7, F8, F9]]
    ].
    
d([
    [[E1, E2, E3], [E4, E5, E6], [E7, E8, E9]],
    [[A1, A2, A3], [A4, A5, A6], [A7, A8, A9]],
    [[B1, B2, B3], [B4, B5, B6], [B7, B8, B9]],
    [[C1, C2, C3], [C4, C5, C6], [C7, C8, C9]],
    [[D1, D2, D3], [D4, D5, D6], [D7, D8, D9]],
    [[F1, F2, F3], [F4, F5, F6], [F7, F8, F9]]
], TransformedRC) :- 
    TransformedRC = [
        [[E1, E2, E3], [E4, E5, E6], [E7, E8, E9]],
        [[A1, A2, A3], [A4, A5, A6], [B7, B8, B9]],
        [[B1, B2, B3], [B4, B5, B6], [C7, C8, C9]],
        [[C1, C2, C3], [C4, C5, C6], [D7, D8, D9]],
        [[D1, D2, D3], [D4, D5, D6], [A7, A8, A9]],
        [[F3, F6, F9], [F2, F5, F8], [F1, F4, F7]]
    ].

d_prime([
    [[E1, E2, E3], [E4, E5, E6], [E7, E8, E9]],
    [[A1, A2, A3], [A4, A5, A6], [A7, A8, A9]],
    [[B1, B2, B3], [B4, B5, B6], [B7, B8, B9]],
    [[C1, C2, C3], [C4, C5, C6], [C7, C8, C9]],
    [[D1, D2, D3], [D4, D5, D6], [D7, D8, D9]],
    [[F1, F2, F3], [F4, F5, F6], [F7, F8, F9]]
], TransformedRC) :- 
    TransformedRC = [
        [[E1, E2, E3], [E4, E5, E6], [E7, E8, E9]],
        [[A1, A2, A3], [A4, A5, A6], [D7, D8, D9]],
        [[B1, B2, B3], [B4, B5, B6], [A7, A8, A9]],
        [[C1, C2, C3], [C4, C5, C6], [B7, B8, B9]],
        [[D1, D2, D3], [D4, D5, D6], [C7, C8, C9]],
        [[F7, F4, F1], [F8, F5, F2], [F9, F6, F3]]
    ].

r([
    [[E1, E2, E3], [E4, E5, E6], [E7, E8, E9]],
    [[A1, A2, A3], [A4, A5, A6], [A7, A8, A9]],
    [[B1, B2, B3], [B4, B5, B6], [B7, B8, B9]],
    [[C1, C2, C3], [C4, C5, C6], [C7, C8, C9]],
    [[D1, D2, D3], [D4, D5, D6], [D7, D8, D9]],
    [[F1, F2, F3], [F4, F5, F6], [F7, F8, F9]]
], TransformedRC) :- 
    TransformedRC = [
        [[E1, E2, A3], [E4, E5, A6], [E7, E8, A9]],
        [[A1, A2, F3], [A4, A5, F6], [A7, A8, F9]],
        [[B7, B4, B1], [B8, B5, B2], [B9, B6, B3]],
        [[E9, C2, C3], [E6, C5, C6], [E3, C8, C9]],
        [[D1, D2, D3], [D4, D5, D6], [D7, D8, D9]],
        [[F1, F2, C7], [F4, F5, C4], [F7, F8, C1]]
    ].

r_prime([
    [[E1, E2, E3], [E4, E5, E6], [E7, E8, E9]],
    [[A1, A2, A3], [A4, A5, A6], [A7, A8, A9]],
    [[B1, B2, B3], [B4, B5, B6], [B7, B8, B9]],
    [[C1, C2, C3], [C4, C5, C6], [C7, C8, C9]],
    [[D1, D2, D3], [D4, D5, D6], [D7, D8, D9]],
    [[F1, F2, F3], [F4, F5, F6], [F7, F8, F9]]
    ], TransformedRC) :- 
        TransformedRC = [
        [[E1, E2, C7], [E4, E5, C4], [E7, E8, C1]],
        [[A1, A2, E3], [A4, A5, E6], [A7, A8, E9]],
        [[B3, B6, B9], [B2, B5, B8], [B1, B4, B7]],
        [[F9, C2, C3], [F6, C5, C6], [F3, C8, C9]],
        [[D1, D2, D3], [D4, D5, D6], [D7, D8, D9]],
        [[F1, F2, A3], [F4, F5, A6], [F7, F8, A9]]
    ].


l([
    [[E1, E2, E3], [E4, E5, E6], [E7, E8, E9]],
    [[A1, A2, A3], [A4, A5, A6], [A7, A8, A9]],
    [[B1, B2, B3], [B4, B5, B6], [B7, B8, B9]],
    [[C1, C2, C3], [C4, C5, C6], [C7, C8, C9]],
    [[D1, D2, D3], [D4, D5, D6], [D7, D8, D9]],
    [[F1, F2, F3], [F4, F5, F6], [F7, F8, F9]]
    ], TransformedRC) :- 
        TransformedRC = [
        [[C9, E2, E3], [C6, E5, E6], [C3, E8, E9]],
        [[E1, A2, A3], [E4, A5, A6], [E7, A8, A9]],
        [[B1, B2, B3], [B4, B5, B6], [B7, B8, B9]],
        [[C1, C2, F7], [C4, C5, F4], [C7, C8, F1]],
        [[D7, D4, D1], [D8, D5, D2], [D9, D6, D3]],
        [[A1, F2, F3], [A4, F5, F6], [A7, F8, F9]]
    ].

l_prime([
    [[E1, E2, E3], [E4, E5, E6], [E7, E8, E9]],
    [[A1, A2, A3], [A4, A5, A6], [A7, A8, A9]],
    [[B1, B2, B3], [B4, B5, B6], [B7, B8, B9]],
    [[C1, C2, C3], [C4, C5, C6], [C7, C8, C9]],
    [[D1, D2, D3], [D4, D5, D6], [D7, D8, D9]],
    [[F1, F2, F3], [F4, F5, F6], [F7, F8, F9]]
    ], TransformedRC) :- 
        TransformedRC = [
        [[A1, E2, E3], [A4, E5, E6], [A7, E8, E9]],
        [[F1, A2, A3], [F4, A5, A6], [F7, A8, A9]],
        [[B1, B2, B3], [B4, B5, B6], [B7, B8, B9]],
        [[C1, C2, E7], [C4, C5, E4], [C7, C8, E1]],
        [[D3, D6, D9], [D2, D5, D8], [D1, D4, D7]],
        [[C9, F2, F3], [C6, F5, F6], [C3, F8, F9]]
    ].

f([
    [[E1, E2, E3], [E4, E5, E6], [E7, E8, E9]],
    [[A1, A2, A3], [A4, A5, A6], [A7, A8, A9]],
    [[B1, B2, B3], [B4, B5, B6], [B7, B8, B9]],
    [[C1, C2, C3], [C4, C5, C6], [C7, C8, C9]],
    [[D1, D2, D3], [D4, D5, D6], [D7, D8, D9]],
    [[F1, F2, F3], [F4, F5, F6], [F7, F8, F9]]
    ], TransformedRC) :- 
        TransformedRC = [
        [[E1, E2, E3], [E4, E5, E6], [D9, D6, D3]],
        [[A7, A4, A1], [A8, A5, A2], [A9, A6, A3]],
        [[E7, B2, B3], [E8, B5, B6], [E9, B8, B9]],
        [[C1, C2, C3], [C4, C5, C6], [C7, C8, C9]],
        [[D1, D2, F1], [D4, D5, F2], [D7, D8, F3]],
        [[B7, B4, B1], [F4, F5, F6], [F7, F8, F9]]
    ].

f_prime([
    [[E1, E2, E3], [E4, E5, E6], [E7, E8, E9]],
    [[A1, A2, A3], [A4, A5, A6], [A7, A8, A9]],
    [[B1, B2, B3], [B4, B5, B6], [B7, B8, B9]],
    [[C1, C2, C3], [C4, C5, C6], [C7, C8, C9]],
    [[D1, D2, D3], [D4, D5, D6], [D7, D8, D9]],
    [[F1, F2, F3], [F4, F5, F6], [F7, F8, F9]]
    ], TransformedRC) :- 
        TransformedRC = [
        [[E1, E2, E3], [E4, E5, E6], [B1, B4, B7]],
        [[A3, A6, A9], [A2, A5, A8], [A1, A4, A7]],
        [[F3, B2, B3], [F2, B5, B6], [F1, B8, B9]],
        [[C1, C2, C3], [C4, C5, C6], [C7, C8, C9]],
        [[D1, D2, E9], [D4, D5, E8], [D7, D8, E7]],
        [[D3, D6, D9], [F4, F5, F6], [F7, F8, F9]]
        ].

b([
    [[E1, E2, E3], [E4, E5, E6], [E7, E8, E9]],
    [[A1, A2, A3], [A4, A5, A6], [A7, A8, A9]],
    [[B1, B2, B3], [B4, B5, B6], [B7, B8, B9]],
    [[C1, C2, C3], [C4, C5, C6], [C7, C8, C9]],
    [[D1, D2, D3], [D4, D5, D6], [D7, D8, D9]],
    [[F1, F2, F3], [F4, F5, F6], [F7, F8, F9]]
    ], TransformedRC) :- 
        TransformedRC = [
        [[D7, D4, D1], [E4, E5, E6], [E7, E8, E9]],
        [[A1, A2, A3], [A4, A5, A6], [A7, A8, A9]],
        [[B1, B2, E1], [B4, B5, E2], [B7, B8, E3]],
        [[C3, C6, C9], [C2, C5, C8], [C1, C4, C7]],
        [[F7, D2, D3], [F8, D5, D6], [F9, D8, D9]],
        [[F1, F2, F3], [F4, F5, F6], [B9, B6, B3]]
    ].
    
b_prime([
    [[E1, E2, E3], [E4, E5, E6], [E7, E8, E9]],
    [[A1, A2, A3], [A4, A5, A6], [A7, A8, A9]],
    [[B1, B2, B3], [B4, B5, B6], [B7, B8, B9]],
    [[C1, C2, C3], [C4, C5, C6], [C7, C8, C9]],
    [[D1, D2, D3], [D4, D5, D6], [D7, D8, D9]],
    [[F1, F2, F3], [F4, F5, F6], [F7, F8, F9]]
    ], TransformedRC) :- 
        TransformedRC = [
        [[B3, B6, B9], [E4, E5, E6], [E7, E8, E9]],
        [[A1, A2, A3], [A4, A5, A6], [A7, A8, A9]],
        [[B1, B2, F9], [B4, B5, F8], [B7, B8, F7]],
        [[C7, C4, C1], [C8, C5, C2], [C9, C6, C3]],
        [[E3, D2, D3], [E2, D5, D6], [E1, D8, D9]],
        [[F1, F2, F3], [F4, F5, F6], [D1, D4, D7]]
    ].

m([
    [[E1, E2, E3], [E4, E5, E6], [E7, E8, E9]],
    [[A1, A2, A3], [A4, A5, A6], [A7, A8, A9]],
    [[B1, B2, B3], [B4, B5, B6], [B7, B8, B9]],
    [[C1, C2, C3], [C4, C5, C6], [C7, C8, C9]],
    [[D1, D2, D3], [D4, D5, D6], [D7, D8, D9]],
    [[F1, F2, F3], [F4, F5, F6], [F7, F8, F9]]
    ], TransformedRC) :- 
        TransformedRC = [
        [[E1, C8, E3], [E4, C5, E6], [E7, C2, E9]],
        [[A1, E2, A3], [A4, E5, A6], [A7, E8, A9]],
        [[B1, B2, B3], [B4, B5, B6], [B7, B8, B9]],
        [[C1, F8, C3], [C4, F5, C6], [C7, F2, C9]],
        [[D1, D2, D3], [D4, D5, D6], [D7, D8, D9]],
        [[F1, A2, F3], [F4, A5, F6], [F7, A8, F9]]
    ].
    
m_prime([
    [[E1, E2, E3], [E4, E5, E6], [E7, E8, E9]],
    [[A1, A2, A3], [A4, A5, A6], [A7, A8, A9]],
    [[B1, B2, B3], [B4, B5, B6], [B7, B8, B9]],
    [[C1, C2, C3], [C4, C5, C6], [C7, C8, C9]],
    [[D1, D2, D3], [D4, D5, D6], [D7, D8, D9]],
    [[F1, F2, F3], [F4, F5, F6], [F7, F8, F9]]
    ], TransformedRC) :- 
        TransformedRC = [
        [[E1, A2, E3], [E4, A5, E6], [E7, A8, E9]],
        [[A1, F2, A3], [A4, F5, A6], [A7, F8, A9]],
        [[B1, B2, B3], [B4, B5, B6], [B7, B8, B9]],
        [[C1, E8, C3], [C4, E5, C6], [C7, E2, C9]],
        [[D1, D2, D3], [D4, D5, D6], [D7, D8, D9]],
        [[F1, C8, F3], [F4, C5, F6], [F7, C2, F9]]
    ].


e([
    [[E1, E2, E3], [E4, E5, E6], [E7, E8, E9]],
    [[A1, A2, A3], [A4, A5, A6], [A7, A8, A9]],
    [[B1, B2, B3], [B4, B5, B6], [B7, B8, B9]],
    [[C1, C2, C3], [C4, C5, C6], [C7, C8, C9]],
    [[D1, D2, D3], [D4, D5, D6], [D7, D8, D9]],
    [[F1, F2, F3], [F4, F5, F6], [F7, F8, F9]]
    ], TransformedRC) :- 
        TransformedRC = [
        [[E1, E2, E3], [E4, E5, E6], [E7, E8, E9]],
        [[A1, A2, A3], [D4, D5, D6], [A7, A8, A9]],
        [[B1, B2, B3], [A4, A5, A6], [B7, B8, B9]],
        [[C1, C2, C3], [B4, B5, B6], [C7, C8, C9]],
        [[D1, D2, D3], [C4, C5, C6], [D7, D8, D9]],
        [[F1, F2, F3], [F4, F5, F6], [F7, F8, F9]]
    ].
    
e_prime([
    [[E1, E2, E3], [E4, E5, E6], [E7, E8, E9]],
    [[A1, A2, A3], [A4, A5, A6], [A7, A8, A9]],
    [[B1, B2, B3], [B4, B5, B6], [B7, B8, B9]],
    [[C1, C2, C3], [C4, C5, C6], [C7, C8, C9]],
    [[D1, D2, D3], [D4, D5, D6], [D7, D8, D9]],
    [[F1, F2, F3], [F4, F5, F6], [F7, F8, F9]]
    ], TransformedRC) :- 
        TransformedRC = [
        [[E1, E2, E3], [E4, E5, E6], [E7, E8, E9]],
        [[A1, A2, A3], [B4, B5, B6], [A7, A8, A9]],
        [[B1, B2, B3], [C4, C5, C6], [B7, B8, B9]],
        [[C1, C2, C3], [D4, D5, D6], [C7, C8, C9]],
        [[D1, D2, D3], [A4, A5, A6], [D7, D8, D9]],
        [[F1, F2, F3], [F4, F5, F6], [F7, F8, F9]]
    ].

s([
    [[E1, E2, E3], [E4, E5, E6], [E7, E8, E9]],
    [[A1, A2, A3], [A4, A5, A6], [A7, A8, A9]],
    [[B1, B2, B3], [B4, B5, B6], [B7, B8, B9]],
    [[C1, C2, C3], [C4, C5, C6], [C7, C8, C9]],
    [[D1, D2, D3], [D4, D5, D6], [D7, D8, D9]],
    [[F1, F2, F3], [F4, F5, F6], [F7, F8, F9]]
    ], TransformedRC) :- 
        TransformedRC = [
        [[E1, E2, E3], [D8, D5, D2], [E7, E8, E9]],
        [[A1, A2, A3], [A4, A5, A6], [A7, A8, A9]],
        [[B1, E4, B3], [B4, E5, B6], [B7, E6, B9]],
        [[C1, C2, C3], [C4, C5, C6], [C7, C8, C9]],
        [[D1, F4, D3], [D4, F5, D6], [D7, F6, D9]],
        [[F1, F2, F3], [B8, B5, B2], [F7, F8, F9]]
    ].
    
s_prime([
    [[E1, E2, E3], [E4, E5, E6], [E7, E8, E9]],
    [[A1, A2, A3], [A4, A5, A6], [A7, A8, A9]],
    [[B1, B2, B3], [B4, B5, B6], [B7, B8, B9]],
    [[C1, C2, C3], [C4, C5, C6], [C7, C8, C9]],
    [[D1, D2, D3], [D4, D5, D6], [D7, D8, D9]],
    [[F1, F2, F3], [F4, F5, F6], [F7, F8, F9]]
    ], TransformedRC) :- 
        TransformedRC = [
        [[E1, E2, E3], [B2, B5, B8], [E7, E8, E9]],
        [[A1, A2, A3], [A4, A5, A6], [A7, A8, A9]],
        [[B1, F6, B3], [B4, F5, B6], [B7, F4, B9]],
        [[C1, C2, C3], [C4, C5, C6], [C7, C8, C9]],
        [[D1, E6, D3], [D4, E5, D6], [D7, E4, D9]],
        [[F1, F2, F3], [D2, D5, D8], [F7, F8, F9]]
    ].