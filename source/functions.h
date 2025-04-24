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


/// @brief Simulación de email de recuperación de contraseña
/// @param email email del usuario
/// @param code codigo que debe recibir e introducir el cliente
void SendRecoveryEmail(const std::string& email, const std::string& code) {
  std::cout << "Simulando envío de correo a " << email << "...\n";
  std::cout << "------------------------------------------\n";
  std::cout << "ASUNTO: Código de recuperación\n";
  std::cout << "Hola,\nTu código de recuperación es: " << code << "\n";
  std::cout << "------------------------------------------\n";
}


/// @brief Genera un código aleatorio que el cliente debe introducir
/// @return codigo aleatorio de 6 dígitos
std::string GenerateRecoveryCode() {
  std::srand(static_cast<unsigned int>(std::time(nullptr)));
  std::string code;
  for(int i = 0; i < 6; ++i) {
    code += std::to_string(std::rand() % 10);
  }
  return code;
}


/// @brief Busca el email del cliente cuyo nombre se pasa por parámetro
/// @param name nombre del cliente del que se extraerá el email
/// @return string 0 si no se encuentra el usuario, sino el email de este
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


/// @brief Usa todas las funciones auxiliares para hacer el proceso de recuperar la contraseña
/// @return 1 si es intento fallido, 0 si no
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


/// @brief Encuentra y da al usuario su email enlazado a su username
/// @return 1 si no puede encontrar el username, 0 si sí
int RecoverEmail() {
  std::string username;
  std::cout << "=== Recuperación de Email ===\n";
  std::cout << "Introduce tu nombre de usuario: ";
  std::cin >> username;
  
  std::string email = EmailClient(username);
  if(email == "0") {
    std::cout << "El usuario no existe o no se pudo acceder a la base de datos.\n\n";
    return 1;
  }
  std::cout << "Cliente encontrado. Tu email es: " << email << "\n\n";
  return 0;
}


Client* BuildClient(std::string name) {
  std::ifstream lista("database/data_user");
  // Vamos viendo username por username en la base de datos
  Client* cliente;
  std::string linea;
  while(std::getline(lista, linea)) {
    std::istringstream iss(linea);
    std::string username;
    int rol;
    std::string password;
    std::string email;
    iss >> username >> rol >> password >> email;
    if(username == name) {
      if(rol == 1) {
        cliente = new Admin(name, password, email);
      }
      else {
        cliente = new User(name, password, email);
      }
      return cliente;
    }
  }
  return cliente;
}


/// @brief Comprueba el nombre y contraseña por el usuario y hace login con el tipo del objeto
/// @return string con el nombre del cliente o "0" si hay algún error en el proceso
std::string Login() {
  std::string username;
  std::cout << "=== Iniciar Sesión ===\n";
  std::cout << "Introduce tu nombre de usuario: ";
  std::cin >> username;
  while(!SearchAlreadyClient(username)) {
    std::cout << "\nUsuario no encontrado. Introduzca un nombre válido\n";
    std::cout << "También, puede cerrar el proceso introduciendo 'QUIT'.\n";
    std::cin >> username;
    if(username == "QUIT") {
      std::cout << "\n";
      return "0";
    }
  }

  Client* cliente = BuildClient(username);

  std::cout << "\nNombre de usuario válido, introduzca su contraseña: ";
  std::string password;
  std::cin >> password;
  int contador = 3;
  while(password != cliente->GetPassword()) {
    if(contador == 0) {
      std::cout << "Número de intentos agotados. Se cierra el proceso...\n\n";
      return "0";
    }
    std::cout << "Contraseña incorrecta. Introduzca una válida [" << contador << " intentos restantes].\n";
    std::cout << "También, puede cerrar el proceso introduciendo 'QUIT'.\n";
    std::cin >> password;
    if(password == "QUIT") {
      std::cout << "\n";
      return "0";
    }
    contador--;
  }
  std::cout << "Contraseña válida. Iniciando sesión...\n\n";
  return cliente->GetUsername();
}


void Logout(std::string& username) {
  Client* cliente = BuildClient(username);
  cliente->Logout();
}

/// @brief Invita a colaboradores al proyecto
void InvitarColab(Client* client) {
  std::ifstream project_list("database/project_list");
  std::string project_name;
  std::string buffer;
  std::string allowed_projects;

  std::cout << "Lista de proyectos:" << std::endl;
  while (std::getline(project_list, project_name)) {
    std::ifstream project_data{"database/" + project_name + "_data"};
    while (std::getline(project_data, buffer)) {
      if (buffer.substr(0, buffer.find(" ")) == client->GetUsername()) {
        allowed_projects += project_name + " ";
        std::cout << project_name << std::endl;
      }
    }
    project_data.close();
  }
  project_list.close();

  size_t pos{allowed_projects.find(" ")};
  std::string selection;
  bool allowed{0};

  while (!allowed) {
    std::cout << "\nSeleccione proyecto: ";
    std::cin >> selection;

    if (selection == "QUIT") { return; }
    while (pos != std::string::npos) {
      if (allowed_projects.substr(0, pos) == selection) {
        allowed = 1;
        break;
      }
      pos = allowed_projects.find(" ", ++pos);
    }
    if (!allowed) { std::cout << "El proyecto seleccionado no existe o no tiene acceso al mismo, inténtelo de nuevo. QUIT para salir" << std::endl; }
  }

  std::string username;
  std::cout << "Introduzca el nombre del usuario a invitar: ";
  std::cin >> username;

  if (SearchAlreadyClient(username))  {
    std::cout << "Seleccione permisos de edición (0: editor, 1: lector): ";
    int permisos;
    std::cin >> permisos;

    std::ofstream project_data("database/" + selection + "_data", std::ios::app);
    project_data << username << " " << permisos << "\n";
    project_data.close();

    std::cout << "Usuario añadido con éxito al proyecto" << std::endl;
    return;

  } else {
    std::cout << "El usuario seleccionado no está dado de alta" << std::endl;
    return;
  }
}

/// @brief Crea un proyecto en la base de datos
void CrearProyecto(Client* client) {
  std::string project_name;
  std::string project_path{"database/"};
  std::cout << "Introduzca el nombre del proyecto a crear: ";
  std::cin >> project_name;

  std::ifstream project_list_input("database/project_list");
  std::string buffer;
  while (std::getline(project_list_input, buffer)) {
    if (buffer == project_name) {
      std::cout << "Ya existe un proyecto con el mismo nombre" << std::endl;
      return;
    }
  }
  project_list_input.close();

  std::ofstream project_list_output("database/project_list", std::ios::app);
  project_list_output << project_name << "\n";
  project_list_output.close();

  std::ofstream project(project_path + project_name, std::ios::app);
  std::ofstream project_data(project_path + project_name + "_data", std::ios::app);
  if (!project.is_open() || !project_data.is_open()) {
    std::cout << "Ha ocurrido un error al crear el proyecto, inténtelo de nuevo más tarde\n" << std::endl;
  } else {
    project << "";
    project_data << client->GetUsername() << " " << "0\n";
    std::cout << "Proyecto creado con éxito\n" << std::endl;
    project.close();
    project_data.close();
  }
}


void AccederProyecto(Client* client) {
  std::ifstream project_list{"database/project_list"};
  std::string project_name;
  std::string allowed_projects;
  std::string buffer;

  std::cout << "Lista de proyectos:" << std::endl;
  while (std::getline(project_list, project_name)) {
    std::ifstream project_data{"database/" + project_name + "_data"};
    while (std::getline(project_data, buffer)) {
      if (buffer.substr(0, buffer.find(" ")) == client->GetUsername()) {
        allowed_projects += project_name + " ";
        std::cout << project_name << std::endl;
      }
    }
    project_data.close();
  }
  project_list.close();

  std::cout << "\nSeleccione un proyecto: ";
  std::string selection;
  std::cin >> selection;

  bool allowed{0};
  size_t pos{allowed_projects.find(" ")};
  while (pos != std::string::npos) {
    if (allowed_projects.substr(0, pos) == selection) {
      allowed = 1;
      break;
    }
    pos = allowed_projects.find(" ", ++pos);
  }

  if (!allowed) {
    std::cout << "El proyecto seleccionado no existe o no tiene acceso al mismo" << std::endl;
  } else {
    std::cout << "Accediendo al proyecto";

    std::string privileges;
    std::ifstream selection_data{"database/" + selection + "_data"};
    while (std::getline(selection_data, buffer)) {
      if (buffer.substr(0, buffer.find(" ")) == client->GetUsername()) {
        privileges = buffer.substr(buffer.find(" ") + 1);
      }
    }
    selection_data.close();

    if (privileges == "0") {
      std::cout << " con privilegios de escritura..." << std::endl;
      std::string command{"vim database/" + selection};
      system(command.c_str());
    } else {
      std::cout << " con privilegios de lectura..." << std::endl;
      std::cout << "Contenido proyecto:" << std::endl;
      std::string command{"cat database/" + selection};
      system(command.c_str());
      std::cout << std::endl;
    }
  }
}

#endif