// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: FIS
// Curso: 2º
// Práctica Desarrollo ágil: Historias de usuario y criterios de aceptación
// Autor: Diego Vegas Acosta
// Correo: alu0101651361@ull.edu.es
// Fecha: 22/04/2025
// Archivo user.h: Clase representativa de los clientes usuarios 

#ifndef USER_H
#define USER_H

#include "client.h"

class User : public Client {
 public:
  User() {};  // Constructor por defecto
  User(const std::string& name) : username_(name), role_(false) {}


  // Métodos
  void Login() override {
    // std::cout << "User " << username << " logged in.\n";
  }

  void Logout() override {
    // std::cout << "User " << username << " logged out.\n";
  }

  void Read(std::istream& in) override {
    in >> username_;
  }

  // Getters
  std::string GetUsername() const override { return username_; }
  bool GetRole() const override { return role_; }

 private:
  std::string username_;
  bool role_;
};


#endif