#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class Transazione {
public:
    string tipo;
    string data;
    double importo;
    string descrizione;

    Transazione(string t, string d, double i, string desc)
        : tipo(t), data(d), importo(i), descrizione(desc) {}

    string toFileString() const {
        return tipo + ";" + data + ";" + to_string(importo) + ";" + descrizione;
    }

    static Transazione fromFileString(const string& riga) {
        stringstream ss(riga);
        string tipo, data, importoStr, descrizione;

        getline(ss, tipo, ';');
        getline(ss, data, ';');
        getline(ss, importoStr, ';');
        getline(ss, descrizione);

        return Transazione(tipo, data, stod(importoStr), descrizione);
    }

    string toDisplayString(const string& utente) const {
        return "Utente: " + utente +
               "\nTipo: " + tipo +
               "\nData: " + data +
               "\nImporto: " + to_string(importo) +
               "\nDescrizione: " + descrizione +
               "\n-----------------------------\n";
    }
};

class ContoCorrente {
private:
    vector<Transazione> transazioni;

public:
    void aggiungiTransazione(const Transazione& t) {
        transazioni.push_back(t);
    }

    void salvaSuFile(const string& nomeFile) const {
        ofstream file(nomeFile);
        for (const auto& t : transazioni) {
            file << t.toFileString() << "\n";
        }
    }

    void caricaDaFile(const string& nomeFile) {
        ifstream file(nomeFile);
        string riga;
        while (getline(file, riga)) {
            if (!riga.empty()) {
                transazioni.push_back(Transazione::fromFileString(riga));
            }
        }
    }

    void stampaEstratto(const string& utente) const {
        for (const auto& t : transazioni) {
            cout << t.toDisplayString(utente);
        }
    }

    double calcolaSaldo() const {
        double saldo = 0;
        for (const auto& t : transazioni) {
            if (t.tipo == "INGRESSO") saldo += t.importo;
            else if (t.tipo == "USCITA") saldo -= t.importo;
        }
        return saldo;
    }
};

int main() {
    ContoCorrente conto;
    conto.caricaDaFile("data/transazioni.txt");

    string utente = "Marco Rossi";
    int scelta;

    do {
        cout << "\n===== MENU =====\n";
        cout << "1. Aggiungi transazione\n";
        cout << "2. Visualizza estratto conto\n";
        cout << "3. Visualizza saldo\n";
        cout << "0. Esci\n";
        cout << "Scegli un'opzione: ";
        cin >> scelta;
        cin.ignore();

        if (scelta == 1) {
            string tipo, data, descrizione;
            double importo;

            do {
                cout << "Tipo (INGRESSO o USCITA): ";
                getline(cin, tipo);
                transform(tipo.begin(), tipo.end(), tipo.begin(), ::toupper);
                if (tipo != "INGRESSO" && tipo != "USCITA") {
                    cout << "Tipo non valido. Inserisci solo INGRESSO o USCITA.\n";
                }
            } while (tipo != "INGRESSO" && tipo != "USCITA");

            cout << "Data (es. 2025-07-23): ";
            getline(cin, data);

            cout << "Importo: ";
            cin >> importo;
            cin.ignore();

            cout << "Descrizione: ";
            getline(cin, descrizione);

            conto.aggiungiTransazione(Transazione(tipo, data, importo, descrizione));
            conto.salvaSuFile("data/transazioni.txt");

            cout << "Transazione aggiunta e salvata con successo.\n";

        } else if (scelta == 2) {
            cout << "\n===== ESTRATTO CONTO =====\n";
            conto.stampaEstratto(utente);

        } else if (scelta == 3) {
            cout << "Saldo attuale: " << conto.calcolaSaldo() << " EUR\n";

        } else if (scelta == 0) {
            cout << "\n===== ESTRATTO CONTO FINALE =====\n";
            conto.stampaEstratto(utente);
            cout << "Saldo finale: " << conto.calcolaSaldo() << " EUR\n";

        } else {
            cout << "Scelta non valida. Riprova.\n";
        }

    } while (scelta != 0);

    return 0;
}
