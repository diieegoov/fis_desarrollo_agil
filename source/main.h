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

#include "functions.h"

/// @brief Imprime las opciones prelogin
void PreOptions() {
  std::cout << "1. Registrarse\n";
  std::cout << "2. Iniciar sesión\n";
  std::cout << "3. Ver información de la aplicación\n";
  std::cout << "4. Cerrar sesión\n";
  std::cout << "0. Salir\n";
}


/// @brief Muestra menú sin haberse logeado y sus opciones
void Menu() {
  int opcion = -1;
  while(opcion != 0) {
    std::cout << "\n--- MENÚ PRINCIPAL ---\n";
    PreOptions();
    std::cout << "--- MENÚ PRINCIPAL ---\n";
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
        PreOptions();
        break;
    }
  }

}


#endif