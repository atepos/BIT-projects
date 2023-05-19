## Implementační dokumentace k úloze interpret.py do IPP 2022/23
## Jméno: Petr Kaška (xkaska01)

### Popis programu:

Program implementuje všechna rozšíření a jejich kombinaci. To znamená, že do výsledné statistiky jsou zahrnuty i instrukce z obou rozšíření (STACK, FLOAT). Dále je podporována práce s datovým typem float pro instrukce v zásobníkové variantě. Pouze instrukce DIV z rozšíření FLOAT není implementována v zásobníkové variantě.

Program začíná načtením a kontrolou uživatelsky zadaných argumentů v instanci třídy `ProcessArgs`. Argumenty s parametrem `--stats` jsou považovány za validní i v případě duplicitního zadání parametrů, např. `--stats stats.out --vars --vars`. Následně je kód odeslán knihovně `xml.etree.ElementTree` pro kontrolu správnosti XML vstupu. Tato kontrola probíhá v instanci třídy `ParseT`, která také zpracovává návěští tak, aby byla v případě dopředných skoků již definována. Po zpracování vstupního těla programu a kontrole správnosti uživatelských argumentů se spustí hlavní část těla programu, která se skládá z jednoho `while` cyklu. V každé iteraci seřadí program argumenty jednotlivých instrukcí podle jejich `tagů`. Poté pro každou instrukci vytvoří novou instanci třídy `InstructionOnLineCheck`, která ukládá informace o jednotlivých argumentech (operační kód, počet argumentů a obsah jednotlivých argumentů) do slovníku s názvem `argdictionary`. V jednotlivých objektech vytvořených ze třídy `InstructionOnLineCheck` dochází k několika kontrolám, které jsou společné pro všechny instrukce. Tyto kontroly jsou prováděny pro přesnější ověření správnosti instrukcí a zajištění toho, že při jejich vykonávání nebude nutné provádět další kontroly. Mezi tyto kontroly patří například kontrola existence definice proměnné a její inicializace, existence rámce (při definici proměnných), validita instrukcí, validita operandů jednotlivých instrukcí (počet a typ) a převod datového typu - přetypování. Při převodu datového typu se vstup brání jako řetězec a pokud je například u proměnné typu `int` očekáváno číslo, vyhodí se výjimka v případě přítomnosti jiného typu.

Dále je volána speciální metoda na převod escape sekvencí na ekvivalentní hodnoty v ASCII, která obsahuje dvě podmínky. Pokud první podmínka vyhodnotí, že `string_value` není rovno `STRING`, kód se vrátí zpět. Pokud je `string_value` rovno `STRING`, kód pokračuje. Druhá podmínka kontroluje, zda je hodnota proměnné `value` prázdná nebo ne. Pokud je hodnota `None`, nastaví se proměnná value na prázdný řetězec. Pokud hodnota není `None`, použije se regulární výraz pro nahrazení řetězců ve formátu `\d{3}` odpovídajícími znaky ASCII.

Nakonec se přidá nový záznam do slovníku `argdictionary` obsahující klíč `number` a hodnoty `type` a `value`, které jsou přiřazeny proměnným `string_value` a `value`. Kontrola je tím dokončena a instrukce může být vykonána. Nejprve je z objektů třídy `InstructionOnLineCheck` extrahována struktura `argdictionary` do proměnné `args`. Provádění instrukcí má na starosti instance třídy `ExecuteCommands`, ve které je metoda pro volání každé instrukce. Instrukce se volají pomocí slovníku instrukcí `callíng_func`, ve kterém se najde pomocí klíče (názvu instrukce), příslušná metoda a metoda pro vykonání instrukce je zavolána. Pro korektní používání instrukcí skokových instrukcí je v hlavním těle programu čítač, který se s provedením každé instrukce inkrementuje a v případě skoku je nastaven na přislušnou hodnotu návěští. 

### Datové Struktury 

1. `instruction_num_of_arguments` - struktura, pomocí které je kontrolován počet argumentů u jednotlivých instrukcí
2. `instruction_formats` - struktura, která uchovává, kterých typů můžou jednotlivé argumenty instrukcí nabývat
3. `calling_func` - struktura, která slouží k volání metod sloužících k vykonání instrukcí
4. `dict_of_variables` - slovník všech aktuálně platných proměnných a zároveň jediná struktura ukládající proměnné. Při práci s rámci (funkce `PUSHFRAME`, `POPFRAME`), jsou lokální proměnné uloženy na zásobník proměnných. a následně proměnné s označením `TF@*` na `LF@*`.
5. `label_positions` - slovník, který uchovává návěští
6. `data_stack` - zásobníková struktura pro instrukce, které pracují se zásobníkem
7. `call_jump` - zásobník, který slouží pro volání instrukcí (`CALL` a `RETURN` instrukce)
8. `iterator` - int, uchovávající, kde se číslo instrukce, při provedení instrukce se inkrementuje (slouží při skokových instrukcích)
9. `creating_temporary_frame` a `is_temporary_frame` - bool hodnoty, které slouží k vyhodnocení chyby (např. když přijde instrukce `POPFRAME`, ale žádný LF rámec není k dispozici)

### UML diagram 
![Alt text](IPP.png "Optional title")
