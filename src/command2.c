/*
 * COMMAND2.C:
 *
 *      Addition user command routines.
 *
 *      Copyright (C) 1991, 1992, 1993 Brett J. Vickers
 *
 */

#include "mstruct.h"
#include "mextern.h"
#include "board.h"
#include <stdlib.h>

#ifndef WIN32

#include <sys/time.h>
#else
#include <time.h>

#endif

#include <string.h>

extern long login_time[PMAX];

/**********************************************************************/
/*                              look                                  */
/**********************************************************************/

/* This function is called when a player tries to look at the room he */
/* is in, or at an object in the room or in his inventory.            */

int look(ply_ptr, cmnd)
creature        *ply_ptr;
cmd             *cmnd;
{
    room            *rom_ptr;
	room			*new_rom;
    object          *obj_ptr;
	object			*bnk_ptr;
    creature        *crt_ptr;
	exit_			*ext_ptr;
	char	file[80];
    char            str[2048];
    int             fd, n, match=0;

    if(cmnd->num < 2) {
        display_rom(ply_ptr, ply_ptr->parent_rom);
        return(0);
    }

    fd = ply_ptr->fd;
    if(F_ISSET(ply_ptr, PBLIND)) {
        ANSI(fd, BOLD);
        ANSI(fd, RED);
        print(fd, "당신은 눈이 멀어 있습니다!");
        ANSI(fd, WHITE);
        ANSI(fd, NORMAL);
        return(0);
    }
    rom_ptr = ply_ptr->parent_rom;
	ext_ptr=find_ext(ply_ptr, rom_ptr->first_ext, 
					cmnd->str[1], cmnd->val[1]);
	if(ext_ptr) {
		if(F_ISSET(ext_ptr, XCLOSD)) {
			print(fd, "그 출구는 닫혀 있습니다.");
			return(0);
		}
		sprintf(file, "%s/r%02d/r%05d", ROOMPATH,ext_ptr->room/1000, ext_ptr->room);
		if(!file_exists(file)) {
			print(fd, "지도가 없습니다.");
			return(0);
		}
		load_rom(ext_ptr->room, &new_rom);
		if(!new_rom || rom_ptr == new_rom) {
			print(fd, "지도가 없습니다.");
			return(0);
		}
		if(F_ISSET(new_rom, RONMAR) || F_ISSET(new_rom, RONFML)) {
			print(fd, "그 방은 볼 수가 없습니다.");
			return(0);
		}
		display_rom(ply_ptr, new_rom);
		return(0);
	}

    obj_ptr = find_obj(ply_ptr, ply_ptr->first_obj, cmnd->str[1],
               cmnd->val[1]);

    if(!obj_ptr || !cmnd->val[1]) {
        for(n=0; n<MAXWEAR; n++) {
            if(!ply_ptr->ready[n])
                continue;
            if(EQUAL(ply_ptr->ready[n], cmnd->str[1]))
                match++;
            else continue;
            if(cmnd->val[1] == match || !cmnd->val[1]) {
                obj_ptr = ply_ptr->ready[n];
                break;
            }
        }
    }

    if(!obj_ptr)
        obj_ptr = find_obj(ply_ptr, rom_ptr->first_obj,
                   cmnd->str[1], cmnd->val[1]);

    if(obj_ptr) {

        if(obj_ptr->special==SP_BOARD) {
            board_obj[fd]=obj_ptr;
            list_board(fd,0,"");
            return(DOPROMPT);
        }
        if(obj_ptr->special) {
            n = special_obj(ply_ptr, cmnd, SP_MAPSC);
            if(n != -2) return(MAX(n, 0));
        }

        if(obj_ptr->description[0])
            print(fd, "%s\n", obj_ptr->description);
        else
            print(fd, "특별한 점이 없습니다.\n");

        if(F_ISSET(ply_ptr, PKNOWA)) {
            if(F_ISSET(obj_ptr, OGOODO))
                print(fd, "푸른 광채가 뻗어 나오고 있습니다.\n");
            if(F_ISSET(obj_ptr, OEVILO))
                print(fd, "붉은 광채가 뻗어 나오고 있습니다.\n");
        }

        if(F_ISSET(obj_ptr, OCONTN)) {
            strcpy(str, "내용물: ");
            n = list_obj(&str[8], ply_ptr, obj_ptr->first_obj);
            if(n)
                print(fd, "%s.\n", str);
        }

        if(obj_ptr->type <= MISSILE) {
            print(fd, "%I%j ", obj_ptr,"0");
            switch(obj_ptr->type) {
            case SHARP: print(fd, "매우 날카로운 '도'입니다.\n"); break;
            case THRUST: print(fd, "매우 공격적인 '검'입니다.\n"); break;
            case POLE: print(fd, "날이 바짝 선 '창'입니다.\n"); break;
            case BLUNT: print(fd, "매우 위력적인 '봉'입니다.\n"); break;
            case MISSILE: print(fd, "매우 강력하게 보이는 '궁'입니다.\n"); break;
            }
        }

        if(obj_ptr->type <= MISSILE || obj_ptr->type == ARMOR ||
           obj_ptr->type == LIGHTSOURCE || obj_ptr->type == WAND ||
           obj_ptr->type == KEY) {
            if(obj_ptr->shotscur < 1)
                print(fd, "그것은 부서져 버렸거나 다 써버렸습니다.\n");
            else if(obj_ptr->shotscur <= obj_ptr->shotsmax/10)
                print(fd, "그것은 곧 부서질것 같습니다.\n");
        }

        return(0);
    }

    if(!strcmp(cmnd->str[1],"나")) crt_ptr=ply_ptr;
    else crt_ptr = find_crt(ply_ptr, rom_ptr->first_mon, cmnd->str[1],
               cmnd->val[1]);
    if(crt_ptr) {

        if(!strncmp(crt_ptr->name, ply_ptr->name, strlen(crt_ptr->name))) {
          print(fd,"당신은 거울을 들고 자신을 봅니다.\n");
          broadcast_rom(fd,rom_ptr->rom_num,
             "\n%M%j 거울을 들고 자신을 바라 봅니다.",ply_ptr,"1");
        }
        else {
          print(fd, "당신은 %1M%j 봅니다.\n", crt_ptr,"3");
          broadcast_rom(fd,rom_ptr->rom_num,
             "\n%M%j %M%j 봅니다.",ply_ptr,"1",crt_ptr,"3");
        }
        if(crt_ptr->type == PLAYER && F_ISSET(crt_ptr, PMARRI)) {
            print(fd, "%s는 %s님과 결혼한 기혼자입니다.\n", 
                    F_ISSET(crt_ptr, PMALES) ? "그":"그녀", &crt_ptr->key[2][1]);
        }            
        
		if(crt_ptr->type != PLAYER) {
        if(crt_ptr->description[0])
            print(fd, "%s\n", crt_ptr->description);
        else
            print(fd, "특별한 것은 보이지 않습니다.\n");
		}
		else {
			print(fd, "%s는 %s서 있습니다.\n", F_ISSET(crt_ptr, PMALES) ? "그":"그녀",
				crt_ptr->description);
		}
        if(F_ISSET(ply_ptr, PKNOWA) && crt_ptr->alignment!=0) {
            print(fd, "%s에게서 ",
                F_ISSET(crt_ptr, MMALES) ? "그":"그녀");
            if(crt_ptr->alignment < 0)
                print(fd, "붉은 광채");
            else print(fd, "푸른 광채");
            print(fd, "가 뻗어 나오고 있습니다.\n");
        }
        if(crt_ptr->hpcur < (crt_ptr->hpmax*9)/10 && (crt_ptr->hpcur > (crt_ptr->hpmax*8)/10) )
            print(fd, "%s는 가벼운 상처를 입었습니다.\n",
                  F_ISSET(crt_ptr, MMALES) ? "그":"그녀");
        if(crt_ptr->hpcur < (crt_ptr->hpmax*8)/10 && (crt_ptr->hpcur > (crt_ptr->hpmax*6)/10) )
            print(fd, "%s는 여러군데 상처를 입었습니다.\n",
                  F_ISSET(crt_ptr, MMALES) ? "그":"그녀");
        if(crt_ptr->hpcur < (crt_ptr->hpmax*6)/10 && (crt_ptr->hpcur > (crt_ptr->hpmax*4)/10) )
            print(fd, "%s는 많은 상처를 입었습니다.\n",
                  F_ISSET(crt_ptr, MMALES) ? "그":"그녀");
        if(crt_ptr->hpcur < (crt_ptr->hpmax*4)/10 && (crt_ptr->hpcur > (crt_ptr->hpmax*2)/10) )
            print(fd, "%s는 심각한 상처를 입었습니다.\n",
                  F_ISSET(crt_ptr, MMALES) ? "그":"그녀");
        if(crt_ptr->hpcur < (crt_ptr->hpmax*2)/10)
            print(fd, "%s는 죽기 직전입니다.\n",
                  F_ISSET(crt_ptr, MMALES) ? "그":"그녀");
        if(is_enm_crt(ply_ptr->name, crt_ptr))
            print(fd, "%s는 당신에게 매우 화가 난것 같습니다.\n",
                  F_ISSET(crt_ptr, MMALES) ? "그":"그녀");
        if(crt_ptr->first_enm) {
            if(!strcmp(crt_ptr->first_enm->enemy, ply_ptr->name))
                print(fd, "%s는 당신과 싸우고 있습니다.\n",
                      F_ISSET(crt_ptr, MMALES) ? "그":"그녀");
            else
                print(fd, "%s는 %S%j 싸우고 있습니다.\n",
                      F_ISSET(crt_ptr, MMALES) ? "그":"그녀",
                      crt_ptr->first_enm->enemy,"2");
        }
        consider(ply_ptr, crt_ptr);
        equip_list(fd, crt_ptr);
        return(0);
    }

    cmnd->str[1][0] = up(cmnd->str[1][0]);
    crt_ptr = find_crt(ply_ptr, rom_ptr->first_ply, cmnd->str[1],
               cmnd->val[1]);

    if(crt_ptr) {
        print(fd, "당신은 %M%j 봅니다.\n",crt_ptr,"3");
        broadcast_rom(fd,rom_ptr->rom_num,"\n%M%j %M%j 봅니다.",ply_ptr,"1",crt_ptr,"3");
        if(crt_ptr->type == PLAYER && F_ISSET(crt_ptr, PMARRI)) {
            print(fd, "%s는 %s님과 결혼한 기혼자입니다.\n", 
                    F_ISSET(crt_ptr, PMALES) ? "그":"그녀", &crt_ptr->key[2][1]);
        }            
		if(crt_ptr->type != PLAYER) {
        if(crt_ptr->description[0])
            print(fd, "%s\n", crt_ptr->description);
        else
            print(fd, "특별한 것은 보이지 않습니다.\n");
		}
		else {
			print(fd, "%s는 %s서 있습니다.\n", F_ISSET(crt_ptr, PMALES) ? "그":"그녀",
				crt_ptr->description);
		}
        if(F_ISSET(ply_ptr, PKNOWA) && crt_ptr->alignment>=-100 && crt_ptr->alignment<101) {
            print(fd, "%s에게서 ",
                F_ISSET(crt_ptr, MMALES) ? "그":"그녀");
            if(crt_ptr->alignment < -100)
                print(fd, "붉은 광채");
            else print(fd, "푸른 광채");
            print(fd, "가 뻗어 나오고 있습니다.\n");
        }
        if(crt_ptr->hpcur < (crt_ptr->hpmax*3)/10)
            print(fd, "%s는 가벼운 상처를 입었습니다.\n",
                  F_ISSET(crt_ptr, PMALES) ? "그":"그녀");
        equip_list(fd, crt_ptr);
        return(0);
    }

    else
        print(fd, "그런 건 보이지 않습니다.\n");

    return(0);

}

/**********************************************************************/
/*                              move                                  */
/**********************************************************************/

/* This function takes the player using the socket descriptor specified */
/* in the first parameter, and attempts to move him in one of the six   */
/* cardinal directions (n,s,e,w,u,d) if possible.                       */

int move(ply_ptr, cmnd)
creature        *ply_ptr;
cmd             *cmnd;
{
	FILE			*fp;
    room            *rom_ptr, *old_rom_ptr;
    creature        *crt_ptr;
    char            tempstr[10];
    char            *str;
	char			file[80], name[10][15];
	int				i;
    ctag            *cp, *temp;
    xtag            *xp;
    int             found=0, fd, old_rom_num, fall, dmg, n, chance;
    long            t;

    rom_ptr = ply_ptr->parent_rom;
    fd = ply_ptr->fd;
    str = cmnd->str[0];

    t=time(0);

/* 이동 딜레이 없앰.
     = 샤(0);
    if(ply_ptr->lasttime[LT_MOVED].ltime == t) {
        if(++ply_ptr->lasttime[LT_MOVED].misc > 2) {
            please_wait(fd, 1);
            return(0);
        }
    }
    else if(ply_ptr->lasttime[LT_ATTCK].ltime+2 > t) {
        please_wait(fd, 3-t+ply_ptr->lasttime[LT_ATTCK].ltime);
        return(0);
    }
    else if(ply_ptr->lasttime[LT_SPELL].ltime+2 > t) {
        please_wait(fd, 3-t+ply_ptr->lasttime[LT_SPELL].ltime);
        return(0);
    }
    else {
        ply_ptr->lasttime[LT_MOVED].ltime = t;
        ply_ptr->lasttime[LT_MOVED].misc = 1;
    }
*/

/*
    if(!strcmp(str,"ㄴㅅ") || !strcmp(str,"ㅅㄴ") || !strcmp(str,"24") || !strcmp(str,"42") || !strcmp(str,"서남"))
        strcpy(tempstr, "남서");
    else if(!strcmp(str,"ㅂㅅ") || !strcmp(str,"ㅅㅂ") || !strcmp(str,"84") || !strcmp(str,"48") || !strcmp(str,"서북"))
        strcpy(tempstr, "북서");
    else if(!strcmp(str,"ㄴㄷ") || !strcmp(str,"ㄷㄴ") || !strcmp(str,"26") || !strcmp(str,"62") || !strcmp(str,"동남"))
        strcpy(tempstr, "남동");
    else if(!strcmp(str,"ㅂㄷ") || !strcmp(str,"ㄷㅂ") || !strcmp(str,"86") || !strcmp(str,"68") || !strcmp(str,"동북"))
        strcpy(tempstr, "북동");
    else  */
         if(str[0]=='2' || !strcmp(str,"�ⅲ�") || !strcmp(str,"ㄴ")) strcpy(tempstr, "남");
    else if(str[0]=='4' || !strcmp(str,"�В�") || !strcmp(str,"ㅅ")) strcpy(tempstr, "서");
    else if(str[0]=='6' || !strcmp(str,"�┌�") || !strcmp(str,"ㄷ")) strcpy(tempstr, "동");
    else if(str[0]=='8' || !strcmp(str,"�ぃ�") || !strcmp(str,"ㅂ")) strcpy(tempstr, "북");
    else if(str[0]=='3' || !strcmp(str,"�ð�") || !strcmp(str,"ㅁ")) strcpy(tempstr, "밑");
    else if(str[0]=='9' || !strcmp(str,"��＂") || !strcmp(str,"ㅇ")) strcpy(tempstr, "위");
    else if(!strcmp(str,"나가")) strcpy(tempstr, "밖");
    else strcpy(tempstr, str);

    xp = rom_ptr->first_ext;
    while(xp) {
        if(!strcmp(xp->ext->name, tempstr) &&
            !F_ISSET(xp->ext,XNOSEE)){
            found = 1;
            break;
        }
        xp = xp->next_tag;
    }

    if(!found) {
        print(fd, "길이 막혀 있습니다.");
        return(0);
    }

    if(F_ISSET(ply_ptr, PSILNC)) {
    	print(fd, "당신은 움직일수 없습니다.");
    	return(0);
    }

	if(ply_is_attacking(ply_ptr, cmnd)) {
		print(fd, "싸우는 중에는 이동할 수 없습니다.");
		return(0);
	}

/*
    if(!F_ISSET(ply_ptr, PFLYSP) || !F_ISSET(ply_ptr, PLEVIT)) {
    		dmg = mrand(1,200);
    }
    else 	dmg = mrand(1,400);

    if(dmg==30 || dmg==93 || dmg==75 || dmg==100 || dmg==150 || 
       dmg==200 || dmg == 120 || dmg == 4 || dmg == 8 || dmg== 105 ||
       dmg == 127) {
        print(fd, "돌에 걸려 넘어졌습니다. 쿵!");
        return(0);
    }
*/

    if(F_ISSET(xp->ext, XLOCKD)) {
        print(fd, "문이 잠겨 있습니다.");
        return(0);
    }
    else if(F_ISSET(xp->ext, XCLOSD)) {
        print(fd, "문이 닫혀 있습니다.");
        return(0);
    }

    if(F_ISSET(xp->ext, XFLYSP) && !F_ISSET(ply_ptr, PFLYSP)) {
        print(fd, "그 쪽으로는 날아서 가야 될것 같군요.");
        return(0);
    }

    t = Time%24L;
    if(F_ISSET(xp->ext, XNGHTO) && (t>6 && t < 20)) {
        print(fd, "그 출구는 밤에만 열려 있습니다.");
        return(0);
    }

    if(F_ISSET(xp->ext, XDAYON) && (t<6 || t > 20)){
        print(fd, "그 출구는 밤에는 닫혀 있습니다.");
        return(0);
    }

    if(F_ISSET(xp->ext,XPGUAR)){
    cp = rom_ptr->first_mon;
    while(cp) {
        if(F_ISSET(cp->crt, MPGUAR)) {
        if(!F_ISSET(ply_ptr, PINVIS) && ply_ptr->class < SUB_DM) {
            print(fd, "%M%j 당신의 길을 막습니다.", cp->crt,"1");
            return(0);
        }
        if(F_ISSET(cp->crt, MDINVI) && ply_ptr->class < SUB_DM) {
            print(fd, "%M%j 당신의 길을 막습니다.", cp->crt,"1");
            return(0);
        }
        }
        cp = cp->next_tag;
    }
    }

    if(F_ISSET(xp->ext, XFEMAL) && F_ISSET(ply_ptr, PMALES)){
        print(fd, "여성만 들어갈수 있습니다. 여탕인가~~");
        return(0);
    }

    if(F_ISSET(xp->ext, XMALES) && !F_ISSET(ply_ptr, PMALES)){
        print(fd, "남성만 들어갈수 있습니다.");
        return(0);
    }

    if(F_ISSET(xp->ext, XNAKED) && weight_ply(ply_ptr)) {
        print(fd, "뭘 가지고는 들어갈수 없습니다.");
        return(0);
    }

    if((F_ISSET(xp->ext, XCLIMB) || F_ISSET(xp->ext, XREPEL)) &&
       !F_ISSET(ply_ptr, PLEVIT)) {
        fall = (F_ISSET(xp->ext, XDCLIM) ? 50:0) + 50 -
               fall_ply(ply_ptr);

        if(mrand(1,100) < fall) {
            dmg = mrand(5,15+fall/10);
            if(ply_ptr->hpcur <= dmg){
                print(fd, "당신은 죽음이 다가오는것같은 느낌이 듭니다.");
                ply_ptr->hpcur=0;
                broadcast_rom(fd, ply_ptr->rom_num, "\n%M%j 구덩이에 떨어져서 죽었습니다.", ply_ptr,"1");
                die(ply_ptr, ply_ptr);
                return(0);
            }
            ply_ptr->hpcur -= dmg;
            print(fd, "당신은 구덩이에 떨어져서 %d 만큼의 상처를 입었습니다",
                  dmg);
            broadcast_rom(fd, ply_ptr->rom_num, "\n%M%j 구덩이에 떨어졌습니다.",
                      ply_ptr,"1");
            if(F_ISSET(xp->ext, XCLIMB))
                return(0);
        }
    }

    if((ply_ptr->class == ASSASSIN || ply_ptr->class == THIEF || ply_ptr->class >
 INVINCIBLE) && (F_ISSET(ply_ptr, PHIDDN))) {

        chance = MIN(85, 5 + 6*((ply_ptr->level+3)/4) +
                3*bonus[ply_ptr->dexterity]);
        if(F_ISSET(ply_ptr, PBLIND))
                chance = MIN(20, chance);

        if(mrand(1,100) > chance)
        {
            print(fd,"당신은 은신술을 사용하는데 실패하였습니다.\n");
            F_CLR(ply_ptr, PHIDDN);

            cp = rom_ptr->first_mon;
            while(cp) {
                if(F_ISSET(cp->crt, MBLOCK) &&
                is_enm_crt(ply_ptr->name, cp->crt) &&
                F_ISSET(ply_ptr, PINVIS) && ply_ptr->class < SUB_DM) {
                    print(fd, "%M가 당신의 길을 가로막습니다.\n", cp->crt);
                    return(0);
                    }
                cp = cp->next_tag;
            }
         }
     }
     else F_CLR(ply_ptr, PHIDDN);


    if(!F_ISSET(rom_ptr, RPTRAK))
        strcpy(rom_ptr->track, tempstr);

    old_rom_num = rom_ptr->rom_num;
    old_rom_ptr = rom_ptr;

    load_rom(xp->ext->room, &rom_ptr);
    if(rom_ptr == old_rom_ptr) {
        print(fd, "그쪽으로 지도가 없습니다. 신에게 연락해 주세요.");
        return(0);
    }

    n = count_vis_ply(rom_ptr);

    if(rom_ptr->lolevel > ply_ptr->level && ply_ptr->class <INVINCIBLE/*CARETAKER*/) {
        print(fd, "레벨 %d 이상이어야 그 곳으로 갈 수 있습니다.",
              rom_ptr->lolevel);
        return(0);
    }
    else if(ply_ptr->level > rom_ptr->hilevel && rom_ptr->hilevel &&
        ply_ptr->class < CARETAKER) {
        print(fd, "그곳으로 갈려면 레벨 %d이하여야만 합니다.",
              rom_ptr->hilevel);
        return(0);
    }
    else if((F_ISSET(rom_ptr, RONEPL) && n > 0) ||
        (F_ISSET(rom_ptr, RTWOPL) && n > 1) ||
        (F_ISSET(rom_ptr, RTHREE) && n > 2)) {
        print(fd, "그 방에 있는 사용자가 너무 많습니다.");
        return(0);
    }
    else if((F_ISSET(rom_ptr, RFAMIL)) && !F_ISSET(ply_ptr, PFAMIL)) {
    	print(fd, "그곳에는 패거리 가입자만 갈 수 있습니다. ");
    	return(0);
    }
    else if(ply_ptr->class < DM && (F_ISSET(rom_ptr, RONFML)) && (ply_ptr->daily[DL_EXPND].max != rom_ptr->special)) {
    	print(fd, "그곳은 당신이 갈수 없는 곳입니다.");
    	return(0);
    }
    else if(ply_ptr->class < DM && (F_ISSET(rom_ptr, RONMAR)) &&
    	(ply_ptr->daily[DL_MARRI].max != rom_ptr->special)) {
		sprintf(file, "%s/invite/invite_%d", PLAYERPATH, rom_ptr->special);
	if(file_exists(file)) {
	fp = fopen(file, "r");
	for(i=0; i<10; i++) {
		fscanf(fp, "%s", name[i]);
		if(!strcmp(ply_ptr->name, name[i])) {
			fclose(fp);
			goto mmm;
		}
	}
	fclose(fp);
	}
    	print(fd, "그곳은 사유지입니다.");
    	return(0);
    }

mmm:
    if(t-login_time[fd]<120) login_time[fd]-=120;

    if(ply_ptr->class == DM && !F_ISSET(ply_ptr, PDMINV)){
        broadcast_rom(fd, old_rom_ptr->rom_num, "\n%M이 %s쪽으로 갔습니다.",
                  ply_ptr, tempstr);
    }
    if(!F_ISSET(ply_ptr, PDMINV) && ply_ptr->class < DM) {
        broadcast_rom(fd, old_rom_ptr->rom_num, "\n%M이 %s쪽으로 갔습니다.",
                  ply_ptr, tempstr);
    }

    del_ply_rom(ply_ptr, ply_ptr->parent_rom);
    add_ply_rom(ply_ptr, rom_ptr);

/*
    if(ply_ptr->class == RANGER){
             auto_search(ply_ptr);
    }
*/

    cp = ply_ptr->first_fol;
    while(cp) {
        if(cp->crt->rom_num == old_rom_num && cp->crt->type != MONSTER)
            move(cp->crt, cmnd);
        if(F_ISSET(cp->crt, MDMFOL) && cp->crt->rom_num == old_rom_num && cp->crt->type == MONSTER) {
        del_crt_rom(cp->crt, old_rom_ptr);
        broadcast_rom(fd, old_rom_ptr->rom_num, "\n%M이 %s쪽으로 갔습니다.", cp->crt, tempstr);
        add_crt_rom(cp->crt, rom_ptr, 1);
        add_active(cp->crt);
        }
    cp = cp->next_tag;
    }

    if(is_rom_loaded(old_rom_num)) {
        cp = old_rom_ptr->first_mon;
        while(cp) {
            if(!F_ISSET(cp->crt, MFOLLO) || F_ISSET(cp->crt, MDMFOL)) {
                /* 투명일때는 안따라가게..*/
                if((!F_ISSET(cp->crt,MDINVI) &&
                    F_ISSET(ply_ptr,PINVIS)) || F_ISSET(ply_ptr,PDMINV)) {
                    cp = cp->next_tag;
                    continue;
                }
            }
            if(!cp->crt->first_enm) {
                cp = cp->next_tag;
                continue;
            }
            if(strcmp(cp->crt->first_enm->enemy, ply_ptr->name)) {
                cp = cp->next_tag;
                continue;
            }
            if(mrand(1,50) > 15 - ply_ptr->dexterity +
               cp->crt->dexterity) {
                cp = cp->next_tag;
                continue;
            }
            print(fd, "\n%M%j 당신을 따라옵니다.", cp->crt,"1");
            broadcast_rom(fd, old_rom_num, "\n%M%j %M%j 따라갑니다.",
                      cp->crt, "1", ply_ptr,"3");
            temp = cp->next_tag;
            crt_ptr = cp->crt;
            if(F_ISSET(crt_ptr, MPERMT))
                die_perm_crt(ply_ptr, crt_ptr);
            del_crt_rom(crt_ptr, old_rom_ptr);
            add_crt_rom(crt_ptr, rom_ptr, 1);
            add_active(crt_ptr);
            F_CLR(crt_ptr, MPERMT);
            cp = temp;

        }
    }

    check_traps(ply_ptr, rom_ptr);
    return(0);
}

/**********************************************************************/
/*                              say                                   */
/**********************************************************************/

/* This function allows the player specified by the socket descriptor */
/* in the first parameter to say something to all the other people in */
/* the room.                                                          */

/* say = 말 */
int say(ply_ptr, cmnd)
creature        *ply_ptr;
cmd             *cmnd;
{
    room            *rom_ptr;
    int             index = -1, i, fd;
    int             len;

    fd = ply_ptr->fd;
    rom_ptr = ply_ptr->parent_rom;

    len = strlen(cmnd->fullstr);
    if(cmnd->fullstr[len-1]==' ' || cmnd->fullstr[len-1]=='.' || \
       cmnd->fullstr[len-1]=='!' || cmnd->fullstr[len-1]=='?') index=len;
    else    for(i=len-1; i>=0; i--) {
            if(cmnd->fullstr[i] == ' ') {
                cmnd->fullstr[i]=0;
                index = i;
                break;
            }
        }

    cmnd->fullstr[255] = 0;

    if(index == -1 || strlen(&cmnd->fullstr[0]) ==0) {
        print(fd, "뭘 말하고 싶으세요?");
        return(0);
    }
    if(F_ISSET(ply_ptr, PSILNC)) {
        print(fd, "말을 해 보았지만 이 방 밖의 사람들은 들리지 않는듯 하군요.");
        return(0);
    }

    F_CLR(ply_ptr, PHIDDN);
    if(F_ISSET(ply_ptr, PLECHO)){
        print(fd, "당신은 \"%s\"라고 말합니다.", &cmnd->fullstr[0]);
    }
    else
        print(fd, "예. 좋습니다.");

    broadcast_rom(fd, rom_ptr->rom_num, "\n%C%M%j \"%s\"라고 말합니다.%D",
              "36",ply_ptr,"1", &cmnd->fullstr[0],"37");

    cmnd->fullstr[index]=' ';
    return(0);

}

/**********************************************************************/
/*                              get                                   */
/**********************************************************************/

/* This function allows players to get things lying on the floor or   */
/* inside another object.                                             */

int get(ply_ptr, cmnd)
creature        *ply_ptr;
cmd             *cmnd;
{
    room            *rom_ptr;
    object          *obj_ptr, *cnt_ptr, *obj_temp;
    ctag            *cp;
    int             fd, n, match=0;
    int cnt=0,i;

    fd = ply_ptr->fd;

    if(cmnd->num < 2) {
        print(fd, "뭘 주우시게요?");
        return(0);
    }

    rom_ptr = ply_ptr->parent_rom;
    F_CLR(ply_ptr, PHIDDN);

    for(i=0,cnt=0; i<MAXWEAR; i++)
        if(ply_ptr->ready[i]) cnt++;
    cnt += count_inv(ply_ptr, -1);

    if(cmnd->num == 2) {

        cp = rom_ptr->first_mon;
        while(cp) {
            if(F_ISSET(cp->crt, MGUARD))
                break;
            cp = cp->next_tag;
        }

        obj_temp=NULL;
        obj_temp = find_obj(ply_ptr, rom_ptr->first_obj,
                   "초인의 돌", cmnd->val[1]);
        if(obj_temp && ply_ptr->class < CARETAKER) {
            if(obj_temp->value == 1001) {
                print(fd, "초인의 돌의 힘이 당신이 무언가를 줍는것을 방해합니다.");
                return(0);
            }   
        }    

        if(cp && ply_ptr->class < CARETAKER) {
            print(fd, "%M%j 당신이 어떤것을 줍는 것을 방해합니다.", cp->crt,"1");
            return(0);
        }
        if(F_ISSET(ply_ptr, PBLIND)) {
            print(fd, "그런 건 보이지 않습니다.");
            return(0);
        }
        if(!strcmp(cmnd->str[1], "모두")) {
            get_all_rom(ply_ptr,cmnd->str[1]);
            return(0);
        }
        if(!strncmp(cmnd->str[1], "모든",4)) {
            get_all_rom(ply_ptr,cmnd->str[1]+4);
            return(0);
        }

        obj_ptr = find_obj(ply_ptr, rom_ptr->first_obj,
                   cmnd->str[1], cmnd->val[1]);

        if(!obj_ptr) {
            print(fd, "그런건 여기 없어요.");
            return(0);
        }

        if(F_ISSET(obj_ptr, OINVIS)) {
            print(fd, "그런건 여기 없어요.");
            return(0);
        }

        if(F_ISSET(obj_ptr, ONOTAK) || F_ISSET(obj_ptr, OSCENE)) {
            print(fd, "주을 수 있는 물건이 아닙니다.");
            return(0);
        }

        if(weight_ply(ply_ptr) + weight_obj(obj_ptr) >
           max_weight(ply_ptr) || cnt>150) {
            print(fd, "당신은 더이상 가질 수 없습니다.\n");
            return(0);
        }

        if(obj_ptr->questnum && Q_ISSET(ply_ptr, obj_ptr->questnum-1)) {
            print(fd, "당신은 그것을 주울수 없습니다. %s.",
                  "이미 당신은 임무를 완수하였습니다.");
            return(0);
        }

        if(F_ISSET(obj_ptr, OTEMPP)) {
            F_CLR(obj_ptr, OPERM2);
            F_CLR(obj_ptr, OTEMPP);
        }

        if(F_ISSET(obj_ptr, OPERMT))
            get_perm_obj(obj_ptr);

        F_CLR(obj_ptr, OHIDDN);
        del_obj_rom(obj_ptr, rom_ptr);
        print(fd, "당신은 %1i%j 줍습니다.", obj_ptr,"3");
        if(obj_ptr->questnum) {
            print(fd, "임무를 완수하였습니다. 버리지 마십시요!.");
            print(fd, "버리면 다시는 주울 수가 없습니다.");
            Q_SET(ply_ptr, obj_ptr->questnum-1);
            ply_ptr->experience += quest_exp[obj_ptr->questnum-1];
            print(fd, "당신은 경험치 %ld를 받았습니다.",
                quest_exp[obj_ptr->questnum-1]);
            add_prof(ply_ptr,quest_exp[obj_ptr->questnum-1]);
        }
        broadcast_rom(fd, rom_ptr->rom_num, "\n%M%j %1i%j 줍습니다.",
                  ply_ptr, "1", obj_ptr,"3");

        if(obj_ptr->type == MONEY) {
            ply_ptr->gold += obj_ptr->value;
            free_obj(obj_ptr);
            print(fd, "\n당신은 이제 %ld냥을 갖고 있습니다.",
                ply_ptr->gold);
        }
        else
            add_obj_crt(obj_ptr, ply_ptr);
        return(0);
    }

    else {

        cnt_ptr = find_obj(ply_ptr, ply_ptr->first_obj,
                   cmnd->str[1], cmnd->val[1]);
/* by. latok */
        if(!cnt_ptr)
            cnt_ptr = find_obj(ply_ptr, rom_ptr->first_obj,
                       cmnd->str[1], cmnd->val[1]);

        if(!cnt_ptr || !cmnd->val[1]) {
            for(n=0; n<MAXWEAR; n++) {
                if(!ply_ptr->ready[n]) continue;
                if(EQUAL(ply_ptr->ready[n], cmnd->str[1]))
                    match++;
                else continue;
                if(match == cmnd->val[1] || !cmnd->val[1]) {
                    cnt_ptr = ply_ptr->ready[n];
                    break;
                }
            }
        }

        if(!cnt_ptr) {
            print(fd, "그런것은 보이지 않습니다.");
            return(0);
        }

        if(!F_ISSET(cnt_ptr, OCONTN)) {
            print(fd, "그것은 담는 종류가 아닙니다.");
            return(0);
        }

        if(!strcmp(cmnd->str[2], "모두")) {
            get_all_obj(ply_ptr, cnt_ptr,cmnd->str[2]);
            return(0);
        }
        if(!strncmp(cmnd->str[2], "모든",4)) {
            get_all_obj(ply_ptr, cnt_ptr,cmnd->str[2]+4);
            return(0);
        }
        obj_ptr = find_obj(ply_ptr, cnt_ptr->first_obj,
                   cmnd->str[2], cmnd->val[2]);

        if(!obj_ptr) {
            print(fd, "그 안에 그런것은 없어요.");
            return(0);
        }

        if((weight_ply(ply_ptr) + weight_obj(obj_ptr) >
           max_weight(ply_ptr) && cnt_ptr->parent_rom) || cnt>150) {
            print(fd, "당신은 더이상 가질 수 없습니다.");
            return(0);
        }

        if(F_ISSET(obj_ptr, OPERMT))
            get_perm_obj(obj_ptr);

        cnt_ptr->shotscur--;
        del_obj_obj(obj_ptr, cnt_ptr);
        print(fd, "당신은 %1i에서 %1i%j 꺼냅니다.", cnt_ptr, obj_ptr,"3");
        broadcast_rom(fd, rom_ptr->rom_num, "\n%M이 %1i에서 %1i%j 꺼냅니다.",
                  ply_ptr, cnt_ptr, obj_ptr,"3");

	if(F_ISSET(cnt_ptr, OPERMT)) {
		resave_rom(rom_ptr->rom_num);
#ifdef NODUPLE
		savegame_nomsg(ply_ptr);
#endif
	}

        if(obj_ptr->type == MONEY) {
            ply_ptr->gold += obj_ptr->value;
            free_obj(obj_ptr);
            print(fd, "\n당신은 이제 %ld냥을 가지고 있습니다.",
                ply_ptr->gold);
        }
        else
            add_obj_crt(obj_ptr, ply_ptr);

        return(0);
    }

}

/**********************************************************************/
/*                              get_all_rom                           */
/**********************************************************************/

/* This function will cause the player pointed to by the first parameter */
/* to get everything he is able to see in the room.                      */

void get_all_rom(ply_ptr,part_obj)
creature        *ply_ptr;
char *part_obj;
{
    room    *rom_ptr;
    object  *obj_ptr, *last_obj;
    otag    *op;
    char    str[2048];
    int     fd, n = 1, found = 0, heavy = 0, dogoldmsg = 0;
    int index=1;
    int i,cnt;

        for(i=0,cnt=0; i<MAXWEAR; i++)
                if(ply_ptr->ready[i]) cnt++;
        cnt += count_inv(ply_ptr, -1);


    if(!strcmp(part_obj,"모두")) {
        index=0;
    }
    last_obj = 0; str[0] = 0;

    fd = ply_ptr->fd;
    rom_ptr = ply_ptr->parent_rom;

    op = rom_ptr->first_obj;
    while(op) {
        if(!F_ISSET(op->obj, OSCENE) &&
           !F_ISSET(op->obj, ONOTAK) && !F_ISSET(op->obj, OHIDDN) &&
           (F_ISSET(ply_ptr, PDINVI) ? 1:!F_ISSET(op->obj, OINVIS))) {

            if(index) {
                obj_ptr = find_obj(ply_ptr, op, part_obj, 1);
                if(!obj_ptr) break;

                while(op->obj!=obj_ptr) op=op->next_tag;
                op=op->next_tag;
                found++;
            }
            else {
                found++;
                obj_ptr = op->obj;
                op = op->next_tag;
            }
            if(weight_ply(ply_ptr) + weight_obj(obj_ptr) >
              max_weight(ply_ptr) || cnt>150) {
                heavy++;
                continue;
            }
            if(obj_ptr->questnum &&
               Q_ISSET(ply_ptr, obj_ptr->questnum-1)) {
                heavy++;
                continue;
            }
            if(F_ISSET(obj_ptr, OTEMPP)) {
                F_CLR(obj_ptr, OPERM2);
                F_CLR(obj_ptr, OTEMPP);
            }
            if(F_ISSET(obj_ptr, OPERMT))
                get_perm_obj(obj_ptr);
            F_CLR(obj_ptr, OHIDDN);
            cnt++;
            if(obj_ptr->questnum) {
                print(fd,"임무를 완수하였습니다! 버리지 마십시요!.\n");
                print(fd,"버리면 다시 주울 수 없습니다.");
                Q_SET(ply_ptr, obj_ptr->questnum-1);
                ply_ptr->experience +=
                    quest_exp[obj_ptr->questnum-1];
                print(fd, "\n당신은 경험치 %ld 을 받았습니다.",
                    quest_exp[obj_ptr->questnum-1]);
            add_prof(ply_ptr,quest_exp[obj_ptr->questnum-1]);
            }
            del_obj_rom(obj_ptr, rom_ptr);
            if(last_obj && !strcmp(last_obj->name, obj_ptr->name) &&
               last_obj->adjustment == obj_ptr->adjustment)
                n++;
            else if(last_obj) {
                strcat(str, obj_str(last_obj, n, 0));
                strcat(str, ", ");
                n=1;
            }
            if(obj_ptr->type == MONEY) {
                strcat(str, obj_str(obj_ptr, 1, 0));
                strcat(str, ", ");
                ply_ptr->gold += obj_ptr->value;
                free_obj(obj_ptr);
                last_obj = 0;
                dogoldmsg = 1;
            }
            else {
                add_obj_crt(obj_ptr, ply_ptr);
                last_obj = obj_ptr;
            }
        }
        else
            op = op->next_tag;
    }

    if(found && last_obj)
        strcat(str, obj_str(last_obj, n, 0));
    else if(!found) {
        print(fd, "여기에는 아무것도 없습니다.");
        return;
    }

    if(dogoldmsg && !last_obj)
        str[strlen(str)-2] = 0;

    if(heavy) {
        print(fd, "가지고 있는것이 너무 무거워 더이상 가질 수가 없습니다.\n");
        if(heavy == found) return;
    }

    if(!strlen(str)) return;

    broadcast_rom(fd, rom_ptr->rom_num, "\n%M%j %S%j 줍습니다.", ply_ptr, "1",str,"3");
    print(fd, "당신은 %S%j 줍습니다.", str,"3");
    if(dogoldmsg)
        print(fd, "\n당신은 이제 %ld냥을 가지고 있습니다.",
            ply_ptr->gold);

}

/**********************************************************************/
/*                              get_all_obj                           */
/**********************************************************************/

/* This function allows a player to get the entire contents from a        */
/* container object.  The player is pointed to by the first parameter and */
/* the container by the second.                                           */

void get_all_obj(ply_ptr, cnt_ptr,part_obj)
creature        *ply_ptr;
object          *cnt_ptr;
char *part_obj;
{
    room    *rom_ptr;
    object  *obj_ptr, *last_obj;
    otag    *op;
    char    str[2048];
    int     fd, n = 1, found = 0, heavy = 0;
    int index=1;
    int i,cnt;

        for(i=0,cnt=0; i<MAXWEAR; i++)
                if(ply_ptr->ready[i]) cnt++;
        cnt += count_inv(ply_ptr, -1);

    if(!strcmp(part_obj,"모두")) {
        index=0;
    }
    last_obj = 0; str[0] = 0;

    fd = ply_ptr->fd;
    rom_ptr = ply_ptr->parent_rom;

    op = cnt_ptr->first_obj;
    while(op) {
        if(!F_ISSET(op->obj, OSCENE) &&
           !F_ISSET(op->obj, ONOTAK) && !F_ISSET(op->obj, OHIDDN) &&
           (F_ISSET(ply_ptr, PDINVI) ? 1:!F_ISSET(op->obj, OINVIS))) {
            if(index) {
                obj_ptr = find_obj(ply_ptr, op, part_obj, 1);
                if(!obj_ptr) break;
                while(op->obj!=obj_ptr) op=op->next_tag;
                op=op->next_tag;
                found++;
            }
            else {
                found++;
                obj_ptr = op->obj;
                op = op->next_tag;
            }

            if(weight_ply(ply_ptr) + weight_obj(obj_ptr) >
               max_weight(ply_ptr) || cnt>150) {
                heavy++;
                continue;
            }
            if(F_ISSET(obj_ptr, OTEMPP)) {
                F_CLR(obj_ptr, OPERM2);
                F_CLR(obj_ptr, OTEMPP);
            }
            if(F_ISSET(obj_ptr, OPERMT))
                get_perm_obj(obj_ptr);
            cnt_ptr->shotscur--;
            cnt++;
            del_obj_obj(obj_ptr, cnt_ptr);
            if(last_obj && !strcmp(last_obj->name, obj_ptr->name) &&
                last_obj->adjustment == obj_ptr->adjustment)
                n++;
            else if(last_obj) {
                strcat(str, obj_str(last_obj, n, 0));
                strcat(str, ", ");
                n = 1;
            }
            if(obj_ptr->type == MONEY) {
                ply_ptr->gold += obj_ptr->value;
                free_obj(obj_ptr);
                last_obj = 0;
                print(fd, "\n당신은 이제 %ld냥을 가지고 있습니다.",
                    ply_ptr->gold);
            }
            else {
                add_obj_crt(obj_ptr, ply_ptr);
                last_obj = obj_ptr;
            }
        }
        else
            op = op->next_tag;
    }

    if(found && last_obj)
        strcat(str, obj_str(last_obj, n, 0));
    else if(!found) {
        print(fd, "그 안에는 아무것도 없습니다.");
        return;
    }

    if(heavy) {
        print(fd, "가지고 있는 물건이 너무 무거워 들 수가 없습니다.\n");
        if(heavy == found) return;
    }

    if(!strlen(str)) return;

    broadcast_rom(fd, rom_ptr->rom_num, "\n%M%j %1i에서 %S%j 꺼냅니다.",
              ply_ptr, "1",cnt_ptr, str,"3");
    print(fd, "당신은 %1i에서 %S%j 꺼냅니다.", cnt_ptr, str,"3");
    if(F_ISSET(cnt_ptr, OPERMT)) {
    	resave_rom(rom_ptr->rom_num);
#ifdef NODUPLE
    	savegame_nomsg(ply_ptr);
#endif
    }

}

/**********************************************************************/
/*                              get_perm_obj                          */
/**********************************************************************/

/* This function is called whenever someone picks up a permanent item */
/* from a room.  The item's room-permanent flag is cleared, and the   */
/* inventory-permanent flag is set.  Also, the room's permanent       */
/* time for that item is updated.                                     */

void get_perm_obj(obj_ptr)
object  *obj_ptr;
{
    object  *temp_obj;
    room    *rom_ptr;
    long    t;
    int     i;

    t = time(0);

    F_SET(obj_ptr, OPERM2);
    F_CLR(obj_ptr, OPERMT);

    rom_ptr = obj_ptr->parent_rom;
    if(!rom_ptr) return;

    for(i=0; i<10; i++) {
        if(!rom_ptr->perm_obj[i].misc) continue;
        if(rom_ptr->perm_obj[i].ltime + rom_ptr->perm_obj[i].interval >
           t) continue;
        if(load_obj(rom_ptr->perm_obj[i].misc, &temp_obj) < 0)
            continue;
        if(!strcmp(temp_obj->name, obj_ptr->name)) {
            rom_ptr->perm_obj[i].ltime = t;
            free_obj(temp_obj);
            break;
        }
        free_obj(temp_obj);
    }
}

/**********************************************************************/
/*                              inventory                             */
/**********************************************************************/

/* This function outputs the contents of a player's inventory.        */

int inventory(ply_ptr, cmnd)
creature        *ply_ptr;
cmd             *cmnd;
{
    otag            *op;
    char            str[2048];
    int             m, n, fd, flags = 0;

    fd = ply_ptr->fd;

    if(F_ISSET(ply_ptr, PBLIND)) {
        ANSI(fd, BLUE);
        print(fd, "당신은 눈이 멀어서 아무것도 볼 수가 없습니다!");
        ANSI(fd, WHITE);
        return(0);
    }
    if(F_ISSET(ply_ptr, PDINVI))
        flags |= INV;
    if(F_ISSET(ply_ptr, PDMAGI))
        flags |= MAG;

    op = ply_ptr->first_obj; n=0; str[0]=0;
    strcat(str, "소지품:\n  ");
    if(!op) {
        strcat(str, "없음.");
        print(fd, "%s", str);
        return(0);
    }
    while(op) {
        if(F_ISSET(ply_ptr, PDINVI) ? 1:!F_ISSET(op->obj, OINVIS)) {
            m=1;
            while(op->next_tag) {
                if(!strcmp(op->next_tag->obj->name,
                       op->obj->name) &&
                   op->next_tag->obj->adjustment ==
                   op->obj->adjustment &&
                   (F_ISSET(ply_ptr, PDINVI) ?
                   1:!F_ISSET(op->next_tag->obj, OINVIS))) {
                    m++;
                    op = op->next_tag;
                }
                else
                    break;
            }
            strcat(str, obj_str(op->obj, m, flags));
            strcat(str, ", ");
            n++;
        }
        op = op->next_tag;
    }
    if(n) {
        str[strlen(str)-2] = 0;
        print(fd, "%s.", str);
    }

    return(0);

}

/**********************************************************************/
/*                              drop                                  */
/**********************************************************************/

/* This function allows the player pointed to by the first parameter */
/* to drop an object in the room at which he is located.             */
void drop_money();

int drop(ply_ptr, cmnd)
creature        *ply_ptr;
cmd             *cmnd;
{
    room            *rom_ptr;
    otag            *otag_ptr;
    object          *obj_ptr, *cnt_ptr;
    int             fd, n, match=0;

    fd = ply_ptr->fd;

    if(cmnd->num < 2) {
        print(fd, "무엇을 버리실려구요?");
        return(0);
    }

    rom_ptr = ply_ptr->parent_rom;
    F_CLR(ply_ptr, PHIDDN);

    if(cmnd->num == 2) {

        if(utf8_ends_with((unsigned char *)cmnd->str[1], (unsigned char *)"냥")) {
            drop_money(ply_ptr, cmnd);
            return(0);
        }
        if(!strcmp(cmnd->str[1], "모두")) {
            drop_all_rom(ply_ptr,cmnd->str[1]);
            return(0);
        }
        if(!strncmp(cmnd->str[1], "모든",4)) {
            drop_all_rom(ply_ptr,cmnd->str[1]+4);
            return(0);
        }

        obj_ptr = find_obj(ply_ptr, ply_ptr->first_obj,
                   cmnd->str[1], cmnd->val[1]);

        if(!obj_ptr) {
            print(fd, "당신은 그런것을 갖고 있지 않습니다.");
            return(0);
        }
/*               임무아이템 버리지 못하게 함 */

        if(obj_ptr->questnum && ply_ptr->class<DM) {
            print(fd, "임무 아이템은 버리지 못합니다.");
            return(0);
        }
       if(F_ISSET(obj_ptr, ONEWEV) && ply_ptr->class < DM) {
       		print(fd, "이벤트 아이템은 버리지 못합니다.");
       		return(0);
       	} 

       if(obj_ptr->first_obj) {
           for(otag_ptr=obj_ptr->first_obj; otag_ptr; otag_ptr=otag_ptr->next_tag) {
                if(otag_ptr->obj->questnum && ply_ptr->class<DM) {
                    print(fd, "임무 아이템이 들어있으면 버리지 못합니다.");
                    return(0);
                }
               if(F_ISSET(otag_ptr->obj, ONEWEV) && ply_ptr->class < DM) {
       	        	print(fd, "이벤트 아이템이 들어있으면 버리지 못합니다.");
       		        return(0);
               } 
           }
       }

        del_obj_crt(obj_ptr, ply_ptr);
#ifdef NODUPLE
        savegame_nomsg(ply_ptr);
#endif
        print(fd, "당신은 %1i%j 버렸습니다.", obj_ptr,"3");
        broadcast_rom(fd, rom_ptr->rom_num, "\n%M%j %1i%j 버렸습니다.",
                  ply_ptr,"1", obj_ptr,"3");
        if(!F_ISSET(rom_ptr, RDUMPR))
            add_obj_rom(obj_ptr, rom_ptr);
        else {
            free_obj(obj_ptr);
            ply_ptr->gold += 10;
            ply_ptr->experience += 2;
            print(fd, "\n당신의 물건을 제물로 바쳤습니다.\n당신은 약간의 상금과 경험을 받았습니다.");
        }
        return(0);
    }

    else {

        cnt_ptr = find_obj(ply_ptr, ply_ptr->first_obj,
                   cmnd->str[2], cmnd->val[2]);
/**** 수정요구함 ****/
        if(!cnt_ptr)
            cnt_ptr = find_obj(ply_ptr, rom_ptr->first_obj,
                        cmnd->str[2], cmnd->val[2]);

        if(!cnt_ptr || !cmnd->val[2]) {
            for(n=0; n<MAXWEAR; n++) {
                if(!ply_ptr->ready[n]) continue;
                if(EQUAL(ply_ptr->ready[n], cmnd->str[2]))
                    match++;
                else continue;
                if(match == cmnd->val[2] || !cmnd->val[2]) {
                    cnt_ptr = ply_ptr->ready[n];
                    break;
                }
            }
        }

        if(!cnt_ptr) {
            print(fd, "그런 물건은 없습니다.");
            return(0);
        }

        if(!F_ISSET(cnt_ptr, OCONTN)) {
            print(fd, "그것은 담을수 있는것이 아닙니다.");
            return(0);
        }

        if(!strcmp(cmnd->str[1], "모두")) {
            drop_all_obj(ply_ptr, cnt_ptr,cmnd->str[1]);
            return(0);
        }
        if(!strncmp(cmnd->str[1], "모든",4)) {
            drop_all_obj(ply_ptr, cnt_ptr,cmnd->str[1]+4);
            return(0);
        }

        obj_ptr = find_obj(ply_ptr, ply_ptr->first_obj,
                   cmnd->str[1], cmnd->val[1]);

        if(!obj_ptr) {
            print(fd, "당신은 그런것을 갖고 있지 않습니다.");
            return(0);
        }

        if(obj_ptr == cnt_ptr) {
            print(fd, "그것을 그것 자신한테는 넣을수 없습니다.");
            return(0);
        }

        if(obj_ptr->questnum && ply_ptr->class<DM) {
            print(fd, "임무 아이템은 버리지 못합니다.");
            return(0);
        }
       if(F_ISSET(obj_ptr, ONEWEV) && ply_ptr->class < DM) {
            print(fd, "이벤트 아이템은 버리지 못합니다.");
            return(0);
        } 

        if(cnt_ptr->shotscur >= cnt_ptr->shotsmax) {
            print(fd, "%I안에 더이상 넣을 수 없습니다.\n", cnt_ptr);
            return(0);
        }

        if(F_ISSET(obj_ptr, OCONTN)) {
            print(fd, "담을수 있는 물건 안에 담을 수 있는 물건은 넣을 수 없습니다.\n");
        }

        if(F_ISSET(cnt_ptr, OCNDES)) {
        print(fd, "%1i%j %1i%j 삼켜 버려 흔적도 없이 사라집니다!\n", obj_ptr, "3",cnt_ptr,"1");
        broadcast_rom(fd, rom_ptr->rom_num, "\n%M이 %1i%j %1i에 넣습니다.\n",
                  ply_ptr, obj_ptr,"3", cnt_ptr);
        del_obj_crt(obj_ptr, ply_ptr);
        free(obj_ptr);
            return(0);
        }

        del_obj_crt(obj_ptr, ply_ptr);
        add_obj_obj(obj_ptr, cnt_ptr);
        cnt_ptr->shotscur++;
        print(fd, "당신은 %1i%j %1i 안에 넣습니다.\n", obj_ptr,"3", cnt_ptr);
        broadcast_rom(fd, rom_ptr->rom_num, "\n%M%j %1i%j %1i안에 넣었습니다.",
                  ply_ptr, "1", obj_ptr,"3", cnt_ptr);
        if(F_ISSET(cnt_ptr, OPERMT) && F_ISSET(ply_ptr->parent_rom, RBANK)) {
        	resave_rom(rom_ptr->rom_num);
#ifdef NODUPLE
        	savegame_nomsg(ply_ptr);
#endif
        }
        return(0);
    }

}


void drop_money(ply_ptr, cmnd)
creature        *ply_ptr;
cmd             *cmnd;
{
        creature        *crt_ptr;
        room            *rom_ptr;
        object		*obj_ptr;
        long            amt;
        int             fd;

        rom_ptr = ply_ptr->parent_rom;
        fd = ply_ptr->fd;

        amt = atol(cmnd->str[1]);
        if(amt < 1) {
                print(fd, "돈의 단위는 음수가 될수 없습니다.");
                return;
        }
        if(amt > ply_ptr->gold) {
                print(fd, "당신은 그만큼의 돈을 가지고 있지 않습니다.");
                return;
        }

        load_obj(0, &obj_ptr);
        sprintf(obj_ptr->name, "%d냥", amt);
        obj_ptr->value = amt;
        add_obj_rom(obj_ptr, ply_ptr->parent_rom);

        ply_ptr->gold -= amt;
#ifdef NODUPLE
        savegame_nomsg(ply_ptr);
#endif


        print(fd, "당신은 %ld냥을 버렸습니다.\n",amt);

        broadcast_rom2(fd, ply_ptr->fd, ply_ptr->rom_num,
                       "\n%M%j %ld냥을 버렸습니다.", ply_ptr,"1", amt);

}

/**********************************************************************/
/*                              drop_all_rom                          */
/**********************************************************************/

/* This function is called when a player wishes to drop his entire    */
/* inventory into the the room.                                       */

void drop_all_rom(ply_ptr,part_obj)
creature        *ply_ptr;
char *part_obj;
{
    object  *obj_ptr,*last_obj=NULL;
    room    *rom_ptr;
    otag    *op;
    char    str[2048], str2[2048];
    int     fd, n=1,found=0;
    int index=1;

    if(!strcmp(part_obj,"모두")) index=0;

    fd = ply_ptr->fd;
    rom_ptr = ply_ptr->parent_rom;

    str[0]=0;

    if(check_event_obj(ply_ptr)) {
        print(fd, "임무 아이템이 있어 모두 버릴 수 없습니다.\n");
        return;
    }    

    op = ply_ptr->first_obj;
    while(op) {
        if(F_ISSET(ply_ptr, PDINVI) ? 1:!F_ISSET(op->obj, OINVIS)) {
            if(index) {
                obj_ptr = find_obj(ply_ptr, op, part_obj, 1);
                if(!obj_ptr) break;
                while(op->obj!=obj_ptr) op=op->next_tag;
                op=op->next_tag;
                found++;
            }
            else {
                found++;
                obj_ptr = op->obj;
                op = op->next_tag;
            }

            if(obj_ptr->questnum && ply_ptr->class < DM) continue;
            if(F_ISSET(obj_ptr, ONEWEV) && ply_ptr->class < DM) continue;
            if(last_obj && !strcmp(last_obj->name, obj_ptr->name) &&
               last_obj->adjustment == obj_ptr->adjustment)
                n++;
            else if(last_obj) {
                strcat(str, obj_str(last_obj, n, 0));
                strcat(str, ", ");
                n=1;
                if(F_ISSET(rom_ptr, RDUMPR)) free_obj(last_obj);
            }
            del_obj_crt(obj_ptr, ply_ptr);
            if(!F_ISSET(rom_ptr, RDUMPR)) {
                add_obj_rom(obj_ptr, rom_ptr);
                last_obj=obj_ptr;
            }
            else {
                if(n!=1) free_obj(obj_ptr);
                else last_obj=obj_ptr;
                ply_ptr->gold+=10;
            }
        }
        else
            op = op->next_tag;
    }
    if(found && last_obj) {
        strcat(str, obj_str(last_obj, n, 0));
        if(F_ISSET(rom_ptr, RDUMPR)) free_obj(last_obj);
    }
    else {
        print(fd, "당신은 아무것도 가지고 있지 않습니다.");
        return;
    }

#ifdef NODUPLE
    savegame_nomsg(ply_ptr);
#endif

    broadcast_rom(fd, rom_ptr->rom_num, "\n%M이 %S%j 버렸습니다.", ply_ptr, str,"3");
    print(fd, "당신은 %S%j 버렸습니다.", str,"3");

    if(F_ISSET(rom_ptr, RDUMPR))
        print(fd, "\n당신의 물건을 제물로 바쳤습니다.\n당신은 약간의 상금을 받았습니다.");
}

/**********************************************************************/
/*                              drop_all_obj                          */
/**********************************************************************/

/* This function drops all the items in a player's inventory into a      */
/* container object, if possible.  The player is pointed to by the first */
/* parameter, and the container by the second.                           */

void drop_all_obj(ply_ptr, cnt_ptr,part_obj)
creature        *ply_ptr;
object          *cnt_ptr;
char *part_obj;
{
    object  *obj_ptr, *last_obj;
    room    *rom_ptr;
    otag    *op;
    char    str[2048];
    int     fd, n = 1, found = 0, full = 0;
    int index=1;

    if(!strcmp(part_obj,"모두")) {
        index=0;
    }
    fd = ply_ptr->fd;

    last_obj = 0; str[0] = 0;
    rom_ptr = ply_ptr->parent_rom;

    op = ply_ptr->first_obj;
    while(op) {
        if((F_ISSET(ply_ptr, PDINVI) ? 1:!F_ISSET(op->obj, OINVIS)) &&
           op->obj != cnt_ptr) {
            if(index) {
                obj_ptr = find_obj(ply_ptr, op, part_obj, 1);
                if(!obj_ptr) break;
                while(op->obj!=obj_ptr) op=op->next_tag;
                op=op->next_tag;
                found++;
            }
            else {
                found++;
                obj_ptr = op->obj;
                op = op->next_tag;
            }

            if(obj_ptr->questnum && ply_ptr->class<DM) continue;
            if(F_ISSET(obj_ptr, ONEWEV)) continue;
            if(F_ISSET(obj_ptr, OCONTN)) {
                full++;
                continue;
            }
            if(F_ISSET(cnt_ptr, OCNDES))
            {
                full++;
                del_obj_crt(obj_ptr, ply_ptr);
                free(obj_ptr);
                continue;

            }

            if(cnt_ptr->shotscur >= cnt_ptr->shotsmax) {
                full++;
                continue;
            }
            cnt_ptr->shotscur++;
            del_obj_crt(obj_ptr, ply_ptr);
            add_obj_obj(obj_ptr, cnt_ptr);
            if(last_obj && !strcmp(last_obj->name, obj_ptr->name) &&
               last_obj->adjustment == obj_ptr->adjustment)
                n++;
            else if(last_obj) {
                strcat(str, obj_str(last_obj, n, 0));
                strcat(str, ", ");
                n = 1;
            }
            last_obj = obj_ptr;
        }
        else
            op = op->next_tag;
    }

    if(found && last_obj)
        strcat(str, obj_str(last_obj, n, 0));
    else {
        print(fd, "당신은 그것 안에 넣을 물건을 아무것도 갖고 있지 않습니다.");
        return;
    }

    if(full)
        print(fd, "%I안에 더이상 물건을 넣을 수 없습니다.", cnt_ptr);

    if(full == found) return;
#ifdef NODUPLE
    savegame_nomsg(ply_ptr);
#endif


    broadcast_rom(fd, rom_ptr->rom_num, "\n%M%j %S%j %1i안에 넣습니다.", ply_ptr,"1",
              str,"3",cnt_ptr);
    print(fd, "당신은 %S%j %1i안에 넣습니다.", str,"3", cnt_ptr);
    if(F_ISSET(cnt_ptr, OPERMT)) {
    	resave_rom(rom_ptr->rom_num);
#ifdef NODUPLE
    	savegame_nomsg(ply_ptr);
#endif
    }

}

/**********************************************************************/
/*                            burn태워                                */
/**********************************************************************/

/* 제사장이 아닌곳에서도 소각할수 있게하는 명령이다  */
long ply_burn_time[PMAX];

int burn(ply_ptr, cmnd)
creature        *ply_ptr;
cmd             *cmnd;
{
    room            *rom_ptr;
    object          *obj_ptr, *cnt_ptr;
    int             fd, n, match=0;
    long t;

    fd = ply_ptr->fd;

    if(cmnd->num < 2) {
        print(fd, "무엇을 태우시려구요?");
        return(0);
    }
    t=time(0);
    if(ply_burn_time[fd]>t) {
        please_wait(fd,ply_burn_time[fd]-t);
        return 0;
    }

    rom_ptr = ply_ptr->parent_rom;
    F_CLR(ply_ptr, PHIDDN);


    obj_ptr = find_obj(ply_ptr, ply_ptr->first_obj,
               cmnd->str[1], cmnd->val[1]);

    if(!obj_ptr) {
        print(fd, "당신은 그런것을 갖고 있지 않습니다.");
        return(0);
    }
	if(F_ISSET(obj_ptr, ONOBUN)) {
		print(fd, "소각할수 없는 아이템입니다.");
		return(0);
	}
/*               임무아이템 버리지 못하게 함 */
    if(obj_ptr->questnum && ply_ptr->class<SUB_DM && obj_ptr->shotscur > 0) {
        print(fd, "임무 아이템은 태우지 못합니다.");
        return(0);
    }
	if(F_ISSET(obj_ptr, ONEWEV) && ply_ptr->class < SUB_DM && obj_ptr->shotscur > 0) {
		print(fd, "이벤트 아이템은 소각할수 없습니다.");
		return(0);
	}
    del_obj_crt(obj_ptr, ply_ptr);
    print(fd, "당신은 %1i%j 태웠습니다.", obj_ptr,"3");
    broadcast_rom(fd, rom_ptr->rom_num, "\n%M%j %1i%j 태웠습니다.",
              ply_ptr,"1", obj_ptr,"3");
    free_obj(obj_ptr);
    ply_ptr->gold += 4;
    ply_ptr->experience += 1;
    print(fd, "\n당신은 약간의 상금과 경험을 받았습니다.");
    if(((time(0)+mrand(1,100))%250)==9) {
        print(fd,"\n신이 당신의 정성이 갸륵해서 경험치와 돈벼락을 내립니다.");
        broadcast_rom(fd,rom_ptr->rom_num,"\n신이 %M에게 경험치와 돈벼락을 내립니다.",ply_ptr);
        ply_ptr->gold+=100000;
        ply_ptr->experience+=10;
    }
    ply_burn_time[fd]=t+2;

    return(0);
}

/**********************************************************************/
/*                                auto_search                         */
/**********************************************************************/

/* 자동 수색  */

int auto_search(ply_ptr)
creature    *ply_ptr;
{
    room    *rom_ptr;
    xtag    *xp;
    otag    *op;
    ctag    *cp;
    int fd, chance, found = 0;

    fd = ply_ptr->fd;
    rom_ptr = ply_ptr->parent_rom;

    chance = 15 + 5*bonus[(int)ply_ptr->piety] + (ply_ptr->level/4)*2;
    chance = MIN(chance, 90);
    if(ply_ptr->class == RANGER)
        chance += ((ply_ptr->level+3)/4)*8;
    if(F_ISSET(ply_ptr, PBLIND))
        chance = MIN(chance, 20);


    xp = rom_ptr->first_ext;
    while(xp) {
        if(F_ISSET(xp->ext, XSECRT) && mrand(1,100) <= chance)
           if((!F_ISSET(xp->ext, XINVIS) || F_ISSET(ply_ptr,PDINVI))
            && !F_ISSET(xp->ext, XNOSEE)){
            found++;
            print(fd, "\n출구를 찾았습니다: %s.", xp->ext->name);
        }
        xp = xp->next_tag;
    }

    op = rom_ptr->first_obj;
    while(op) {
        if(F_ISSET(op->obj, OHIDDN) && mrand(1,100) <= chance)
        if(!F_ISSET(op->obj, OINVIS) || F_ISSET(ply_ptr,PDINVI)) { 
            found++;
            print(fd, "\n당신은 %1i%j 찾았습니다.", op->obj,"3");
        }
        op = op->next_tag;
    }

    cp = rom_ptr->first_ply;
    while(cp) {
        if(F_ISSET(cp->crt, PHIDDN) && !F_ISSET(cp->crt, PDMINV) &&
           mrand(1,100) <= chance)
        if(!F_ISSET(cp->crt, PINVIS) || F_ISSET(ply_ptr,PDINVI)) {
            found++;
            print(fd, "\n당신은 숨어있는 %S%j 찾아내었습니다.", cp->crt->name,"3");
        }
        cp = cp->next_tag;
    }

    cp = rom_ptr->first_mon;
    while(cp) {
        if(F_ISSET(cp->crt, MHIDDN) && mrand(1,100) <= chance)
        if(!F_ISSET(cp->crt, MINVIS) || F_ISSET(ply_ptr,PDINVI)) {
            found++;
            print(fd, "\n당신은 숨어있는 %1M%j 찾아내었습니다.", cp->crt,"3");
        }
        cp = cp->next_tag;
    }

    if(found)
        broadcast_rom(fd, ply_ptr->rom_num, "\n%s가 뭘 발견한것 같군요!",
                  F_ISSET(ply_ptr, MMALES) ? "그":"그녀");

    return(0);

}

