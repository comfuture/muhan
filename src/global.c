/*
 * GLOBAL.C:
 *
 *      Global variables.
 *
 *      Copyright (C) 1991, 1992, 1993 Brett J. Vickers
 *
 */

#include "mstruct.h"
#define MIGNORE
#include "mextern.h"
#include "board.h"

int             Tablesize;
int             Cmdnum;
long            Time;
struct lasttime Shutdown;
int             Spy[PMAX];
int             Numlockedout;
lockout         *Lockout;

struct {
        creature        *ply;
        iobuf           *io;
        extra           *extr;
} Ply[PMAX];

struct {
        short           hpstart;
        short           mpstart;
        short           hp;
        short           mp;
        short           ndice;
        short           sdice;
        short           pdice;
} class_stats[13] = {
        { 1,  1,  1,  1,  1,  1,  1},
        { 55,  40,  5,  2,  1,  6,  0},  /* assassin */
        { 57,  40,  7,  1,  2,  3,  1},  /* barbarian */
        { 54,  50,  4,  3,  1,  4,  0},  /* cleric */
        { 56,  50,  6,  1,  1,  5,  0},  /* fighter */
        { 54,  50, 4,  3,  1,  3,  0},  /* mage */
        { 55,  50,  5,  2,  1,  4,  0},  /* paladin */
        { 56,  40,  6,  2,  2,  2,  0},  /* ranger */
        { 55,  50,  5,  2,  2,  2,  1},  /* thief */
        { 400, 250, 4, 4,  2,  4,  0},  /* invincible */
        { 50, 50, 5, 5,  5,  5,  5},  /* caretaker */
        { 50, 50,  5,  5,  5,  5, 5}, /* sub_dm */
        { 50, 50, 7, 4,  5,  5,  5}   /* DM */
};

/*
int bonus[35] = { -4, -4, -4, -3, -3, -2, -2, -1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                   2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 };
*/
int bonus[64] = { -4, -4, -4, -3, -3, -2, -2, -1, -1, -1, 0, 0, 0, 0, 1, 1, 
                   1, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5,
                   5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7,
                   7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
};

char class_str[][15] = { "제작", "자객", "권법가", "불제자",
        "검사", "도술사", "무사", "포졸", "도둑", "무적", "초인",
        "운영자", "관리자" };

char race_str[][15] = { "Unknown", "난장이족", "용신족", "요괴족", "토신족",
                        "인간족", "도깨비족", "거인족", "땅귀신족", "개구리족" };

char race_adj[][15] = { "Unknown", "난장이족", "용신족", "요괴족",
                  "토신족", "인간족", "도깨비족", "거인족", "땅귀신족", "개구리족" };

int family_num[16];
char family_str[16][15];
char fmboss_str[16][15];
int family_gold[16];

short level_cycle[][10] = {
        { 0,   0,   0,   0,   0,   0,   0,   0,   0,   0   },
        { CON, PTY, STR, INT, DEX, INT, DEX, PTY, STR, DEX },
        { INT, DEX, PTY, CON, STR, CON, DEX, STR, PTY, STR },
        { STR, DEX, CON, PTY, INT, PTY, INT, DEX, CON, INT },
        { PTY, INT, DEX, CON, STR, CON, INT, STR, DEX, STR },
        { STR, DEX, PTY, CON, INT, CON, INT, DEX, PTY, INT },
        { DEX, INT, CON, STR, PTY, STR, INT, PTY, CON, PTY },
        { PTY, STR, INT, CON, DEX, CON, DEX, STR, INT, DEX },
        { INT, CON, PTY, STR, DEX, STR, CON, DEX, PTY, DEX },
        { STR, DEX, INT, CON, PTY, STR, DEX, INT, CON, PTY },
        { STR, DEX, INT, CON, PTY, STR, DEX, INT, CON, PTY },
        { STR, DEX, INT, CON, PTY, STR, DEX, INT, CON, PTY },
        { STR, DEX, INT, CON, PTY, STR, DEX, INT, CON, PTY }
};

short thaco_list[][20] = {
        { 20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20 }, 
/*a*/   { 18,18,18,17,17,16,16,15,15,14,14,13,13,12,12,11,10,10, 9, 9 },
/*b*/   { 20,19,18,17,16,15,14,13,12,11,10, 9, 8, 7, 6, 5, 4, 3, 3, 2 },
/*c*/   { 20,20,19,18,18,17,16,16,15,14,14,13,13,12,12,11,10,10, 9, 8 },
/*f*/   { 20,19,18,17,16,15,14,13,12,11,10, 9, 8, 7, 6, 5, 4, 3, 3, 3 },
/*m*/   { 20,20,19,19,18,18,18,17,17,16,16,16,15,15,14,14,14,13,13,11 },
/*p*/   { 19,19,18,18,17,16,16,15,15,14,14,13,13,12,11,11,10, 9, 8, 7 },
/*r*/   { 19,19,18,17,16,16,15,15,14,14,13,12,12,11,11,10, 9, 9, 8, 7 },
/*t*/   { 20,20,19,19,18,18,17,17,16,16,15,15,14,14,13,13,12,12,11,11 },
		{  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ -5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5 },
		{ -5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5 },
		{ -5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5 }
};

long quest_exp[] = {
        120, 500, 1000, 1000, 5000, 8000, 10000, 20000,
        50000, 80000, 100000, 200000, 500000, 800000, 2500, 2500,
        2500, 5, 5, 5, 125, 125, 125, 125,
        125, 125, 125, 125, 125, 125, 125, 125,
        125, 125, 125, 125, 125, 125, 125, 125,
        125, 125, 125, 125, 125, 125, 125, 125,
        125, 125, 125, 125, 125, 125, 125, 125,
        125, 125, 125, 125, 125, 125, 125, 125,
        125, 125, 125, 125, 125, 125, 125, 125,
        125, 125, 125, 125, 125, 125, 125, 125,
        125, 125, 125, 125, 125, 125, 125, 125,
        125, 125, 125, 125, 125, 125, 125, 125,
        125, 125, 125, 125, 125, 125, 125, 125,
        125, 125, 125, 125, 125, 125, 125, 125,
        125, 125, 125, 125, 125, 125, 125, 125,
        125, 125, 125, 125, 125, 125, 125, 125
};
 
long needed_exp[] = {
128, 256, 384, 512, /* 2 */
640, 768, 896, 1024, /* 6 */
1280, 1536, 1792, 2048, /* 10 */
2560, 3072, 3584, 4096, /* 14 */
5120, 6144, 7168, 8192, /* 18 */
10240, 12288, 14336, 16384, /* 22 */
20480, 24576, 28672, 32768, /* 26 */
40960, 49152, 57344, 65536, /* 30 */
74152, 82768, 91384, 100000, /* 34 */
111602, 123205, 134807, 146410, /* 38 */
161647, 176885, 192122, 207360, /* 42 */
234062, 260765, 287468, 314171, /* 46 */
350876, 387581, 424286, 460992, /* 50 */
510275, 559558, 608841, 658125, /* 54 */
715469, 772814, 830159, 887504, /* 58 */
966331, 1045159, 1123987, 1202815, /* 62 */
1327015, 1451215, 1575415, 1699616, /* 66 */
1825576, 1951536, 2077496, 2120345, /* 70 */
2272342, 2421228, 2570114, 2729000, /* 74 */
2875534, 3022069, 3178604, 3325139, /* 78 */
3475134, 3825129, 3975124, 4125120, /* 82 */
4272005, 4428890, 4570775, 4722661, /* 86 */
4894263, 5045866, 5197469, 5529072, /* 90 */
5857897, 6196723, 6435549, 6774375, /* 94 */
7005781, 7337187, 7676859, 7984959, /* 98 */
8229756, 9079416, 9441769, 10000000, /* 102 */
12728240, 14066499, 16511819, 18072765, /* 106 */
20758586, 22579273, 24545614, 26669264, /* 110 */
28962805, 30439829, 32115015, 34920766, /* 114 */
36333635, 39053662, 43108492, 48528256, /* 118 */
54345799, 60596921, 65320644, 71559502, /* 122 */
76359857, 86000000, 100000000, 190000000, /* 126 */
};

/*

        512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 100000,
     146410, 207360, 314171, 460992, 658125, 887504, 1202815,
    1699616, 2203457, 2799000, 3505139, 4465120, 5792661, 7319072,
    8874375, 2000000};
*/

char lev_title[][8][20] = {
        { "", "", "", "", "", "", "", "" },
        { "깡패", "강도", "살인자", "도살자",
          "왕자객", "응징자", "살성", "살신" },
        { "초보", "수련생", "무협", "철권",
          "권성", "권황", "지존", "무신" },
        { "땡중", "사미승", "소승", "감찰승",
          "주지", "대사", "국사", "부처" },
        { "백정", "칼잡이", "무인", "용병",
          "검객", "검성", "검황", "무림파천" },
        { "심부름꾼", "도제자", "마술사", "도객",
          "마인", "도인", "신선", "마존" },
        { "골목대장", "무객", "협객", "의협",
          "정전자", "용전사", "수호자", "성전사" },
        { "쫄따구", "순찰병", "감찰원", "도성지기",
          "포교", "포도대장", "감찰어사", "감찰장군" },
        { "바늘도둑", "개도둑", "좀도둑", "소도둑",
          "왕도둑", "도성", "도신", "신수" },
        { "무적", "무적", "무적", "무적", "무적",
          "무적", "무적", "무적"},
        { "초인", "초인", "초인", "초인", "초인",
          "초인", "초인", "초인"},
        { "도우미", "도우미", "도우미", "도우미",
          "도우미", "도우미", "도우미", "도우미" },
        { "바보", "멍청이", "또라이", "머저리",
          "띨띨이", "왕바보", "백치황제", "바보들의신"  },
};
struct {
        char    *cmdstr; 
        int     cmdno; 
        int     (*cmdfn)(); 
} cmdlist[] = { 
        { "나가는길", 1, move },
        { "광장", 1, move },
        { "향로", 1, move },
        { "수련장", 1, move },
        { "현감에게", 1, move },
        { "면회허가", 1, move },
        { "반하도인", 1, move},
        { "월광반", 1, move }, 
        { "반야바라밀", 1, move },
        { "8", 1, move },
        { "북", 1, move },
        { "ㅂ", 1, move },
        { "�ぃ�", 1, move },
        { "2", 1, move },
        { "남", 1, move },
        { "ㄴ", 1, move },
        { "�ⅲ�", 1, move },
        { "6", 1, move },
        { "동", 1, move },
        { "ㄷ", 1, move },
        { "�┌�", 1, move },
        { "4", 1, move },
        { "서", 1, move },
        { "ㅅ", 1, move },
        { "�В�", 1, move },
        { "북동", 1, move },
        { "ㅂㄷ", 1, move },
        { "북서", 1, move },
        { "북동", 1, move },
        { "남서", 1, move },
        { "ㅂㅅ", 1, move },
        { "남동", 1, move },
        { "ㄴㄷ", 1, move },
        { "남서", 1, move },
        { "ㄴㅅ", 1, move },
        { "9", 1, move },
        { "위", 1, move },
        { "ㅇ", 1, move },
        { "��＂", 1, move },
        { "3", 1, move },
        { "밑", 1, move },
        { "ㅁ", 1, move },
        { "�ð�", 1, move },
        { "밖", 1, move },
        { "나가", 1, move }, /* leave */
        { "봐", 2, look },     /* look */  
        { "보다", 2, look },
        { "보아", 100, action },
        { "조사", 2, look }, /* examine */
        { "끝", 3, quit }, /* quit */
        { "말", 4, say },
        { "\"", 4, say },
        { "'", 4, say },
        { "주워", 5, get }, /* get */
        { "주", 5, get },
        { "가져", 5, get }, /* take */
        { "꺼내", 5, get }, /* 추가 */
        { "소지품", 6, inventory }, /* inventory */
        { "버려", 7, drop }, /* drop */
        { "넣어", 7, drop }, /* put */
        { "누구", 8, who }, /* who */
        { "입어", 9, wear }, /* wear */
        { "벗어", 10, remove_obj }, /* remove */
        { "장비", 11, equipment }, /* equipment */
        { "장", 11, equipment },
        { "쥐어", 12, hold }, /* hold */
        { "잡아", 12, hold },
        { "무장", 13, ready }, /* wield */
        { "도움말", 14, help }, /* 도움말 */
        { "?", 14, help },
        { "건강", 15, health }, /* health */
        { "점수", 15, health }, /* score */
        { "정보", 16, info }, /* information */
        { "얘기", 17, sendman }, /* send */
        { "이야기", 17, sendman }, /* tell */
        { "따라", 18, follow }, /* follow */
        { "내보내", 19, lose },
        { "그룹", 20, group }, /* group */
        { "무리", 20, group }, /* party */
        { "추적", 21, track },
        { "엿봐", 22, peek }, /* peek */
        { "공격", 23, attack }, /* attack */
        { "공", 23, attack },
        { "쳐", 23, attack }, /* kill */
        { "때려", 23, attack }, /* k */
        { "검색", 24, search }, /* search */
        { "찾아", 24, search },
        { "표현",25, emote},
        { "숨겨", 26, hide },
        { "숨어", 26, hide },
        { "설정", 27, set }, /* set */
        { "해제", 28, clear }, /* clear */
        { "외쳐", 29, yell }, /* yell */
        { "가", 30, go }, /* go */
        { "들어가", 30, go },
        { "열어", 31, openexit }, /* open */
        { "닫아", 32, closeexit }, /* close */
        { "풀어", 33, unlock }, /* 풀어 */
        { "잠궈", 34, lock }, /* lock */
        { "따", 35, picklock }, /* pick */
        { "훔쳐", 36, steal }, /* steal */
        { "도망", 37, flee }, /* flee */
        { "도", 37, flee },
        { "주문", 38, cast }, /* cast */
        { "배워", 39, study },
        { "연마", 39, study },
        { "읽어", 40, readscroll },
        { "품목", 41, list },
        { "사", 42, buy },
        { "팔아", 43, sell },
        { "가치", 44, value },
        { "가격", 44, value },
        { "기습", 45, backstab },
        { "수련", 46, train },
        { "줘", 47, give },
        { "수리", 48, repair },
        { "시간", 49, prt_time },
        { "교란", 50, circle },
        { "맹공", 51, bash },
        { "저장", 52, savegame }, /* save */
        { "편지보내기", 53, postsend },
        { "편지삭제", 55, postdelete },
        { "편지받기", 54, postread },
        { "대화", 56, talk },
        { "그룹말",57, gtalk }, /* gtalk */
        { "무리말",57, gtalk },
        { "=", 57, gtalk },
        { "마셔", 58, drink }, /* drink */
        { "먹어", 58, drink },
        { "잡담", 59, broadsend },
        { "잡", 59, broadsend },
		{ "환호", 70, broadsend2 },
        { "zap", 60, zap },
        { "환영", 61, welcome },
        { "방혼술", 62, turn },
		{ "보관물", 63, bank_inv },
		{ "잔액", 63, bank },
		{ "입금", 63, deposit },
		{ "출금", 63, withdraw },
		{ "받아", 63, output_bank },
        { "활보법", 64, haste },
        { "신원법", 65, pray }, /* pray */
        { "경계", 66, prepare },
        { "사용", 67, use }, /* use */
        { "듣기거부", 68, ignore }, /* ignore */
        { "사용자검색", 69, whois },
		{ "묘사", 73, description },
/*        { "은신술", 70, sneak },    */
        { "가르쳐", 71, teach },
		{ "선전포고", 72, call_war },
/*
        { "가입",72, pledge },
        { "탈퇴",73, rescind },
*/
        { "구입",74, purchase },
        { "선택",75, selection },
        { "교환",76, trade},
        { "목매달기", 77, ply_suicide }, /* suicide */
        { "암호", 78, passwd}, /* passwd */
        { "투표",79,vote},
        { "사용자정보", 80, pfinger},
        { "귀환", 81, return_square },
        { "귀", 81, return_square },
        { "줄임말", 82, ply_aliases },
        { "줄", 82, ply_aliases },
        { "태워", 83, burn },
        { "소각", 83, burn },
        { "칭호", 84, set_title },
        { "칭호삭제", 84, clear_title },
        { "제련", 85, forge },
	{ "무기만들기", 85, newforge }, 
        { "직업전환", 86, change_class },
        { "기공집결", 87, power },
        { "살기충전", 88, accurate },
        { "흡성대법", 89, absorb },
        { "참선", 90, meditate },
        { "혈도봉쇄", 91, magic_stop },
        { "써", 92, writeboard },
        { "글삭제", 93, del_board },
        { "게시판", 94, look_board },
        { "명명", 95, chg_name },
        { "감정", 96, info_obj },
        { "비교", 96, obj_compare },
        { "차기", 97, kick },
        { "독살포", 98, poison_mon },
        { "잠력격발", 99, up_dmg },
        { "감정표현", 100, action },
        { "노려봐", 100, action },
        { "끄덕", 100, action },
        { "응", 100, action },
        { "아니", 100, action },
        { "감", 100, action },
        { "감사", 100, action },
        { "미소", 100, action },
        { "청혼", 100, action },
        { "떨어", 100, action },
        { "해", 100, action },
        { "하품", 100, action },
        { "웃어", 100, action },
        { "미안", 100, action },
        { "악수", 100, action },
        { "하이파이브", 100, action },
        { "박수", 100, action },
        { "흡연", 100, action },
        { "담배", 100, action },
        { "절", 100, action },
        { "찔러", 100, action },
        { "춤", 100, action },
        { "노래", 100, action },
        { "울어", 100, action },
        { "달래", 100, action },
        { "당황", 100, action },
        { "생각", 100, action },
        { "부끄러", 100, action },
        { "놀려", 100, action },
        { "설레", 100, action },
        { "잘가", 100, action },
        { "바이", 100, action },
        { "안녕", 100, action },
        { "뽀뽀", 100, action },
        { "윙크", 100, action },
        { "구걸", 100, action },
	{ "구박", 100, action },
	{ "안아", 100, action },
	{ "껴안아", 100, action },
/****************************************************************/
/*  { "전수", 149, trans_exp },    */
/****************************************************************/
	{ "향상", 149, buy_states },
	{ "결혼", 150, marriage },
	{ "사랑말", 150, m_send },
    { "이혼", 150, divorce },   
    { "기억", 153, moon_set }, 
	{ "메모", 151, memo },
	{ "초대", 152, invite },
    { "상태", 154, enemy_status },
	{ "패거리누구", 148, family_who },
	{ "패거리공지", 148, family_news },
	{ "패거리가입", 148, family }, 
	{ "가입허가", 148, boss_family },
	{ "패거리탈퇴", 148, out_family },
	{ "패거리추방", 148, fm_out },
	{ "패거리원", 148, family_member },
	{ "패거리말", 148, family_talk },
	{ "]", 148, family_talk },
	{ "모든패거리", 148, list_family },
	{ "대답", 153, resend },
	{ "/", 153, resend },
        { "*teleport", 101, dm_teleport },
        { "*순간이동", 101, dm_teleport },
        { "*방번호", 102, dm_rmstat },
        { "*rm", 102, dm_rmstat },
        { "*방", 102, dm_rmstat },
        { "*reload", 103, dm_reload_rom },
        { "*로드", 103, dm_reload_rom },
        { "*save", 104, dm_resave },
        { "*세이브", 104, dm_resave },
        { "*create", 105, dm_create_obj },
        { "*뭐든지다만들어", 105, dm_create_obj},
        { "*c", 105, dm_create_obj },
        { "*perm", 106, dm_perm },
        { "*영원", 106, dm_perm },
        { "*invis", 107, dm_invis },
        { "*바람처럼사라져", 107, dm_invis },
        { "*i", 107, dm_invis },
        { "*s", 108, dm_send },
        { "*send", 108, dm_send },
        { "*공지", 108, dm_send },
        { "*purge", 109, dm_purge },
        { "*청소", 109, dm_purge },
        { "*청", 109, dm_purge },
        { "*ac", 110, dm_ac },
        { "*방어력", 110, dm_ac },
        { "*users", 111, dm_users },
        { "*누", 111, dm_users },
        { "*누구", 111, dm_users },
        { "*echo", 112, dm_echo },
        { "*말", 112, dm_echo },
        { "*flushrooms", 113, dm_flushsave },
        { "*모든저장", 113, dm_flushsave },
        { "*shutdown", 114, dm_shutdown },
        { "*종료", 114, dm_shutdown },
        { "*f", 115, dm_force },
        { "*force", 115, dm_force },
        { "*뭐든지다시켜", 115, dm_force },
        { "*flushcrtobj", 116, dm_flush_crtobj },
        { "*재설정", 116, dm_flush_crtobj },
        { "*재", 116, dm_flush_crtobj },
        { "*monster", 117, dm_create_crt },
        { "*괴물", 117, dm_create_crt },
        { "*괴", 117, dm_create_crt },
        { "*status", 118, dm_stat },
        { "*상태", 118, dm_stat },
        { "*add", 119, dm_add_rom },
        { "*방제작", 119, dm_add_rom },
        { "*뭐든지다해", 120, dm_set },
        { "*log", 121, dm_log },
        { "*접속", 121, dm_log },
        { "*spy", 122, dm_spy },
        { "*뭐든지다엿봐", 122, dm_spy },
        { "*lock", 123, dm_loadlockout },
        { "*제한", 123, dm_loadlockout },
        { "*finger", 124, dm_finger },
        { "*핑거", 124, dm_finger },
        { "*list", 125, dm_list },
        { "*누구든지다봐", 125, dm_list }, 
        { "*info", 126, dm_info },
        { "*정보", 126, dm_info },
        { "*parameter", 127, dm_param },
        { "*수치", 127, dm_param },
        { "*silence", 128, dm_silence},
        { "*벙어리", 128, dm_silence},
        { "*broad", 129, dm_broadecho},
        { "*방송", 129, dm_broadecho },
        { "*replace",130, dm_replace},
        { "*교체", 130, dm_replace},
        { "*name",131, dm_nameroom},
        { "*방이름", 131, dm_nameroom},
        { "*append",132, dm_append},
        { "*추가", 132, dm_append},
        { "*prepend",133, dm_prepend},
        { "*서언", 133, dm_prepend},     
        { "*gcast",134, dm_cast},
        { "*전주문", 134, dm_cast},
        { "*group",135, dm_group},
        { "*그룹", 135, dm_group},
        { "*notepad",136, notepad},
        { "*메모", 136, notepad},
        { "*delete",137, dm_delete},
        { "*지우기", 137, dm_delete},
        { "*oname", 138, dm_obj_name },
        { "*뭐든지다바꿔", 138, dm_obj_name },
        { "*cname", 139, dm_crt_name },
        { "*괴물이름", 139, dm_crt_name },
        { "*active", 140, list_act },
        { "*활성", 140, list_act },
        { "*dust", 141, dm_dust },
        { "*나도가끔화낸다", 141, dm_dust },
        { "*cfollow", 142, dm_follow },
        { "*따르기", 142, dm_follow },
        { "*떨어져라", 148, dm_moonstone },
        { "*침공", 148, dm_monster },
        { "*dmhelp", 143, dm_help },
        { "*도움말", 143, dm_help },
        { "*attack", 144, dm_attack },
        { "*공격", 144, dm_attack },
        { "*enemy", 145, list_enm },
        { "*적", 145, list_enm },
        { "*charm", 146, list_charm },
        { "*최면", 146, list_charm },
        { "*사용자저장", 147, dm_save_all_ply },
        { "눌러", -2, 0 },
        { "밀어", -2, 0 },
        { "@", 0, 0 }
};

char article[][10] = {
                  "the",
                  "from",
                  "to",
                  "with",
                  "an",
                  "in",
                  "for",
                  "@"
};

struct {
                  char    *splstr;
                  int     splno;
                  int     (*splfn)();
                  int	spllv;
} spllist[] = {
                  { "회복", SVIGOR, vigor, 1 },
                  { "삭풍", SHURTS, offensive_spell, 2 },
                  { "발광", SLIGHT, light, 2 },
                  { "해독", SCUREP, curepoison, 1 },
                  { "성현진", SBLESS, bless, 2 },
                  { "수호진", SPROTE, protection, 2 },
                  { "화궁", SFIREB, offensive_spell, 3 },
                  { "은둔법", SINVIS, invisibility, 5 },
                  { "도력반", SRESTO, restore, 4 },
                  { "은둔감지술", SDINVI, detectinvis, 3 },
                  { "주문감지술", SDMAGI, detectmagic, 3 },
                  { "축지법", STELEP, teleport, 4 },
                  { "혼동", SBEFUD, befuddle, 3 },
                  { "뇌전", SLGHTN, offensive_spell, 4 },
                  { "동설주", SICEBL, offensive_spell, 5 },
                  { "빙의", SENCHA, enchant, 4 },
                  { "귀환", SRECAL, recall, 3 },
                  { "소환", SSUMMO, summon, 3 },
                  { "원기회복", SMENDW, mend, 3 },
                  { "완치", SFHEAL, heal, 5 },
                  { "추적", STRACK, magictrack, 4 },
                  { "부양술", SLEVIT, levitate, 3 },
                  { "방열진", SRFIRE, resist_fire, 3 },
                  { "비상술", SFLYSP, fly, 4 },
                  { "보마진", SRMAGI, resist_magic, 3 },
                  { "권풍술", SSHOCK, offensive_spell, 5 },
                  { "지동술", SRUMBL, offensive_spell, 2 },
                  { "화선도", SBURNS, offensive_spell, 2 },
                  { "탄수공", SBLIST, offensive_spell, 2 },
                  { "풍마현", SDUSTG, offensive_spell, 3 },
                  { "파초식", SWBOLT, offensive_spell, 3 },
                  { "폭진", SCRUSH, offensive_spell, 3 },
                  { "낙석", SENGUL, offensive_spell, 5 },
                  { "화풍술", SBURST, offensive_spell, 3 },
                  { "화룡대천", SSTEAM, offensive_spell, 3 },
                  { "토합술", SSHATT, offensive_spell, 4 },
                  { "주작현", SIMMOL, offensive_spell, 4 },
                  { "열사천", SBLOOD, offensive_spell, 4 },
                  { "파천풍", STHUND, offensive_spell, 5 },
                  { "지옥패", SEQUAK, offensive_spell, 5 },
                  { "태양안", SFLFIL, offensive_spell, 5 },
                  { "선악감지", SKNOWA, know_alignment, 3 },
                  { "저주해소", SREMOV, remove_curse, 4 },
                  { "방한진", SRCOLD, resist_cold, 3 },
                  { "수생술", SBRWAT, breathe_water, 3 },
                  { "지방호", SSSHLD, earth_shield, 3 },
                  { "천리안", SLOCAT, locate_player, 4 },
                  { "백치술", SDREXP, drain_exp, 5 },
                  { "치료", SRMDIS, rm_disease, 3 },
                  { "개안술", SRMBLD, rm_blind, 3 },
                  { "공포", SFEARS, fear, 4 },
                  { "전회복", SRVIGO, room_vigor, 4 },
                  { "전송", STRANO, object_send, 4 },
                  { "실명", SBLIND, blind, 5 },
                  { "봉합구", SSILNC, silence, 5 },
                  { "이혼대법", SCHARM, charm, 5 },
/*                { "변수나한권", SNAHAN, offensive_spell },     */
                  { "@", -1,0,0 }
};

struct osp_t ospell[] = {
                  { SHURTS,  WIND,  3, 1, 8,  0, 1 },     /* hurt */
                  { SRUMBL, EARTH,  3, 1, 8,  0, 1 },     /* rumble */
                  { SBURNS,  FIRE,  3, 1, 7,  1, 1 },     /* burn */
                  { SBLIST, WATER,  3, 1, 8,  0, 1 },     /* blister */

                  { SDUSTG,  WIND,  7, 2, 5,  7, 2 },     /* dustgust */
                  { SCRUSH, EARTH,  7, 2, 5,  7, 2 },     /* stonecrush */
                  { SFIREB,  FIRE,  7, 2, 5,  8, 2 },     /* fireball */
                  { SWBOLT, WATER,  7, 2, 5,  8, 2 },     /* waterbolt */

                  { SSHOCK,  WIND, 10, 2, 5, 13, 2 },     /* shockbolt */
                  { SENGUL, EARTH, 10, 2, 5, 13, 2 },     /* engulf */
                  { SBURST,  FIRE, 10, 2, 5, 13, 2 },     /* burstflame */
                  { SSTEAM, WATER, 10, 2, 5, 13, 2 },     /* steamblast */

                  { SLGHTN,  WIND, 15, 3, 4, 18, 3 },     /* lightning */
                  { SSHATT, EARTH, 15, 3, 4, 19, 3 },     /* shatterstone */
                  { SIMMOL,  FIRE, 15, 3, 4, 18, 3 },     /* immolate */
                  { SBLOOD, WATER, 15, 3, 4, 18, 3 },     /* bloodboil */

                  { STHUND,  WIND, 25, 4, 5, 30, 3 },     /* thuderbolt */
                  { SEQUAK, EARTH, 25, 4, 5, 30, 3 },     /* earthquake */
                  { SFLFIL,  FIRE, 25, 4, 5, 30, 3 },     /* flamefill */
                  { SICEBL, WATER, 25, 4, 5, 30, 3 },     /* iceblade */
/*                { SNAHAN,  WIND, 10, 2, 5, 13, 2 },  */ /* 변수나한권 */
                  { -1, 0, 0, 0, 0, 0, 0 }
};

char number[][10] = {
                  "zero",
                  "one",
                  "two",
                  "three",
                  "four",
                  "five",
                  "six",
                  "seven",
                  "eight",
                  "nine",
                  "ten",
                  "eleven",
                  "twelve",
                  "thirteen",
                  "fourteen",
                  "fifteen",
                  "sixteen",
                  "seventeen",
                  "eighteen",
                  "nineteen",
                  "twenty"
};


