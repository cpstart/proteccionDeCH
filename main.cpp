/* PROGRAMA PARA LIBERAR LOS CHEQUES DESDE UNA LISTA CSV*/

#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<algorithm>
#include<string>
#include<iterator>
#include<ctime>
#include<iomanip>

template<char C>
class myString : public std::string{
    std::istream& operator>>(std::istream& in){
        std::getline(in,*this,C);
        return in;
    }
};

struct cheque{
    cheque(std::string linea){
        std::istringstream in(linea);
        std::string token;
        std::getline(in,token,'|');
        folio = std::stoi(token);
        std::getline(in,token,'|');
        importe = std::stod(token);
    }
    int folio;
    double importe;
};

std::ostream& operator<<(std::ostream& out,cheque const& CH){
    out << CH.folio << "|" << CH.importe;
    return out;
}

/* funcion para cargar la lista de cheques en un container*/
 template<typename T, typename container = std::vector<T>>
container& loadListaCheques(const char* filename){
    typename container cont;
    std::ifstream File(filename);
    std::vector<std::string> lineas;
    std::copy(std::istream_iterator<std::string>(File),std::istream_iterator<std::string>{},std::back_inserter(lineas));
    for (size_t i = 0; i < lineas.size(); i++){
        cont.emplace_back(cheque(lineas.at(i)));
    }
    return cont;
}

int main(){
    const char* lista = "data\\listaCheques.csv";
    auto cheques = loadListaCheques<cheque>(lista);


    //Imprimimos el header
    time_t ahora = time(NULL);
		
	//Nombre del Archivo
	struct tm * Hoy = localtime(&ahora);
	std::ostringstream briefdate,nombreArchivo;
	briefdate << std::setw(2) << std::setfill('0') << Hoy->tm_year - 100 << std::setw(2) << Hoy->tm_mon + 1 << std::setw(2) << Hoy->tm_mday;
	int consecutivo = 1;
	std::string Archivo;
	nombreArchivo << "archivosParaTransmitir\\00110149" << briefdate.str() << std::setw(3) << std::setfill('0') << consecutivo << ".CHP";
	Archivo = nombreArchivo.str();
	while(FILE *file = fopen(Archivo.c_str(), "r")){
		consecutivo+=1;
		nombreArchivo.str("");
		nombreArchivo << "C:\\Pagos\\00110149" << briefdate.str() << std::setw(3) << std::setfill('0') << consecutivo << ".CHP";
		Archivo = nombreArchivo.str();
		fclose(file);
	}

	std::ofstream Out(Archivo.c_str());
    
    double total = 0;
    for(const auto CH : cheques){
        total += CH.importe;
    }


    if(!cheques.empty()){
		//Imprimimos el Header
		Out << "H" 																							//1.header (H)
		<< std::setw(5) << std::setfill('0') << 1 << 20															//2.NoCtas (1)
		<< Hoy->tm_year - 100 << std::setw(2) << Hoy->tm_mon + 1 
		<< std::setw(2) << Hoy->tm_mday                 													//3.fecha
		<< std::setw(3) << consecutivo 																		//4.Consecutivo (1)
		<< std::setw(5) << cheques.size() 																	//5.NoCheques
		<< std::setw(18) << std::fixed << std::setprecision(0) << total * 100 								//6.importe
		//<< '\n' << total  << '\n' 
		<< std::setw(5) << 0																				//7.NoCheques des(0)
		<< std::setw(18) << 0 																				//8.importe des(0)
		<< std::setw(5) << cheques.size() 																	//9.NoCheques total
		<< std::setw(18) << std::fixed << std::setprecision(0) << total * 100 								//10.importe Gral
		<< std::setw(106) << 0 																				//11.usoFuturo		
		<< std::endl;
		
		//Imprimimos los items
		std::vector<cheque>::iterator it; int Item_consecutivo{0};
		for (it = cheques.begin(); it != cheques.end(); ++it) {
			Out << "D"																						//1.Tipo D
			<< std::setw(2) << "11" 																		//2.TipoServ (11)
			<< std::setw(5) << ++Item_consecutivo															//3.Consecutivo por Item
			<< std::setw(2) << "60"																			//4.Operacion (60)
			<< std::setw(10) << "1094399085"																//5.NoCuenta
			<< std::setw(7) << it->folio << 20																	//6.NoCheque
			<< Hoy->tm_year - 100 << std::setw(2) << Hoy->tm_mon + 1
			<< std::setw(2) << Hoy->tm_mday                  												//7.fecha prot
			<< std::setw(8) << "20991231"																	//8.FechaFinProt
			<< std::setw(13) << std::fixed << std::setprecision(0) << it->importe * 100						//9.importe
			<< std::setw(4) << 0																			//10.sucursal
			<< "N"																							//11.ValidarBen
			<< std::setw(50) << std::setfill(' ') << " "													//12. NombreBen
			<< std::setw(3) << "MXP"																		//13.Divisa
			<< std::setw(78) << std::setfill('0') << 0 														//14.usoFuturo	
			<< std::endl;
		}
	}

}