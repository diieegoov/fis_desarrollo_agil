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

int SearchClient(std::string name);

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
    std::cout << "Admin " << this->GetUsername() << " logged out.\n";
  }

  void Read(std::istream& in) override {
    in >> username_;
  }

  void Tasks() {
    std::cout << "¿Quieres asignar tareas a un usuario o ver las tuyas?:\n";
    std::cout << "[0] Asignar tareas a usuario.\n";
    std::cout << "[1] Ver las tareas propias.\n";
    unsigned opcion;
    std::cin >> opcion;
    if(opcion == 0) {  // Asignar
      system("clear");
      std::cout << "Elegir usuario a asignar tareas: ";
      std::string username;
      std::cin >> username;
      while(!SearchClient(username)) {  // Manejo de errores
        std::cout << "\nEl usuario no se encuentra. Ingrese un nombre válido.\n";
        std::cout << "También, puede cerrar el proceso introduciendo 'QUIT'.\n";
        std::cin >> username;
        if(username == "QUIT") {
          std::cout << "\n";
        }
      }
      std::cout << "¿Qúe nueva tarea quiere añadir al usuario " << username << "?:\n";
      std::cin.ignore();  // limpiar entrada
      std::string task;
      std::getline(std::cin, task);

      std::ifstream file_in("database/tasks");
      std::stringstream buffer;
      bool found_user = false;
      std::string line;
      std::string section;
      std::vector<std::string> new_content;

      while (std::getline(file_in, line)) {
        // Si encontramos el nombre del usuario, comenzamos a procesar su sección
        if (line == username) {
          found_user = true;
          new_content.push_back(line);
          int task_index = 0;

          // Añadir sus tareas existentes
          while(std::getline(file_in, line) && line != "-") {
            if(line[0] == '[') {
              ++task_index;
              new_content.push_back(line);
            }
          }

          // Añadir la nueva tarea
          new_content.push_back("[" + std::to_string(task_index) + "] " + task);
          new_content.push_back("-");
        } 
        else {
          new_content.push_back(line);
        }
      }
      file_in.close();

      if(!found_user) {
        // Añadir nueva entrada al final
        new_content.push_back(username);
        new_content.push_back("[0] " + task);
        new_content.push_back("-");
      }

      // Reescribir archivo completo
      std::ofstream file_out("database/tasks");
      for(const auto& l : new_content) {
        file_out << l << "\n";
      }
      file_out.close();

      std::cout << "Tarea asignada correctamente.\n";
    }
    else if(opcion == 1) {  // Ver lista
      this->PrintTasks();
    }
    else {
      std::cout << "Opción incorrecta: Saliendo del proceso...\n";
    }
  }

  void PrintTasks() {
    std::ifstream file("database/tasks");
    if (!file.is_open()) {
        std::cerr << "No se pudo abrir el archivo de tareas.\n";
        return;
    }

    std::string username = this->GetUsername();
    std::string line;
    bool found = false;
    std::vector<std::string> new_file_lines;
    std::vector<std::string> user_tasks;

    while (std::getline(file, line)) {
        if (line == username) {
            found = true;
            new_file_lines.push_back(line);  // username

            while (std::getline(file, line) && line != "-") {
                if (!line.empty() && line[0] == '[') {
                    user_tasks.push_back(line);
                }
            }

            // Mostrar tareas
            std::cout << "Tareas de " << username << ":\n";
            for (const auto& task : user_tasks) {
                std::cout << "  " << task << "\n";
            }

            // Opción: eliminar tarea completada
            std::cout << "\n¿Desea marcar alguna tarea como completada y eliminarla? (s/n): ";
            std::string resp;
            std::cin >> resp;
            if (resp == "s" || resp == "S") {
                std::cout << "Ingrese el número de la tarea a eliminar: ";
                int idx;
                std::cin >> idx;
                if (idx >= 0 && static_cast<size_t>(idx) < user_tasks.size()) {
                    user_tasks.erase(user_tasks.begin() + idx);
                    std::cout << "Tarea eliminada.\n";
                } else {
                    std::cout << "Índice inválido.\n";
                }
            }

            // Opción: marcar favorita
            std::cout << "\n¿Desea marcar alguna tarea como favorita? (s/n): ";
            std::cin >> resp;
            if (resp == "s" || resp == "S") {
                std::cout << "Ingrese el número de la tarea a marcar como favorita: ";
                int idx;
                std::cin >> idx;
                if (idx >= 0 && static_cast<size_t>(idx) < user_tasks.size()) {
                    // Extraer contenido
                    size_t pos = user_tasks[idx].find(']');
                    std::string contenido = user_tasks[idx].substr(pos + 2);
                    // Evitar doble estrella
                    if (contenido.find("★") == std::string::npos) {
                        contenido += " ★";
                    }
                    user_tasks[idx] = contenido; // guardamos sin índice para reindexar
                } else {
                    std::cout << "Índice inválido.\n";
                }
            }

            // Reindexar y guardar tareas
            for (size_t i = 0; i < user_tasks.size(); ++i) {
                std::string contenido = user_tasks[i];
                size_t pos = contenido.find(']');
                if (pos != std::string::npos) {
                    contenido = contenido.substr(pos + 2);
                }
                new_file_lines.push_back("[" + std::to_string(i) + "] " + contenido);
            }

            new_file_lines.push_back("-");

        } else {
            new_file_lines.push_back(line);
        }
    }

    file.close();

    if (!found) {
        std::cout << "No se encontraron tareas para el usuario " << username << ".\n";
        return;
    }

    // Reescribir archivo completo
    std::ofstream out("database/tasks");
    for (const auto& l : new_file_lines) {
        out << l << "\n";
    }
    out.close();
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


int SearchClient(std::string name) {
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

#endif