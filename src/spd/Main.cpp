/**
 * Main.cpp
 *
 * @date 2012/08/31
 * @author katsumata
 */



#include <random>
#include <functional>

#include <iostream>
#include <iomanip>

#include <vector>
#include <string>
#include <cstdlib>

#include <memory>

#include <unistd.h>

#include <sys/sysinfo.h>
#include <fstream>

#include <limits>

#include <chrono>
#include <thread>
#include <ratio>

#include <stdlib.h>

#include <regex>
#include <array>

#include <boost/any.hpp>

#include <memory>

#include "core/Strategy.hpp"
#include "param/Parameter.hpp"
#include "topology/Moore.hpp"
#include "core/Player.hpp"
#include "output/ConsoleOutput.hpp"
#include "core/Space.hpp"
#include "rule/SpdRule.hpp"

#include "core/Converter.hpp"

#include "param/parser/ParseParam.hpp"

// シリアライズ用
#include "core/msgpack/SerializedSpace.hpp"


#include "core/maker/PlayerMaker.hpp"
#include <fstream>

#include <functional>

#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/device/file.hpp>

using namespace std;

using namespace spd;
using namespace spd::core;


void timeCount(std::function<void(int, int)> lambda, int a, int b, std::string name) {

	cout << "start" << endl;

	const int COUNT = 1000000000;

	auto t1 = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < COUNT; ++i) {

		lambda(a, b);
	}

	auto t2 = std::chrono::high_resolution_clock::now();
	std::cout << name << " : "
			<< std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count()
			<< " milliseconds\n";

}


#include "core/BoostAnyConverter.hpp"


void test(int target, int radius) {
	int side = 10;

	int x0 = target % side;
	int y0 = target / side;

	std::cout << "p ( "<<  target << ", " << radius << " ): " << std::endl;
	for (int y = -1 * radius; y <= radius; ++y) {

		int len = radius * 2 - std::abs(y) + 1;

		int startX = 0 - static_cast<int> (len / 2);
		if (((y0 + y) % 2 == 0) &&
				(y0 % 2 == 1) && y != 0) {
			++startX;
		}
		int endX = startX + len - 1;


		for (int x = startX; x <= endX; ++x) {

			int nSite = (x0 + x +side)%side+(y0 + y +side)%side*side;


			int rad;
			if (y0 % 2 != 0) {
				rad = (y % 2 == 0)? std::abs(x) + std::abs(y / 2) : std::abs(std::round(x - 0.5)) + std::abs(y / 2);
			} else {
				rad = (y % 2 == 0)? std::abs(x) + std::abs(y / 2) : std::abs(std::round(x + 0.5)) + std::abs(y / 2);
			}

			std::cout << "[ rad : " << rad << " , n : ";
			int kore = max(std::abs(y), std::abs(rad));
			if (y == 0) {
				//kore = abs(x);
			}

			std::cout << kore << "]" << " " << nSite << std::endl;



		}
	}

	std::cout << std::endl;
}


int main(int argc, char* argv[]) {

	std::vector<int> test;
	test.reserve(100);


	cout << "reserve size:" << test.size() << "\n";






	auto show = [] (std::vector<int> x) -> void {
		for (auto i : x) {
			cout << i << ", ";
		}
		cout << "\n";
	};

	show(test);

	test.push_back(22);
	cout << "af push size:" << test.size() << "\n";
	show(test);



	exit (1);


//	{
//		test(55, 5);
//
//		std::cout << "----------------------------\n";
//		test(22, 5);
//	}
//	exit(0);


	/*
	// シリアライズ
	{
		spd::param::Parameter param = spd::param::Parameter();

		// オプションパーサー
		spd::param::parser::ParseParam pp;
		pp.parse(argc, argv, param);

		param.showParameter(std::cout);

		// シミュレーション空間の作成
		spd::core::Space space = Space(param);
		space.getParameter().getPlayerMaker()->initPlayer(space.getPlayers(), space);

		std::ofstream file("/home/katsumata/Desktop/backup/output/data.mpac");

		spd::core::serialize::SerializeSpace ss (space);

		msgpack::pack(&file, ss);

	}

	cout << "serialize finish." << std::endl;
	// デシリアライズ
	{
		std::ifstream file("/home/katsumata/Desktop/backup/output/dump/spd_dump_000_00002.mpac");
		std::istreambuf_iterator<char> first(file);
		std::istreambuf_iterator<char> last;

		const std::string data(first, last);

		try {
			msgpack::unpacked msg;
			msgpack::unpack(&msg, data.data(), data.size());

			msgpack::object obj = msg.get();


			auto ss = obj.as<spd::core::serialize::SerializeSpace>();

			std::cout << ss.getPlayersMap().size() << std::endl;
			for (auto maps : ss.getPlayersMap()) {
				std::cout << maps.first.getScore() << std::endl;
				std::cout << "koko" << std::endl;
			}


		}
		catch (msgpack::unpack_error&) {
			std::cout << "unpack failed" << std::endl;
		}
		catch (msgpack::type_error& e) {
			std::cout << e.what() << std::endl;
			std::cout << "type error" << std::endl;
		}
	}
	exit(0);
	/**

*
	{
		// 圧縮を試す
		std::ifstream infile("/home/katsumata/Desktop/backup/output/dump/spd_dump_000_00002.mpac", ios_base::in | ios_base::binary);

		if (!infile) {
			std::cerr << "aaaaaaaaaaaaaaaa" << std::endl;
		}
		std::ofstream outfile("/home/katsumata/Desktop/backup/output/dump/spd_dump_000_00002.mpac.gzip", ios_base::out | ios_base::binary);

		boost::iostreams::filtering_istreambuf in;

		in.push(boost::iostreams::gzip_compressor());
		in.push(infile);

		boost::iostreams::copy(in, outfile);

		std:cout << "comp fin" << std::endl;

	}*/
//	{
//		std::ifstream infile("/home/katsumata/Desktop/backup/output/dump/spd_dump_000_00002.mpac", ios_base::in | ios_base::binary);
//
//
//		if (!infile) {
//			std::cerr << "aaaaaaaaaaaaaaaa" << std::endl;
//		}
//		std::cout << "bbbb" << std::endl;
//		std::ofstream outfile("/home/katsumata/Desktop/backup/output/dump/spd_dump_000_00002.mpac.gz", ios_base::out | ios_base::binary);
//
//		boost::iostreams::filtering_istreambuf in;
//
//		in.push(boost::iostreams::gzip_decompressor());
//		in.push(infile);
//
//		std::cout << "ccc" << std::endl;
//
//		boost::iostreams::copy(in, outfile);
//
//		std::cout << "fin." << std::endl;
//	}
//	exit(0);
//*/
	spd::param::Parameter param = spd::param::Parameter();

//	auto iii = param.getRuntimeParameter()->getPayoffMatrix();
//	std::cout << iii[0][0] << std::endl;
//	std::cout << iii[0][1] << std::endl;
//	std::cout << iii[1][0] << std::endl;
//	std::cout << iii[1][1] << std::endl;


	// オプションパーサー
	spd::param::parser::ParseParam pp;
	pp.parse(argc, argv, param);

	param.showParameter(std::cout);

	// シミュレーション空間の作成
	spd::core::Space space = Space(param);

//	int a = 8;
//	int b = 4;
//
//	timeCount([](int a, int b){
//			a + b;
//	}, a, b, std::string("adder"));
//
//	timeCount([](int a, int b){
//		a / b;
//	}, a, b, std::string("div"));
//
//	timeCount([](int a, int b){
//		a % b;
//	}, a, b, std::string("mod"));
//
//	exit(0);


	while (!space.run()){};

//	for (int sim = 0, endSim = param.getInitialParameter()->getSimCount(); sim < endSim; ++sim) {
//		space.run();
//	}
	std::cout << "Finish!!!!!" << std::endl;

	 /**/
/** random*
	int RANDNUM = 10;

	std::random_device rd;
	auto seed = rd();

	std::mt19937_64 engine(seed);
	std::uniform_real_distribution<double> dist (10, 13);
	auto hoge = std::bind(std::uniform_real_distribution<double> (10, 13), engine);

	cout << "seed: " << seed << endl;
	std::cout << "min: " << engine.min() << std::endl ;
	std::cout << "max: " << engine.max() << std::endl ;

	for (int i =0; i < RANDNUM; ++i){
		cout << " " << hoge();
		cout << endl;
	}

	cout << "a" << dist.a() << endl;
	cout << "" << dist.min() << endl;

	cout << "after --\n";
	engine.seed(seed);
	engine.discard(5);
	hoge = std::bind(std::uniform_real_distribution<double> (0, 1), engine);
	for (int i =5; i < RANDNUM; ++i){
		cout <<" " << dist(engine); cout << endl;
	}

/*
	double repeatNum = 1000000000;
	cout << "nomal aaaaa \n";
	int minus = 0, plus =0;
	std::normal_distribution<> noD (0.5+repeatNum, 0.25);
	for (int i = 0; i < repeatNum; ++i) {
		if (noD(engine) < repeatNum) {
			minus++;
		} else if (noD(engine) >= repeatNum + 1) {
			plus++;
		}
	}
	cout << " minu" << minus << "\n";
	cout << " plus" << plus << "\n";

	/**
	cout << "ユニークポインタのサイズ" << endl;
	cout << sizeof(std::unique_ptr<spd::param::Parameter>) << endl; // 8

	cout << "パラメタクラスのポインタサイズ" << endl;
	cout << sizeof(spd::param::Parameter*) << endl; // 8

	cout << "onlyVec : " ;
	cout << sizeof(std::vector<std::shared_ptr<spd::core::Player>>) << endl; // 24

	cout << "playPtr : " ;
	cout << sizeof(std::shared_ptr<spd::core::Player>) << endl; // 16
	cout << sizeof(std::unique_ptr<spd::core::Player>) << endl; //8
	cout << sizeof(std::weak_ptr<spd::core::Player>) << endl;   // 16

	cout << "プレイヤサイズ" << endl;
	cout << sizeof(spd::core::Player) << endl; // 936

	cout << "----------------" << endl;
	int hoge = 10* (static_cast<int>(sizeof(std::vector<std::shared_ptr<spd::core::Player>>)) + 8 * (static_cast<int>(sizeof(spd::core::Player))));
	cout << hoge << endl;
	 */


	/**
	cout << "----- ここからてすと -----" << endl;
	int side = 3;
	unsigned int radius = 1;
	for (unsigned int i = 0; i < 9; ++i) {
		for (int y = (i / side) - radius, yMax = (i / side) + radius; y <= yMax; ++y) {
			//cout << yMax << endl;
			for (int x = (i % side) - radius, xMax = (i % side) + radius; x <= xMax; ++x) {
				cout << "self: " << i << "   neigh(i " << (x+side)%side+(y+side)%side*side << " : y " << y << " ,x " << x << ")\n";
				//cout << xMax << endl;
			}

		}
		cout << "next player---" << endl;
	}
	*/

	/**
	 * オペレータオーバーロードに関する時間計測

	std::shared_ptr<Strategy> str = std::make_shared<Strategy>("cccccccccccc", 9);

	Player p('C', str);

	const int LOOP = 500000;

	cout << "p1:socre "<< p.getScore() << endl;
	auto t1 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < LOOP; ++i) {
		for (int j = 0; j< LOOP; ++j) {
			p += 1;
		}
	}

	auto t2 = std::chrono::high_resolution_clock::now();
	cout << "p:score " << p.getScore() << endl;
	std::cout << "operator over "
			<< std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count()
			<< " milliseconds\n";

	//-------------------------------------------------------
	Player p2('C', str);
	cout << "p:socre "<< p2.getScore() << endl;
	t1 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < LOOP; ++i) {
		for (int j = 0; j< LOOP; ++j) {
			p2.setScore(p2.getScore()+1);
		}
	}

	t2 = std::chrono::high_resolution_clock::now();
	cout << "p:score " << p2.getScore() << endl;
	std::cout << "getter/setter "
			<< std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count()
			<< " milliseconds\n";
*/



	/*
	cout << "---------memory-----------" << endl;
	std::string token;
	    std::ifstream file("/proc/meminfo");
	    unsigned long mem, buf;
	    while(file >> token) {
	        if(token == "MemFree:") {
	            if(file >> mem) {
	                cout << "mem: " << mem << endl;
	            } else {
	            	cout << "0 de owari" << endl;
	            }
	        } else if (token == "Inactive:") {
	        	if (file >> buf) {
	        		cout << "Ina: " << buf << endl;
	        	}
	        }
	        // ignore rest of the line
	        //file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	    }
	    cout << "saigo: " << mem + buf << endl;
	    return 0; // nothing found
*/


//http://blog.cfelde.com/2010/06/c-vs-java-perfoemance/
	//  ふぁんくた stl

	// mojiretu suuji henkan
	// rand
	// file direcotory
	// xml
	// std: unique ptr, boost shared ptr
	// weak_ptr

	// ろべーるc++ , effective c++ , c++ designe and evolution



	//spd::param::Parameter params;

	//params.getInitialParameter().setUnitNum(32767);

	//cout << "setUni: " << params.getInitialParameter().getUnitNum() << endl;
	/**

	std::vector<std::pair<Strategy, int> > slist = params.getStrategyList();

	show(slist);


	pair<Strategy, int> test1 = pair<Strategy, int>(Strategy("cd9", 9), 1);
	pair<Strategy, int> test2 = pair<Strategy, int>(Strategy("c2d9", 9), 1);
	pair<Strategy, int> test3 = pair<Strategy, int>(Strategy("c3d9", 9), 1);
	slist.push_back(test1);
	slist.push_back(test2);
	slist.push_back(test3);
	show(slist);



	/*
	cout << "test weak_ptr lock" << endl;
	weak_ptr<int> wp;
	{
		shared_ptr<int> sp = shared_ptr<int>(new int(100));
		wp = sp;
		cout << boolalpha << wp.expired() << endl;
		cout << *(wp.lock()) << endl;

	}
	cout << boolalpha << wp.expired() << endl;
	cout << wp.lock() << endl;

	if (nullptr == wp.lock()) {
		cout << "nullptr!!!";
	} else {
		cout << "not nullptr!!";
	}
	cout << endl;
	*/

	/*
	cout << numeric_limits<unsigned int>::max() << endl;
	cout << numeric_limits<long>::max() << endl;
	cout << numeric_limits<unsigned long int>::max() << endl;
	cout << numeric_limits<unsigned long long int>::max() << endl;
	 */



}

