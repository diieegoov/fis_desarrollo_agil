// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: FIS
// Curso: 2º
// Práctica Desarrollo ágil: Historias de usuario y criterios de aceptación
// Autor: Diego Vegas Acosta
// Correo: alu0101651361@ull.edu.es
// Fecha: 22/04/2025
// Archivo admin.h: Clase representativa de los clientes administradores

#ifndef ADMIN_H
#define ADMIN_H

#include "client.h"

class Admin : public Client {
 public:
  Admin() {};  // Constructor por defecto
  Admin(const std::string& name, const std::string& password, const std::string& email)
   : username_(name), password_(password), email_(email) {}


  // Métodos
  void Login() override {
    std::cout << "Admin " << this->GetUsername() << " logged in.\n";
  }

  void Logout() override {
    // std::cout << "Admin " << username << " logged out.\n";
  }

  void Read(std::istream& in) override {
    in >> username_;
  }

  // Getters
  std::string GetUsername() const override { return username_; }
  std::string GetPassword() const override { return password_; }
  std::string GetEmail() const override { return email_; }
  bool GetRole() const override { return role_; }


 private:
  std::string username_;
  std::string password_;
  std::string email_;
  bool role_ = true;
};


#endif