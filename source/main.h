// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: FIS
// Curso: 2º
// Práctica Desarrollo ágil: Historias de usuario y criterios de aceptación
// Autor: Diego Vegas Acosta
// Correo: alu0101651361@ull.edu.es
// Fecha: 22/04/2025
// Archivo main.h: Fichero con funciones auxiliares al main

#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <string>

#include "../users/client.h"
#include "../users/user.h"
#include "../users/admin.h"

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
  if(rol == 0) {
    cliente = new User(username);
  }
  else {
    cliente = new Admin(username);
  }
  std::cout << "El cliente "  << cliente->GetUsername() << " de rol [";
  if(cliente->GetRole()) {
    std::cout << "ADMINISTRADOR] se ha creado correctamente.\n";
  }
  else {
    std::cout << "USUARIO] se ha creado correctamente.\n";
  }

  return 0;
}



void Menu() {
  std::cout << "\n--- MENÚ PRINCIPAL ---\n";
  std::cout << "1. Registrarse\n";
  std::cout << "2. Iniciar sesión\n";
  std::cout << "3. Ver información de la aplicación\n";
  std::cout << "4. Cerrar sesión\n";
  std::cout << "0. Salir\n";
  std::cout << "--- MENÚ PRINCIPAL ---\n";
  std::cout << "\nSeleccione una opción: ";
  int opcion = -1;
  while(opcion != 0) {
    std::cin >> opcion; 
    switch (opcion) {
      case 1: {
        system("clear");
        SignIn();
        break;
      }



      case 0: {
        std::cout << "Saliendo del programa...\n";
        break;
      }

      default :
        system("clear");
        std::cout << "Opción no válida. Seleccione una opción válida:\n";
        std::cout << "1. Registrarse\n";
        std::cout << "2. Iniciar sesión\n";
        std::cout << "3. Ver información de la aplicación\n";
        std::cout << "4. Cerrar sesión\n";
        std::cout << "0. Salir\n";
        break;
    }
  }

}


#endif