/**
 * GEXF ファイルを作成する
 * version 0.1
 */

(function($) {
	
	/**
	 * プレイヤの設定を保持するクラス
	 */
	var Player = function(_settingId, _pre_act, _act, _pre_score, _score, _pre_str, _str) {
		var settingId = _settingId;
		var pre_action = _pre_act;
		var action = _act;
		var pre_score = _pre_score;
		var score = _score;
		var pre_strategy = _pre_str;
		var strategy = _str;
		
		this.getSettingId = function() {
			return settingId;
		};
		this.getPreAction = function() {
			return pre_action;
		};
		this.getAction = function() {
			return action;
		};
		this.getPreScore = function() {
			return pre_score;
		};
		this.getScore = function() {
			return score;
		};
		this.getPreStrategy = function() {
			return pre_strategy;
		};
		this.getStrategy = function() {
			return strategy;
		};
	};
	
	
	/**
	 * 読み込み完了時に実行
	 */
	$(function(){
		
		var all_parameter = {};
		
	
		// jquery ui の設定
		// -------------------------
		// 環境
		$("#environment_alert_area").hide();
		$("button").button();
		$("#environment").accordion({
			collapsible : true
		});
		$("#self_interaction").button().click(
				function(){
				if (this.checked) {
					$(this).button("option", "label", "Self interaction : ON");
				} else {
					$(this).button("option", "label", "Self interaction : OFF");
				}
				});
			
		// 戦略リストの追加
		$("#strategy_list").addInputArea();
		
		$("#apply_environment_btn").click(apply_environment);
		$("#edit_environment_btn").click(edit)
		
		// -------------------------
		// プレイヤのデフォルト
		$("#player_default").accordion({
			active : false,
			collapsible : true,
			disabled : true
		});
		
		// -------------------------
		// 個別の設定
		var stop = false;
		$(".player_settings_title")
		. click(function(event){
			if (stop) {
				event.stopImmediatePropagation ();
				event.preventDefault();
				stop = false;
			}
		});
		
		$("#player_settings")
		.accordion({
			header: '.player_settings_title',
			collapsible : true,
			active : false,
			disabled : true
		})
		.sortable({
			axis: 'y',
			handle: '.player_settings_title',
			stop: function () {
				stop = true;
			}
		})
		.addInputArea();
		
		// 設定のOn/Off の切り替えボタン
		$("#enable_settings").button("disable").click(function(){
			if ($("#player_settings").accordion("option", "disabled")) {
				
				$("#player_settings").accordion({disabled : false});
				$("#player_settings_add").button("enable");
				
				$("#enable_settings").button("option", "label", "Disable settings");
			} else {
				$("#player_settings").accordion({disabled : true});
				$("#player_settings_add").button("disable");
				
				$("#enable_settings").button("option", "label", "Enable settings");
				
			}
		});
		
		
		$("#player_settings_add").bind("click", "player_settings", add_accordion);
		
		// 作成
		$("#create_btn").click(create_gexf).button("disable");
		
		$("#info_area").hide();
		$("#alert_area").hide();
		
		// ダウンロード
		$("#download_btn").button().button("disable");
		

		// 環境を有効化し、入力を出来なくする
		function apply_environment() {
			
			// 環境の入力チェック
			var verify_environment = function() {
				$("#environment_alert_area").hide();
				
				var errStr = "";
				if ($("#unit").val() <= 0 ) {
					errStr += "<li>Unit が1以上でない</li>";
				}
				if ($("#step").val() <= 0) {
					errStr += "<li>Step が1以上でない</li>";
				}
				if ($("#s_cycle").val() <= 0) {
					errStr += "<li>Strategy update cycle が1以上でない</li>";
				}
				if ($("#payoff_r").val() == "") {
					errStr += "<li>Payoff R が空</li>";
				}
				if ($("#payoff_s").val() == "") {
					errStr += "<li>Payoff S が空</li>";
				}
				if ($("#payoff_t").val() == "") {
					errStr += "<li>Payoff T が空</li>";
				}
				if ($("#payoff_p").val() == "") {
					errStr += "<li>Payoff P が空</li>";
				}
				if ($(".environment-element.strategy")
						.filter(function(){
							return $(this).val() != "";
						})
						.length <= 0) {
					errStr += "<li>戦略が指定されていない</li>";
					
				}
				if ($("#genRand").val() < 0 || $("#genRand").val() == "") {
					errStr += "<li>Generated random number が0以上でない</li>";
				}
				
				if (errStr != "") {
					$("#environment_alert_area li").html(errStr);
					$("#environment_alert_area").show();
					return false;
				}
				return true;
			};
			
			if (!verify_environment()) {
				return;
			}
			
			$("#player_default").accordion({
				active : 0,
				disabled : false,
			});
			$("#player_settings").accordion({
				disabled : true,
			});
			$(".player_settings_title > .player_settings_del").removeClass("hidden-del-img");
			$(".player_settings_title > .player_settings_del").addClass("visible-del-img");
			
			$("#apply_environment_btn").button("disable");
			$("#edit_environment_btn").button("enable");
			
			$("button.environment-element, input.environment-element[type=checkbox]").button("disable");
			$(".environment-element").prop("disabled", true);
			$(".strategy_list_del").addClass("hidden-del-img");
			$(".strategy_list_del").removeClass("visible-del-img");
			
			// パラメタの適用
			all_parameter.step = $("#step").val();
			all_parameter.SUC = $("#s_cycle").val();
			all_parameter.topology = $("#topology").val();
			all_parameter.rVal = $("#payoff_r").val();
			all_parameter.sVal = $("#payoff_s").val();
			all_parameter.tVal = $("#payoff_t").val();
			all_parameter.pVal = $("#payoff_p").val();
			if ($("#self_intefaction").val()) {
				all_parameter.SI = 1;
			} else {
				all_parameter.SI = 0;
			}
			all_parameter.strategies = [];
			$(".environment-element.strategy").each(function(){
				if ($(this).val() != "") {
					all_parameter.strategies.push($(this).val());
				}
			});
			
			// 戦略のセレクトボタンの中身を書き換える
			$("select.strategy").html(
					(function () {
						var option_values = "";
						all_parameter.strategies.forEach(function(element){
							option_values = option_values + "<option value=" 
								+ element + ">" + element + "</option>";
						});
						return option_values;
					}())
			);
			
			all_parameter.rand = $("#genRand").val();
			
			$("#create_btn").button("enable");
			$("#enable_settings").button("enable");
		};

		// 環境の再編集
		function edit() {
			$("#environment_alert_area").hide();
			
			$("#player_default").accordion({
				active : false,
				disabled : true,
			});
			$("#player_settings").accordion({
				active : false,
				disabled : true,
			});
			$("#enable_settings").button("disable");
			
			$(".player_settings_title > .player_settings_del").removeClass("visible-del-img");
			$(".player_settings_title > .player_settings_del").addClass("hidden-del-img");
			
			$("#player_settings_add").button("disable");
			
			$("#apply_environment_btn").button("enable");
			$("#edit_environment_btn").button("disable");

			$("button.environment-element, input.environment-element[type=checkbox]").button("enable");
			$(".environment-element").prop("disabled", false);
			$(".strategy_list_del").addClass("visible-del-img");
			$(".strategy_list_del").removeClass("hidden-del-img");
			
			$("#create_btn").button("disable");
		}
		
		// accordion を追加したときの追処理
		function add_accordion(name){
			
			$("#" + name.data).accordion("refresh")
			.accordion({
				collapsible: true,
				active : $('.' + name.data + "_contens").length -1
			});
		};
		
		// gexfファイルを作成
		function create_gexf() {
			var all_player = [];
			all_player.length = $("#unit").val();
			
			default_setting = {};
			
			(function load_settings(){
				player_settings.length = 0;

				$("#info_area").hide();
				$("#alert_area").hide();
			
				var settings = $("#player_settings .player_settings_contens");
				
				var info_text = "";
				var alert_text = "";
				
				if (!($("#player_settings").accordion("option", "disabled"))) {

					settings.each(
						function(index) {
							var setting_id = index + 1;

							var targets = [];

							var input_targets = $(this)
							.children("#target_range:text")
							.val()
							.replace(/\s/g, "")
							.split(",");

							// 設定のターゲット情報を綺麗にする
							var correct_targets = input_targets.filter(function(element){
								return (/^[0-9]+(-[0-9]+)?$/.test(element));
							});

							$(this).children("#target_range:text").val(correct_targets.toString());

							if (input_targets.length != correct_targets.length) {
								info_text += "<li>Setting " + setting_id + " : Target を ' "
								+ input_targets + " ' から ' " + correct_targets + " ' へ変更</li>"
							}

							if (correct_targets.length < 1) {
								alert_text += "<li>Setting " + setting_id + " : Target が空</li>"
							}

							var _pre_act = $(this).children("#pre_action").val();
							var _act = $(this).children("#action").val();

							var _pre_score = $(this).children("#pre_score").val();
							var _score = $(this).children("#score").val();

							var _pre_str = $(this).children("#pre_strategy").val();
							var _str = $(this).children("#strategy").val();

							// プレイヤの設定
							var player_setting = new Player(
									setting_id, _pre_act, _act, _pre_score, _score, _pre_str, _str);
							
							// id番号のプレイヤの設定をplayer にする
							var addPlayer = function(id, player) {
								if (all_player.length <= id) {
									info_text += "<li>Setting " + setting_id + " : Target の設定で" +
											"プレイヤ数を越える対象が存在した</li>";
								} else if (!all_player[id]) {
									all_player[id] = player;
								} else {
									info_text += "<li>Setting " + setting_id + " : プレイヤ(" + id + 
									")に対する設定が Setting " + all_player[id].getSettingId() + 
									"と重複するため当該プレイヤへは無視</li>";
								}
							};

							// ターゲットユニットの文字列を解析する
							correct_targets.forEach(
									function(val){
										if (/^[0-9]+$/.test(val)) {
											// 数値だけのとき
											addPlayer(val, player_setting);
										} else {
											// 範囲指定が期待されるとき
											var range = val.split("-");
											if (range.length != 2) {
												alert_text += "<li>Setting " + setting_id + " : Target の設定" +
												"で未定義のエラー</li>";
												return;
											} else if ((range[0] | 0) > (range[1] | 0)) {
												alert_text += "<li>Setting " + setting_id + " : Target の" +
												"範囲選択でエラー(" + val + ")</li>";
											} else {
												for (var i = (range[0] | 0); i < (range[1] | 0); i++) {
													addPlayer(i, player_setting);
												}
											}
										}
									});
						});
				}
				
				// 残りをデフォルトで埋める
				var default_setting = new Player(-1, 
						$("#player_default_contents #default_pre_action").val(), 
						$("#player_default_contents #default_action").val(), 
						$("#player_default_contents #default_pre_score").val(), 
						$("#player_default_contents #default_score").val(),
						$("#player_default_contents #default_pre_strategy").val(),
						$("#player_default_contents #default_strategy").val());
				var i;
				for (i = 0; i < all_player.length; i++) {
					if (!all_player[i]) {
						all_player[i] = default_setting;
					}
				}
				
				if (info_text != "") {
					$("#info_area ul").html(info_text);
					$("#info_area").show();
				}

				if (alert_text != "") {
					$("#alert_area ul").html(alert_text);
					$("#alert_area").show();
					
					return;
				}
			}());
			
			(function create_node() {
				var digit = ((Math.log(all_player.length) / Math.log(10)) | 0) + 1; 

				all_parameter.nodes = all_player.map(function(value, index){
					return {
							id : (Array(digit).join('0') + index).slice(-1 * (digit)),
							preAction : value.getPreAction(),
							action : value.getAction(),
							preScore : value.getPreScore(),
							score : value.getScore(),
							preStrategy : value.getPreStrategy(),
							strategy : value.getStrategy()
					};
				});
			}());
			
			var creates_connections = (function() {
				var digit = ((Math.log(all_player.length) / Math.log(10)) | 0) + 1; 
				
				var edgeId = 0;
				var i;
				
				try {
					// トポロジによって場合分け
					var topology;
					if ($("#topology").val() === "Moore") {
						topology = moore(all_player.length);
					} else {
						$("#alert_area ul").html("<li>未知のトポロジであったため、プレイヤ接続の構築が不可</li>");
					}

					all_parameter.edges = [];

					for (i = 0; i < all_player.length; i++) {

						topology(i).forEach(function(target) {
							all_parameter.edges.push(
									{
										id : edgeId,
										source : (Array(digit).join('0') + i).slice(-1 * (digit)),
										target : (Array(digit).join('0') + target).slice(-1 * (digit))
									});
							edgeId++;
						})
					}
				
				} catch (e) {
					alert("Error : " + e);
					return false;
				}
				
				return true;
			}());
			
			if (!creates_connections) {
				return;
			}
			var template = Handlebars.compile($("#result").html());
			
			var html = template(all_parameter);
			
			var blob = new Blob([html]);
			
			var url = '<a id="download_btn" href="' + URL.createObjectURL(blob) 
						+ '" target="_brank" download="state.gexf">Download</a>';
			
			$("#download_btn").html(url).button("destroy").button().button("enable");
		};	
	});
})(jQuery);
