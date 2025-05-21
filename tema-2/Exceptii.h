#ifndef Exceptii_h
#define Exceptii_h

#include <stdexcept>
#include <string>

// Clasa de baza pentru exceptiile aplicatiei
class ExceptieAppBase : public std::runtime_error {
public:
    explicit ExceptieAppBase(const std::string& message)
        : std::runtime_error(message) {}
};

// Exceptii specifice pentru Curs
class ExceptieCurs : public ExceptieAppBase {
public:
    explicit ExceptieCurs(const std::string& message)
        : ExceptieAppBase("Eroare Curs: " + message) {}
};

class DateCursInvalide : public ExceptieCurs {
public:
    explicit DateCursInvalide(const std::string& message)
        : ExceptieCurs("Date invalide pentru curs - " + message) {}
};

// Exceptii specifice pentru Student
class ExceptieStudent : public ExceptieAppBase {
public:
    explicit ExceptieStudent(const std::string& message)
        : ExceptieAppBase("Eroare Student: " + message) {}
};

class CursDuplicat : public ExceptieStudent {
public:
    explicit CursDuplicat(const std::string& nume_curs)
        : ExceptieStudent("Studentul este deja inscris la cursul: " + nume_curs) {}
};

#endif