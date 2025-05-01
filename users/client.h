// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: FIS
// Curso: 2º
// Práctica Desarrollo ágil: Historias de usuario y criterios de aceptación
// Autor: Diego Vegas Acosta
// Correo: alu0101651361@ull.edu.es
// Fecha: 22/04/2025
// Archivo client.h: Clase abstracta representativa de los clientes
//                   Contiene a los usuarios y a los administradores


#ifndef CLIENT_H
#define CLIENT_H

#include <algorithm> 
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

class Client {
 public:
  virtual ~Client() {};  // Destructor virtual

  // Métodos
  virtual void Login() = 0;
  virtual void Logout() = 0;

  // Sobrecargas
  virtual void Read(std::istream& in) = 0;

  virtual void Tasks() = 0;

  // Sobrecarga del operador >>
  friend std::istream& operator>>(std::istream& in, Client& client) {
    client.Read(in);
    return in;
  }

  // Getters
  virtual std::string GetUsername() const = 0;
  virtual std::string GetPassword() const = 0;
  virtual std::string GetEmail() const = 0;
  virtual bool GetRole() const = 0;  // Usaremos true como admin
  
};


#endif