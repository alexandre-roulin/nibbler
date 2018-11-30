#include <utility>


#include <iostream>
#include <KINU/Entity.hpp>
#include <boost/thread/thread.hpp>

#include <KINU/World.hpp>


#include <random>
#include <fstream>
#include <network/ServerTCP.hpp>
#include <network/ClientTCP.hpp>
#include <gui/Core.hpp>
#include <boost/program_options.hpp>
#include <logger.h>
std::string const Snake::basicName[MAX_SNAKE] = {"Jack O'Lantern", "Eden",
												 "Jacky", "Emerald",
												 "Broutille", "Veggie-vie",
												 "jinou42", "Dautta c bo"};

std::ostream &operator<<(std::ostream &os, const Snake &snake) {
	os << "name: " << snake.name << " sprite: "
	   << static_cast<int>(snake.sprite) << " isReady: "
	   << snake.isReady << " id: " << snake.id;
	return os;
}

bool demoGui(int ac, char **av, Univers &univers) {

	if (ac > 1 && !strcmp(av[1], "demo")) {
		univers.create_ui();
		univers.getCore_().demo();
		return (true);
	}
	return (false);
}

void cal() {
	printf("%ld\n", 1543606811383 - 1543606811385);
	printf("%ld\n", 1543606811585 - 1543606811586);
	printf("%ld\n", 1543606811781 - 1543606811783);
	printf("%ld\n", 1543606811985 - 1543606811985);
	printf("%ld\n", 1543606812182 - 1543606812184);
	printf("%ld\n", 1543606812384 - 1543606812386);
	printf("%ld\n", 1543606812583 - 1543606812581);
	printf("%ld\n", 1543606812782 - 1543606812783);
	printf("%ld\n", 1543606812984 - 1543606812981);
	printf("%ld\n", 1543606813185 - 1543606813181);
	printf("%ld\n", 1543606813381 - 1543606813383);
	printf("%ld\n", 1543606813582 - 1543606813585);
	printf("%ld\n", 1543606813782 - 1543606813785);
	printf("%ld\n", 1543606813984 - 1543606813981);
	printf("%ld\n", 1543606814182 - 1543606814184);
	printf("%ld\n", 1543606814384 - 1543606814386);
	printf("%ld\n", 1543606814584 - 1543606814586);
	printf("%ld\n", 1543606814785 - 1543606814786);
	printf("%ld\n", 1543606814981 - 1543606814984);
	printf("%ld\n", 1543606815186 - 1543606815185);
	printf("%ld\n", 1543606815381 - 1543606815383);
	printf("%ld\n", 1543606815583 - 1543606815584);
	printf("%ld\n", 1543606815785 - 1543606815786);
	printf("%ld\n", 1543606815982 - 1543606815985);
	printf("%ld\n", 1543606816185 - 1543606816181);
	printf("%ld\n", 1543606816383 - 1543606816384);
	printf("%ld\n", 1543606816585 - 1543606816586);
	printf("%ld\n", 1543606816785 - 1543606816786);
	printf("%ld\n", 1543606816980 - 1543606816981);
	printf("%ld\n", 1543606817181 - 1543606817183);
	printf("%ld\n", 1543606817383 - 1543606817385);
	printf("%ld\n", 1543606817584 - 1543606817586);
	printf("%ld\n", 1543606817781 - 1543606817784);
	printf("%ld\n", 1543606817982 - 1543606817986);
	printf("%ld\n", 1543606818184 - 1543606818183);
	printf("%ld\n", 1543606818386 - 1543606818385);
	printf("%ld\n", 1543606818584 - 1543606818585);
	printf("%ld\n", 1543606818785 - 1543606818786);
	printf("%ld\n", 1543606818983 - 1543606818985);
	printf("%ld\n", 1543606819185 - 1543606819184);
	printf("%ld\n", 1543606819385 - 1543606819381);
	printf("%ld\n", 1543606819585 - 1543606819583);
	printf("%ld\n", 1543606819781 - 1543606819784);
	printf("%ld\n", 1543606819983 - 1543606819982);
	printf("%ld\n", 1543606820184 - 1543606820185);
	printf("%ld\n", 1543606820385 - 1543606820385);
	printf("%ld\n", 1543606820585 - 1543606820586);
	printf("%ld\n", 1543606820785 - 1543606820781);
	printf("%ld\n", 1543606820983 - 1543606820983);
	printf("%ld\n", 1543606821181 - 1543606821185);
	printf("%ld\n", 1543606821385 - 1543606821386);
	printf("%ld\n", 1543606821585 - 1543606821581);
	printf("%ld\n", 1543606821785 - 1543606821783);
	printf("%ld\n", 1543606821982 - 1543606821985);
	printf("%ld\n", 1543606822183 - 1543606822186);
	printf("%ld\n", 1543606822384 - 1543606822385);
	printf("%ld\n", 1543606822581 - 1543606822583);
	printf("%ld\n", 1543606822785 - 1543606822785);
	printf("%ld\n", 1543606822984 - 1543606822982);
	printf("%ld\n", 1543606823181 - 1543606823184);
	printf("%ld\n", 1543606823382 - 1543606823383);
	printf("%ld\n", 1543606823583 - 1543606823585);
	printf("%ld\n", 1543606823784 - 1543606823786);
	printf("%ld\n", 1543606823981 - 1543606823982);
	printf("%ld\n", 1543606824181 - 1543606824183);
	printf("%ld\n", 1543606824382 - 1543606824385);
	printf("%ld\n", 1543606824583 - 1543606824586);
	printf("%ld\n", 1543606824785 - 1543606824781);
	printf("%ld\n", 1543606824982 - 1543606824983);
	printf("%ld\n", 1543606825184 - 1543606825181);
	printf("%ld\n", 1543606825385 - 1543606825382);
	printf("%ld\n", 1543606825585 - 1543606825583);
	printf("%ld\n", 1543606825782 - 1543606825784);
	printf("%ld\n", 1543606825985 - 1543606825985);
	printf("%ld\n", 1543606826186 - 1543606826184);
	printf("%ld\n", 1543606826381 - 1543606826384);
	printf("%ld\n", 1543606826583 - 1543606826581);
	printf("%ld\n", 1543606826784 - 1543606826783);
	printf("%ld\n", 1543606826985 - 1543606826985);
	printf("%ld\n", 1543606827182 - 1543606827181);
	printf("%ld\n", 1543606827384 - 1543606827383);
	printf("%ld\n", 1543606827581 - 1543606827585);
	printf("%ld\n", 1543606827784 - 1543606827782);
	printf("%ld\n", 1543606827981 - 1543606827981);
	printf("%ld\n", 1543606828183 - 1543606828181);
	printf("%ld\n", 1543606828385 - 1543606828384);
	printf("%ld\n", 1543606828582 - 1543606828585);
	printf("%ld\n", 1543606828781 - 1543606828782);
	printf("%ld\n", 1543606828985 - 1543606828984);
	printf("%ld\n", 1543606829181 - 1543606829182);
	printf("%ld\n", 1543606829382 - 1543606829384);
	printf("%ld\n", 1543606829581 - 1543606829583);
	printf("%ld\n", 1543606829785 - 1543606829785);
	printf("%ld\n", 1543606829984 - 1543606829983);
	printf("%ld\n", 1543606830186 - 1543606830184);
	printf("%ld\n", 1543606830384 - 1543606830383);
	printf("%ld\n", 1543606830585 - 1543606830585);
	printf("%ld\n", 1543606830785 - 1543606830782);
	printf("%ld\n", 1543606830981 - 1543606830981);
	printf("%ld\n", 1543606831183 - 1543606831184);
	printf("%ld\n", 1543606831385 - 1543606831384);
	printf("%ld\n", 1543606831585 - 1543606831584);
	printf("%ld\n", 1543606831784 - 1543606831783);
	printf("%ld\n", 1543606831984 - 1543606831984);
	printf("%ld\n", 1543606832184 - 1543606832182);
	printf("%ld\n", 1543606832383 - 1543606832381);
	printf("%ld\n", 1543606832585 - 1543606832585);
	printf("%ld\n", 1543606832782 - 1543606832783);
	printf("%ld\n", 1543606832984 - 1543606832984);
	printf("%ld\n", 1543606833185 - 1543606833181);
	printf("%ld\n", 1543606833382 - 1543606833383);
	printf("%ld\n", 1543606833585 - 1543606833584);
	printf("%ld\n", 1543606833785 - 1543606833785);
	printf("%ld\n", 1543606833985 - 1543606833981);
	printf("%ld\n", 1543606834185 - 1543606834184);
	printf("%ld\n", 1543606834387 - 1543606834386);
	printf("%ld\n", 1543606834586 - 1543606834583);
	printf("%ld\n", 1543606834782 - 1543606834785);
	printf("%ld\n", 1543606834984 - 1543606834985);
	printf("%ld\n", 1543606835181 - 1543606835184);
	printf("%ld\n", 1543606835383 - 1543606835386);
	printf("%ld\n", 1543606835582 - 1543606835582);
	printf("%ld\n", 1543606835784 - 1543606835782);
	printf("%ld\n", 1543606835982 - 1543606835983);
	printf("%ld\n", 1543606836183 - 1543606836186);
	printf("%ld\n", 1543606836384 - 1543606836386);
	printf("%ld\n", 1543606836586 - 1543606836586);
	printf("%ld\n", 1543606836781 - 1543606836782);
	printf("%ld\n", 1543606836983 - 1543606836986);
	printf("%ld\n", 1543606837184 - 1543606837185);
	printf("%ld\n", 1543606837381 - 1543606837381);
	printf("%ld\n", 1543606837583 - 1543606837581);
	printf("%ld\n", 1543606837785 - 1543606837781);
	printf("%ld\n", 1543606837985 - 1543606837983);
	printf("%ld\n", 1543606838182 - 1543606838184);
	printf("%ld\n", 1543606838384 - 1543606838386);
	printf("%ld\n", 1543606838584 - 1543606838586);
	printf("%ld\n", 1543606838786 - 1543606838781);
	printf("%ld\n", 1543606838986 - 1543606838983);
	printf("%ld\n", 1543606839181 - 1543606839185);
	printf("%ld\n", 1543606839385 - 1543606839385);
	printf("%ld\n", 1543606839582 - 1543606839584);
	printf("%ld\n", 1543606839784 - 1543606839786);
	printf("%ld\n", 1543606839986 - 1543606839981);
	printf("%ld\n", 1543606840182 - 1543606840181);
	printf("%ld\n", 1543606840385 - 1543606840383);
	printf("%ld\n", 1543606840584 - 1543606840584);
	printf("%ld\n", 1543606840782 - 1543606840784);
	printf("%ld\n", 1543606840985 - 1543606840985);
	printf("%ld\n", 1543606841181 - 1543606841182);
	printf("%ld\n", 1543606841383 - 1543606841385);
	printf("%ld\n", 1543606841581 - 1543606841581);
	printf("%ld\n", 1543606841782 - 1543606841783);
	printf("%ld\n", 1543606841981 - 1543606841984);
	printf("%ld\n", 1543606842184 - 1543606842185);
	printf("%ld\n", 1543606842381 - 1543606842382);
	printf("%ld\n", 1543606842584 - 1543606842584);
	printf("%ld\n", 1543606842786 - 1543606842785);
	printf("%ld\n", 1543606842985 - 1543606842981);
	printf("%ld\n", 1543606843181 - 1543606843182);
	printf("%ld\n", 1543606843384 - 1543606843384);
	printf("%ld\n", 1543606843583 - 1543606843586);
	printf("%ld\n", 1543606843784 - 1543606843783);
	printf("%ld\n", 1543606843981 - 1543606843986);
	printf("%ld\n", 1543606844181 - 1543606844181);
	printf("%ld\n", 1543606844382 - 1543606844383);
	printf("%ld\n", 1543606844581 - 1543606844584);
	printf("%ld\n", 1543606844784 - 1543606844785);
	printf("%ld\n", 1543606844981 - 1543606844981);
	printf("%ld\n", 1543606845183 - 1543606845183);
	printf("%ld\n", 1543606845384 - 1543606845381);
	printf("%ld\n", 1543606845581 - 1543606845584);
	printf("%ld\n", 1543606845785 - 1543606845786);
	printf("%ld\n", 1543606845982 - 1543606845985);
	printf("%ld\n", 1543606846184 - 1543606846185);
	printf("%ld\n", 1543606846381 - 1543606846382);
	printf("%ld\n", 1543606846581 - 1543606846584);
	printf("%ld\n", 1543606846784 - 1543606846781);
	printf("%ld\n", 1543606846983 - 1543606846985);
	printf("%ld\n", 1543606847184 - 1543606847181);
	printf("%ld\n", 1543606847385 - 1543606847383);
	printf("%ld\n", 1543606847584 - 1543606847586);
	printf("%ld\n", 1543606847786 - 1543606847786);
	printf("%ld\n", 1543606847985 - 1543606847981);
	printf("%ld\n", 1543606848183 - 1543606848184);
	printf("%ld\n", 1543606848384 - 1543606848386);
	printf("%ld\n", 1543606848585 - 1543606848585);
	printf("%ld\n", 1543606848785 - 1543606848781);
	printf("%ld\n", 1543606848985 - 1543606848981);
	printf("%ld\n", 1543606849184 - 1543606849183);
	printf("%ld\n", 1543606849382 - 1543606849385);
	printf("%ld\n", 1543606849584 - 1543606849586);
	printf("%ld\n", 1543606849784 - 1543606849780);
	printf("%ld\n", 1543606849981 - 1543606849983);
	printf("%ld\n", 1543606850184 - 1543606850185);
	printf("%ld\n", 1543606850385 - 1543606850386);
	printf("%ld\n", 1543606850585 - 1543606850586);
	printf("%ld\n", 1543606850785 - 1543606850781);
	printf("%ld\n", 1543606850985 - 1543606850983);
	printf("%ld\n", 1543606851181 - 1543606851184);
	printf("%ld\n", 1543606851381 - 1543606851386);
	printf("%ld\n", 1543606851583 - 1543606851581);
	printf("%ld\n", 1543606851783 - 1543606851783);
	printf("%ld\n", 1543606851985 - 1543606851985);
	printf("%ld\n", 1543606852184 - 1543606852186);
	printf("%ld\n", 1543606852385 - 1543606852381);
	printf("%ld\n", 1543606852582 - 1543606852583);
	printf("%ld\n", 1543606852783 - 1543606852785);
	printf("%ld\n", 1543606852984 - 1543606852983);
	printf("%ld\n", 1543606853181 - 1543606853185);
	printf("%ld\n", 1543606853383 - 1543606853382);
	printf("%ld\n", 1543606853585 - 1543606853585);
	printf("%ld\n", 1543606853781 - 1543606853781);
	printf("%ld\n", 1543606853982 - 1543606853984);
	printf("%ld\n", 1543606854185 - 1543606854182);
	printf("%ld\n", 1543606854381 - 1543606854384);
	printf("%ld\n", 1543606854586 - 1543606854581);
	printf("%ld\n", 1543606854786 - 1543606854784);
	printf("%ld\n", 1543606854981 - 1543606854982);
	printf("%ld\n", 1543606855181 - 1543606855185);
	printf("%ld\n", 1543606855381 - 1543606855384);
	printf("%ld\n", 1543606855583 - 1543606855586);
	printf("%ld\n", 1543606855783 - 1543606855785);
	printf("%ld\n", 1543606855985 - 1543606855982);
	printf("%ld\n", 1543606856181 - 1543606856181);
	printf("%ld\n", 1543606856385 - 1543606856383);
	printf("%ld\n", 1543606856581 - 1543606856584);
	printf("%ld\n", 1543606856784 - 1543606856786);
	printf("%ld\n", 1543606856984 - 1543606856985);
	printf("%ld\n", 1543606857185 - 1543606857183);
	printf("%ld\n", 1543606857385 - 1543606857384);
	printf("%ld\n", 1543606857582 - 1543606857585);
	printf("%ld\n", 1543606857785 - 1543606857782);
	printf("%ld\n", 1543606857984 - 1543606857981);
	printf("%ld\n", 1543606858184 - 1543606858183);
	printf("%ld\n", 1543606858381 - 1543606858384);
	printf("%ld\n", 1543606858585 - 1543606858586);
	printf("%ld\n", 1543606858782 - 1543606858781);
	printf("%ld\n", 1543606858984 - 1543606858981);
	printf("%ld\n", 1543606859182 - 1543606859184);
	printf("%ld\n", 1543606859385 - 1543606859386);
	printf("%ld\n", 1543606859585 - 1543606859582);
	printf("%ld\n", 1543606859781 - 1543606859781);
	printf("%ld\n", 1543606859984 - 1543606859985);
	printf("%ld\n", 1543606860182 - 1543606860185);
	printf("%ld\n", 1543606860384 - 1543606860386);
	printf("%ld\n", 1543606860583 - 1543606860586);
	printf("%ld\n", 1543606860785 - 1543606860786);
	printf("%ld\n", 1543606860986 - 1543606860982);
	printf("%ld\n", 1543606861186 - 1543606861184);
	printf("%ld\n", 1543606861385 - 1543606861381);
	printf("%ld\n", 1543606861584 - 1543606861583);
	printf("%ld\n", 1543606861784 - 1543606861785);
	printf("%ld\n", 1543606861981 - 1543606861982);
	printf("%ld\n", 1543606862185 - 1543606862184);
	printf("%ld\n", 1543606862385 - 1543606862381);
	printf("%ld\n", 1543606862581 - 1543606862582);
	printf("%ld\n", 1543606862781 - 1543606862784);
	printf("%ld\n", 1543606862983 - 1543606862986);
	printf("%ld\n", 1543606863185 - 1543606863185);
	printf("%ld\n", 1543606863385 - 1543606863385);
	printf("%ld\n", 1543606863582 - 1543606863585);
	printf("%ld\n", 1543606863784 - 1543606863786);
	printf("%ld\n", 1543606863985 - 1543606863985);
	printf("%ld\n", 1543606864185 - 1543606864182);
	printf("%ld\n", 1543606864385 - 1543606864384);
	printf("%ld\n", 1543606864581 - 1543606864583);
	printf("%ld\n", 1543606864786 - 1543606864786);
	printf("%ld\n", 1543606864985 - 1543606864985);
	printf("%ld\n", 1543606865182 - 1543606865185);
	printf("%ld\n", 1543606865381 - 1543606865382);
	printf("%ld\n", 1543606865582 - 1543606865585);
	printf("%ld\n", 1543606865783 - 1543606865785);
	printf("%ld\n", 1543606865985 - 1543606865986);
	printf("%ld\n", 1543606866183 - 1543606866186);
	printf("%ld\n", 1543606866381 - 1543606866382);
	printf("%ld\n", 1543606866584 - 1543606866584);
	printf("%ld\n", 1543606866784 - 1543606866785);
	printf("%ld\n", 1543606866983 - 1543606866986);
	printf("%ld\n", 1543606867181 - 1543606867186);
	printf("%ld\n", 1543606867384 - 1543606867381);
	printf("%ld\n", 1543606867584 - 1543606867583);
	printf("%ld\n", 1543606867781 - 1543606867785);
	printf("%ld\n", 1543606867983 - 1543606867984);
	printf("%ld\n", 1543606868182 - 1543606868184);
	printf("%ld\n", 1543606868383 - 1543606868385);
	printf("%ld\n", 1543606868584 - 1543606868586);
	printf("%ld\n", 1543606868785 - 1543606868781);
	printf("%ld\n", 1543606868986 - 1543606868983);
	printf("%ld\n", 1543606869183 - 1543606869185);
	printf("%ld\n", 1543606869383 - 1543606869386);
	printf("%ld\n", 1543606869581 - 1543606869583);
	printf("%ld\n", 1543606869783 - 1543606869785);
	printf("%ld\n", 1543606869983 - 1543606869985);
	printf("%ld\n", 1543606870185 - 1543606870186);
	printf("%ld\n", 1543606870385 - 1543606870381);
	printf("%ld\n", 1543606870582 - 1543606870582);
	printf("%ld\n", 1543606870784 - 1543606870785);
	printf("%ld\n", 1543606870984 - 1543606870986);
	printf("%ld\n", 1543606871184 - 1543606871181);
	printf("%ld\n", 1543606871383 - 1543606871383);
	printf("%ld\n", 1543606871586 - 1543606871585);
	printf("%ld\n", 1543606871781 - 1543606871782);
	printf("%ld\n", 1543606871984 - 1543606871983);
	printf("%ld\n", 1543606872184 - 1543606872185);
	printf("%ld\n", 1543606872385 - 1543606872385);
	printf("%ld\n", 1543606872581 - 1543606872582);
	printf("%ld\n", 1543606872785 - 1543606872784);
	printf("%ld\n", 1543606872985 - 1543606872981);
	printf("%ld\n", 1543606873186 - 1543606873183);
	printf("%ld\n", 1543606873381 - 1543606873382);
	printf("%ld\n", 1543606873585 - 1543606873585);
	printf("%ld\n", 1543606873783 - 1543606873785);
	printf("%ld\n", 1543606873985 - 1543606873985);
	printf("%ld\n", 1543606874183 - 1543606874185);
	printf("%ld\n", 1543606874381 - 1543606874384);
	printf("%ld\n", 1543606874583 - 1543606874586);
	printf("%ld\n", 1543606874781 - 1543606874781);
	printf("%ld\n", 1543606874985 - 1543606874983);
	printf("%ld\n", 1543606875182 - 1543606875181);
	printf("%ld\n", 1543606875385 - 1543606875383);
	printf("%ld\n", 1543606875581 - 1543606875584);
	printf("%ld\n", 1543606875783 - 1543606875785);
	printf("%ld\n", 1543606875985 - 1543606875981);
	printf("%ld\n", 1543606876185 - 1543606876183);
	printf("%ld\n", 1543606876381 - 1543606876384);
	printf("%ld\n", 1543606876585 - 1543606876586);
	printf("%ld\n", 1543606876785 - 1543606876785);
	printf("%ld\n", 1543606876985 - 1543606876985);
	printf("%ld\n", 1543606877186 - 1543606877180);
	printf("%ld\n", 1543606877385 - 1543606877381);
	printf("%ld\n", 1543606877581 - 1543606877585);
	printf("%ld\n", 1543606877785 - 1543606877782);
	printf("%ld\n", 1543606877981 - 1543606877982);
	printf("%ld\n", 1543606878184 - 1543606878184);
	printf("%ld\n", 1543606878383 - 1543606878386);
	printf("%ld\n", 1543606878586 - 1543606878585);
	printf("%ld\n", 1543606878780 - 1543606878785);
	printf("%ld\n", 1543606878983 - 1543606878986);
	printf("%ld\n", 1543606879185 - 1543606879185);
	printf("%ld\n", 1543606879385 - 1543606879385);
	printf("%ld\n", 1543606879585 - 1543606879581);
	printf("%ld\n", 1543606879782 - 1543606879783);
	printf("%ld\n", 1543606879984 - 1543606879984);
	printf("%ld\n", 1543606880186 - 1543606880184);
	printf("%ld\n", 1543606880386 - 1543606880384);
	printf("%ld\n", 1543606880585 - 1543606880581);
	printf("%ld\n", 1543606880782 - 1543606880784);
	printf("%ld\n", 1543606880985 - 1543606880985);
	printf("%ld\n", 1543606881186 - 1543606881186);
	printf("%ld\n", 1543606881385 - 1543606881381);
	printf("%ld\n", 1543606881585 - 1543606881584);
	printf("%ld\n", 1543606881781 - 1543606881784);
	printf("%ld\n", 1543606881981 - 1543606881981);
	printf("%ld\n", 1543606882183 - 1543606882184);
	printf("%ld\n", 1543606882384 - 1543606882381);
	printf("%ld\n", 1543606882583 - 1543606882584);
	printf("%ld\n", 1543606882782 - 1543606882783);
	printf("%ld\n", 1543606882981 - 1543606882984);
	printf("%ld\n", 1543606883184 - 1543606883186);
	printf("%ld\n", 1543606883385 - 1543606883381);
	printf("%ld\n", 1543606883584 - 1543606883583);
	printf("%ld\n", 1543606883786 - 1543606883785);
	printf("%ld\n", 1543606883981 - 1543606883984);
	printf("%ld\n", 1543606884183 - 1543606884186);
	printf("%ld\n", 1543606884385 - 1543606884386);
	printf("%ld\n", 1543606884585 - 1543606884585);
	printf("%ld\n", 1543606884785 - 1543606884786);
	printf("%ld\n", 1543606884985 - 1543606884985);
	printf("%ld\n", 1543606885185 - 1543606885185);
	printf("%ld\n", 1543606885381 - 1543606885382);
	printf("%ld\n", 1543606885583 - 1543606885584);
	printf("%ld\n", 1543606885781 - 1543606885781);
	printf("%ld\n", 1543606885981 - 1543606885984);
	printf("%ld\n", 1543606886185 - 1543606886186);
	printf("%ld\n", 1543606886385 - 1543606886385);
	printf("%ld\n", 1543606886582 - 1543606886581);
	printf("%ld\n", 1543606886785 - 1543606886786);
	printf("%ld\n", 1543606886985 - 1543606886986);
	printf("%ld\n", 1543606887185 - 1543606887186);
	printf("%ld\n", 1543606887383 - 1543606887385);
	printf("%ld\n", 1543606887581 - 1543606887581);
	printf("%ld\n", 1543606887784 - 1543606887784);
	printf("%ld\n", 1543606887985 - 1543606887985);
	printf("%ld\n", 1543606888182 - 1543606888185);
	printf("%ld\n", 1543606888383 - 1543606888383);
	printf("%ld\n", 1543606888584 - 1543606888585);
	printf("%ld\n", 1543606888783 - 1543606888784);
	printf("%ld\n", 1543606888984 - 1543606888982);
	printf("%ld\n", 1543606889182 - 1543606889184);
	printf("%ld\n", 1543606889383 - 1543606889381);
	printf("%ld\n", 1543606889585 - 1543606889583);
	printf("%ld\n", 1543606889784 - 1543606889781);
	printf("%ld\n", 1543606889986 - 1543606889982);
	printf("%ld\n", 1543606890183 - 1543606890184);
	printf("%ld\n", 1543606890385 - 1543606890386);
	printf("%ld\n", 1543606890586 - 1543606890586);
	printf("%ld\n", 1543606890786 - 1543606890785);
	printf("%ld\n", 1543606890981 - 1543606890985);
	printf("%ld\n", 1543606891183 - 1543606891182);
	printf("%ld\n", 1543606891385 - 1543606891385);
	printf("%ld\n", 1543606891584 - 1543606891585);
	printf("%ld\n", 1543606891786 - 1543606891782);
	printf("%ld\n", 1543606891982 - 1543606891984);
	printf("%ld\n", 1543606892185 - 1543606892183);
	printf("%ld\n", 1543606892381 - 1543606892385);
	printf("%ld\n", 1543606892583 - 1543606892583);
	printf("%ld\n", 1543606892784 - 1543606892785);
	printf("%ld\n", 1543606892985 - 1543606892985);
	printf("%ld\n", 1543606893185 - 1543606893182);
	printf("%ld\n", 1543606893385 - 1543606893384);
	printf("%ld\n", 1543606893582 - 1543606893585);
	printf("%ld\n", 1543606893785 - 1543606893785);
	printf("%ld\n", 1543606893981 - 1543606893981);
	printf("%ld\n", 1543606894185 - 1543606894183);
	printf("%ld\n", 1543606894386 - 1543606894384);
	printf("%ld\n", 1543606894581 - 1543606894583);
	printf("%ld\n", 1543606894785 - 1543606894785);
	printf("%ld\n", 1543606894986 - 1543606894985);
	printf("%ld\n", 1543606895185 - 1543606895184);
	printf("%ld\n", 1543606895382 - 1543606895386);
	printf("%ld\n", 1543606895581 - 1543606895581);
	printf("%ld\n", 1543606895783 - 1543606895782);
	printf("%ld\n", 1543606895984 - 1543606895984);
	printf("%ld\n", 1543606896184 - 1543606896186);
	printf("%ld\n", 1543606896381 - 1543606896382);
	printf("%ld\n", 1543606896585 - 1543606896584);
	printf("%ld\n", 1543606896781 - 1543606896781);
	printf("%ld\n", 1543606896983 - 1543606896983);
	printf("%ld\n", 1543606897185 - 1543606897181);
	printf("%ld\n", 1543606897383 - 1543606897383);
	printf("%ld\n", 1543606897581 - 1543606897584);
	printf("%ld\n", 1543606897784 - 1543606897784);
	printf("%ld\n", 1543606897984 - 1543606897985);
	printf("%ld\n", 1543606898186 - 1543606898185);
	printf("%ld\n", 1543606898385 - 1543606898381);
	printf("%ld\n", 1543606898582 - 1543606898583);
	printf("%ld\n", 1543606898784 - 1543606898781);
	printf("%ld\n", 1543606898983 - 1543606898984);
	printf("%ld\n", 1543606899185 - 1543606899184);
	printf("%ld\n", 1543606899385 - 1543606899386);
	printf("%ld\n", 1543606899585 - 1543606899582);
	printf("%ld\n", 1543606899785 - 1543606899784);
	printf("%ld\n", 1543606899982 - 1543606899985);
	printf("%ld\n", 1543606900185 - 1543606900184);
	printf("%ld\n", 1543606900383 - 1543606900385);
	printf("%ld\n", 1543606900581 - 1543606900585);
	printf("%ld\n", 1543606900783 - 1543606900781);
	printf("%ld\n", 1543606900985 - 1543606900983);
	printf("%ld\n", 1543606901181 - 1543606901184);
	printf("%ld\n", 1543606901382 - 1543606901385);
	printf("%ld\n", 1543606901586 - 1543606901581);
	printf("%ld\n", 1543606901784 - 1543606901783);
	printf("%ld\n", 1543606901985 - 1543606901985);
	printf("%ld\n", 1543606902181 - 1543606902185);
	printf("%ld\n", 1543606902381 - 1543606902383);
	printf("%ld\n", 1543606902582 - 1543606902582);
	printf("%ld\n", 1543606902783 - 1543606902784);
	printf("%ld\n", 1543606902985 - 1543606902986);
	printf("%ld\n", 1543606903181 - 1543606903184);
	printf("%ld\n", 1543606903386 - 1543606903386);
	printf("%ld\n", 1543606903586 - 1543606903586);
	printf("%ld\n", 1543606903785 - 1543606903786);
	printf("%ld\n", 1543606903986 - 1543606903982);
	printf("%ld\n", 1543606904186 - 1543606904184);
	printf("%ld\n", 1543606904385 - 1543606904381);
	printf("%ld\n", 1543606904582 - 1543606904582);
	printf("%ld\n", 1543606904784 - 1543606904783);
	printf("%ld\n", 1543606904981 - 1543606904983);
	printf("%ld\n", 1543606905181 - 1543606905185);
	printf("%ld\n", 1543606905383 - 1543606905383);
	printf("%ld\n", 1543606905583 - 1543606905586);
	printf("%ld\n", 1543606905785 - 1543606905782);
}

void nibbler(Univers &univers) {
	std::string buffer;

	if (univers.testFlag(Univers::SOUND)) {
		univers.load_external_sound_library(std::string("Game pro"),
											std::string(
													PATH_SOUND_LIBRARY_SFML));

		univers.getSound().addNoise(
				std::string("./ressource/sound/appear-online.ogg"));
		univers.getSound().addNoise(std::string("./ressource/sound/yes-2.wav"));
		univers.getSound().addNoise(std::string("./ressource/sound/click.wav"));
		univers.getSound().addNoise(
				std::string("./ressource/sound/slime10.wav"));
		univers.getSound().addNoise(std::string("./ressource/sound/hit17.ogg"));
		/*
		univers.getSound().setMusic("./ressource/sound/zelda.ogg");
		univers.getSound().playMusic();
		*/
	}

	for (;;) {
		std::cout << "$> ";
		std::getline(std::cin, buffer);
		if (buffer == "autos") {
			univers.create_server();
			univers.getClientTCP_().connect("localhost", "4242");
			sleep(1);
			univers.getClientTCP_().change_state_ready();
			univers.setMapSize(35);
			univers.load_external_display_library(std::string("Game pro"),
												  std::string(
														  PATH_DISPLAY_LIBRARY_SFML));

		}
		if (buffer == "autoc") {
			univers.getClientTCP_().connect("localhost", "4242");
			sleep(1);
			univers.getClientTCP_().change_state_ready();
			univers.setMapSize(35);
			univers.load_external_display_library(std::string("Game pro"),
												  std::string(
														  PATH_DISPLAY_LIBRARY_SFML));
			univers.loop();
		}
		if (buffer == "autocs") {
			std::cout << "connect > ";
			std::getline(std::cin, buffer);
			univers.getClientTCP_().connect(buffer.c_str(), "4242");
			sleep(1);
			univers.getClientTCP_().change_state_ready();
			univers.setMapSize(35);
			univers.load_external_display_library(std::string("Game pro"),
												  std::string(
														  PATH_DISPLAY_LIBRARY_SFML));
			univers.loop();
		}
		if (buffer == "loop") {
			ClientTCP::StartInfo startInfo;
			univers.getClientTCP_()
					.write_socket(
							ClientTCP::add_prefix(START_GAME, &startInfo));
			univers.loop();
		}
		if (buffer == "server")
			univers.create_server();
		if (buffer == "connect") {
			std::string dns, port;
			std::cout << "dns > ";
			std::getline(std::cin, dns);
			std::cout << "port > ";
			std::getline(std::cin, port);
			univers.getClientTCP_().connect(dns, port);
		}
		if (buffer == "ready") {
			univers.getClientTCP_().change_state_ready();
		}

		if (buffer == "game") {
			univers.setMapSize(35);
			univers.load_external_display_library(std::string("Game pro"),
												  std::string(
														  PATH_DISPLAY_LIBRARY_SFML));

			ClientTCP::StartInfo startInfo;
			univers.getClientTCP_()
					.write_socket(
							ClientTCP::add_prefix(START_GAME, &startInfo));
			univers.loop();
		}
		if (buffer == "game1") {
			univers.setMapSize(35);
			univers.load_external_display_library(std::string("Game pro"),
												  std::string(
														  PATH_DISPLAY_LIBRARY_SFML));
			univers.loop();
		}
		if (buffer == "ui") {
			univers.create_ui();
			univers.getCore_().aState();
			Core *core = univers.releaseCore_();
			if (core)
				delete core;

			if (univers.getClientTCP_().isOpenGame()) {

				//univers.getClientTCP_().change_state_ready();
				//sleep(1);

				univers.load_external_display_library(std::string("Game pro"),
													  std::string(
															  PATH_DISPLAY_LIBRARY_SFML));

				ClientTCP::StartInfo startInfo;
				if (univers.isServer()) {
					univers.getClientTCP_()
							.write_socket(
									ClientTCP::add_prefix(START_GAME,
														  &startInfo));
				} else {
					std::cout << "I launch " << std::endl;
				}
				univers.loop();
			}
			return;
		}
	}
}

int main(int argc, char **argv) {
	srand(time(NULL));
	char path[] = "/tmp/log.out";
	logger_init(path);
	try {
		Univers univers;

		boost::program_options::options_description desc("Options");
		desc.add_options() // <- Retourne une reference ! OP
				("help", "Print help messages")
				("sound", "enable the sound");

		boost::program_options::variables_map vm;
		try {
			boost::program_options::store(
					boost::program_options::parse_command_line(argc, argv,
															   desc), vm);

			if (vm.count("help")) {
				std::cout << "Basic Command Line Parameter App" << std::endl
						  << desc << std::endl;
				return (0);
			}
			if (vm.count("sound")) {
				univers.setFlag(Univers::SOUND);
			}
			boost::program_options::notify(vm);
		}
		catch (const boost::program_options::error &e) {
			std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
			std::cerr << desc << std::endl;
			return (0);
		}
		if (demoGui(argc, argv, univers))
			return (0);
		nibbler(univers);
	}
	catch (const std::exception &e) {
		std::cerr << "Unhandled Exception reached the top of main: "
				  << e.what() << ", application will now exit" << std::endl;
	}

	return (0);
}
