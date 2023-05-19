# IDS projekt

## Links
- [Zadání](https://moodle.vut.cz/mod/wiki/view.php?pageid=186)
- [SQL notes](https://full-snowflake-d30.notion.site/SQL-a24e5aae3504427d869af87488fbfd48)
- [LucidCharts](https://lucid.app/lucidchart/cca44e17-f3f0-4da2-8a8c-19ed0104a8ab/edit?invitationId=inv_e8e7b35d-3c93-4c11-8cb2-8e1cf9a082bd&fbclid=IwAR2Lr_BSHpjwn7jgKWNChHRv5YThfqPygNpN28ng4ObxBZrp4b5Jzuq77-c&page=0_0#)

## Varianta zadání - Rezervace letenek
Vaším úkolem je navrhnout webovou aplikaci pro rezervaci letenek. Systém musí uživateli umožnit specifikovat požadavek pomocí místa odletu a příletu, data, času, třídy, letecké společnosti apod. Jedno letadlo může létat na více letech, stejně tak mezi dvěmi destinacemi může létat více letadel různých společností. Zákazník si rezervuje letenku, která může být na několik letů, i různých společností (např. letenka Praha -> San Francisco s lety Praha -> Londýn ČSA, Londýn -> San Francisco British Airways). Systém musí umožnit klientovi tisk letového itineráře, který obsahuje informace o dobách příletů a odletů na jednotlivých letištích. Každý typ letadla má různý počet sedadel a jejich rozvržení do tříd. Nemodelujte jednotlivá sedadla. Rezervací může klient zamluvit i více míst v jednom letu. Technická mezipřistání modelovat nemusíte. Systém musí evidovat, která společnost kdy, odkud a kam létá. Cena sedadla v dané třídě může být u každé společnosti jiná, cena letenky je dána součtem cen všech rezervovaných sedadel na všech letech. Pokud klient rezervovanou letenku včas nezaplatí, rezervace se z databáze smaže.

[IUS stránka s projekty](https://www.fit.vutbr.cz/study/courses/IUS/private/projekt.html)

## TODO :white_check_mark:
[Board](https://github.com/users/hemzamartin/projects/2)
### 1. část
- Vykrást IUS :white_check_mark:
- Digram tříd nebo ER diagram  :white_check_mark:
- Diagram případů užití  :white_check_mark:

### 2. část
- Task  :white_check_mark:

### 3. část
- Task  :white_check_mark:

### 4. část
- Task 1/2 *  :white_check_mark:

## Deadlines
- **12.3.** - odevzdání první části projektu (ER diagram, diagram případů užití)
- **26.3.** - odevzdání druhé části projektu (SQL skript pro vytvoření základních objektů schématu databáze)
- **21.3.** - Transformace konceptuálního modelu na schéma DB 
- **9.4.** - odevzdání třetí části projektu (SQL skript s dotazy SELECT)
- **1.5.** - odevzdání čtvrté části projektu (SQL skript s pokročilými objekty databáze)

## Poznámky
### General
- skripty musí umožňovat opakované volání, tedy odstranění a opětovné vytvoření, nebo rovnou přepsání
- část projektu s diagramy se odevzdá jako jeden PDF soubor
- SQL skripty vždy v jednom souboru UTF-8

### 1. část - Datový model (ERD) a model případů užití
- digram tříd v UML nebo ER diagram např. v tzv. [Crow's Foot](https://cs.wikipedia.org/wiki/Entity-relationship_model#Crow.27s_Foot) notaci
- diagram případů užití v UML
- alespoň jeden vztah generalizace/specializace
- stručný popis datového modelu

### 2. část - SQL skript pro vytvoření objektů schématu databáze
- skript pro vytváření tabulek, vč. definice integritních omezení (zejména primárních a cizích klíčů)
- naplnění ukázkovými daty
- musí odpovídat datovému modelu z předchozí části (případně upravit diagramy)
- alespoň jeden sloupec se speciálním omezením hodnot (RČ, IČ, IČPE, viz [zadání](https://moodle.vut.cz/mod/wiki/view.php?pageid=186)), musí povolit pouze platné hodnoty (implementujte pomocí CHECK integritního omezení)
- realizace vztahu generalizace/specializace (strčeně vysvětlena v SQL komentáři)
- musí obsahovat automatické generování primárního klíče

1. Customer :white_check_mark:
2.  Travel_Agent :white_check_mark:
3. Ticket_Status :white_check_mark:
4. Class :white_check_mark:
5. Airline :white_check_mark:
6. Routes :white_check_mark:
7. Airplane :white_check_mark:
8. Flight :white_check_mark:
9. Class_Price :white_check_mark:
10. Flight_Schedule :white_check_mark:
11. Ticket :white_check_mark:
12. Reservation :white_check_mark:
13. Seat_Configuration :white_check_mark:

### 3. část - SQL skript s dotazy SELECT
- skript, který vytvoří tabulky a naplní ukázkovými daty (stejně jako skript v bodě 2) a poté provede několik dotazů SELECT 
- alespoň dva dotazy využívající spojení tabulek
- jeden využívající spojení tří tabulek
- dva dotazy s klauzulí GROUP BY a agregační funkcí
- jeden dotaz obsahující predikát EXISTS
- jeden dotaz s predikátem IN s vnořeným SELECTem (nikoliv IN s množinou konstantních dat)
- tj. celkem minimálně 7 dotazů
- u každého dotazu musí být v komentáři stručně popsáno, jaká data hledá

### 4. část - SQL skript pro vytvoření pokročilých objektů schématu databáze
- vytvoří základní objekty a schéma databáze, naplní ukázkovými daty (stejně jako skript v bodě 2)
- zadefinuje či vytvoří pokročilá omezení či objekty databáze dle upřesňujících požadavků zadání
- ukázkové příkazy manipulace dat a dotazy demonstrující použití výše zmiňovaných omezení a objektů tohoto skriptu (viz [zadání](https://moodle.vut.cz/mod/wiki/view.php?pageid=186))
- zbytek viz [zadání](https://moodle.vut.cz/mod/wiki/view.php?pageid=186)
