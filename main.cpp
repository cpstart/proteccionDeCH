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
    friend std::istream& operator>>(std::istream& in, myString<C>& item){
        std::getline(in,item,C);
        return in;
    }


    /* APARENTEMENTE LOS OPERADORES << Y >> DEBEN SER DECLARADOS COMO FUNCIONES GLOBALES... https://www.geeksforgeeks.org/overloading-stream-insertion-operators-c/ 
    En C ++, el operador de inserción de flujo "<<" se usa para la salida y el operador de extracción ">>" se usa para la entrada.

Debemos saber lo siguiente antes de comenzar a sobrecargar estos operadores.
1) cout es un objeto de la clase ostream y cin es un objeto de la clase istream
2) Estos operadores deben sobrecargarse como una función global. Y si queremos permitirles acceder a los datos privados de los miembros de la clase, 
debemos hacerlos friends.

¿Por qué estos operadores deben sobrecargarse como globales?
En la sobrecarga del operador, si un operador se sobrecarga como miembro, debe ser un miembro del objeto en el lado izquierdo del operador. Por ejemplo, 
considere la declaración "ob1 + ob2" (deje que ob1 y ob2 sean objetos de dos clases diferentes). Para hacer que esta declaración se compile, debemos sobrecargar "+" 
en la clase de "ob1" o hacer que "+" sea una función global.
Los operadores ‘<< 'y' >> 'se llaman como' cout << ob1 'y' cin >> ob1 '. Entonces, si queremos convertirlos en un método miembro, entonces deben ser miembros de 
las clases ostream e istream, lo cual no es una buena opción la mayor parte del tiempo. Por lo tanto, estos operadores están sobrecargados como funciones globales 
con dos parámetros, cout y objeto de clase definida por el usuario.

cin >> obj, significa operator<<(cin,obj), el lado izquierdo es cin, y sobrecargar >>(obj1) no vale la pena... ni es tan simple ;)

    // ESTO NO FUNCIONA    
    // std::istream& operator>>(std::istream& in){
    //     std::getline(in,*this,C);
    //     return in;
    // }
*/
};

// template<char C>
// std::istream& operator>>(std::istream& in, myString<C>& token){
//     std::getline(in,token,C);
//     return in;
// }

struct cheque{
    cheque(std::string linea){
        std::istringstream in(linea);
        std::vector<std::string> item;
        std::copy(std::istream_iterator<myString<'|'>>(in),std::istream_iterator<myString<'|'>>(), std::back_inserter(item));
        folio = std::stoi(item.at(0));
        importe = std::stod(item.at(1));
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
container loadListaCheques(const char* filename){
    typename container cont;
    std::ifstream File(filename);
    std::vector<std::string> lineas;
    std::copy(std::istream_iterator<std::string>(File),std::istream_iterator<std::string>{},std::back_inserter(lineas));
    for (size_t i = 0; i < lineas.size(); i++){
        cont.emplace_back(T{lineas.at(i)});
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
		nombreArchivo << "archivosParaTransmitir\\00110149" << briefdate.str() << std::setw(3) << std::setfill('0') << consecutivo << ".CHP";
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