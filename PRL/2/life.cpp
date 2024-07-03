/*
 * Projekt: Projekt 2 do predmetu PRL - Game of Life
 * Autor: Petr Kaška, xkaska01@stud.fit.vutbr.cz
 * Datum: 30.3.2024
 */

// Implementace hry Game of Life s využitím MPI
// Má implementace funguje s maticí o velikosti m *n, kde (m,n ∈ N)
// algoritmus funguje jak pro liché, tak pro sudé vstupní počty procesorů
// Výsledek může být vypsán v proházeném pořadí. Protože nechávám jednotlivé procesy přistupovat k vypisování a neposílám výsledek do jednoho
#include <mpi.h>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <iterator>
#include <algorithm>

using namespace std;

/**
 * Zpracování vstupních argumentů.
 *
 * @param argc Počet vstupních argumentů.
 * @param argv Pole vstupních argumentů.
 * @param nazevSouboru Reference na proměnnou pro uložení názvu vstupního souboru.
 * @param pocetKroku Reference na proměnnou pro uložení počtu kroků simulace.
 * @return True, pokud byly argumenty úspěšně zpracovány, jinak false.
 */
bool zpracujArgumenty(int argc, char *argv[], string &nazevSouboru, int &pocetKroku)
{
    if (argc < 3)
    {
        return false;
    }

    nazevSouboru = argv[1];
    pocetKroku = stoi(argv[2]);

    return true;
}

/**
 * Zpracování souboru do 2D pole.
 *
 * @param nazevSouboru Název souboru, který se má načíst.
 * @return 2D pole reprezentující obsah souboru, kde každý řádek je reprezentován jako vektor čísel.
 * @throws std::runtime_error Pokud se nepodaří otevřít soubor.
 */
vector<vector<int>> zpracujSoubor(const string &nazevSouboru)
{
    ifstream soubor(nazevSouboru);
    if (!soubor.is_open())
    {
        cerr << "Chyba: Nelze otevřít soubor " << nazevSouboru << endl;
        exit(1);
    }

    vector<vector<int>> mriz;
    string radek;
    while (getline(soubor, radek))
    {
        vector<int> radekCisel;
        // Převedení znaku na celé číslo
        for (char znak : radek)
        {
            radekCisel.push_back(znak - '0');
        }
        mriz.push_back(radekCisel);
    }

    soubor.close();
    return mriz;
}

/**
 * Nalezení počátečního řádku pro simulaci Game of Life.
 *
 * @param radek Číslo řádku.
 * @return Počáteční řádek pro simulaci.
 */
int najdiPocatecniRadek(int radek)
{
    return max(radek - 1, 0);
}

/**
 * Nalezení koncového řádku pro simulaci Game of Life.
 *
 * @param radek Číslo řádku.
 * @param pocetRadku Celkový počet řádků.
 * @return Koncový řádek pro simulaci.
 */
int najdiKoncovyRadek(int radek, int pocetRadku)
{
    return min(radek + 1, pocetRadku - 1);
}

/**
 * Nalezení počátečního sloupce pro simulaci Game of Life.
 *
 * @param sloupec Číslo sloupce.
 * @return Počáteční sloupec pro simulaci.
 */
int najdiPocatecniSloupec(int sloupec)
{
    return max(sloupec - 1, 0);
}

/**
 * Nalezení koncového sloupce pro simulaci Game of Life.
 *
 * @param sloupec Číslo sloupce.
 * @param pocetSloupcu Celkový počet sloupců.
 * @return Koncový sloupec pro simulaci.
 */
int najdiKoncovySloupec(int sloupec, int pocetSloupcu)
{
    return min(sloupec + 1, pocetSloupcu - 1);
}

/**
 * Spočítání živých sousedů buňky.
 *
 * @param mriz 2D pole reprezentující mřížku buněk.
 * @param radek Číslo řádku buňky, pro kterou se mají spočítat živí sousedé.
 * @param sloupec Číslo sloupce buňky, pro kterou se mají spočítat živí sousedé.
 * @return Počet živých sousedů buňky.
 */
int spoctiZiveSousedy(const vector<vector<int>> &mriz, int radek, int sloupec)
{
    int ziviSousede = 0;
    int pocetRadku = mriz.size();
    int pocetSloupcu = mriz[0].size();

    int pocatecniRadek = najdiPocatecniRadek(radek);
    int koncovyRadek = najdiKoncovyRadek(radek, pocetRadku);
    int pocatecniSloupec = najdiPocatecniSloupec(sloupec);
    int koncovySloupec = najdiKoncovySloupec(sloupec, pocetSloupcu);

    for (int i = pocatecniRadek; i <= koncovyRadek; ++i)
    {
        for (int j = pocatecniSloupec; j <= koncovySloupec; ++j)
        {
            // Přeskočit aktuální buňku
            if (i == radek && j == sloupec)
                continue;
            ziviSousede += mriz[i][j];
        }
    }

    return ziviSousede;
}

/**
 * Aktualizace stavu mřížky buňek podle pravidel hry Game of Life.
 *
 * @param mriz Reference na mřížku buňek, která se má aktualizovat.
 * @param pocatecniRadek Index počátečního řádku pro aktualizaci.
 * @param koncovyRadek Index koncového řádku pro aktualizaci.
 */
void aktualizujMriz(vector<vector<int>> &mriz, int pocatecniRadek, int koncovyRadek)
{
    // Získání počtu řádků a sloupců mřížky
    int pocetRadek = mriz.size();
    int pocetSloupcu = mriz[0].size();

    // Vytvoření nové mřížky pro uložení aktualizovaného stavu
    vector<vector<int>> novaMriz(pocetRadek, vector<int>(pocetSloupcu));

    // Procházení buněk v zadaném rozsahu
    for (int i = pocatecniRadek; i <= koncovyRadek; ++i)
    {
        for (int j = 0; j < pocetSloupcu; ++j)
        {
            // Spočítání počtu živých sousedů pro aktuální buňku
            int ziveSousedi = spoctiZiveSousedy(mriz, i, j);

            // Aplikace pravidel hry Game of Life na aktuální buňku
            if (mriz[i][j] == 1)
            {
                if (ziveSousedi < 2 || ziveSousedi > 3)
                {
                    // Buňka umírá
                    novaMriz[i][j] = 0;
                }
                else
                {
                    // Buňka přežívá
                    novaMriz[i][j] = 1;
                }
            }
            else
            {
                if (ziveSousedi == 3)
                {
                    // Buňka se rodí
                    novaMriz[i][j] = 1;
                }
                else
                {
                    // Buňka zůstává mrtvá
                    novaMriz[i][j] = 0;
                }
            }
        }
    }

    // Aktualizace původní mřížky s novým stavem pro zadané řádky
    for (int i = pocatecniRadek; i <= koncovyRadek; ++i)
    {
        mriz[i] = novaMriz[i];
    }
}

/**
 * Rozdělení počtu řádků na části.
 *
 * @param pocet Počet řádků, které se mají rozdělit.
 * @param casti Počet částí, na které se mají řádky rozdělit.
 * @return Vektor obsahující počet řádků v každé části.
 */
vector<int> rozdelRady(int pocet, int casti)
{
    int rovnomerneCasti = pocet / casti;
    vector<int> vysledek(casti, rovnomerneCasti);
    int zbytek = pocet % casti;

    // Rozděl zbytek rovnoměrně mezi části
    for_each(vysledek.begin(), vysledek.begin() + zbytek, [](int &hodnota)
             { hodnota++; });

    return vysledek;
}

/**
 * Získání čísla procesu pro daný řádek.
 *
 * @param radek Číslo řádku.
 * @param radekNaProces Vektor obsahující počet řádků přidělených každému procesu.
 * @return Číslo procesu, který má zpracovávat daný řádek.
 */
int ziskejProcesRadek(int radek, const vector<int> &radekNaProces)
{
    int proces = 0;
    int aktualniRadek = 0;
    for (int i = 0; i < radekNaProces.size(); ++i)
    {
        if (aktualniRadek + radekNaProces[i] <= radek)
        {
            aktualniRadek += radekNaProces[i];
            proces++;
        }
        else
        {
            break;
        }
    }
    return proces;
}

/**
 * Simulace hry Game of Life.
 *
 * @param mriz Reference na mřížku buněk, která se má simulovat.
 * @param pocatecniRadek Index počátečního řádku pro aktualizaci.
 * @param koncovyRadek Index koncového řádku pro aktualizaci.
 * @param pocetKroku Počet kroků simulace.
 * @param radkyNaProces Vektor obsahující počet řádků přidělených každému procesu.
 */
void simulujGameOfLife(vector<vector<int>> &mriz, int pocatecniRadek, int koncovyRadek, int pocetKroku, vector<int> &radkyNaProces)
{
    for (int i = 0; i < pocetKroku; i++)
    {
        aktualizujMriz(mriz, pocatecniRadek, koncovyRadek);

        for (int i = 0; i < mriz.size(); i++)
        {
            int procesRadku = ziskejProcesRadek(i, radkyNaProces);
            MPI_Bcast(mriz[i].data(), mriz[i].size(), MPI_INT, procesRadku, MPI_COMM_WORLD);
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
    MPI_Barrier(MPI_COMM_WORLD);
}

/**
 * Výpis stavu mřížky pro daný rozsah řádků a proces.
 *
 * @param mriz Reference na mřížku buněk, jejíž stav se má vypsat.
 * @param pocatecniRadek Index počátečního řádku pro výpis.
 * @param koncovyRadek Index koncového řádku pro výpis.
 * @param rankProcesu Číslo procesu, který zpracovává dané řádky.
 */
void vypisStavMrizky(const vector<vector<int>> &mriz, int pocatecniRadek, int koncovyRadek, int rankProcesu)
{
    for (int i = pocatecniRadek; i <= koncovyRadek; i++)
    {
        cout << rankProcesu << ": ";
        for (int bunka : mriz[i])
        {
            cout << bunka;
        }
        cout << endl;
    }
}

/**
 * Inicializace MPI prostředí.
 *
 * @param argc Počet argumentů příkazové řádky.
 * @param argv Pole obsahující argumenty příkazové řádky.
 * @param rankProcesu Reference na proměnnou pro uložení ranku aktuálního procesu.
 * @param pocetProcesu Reference na proměnnou pro uložení počtu procesů.
 */
void inicializujMPI(int argc, char *argv[], int &rankProcesu, int &pocetProcesu)
{
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &pocetProcesu);
    MPI_Comm_rank(MPI_COMM_WORLD, &rankProcesu);
}

/**
 * Hlavní funkce programu.
 *
 * @param argc Počet argumentů příkazové řádky.
 * @param argv Pole obsahující argumenty příkazové řádky.
 * @return Návratový kód programu.
 */
int main(int argc, char *argv[])
{
    int rankProcesu, pocetProcesu;
    inicializujMPI(argc, argv, rankProcesu, pocetProcesu);

    string nazevSouboru;
    int pocetKroku;

    if (!zpracujArgumenty(argc, argv, nazevSouboru, pocetKroku))
    {
        return 1;
    }

    vector<vector<int>> mriz = zpracujSoubor(nazevSouboru);
    int pocetRadku = mriz.size();
    vector<int> radkyNaProces = rozdelRady(pocetRadku, pocetProcesu);

    int pocatecniRadek = 0;
    for (int i = 0; i < rankProcesu; ++i)
    {
        pocatecniRadek += radkyNaProces[i];
    }
    int koncovyRadek = pocatecniRadek + radkyNaProces[rankProcesu] - 1;

    simulujGameOfLife(mriz, pocatecniRadek, koncovyRadek, pocetKroku, radkyNaProces);

    vypisStavMrizky(mriz, pocatecniRadek, koncovyRadek, rankProcesu);

    MPI_Finalize();
    return 0;
}
