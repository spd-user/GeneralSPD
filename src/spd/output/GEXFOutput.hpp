/**
 * GEXFOutput.hpp
 *
 * @date 2013/01/24
 * @author katsumata
 */

#ifndef GEXFOUTPUT_H_
#define GEXFOUTPUT_H_

#include <sstream>
#include <fstream>

#include "Output.hpp"

namespace spd {
namespace output {

/**
 * 盤面のGEXF v1.2 の出力を行うクラス
 *
 * @note プロパティを扱うルールへの対応は未確認
 */
class GEXFOutput : public Output {
public:

	/**
	 * 空間の情報を出力する
	 * @param[in, out] space 空間
	 * @return 圧縮が必要なので second がtrueで、firstにファイル名
	 */
	std::pair<std::string, bool> output(spd::core::Space& space);

	/**
	 * 初期化
	 *
	 * @par
	 * configファイルの作成
	 *
	 * @param[in, out] space 空間
	 * @param[in, out] param パラメタ
	 */
	void init(spd::core::Space& space, spd::param::Parameter& param);

	/**
	 * クラス情報の文字出力
	 */
	std::string toString() const {
		return "gexf";
	}

private:

	/**
	 * 出力ディレクトリ
	 */
	const std::string DIR  = "/gexf";

	/**
	 * ファイルの接頭辞
	 */
	const std::string PREFIX = "/spd_gexf_";

	/**
	 * ファイルの接尾辞
	 */
	const std::string SUFFIX = ".gexf";

	// 属性idと、名前、種類の出力
	void writeNodeAttr(std::ofstream& outputfile, int id, std::string name, std::string type);

	// 属性のid とその値を出力
	void writeNodeValue(std::ofstream& outputfile, int attrId, std::string value);

	// エッジの出力
	void writeEdge(std::ofstream& outputfile, int edgeId, std::string source, std::string target);

	// マニピュレータの設定
	void setZeroPadding(std::ostringstream& oss, int width, int val);

};

} /* namespace output */
} /* namespace spd */
#endif /* GEXFOUTPUT_H_ */
