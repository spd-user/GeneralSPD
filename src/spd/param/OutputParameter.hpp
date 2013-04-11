/**
 * OutputParameter.hpp
 *
 * @date 2012/09/20
 * @author katsumata
 */

#ifndef OUTPUTPARAMETERETER_H_
#define OUTPUTPARAMETERETER_H_

#include <string>
#include <vector>
#include <tuple>
#include <memory>
#include "IShowParameter.hpp"

namespace spd {
namespace output {
class Output;
}
namespace param {

/**
 * 出力に関わるパラメタを保持するクラス
 */
class OutputParameter : public IShowParameter {

public:

	/**
	 * デフォルト値で初期化を行う
	 */
	OutputParameter();

	/**
	 * 出力ファイルのルートディレクトリを取得
	 * @return 出力ファイルのルートディレクトリ
	 */
	const std::string& getDirectory() const;

	/**
	 * 出力ファイルのルートディレクトリを設定
	 * @param directory 出力先のルートディレクトリ名
	 */
	void setDirectory(const std::string& directory);

	/**
	 * 出力方法と開始ステップ、終了ステップ、間隔のリストを取得
	 * @return 出力方法と開始ステップ、終了ステップ、間隔のリスト
	 */
	std::vector<std::tuple<std::shared_ptr<output::Output>, int, int, int>>& getOutputs();

	/**
	 * セルサイズを設定する
	 * @param[in] size セルサイズ
	 */
	void setCellSize(int size) {
		cellSize = size;
	}

	/**
	 * セルサイズを取得する
	 * @return セルサイズ
	 */
	int getCellSize() {
		return cellSize;
	}

	/**
	 * パラメタを出力する
	 * @param out 出力先
	 */
	void showParameter(std::ostream& out) const;

private:

	// デフォルト値
	const std::string DEFAULT_DIR = "/home/katsumata/documents/study";

	// パラメタの実態
	// 出力先ディレクトリ('/'で終わっていない)
	std::string directory;

	// 出力
	std::vector<std::tuple<std::shared_ptr<output::Output>, int, int, int>> outputs;

	// 画像の出力のセルサイズ
	int cellSize;
};

} /* namespace param */
} /* namespace spd */
#endif /* OUTPUTPARAMETERETER_H_ */
