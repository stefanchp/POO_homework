#include "Curs.h"
#include "CursObligatoriu.h"
#include "CursOptional.h"
#include "Student.h"
#include "Facultate.h"
#include "Exceptii.h"

#include <iostream>
#include <vector>
#include <memory>

int Curs::id_generator_ = 0;
int Student::id_generator_student_ = 0;

void test_functionalitati() 
{
    std::cout << "\n--- Demonstratie Functionalitati ---\n";
    
    std::cout << "\n1. Creare Cursuri:\n";
    std::shared_ptr<Curs> poo, bd, tw, pa, asc;
    try {
        poo = std::make_shared<CursObligatoriu>("Programare Orientata pe Obiecte", 5, 8.5, 9.0);
        bd = std::make_shared<CursObligatoriu>("Baze de Date", 5, 7.0, 7.5);
        tw = std::make_shared<CursOptional>("Tehnici Web", 4, 9.5, true);
        pa = std::make_shared<CursObligatoriu>("Programarea Algoritmilor", 6, 9.0, 10.0);
        asc = std::make_shared<CursOptional>("Arhitectura Sistemelor de Calcul", 4, 8.0, false);

        std::cout << *poo << std::endl;
        std::cout << *tw << std::endl;

        // test exceptie la creare curs
        // std::shared_ptr<Curs> cursInvalid = std::make_shared<CursObligatoriu>("Invalid", 0, 5, 5); 
    } catch (const DateCursInvalide& e) {
        std::cerr << "Eroare la crearea unui curs: " << e.what() << std::endl;
    }
    std::cout << "Numar total de cursuri create: " << Curs::get_numar_total_cursuri_create() << std::endl;


    std::cout << "\n2. Creare Studenti si Adaugare Cursuri:\n";
    Student s1("Popescu Ion");
    Student s2("Ionescu Ana");

    try {
        // teoretic aici ar trebui sa pui ce note are fiecare student
        // sincer prea multe linii de scris pentru a demonstra acelasi lucru :(((
        s1.adauga_curs(poo);
        s1.adauga_curs(bd);
        s1.adauga_curs(tw);
        // s1.adauga_curs(poo); // test exceptie CursDuplicat
        s2.adauga_curs(poo->clone()); 
        s2.adauga_curs(pa);
        s2.adauga_curs(asc);
    } catch (const ExceptieStudent& e) {
        std::cerr << "Eroare la student: " << e.what() << std::endl;
    }
    
    std::cout << s1;
    std::cout << "Medie s1: " << s1.calcul_medie_generala() << std::endl;
    std::cout << s2;
    std::cout << "Medie s2: " << s2.calcul_medie_generala() << std::endl;


    std::cout << "\n3. Demonstrare Copiere Student:\n";
    Student s3 = s1;
    s3.adauga_curs(pa); // modificam s3, s1 ramane neschimbat
    std::cout << "Student s1 (original):\n" << s1;
    std::cout << "Student s3 (copie a s1, cu PA adaugat):\n" << s3;
    
    Student s4("NUME INITIAL");
    s4 = s2; // copy assignment
    std::cout << "Student s4 (atribuit din s2):\n" << s4;

    std::cout << "\n4. Demonstrare dynamic_cast:\n";
    s1.afiseaza_cursuri_tip("Obligatoriu");
    s1.afiseaza_cursuri_tip("Optional");
    s1.afiseaza_cursuri_tip("Facultativ"); // tip care nu exista

    std::cout << "\n5. Utilizare Clasa Facultate:\n";
    Facultate fmi("Facultatea de Matematica si Informatica");
    fmi.adauga_student(s1);
    fmi.adauga_student(s2);
    fmi.adauga_student(s3);
    fmi.adauga_student(s4);
                            
    std::cout << fmi;
    std::cout << "Numar total de studenti creati vreodata (instante): " << Student::get_numar_total_studenti() << std::endl;
    std::cout << "Numar studenti in FMI: " << fmi.get_numar_studenti() << std::endl;

    std::cout << "\n6. Testare Exceptii Generale:\n";
    try {
        // simulare eroare logica in aplicatie
        throw ExceptieAppBase("O eroare generica in aplicatie a avut loc.");
    } catch (const ExceptieAppBase& e) {
        std::cerr << "Exceptie prinsa: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Exceptie std prinsa: " << e.what() << std::endl;
    }
}

int main() {
    try {
        test_functionalitati();
    } catch (const std::exception& e) {
        std::cerr << "\n!!! Eroare neasteptata in main: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}