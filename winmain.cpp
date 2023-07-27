#include <DxLib.h>
#include <string>

int raref = 0;//レアフラグ
int deffectf = 0;//ダメージエフェクトフラグ
int turnf = 0;//アイテムが無い状態で選択したときに次のターンに進めないフラグ
int keysound;//曲を入れる変数
int backsound;
int backsoundrea;
int backsoundrea2;
int soundstop = 0;
int graphstop = 0;//ダメージエフェクトの時間を計るやつ
int lvcount = 0;//レベルアップフラグ
int nazonoiti[2] = {};//レベルアップ表記の微調整（最初だけ

int UIene;
int UIp;
int UIp2;

int lvupsound;

int deathcount[2] = {};//一人づつのデスカウント
int countdeathsum=0;//合計デスカウント

int pop3 = 0;
int pop2 = 0;
bool stop = false;
int backgraph = 0;

enum GameStat
{
	BattleStart,
	BattleStartDisp,
	PlayerSelect,
	PlayerSelectEnd,
	MagicSelect,
	MagicSelectEnd,
	Toolselect,
	ToolselectEnd,
	PlayerAtack,
	AtackEnd,
	EnemyJudge,
	EnemySelect,
	EnemyAtack,
	EnemyAtackDisp,
	PlayerJudge,
	GameOver,
	GameOverEnd,
	Result,
	Levelup,
	End
};

enum MagicId
{
	Maya,
	Photosho,
	Unity,
	VisualStudio,
	Lagnarok,
	taberu,
	reaattak,

	MagicMax
};
enum MagicType
{
	Atack,//攻撃魔法
	Heal,//回復魔法
	Buff,//強化魔法
	harm,
	downheal,

	magictypeMax
};
enum ToolId
{
	grass,
	grassmed,
	grasslar,
	MPpotion,
	MPpotionmed,
	MPpotionlar,

	ToolMax
};
enum ToolType
{
	Hpheal,
	Mpheal,

	tooltypeMax
};

//魔法構造体
struct MagicData
{
	std::string name = "";
	std::string message = "";
	std::string setumei = "";
	double power = 0;
	int cost = 0;
	double harmd = 0;
	MagicType type = Atack;
	bool enable = false;//有効フラグ
};

//道具構造体
struct ToolData
{
	std::string name = "";
	std::string message = "";
	std::string setumei = "";
	double power = 0;
	int cost = 0;
	int harmd = 0;
	int graph = 0;//画像
	ToolType type = Hpheal;
	bool enable = false;//有効フラグ
};

enum EnemyId
{
	Tachiki,
	Sakuma,
	Yamada,
	Shibata,
	Ueda,
	rea1,
	rea2,
	EnemyMax
};
//敵構造体
struct Enemy
{
	std::string name = "";//名前
	int exp = 0;
	int hp = 0;
	int mp = 0;
	int ap = 0;
	int graphic = 0;//画像
	MagicData magic[1];//敵の魔法は最大一種類
};
enum CharacterId
{
	Chino,
	ChinoKen,
	CharacterMax
};
//プレイヤー構造体
struct Player
{
	std::string name = "";
	int exp = 0;
	int hp = 0;
	int mp = 0;
	int ap = 0;
	int item[ToolMax] = {};
	MagicData magic[3];//魔法は３種類
	ToolData tool[6];
};

const int PartyMax = 2;//パーティ2人
bool keystop = false;
int GameStatus = BattleStart;
int SelectNo = 0;//選択肢


ToolData toolTable[ToolMax];
MagicData MagicTable[MagicMax];//魔法情報
Enemy EnemyTable[EnemyMax];//敵情報
Player CharacterTable[CharacterMax];

Enemy BattleEnemy;//戦闘中の敵
Player Party[PartyMax];//パーティ(一人)
std::string MessageText = "";
std::string StatusText[PartyMax] = { "","" };//ステータスメッセージ
std::string debugstatustext[PartyMax] = { "","" };//DebugStatus(Player
std::string EnemyStatusText = "";
std::string debugEnemyStatusText = "";//DebugStatus(Enemy


const int MenuMax = 7;//メニュー最大数

std::string SetumeiText[MenuMax] = {"","","","","","",""};
std::string SelectText[MenuMax] = { "","","","","","",""};
int toolgraph[6] = {};
//キャラの経験値
int charexp[2] = {};
//キャラのレベル
int charlv[2] = {};
//上がり具合を入れる箱
int charlvUP[2] = {};
//基本経験値テーブル
int exptable[100] = {};

//何人目のキャラまで行ったか
int SelectPlayerNo = 0;

//最初に1回呼ばれる処理
void Init()
{
	//音設定
	keysound = LoadSoundMem("maoudamasii\\poon.mp3");
	ChangeVolumeSoundMem(255*80/100, keysound); 
	backsound = LoadSoundMem("BGM\\n2.mp3");
	ChangeVolumeSoundMem(255 * 50 / 100, backsound);
	backsoundrea = LoadSoundMem("BGM\\n136.mp3");
	ChangeVolumeSoundMem(255 * 70 / 100, backsoundrea);
	backsoundrea2 = LoadSoundMem("BGM\\n65.mp3");
	ChangeVolumeSoundMem(255 * 70 / 100, backsoundrea2);
	lvupsound = LoadSoundMem("maoudamasii\\lv.mp3");
	ChangeVolumeSoundMem(255 * 70 / 100, lvupsound);


	backgraph = LoadGraph("back.png");
	UIene = LoadGraph("UIene.png");
	UIp = LoadGraph("UIpl.png");
	//UIp2 = LoadGraph("UIpl2.png");


	//魔法情報を初期化
	MagicTable[Maya].name = "トルネード";
	MagicTable[Maya].message = "激しい竜巻が襲う！";
	MagicTable[Maya].setumei = "激しい竜巻を巻き起こして敵に500のダメージ！";
	MagicTable[Maya].cost = 20;
	MagicTable[Maya].power = 500;//450
	MagicTable[Maya].type = Atack;
	MagicTable[Maya].enable = true;

	//魔法情報を初期化
	MagicTable[Photosho].name = "リカバーライト";
	MagicTable[Photosho].message = "癒しの光が体を包む…！";
	MagicTable[Photosho].setumei = "自身の体力を250回復！";
	MagicTable[Photosho].cost = 10;
	MagicTable[Photosho].power = 250;
	MagicTable[Photosho].type = Heal;
	MagicTable[Photosho].enable = true;

	//魔法情報を初期化
	MagicTable[Unity].name = "パワーブースト";
	MagicTable[Unity].message = "体に力がみなぎってきた！";
	MagicTable[Unity].setumei = "自身の攻撃効率が1.3倍に！";
	MagicTable[Unity].cost = 20;
	MagicTable[Unity].power = 1.3;
	MagicTable[Unity].type = Buff;
	MagicTable[Unity].enable = true;

	//魔法情報を初期化
	MagicTable[VisualStudio].name = "ヴィジュアルスタジオ";
	MagicTable[VisualStudio].message = "バグが発生した！！";
	MagicTable[VisualStudio].setumei = "例外がスローされました。識別子””が設定されていません";
	MagicTable[VisualStudio].cost = 10;
	MagicTable[VisualStudio].power = 0.7;
	MagicTable[VisualStudio].type = Buff;
	MagicTable[VisualStudio].enable = true;

	//魔法情報を初期化
	MagicTable[Lagnarok].name = "ヘルプロミネンス";
	MagicTable[Lagnarok].message = "身を焼く程の炎を纏い、全身全霊の攻撃！！！";
	MagicTable[Lagnarok].setumei = "自身の体力を1000消費する代わりに\n敵に1000の大ダメージ！！！";
	MagicTable[Lagnarok].cost = 50;
	MagicTable[Lagnarok].power = 1000;
	MagicTable[Lagnarok].harmd = 1000;//自傷ダメージ（harmd
	MagicTable[Lagnarok].type = harm;
	MagicTable[Lagnarok].enable = true;

	//魔法情報を初期化
	MagicTable[taberu].name = "ロストヒール";

	MagicTable[taberu].message = "攻撃力を犠牲に体力を回復！";
	MagicTable[taberu].setumei = "回復する代わりに攻撃力が２割低下";
	MagicTable[taberu].cost = 20;
	MagicTable[taberu].power = 500;
	MagicTable[taberu].harmd = 0.8;
	MagicTable[taberu].type = downheal;
	MagicTable[taberu].enable = true;

	//魔法情報を初期化
	MagicTable[reaattak].name = "ウォータースパイラル";
	MagicTable[reaattak].message = "凄まじい激流が襲い掛かる！";
	MagicTable[reaattak].setumei = "おそらくここは味方の技の時しか見れない";
	MagicTable[reaattak].cost = 30;
	MagicTable[reaattak].power = 700;
	MagicTable[reaattak].type = Atack;
	MagicTable[reaattak].enable = true;

	//道具情報を初期化
	toolTable[grass].name = "薬草";
	toolTable[grass].message = "体力を小回復！";
	toolTable[grass].setumei = "体を癒すがマズイ。良薬は口に苦し。";
	toolTable[grass].cost = 1;
	toolTable[grass].power = 100;
	toolTable[grass].harmd = 0;
	toolTable[grass].type = Hpheal;
	toolTable[grass].graph = LoadGraph("hana.png");
	toolTable[grass].enable = true;

	toolTable[grassmed].name = "回復薬";
	toolTable[grassmed].message = "体力を中回復！";
	toolTable[grassmed].setumei = "薬草より効果がいい。味も美味い！";
	toolTable[grassmed].cost = 1;
	toolTable[grassmed].power = 300;
	toolTable[grassmed].harmd = 0;
	toolTable[grassmed].type = Hpheal;
	toolTable[grassmed].graph = LoadGraph("po.png");
	toolTable[grassmed].enable = true;

	toolTable[grasslar ].name = "強化回復薬";
	toolTable[grasslar].message = "体力を大回復！";
	toolTable[grasslar].setumei = "これさえ飲めば大丈夫！めっちゃ美味い！！！";
	toolTable[grasslar].cost = 1;
	toolTable[grasslar].power = 500;
	toolTable[grasslar].harmd = 0;
	toolTable[grasslar].type = Hpheal;
	toolTable[grasslar].graph = LoadGraph("kaihuku.png");
	toolTable[grasslar].enable = true;

	toolTable[MPpotion].name = "マナポーション";
	toolTable[MPpotion].message = "MPを小回復！";
	toolTable[MPpotion].setumei = "MP維持に欠かせない。ソーダ味。";
	toolTable[MPpotion].cost = 1;
	toolTable[MPpotion].power = 15;
	toolTable[MPpotion].harmd = 0;
	toolTable[MPpotion].type = Mpheal;
	toolTable[MPpotion].graph = LoadGraph("soda.png");
	toolTable[MPpotion].enable = true;

	toolTable[MPpotionmed].name = "マナポーション中";
	toolTable[MPpotionmed].message = "MPを中回復！";
	toolTable[MPpotionmed].setumei = "ちょっと多めのMPチャージ。これもソーダ味。";
	toolTable[MPpotionmed].cost = 1;
	toolTable[MPpotionmed].power = 25;
	toolTable[MPpotionmed].harmd = 0;
	toolTable[MPpotionmed].type = Mpheal;
	toolTable[MPpotionmed].graph = LoadGraph("manat.png");
	toolTable[MPpotionmed].enable = true;
	
	toolTable[MPpotionlar].name = "マナポーション大";
	toolTable[MPpotionlar].message = "MPを大回復！";
	toolTable[MPpotionlar].setumei = "MPを大量チャージ！やっぱりソーダ味。";
	toolTable[MPpotionlar].cost = 1;
	toolTable[MPpotionlar].power = 50;
	toolTable[MPpotionlar].harmd = 0;
	toolTable[MPpotionlar].type = Mpheal;
	toolTable[MPpotionlar].graph = LoadGraph("manad.png");
	toolTable[MPpotionlar].enable = true;

	//敵情報を初期化
	EnemyTable[Tachiki].name = "フワりん";
	EnemyTable[Tachiki].exp = 180;//レベル
	EnemyTable[Tachiki].hp = 2000;
	EnemyTable[Tachiki].ap = 250;
	EnemyTable[Tachiki].mp = 30;
	EnemyTable[Tachiki].magic[0] = MagicTable[Maya];
	EnemyTable[Tachiki].graphic = LoadGraph("digd.png");

	EnemyTable[Sakuma].name = "ツインビー";
	EnemyTable[Sakuma].exp= 180;
	EnemyTable[Sakuma].hp = 2100;
	EnemyTable[Sakuma].ap = 450;
	EnemyTable[Sakuma].mp = 40;
	EnemyTable[Sakuma].magic[0] = MagicTable[Unity];
	EnemyTable[Sakuma].graphic = LoadGraph("hati.png");

	EnemyTable[Yamada].name = "イビルモール";
	EnemyTable[Yamada].exp = 300;
	EnemyTable[Yamada].hp = 2300;
	EnemyTable[Yamada].ap = 450;
	EnemyTable[Yamada].mp = 20;
	EnemyTable[Yamada].magic[0] = MagicTable[Unity];
	EnemyTable[Yamada].graphic = LoadGraph("mohu.png");

	EnemyTable[Shibata].name = "フレアワイバーン";
	EnemyTable[Shibata].exp = 480;
	EnemyTable[Shibata].hp = 2600;
	EnemyTable[Shibata].ap = 450;
	EnemyTable[Shibata].mp = 100;
	EnemyTable[Shibata].magic[0] = MagicTable[Maya];
	EnemyTable[Shibata].graphic = LoadGraph("tutinoko.png");

	EnemyTable[Ueda].name = "スラたろう";
	EnemyTable[Ueda].exp = 60;
	EnemyTable[Ueda].hp = 1700;
	EnemyTable[Ueda].ap = 250;
	EnemyTable[Ueda].mp = 10;
	EnemyTable[Ueda].magic[0] = MagicTable[Photosho];
	EnemyTable[Ueda].graphic = LoadGraph("surataro.png");

	//レア敵１
	EnemyTable[rea1].name = "アクアデビル";
	EnemyTable[rea1].exp = 1200;
	EnemyTable[rea1].hp = 2900;
	EnemyTable[rea1].ap = 500;
	EnemyTable[rea1].mp = 240;
	EnemyTable[rea1].magic[0] = MagicTable[reaattak];
	EnemyTable[rea1].graphic = LoadGraph("ryu.png");
	//レア敵２
	EnemyTable[rea2].name = "ラグナーク";
	EnemyTable[rea2].exp = 1800;
	EnemyTable[rea2].hp = 3500;
	EnemyTable[rea2].ap = 600;
	EnemyTable[rea2].mp = 400;
	EnemyTable[rea2].magic[0] = MagicTable[Lagnarok];
	EnemyTable[rea2].graphic = LoadGraph("hebi.png");

	//キャラクターテーブル初期化
	CharacterTable[Chino].name = "剣士";
	CharacterTable[Chino].exp = 60;
	CharacterTable[Chino].hp = 1300;//800
	CharacterTable[Chino].ap = 250;
	CharacterTable[Chino].mp = 60;
	CharacterTable[Chino].magic[0]= MagicTable[Unity];
	CharacterTable[Chino].magic[1] = MagicTable[Photosho];
	CharacterTable[Chino].magic[2] = MagicTable[Lagnarok];
	CharacterTable[Chino].tool[0] = toolTable[grass];
	CharacterTable[Chino].tool[1] = toolTable[grassmed];
	CharacterTable[Chino].tool[2] = toolTable[grasslar];
	CharacterTable[Chino].tool[3] = toolTable[MPpotion];
	CharacterTable[Chino].tool[4] = toolTable[MPpotionmed];
	CharacterTable[Chino].tool[5] = toolTable[MPpotionlar];
	CharacterTable[Chino].item[grass] = 5;						//アイテムの個数
	CharacterTable[Chino].item[grassmed] = 3;
	CharacterTable[Chino].item[grasslar] = 1;
	CharacterTable[Chino].item[MPpotion] = 5;
	CharacterTable[Chino].item[MPpotionmed] =3;
	CharacterTable[Chino].item[MPpotionlar] = 1;

	CharacterTable[ChinoKen].name = "魔導士";
	CharacterTable[ChinoKen].exp = 60;
	CharacterTable[ChinoKen].hp = 1800;//1200
	CharacterTable[ChinoKen].ap = 150;
	CharacterTable[ChinoKen].mp = 100;
	CharacterTable[ChinoKen].magic[0] = MagicTable[Maya];
	CharacterTable[ChinoKen].magic[1] = MagicTable[Photosho];
	CharacterTable[ChinoKen].magic[2] = MagicTable[taberu];
	CharacterTable[ChinoKen].tool[0] = toolTable[grass];
	CharacterTable[ChinoKen].tool[1] = toolTable[grassmed];
	CharacterTable[ChinoKen].tool[2] = toolTable[grasslar];
	CharacterTable[ChinoKen].tool[3] = toolTable[MPpotion];
	CharacterTable[ChinoKen].tool[4] = toolTable[MPpotionmed];
	CharacterTable[ChinoKen].tool[5] = toolTable[MPpotionlar];
	CharacterTable[ChinoKen].item[grass] = 5;					//アイテムの個数
	CharacterTable[ChinoKen].item[grassmed] = 3;
	CharacterTable[ChinoKen].item[grasslar] = 1;
	CharacterTable[ChinoKen].item[MPpotion] = 5;
	CharacterTable[ChinoKen].item[MPpotionmed] = 3;
	CharacterTable[ChinoKen].item[MPpotionlar] = 1;

	charlv[0] = 1;
	charlv[1] = 1;
}

//最大50lev   500,000exp
//経験値の初期化
void LevelExp() {
	exptable[0] = 0;
	exptable[1] = 10;
	exptable[2] = 20;
	exptable[3] = 30;
	exptable[4] = 45;
	exptable[5] = 60;
	exptable[6] = 80;
	exptable[7] = 90;
	exptable[8] = 105;
	exptable[9] = 135;
	exptable[10] = 200;
	exptable[11] = 240;
	exptable[12] = 240;
	exptable[13] = 305;
	exptable[14] = 365;
	exptable[15] = 435;
	exptable[16] = 515;
	exptable[17] = 605;
	exptable[18] = 700;
	exptable[19] = 785;
	exptable[20] = 885;
	exptable[21] = 1070;
	exptable[22] = 1195;
	exptable[23] = 1335;
	exptable[24] = 1490;
	exptable[25] = 1660;
	exptable[26] = 1945;
	exptable[27] = 2150;
	exptable[28] = 2375;
	exptable[29] = 2620;
	exptable[30] = 2885;
	exptable[31] = 3750;
	exptable[32] = 4500;
	exptable[33] = 5400;
	exptable[34] = 6480;
	exptable[35] = 7775;
	exptable[36] = 9675;
	exptable[37] = 11170;
	exptable[38] = 12845;
	exptable[39] = 14770;
	exptable[40] = 16985;
	exptable[41] = 20380;
	exptable[42] = 22415;
	exptable[43] = 24655;
	exptable[44] = 27210;
	exptable[45] = 29830;
	exptable[46] = 35795;
	exptable[47] = 39370;
	exptable[48] = 43305;
	exptable[49] = 47635;
	exptable[50] = 52000;

	/*for (int i = 0; i < 101; i++) {
		for (int j = 0; j < 2; j++) {
			if (charexp[j] > i * 1 && charexp[j] > i * 20) {
				lvcount = i;
				charlv[j] = i;
			}
		}
	}*/

	/*for (int i = 0; i < 2; i++) {
		if (charexp[i] <= 0 && 10 > charexp[i]) {
			charlv[i] = 1; 
			lvcount++;
		} 
		if (charexp[i] >= 10 && 20 > charexp[i]) {
			charlv[i] = 2; 
			lvcount++;
		} 
		if (charexp[i] >= 20 && 30 > charexp[i]) {
			charlv[i] = 3; 
			lvcount++;
		}
		if (charexp[i] >= 30 && 45 > charexp[i]) {
			charlv[i] = 4; 
			lvcount++;
		} if (charexp[i] >= 45 && 60 > charexp[i]) {
			charlv[i] = 5; 
			lvcount++;
		} if (charexp[i] >= 60 && 80 > charexp[i]) {
			charlv[i] = 6; 
			lvcount++;
		} if (charexp[i] >= 80 && 90 > charexp[i]) {
			charlv[i] = 7; 
			lvcount++;
		} if (charexp[i] >= 90 && 105 > charexp[i]) {
			charlv[i] = 8;
			lvcount++;
		} if (charexp[i] >= 105 && 135 > charexp[i]) {
			charlv[i] = 9;
			lvcount++;
		} if (charexp[i] >= 135 && 200 > charexp[i]) {
			charlv[i] = 10;
			lvcount++;
		} 
		if (charexp[i] >= 200 && 240 > charexp[i]) {
			charlv[i] = 11;
			lvcount++;
		}
		if (charexp[i] >= 240 && 305 > charexp[i]) {
			charlv[i] = 12;
			lvcount++;
		} 
		if (charexp[i] >= 305 && 365 > charexp[i]) {
			charlv[i] = 13; 
			lvcount++;
		} 
		if (charexp[i] >= 362 && 435 > charexp[i]) {
			charlv[i] = 14; 
			lvcount++;
		} 
		if (charexp[i] >= 435 && 515 > charexp[i]) {
			charlv[i] = 15; 
			lvcount++;
		} 
		if (charexp[i] >= 515 && 605 > charexp[i]) {
			charlv[i] = 16;
			lvcount++;
		} 
		if (charexp[i] >= 605 && 700 > charexp[i]) {
			charlv[i] = 17; 
			lvcount++;
		} if (charexp[i]>= 700 && 785 > charexp[i]) {
			charlv[i] = 18;
			lvcount++;
		} if (charexp[i] >= 780 && 885 > charexp[i]) {
			charlv[i] = 19;
			lvcount++;
		} if (charexp[i] >= 885 && 1070 > charexp[i]) {
			charlv[i] = 20;
			lvcount++;
		} if (charexp[i] >= 1070 && 1195 > charexp[i]) {
			charlv[i] = 21;
			lvcount++;
		} if (charexp[i] >= 1195 && 1335 > charexp[i]) {
			charlv[i] = 22; 
			lvcount++;
		} if (charexp[i] >= 1335 && 1490 > charexp[i]) {
			charlv[i] = 23; 
			lvcount++;
		} if (charexp[i] >= 1490 && 1660 > charexp[i]) {
			charlv[i] = 24; 
			lvcount++;
		} if (charexp[i] >= 1660 && 1945 > charexp[i]) {
			charlv[i] = 25; 
			lvcount++;
		} if (charexp[i] >= 1945 && 2150 > charexp[i]) {
			charlv[i] = 26; 
			lvcount++;
		} if (charexp[i] >= 2150 && 2375 > charexp[i]) {
			charlv[i] = 27; 
			lvcount++;
		} if (charexp[i] >= 2375 && 2620 > charexp[i]) {
			charlv[i] = 28; 
			lvcount++;
		} if (charexp[i] >= 2620 && 2885 > charexp[i]) {
			charlv[i] = 29; 
			lvcount++;
		} if (charexp[i] >= 2885 && 3750 > charexp[i]) {
			charlv[i] = 30; 
			lvcount++;
		} if (charexp[i] >= 3750 && 4500 > charexp[i]) {
			charlv[i] = 31;
			lvcount++;
		} if (charexp[i] >= 4500 && 5400 > charexp[i]) {
			charlv[i] = 32;
			lvcount++;
		} if (charexp[i] >= 5400 && 6480 > charexp[i]) {
			charlv[i] = 33; 
			lvcount++;
		} if (charexp[i] >= 6480 && 7775 > charexp[i]) {
			charlv[i] = 34; 
			lvcount++;
		} if (charexp[i] >= 7775 && 9675 > charexp[i]) {
			charlv[i] = 35;
			lvcount++;
		} if (charexp[i] >= 9675 && 11170 > charexp[i]) {
			charlv[i] = 36; 
			lvcount++;
		} if (charexp[i] >= 11170 && 12845 > charexp[i]) {
			charlv[i] = 37;
			lvcount++;
		} if (charexp[i] >= 12845 && 14770 > charexp[i]) {
			charlv[i] = 38; 
			lvcount++;
		} if (charexp[i] >= 14770 && 16985 > charexp[i]) {
			charlv[i] = 39; 
			lvcount++;
		} if (charexp[i] >= 16985 && 20380 > charexp[i]) {
			charlv[i] = 40;
			lvcount++;
		} if (charexp[i] >= 20380 && 22415 > charexp[i]) {
			charlv[i] = 41; 
			lvcount++;
		} if (charexp[i] >= 22415 && 24655 > charexp[i]) {
			charlv[i] = 42;
			lvcount++;
		} if (charexp[i] >= 24655 && 27120 > charexp[i]) {
			charlv[i] = 43; 
			lvcount++;
		} if (charexp[i] >= 27120 && 29830 > charexp[i]) {
			charlv[i] = 44;
			lvcount++;
		} if (charexp[i] >= 29830 && 35795 > charexp[i]) {
			charlv[i] = 45; 
			lvcount++;
		} if (charexp[i] >= 35795 && 39370 > charexp[i]) {
			charlv[i] = 46; 
			lvcount++;
		} if (charexp[i] >= 39370 && 39370 > charexp[i]) {
			charlv[i] = 47; 
			lvcount++;
		} if (charexp[i] >= 39370 && 43305 > charexp[i]) {
			charlv[i] = 48;
			lvcount++;
		} if (charexp[i] >= 43305 && 47635 > charexp[i]) {
			charlv[i] = 49; 
			lvcount++;
		} if (charexp[i] >= 47635 && 52000 > charexp[i]) {
			charlv[i] = 50; 
			lvcount++;
		}
	}*/
}
void debug() {
	if (CheckHitKey(KEY_INPUT_NUMPAD3) == 1) {
		debugstatustext[SelectPlayerNo] = "Name/" + Party[0].name + "\n" + "Lv/" + std::to_string(charlv[0]) + "\n" + "HP/" + std::to_string(Party[0].hp) + "\n" + "MP/" + std::to_string(Party[0].mp) + "\n" + "AP/" + std::to_string(Party[0].ap) + "\n"+ "\n"
			+ "Name/" + Party[1].name + "\n" + "Lv/" + std::to_string(charlv[1]) + "\n" + "HP/" + std::to_string(Party[1].hp) + "\n" + "MP/" + std::to_string(Party[1].mp) + "\n" + "AP/" + std::to_string(Party[1].ap) + "\n";
		pop3 = 1;
	}else pop3 = 0;

	if (CheckHitKey(KEY_INPUT_NUMPAD2) == 1) {
		debugEnemyStatusText = "Name/" + BattleEnemy.name + "\n" + "Lv/" + std::to_string(BattleEnemy.exp / 60) + "\n" + "HP/" + std::to_string(BattleEnemy.hp) + "\n" + "MP/" + std::to_string(BattleEnemy.mp) + "\n" + "AP/" + std::to_string(BattleEnemy.ap) + "\n";
		pop2 = 1;
	}else pop2 = 0;

	if (CheckHitKey(KEY_INPUT_NUMPAD1) == 1 && stop == false) {
		stop = true;
		GameStatus = BattleStart;
	}
	if (CheckHitKey(KEY_INPUT_NUMPAD1) == 0) {
		stop = false;

	}
	if (CheckHitKey(KEY_INPUT_NUMPAD4) == 1) {
		BattleEnemy.hp = 0;
	}
	if (CheckHitKey(KEY_INPUT_NUMPAD5) == 1) {
		Party[0].hp = 0;
		Party[1].hp = 0;
	}
}

//毎フレーム呼ばれる処理
void Update()
{
	debug();

	if (GameStatus == AtackEnd)
	{
		graphstop++;
		
	}

	if (CheckHitKey(KEY_INPUT_RETURN) == 1 && keystop == false) {

		GameStatus++;
		if (GameStatus >= End) {
			GameStatus = BattleStart;
			
			SelectPlayerNo = 0;
			lvcount = 0;
			for (int i = 0; i < 2; i++){
				charlvUP[i] = 0;
			}
			
		}
		if (GameStatus == Levelup){
			charexp[0] += BattleEnemy.exp;
			charexp[1] += BattleEnemy.exp;
			LevelExp();
			for (int i = 0; i < 51; i++) {
				for (int j = 0; j < 2; j++) {
					if (charexp[j] > exptable[i] && charexp[j] < exptable[i + 1]) {
						charlv[j] = i+1;
						lvcount++;
					}
				}
			}
			BattleEnemy.graphic = LoadGraph("", false);
			
			charexp[0] -= BattleEnemy.exp;
			charexp[1] -= BattleEnemy.exp;
			for (int i = 0; i < 51; i++) {
				for (int j = 0; j < 2; j++) {
					if (nazonoiti[j] == 0) {
						nazonoiti[j] = 1;
						charlvUP[j] = 1;
					}else if (charexp[j] > exptable[i] && charexp[j] < exptable[i + 1]) {
						charlvUP[j] = i + 1;
					}
				}
			}
			charexp[0] += BattleEnemy.exp;
			charexp[1] += BattleEnemy.exp;
			/*for (int i = 0; i < 2; i++) {
				if (charlvUP[i] >= 1) {
					MessageText = Party[i].name + "のレベルが" + std::to_string(charlvUP[i]) + "上がった\n";
					StatusText[i] = "Name/" + Party[0].name + "\n" + "Level/" + std::to_string(charlv[0]) + "\n" + "HP/" + std::to_string(Party[0].hp) + "\n" + "MP/" + std::to_string(Party[0].mp) + "\n"
						+ "Name/" + Party[1].name + "\n" + "Level/" + std::to_string(charlv[1]) + "\n" + "HP/" + std::to_string(Party[1].hp) + "\n" + "MP/" + std::to_string(Party[1].mp) + "\n";
				}
			}*/

			if (lvcount>=1) {
				PlaySoundMem(lvupsound, DX_PLAYTYPE_BACK, true);

				MessageText = Party[0].name + "のレベルが" + std::to_string(charlv[0]-charlvUP[0]) + "上がった\n" + Party[1].name + "のレベルが" + std::to_string(charlv[1] - charlvUP[1]) + "上がった\n";
				UIp = LoadGraph("UIpl.png");
				StatusText[SelectPlayerNo] = "Name/" + Party[0].name + "\n" + "Lv/" + std::to_string(charlv[0]) + "\n" + "HP/" + std::to_string(Party[0].hp) + "\n" + "MP/" + std::to_string(Party[0].mp) + "\n" + "AP/" + std::to_string(Party[0].ap) + "\n" + "\n"
					+ "Name/" + Party[1].name + "\n" + "Lv/" + std::to_string(charlv[1]) + "\n" + "HP/" + std::to_string(Party[1].hp) + "\n" + "MP/" + std::to_string(Party[1].mp) + "\n" + "AP/" + std::to_string(Party[1].ap) + "\n";
					DrawBox(640, 40, 755, 270, GetColor(0, 0, 0), true);
					DrawBox(640, 40, 755, 270, GetColor(255, 255, 255), false);
				
			}
		}
		if (GameStatus == Result) {
			MessageText = BattleEnemy.name + "を倒した";
			BattleEnemy.graphic = LoadGraph("", false);
			

		}
		if (GameStatus == EnemyJudge)
		{
			if (turnf>=1){
				turnf = 0;
				SelectPlayerNo--;
			}
					
			if (BattleEnemy.hp <= 0) {
				MessageText = BattleEnemy.name + "を倒した";
				SetumeiText[SelectNo] = "経験値を" + std::to_string(BattleEnemy.exp) + "ポイント獲得した";
				UIp = LoadGraph("");
				StatusText[SelectPlayerNo] ="";
				UIene = LoadGraph("");
				EnemyStatusText = "";
				GameStatus = Result;//倒した
				raref++;
			}else{

				if (SelectPlayerNo >= PartyMax - 1) {
					GameStatus = EnemySelect;
				}
				else {
					SelectPlayerNo++;
					GameStatus = PlayerSelect;
					
				}
				
			}
		}

		if (GameStatus == PlayerJudge)
		{
			
			if (Party[0].hp <= 0 || Party[1].hp <= 0) {
				for (int i = 0; i < PartyMax; i++)
				{
					if (Party[i].hp <= 0) {
						deathcount[i]++;
						//死亡人数
						if (deathcount[i] == 1)
						{
							countdeathsum++;
						}
					}
				}
			}
			//現状は一人だが増やすことも可能
			
				
			//全滅したらゲームオーバー
			if (countdeathsum >= PartyMax) {
				UIp = LoadGraph("");
				UIp2 = LoadGraph("UIpl2.png");
				DrawBox(640, 40, 755, 270, GetColor(0, 0, 0), true);
				DrawBox(640, 40, 755, 270, GetColor(255, 255, 255), false);
				StatusText[SelectPlayerNo] = "Name/" + Party[0].name + "\n" + "Lv/" + std::to_string(charlv[0]) + "\n" + "HP/" + std::to_string(Party[0].hp) + "\n" + "MP/" + std::to_string(Party[0].mp) + "\n" + "AP/" + std::to_string(Party[0].ap) + "\n" + "\n"
					+ "Name/" + Party[1].name + "\n" + "Lv/" + std::to_string(charlv[1]) + "\n" + "HP/" + std::to_string(Party[1].hp) + "\n" + "MP/" + std::to_string(Party[1].mp) + "\n" + "AP/" + std::to_string(Party[1].ap) + "\n";
				MessageText = Party[0].name+"と"+Party[1].name + "は死んでしまった。";
				for (int i = 0; i < PartyMax; i++) {
					deathcount[i] = 0;
				}
				countdeathsum = 0;
				GameStatus = GameOver;
			}
			else {
				GameStatus = PlayerSelect;
			}
			
		}
		if (GameStatus == GameOverEnd) {
			UIp = LoadGraph("");
			UIp2 = LoadGraph("");
			StatusText[SelectPlayerNo] = "";
			UIene = LoadGraph("");
			EnemyStatusText = "";
			GameStatus = BattleStart;
		}



		if (GameStatus == PlayerSelectEnd) {
			if (SelectNo == 0) {
				//攻撃
				GameStatus = PlayerAtack;
			}
			if (SelectNo == 1) {
				//魔法
				SelectText[0] = Party[SelectPlayerNo].magic[0].name;
				SelectText[1] = Party[SelectPlayerNo].magic[1].name;
				SelectText[2] = Party[SelectPlayerNo].magic[2].name;
				SelectText[3] = "戻る";
				SetumeiText[0] = "必要MP/"+std::to_string(Party[SelectPlayerNo].magic[0].cost)+"\n"+Party[SelectPlayerNo].magic[0].setumei;
				SetumeiText[1] = "必要MP/" + std::to_string(Party[SelectPlayerNo].magic[1].cost) + "\n" + Party[SelectPlayerNo].magic[1].setumei;
				SetumeiText[2] = "必要MP/" + std::to_string(Party[SelectPlayerNo].magic[2].cost) + "\n" + Party[SelectPlayerNo].magic[2].setumei;
				SetumeiText[3] = "";

				MessageText = "魔法を選択してください";
				GameStatus = MagicSelect;
			}
			if (SelectNo == 2)//道具を選んだ時
			{
				toolgraph[0] = Party[SelectPlayerNo].tool[0].graph;//薬草画像
				toolgraph[1] = Party[SelectPlayerNo].tool[1].graph;
				toolgraph[2] = Party[SelectPlayerNo].tool[2].graph;
				toolgraph[3] = Party[SelectPlayerNo].tool[3].graph;
				toolgraph[4] = Party[SelectPlayerNo].tool[4].graph;
				toolgraph[5] = Party[SelectPlayerNo].tool[5].graph;

				SelectText[0] = Party[SelectPlayerNo].tool[0].name;
				SelectText[1] = Party[SelectPlayerNo].tool[1].name;
				SelectText[2] = Party[SelectPlayerNo].tool[2].name;
				SelectText[3] = Party[SelectPlayerNo].tool[3].name;
				SelectText[4] = Party[SelectPlayerNo].tool[4].name;
				SelectText[5] = Party[SelectPlayerNo].tool[5].name;
				SelectText[6] = "戻る";
				SetumeiText[0] = "残り個数/" + std::to_string( Party[SelectPlayerNo].item[0]) + "\n" + Party[SelectPlayerNo].tool[0].setumei;
				SetumeiText[1] = "残り個数/" + std::to_string(Party[SelectPlayerNo].item[1]) + "\n" + Party[SelectPlayerNo].tool[1].setumei;
				SetumeiText[2] = "残り個数/" + std::to_string(Party[SelectPlayerNo].item[2]) + "\n" + Party[SelectPlayerNo].tool[2].setumei;
				SetumeiText[3] = "残り個数/" + std::to_string(Party[SelectPlayerNo].item[3]) + "\n" + Party[SelectPlayerNo].tool[3].setumei;
				SetumeiText[4] = "残り個数/" + std::to_string(Party[SelectPlayerNo].item[4]) + "\n" + Party[SelectPlayerNo].tool[4].setumei;
				SetumeiText[5] = "残り個数/" + std::to_string(Party[SelectPlayerNo].item[5]) + "\n" + Party[SelectPlayerNo].tool[5].setumei;

				MessageText = "道具を選択してください";
				GameStatus = Toolselect;
			}
			SelectNo = 0;
		}

		if (GameStatus == PlayerAtack) {
			BattleEnemy.hp -= Party[SelectPlayerNo].ap;
			deffectf = +1;

			MessageText = Party[SelectPlayerNo].name + "の攻撃！\n" + BattleEnemy.name + "に" + std::to_string(Party[SelectPlayerNo].ap) + "のダメージ！\n残りHP" + std::to_string(BattleEnemy.hp);
			UIp = LoadGraph("UIpl.png");
			StatusText[SelectPlayerNo] = "Name/" + Party[SelectPlayerNo].name + "\n"+"Lv/"+std::to_string(charlv[SelectPlayerNo])+"\n"+"HP/" + std::to_string( Party[SelectPlayerNo].hp) + "\n"+"MP/" + std::to_string(Party[SelectPlayerNo].mp) + "\n" + "AP/" + std::to_string(Party[SelectPlayerNo].ap) + "\n";
			UIene = LoadGraph("UIene.png");
			EnemyStatusText = "Name/" + BattleEnemy.name + "\n" + "Lv/" + std::to_string(BattleEnemy.exp/60) + "\n" + "HP/" + std::to_string(BattleEnemy.hp) + "\n" + "MP/" + std::to_string(BattleEnemy.mp) + "\n" + "AP/" + std::to_string(BattleEnemy.ap) + "\n";
			if (Party[1].hp <= 0) {
				GameStatus = EnemyJudge;
			}
			else
			GameStatus=AtackEnd;
		}

		if (GameStatus == MagicSelectEnd) {
			if (SelectNo == 3) {
				GameStatus = PlayerSelect;
				
			}
			else if (SelectNo < 4) {
				if (Party[SelectPlayerNo].magic[SelectNo].cost > Party[SelectPlayerNo].mp) {
					MessageText = "MPが足りない！";
					turnf++;
				}
				else {
					MessageText = Party[SelectPlayerNo].name + "は" + Party[SelectPlayerNo].magic[SelectNo].name + "を唱えた！\n" + Party[SelectPlayerNo].magic[SelectNo].message + "\n残りMP" + std::to_string(Party[SelectPlayerNo].mp-Party[SelectPlayerNo].magic[SelectNo].cost) + "\n";

					if (Party[SelectPlayerNo].magic[SelectNo].type == Atack) {
						BattleEnemy.hp -= (int)Party[SelectPlayerNo].magic[SelectNo].power;
						deffectf = +1;
						MessageText = MessageText + BattleEnemy.name + "に" + std::to_string((int)Party[SelectPlayerNo].magic[SelectNo].power) + "のダメージ！\n残りHP" + std::to_string(BattleEnemy.hp);
					}
					if (Party[SelectPlayerNo].magic[SelectNo].type == Heal) {
						Party[SelectPlayerNo].hp += (int)Party[SelectPlayerNo].magic[SelectNo].power;
						MessageText = MessageText + Party[SelectPlayerNo].name + "のHPが" + std::to_string((int)Party[SelectPlayerNo].magic[SelectNo].power) + "回復！\n残りHP" + std::to_string(Party[SelectPlayerNo].hp);
					}
					if (Party[SelectPlayerNo].magic[SelectNo].type == Buff) {
						Party[SelectPlayerNo].ap *= Party[SelectPlayerNo].magic[SelectNo].power;
						MessageText = MessageText + Party[SelectPlayerNo].name + "の攻撃力が" + std::to_string(Party[SelectPlayerNo].magic[SelectNo].power) + "倍になった！\n攻撃力" + std::to_string(Party[SelectPlayerNo].ap);
					}
					if (Party[SelectPlayerNo].magic[SelectNo].type == harm) {
						BattleEnemy.hp -= (int)Party[SelectPlayerNo ].magic[SelectNo].power;
						Party[SelectPlayerNo].hp -= Party[SelectPlayerNo].magic[SelectNo].harmd;
						deffectf = +1;
						MessageText = MessageText + BattleEnemy.name + "に" + std::to_string((int)Party[SelectPlayerNo].magic[SelectNo].power) + "のダメージ！\n残りHP" + std::to_string(BattleEnemy.hp)+"\n"
							+ Party[SelectPlayerNo].name + "のHPが" + std::to_string((int)Party[SelectPlayerNo].magic[SelectNo].harmd) + "減ってしまった！\n残りHP" + std::to_string(Party[SelectPlayerNo].hp);
					}
					if (Party[SelectPlayerNo].magic[SelectNo].type == downheal)
					{
						int down = Party[SelectPlayerNo].ap-(Party[SelectPlayerNo].ap * Party[SelectPlayerNo].magic[SelectNo].harmd);
						Party[SelectPlayerNo].ap = Party[SelectPlayerNo].ap*Party[SelectPlayerNo].magic[SelectNo].harmd;
						Party[SelectPlayerNo].hp += (int)Party[SelectPlayerNo].magic[SelectNo].power;
						MessageText = MessageText + Party[SelectPlayerNo].name + "の攻撃力が"+ std::to_string(down) +"下がり、HPが"+ std::to_string((int)Party[SelectPlayerNo].magic[SelectNo].power) + "回復！\n"
							+"残りHP" + std::to_string(Party[SelectPlayerNo].hp) + "攻撃力" + std::to_string((int)Party[SelectPlayerNo].ap);
					}
					   
					Party[SelectPlayerNo].mp -= Party[SelectPlayerNo].magic[SelectNo].cost;
				}

				UIp = LoadGraph("UIpl.png");
				StatusText[SelectPlayerNo] = "Name/" + Party[SelectPlayerNo].name + "\n"+"Lv/"+std::to_string(charlv[SelectPlayerNo])+"\n" + "HP/" + std::to_string(Party[SelectPlayerNo].hp) + "\n" + "MP/" + std::to_string(Party[SelectPlayerNo].mp) + "\n" + "AP/" + std::to_string(Party[SelectPlayerNo].ap) + "\n";
				UIene = LoadGraph("UIene.png");
				EnemyStatusText = "Name/" + BattleEnemy.name + "\n" + "Lv/" + std::to_string(BattleEnemy.exp / 60) + "\n" + "HP/" + std::to_string(BattleEnemy.hp) + "\n" + "MP/" + std::to_string(BattleEnemy.mp) + "\n" + "AP/" + std::to_string(BattleEnemy.ap) + "\n";
				if (Party[1].hp <= 0) {
					GameStatus = EnemyJudge;
				}
				else
				GameStatus = AtackEnd;

			}
			SelectNo = 0;
		}
		if (GameStatus == ToolselectEnd){
			if (SelectNo == 6) {
				GameStatus = PlayerSelect;
				
			}
			else if (SelectNo < 6) {
				if (Party[SelectPlayerNo].tool[SelectNo].cost> Party[SelectPlayerNo].item[SelectNo]) {
					MessageText = "アイテムがない！？";
					turnf++;
					
				}
				else {
					MessageText=Party[SelectPlayerNo].name+"は"+Party[SelectPlayerNo].tool[SelectNo].name+"を使った！\n"+"残り"+std::to_string(Party[SelectPlayerNo].item[SelectNo]-1)+"個\n";

					if (Party[SelectPlayerNo].tool[SelectNo].type == Hpheal) {
						Party[SelectPlayerNo].hp += (int)Party[SelectPlayerNo].tool[SelectNo].power;
						MessageText = MessageText + Party[SelectPlayerNo].name + "のHPが" + std::to_string((int)Party[SelectPlayerNo].tool[SelectNo].power) + "回復！\n残りHP" + std::to_string(Party[SelectPlayerNo].hp);
						UIene = LoadGraph("UIene.png");
						EnemyStatusText = "Name/" + BattleEnemy.name + "\n" +"Level/"+std::to_string(BattleEnemy.exp / 60)+"\n"+ "HP/" + std::to_string(BattleEnemy.hp) + "\n" + "MP/" + std::to_string(BattleEnemy.mp) + "\n";
					}
					if (Party[SelectPlayerNo].tool[SelectNo].type == Mpheal) {
						Party[SelectPlayerNo].mp += (int)Party[SelectPlayerNo].tool[SelectNo].power;
						MessageText = MessageText + Party[SelectPlayerNo].name + "のMPが" + std::to_string((int)Party[SelectPlayerNo].tool[SelectNo].power) + "回復！\n残りMP" + std::to_string(Party[SelectPlayerNo].mp);
						UIene = LoadGraph("UIene.png");
						EnemyStatusText = "Name/" + BattleEnemy.name + "\n"+"Level/"+std::to_string(BattleEnemy.exp / 60)+"\n" + "HP/" + std::to_string(BattleEnemy.hp) + "\n" + "MP/" + std::to_string(BattleEnemy.mp) + "\n";
					}


					Party[SelectPlayerNo].item[SelectNo] -= Party[SelectPlayerNo].tool[SelectNo].cost;
				}
				UIp = LoadGraph("UIpl.png");
				StatusText[SelectPlayerNo] = "Name/" + Party[SelectPlayerNo].name + "\n"+"Lv/"+std::to_string(charlv[SelectPlayerNo])+"\n" + "HP/" + std::to_string(Party[SelectPlayerNo].hp) + "\n" + "MP/" + std::to_string(Party[SelectPlayerNo].mp) + "\n" + "AP/" + std::to_string(Party[SelectPlayerNo].ap) + "\n";
				UIene = LoadGraph("UIene.png");
				EnemyStatusText = "Name/" + BattleEnemy.name + "\n" +"Lv/"+std::to_string(BattleEnemy.exp / 60)+"\n"+ "HP/" + std::to_string(BattleEnemy.hp) + "\n" + "MP/" + std::to_string(BattleEnemy.mp) + "\n" + "AP/" + std::to_string(BattleEnemy.ap) + "\n";
				if (Party[1].hp <= 0) {
					GameStatus = EnemyJudge;
				}
				else
				GameStatus = AtackEnd;
			}
			SelectNo = 0;
		}

		if (GameStatus == PlayerSelect) {
			MessageText = "コマンドを選択してください";
			SelectText[0] = "戦う";
			SelectText[1] = "魔法";
			SelectText[2] = "道具";
			SelectText[3] = "";
			SelectText[4] = "";
			SelectText[5] = "";
			SelectText[6] = "";
			SetumeiText[0] = "物理で殴る！";
			SetumeiText[1] = "MPを消費して様々な技を発動！";
			SetumeiText[2] = "アイテムを選択！";
			SetumeiText[3] = "";
			SetumeiText[4] = "";
			SetumeiText[5] = "";
			SetumeiText[6] = "";

			PlaySoundMem(keysound, DX_PLAYTYPE_BACK, true);
			UIp = LoadGraph("UIpl.png");
			StatusText[SelectPlayerNo] = "Name/" + Party[SelectPlayerNo].name + "\n" +"Lv/"+std::to_string(charlv[SelectPlayerNo])+"\n"
				+ "HP/" + std::to_string(Party[SelectPlayerNo].hp) + "\n" + "MP/" + std::to_string(Party[SelectPlayerNo].mp) + "\n" + "AP/" + std::to_string(Party[SelectPlayerNo].ap) + "\n";
			UIene = LoadGraph("UIene.png");
			EnemyStatusText = "Name/" + BattleEnemy.name + "\n" +"Lv/"+std::to_string(BattleEnemy.exp / 60)+"\n"+ "HP/" + std::to_string(BattleEnemy.hp) + "\n" 
				+ "MP/" + std::to_string(BattleEnemy.mp) + "\n" + "AP/" + std::to_string(BattleEnemy.ap) + "\n";
		}
		//効果音です
		if (GameStatus == MagicSelect || GameStatus==Toolselect){
			PlaySoundMem(keysound, DX_PLAYTYPE_BACK, true);
		}
		if (GameStatus == EnemySelect) {
			int r = 0;
			if (BattleEnemy.magic[0].cost <= BattleEnemy.mp) {
				r = GetRand(1);
			}

			int target = GetRand(PartyMax - 1);
			if (Party[0].hp <= 0) {
				target = 1;
			}
			else if (Party[1].hp <= 0) {
				target = 0;
			}
			if (r == 0) {
				//通常攻撃
				Party[target].hp -= BattleEnemy.ap;
				MessageText = BattleEnemy.name + "の攻撃！\n" + Party[target].name + "に" + std::to_string(BattleEnemy.ap) + "のダメージ！\n残りHP" + std::to_string(Party[target].hp);
				UIp = LoadGraph("UIpl.png");
				StatusText[target] = "Name/" + Party[target].name + "\n" + "Lv/" + std::to_string(charlv[SelectPlayerNo]) + "\n" + "HP/" + std::to_string(Party[target].hp) + "\n" + "MP/" + std::to_string(Party[target].mp) + "\n" + "AP/" + std::to_string(Party[target].ap) + "\n";
				UIene = LoadGraph("UIene.png");
				EnemyStatusText = "Name/" + BattleEnemy.name + "\n" + "Lv/" + std::to_string(BattleEnemy.exp / 60) + "\n" + "HP/" + std::to_string(BattleEnemy.hp) + "\n" + "MP/" + std::to_string(BattleEnemy.mp) + "\n" + "AP/" + std::to_string(BattleEnemy.ap) + "\n";
			}
			if (r == 1) {
				//魔法攻撃
				MessageText = BattleEnemy.name + "は" + BattleEnemy.magic[0].name + "を唱えた！\n" + BattleEnemy.magic[0].message + "\n";
				if (BattleEnemy.magic[0].type == Atack) {
					Party[target].hp -= (int)BattleEnemy.magic[0].power;
					MessageText = MessageText + Party[target].name + "に" + std::to_string((int)BattleEnemy.magic[0].power) + "のダメージ！\n残りHP" + std::to_string(Party[target].hp);
				}
				if (BattleEnemy.magic[0].type == Heal) {
					BattleEnemy.hp += (int)BattleEnemy.magic[0].power;
					MessageText = MessageText + BattleEnemy.name + "のHPが" + std::to_string((int)BattleEnemy.magic[0].power) + "回復！\n残りHP" + std::to_string(BattleEnemy.hp);
				}
				if (BattleEnemy.magic[0].type == Buff) {
					BattleEnemy.ap *= BattleEnemy.magic[0].power;
					MessageText = MessageText + BattleEnemy.name + "の攻撃力が" + std::to_string(BattleEnemy.magic[0].power) + "倍になった！\n攻撃力" + std::to_string(BattleEnemy.ap);
				}
				if (BattleEnemy.magic[0].type == harm) {
					Party[target].hp -= (int)BattleEnemy.magic[0].power;
					BattleEnemy.hp -= (int)BattleEnemy.magic[0].harmd;
					if (BattleEnemy.hp <= 0) {
						BattleEnemy.hp = 1;
					}
					MessageText = MessageText + Party[target].name + "に" + std::to_string((int)BattleEnemy.magic[0].power) + "のダメージ！\n残りHP" + std::to_string(Party[target].hp) + "\n"
						+ BattleEnemy.name + "のHPが"  + "減ってしまった！\n残りHP" + std::to_string(BattleEnemy.hp);
				}
				BattleEnemy.mp -= BattleEnemy.magic[0].cost;

			}
			UIp = LoadGraph("UIpl.png");
			StatusText[target] = "Name/" + Party[target].name + "\n" + "Lv/" + std::to_string(charlv[SelectPlayerNo]) + "\n" + "HP/" + std::to_string(Party[target].hp) + "\n" + "MP/" + std::to_string(Party[target].mp) + "\n" + "AP/" + std::to_string(Party[target].ap) + "\n";
			UIene = LoadGraph("UIene.png");
			EnemyStatusText = "Name/" + BattleEnemy.name + "\n" + "Lv/" + std::to_string(BattleEnemy.exp / 60) + "\n" + "HP/" + std::to_string(BattleEnemy.hp) + "\n" + "MP/" + std::to_string(BattleEnemy.mp) + "\n" + "AP/" + std::to_string(BattleEnemy.ap) + "\n";
			GameStatus = EnemyAtackDisp;
			if (Party[0].hp <= 0) {
				SelectPlayerNo = 1;
			}else
			SelectPlayerNo = 0;
		}
		keystop = true;//キー停止
	}


	if (CheckHitKey(KEY_INPUT_UP) == 1 && keystop == false) {
		if(SelectNo > 0) SelectNo--;
		keystop = true;//キー停止
		PlaySoundMem(keysound, DX_PLAYTYPE_BACK, true);
	}
	if (CheckHitKey(KEY_INPUT_DOWN) == 1 && keystop == false) {
		if (GameStatus == PlayerSelect && SelectNo < 2) {
			SelectNo++;
			PlaySoundMem(keysound, DX_PLAYTYPE_BACK, true);
		}
		if (GameStatus == MagicSelect && SelectNo < 3) {
			SelectNo++;
			PlaySoundMem(keysound, DX_PLAYTYPE_BACK, true);
		}
		if (GameStatus == Toolselect && SelectNo < 6) {
			SelectNo++;
			PlaySoundMem(keysound, DX_PLAYTYPE_BACK, true);
		}
		keystop = true;//キー停止
	}
	

	//キー停止処理解除
	if (CheckHitKey(KEY_INPUT_RETURN) == 0 &&
		CheckHitKey(KEY_INPUT_UP) == 0 &&
		CheckHitKey(KEY_INPUT_DOWN)==0)
	{
		keystop = false;
	}


	if (GameStatus == BattleStart) {
		//パーティメンバーは千野と千野謙
		Party[0] = CharacterTable[Chino];
		Party[1] = CharacterTable[ChinoKen];
		
	
	
		if (raref==4 || raref==9) {
			int r = GetRand(1)+5;
			if (r==5) {
				StopSoundMem(backsoundrea2);
				StopSoundMem(backsound);
				PlaySoundMem(backsoundrea, DX_PLAYTYPE_BACK, true);
			}
			if (r==6) {
				StopSoundMem(backsound);
				StopSoundMem(backsoundrea);
				PlaySoundMem(backsoundrea2, DX_PLAYTYPE_BACK, true);
			}
			soundstop = 1;
			BattleEnemy = EnemyTable[r];
		}
		else {
			//敵の抽選
			int r = GetRand(EnemyMax-2 - 1);
			BattleEnemy = EnemyTable[r];
			if (soundstop == 1) {
				StopSoundMem(backsound);
				StopSoundMem(backsoundrea2);
				StopSoundMem(backsoundrea);
				soundstop = 0;
				PlaySoundMem(backsound, DX_PLAYTYPE_BACK, true);
			}
			
		}
		
		MessageText = BattleEnemy.name + "が現れた";
		UIp = LoadGraph("");
		StatusText[SelectPlayerNo] = "";
		UIene = LoadGraph("");
		EnemyStatusText = "";
		GameStatus = BattleStartDisp;
	}


}

void Draw()
{
	DrawGraph(0, 0, backgraph, true);
	//敵表示
	if (GameStatus == AtackEnd) {

		if (deffectf == 1) {
			if ((graphstop > 5 && graphstop < 10) || (graphstop > 15 && graphstop < 20) || (graphstop > 25 && graphstop < 30)) {
			}
			else
			{
				DrawGraph(200, 10, BattleEnemy.graphic, true);
			}

		}
		else {
			if (GameStatus == Result) {

			}
			else DrawGraph(200, 10, BattleEnemy.graphic, true);
		}
	}
	else {
		//敵表示
		DrawGraph(200, 10, BattleEnemy.graphic, true);
		graphstop = 0;
		deffectf = 0;
	}


	DrawBox(200, 410, 750, 570, GetColor(0, 0, 0), true);
	DrawBox(200, 410, 750, 570, GetColor(255, 255, 255), false);

	//メッセージ表示
	DrawFormatString(240, 440, GetColor(255, 255, 255), MessageText.c_str());//デフォルト
	DrawGraph(0, 0, UIp2, true);
	DrawGraph(0, 0, UIp, true);
	if (GameStatus == Levelup && lvcount >= 1) {
		DrawBox(640, 40, 755, 270, GetColor(0, 0, 0), true);
		DrawBox(640, 40, 755, 270, GetColor(255, 255, 255), false);
	}
	/*if (GameStatus == PlayerJudge) {
		DrawBox(640, 40, 755, 270, GetColor(0, 0, 0), true);
		DrawBox(640, 40, 755, 270, GetColor(255, 255, 255), false);
	}*/
	DrawFormatString(650, 50, GetColor(255, 255, 255), StatusText[SelectPlayerNo].c_str());//味方ステータス
	DrawGraph(0, 0, UIene, true);
	DrawFormatString(50, 50, GetColor(255, 255, 255), EnemyStatusText.c_str());//敵ステータス
	//メニュー表示
	if (GameStatus == PlayerSelect || GameStatus == MagicSelect || GameStatus == Toolselect)
	{
		DrawBox(10, 360, 200, 570, GetColor(0, 0, 0), true);
		//選択メニュー表示
		for (int i = 0; i < MenuMax; i++) {
			DrawFormatString(60, 370 + i * 30, GetColor(255, 255, 255), SelectText[i].c_str());
			DrawFormatString(240, 480, GetColor(255, 255, 255), SetumeiText[SelectNo].c_str());
		}
		//カーソル表示
		DrawFormatString(30, 370 + SelectNo * 30, GetColor(255, 255, 255), "⇒");

		DrawBox(10, 360, 200, 570, GetColor(255, 255, 255), false);
	}
	if (GameStatus == Result)
	{
		DrawFormatString(240, 480, GetColor(255, 255, 255), SetumeiText[SelectNo].c_str());
	}
	//デバッグ用ページ表記です。気にしないでください

	if (pop3 == 1) {
		DrawBox(640, 40, 755, 270, GetColor(0, 0, 0), true);
		DrawBox(640, 40, 755, 270, GetColor(255, 255, 255), false);
		DrawFormatString(650, 50, GetColor(255, 255, 255), debugstatustext[SelectPlayerNo].c_str());
	}
	if (pop2 == 1) {
		DrawBox(40, 40, 240, 150, GetColor(0, 0, 0), true);
		DrawBox(40, 40, 240, 150, GetColor(255, 255, 255), false);
		DrawFormatString(50, 50, GetColor(255, 255, 255), debugEnemyStatusText.c_str());
	}
	if (GameStatus == Toolselect)//画像を入れているだけ
	{
		DrawGraph(180, 220, toolgraph[SelectNo], true);
	}
}

void Delete()
{
	for (int i = 0; i < EnemyMax; i++) {
		DeleteGraph(EnemyTable[i].graphic);
	}
	for (int i = 0; i < ToolMax; i++)
	{
		DeleteGraph(toolTable[i].graph);
	}
	
}

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(TRUE);	//ウィンドウモードにする
	SetGraphMode(800, 600, 32);	//ウィンドウサイズを設定する
	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}
	SetDrawScreen(DX_SCREEN_BACK);	//裏画面を描画対象へ

	Init();//初期化
	PlaySoundMem(backsound, DX_PLAYTYPE_BACK, true);
	//メイン処理
	while (ProcessMessage() == 0 &&(CheckHitKey(KEY_INPUT_ESCAPE)||CheckHitKey(KEY_INPUT_DECIMAL)) == 0) {
		Update();//更新処理
		Draw();//描画処理

		ScreenFlip();		//裏画面と表画面の入替
		ClearDrawScreen();	//裏画面の描画を全て消去
	}
	Delete();
	DxLib_End();			// ＤＸライブラリ使用の終了処理
	return 0;				// ソフトの終了 
}