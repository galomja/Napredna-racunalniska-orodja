#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <omp.h>

/*Definicija funkcije za preverjanje sosednosti: 
0...levo
1...spodaj
2...desno
3...zgoraj
-1...nista soseda*/
int soseska(int X_trenutno, int Y_trenutno, int X_sosednje, int Y_sosednje, double toleranca = 0.00005) {
	int pozicija;
	
	if (X_trenutno - X_sosednje < toleranca && X_trenutno - X_sosednje > -1 * toleranca) {//preverimo ali smo zgoraj/spodaj
		if (Y_trenutno - Y_sosednje > 0){
			//smo spodaj
			pozicija = 1;
		}
		else if (Y_trenutno - Y_sosednje < 0) {
			//smo zgoraj
			pozicija = 3;
		}
	}
	else if (Y_trenutno - Y_sosednje < toleranca && Y_trenutno - Y_sosednje > -1 * toleranca) {//preverimo ali smo levo/desno
		if (X_trenutno - X_sosednje > 0) {
		//smo levo
			pozicija = 0;
		}
		else if (X_trenutno - X_sosednje < 0) {
			pozicija = 2;
		}
	}
	else {
		pozicija = -1;
	}
	return pozicija;
}

int main() {
//pripravimo potrebne spremenljivke za branje datoteke:
	std::vector<double> X;
	std::vector<double> Y;
	std::vector<std::vector<int>> celice;
	std::vector<std::vector<int>> vozlisca_robnih_pogojev;
	std::vector<int> tipi_robnih_pogojev;
	std::vector<int> vrednosti_robnih_pogojev;
	std::vector<int> vrednosti_prestopa_toplote;
	int N_robnih;
//odpremo datoteko:
	std::ifstream datoteka_mreza("./primer2mreza.txt");
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
			X.push_back(x);
			Y.push_back(y);	
		}
		X.erase(X.begin());
		Y.erase(Y.begin());
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
			}
			//indeks vozlisc shranimo v vrednosti node0 do node3 in jih shranimo v vektor cell
			std::istringstream iss(s);
			iss >> n >> node0 >> node1 >> node2 >> node3;
			cell.push_back(node0);
			cell.push_back(node1);
			cell.push_back(node2);
			cell.push_back(node3);
			//vektor cell shranimo v vektor celice
			celice.push_back(cell);
			cell.clear();
		}
		celice.erase(celice.begin());
		size = celice.size();
		std::cout << "Velikost vektorja celic: " << size << std::endl;
		std::cout << "Zadnja celica je: " << celice[N_celic-1][0] << ", " << celice[N_celic - 1][1] << ", " << celice[N_celic - 1][2] << ", " << celice[N_celic - 1][3] << std::endl;
		int N_pogoji;
		datoteka_mreza >> vmesna_vrstica >> vmesna_vrstica >> N_pogoji;
		std::cout << " " << std::endl;
		std::cout << "Stevilo robnih pogojev: "<< N_pogoji << std::endl;
		std::vector<std::vector<int>> vozlisca_robnih_pogojev;
		/*
		LEGENDA TIPA ROBNIH POGOJEV:
		0...Temperatura
		1...Toplotni Tok
		2...Prestop Toplote
		*/
		std::vector<int> tipi_robnih_pogojev;
		std::vector<int> vrednosti_robnih_pogojev;
		/*
		LEGENDA:
		-1 ta tip 0,
		-1 za tip 1,
		koeficient prestopa za tip 2
		*/
		std::vector<double> vrednosti_prestopa_toplote;
		//vektor za ID-je točk v robnih pogojih:
		std::vector<int> vozlisca_v_robnem_pogoju;
		std::string balast1;
		std::string balast2;
		std::string tip_pogoja;
		int vrednost;
		for (int n = 0; n < N_pogoji; n++) {
			std::cout << std::endl;
			vozlisca_v_robnem_pogoju.clear();
			datoteka_mreza >> balast1 >> balast2>>tip_pogoja;
			std::cout << balast1 << balast2<<tip_pogoja<<std::endl;
			if (tip_pogoja == "temperatura") {
				tipi_robnih_pogojev.push_back(0);
				datoteka_mreza >> balast1 >> vrednost;
				std::cout << "Temperatura je " << vrednost << std::endl;
				vrednosti_robnih_pogojev.push_back(vrednost);
				vrednosti_prestopa_toplote.push_back(-1);
			}
			else if (tip_pogoja == "toplotni") {
				tipi_robnih_pogojev.push_back(1);
				datoteka_mreza >> balast1 >> balast2 >> balast1 >> vrednost;
				std::cout << "Toplotni tok je " << vrednost << std::endl;
				vrednosti_robnih_pogojev.push_back(vrednost);
				vrednosti_prestopa_toplote.push_back(-1);
			}
			else if (tip_pogoja == "prestop") {
				tipi_robnih_pogojev.push_back(2);
				datoteka_mreza >> balast1 >> vrednost;
				vrednosti_robnih_pogojev.push_back(vrednost);
				std::cout << "Temperatura prestopa je " << vrednost << std::endl;
				datoteka_mreza >> balast1 >> balast2 >> vrednost;
				vrednosti_prestopa_toplote.push_back(vrednost);
				std::cout << "Koeficient prestopa je " << vrednost << std::endl;
			}
			datoteka_mreza >> N_robnih;
			std::cout << "Stevilo tock v robnm pogoju je: " << N_robnih << std::endl;
			for (int i = 0; i < N_robnih; i++) {
				datoteka_mreza >> node0;
				vozlisca_v_robnem_pogoju.push_back(node0);
			}
			std::cout << "Zadnje vozlisce: " << vozlisca_v_robnem_pogoju.back() << std::endl;
			vozlisca_robnih_pogojev.push_back(vozlisca_v_robnem_pogoju);
		}
		std::cout << std::endl;

		/*
		Datoeka je zdaj prebrana, gremo v reševanje problema.
		Zacnemo z iskanje sosedov vozlisc:
		*/
		std::cout << "Iscem sosede vozlisc..." << std::endl;
		int deltaX = 1;
		int deltaY = 1;
		int k = 1;
		std::vector<std::vector<int>> sosednja_vozlisca;
		std::vector<int> node_i_neighbours;
		//zacnemo z enim vozliscem; z for zanko to napravimo za vsa voslisca:
		for (int node_id = 0; node_id < N_points; node_id++) {
			//najprej nastavimo kot da nima sosedov (povsod so -1):
			node_i_neighbours = { -1, -1, -1, -1 };
			//nato gremo pogledat v vsako celico ali je nase trenudtno vozlisce (node_id) v njej:
			for (int nd = 0; nd < N_celic; nd++) {
				std::vector<int> trenutna_celica = celice[nd];
				node0 = trenutna_celica[0];
				node1 = trenutna_celica[1];
				node2 = trenutna_celica[2];
				node3 = trenutna_celica[3];
				//ce je trenutno vozlisce (node_id) v njej preverimo, ali ima sosede:
				if (node_id == node0 || node_id == node1 || node_id == node2 || node_id == node3) {
					for (int i = 0; i <= 3; i++) {
						int sosednje_vozlisce = trenutna_celica[i];
						if (node_id != sosednje_vozlisce) {
							int X_trenutno = X[node_id];
							int Y_trenutno = Y[node_id];
							int X_sosednje = X[sosednje_vozlisce];
							int Y_sosednje = Y[sosednje_vozlisce];
							/*uporabimo funkcijo soseske(definirana na zacetku datoteke), da preverimo ali je vozlisce zgoraj,
							spodaj ali levo, ali desno*/
							int pozicija = soseska(X_trenutno,Y_trenutno,X_sosednje,Y_sosednje);
							//ce je pozicija -1 (vozlisci nista sosednji) ta korak spustimo
							if (pozicija != -1) {
								node_i_neighbours[pozicija] = sosednje_vozlisce;
							}
						}
					}
				}
			}
			sosednja_vozlisca.push_back(node_i_neighbours);
		}
		std::cout << "Sosedi so poiskani" << std::endl;
		
		//sedaj zgradimo matriko A in vektor neznank b
		std::vector<std::vector<int>> A(N_points, std::vector<int>(N_points, 0));
		std::vector<int> b(N_points, 0);
		
		//za vsako voslisce zapisemo enacbo:
		std::cout << "Pisem enacbe..." << std::endl;
		for(int node_id = 0; node_id < N_points; node_id++) {
			int tip_robnega_pogoja = 0;
			int vrednost = 0;
			int	vrednost_prestopa = 0;
			std::vector<int> sosedi = sosednja_vozlisca[node_id];
			int levi_sosed = sosedi[0];
			int spodnji_sosed = sosedi[1];
			int desni_sosed = sosedi[2];
			int zgornji_sosed = sosedi[3];
			//ce ni nobeden od sosedov enak -1, pomeni da ima tocka vse 4 sosede, torej ne lezi na robu
			if (levi_sosed != -1 && spodnji_sosed != -1 && desni_sosed != -1 && zgornji_sosed != -1) {
				A[node_id][levi_sosed] = 1;
				A[node_id][spodnji_sosed] = 1;
				A[node_id][desni_sosed] = 1;
				A[node_id][zgornji_sosed] = 1;
				A[node_id][node_id] = -4;
			}
			else{
				for (int robni_pogoj_ID = 0; robni_pogoj_ID < N_pogoji; robni_pogoj_ID++) {
					std::vector<int> vozlisca_v_trenutnem_rp = vozlisca_robnih_pogojev[robni_pogoj_ID];
					for (int id_vozlisce_rp = 0; id_vozlisce_rp < vozlisca_v_trenutnem_rp.size(); id_vozlisce_rp++) {
						int vozlisce_v_trenutnem_rp = vozlisca_v_trenutnem_rp[id_vozlisce_rp];
						if (node_id == vozlisce_v_trenutnem_rp) {
							tip_robnega_pogoja = tipi_robnih_pogojev[robni_pogoj_ID];
							vrednost = vrednosti_robnih_pogojev[robni_pogoj_ID];
							vrednost_prestopa = vrednosti_prestopa_toplote[robni_pogoj_ID];
						}
					}
				}
				//ce je tip 0 imamo v robnem pogoju doloceno konstantno temperaturo,enacba je zato preprosta:
				if (tip_robnega_pogoja == 0) {
					A[node_id][node_id] = 1;
					b[node_id] = vrednost;
				}
				else if(tip_robnega_pogoja == 1) {
					/*Imamo dolocen toplotni tok;
					Izracunamo koliko sosedov ima tocka :*/
					int stevilo_sosedov = 0;
					for (int st = 0; st < 4; st++) {
						if (sosedi[st] != -1) {
							stevilo_sosedov += 1;
						}
					}
					if(stevilo_sosedov == 3){
						if (levi_sosed == -1) {
							//pomeni, da je vozlisce na levem robu:
							A[node_id][node_id] += -4;
							A[node_id][desni_sosed] += 2;
							A[node_id][spodnji_sosed] += 1;
							A[node_id][zgornji_sosed] += 1;
							b[node_id] = -2 * (vrednost * deltaX / k);
						}
						if (desni_sosed == -1) {
							//pomeni, da je vozlisce na desnem robu:
							A[node_id][node_id] += -4;
							A[node_id][levi_sosed] += 2;
							A[node_id][spodnji_sosed] += 1;
							A[node_id][zgornji_sosed] += 1;
							b[node_id] = -2 * (vrednost * deltaX / k);
						}
						if (spodnji_sosed == -1) {
							//pomeni, da je vozlisce na spodnjem robu:
							A[node_id][node_id] += -4;
							A[node_id][zgornji_sosed] += 2;
							A[node_id][desni_sosed] += 1;
							A[node_id][levi_sosed] += 1;
							b[node_id] = -2 * (vrednost * deltaX / k);
						}
						if (zgornji_sosed == -1) {
							//pomeni, da je vozlisce na spodnjem robu:
							A[node_id][node_id] += -4;
							A[node_id][spodnji_sosed] += 2;
							A[node_id][desni_sosed] += 1;
							A[node_id][levi_sosed] += 1;
							b[node_id] = -2 * (vrednost * deltaX / k);
						}
					}
				}
				else if (tip_robnega_pogoja == 2) {
					/*Imamo konvektivni robni pogoj;
					Izracunamo koliko sosedov ima tocka :*/
					int stevilo_sosedov = 0;
					for (int st = 0; st < 4; st++) {
						if (sosedi[st] != -1) {
							stevilo_sosedov += 1;
						}
					}
					if (stevilo_sosedov == 3) {
						if (levi_sosed == -1) {
							//pomeni, da je vozlisce na levem robu:
							A[node_id][node_id] += -2*(vrednost_prestopa*deltaX/k+2);
							A[node_id][desni_sosed] += 2;
							A[node_id][spodnji_sosed] += 1;
							A[node_id][zgornji_sosed] += 1;
							b[node_id] = -2 * (vrednost_prestopa * deltaX* vrednost / k);
						}
						if (desni_sosed == -1) {
							//pomeni, da je vozlisce na desnem robu:
							A[node_id][node_id] += -2 * (vrednost_prestopa * deltaX / k + 2);
							A[node_id][levi_sosed] += 2;
							A[node_id][spodnji_sosed] += 1;
							A[node_id][zgornji_sosed] += 1;
							b[node_id] = -2 * (vrednost_prestopa * deltaX * vrednost / k);
						}
						if (spodnji_sosed == -1) {
							//pomeni, da je vozlisce na spodnjem robu:
							A[node_id][node_id] += -2 * (vrednost_prestopa * deltaX / k + 2);
							A[node_id][zgornji_sosed] += 2;
							A[node_id][desni_sosed] += 1;
							A[node_id][levi_sosed] += 1;
							b[node_id] = -2 * (vrednost_prestopa * deltaX * vrednost / k);
						}
						if (zgornji_sosed == -1) {
							//pomeni, da je vozlisce na spodnjem robu:
							A[node_id][node_id] += -2 * (vrednost_prestopa * deltaX / k + 2);
							A[node_id][spodnji_sosed] += 2;
							A[node_id][desni_sosed] += 1;
							A[node_id][levi_sosed] += 1;
							b[node_id] = -2 * (vrednost_prestopa * deltaX * vrednost / k);
						}
					}
				}
			}
		}
		std::cout << "Enacbe spisane" << std::endl;
		std::cout << "Velikost matrike A: " << A.size() << "x" << A[0].size() << std::endl;
		std::cout << "Velikost vektorja b: " << b.size() << std::endl;
		

		
		/*sedaj moramo resiti sistem enacb A.T = b
		Dolocimo zacetni priblizek: T vseh tock naj bo 100 oC*/
		std::vector<double> T(N_points, 100.0); //pripravimo prazen vektor
		std::cout << "Resujem sistem enacb..." << std::endl;

		for (int iitt = 1; iitt < 100; iitt++) {
			for (int jj = 0; jj < N_points; jj++) {
				double d = b[jj];
				for (int ii = 0; ii < N_points; ii++) {
					if (jj != ii) {
						d = d - A[jj][ii] * T[ii];
					}
					T[jj] = d / A[jj][jj];
				}
			}
		}
		std::cout << "Sistem enacb resen" << std::endl;
		std::cout << "" << std::endl;
		std::cout << "Vrednosti T" << std::endl;
		for (int i = 0; i < 10; ++i) {
			std::cout << "T[" << i << "] = " << T[i] << std::endl;
		}
		//zapis v VTK datoteko:
		// Specify the output VTK file name
		const std::string vtkFileName = "resitev2mreza.vtk";

		// Open the VTK file for writing
		std::ofstream vtkFile(vtkFileName);

		if (!vtkFile.is_open()) {
			std::cerr << "Error: Unable to open the VTK file for writing." << std::endl;
			return 1;  // Return an error code
		}

		// Write the VTK file header
		vtkFile << "# vtk DataFile Version 3.0\n";
		vtkFile << "Mreza1\n";
		vtkFile << "ASCII\n";
		vtkFile << "DATASET UNSTRUCTURED_GRID\n";

		// Write the POINTS section with coordinates
		vtkFile << "POINTS " << N_points << " float\n";
		for (int i = 0; i < N_points; ++i) {
			vtkFile << X[i] << " " << Y[i] << " " << 0 << "\n";
		}
		vtkFile << "\n";
		// Write the CELLS section with coordinates
		vtkFile << "CELLS " << N_celic << " " << N_celic * 5 << "\n";
		for (int i = 0; i < N_celic; ++i) {
			vtkFile << celice[i].size() << " " << celice[i][0] << " " << celice[i][1] << " " << celice[i][2] << " " << celice[i][3] << "\n";
		}
		vtkFile << "\n";
		// Write the CELLS section with coordinates
		vtkFile << "CELL_TYPES " << N_celic << "\n";
		for (int i = 0; i < N_celic; ++i) {
			vtkFile << 9 << "\n";
		}
		vtkFile << "\n";
		vtkFile << "POINT_DATA" << " " << N_points << "\n";
		vtkFile << "SCALARS Temperature float 1" << "\n";
		vtkFile << "LOOKUP_TABLE default" << "\n";
		// Write the RESITVE section with coordinates
		for (int i = 0; i < N_points; ++i) {
			vtkFile << T[i] << "\n";
		}

		// Close the file
		vtkFile.close();
		std::cout << "Koncano" << std::endl;
	}
	else {
		std::cout << "Unable to open file" << std::endl;
	}
	
	return 0;
}

