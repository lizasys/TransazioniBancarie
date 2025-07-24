#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

class Transazione {
public:
    std::string tipo;
    std::string data;
    double importo;
    std::string descrizione;

    Transazione(std::string t, std::string d, double i, std::string desc)
        : tipo(t), data(d), importo(i), descrizione(desc) {}

    std::string toFileString() const {
        return tipo + ";" + data + ";" + std::to_string(importo) + ";" + descrizione;
    }

    static Transazione fromFileString(const std::string& riga) {
        std::stringstream ss(riga);
        std::string tipo, data, importoStr, descrizione;

        std::getline(ss, tipo, ';');
        std::getline(ss, data, ';');
        std::getline(ss, importoStr, ';');
        std::getline(ss, descrizione);

        return Transazione(tipo, data, std::stod(importoStr), descrizione);
    }
};

class ContoCorrente {
private:
    std::vector<Transazione> transazioni;

public:
    void aggiungiTransazione(const Transazione& t) {
        transazioni.push_back(t);
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

TEST_CASE("Transazione parsing da stringa", "[Transazione]") {
    std::string riga = "INGRESSO;2025-07-23;1000.5;Stipendio";
    Transazione t = Transazione::fromFileString(riga);

    REQUIRE(t.tipo == "INGRESSO");
    REQUIRE(t.data == "2025-07-23");
    REQUIRE(t.importo == Catch::Approx(1000.5));
    REQUIRE(t.descrizione == "Stipendio");
}

TEST_CASE("Calcolo saldo con transazioni", "[ContoCorrente]") {
    ContoCorrente conto;
    conto.aggiungiTransazione(Transazione("INGRESSO", "2025-07-23", 1000.0, "Stipendio"));
    conto.aggiungiTransazione(Transazione("USCITA", "2025-07-24", 200.0, "Spesa"));

    REQUIRE(conto.calcolaSaldo() == Catch::Approx(800.0));
}