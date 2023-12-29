#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

int main() {
//pripravimo potrebne spremenljivke za branje datoteke:
	std::vector<double> X;
	std::vector<double> Y;
	std::vector<std::vector<int>> celice;
	std::vector<std::vector<int>> vozlisca_robnih_pogojev;
	std::vector<int> tipi_robnih_pogojev;
	std::vector<int> vrednosti_robnih_pogojev;
	std::vector<int> vrednosti_prestopa_toplote;
//odpremo datoteko:
	std::ifstream datoteka_mreza("C:\\Users\\Uporabnik\\OneDrive - Univerza v Ljubljani\\Gal faks\\3.letnik\\1.semester\\Napredna racunalniska orodja\\Projektna naloga\\primer2mreza.txt");
	if (datoteka_mreza.is_open()) {
		//preberemo stevilo tock:
		std::string prva_beseda;
		int N_points;
		datoteka_mreza >> prva_beseda >> N_points;
		std::cout << "Stevilo tock: " << N_points << std::endl;
		//pripravimo spremenljivke za branje koordinat:
		double n = 0;
		double x = 0;
		double y = 0;
		char toReplace1 = ';';
		char toReplace2 = ',';
		char replaceWith = ' ';
		std::string s;
	for (int i = 0; i < N_points+1; i++) {
			std::getline(datoteka_mreza, s);
			//nadomestimo znaka ";" in "," s presledkom " "
			for (size_t i = 0; i < s.length(); i++) {
				if (s[i] == toReplace1) {
					s[i] = replaceWith;
				}
				if (s[i] == toReplace2) {
					s[i] = replaceWith;
				}
			}
			// koordinati zapišemo v X in Y
			std::istringstream iss(s);
			iss >> n >> x >> y;
			if (i) {
				X.push_back(x);
				Y.push_back(y);	
			}
			
		}
		int size = X.size();
		std::cout <<"Velikost vektorjev X in Y: " << size << ", " <<"Zadnji koordinati: "<< X[N_points - 1] << ", " << Y[N_points - 1] << std::endl;
		std::string vmesna_vrstica;
		//preberemo stevilo celic:
		int N_celic;
		datoteka_mreza >> vmesna_vrstica >> N_celic;
		std::cout << "Stevilo celic: " << N_celic << std::endl;
		//preberemo katere tocke so v posamezni celici
		int node0 = 0;
		int node1 = 0;
		int node2 = 0;
		int node3 = 0;
		std::vector<int> cell;
		for (int i = 0; i < N_celic + 1; i++) {
			std::getline(datoteka_mreza, s);
			//nadomestimo znaka ";" in "," s presledkom " "
			for (size_t i = 0; i < s.length(); i++) {
				if (s[i] == toReplace1) {
					s[i] = replaceWith;
				}
				if (s[i] == toReplace2) {
					s[i] = replaceWith;
				}
				//indeks vozlisc shranimo v vrednosti node0 do node3 in jih shranimo v vektor cell
				std::istringstream iss(s);
				iss >> node0 >> node1 >> node2 >> node3;
				cell.push_back(node0);
				cell.push_back(node1);
				cell.push_back(node2);
				cell.push_back(node3);
				//vektor cell shranimo v vektor celice
				celice.push_back(cell);
			}
		}
		//std::cout << "Zadnja celica je" << celice[1][2];//[0] << celice[N_celic - 1][1] << celice[N_celic - 1][2] << celice[N_celic - 1][3] << std::endl;
	}
	else {
		std::cout << "Unable to open file." << std::endl;
		return 1; 
	}
	return 0;
}