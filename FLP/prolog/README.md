# Logický projekt do předmětu FLP

Autor: Petr Kaška (xkaska01)

## Zdrojový kód:

Je členěn do 3 částí pro načtení vstupu, rotace kostky a provádění algoritmu. Kód je popisně komentován. Každá strana kostky je reprezentovaná seznamem seznamů 3 prvků a kostka je pak seznamem stran.

## Algoritmus:

Využívá 18 možných rotací kostky - každá strana a může být rotována po směru a proti směru hodinových ručiček.
Využívá algoritmus IDS - pomocí DFS algoritmu zkouší všechny možné rotace, dokud nedosáhne výsledku. Prohledávací algoritmus je volán opakovaně s postupně rostoucí hloubkou (maximální a minimální prohledávací hloubka lze změnit -> je určena kosntantamy "0" - minimální a "30" maximální ve funkci `solve_DFS`), aby našel co nejkratší výsledek - když najde, ukončí prohledávání.

## Program:

Počet prohledaných rotací je přibližně O(r^n), kde 'r' je počet možých rotací a 'n' počet kroků
nutných ke složení. Jelikož prolog každou novou iteraci řešení počítá celou znovu, běh programu
pro 8 a více kroků se extrémně se navyšuje.

## Přiložené testovací vstupy:

Program byl testován na datech ze vstupních souborů `test[1234]\.txt`

### test1.txt

K vyřešení stačí pouze 1 rotace.

### test2.txt

K vyřešení jsou zapotřebí 4 rotace.

### test3.txt

### test4.txt

K vyřešení jsou zapotřebí 3 rotace.

Ukázka spuštění:
Linux(Ubuntu): ./flp23-log < test1.txt
