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
  std::cout << "4. Recuperar correo\n";
  std::cout << "5. Recuperar contraseña\n";
  std::cout << "0. Salir\n";
}


/// @brief Imprime la información de la aplicación
void Info() {
    std::cout << "╔══════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                 🚀 BIENVENIDO A NOTACOLLAB 🚀                ║\n";
    std::cout << "╠══════════════════════════════════════════════════════════════╣\n";
    std::cout << "║ 🌐  Tu espacio de trabajo TODO-EN-UNO                        ║\n";
    std::cout << "║      para equipos creativos y organizados                    ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════════╝\n\n";

    std::cout << "📌 FUNCIONALIDADES DESTACADAS:\n";
    std::cout << "   ┌────────────────────────────────────────────────────┐\n";
    std::cout << "   │ 📝 Notas colaborativas en tiempo real              │\n";
    std::cout << "   │ ✅ Gestión de tareas con tableros Kanban           │\n";
    std::cout << "   │ 📅 Calendarios integrados con alertas              │\n";
    std::cout << "   │ 🗂️  Bases de datos flexibles y personalizables      │\n";
    std::cout << "   │ 🔗 Integración con correo, Google Calendar y más   │\n";
    std::cout << "   │ 🧠 Wikis y documentación organizadas por proyectos │\n";
    std::cout << "   └────────────────────────────────────────────────────┘\n\n";

    std::cout << "👥 PARA EQUIPOS COMO:\n";
    std::cout << "   📌 Desarrolladores        📌 Diseñadores\n";
    std::cout << "   📌 Equipos de marketing   📌 Estudiantes\n\n";

    std::cout << "✨ VENTAJAS DE USAR NOTACOLLAB:\n";
    std::cout << "   🔄 Sincronización en tiempo real\n";
    std::cout << "   🧩 Interfaz modular y 100% personalizable\n";
    std::cout << "   💡 Mejora la productividad y organización\n";
    std::cout << "   📊 Todo tu trabajo en un solo lugar\n\n";

    std::cout << "💬 \"Organiza tu caos creativo con estilo.\"\n";
    std::cout << "🛠️  ¡Transforma ideas en acción desde un solo panel!\n";

    std::cout << "\n📎 Visítanos: https://www.notacollab.app\n";
    std::cout << "📬 Contacto: soporte@notacollab.app\n";
    std::cout << "════════════════════════════════════════════════════════════════\n\n";
}


void AdminMenu(Client* client) {
  int option{-1};
  while (option != 0) {
    std::cout << "\n--- MENÚ ADMINISTRADOR ---\n";
    std::cout << "1. Crear proyecto\n";
    std::cout << "2. Invitar colaboradores\n";
    std::cout << "3. Acceder a proyecto\n";
    std::cout << "0. Cerrar sesión\n";
    std::cout << "--- MENÚ ADMINISTRADOR ---\n";

    std::cin >> option;
    switch (option) {
      case 0: {
        system("clear");
        client->Logout();
        break;
      }

      case 1: {
        system("clear");
        CrearProyecto(client);
        break;
      }

      case 2: {
        system("clear");
        InvitarColab(client);
        break;
      }

      case 3: {
        system("clear");
        AccederProyecto(client);
        break;
      }
    }
  }
}


void UserMenu(Client* client) {
  int option{-1};
  while (option != 0) {
    std::cout << "\n--- MENÚ USUARIO ---\n";
    std::cout << "1. Acceder a proyecto\n";
    std::cout << "0. Cerrar sesión\n";
    std::cout << "--- MENÚ USUARIO ---\n";

    std::cin >> option;
    switch (option) {
      case 0: {
        system("clear");
        client->Logout();
        break;
      }

      case 1: {
        system("clear");
        AccederProyecto(client);
        break;
      }
    }
  }
}


/// @brief Muestra menú sin haberse logeado y sus opciones
void Menu() {
  int opcion = -1;
  while(opcion != 0) {
    std::cout << "--- MENÚ PRINCIPAL ---\n";
    PreOptions();
    std::cout << "--- MENÚ PRINCIPAL ---\n";
    std::cin >> opcion; 
    switch (opcion) {
      case 1: {
        system("clear");
        SignIn();
        break;
      }

      case 2: {
        system("clear");
        std::string result = Login();
        if (result == "0") {
          break;
        } else {
          Client* cliente = BuildClient(result);
          cliente->Login();  // Se llama al login del rol correspondiente
          if (cliente->GetRole()) {
            AdminMenu(cliente);
          } else {
            UserMenu(cliente);
          }
        }
        break;
      }

      case 3: {
        system("clear");
        Info();
        break;
      }

      case 4: {
        system("clear");
        RecoverEmail();
        break;
      }

      case 5: {
        system("clear");
        RecoverPassword();
        break;
      }

      case 0: {
        std::cout << "Saliendo del programa...\n";
        break;
      }

      default : {
        system("clear");
        PreOptions();
        break;
      }
    }
  }
}

#endif