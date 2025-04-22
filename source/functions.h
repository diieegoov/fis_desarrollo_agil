// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: FIS
// Curso: 2º
// Práctica Desarrollo ágil: Historias de usuario y criterios de aceptación
// Autor: Diego Vegas Acosta
// Correo: alu0101651361@ull.edu.es
// Fecha: 22/04/2025
// Archivo functions.h: Funciones auxiliares a main.h

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include "../users/client.h"
#include "../users/user.h"
#include "../users/admin.h"


/// @brief Comprueba si el usuario que pasa por parámetros está su nombre en la database data_user ya
/// @param name 
/// @return 0 cliente no encontrado, 1 si sí 
int SearchAlreadyClient(std::string name) {
  std::ifstream lista("database/data_user");
  if (!lista.is_open()) {
    std::cerr << "No se pudo abrir el archivo de usuarios.\n";
    return -1; // Error de archivo
  }
  // Vamos viendo username por username en la base de datos
  std::string linea;
  while(std::getline(lista, linea)) {
    std::istringstream iss(linea);
    std::string username;
    int rol;
    std::string password;
    iss >> username >> rol >> password;
    if(username == name) {
      return 1;
    }
  }
  return 0;
}


/// @brief Añade el cliente por parámetros a la database data_user
/// @param cliente 
void AddClient(const Client& cliente) {
  std::ofstream archivo("database/data_user", std::ios::app);  // `app` para añadir sin borrar lo anterior

  archivo << cliente.GetUsername() << " "
          << cliente.GetRole() << " "
          << cliente.GetPassword() << "\n";
}


/// @brief Registra al usuario con la información que da el usuario
/// @return 1 si no es posible registrarse, 0 si sí
int SignIn() {
  std::cout << "Iniciando proceso de registro de cliente. Registrarse con rol:\n";
  std::cout << "0 - Usuario\n";
  std::cout << "1 - Administrador\n";
  int rol;
  std::cin >> rol;
  while(rol != 0 && rol != 1) {
    std::cout << "Rol inválido.\n";
    std::cout << "Por favor, ingrese un rol de cliente válido:\n";
    std::cout << "0 - Usuario\n";
    std::cout << "1 - Administrador\n";
    std::cin >> rol;
  }
  Client* cliente;
  std::cout << "Por favor, ingrese el nombre del ";
  if(rol == 0) {
    std::cout << "usuario: ";
  }
  else {
    std::cout << "administrador: ";
  }
  std::string username;
  std::cin >> username;

  if(SearchAlreadyClient(username)) {
    std::cout << "El cliente " << username << " ya existe.\n";
    std::cout << "No se ha podido crear correctamente.\n"; 
    return 1;
  }

  std::cout << "Por favor, ingrese su contraseña: ";
  std::string password;
  std::cin >> password;
  
  if(rol == 0) {
    cliente = new User(username, password);
  }
  else {
    cliente = new Admin(username, password);
  }

  std::cout << "El cliente "  << cliente->GetUsername() << " de rol [";
  if(cliente->GetRole()) {
    std::cout << "ADMINISTRADOR] se ha creado correctamente.\n";
  }
  else {
    std::cout << "USUARIO] se ha creado correctamente.\n";
  }
  AddClient(*cliente);

  return 0;
}

#endif