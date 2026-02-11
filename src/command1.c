 /*
 * COMMAND1.C:
 *
 *      Command handling/parsing routines.
 *
 *      Copyright (C) 1991, 1992, 1993 Brett J. Vickers
 *
 */

#include "kstbl.h"
#include "mstruct.h"
#include "mextern.h"
#include "resource_path.h"
#include <ctype.h>

#ifndef WIN32
#include <unistd.h>
#endif

#include <time.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>


char pass_num[PMAX];
long last_login[PMAX];

/**********************************************************************/
/*                              login                                 */
/**********************************************************************/

/* This function is the first function that gets input from a player when */
/* he logs in.  It asks for the player's name and password, and performs  */
/* the according function calls.                                          */

void login(fd, param, str)
int     fd;
int     param;
unsigned char    *str;
{
	FILE *fp;
		int             i;
		extern int      Numplayers;
		unsigned char   tempstr[20], str2[50], str3[50], nastr[20];
		long            t;
		creature        *ply_ptr;
              struct stat f_stat;
              char            tmp[256];
char file[80];
              struct tm *login_tt;
              char *wday[7]={"일","월","화","수","목","금","토",};

		switch(param) {
	case -1: str[0]=0;
		case 0:
                            pass_num[fd]=0;
				if(strcmp(Ply[fd].extr->tempstr[0], str)) {
						/* disconnect(fd);
						return; */
						RETURN(fd, login, 1);
				}
				print(fd, "\n당신의 이름은 무엇입니까? ");
				RETURN(fd, login, 1);
		case 1:
				unsigned long name_len;

				if(!utf8_validate((unsigned char *)str, (unsigned long)strlen(str))) {
					print(fd, "이름이 UTF-8 형식이 아닙니다.\n");
					print(fd, "\n당신의 이름은 무엇입니까? ");
					RETURN(fd, login, 1);
				}

				name_len = utf8_codepoint_len((unsigned char *)str);
				if(name_len == 0) {
						print(fd, "이름은 한 자 이상이어야 합니다.\n");
					print(fd, "\n당신의 이름은 무엇입니까? ");
					RETURN(fd, login, 1);
				}

				if(name_len > 12) {
						print(fd, "이름이 너무 깁니다.\n\n");
						print(fd, "당신의 이름은 무엇입니까? ");
						RETURN(fd, login, 1);
				}

			if(!player_name_is_valid((unsigned char *)str, 1, 12)) {
				print(fd, "이름에 사용할 수 없는 문자가 있습니다.\n\n");
				print(fd, "당신의 이름은 무엇입니까? ");
				RETURN(fd, login, 1);
			}

				lowercize(str, 1);
				last_login[fd]=0;
				if(player_path_from_name((char *)str, tmp, sizeof(tmp)) == 0) {
					if (!rp_stat(tmp,&f_stat)) last_login[fd]=f_stat.st_ctime;
				}

				if(load_ply(str, &ply_ptr) < 0) {
						strcpy(Ply[fd].extr->tempstr[0], str);
						print(fd, "\n%S%j 하시겠습니까(예/아니오)? ", str,"4");
						RETURN(fd, login, 2);
				}

				else {
						ply_ptr->fd = -1;
						Ply[fd].ply = ply_ptr;
		  if(strcmp(str , Ply[fd].ply->name)) {

                        scwrite(fd, "\n데이타가 손상되었습니다.\n",25);
			if(str[0]==0 || pass_num[fd]>=3)
                                scwrite(fd,"접속을 끊습니다.",18);
				disconnect(fd);
				return;
			}
          if(F_ISSET(ply_ptr, SUICD)) {
/*
              uninit_ply(ply_ptr);
*/ 
                scwrite(fd, "\n자살 신청한 아이디입니다.\n", 18);
				disconnect(fd);
				return;
          } 
                                                if(checkdouble(ply_ptr->name)) {
                                                                scwrite(fd, "이상하군요.. -_-;.\n", 25);
								disconnect(fd);
								return;
						}
					  print(fd, "암호를 넣어 주십시요: ");
                                          print(fd,"%c%c%c", 255,251,1);
					  RETURN(fd, login, 3);
				}

		case 2:
				if(strcmp(str,"예") && str[0]!='y' && str[0]!='Y') {
						Ply[fd].extr->tempstr[0][0] = 0;
						print(fd, "당신의 이름은 무엇입니까? ");
						RETURN(fd, login, 1);
				}
				else {
						print(fd, "\n[엔터]를 누르십시요.");
						RETURN(fd, create_ply, 1);
				}

		case 3:
                check_item(Ply[fd].ply);      
				if(strcmp(str, Ply[fd].ply->password)) {
                                                scwrite(fd, "\n암호가 틀립니다. ", 18);
                                              pass_num[fd]++;
						if(str[0]==0 || pass_num[fd] >= 3) {
    							t = time(0);
							strcpy(str3, (char *)ctime(&t));
							str3[strlen(str3)-1] = 0;
							log_fl(str3, Ply[fd].ply->name);
                                                        scwrite(fd,"접속을 끊습니다.\n\n",18);
                                                        disconnect(fd);
                                                        return;
                                                }
                                                else {
                                                        print(fd,"다시 입력하십시요.\n암호를 다시 입력하세요: ");
                                                        RETURN(fd, login, 3);
                                                }
				}
				else {
						print(fd, "%c%c%c\n",255,252,1);
						strcpy(tempstr, Ply[fd].ply->name);
						for(i=0; i<Tablesize; i++)
								if(Ply[i].ply && i != fd)
										if(!strcmp(Ply[i].ply->name,
										   Ply[fd].ply->name))
												disconnect(i);
						free_crt(Ply[fd].ply);
				if(load_ply(tempstr, &Ply[fd].ply) < 0)
				{
                                                scwrite(fd, "Player nolonger exits!\n", 23);
						t = time(0);
						strcpy(str2, (char *)ctime(&t));
						str2[strlen(str2)-1] = 0;
						logn("sui_crash","%s: %s (%s) 는 자살하였습니다.\n",
								str2, Ply[fd].ply->name, Ply[fd].io->address);
						disconnect(fd);
						return;
				}
						Ply[fd].ply->fd = fd;


	                                                if(init_ply(Ply[fd].ply) < 0) {
								scwrite(fd, "\n서버 초기화 중 오류가 발생했습니다.\n",
									(int)strlen("\n서버 초기화 중 오류가 발생했습니다.\n"));
								disconnect(fd);
								return;
							}
	                                                init_alias(Ply[fd].ply);


             if(last_login[fd]) {
                 login_tt=localtime(&last_login[fd]);
                 print(fd,"\n마지막 접속시간: %d월 %d일(%s) %d시 %d분입니다.\n",
                 login_tt->tm_mon+1, login_tt->tm_mday, wday[login_tt->tm_wday],
                 login_tt->tm_hour, login_tt->tm_min);
            }

						RETURN(fd, command, 1);
				}
		}
}

/**********************************************************************/
/*                              create_ply                            */
/**********************************************************************/

/* This function allows a new player to create his or her character. */

void create_ply(fd, param, str)
int     fd;
int     param;
char    *str;
{
		int     i, k, l, n, sum;
		int     num[5];

		switch(param) {
		case 1:
				print(fd,"\n\n");
				Ply[fd].ply = (creature *)malloc(sizeof(creature));
				if(!Ply[fd].ply)
						merror("create_ply", FATAL);
				zero(Ply[fd].ply, sizeof(creature));
				Ply[fd].ply->fd = -1;
				Ply[fd].ply->rom_num = 1;
				print(fd, "당신은 남자입니까, 여자입니까(남자/여자)? ");
				RETURN(fd, create_ply, 2);
		case 2:
				if(strncmp(str,"남", 2) && strncmp(str,"여",2)) {
						print(fd, "입력이 잘못되었습니다.\n\n당신은 남자입니까, 여자입니까(남자/여자)? ");
						RETURN(fd, create_ply, 2);
				}
				if(!strncmp(str,"남",2))
						F_SET(Ply[fd].ply, PMALES);
				print(fd, "\n다음과 같은 직업이 있습니다.\n");
				print(fd, "1.자  객  2.권법가  3.불제자  4.검  사\n");
				print(fd, "5.도술사  6.무  사  7.포  졸  8.도  둑\n");
				print(fd, "직업을 고르세요: ");
				RETURN(fd, create_ply, 3);
		case 3:
				switch(low(str[0])) {
						case '1': Ply[fd].ply->class = ASSASSIN; break;
						case '2': Ply[fd].ply->class = BARBARIAN; break;
						case '3': Ply[fd].ply->class = CLERIC; break;
						case '4': Ply[fd].ply->class = FIGHTER; break;
						case '5': Ply[fd].ply->class = MAGE; break;
						case '6': Ply[fd].ply->class = PALADIN; break;
						case '7': Ply[fd].ply->class = RANGER; break;
						case '8': Ply[fd].ply->class = THIEF; break;
						default: print(fd, "직업을 고르세요: ");
								 RETURN(fd, create_ply, 3);
				}
				print(fd, "\n당신은 54점으로 다음 5가지 능력치를 구성할수 있습니다.\n\
3이상 18이하의 수치로 ## ## ## ## ##의 형식으로 5가지 능력치를 적어주십시요.\n\
능력: 힘 민첩 맷집 지식 신앙심\n예: 12 10 12 10 10\n\n");
/* Strength, Dexterity, Constitution, Intelligence, Piety. */

				print(fd, ": ");
				RETURN(fd, create_ply, 4);
		case 4:
				n = strlen(str); l = 0; k = 0;
				for(i=0; i<=n; i++) {
						if(str[i]==' ' || str[i]==0) {
								str[i] = 0;
								num[k++] = atoi(&str[l]);
								l = i+1;
						}
						if(k>4) break;
				}
				if(k<5) {
						print(fd, "5가지 능력치 모두를 위의 형식대로 적어 주십시요.\n");
						print(fd, ": ");
						RETURN(fd, create_ply, 4);
				}
				sum = 0;
				for(i=0; i<5; i++) {
						if(num[i] < 3 || num[i] > 18) {
								print(fd, "각 능력치는 3이상 18이하로 설정해야 합니다.\n");
								print(fd, ": ");
								RETURN(fd, create_ply, 4);
						}
						sum += num[i];
				}
				if(sum > 54) {
						print(fd, "각 능력치의 합이 54점을 초과할수 없습니다.\n");
						print(fd, ": ");
						RETURN(fd, create_ply, 4);
				}
				Ply[fd].ply->strength = num[0];
				Ply[fd].ply->dexterity = num[1];
				Ply[fd].ply->constitution = num[2];
				Ply[fd].ply->intelligence = num[3];
				Ply[fd].ply->piety = num[4];
				print(fd, "\n당신에게 익숙한 무기를 고르십시요.\n");
				print(fd, "1.도   2.검   3.봉   4.창   5.궁.\n");
				print(fd, ": ");
				RETURN(fd, create_ply, 5);
		case 5:
				switch(low(str[0])) {
						case '1': Ply[fd].ply->proficiency[0]=1024; break;
						case '2': Ply[fd].ply->proficiency[1]=1024; break;
						case '3': Ply[fd].ply->proficiency[2]=1024; break;
						case '4': Ply[fd].ply->proficiency[3]=1024; break;
						case '5': Ply[fd].ply->proficiency[4]=1024; break;
						default: print(fd, "다시 고르세요.\n: ");
								 RETURN(fd, create_ply, 5);
				}
				print(fd, "\n선한 구성원은 다른사람을 공격하지 못하고 공격 받을수도 없으며");
				print(fd, "\n그 구성원에게서 물건을 훔칠수도 없습니다.");
				print(fd, "\n그러나 악한 구성원은 공격할수도 있고 물건을 훔칠수도 있으며");
				print(fd, "\n다른 악한 구성원들에게 공격을 받을 수도 있습니다.\n");
				print(fd, "\n성향을 고르십시요(선함/악함): ");
				RETURN(fd, create_ply, 6);
		case 6:
				if(!strncmp(str,"악",2))
						F_SET(Ply[fd].ply, PCHAOS);
				else if(!strncmp(str,"선",2))
						F_CLR(Ply[fd].ply, PCHAOS);
				else {
						print(fd, "성향을 고르십시요(선함/악함): ");
						RETURN(fd, create_ply, 6);
				}
				print(fd, "\n다음과 같은 종족들이 있습니다.");
				print(fd, "\n1.난장이족  2.용 신 족  3.땅귀신족 4.요 괴 족");
				print(fd, "\n5.거 인 족  6.토 신 족  7.인 간 족 8.도깨비족");
				print(fd, "\n종족을 고르십시요: ");
				RETURN(fd, create_ply, 7);
		case 7:
				switch(low(str[0])) {
				case '1': Ply[fd].ply->race = DWARF; break;
				case '2': Ply[fd].ply->race = ELF; break;
				case '3': Ply[fd].ply->race = GNOME; break;
				case '8': Ply[fd].ply->race = ORC; break;
				case '4': Ply[fd].ply->race = HALFELF; break;
				case '5': Ply[fd].ply->race = HALFGIANT; break;
				case '6': Ply[fd].ply->race = HOBBIT; break;
				case '7': Ply[fd].ply->race = HUMAN; break;
				}
				if(!Ply[fd].ply->race) {
						print(fd, "\n종족을 고르십시요: ");
						RETURN(fd, create_ply, 7);
				}

				switch(Ply[fd].ply->race) {
				case DWARF:
						Ply[fd].ply->strength++;
						Ply[fd].ply->piety--;
						break;
				case ELF:
						Ply[fd].ply->intelligence+=2;
						Ply[fd].ply->constitution--;
						Ply[fd].ply->strength--;
						break;
				case GNOME:
						Ply[fd].ply->piety++;
						Ply[fd].ply->strength--;
						break;
				case HALFELF:
						Ply[fd].ply->intelligence++;
						Ply[fd].ply->constitution--;
						break;
				case HOBBIT:
						Ply[fd].ply->dexterity++;
						Ply[fd].ply->strength--;
						break;
				case HUMAN:
						Ply[fd].ply->constitution++;
						break;
				case ORC:
						Ply[fd].ply->strength++;
						Ply[fd].ply->constitution++;
						Ply[fd].ply->dexterity--;
						Ply[fd].ply->intelligence--;
						break;
				case HALFGIANT:
						Ply[fd].ply->strength+=2;
						Ply[fd].ply->intelligence--;
						Ply[fd].ply->piety--;
						break;
				}

				print(fd, "\n새 암호를 넣으십시요(3자이상 14자이하): ");
                                print(fd,"%c%c%c",255,251,1);
				RETURN(fd, create_ply, 8);
		case 8:
				if(strlen(str) > 14) {
						print(fd, "입력된 암호가 너무 깁니다.\n암호를 다시 넣으십시요(3자이상 14자이하): ");
						RETURN(fd, create_ply, 8);
				}
				if(strlen(str) < 3) {
						print(fd, "입력된 암호가 너무 짧습니다.\n암호를 다시 넣으십시요(3자이상 14자이하): ");
						RETURN(fd, create_ply, 8);
				}
				strncpy(Ply[fd].ply->password, str, sizeof(Ply[fd].ply->password) - 1);
				Ply[fd].ply->password[sizeof(Ply[fd].ply->password) - 1] = 0;
				strcpy(Ply[fd].ply->name, Ply[fd].extr->tempstr[0]);
				up_level(Ply[fd].ply);
				Ply[fd].ply->fd = fd;
				if(init_ply(Ply[fd].ply) < 0) {
					scwrite(fd, "\n서버 초기화 중 오류가 발생했습니다.\n",
						(int)strlen("\n서버 초기화 중 오류가 발생했습니다.\n"));
					disconnect(fd);
					return;
				}
	                               init_alias(Ply[fd].ply);
		F_SET(Ply[fd].ply,PLECHO);
		F_SET(Ply[fd].ply,PPROMP);
                Ply[fd].ply->gold = 500;
				save_ply(Ply[fd].ply->name, Ply[fd].ply);
				print(fd, "%c%c%c\n",255,252,1);

				print(fd, "[환영]이라고 치시면 초보자 분들에게 도움이 되는 많은 정보를 얻을수 있습니다.\n");
				print(fd, "레벨 5 가 되지 않으면 아이디가 삭제될 수도 있습니다.\n");

				RETURN(fd, command, 1);
		}
}

/**********************************************************************/
/*                              command                               */
/**********************************************************************/

/* This function handles the main prompt commands, and calls the        */
/* appropriate function, depending on what service is requested by the  */
/* player.                                                              */

void command(fd, param, str)
int     fd;
int     param;
char    *str;
{
		cmd     cmnd;
		int     n;
		unsigned char ch;
		int     i;
		char buf[256];

#ifdef RECORD_ALL
/*
this logn commands wil print out all the commands entered by players.
It should be used in extreme case hen trying to isolate a players
input which causes a crash.
*/

logn("all_cmd","\n%s-%d (%d): %s\n",Ply[fd].ply->name,fd,Ply[fd].ply->rom_num,str);
#endif /* RECORD_ALL */

		switch(param) {
		case 1:

				if(F_ISSET(Ply[fd].ply, PHEXLN)) {
						for(n=0;n<strlen(str);n++) {
								ch = str[n];
								print(fd, "%02X", ch);
						}
						print(fd, "\n");
				}

				if(!strcmp(str, "!"))
						strncpy(str, Ply[fd].extr->lastcommand, 79);
				else if(str[0]=='!') {
						strncpy(buf, Ply[fd].extr->lastcommand, 79);
						strncat(buf,&str[1],79);
						strncpy(str,buf,79);
				}

				if(str[0]) {
						for(n=0; str[n] && str[n] == ' '; n++) ;
						strncpy(Ply[fd].extr->lastcommand, &str[n], 79);
				}

				strncpy(cmnd.fullstr, str, 255);
				lowercize(str, 0);
				parse(str, &cmnd); n = 0;

				/* 파서 분석 시험용..  */
/*      print(fd,"파서 분석....\n");
		for(i=0;i<cmnd.num;i++) print(fd,"%d: %s %d\n",i+1,cmnd.str[i],cmnd.val[i]);
*/
				if(cmnd.num){
					if((n=alias_cmd(Ply[fd].ply,&cmnd))==4000) n = process_cmd(fd, &cmnd);
				} else
						n = PROMPT;

				if(n == DISCONNECT) {
                                                scwrite(fd, "안녕히 가십시요!\n\n\n", 19);
						disconnect(fd);
						return;
				}
				else if(n == PROMPT) {
						if(F_ISSET(Ply[fd].ply, PPROMP))
								sprintf(str, "(%d 체력 %d 도력): ",
										Ply[fd].ply->hpcur, Ply[fd].ply->mpcur);
						else
								strcpy(str, ": ");
                                                scwrite(fd, str, strlen(str));
                                                if(Spy[fd] > -1) scwrite(Spy[fd], str, strlen(str));
				}

				if(n != DOPROMPT) {
						RETURN(fd, command, 1);
				}
				else
						return;
		}
}

/**********************************************************************/
/*                              parse                                 */
/**********************************************************************/

/* This function takes the string in the first parameter and breaks it */
/* up into its component words, stripping out useless words.  The      */
/* resulting words are stored in a command structure pointed to by the */
/* second argument.                                                    */

void parse(str, cmnd)
char    *str;
cmd     *cmnd;
{
		int     i, j, l, m, n, o, art;
		unsigned char   tempstr[25];

		l = m = n = 0;
		j = strlen(str);

		/* include for processing hangul command */
		if(j!=0)
		if(str[j-1]==' ' || str[j-1]=='.' || str[j-1]=='!' || str[j-1]=='?') {
				strncpy(cmnd->str[n++],"말",20);
				cmnd->val[m] = 1L;
		}
		else {
				i=j;
				while(i>=0 && str[i]!=' ') i--;
				if(i>=0) str[i]=0;
				j=i; i++;
				strncpy(cmnd->str[n++], &str[i], 20);
				cmnd->val[m] = 1L;
		}
		/* --- end --- */

		for(i=0; i<=j; i++) {
				if(str[i] == ' ' || str[i] == '#' || str[i] == 0) {
						str[i] = 0;     /* tokenize */

						/* Strip extra white-space */
						while((str[i+1] == ' ' || str[i] == '#') && i < j+1)
								str[++i] = 0;

						strncpy(tempstr, &str[l], 24); tempstr[24] = 0;
						l = i+1;
						if(!strlen(tempstr)) continue;

						/* Ignore article/useless words */
						/*
						o = art = 0;
						while(article[o][0] != '@') {
								if(!strcmp(article[o++], tempstr)) {
										art = 1;
										break;
								}
						}
						if(art) continue;
						*/

						/* Copy into command structure */
						if(n == m) {
								strncpy(cmnd->str[n++], tempstr, 20);
								cmnd->val[m] = 1L;
						}
						else if(is_number(tempstr) || (tempstr[0] == '-' &&
								is_number(tempstr+1))) {
								cmnd->val[m++] = atol(tempstr);
						}
						else {
								strncpy(cmnd->str[n++], tempstr, 20);
								cmnd->val[m++] = 1L;
						}

				}
				if(m >= COMMANDMAX) {
						n = 5;
						break;
				}
		}

		if(n > m)
				cmnd->val[m++] = 1L;
		cmnd->num = n;

}

/**********************************************************************/
/*                              process_cmd                           */
/**********************************************************************/

/* This function takes the command structure of the person at the socket */
/* in the first parameter and interprets the person's command.           */

int process_cmd(fd, cmnd)
int     fd;
cmd     *cmnd;
{
		int     match=0, cmdno=0, c=0, n;
		int current_deep,match_deep=0;

		do {
				if(!strcmp(cmnd->str[0], cmdlist[c].cmdstr)) {
						match = 1;
						cmdno = c;
						break;
				}
				else if((current_deep=str_compare(cmnd->str[0], cmdlist[c].cmdstr))!=0) {
						match = 1;
						if(match_deep==0 || match_deep>current_deep) {
								cmdno = c;
								match_deep = current_deep;
						}
				}
				c++;
		} while(cmdlist[c].cmdno);

		if(match == 0 || (cmnd->str[0][0]=='*' &&
                  (Ply[fd].ply->class<CARETAKER &&
                   Ply[fd].ply->class!=ZONEMAKER))) {
				print(fd, "\"%s\": 이런 명령어는 없네요.",
					  cmnd->str[0]);
				Ply[fd].io->fn = command; Ply[fd].io->fnparam = 1; return(0);
		}
/* 가장 비슷한 명령어를 쓰기 위해 수정.. 여러 낱말이 나올경우 에러처리=>사용
		else if(match > 1) {
				print(fd, "명령을 잘 모르겠네요.");
				RETURN(fd, command, 1);
		}
*/
		if(cmdlist[cmdno].cmdno < 0)
				return(special_cmd(Ply[fd].ply, 0-cmdlist[cmdno].cmdno, cmnd));

		/* DM command log */
		if(cmnd->str[0][0]=='*') {
		    log_dmcmd("%s : %s :\n",Ply[fd].ply->name,cmnd->fullstr);
		}

		return((*cmdlist[cmdno].cmdfn)(Ply[fd].ply, cmnd));

}

#ifdef CHECKDOUBLE

int checkdouble(name)
char *name;
{
		char    path[128], tempname[80];
		FILE    *fp;
		int     rtn=0;

		sprintf(path, "%s/simul/%s", PLAYERPATH, name);
		fp = rp_fopen(path, "r");
		if(!fp)
				return(0);

		while(!feof(fp)) {
				fgets(tempname, 80, fp);
				tempname[strlen(tempname)-1] = 0;
				if(!strcmp(tempname, name))
						continue;
				if(find_who(tempname)) {
						rtn = 1;
						break;
				}
		}

		fclose(fp);
		return(rtn);
}

#endif

char cut_paste_chr;

int cut_command(str)
char *str;
{
	int i;
	
	for(i=strlen(str)-1;i>=0;i--) {
		if(str[i]==' ') {
			break;
		}
	}
	if(i<0) i=0;
	cut_paste_chr=str[i];
	str[i]=0;

	return i;
}

void paste_command(str,index)
char *str; int index;
{
	str[index]=cut_paste_chr;
}

int ishan(str)
unsigned char *str;
{
	unsigned long i, n, c, cp;

	if(!str)
		return 0;

	n = (unsigned long)strlen((char *)str);
	if(n == 0)
		return 0;

	i = 0;
	while(i < n) {
		if(!utf8_next_codepoint(str + i, n - i, &c, &cp))
			return 0;
		if(cp < 0xAC00UL || cp > 0xD7A3UL)
			return 0;
		i += c;
	}

	return 1;
}

int is_hangul(str)
unsigned char *str;    /* one character */
{
	unsigned long c, cp, n;

	if(!str)
		return 0;

	n = (unsigned long)strlen((char *)str);
	if(!utf8_next_codepoint(str, n, &c, &cp))
		return 0;
	if(cp < 0xAC00UL || cp > 0xD7A3UL)
		return 0;

	return 1;
}

int under_han(str)
unsigned char *str;
{
	unsigned char tmp[512];
	unsigned long len;
	char *p;

	if(!str)
		return 0;

	len = (unsigned long)strlen((char *)str);
	if(len == 0)
		return 0;
	if(len >= sizeof(tmp))
		len = sizeof(tmp) - 1;

	memcpy(tmp, str, len);
	tmp[len] = 0;

	if(len > 0 && tmp[len - 1] == ')') {
		p = strrchr((char *)tmp, '(');
		if(p)
			*p = 0;
	}

	return utf8_has_jongseong_last(tmp);
}


char *first_han(str)
unsigned char *str;
{
    unsigned char high,low;
    int len,i;
    char *p = "temp";
    static unsigned char *exam[]={
        "가", "가", "나", "다", "다",
        "라", "마", "바", "바", "사",
        "사", "아", "자", "자", "차", 
        "카", "타", "파", "하", "" };
    static unsigned char *johab_exam[]={
        "늏", "똞", "륾", "봞", "쁝",
        "쏿", "쟞", "쨅", "쮉", "촡",
        "캻", "큑", "퇫", "펈", "픞",
        "훍", "?a", "?a", "?a", "" };

    len=strlen(str);
    if(len<2) return p;

    high=str[0];
    low=str[1];

    if(!is_hangul(&str[0])) return p;
    high=(KStbl[(high-0xb0)*94+low-0xa1] >> 8) & 0x7c;
    for(i=0;johab_exam[i][0];i++) {
        low= (johab_exam[i][0] & 0x7f);
        if(low==high) return exam[i];
    }
    return p;
}

int is_number(str)
unsigned char *str;
{
	int i;
	for(i=0;i<strlen(str);i++) {
		if(!isdigit(str[i])) return 0;
	}
	return 1;
}

int return_square(ply_ptr,cmnd)
creature    *ply_ptr;
cmd     *cmnd;
{
	room    *rom_ply;
	int fd;
        ctag    *cp;

	rom_ply=ply_ptr->parent_rom;
	fd=ply_ptr->fd;

	if(ply_is_attacking(ply_ptr,cmnd)) {
		print(fd,"당신은 싸우고 있는 중입니다!!");
		return 0;
	}

	if(rom_ply->rom_num==1001) {
		print(fd,"당신은 이미 광장에 와 있습니다!");
		return 0;
	}

        if(ply_ptr->following) {
            cp = ply_ptr->following->first_fol;
        }
        else {
            cp = ply_ptr->first_fol;
        }
        if(cp){
            print(fd,"먼저 그룹에서 나오세요.");
            return(0);
        }

	if(ply_ptr->level>20 && ply_ptr->class<INVINCIBLE) {
		print(fd, "당신이 귀환하려하자 흑암의 세력이 당신의 도력을 뺏습니다.\n");
		ply_ptr->mpcur = 0;
		/*
		ply_ptr->experience -= (ply_ptr->experience)/100000;
		*/
	}


	print(fd, "당신이 \"귀환!\"이라고 외치자 이상한 힘에 의해 어딘가로 빨려들어갑니다.");
	if(!F_ISSET(ply_ptr,PDMINV)) broadcast_rom(fd,ply_ptr->rom_num,"\n%m님이 갑자기 사라집니다!",ply_ptr);

	del_ply_rom(ply_ptr,rom_ply);
	if(!F_ISSET(ply_ptr,PFRTUN))
		load_rom(1001,&rom_ply);
	else	
		load_rom(3300 + ply_ptr->daily[DL_EXPND].max, &rom_ply);
	add_ply_rom(ply_ptr,rom_ply);
	if(!F_ISSET(ply_ptr,PDMINV)) broadcast_rom(fd,ply_ptr->rom_num, "\n%m님이 갑자기 자욱한 연기와 함께 나타났습니다!",ply_ptr);
	return 0;
}

int ply_is_attacking(ply_ptr,cmnd)
creature        *ply_ptr;
cmd             *cmnd;
{
    room            *rom_ptr;
    ctag            *cp;
    creature        *crt_ptr;

    rom_ptr=ply_ptr->parent_rom;

    cp = rom_ptr->first_mon;
    while(cp) {
        if(find_enm_crt(ply_ptr->name,cp->crt)>-1) return 1;
        cp = cp->next_tag;
    }
    return 0;
}

int str_compare(str1,str2)
unsigned char *str1,*str2;
{
	unsigned long i, j, n1, n2, c1, c2, cp1, cp2;

	n1 = (unsigned long)strlen((char *)str1);
	n2 = (unsigned long)strlen((char *)str2);
	if(!utf8_validate(str1, n1) || !utf8_validate(str2, n2)) {
		i = 0;
		while(str1[i] != 0 && str2[i] != 0 && str1[i] == str2[i])
			i++;
		if(str1[i] != 0)
			return 0;
		return (int)i;
	}

	i = 0;
	j = 0;
	while(i < n1 && j < n2) {
		if(!utf8_next_codepoint(str1 + i, n1 - i, &c1, &cp1))
			break;
		if(!utf8_next_codepoint(str2 + j, n2 - j, &c2, &cp2))
			break;
		if(cp1 != cp2)
			break;
		i += c1;
		j += c2;
	}

	if(i != n1)
		return 0;
	return (int)i;
}

char *cut_space(str)
char *str;
{
	static char buf[512];
	int i;

	strcpy(buf,str);
	for(i=strlen(buf)-1;i>=0;i--) {
		if(buf[i]!=' ') break;
	}
	buf[i+1]=0;

	return buf;
}

