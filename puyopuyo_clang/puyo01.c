#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <curses.h>

#define WIDTH    6// フィールド幅
#define HEIGHT    12// フィールド高
#define VANISH    4 // 消去に必要な個数
#define COLOR    5  // 色数
#define DELAY 40000 //遅延のためにforループを回す回数

// ぷよ管理用構造体
struct BLOCK
{
    int nColor;        // 色
    int nVanish;    // 消去フラグ
};

// フィールド管理用構造体
typedef struct
{
    struct BLOCK stBlock[WIDTH+2][HEIGHT+1];    // ぷよ構造体配列
}FIELD;

//　ぷよ, 次のぷよ，次の次のぷよ
int puyo[3][3], puyo_next[3][3], puyo_after_next[3][3];

//フィールドに出現しているぷよの座標
int x,y;

//ゲームオーバーかどうかを示すフラグ
int gameover = 0;  


// 関数のプロトタイプ宣言
void ShowField(FIELD* pField);
int Check(FIELD* pField);
void Vanish(FIELD* pField);
int Slide(FIELD* pField);
void Count(FIELD* pField, int nX, int nY, int* pCount);
void Initialize(FIELD* pField);
int CreatePuyo(FIELD* pField);
void ControlPuyo(FIELD* pField, FIELD* pStage);

int CheckOverlap(FIELD* pStage, int x, int y);

void MovePuyo(FIELD* pField, FIELD* pStage, int x, int y);
int TurnPuyo(FIELD* pField, FIELD* pStage, int direction);

void LockPuyo(FIELD* pField, FIELD* pStage);
void FallPuyo(FIELD *pFIELD, FIELD *pStage);
void ShowPuyo();

void DeletePuyo(FIELD *pField, FIELD *pStage);


//kbhit.c
void init_keyboard();
void close_keyboard();
int kbhit();
int readkey();

// メイン関数
int main(void)
{
    //int nChain = 0;
    FIELD stStage; //ステージ．移動しているぷよ以外の情報を持つ
    FIELD stField; //フィールド．ステージに加えて，ぷよの情報も持つ

    // 乱数の初期化
    srand((unsigned)time(NULL));

    //ステージとフィールドの初期化．
    Initialize(&stStage);
    memcpy(&stField,&stStage, sizeof(FIELD));

    //フィールドにはぷよを配置．
    CreatePuyo(&stField);

    //時間稼ぎ
    int wait = 0;
    while(!gameover){

        //入力認識
        if(kbhit()){
            //入力に応じてぷよを制御する
            ControlPuyo(&stField, &stStage);
        }

        //遅延させる
        if(wait < DELAY)
            wait++;
        
        else{
            //下の方に重なりがなければ，ぷよを1マス下に移動
            if (!CheckOverlap(&stStage, x, y+1)){
                MovePuyo(&stField, &stStage, x, y+1);
            }
            else{
                //ぷよをステージに固定
                LockPuyo(&stField, &stStage);
                //浮いているぷよを下に落とす
                FallPuyo(&stField, &stStage);
                //ぷよの消去（連鎖）と移動
                DeletePuyo(&stField, &stStage);
                //新しいぷよを生成する
                CreatePuyo(&stField);
            }

            //画面の描画
            ShowField(&stField);
            wait = 0;
        } 
    }
    return 0;
}

// フィールドを描画
void ShowField(FIELD* pField)
{
    //int i; //forループ制御用変数
    int nX = 0;
    int nY = 0;

    //画面を一旦クリア
    system("clear");

    for (nY = 0; nY < HEIGHT+1; ++nY)
    {
        for (nX = 0; nX < WIDTH+2; ++nX)
        {
            // 色と消去フラグを表示する
            //printf("[%2d,%2d] ", pField->stBlock[nX][nY].nColor, pField->stBlock[nX][nY].nVanish);

            //for debug
            //printf("%d ",pField->stBlock[nX][nY].nColor);

            switch(pField->stBlock[nX][nY].nColor){
                case 0:
                    printf("  ");
                    break;
                case 1:
                    printf("● ");
                    break;
                case 2:
                    printf("▲ ");
                    break;
                case 3:
                    printf("@ ");
                    break;
                case 4:
                    printf("# ");
                    break;
                case 5:
                    printf("$ ");
                    break;
                case 9:
                    printf("■ ");
                    break;
                default:
                    printf("%d ",pField->stBlock[nX][nY].nColor);
                    break;
            }
            //printf("(%d,%d)",nX,nY );
        }
        //putc('\n', stdout);
        printf("\n");
    }
    //putc('\n', stdout);
    printf("\n");
}

// 全ブロックの同色で隣接している個数をチェックする
int Check(FIELD* pField)
{
    int nX = 0;
    int nY = 0;
    int nReturn = 0;
    FIELD stDummy;

    for (nX = 1; nX < WIDTH+1; ++nX)
    {
        for (nY = 0; nY < HEIGHT; ++nY)
        {
            // 空白＝０としているので、０の場合は何もしない
            if(pField->stBlock[nX][nY].nColor == 0)
            {
                continue;
            }

            // 消去フラグをクリアしておく
            pField->stBlock[nX][nY].nVanish = 0;

            // フィールド構造体をコピーして、検査用の構造体領域を作成する
            memcpy(&stDummy, pField, sizeof(FIELD));

            // 同色で隣接している個数をカウントする
            Count(&stDummy, nX, nY, &pField->stBlock[nX][nY].nVanish);

            // 同色で隣接している個数がVANISH定数を超えていたら、戻り値を１にする
            nReturn |=(pField->stBlock[nX][nY].nVanish >= VANISH);
        }
    }

    // １箇所でも消去される箇所があれば１、なければ０が返る
    return nReturn;
}

// VANISH定数よりたくさん同色で隣接していたら消去する
void Vanish(FIELD* pField)
{
    int nX = 0;
    int nY = 0;

    for (nX = 1; nX < WIDTH+1; ++nX)
    {
        for (nY = 0; nY < HEIGHT; ++nY)
        {
            // 同色での隣接数がVANISH定数以上か？
            if(pField->stBlock[nX][nY].nVanish >= VANISH)
            {
                // ブロック情報をクリアする
                memset(&pField->stBlock[nX][nY], 0, sizeof(struct BLOCK));
            }
        }
    }
}

// 消去されて空いた隙間を１段だけ詰める
int Slide(FIELD* pField)
{
    int nX = 0;
    int nY = 0;
    int nReturn = 0;

    for (nY = HEIGHT - 1; nY >= 1; --nY)
    {
        for (nX = 1; nX < WIDTH+1; ++nX)
        {
            // 検査箇所が空白で、１つ上が空白ではない場合
            if(pField->stBlock[nX][nY].nColor == 0 &&
                pField->stBlock[nX][nY - 1].nColor != 0)
            {
                // 検査箇所へ１つ上のブロック情報をコピーする
                memcpy(&pField->stBlock[nX][nY], &pField->stBlock[nX][nY - 1], sizeof(struct BLOCK));

                // １つ上のブロック情報をクリアする
                memset(&pField->stBlock[nX][nY - 1], 0, sizeof(struct BLOCK));

                // 動いた箇所があったので、戻り値を１にする
                nReturn  = 1;
            }
        }
    }

    // 動いた箇所があった場合は１、そうでない場合は０が返る
    return nReturn;
}

// 同色で隣接している個数を数える（再帰関数）
void Count(FIELD* pField, int nX, int nY, int* pCount)
{
    // 検査箇所の色を保持する
    int nColor = pField->stBlock[nX][nY].nColor;

    // ダブって検査しないように、検査した箇所のブロック情報をクリアしておく
    memset(&pField->stBlock[nX][nY], 0, sizeof(struct BLOCK));

    // カウンタをインクリメントする
    ++(*pCount);

    // １つ上のブロックを検査
    if(nY - 1 >= 0 && nColor == pField->stBlock[nX][nY - 1].nColor)
    {
        Count(pField, nX, nY - 1, pCount);
    }

    // １つ下のブロックを検査
    if(nY + 1 < HEIGHT && nColor == pField->stBlock[nX][nY + 1].nColor)
    {
        Count(pField, nX, nY + 1, pCount);
    }

    // １つ左のブロックを検査
    if(nX - 1 >= 0 && nColor == pField->stBlock[nX - 1][nY].nColor)
    {
        Count(pField, nX - 1, nY, pCount);
    }

    // １つ右のブロックを検査
    if(nX + 1 < WIDTH && nColor == pField->stBlock[nX + 1][nY].nColor)
    {
        Count(pField, nX + 1, nY, pCount);
    }
}


void Initialize(FIELD* pField)
{
    int nX, nY; //forループ制御用変数

    //kbhit()のためのキーボード初期化
    init_keyboard();

    //フィールドの初期化
    for (nY = 0; nY < HEIGHT+1; ++nY){
        for (nX = 0; nX < WIDTH+2; ++nX){

            pField->stBlock[nX][nY].nVanish = 0;

            if (nX == 0 || nX == WIDTH+1 || nY == HEIGHT)
                pField->stBlock[nX][nY].nColor = 9;
            else
                pField->stBlock[nX][nY].nColor = 0;
        }
    }

    //未来のぷよを2つ作っておく
    for (nY = 0; nY < 3; ++nY){
        for (nX = 0; nX < 3; ++nX){

            if((nX == 1 && nY == 0) || (nX == 1 && nY == 1)) {

                puyo_next[nX][nY] = rand() % COLOR + 1;
                puyo_after_next[nX][nY] = rand() % COLOR + 1;
            }
            else{
                puyo_next[nX][nY] = 0;
                puyo_after_next[nX][nY] = 0;
            }

        }
    }
}

int CreatePuyo(FIELD* pField)
{
    int nX, nY;

    memcpy(puyo,puyo_next,sizeof(puyo));
    memcpy(puyo_next,puyo_after_next,sizeof(puyo));

    //debug
    printf("puyo %d,%d\n", puyo[1][0],puyo[1][1]);
    printf("puyo_next %d,%d\n", puyo_next[1][0],puyo_next[1][1]);


    //ぷよの座標を最上段中央にセット
    x = 3;
    y = 0;

    //２つ先のぷよを生成する
    for (nY = 0; nY < 3; ++nY){
        for (nX = 0; nX < 3; ++nX){

            if((nX == 1 && nY == 0) || (nX == 1 && nY == 1))
                puyo_after_next[nX][nY] = rand() % COLOR + 1;
            else
                puyo_after_next[nX][nY] = 0;
        }
    }

    //ぷよをフィールド上に配置する
    for (nY = 0; nY < 3; ++nY){
        for (nX = 0; nX < 3; ++nX){

            pField->stBlock[nX+x][nY+y].nColor += puyo[nX][nY];

            //置いたぷよが既に存在するぷよに重なっていればゲームオーバー
            if (pField->stBlock[nX+x][nY+y].nColor != puyo[nX][nY]){
                gameover = 1;
                printf("gameover!!!!\n");
                exit(0);
                return 1;
            }
        }
    }

    return 0;
}

void ControlPuyo(FIELD* pField, FIELD* pStage){
    char key = readkey(); //キーから1文字読み込む
    //キーに応じて各方向へぷよを移動したり、回転させたりする
    switch(key) {

        case 'a':
        if(!CheckOverlap(pStage, x-1, y)) {
            MovePuyo(pField, pStage, x-1, y);
        }
        break;
        case 'd':
        if(!CheckOverlap(pStage, x+1, y)) {
            MovePuyo(pField, pStage, x+1, y);
        }
        break;
        case 's':
        if(!CheckOverlap(pStage, x, y+2)) {
            MovePuyo(pField, pStage, x, y+2);
        }
        break;
        case 'i':
        TurnPuyo(pField,pStage, 0);
        break;
        case 'j':
        TurnPuyo(pField,pStage, 1);
        break;
    }
}

int CheckOverlap(FIELD* pStage, int x2, int y2)
{
    int nX, nY; 

    //ブロックが向かう位置に固定ブロックもしくは壁があるかどうかを検査
    for (nY = 0; nY < 3; ++nY){
        for (nX = 0; nX < 3; ++nX){
            //ぷよが存在している座標に，既に他の固定ぷよか壁が存在している場合
            if ((puyo[nX][nY] > 0) && (pStage->stBlock[x2 + nX][y2 + nY].nColor != 0)){
                return 1;
            }
        }
    }
    return 0;
}

void MovePuyo(FIELD* pField, FIELD* pStage, int x2, int y2)
{
    int nX, nY;

    //今までのぷよを消去する
    // for (nY = 0; nY < 3; ++nY){
    //     for (nX = 0; nX < 3; ++nX){
    //         pField->stBlock[x+nX][y+nY].nColor -= puyo[nX][nY];
    //     }
    // }


    //今あるぷよをフィールドから消す．
    memcpy(pField, pStage, sizeof(FIELD));

    //ブロックの座標を更新
    x = x2;
    y = y2;

    //新しい座標でぷよを入れなおす
    for (nY = 0; nY < 3; ++nY){
        for (nX = 0; nX < 3; ++nX){
            pField->stBlock[x+nX][y+nY].nColor += puyo[nX][nY];
        }
    }
}

void LockPuyo(FIELD* pField, FIELD* pStage)
{
    //フィールドに現在のぷよを埋め込む．
    memcpy(pStage, pField, sizeof(FIELD));
    //CreatePuyo(pField);
}

int TurnPuyo(FIELD* pField, FIELD* pStage, int direction){

    int place; //真ん中以外のぷよがどこにあるかを示す．
    //1:(0,1) 2:(1,0) 3:(1,2) 4:(2,1)

    //ぷよを保存しておく
    int temp[3][3];
    memcpy(temp,puyo,sizeof(puyo));

    //真ん中以外のぷよがどこにあるかを調べる．
    if (puyo[0][1]!=0) place = 1;
    else if (puyo[1][0]!=0) place = 2;
    else if (puyo[1][2]!=0) place = 3;
    else if (puyo[2][1]!=0) place = 4;

    //右回転
    if (direction == 0){ 
        switch(place) {

            case 1:
                puyo[1][0] = puyo[0][1];
                puyo[0][1] = 0;
                break;
            
            case 2:
                puyo[2][1] = puyo[1][0];
                puyo[1][0] = 0;
                break;

            case 3:
                puyo[0][1] = puyo[1][2];
                puyo[1][2] = 0;
                break;

            case 4:
                puyo[1][2] = puyo[2][1];
                puyo[2][1] = 0;
                break;

            default:
                printf("回転エラー\n");
                break;
        }
    }
    //左回転
    else if(direction == 1){
        switch(place) {

            case 1:
                puyo[1][2] = puyo[0][1];
                puyo[0][1] = 0;
                break;
            
            case 2:
                puyo[0][1] = puyo[1][0];
                puyo[1][0] = 0;
                break;

            case 3:
                puyo[2][1] = puyo[1][2];
                puyo[1][2] = 0;
                break;

            case 4:
                puyo[1][0] = puyo[2][1];
                puyo[2][1] = 0;
                break;

            default:
                printf("回転エラー\n");
                break;
            }
    }
    
    //回転した結果，ステージとかぶってしまっている場合
    if(CheckOverlap(pStage, x, y)) {
        
        //左端の場合,右に１マスずらしても大丈夫だったらずらす
        if ( (x == 0)  && (!CheckOverlap(pStage, x+1, y)) ){
            x++;
            MovePuyo(pField, pStage, x, y);
            return 0;
        }

        //右端の場合,左に１マスずらしても大丈夫だったらずらす
        else if ( (x == WIDTH-1)  && (!CheckOverlap(pStage, x-1, y)) ){
            x--;
            MovePuyo(pField, pStage, x, y);
            return 0;
        }

        //それ以外の場合は回さなかったことにする
        memcpy(puyo, temp, sizeof(puyo));
        return 1;
    }

    ShowField(pField);
    return 0;
}

void DeletePuyo(FIELD *pField, FIELD *pStage){

    // 消去箇所が存在する間はループを続ける
    while (Check(pStage) != 0)
    {
        ShowField(pStage);
        Vanish(pStage);
        ShowField(pStage);

        // 空白を詰める為に１段ずつ落下させる
        FallPuyo(pField, pStage);
    }

    //ステージをフィールドと同じ状態  
    memcpy(pField, pStage, sizeof(FIELD));
}

void FallPuyo(FIELD *pField, FIELD *pStage){
    
    int i;

    // 空白を詰める為に１段ずつ落下させる
    while (Slide(pStage) != 0){
        
        for (i = 0; i < DELAY; i++);
        ShowField(pStage);
    }

    memcpy(pField, pStage, sizeof(FIELD));
}