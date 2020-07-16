#include <iostream>
#include <vector>
#include <iomanip>
#include <ctime>
#include <string>
#include <sstream>
#include <fstream>
#include "mysql++.h"
#include "ssqls.h"

sql_create_19(_CH_072610010943990850,1,19,
		mysqlpp::Null<mysqlpp::sql_int>,		idch,
		mysqlpp::Null<mysqlpp::sql_date>,		fecha,
		mysqlpp::Null<mysqlpp::sql_varchar>,	rfc,
		mysqlpp::Null<mysqlpp::sql_varchar>,	beneficiario,
		mysqlpp::sql_int,						folio,
		mysqlpp::sql_decimal,					importe,
		mysqlpp::Null<mysqlpp::sql_varchar>,	concepto,
		mysqlpp::sql_int,						id_area,
		mysqlpp::sql_int,						estatus,
		mysqlpp::Null<mysqlpp::sql_date>,		envio_firma,
		mysqlpp::Null<mysqlpp::sql_date>,		retorno_firma,
		mysqlpp::Null<mysqlpp::sql_date>,		entrega,
		mysqlpp::Null<mysqlpp::sql_date>,		cobro,
		mysqlpp::Null<mysqlpp::sql_int>,		soporte,
		mysqlpp::Null<mysqlpp::sql_varchar>,	observaciones,
		mysqlpp::sql_int,						id_clasificacion,
		mysqlpp::sql_int,						id_resguardante,
		mysqlpp::sql_int,						id_confidencialidad,
		mysqlpp::sql_int,						id_archivo
		);

int main(int argc, char* argv[]) {
	try {
		mysqlpp::Connection conn(false);
		//bool conectado = conn.connect("CUENTAS", "172.16.36.133:3306", "cpo", "cg070310");
		bool conectado = conn.connect("CUENTAS", "172.16.37.91:3306", "cpo", "cg070310");
		if (conectado) {
			std::cout << "Conectado a la banca... \n";
		}
		else { std::cout << "Conexion Fallida"; return 1; }
		mysqlpp::Query query = conn.query("SELECT CUENTAS._CH_072610010943990850.idch,\
		CUENTAS._CH_072610010943990850.fecha,\
		CUENTAS._CH_072610010943990850.rfc,\
		CUENTAS._CH_072610010943990850.beneficiario,\
		CUENTAS._CH_072610010943990850.folio,\
		CUENTAS._CH_072610010943990850.importe,\
		CUENTAS._CH_072610010943990850.concepto,\
		CUENTAS._CH_072610010943990850.id_area,\
		CUENTAS._CH_072610010943990850.estatus,\
		CUENTAS._CH_072610010943990850.envio_firma,\
		CUENTAS._CH_072610010943990850.retorno_firma,\
		CUENTAS._CH_072610010943990850.entrega,\
		CUENTAS._CH_072610010943990850.cobro,\
		BIN(CUENTAS._CH_072610010943990850.soporte) AS soporte,\
		CUENTAS._CH_072610010943990850.observaciones,\
		CUENTAS._CH_072610010943990850.id_clasificacion,\
		CUENTAS._CH_072610010943990850.id_pago,\
		CUENTAS._CH_072610010943990850.id_resguardante,\
		CUENTAS._CH_072610010943990850.id_confidencialidad,\
		CUENTAS._CH_072610010943990850.id_archivo,\
		CUENTAS._CH_072610010943990850.id_sol_pago \
		FROM CUENTAS._CH_072610010943990850 INNER JOIN DRF2020.solicitudesDePagos ON CUENTAS._CH_072610010943990850.id_sol_pago = DRF2020.solicitudesDePagos.id_sol_pago \
		WHERE DRF2020.solicitudesDePagos.idEstatusPago = 0 \
		ORDER BY CUENTAS._CH_072610010943990850.folio");
		std::vector<_CH_072610010943990850> res;
		query.storein(res);
		mysqlpp::sql_decimal total = 0;
		for(const auto CH : res){
			total += CH.importe;
		}
		//Imprimimos el header
		time_t ahora = time(NULL);
		mysqlpp::sql_date fecha(ahora);
		
	//Nombre del Archivo
	struct tm * Hoy = localtime(&ahora);
	std::ostringstream briefdate,nombreArchivo;
	briefdate << std::setw(2) << std::setfill('0') << Hoy->tm_year - 100 << std::setw(2) << Hoy->tm_mon + 1 << std::setw(2) << Hoy->tm_mday;
	int consecutivo = 1;
	std::string Archivo;
	nombreArchivo << "C:\\Pagos\\00110149" << briefdate.str() << std::setw(3) << std::setfill('0') << consecutivo << ".CHP";
	Archivo = nombreArchivo.str();
	while(FILE *file = fopen(Archivo.c_str(), "r")){
		consecutivo+=1;
		nombreArchivo.str("");
		nombreArchivo << "C:\\Pagos\\00110149" << briefdate.str() << std::setw(3) << std::setfill('0') << consecutivo << ".CHP";
		Archivo = nombreArchivo.str();
		fclose(file);
	}
	std::ofstream Out(Archivo.c_str());
	
	if(!res.empty()){
		//Imprimimos el Header
		Out << "H" 																							//1.header (H)
		<< std::setw(5) << std::setfill('0') << 1 															//2.NoCtas (1)
		<< fecha.year() << std::setw(2) << static_cast<int>(fecha.month()) 
		<< std::setw(2) << static_cast<int>(fecha.day()) 													//3.fecha
		<< std::setw(3) << consecutivo 																		//4.Consecutivo (1)
		<< std::setw(5) << res.size() 																		//5.NoCheques
		<< std::setw(18) << std::fixed << std::setprecision(0) << total * 100 								//6.importe
		//<< '\n' << total  << '\n' 
		<< std::setw(5) << 0																				//7.NoCheques des(0)
		<< std::setw(18) << 0 																				//8.importe des(0)
		<< std::setw(5) << res.size() 																		//9.NoCheques total
		<< std::setw(18) << std::fixed << std::setprecision(0) << total * 100 								//10.importe Gral
		<< std::setw(106) << 0 																				//11.usoFuturo		
		<< std::endl;
		
		//Imprimimos los items
		std::vector<_CH_072610010943990850>::iterator it; int Item_consecutivo{0};
		for (it = res.begin(); it != res.end(); ++it) {
			Out << "D"																						//1.Tipo D
			<< std::setw(2) << "11" 																		//2.TipoServ (11)
			<< std::setw(5) << ++Item_consecutivo															//3.Consecutivo por Item
			<< std::setw(2) << "60"																			//4.Operacion (60)
			<< std::setw(10) << "1094399085"																//5.NoCuenta
			<< std::setw(7) << it->folio																	//6.NoCheque
			<< fecha.year() << std::setw(2) << static_cast<int>(fecha.month())
			<< std::setw(2) << static_cast<int>(fecha.day()) 												//7.fecha prot
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
	catch (const mysqlpp::BadQuery& er) { std::cout << "Error de Query: " << er.what() << std::endl; }
	catch (const mysqlpp::BadConversion& er) { std::cout << "Error de Conversion: " << er.what() << std::endl; }
	catch (const mysqlpp::Exception& er) { std::cout << "Error: " << er.what() << std::endl; }
return EXIT_SUCCESS;
}