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
#include <regex>
#include <string>
#include <cstdlib>
#include <curl/curl.h>
#include <ctime>


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
          << cliente.GetPassword() << " "
          << cliente.GetEmail() << "\n";
}


/// @brief Verifica letras, números, guiones bajos y puntos antes del @ y después un dominio
/// @param correo 
/// @return true si es válido, false si no
bool IsValidEmail(const std::string& correo) {
  // Expresión regular para validar correos
  const std::regex patron(R"((^[a-zA-Z0-9_.+-]+@[a-zA-Z0-9-]+\.[a-zA-Z0-9-.]+$))");
  return std::regex_match(correo, patron);
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


  std::cout << "Por favor, ingrese su email: ";
  std::string email;
  std::cin >> email;

  while(!IsValidEmail(email)) {
    std::cout << "Correo no válido. Ingrese un email válido: ";
    std::cin >> email;
  }

  std::cout << "Correo válido.\n";
  
  if(rol == 0) {
    cliente = new User(username, password, email);
  }
  else {
    cliente = new Admin(username, password, email);
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



void SendRecoveryEmail(const std::string& email, const std::string& code) {
  std::cout << "Simulando envío de correo a " << email << "...\n";
  std::cout << "------------------------------------------\n";
  std::cout << "ASUNTO: Código de recuperación\n";
  std::cout << "Hola,\nTu código de recuperación es: " << code << "\n";
  std::cout << "------------------------------------------\n";
}



std::string GenerateRecoveryCode() {
  std::srand(static_cast<unsigned int>(std::time(nullptr)));
  std::string code;
  for(int i = 0; i < 6; ++i) {
    code += std::to_string(std::rand() % 10);
  }
  return code;
}


std::string EmailClient(std::string name) {
  std::ifstream lista("database/data_user");
  if (!lista.is_open()) {
    std::cerr << "No se pudo abrir el archivo de usuarios.\n\n";
    return "0"; // Error de archivo
  }
  // Vamos viendo username por username en la base de datos
  std::string linea;
  while(std::getline(lista, linea)) {
    std::istringstream iss(linea);
    std::string username;
    int rol;
    std::string password;
    std::string email;
    iss >> username >> rol >> password >> email;
    if(username == name) {
      return email;
    }
  }
  return "0";
}


int RecoverPassword() {
  std::string username;
  std::cout << "=== Recuperación de Contraseña ===\n";
  std::cout << "Introduce tu nombre de usuario: ";
  std::cin >> username;
  
  std::string email = EmailClient(username);
  if(email == "0") {
    std::cout << "El usuario no existe o no se pudo acceder a la base de datos.\n\n";
    return 1;
  }

  std::string code = GenerateRecoveryCode();
  SendRecoveryEmail(email, code);

  std::string input_code;
  std::cout << "Introduce el código de recuperación enviado: ";
  std::cin >> input_code;

  while(input_code != code) {
    std::cout << "Código incorrecto. Introduce el código de recuperación enviado.\n";
    std::cout << "También, puede cerrar el proceso introduciendo 'QUIT'.\n";
    std::cin >> input_code;
    if(input_code == "QUIT") {
      std::cout << "\n";
      return 1;
    }
  }
  
  std::string new_password;
  std::cout << "Código correcto. Introduce tu nueva contraseña: ";
  std::cin >> new_password;

  // Reescribimos la contraseña en el archivo
  std::ifstream file_in("database/data_user");
  std::ofstream file_out("database/temp_user");
  std::string line;

  while(std::getline(file_in, line)) {
    std::istringstream iss(line);
    std::string file_username, role, password, file_email;
    iss >> file_username >> role >> password >> file_email;

    if(file_username == username) {
      file_out << file_username << " " << role << " " << new_password << " " << file_email << "\n";
    } 
    else {
      file_out << file_username << " " << role << " " << password << " " << file_email << "\n";
    }
  }

  file_in.close();
  file_out.close();

  std::remove("database/data_user");
  std::rename("database/temp_user", "database/data_user");
  std::cout << "¡Contraseña actualizada exitosamente!\n\n"; 
  return 0;
}




#endif