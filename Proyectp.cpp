#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

struct Alumno {
	string nombre;
	string matricula;
	string grupo;
	string maestro;
	float calific[3];
	float promedio;
};

void cargar(vector<Alumno>& lista) {
	ifstream entrada("alumno.txt");
	if (!entrada.is_open()) {
		cout << "No se encontro alumno.txt, se inicia con datos vacios\n";
		return;
	}

	string linea;
	while (getline(entrada, linea)) {
		stringstream ss(linea);
		Alumno a;
		string c1, c2, c3;
		getline(ss, a.nombre, '|');
		getline(ss, a.matricula, '|');
		getline(ss, a.grupo, '|');
		getline(ss, a.maestro, '|');
		getline(ss, c1, '|');
		getline(ss, c2, '|');
		getline(ss, c3, '|');

		if (c3.empty()) continue;   

		try {
			a.calific[0] = stof(c1);
			a.calific[1] = stof(c2);
			a.calific[2] = stof(c3);
		}
		catch (...) {
			continue;
		}
		a.promedio = (a.calific[0] + a.calific[1] + a.calific[2]) / 3;
		lista.push_back(a);
	}
	entrada.close();
	cout << "Se cargaron " << lista.size() << " alumnos\n";
}

void guardar(const vector<Alumno>& lista) {
	ofstream txt("alumno.txt");
	if (!txt.is_open()) {
		cout << "Error al crear archivo de texto\n";
		return;
	}
	for (const Alumno& a : lista) {
		txt << a.nombre << "|" << a.matricula << "|"
			<< a.grupo << "|" << a.maestro << "|"
			<< a.calific[0] << "|" << a.calific[1] << "|" << a.calific[2] << endl;
	}
	txt.close();

	ofstream bin("alumnos.dat", ios::binary);
	if (!bin.is_open()) {
		cout << "Error al crear archivo binario\n";
		return;
	}
	for (const Alumno& a : lista) {
		unsigned int tam = a.nombre.size();
		bin.write((char*)&tam, sizeof(tam));
		bin.write(a.nombre.c_str(), tam);

		tam = a.matricula.size();
		bin.write((char*)&tam, sizeof(tam));
		bin.write(a.matricula.c_str(), tam);

		tam = a.grupo.size();
		bin.write((char*)&tam, sizeof(tam));
		bin.write(a.grupo.c_str(), tam);

		tam = a.maestro.size();
		bin.write((char*)&tam, sizeof(tam));
		bin.write(a.maestro.c_str(), tam);

		bin.write((char*)a.calific, sizeof(a.calific));
	}
	bin.close();
	cout << "Datos guardados en alumno.txt y alumnos.dat\n";
}

void agregar(vector<Alumno>& lista) {
	Alumno a;
	cout << "Nombre: ";
	getline(cin, a.nombre);
	cout << "Matricula: ";
	getline(cin, a.matricula);
	cout << "Grupo (numero o letra): ";
	getline(cin, a.grupo);
	cout << "Maestro: ";
	getline(cin, a.maestro);
	for (int j = 0; j < 3; j++) {
		cout << "Calificacion " << j + 1 << ": ";
		cin >> a.calific[j];
	}
	cin.ignore(1000, '\n');
	a.promedio = (a.calific[0] + a.calific[1] + a.calific[2]) / 3;
	lista.push_back(a);
	cout << "Alumno agregado\n";
}

void mostrar(const vector<Alumno>& lista) {
	if (lista.empty()) {
		cout << "No hay alumnos registrados\n";
		return;
	}
	for (size_t i = 0; i < lista.size(); i++) {
		cout << i + 1 << ". " << lista[i].nombre
			<< " | Matricula: " << lista[i].matricula
			<< " | Grupo: " << lista[i].grupo
			<< " | Maestro: " << lista[i].maestro
			<< " | Calificaciones: " << lista[i].calific[0] << ", "
			<< lista[i].calific[1] << ", " << lista[i].calific[2]
			<< " | Promedio: " << lista[i].promedio << endl;
	}
}

void reporte(const vector<Alumno>& lista) {
	vector<Alumno> orden = lista;
	sort(orden.begin(), orden.end(), [](const Alumno& x, const Alumno& y) {
		return x.promedio > y.promedio;
		});

	ofstream rep("reporte.txt");
	if (!rep.is_open()) {
		cout << "Error: no se pudo crear reporte.txt\n";
		return;
	}
	rep << "REPORTE: ALUMNOS POR PROMEDIO (de mayor a menor)\n";
	rep << "------------------------------------------------\n";
	for (size_t i = 0; i < orden.size(); i++) {
		rep << i + 1 << ". " << orden[i].nombre
			<< " | Matricula: " << orden[i].matricula
			<< " | Grupo: " << orden[i].grupo
			<< " | Maestro: " << orden[i].maestro
			<< " | Promedio: " << orden[i].promedio << endl;
	}
	rep.close();
	cout << "Reporte generado en reporte.txt\n";
}

int main() {
	vector<Alumno> lista;
	cargar(lista);

	int opcion;
	do {
		cout << "\n--- MENU ---\n";
		cout << "1. Agregar alumno\n";
		cout << "2. Mostrar alumnos\n";
		cout << "3. Guardar\n";
		cout << "4. Generar reporte\n";
		cout << "5. Salir\n";
		cout << "Opcion: ";
		cin >> opcion;
		cin.ignore(1000, '\n');

		switch (opcion) {
		case 1: agregar(lista); break;
		case 2: mostrar(lista); break;
		case 3: guardar(lista); break;
		case 4: reporte(lista); break;
		case 5: guardar(lista); cout << "Hasta luego\n"; break;
		default: cout << "Opcion no valida\n";
		}
	} while (opcion != 5);

	return 0;
}