/*
 * File:   main.c
 * Author: CHAPPIDI_NAVEEN_KUMAR
 *
 * Created on 25 October, 2024, 11:08 AM
 */

#include <xc.h>
#include "main.h"
#include "matrix_keypad.h"
#include "clcd.h"
#include"adc.h"
#include "external_eeprom.h"
#include "ds1307.h"
#include "i2c.h"
#include "uart.h"

void init_ds1307();
static char * gear[] = {"ON","GN","GR","G1","G2","G3","G4","G5","C "};
short i=0,j=0,k=0,star=0,end,hour,min,sec,time_flag,field;
short speed=0,add=0,save_flag=0,view_flag=0,download_flag=0,clear_flag=0,settime_flag=0,menu_flag=0,flag=0,flag_count=0;
static char * menu[] = {"View log", "Set time", "Download log", "Clear log"};
static char  detail[10][18];
unsigned char clock_reg[3];
unsigned char time[9];

void writing_eeprom()
{
    write_external_eeprom(add++,time[0]);
    write_external_eeprom(add++,time[1]);
    write_external_eeprom(add++,time[3]);
    write_external_eeprom(add++,time[4]);
    write_external_eeprom(add++,time[6]);
    write_external_eeprom(add++,time[7]);
    write_external_eeprom(add++,gear[i][0]);
    write_external_eeprom(add++,gear[i][1]);
    write_external_eeprom(add++,(speed / 10) + 48);
    write_external_eeprom(add++,(speed % 10) + 48);             
}

void reading_eeprom()
{
    short index=0 , add2=0;
    if(flag_count==1)
    {
        add2 = add;
        end = 10;
    }
    else
    {
        add2 = 0;
        end = add/10;
    }
    for(short main_index=0;main_index<end;main_index++)
    {
        detail[main_index][index++] = main_index+48;
        detail[main_index][index++] = ' ';
        detail[main_index][index++] = read_external_eeprom(add2++);
        detail[main_index][index++] = read_external_eeprom(add2++);
        detail[main_index][index++] = ':';
        detail[main_index][index++] = read_external_eeprom(add2++);
        detail[main_index][index++] = read_external_eeprom(add2++);
        detail[main_index][index++] = ':';
        detail[main_index][index++] = read_external_eeprom(add2++);
        detail[main_index][index++] = read_external_eeprom(add2++);
        detail[main_index][index++] = ' ';
        detail[main_index][index++] = read_external_eeprom(add2++);
        detail[main_index][index++] = read_external_eeprom(add2++);
        detail[main_index][index++] = ' ';
        detail[main_index][index++] = read_external_eeprom(add2++);
        detail[main_index][index++] = read_external_eeprom(add2++);
        detail[main_index][index++] = '\0';
        index=0;
        if(add2==100)
            add2=0;
    }
}

static void get_time(void)
{
	clock_reg[0] = read_ds1307(HOUR_ADDR);
	clock_reg[1] = read_ds1307(MIN_ADDR);
	clock_reg[2] = read_ds1307(SEC_ADDR);

	time[0] = '0' + ((clock_reg[0] >> 4) & 0x03);
    time[1] = '0' + (clock_reg[0] & 0x0F);
	time[2] = ':';
	time[3] = '0' + ((clock_reg[1] >> 4) & 0x0F);
	time[4] = '0' + (clock_reg[1] & 0x0F);
	time[5] = ':';
	time[6] = '0' + ((clock_reg[2] >> 4) & 0x0F);
	time[7] = '0' + (clock_reg[2] & 0x0F);
	time[8] = '\0';
}

void calculate_speed(unsigned short adc_reg_val)
{
    speed = (adc_reg_val/10);
    if(speed>99)
        speed=99;
}

void check_matrix_keypad(void)
{   
	unsigned char key;
	key = read_switches(STATE_CHANGE);
    
    if(key==1 && settime_flag==1)
    {
        if(field==0)
        {
            hour++;
            if(hour==24)
                hour=0;
        }
        else if(field==1)
        {
            min++;
            if(min==60)
                min=0;
        }
        else if(field==2)
        {
            sec++;
            if(sec==60)
                sec=0;
        }            
        return;
    }
    else if(key==2 && settime_flag==1)
    {
        field++;
        if(field==3)
            field=0;
        return;
    } 
   else if(key==11 && settime_flag==1)
{
    // Set the time in the DS1307 RTC
    write_ds1307(HOUR_ADDR, ((hour / 10) << 4) | (hour % 10));
    write_ds1307(MIN_ADDR, ((min / 10) << 4) | (min % 10));
    write_ds1307(SEC_ADDR, ((sec / 10) << 4) | (sec % 10));

    // After setting the time, go directly to the dashboard
    menu_flag = 0;       // Go to the dashboard
    settime_flag = 0;    // Reset set time flag
    j = 0;
    star = 0;
    k = 0;
    flag = 0;
    time_flag = 0;      // Reset time flag
    field = 0;          // Reset the field selector
    return;
}

    else if(key==12 && settime_flag==1)
    {
        menu_flag=0;
        settime_flag=0;
        j=0;
        star=0;
        k=0;
        flag=0;
        time_flag=0;
        field=0;
        return;
    }
    
    if(key==12 && menu_flag!=0)
    {
        CLEAR_DISP_SCREEN;
        menu_flag--;
        view_flag=0;
        clear_flag=0;
        download_flag=0;
        settime_flag=0;
        j=0;
        star=0;
        k=0;
        flag=0;
        return;
    }   
    
    if(view_flag==1 && key==2 && j<9)
    {
        if(flag_count==1)
            j++;
        else if(flag_count==0 && j<(add/10)-1)
            j++;        
        return;
    }
    else if(view_flag==1 && key==1 && j>0)
    {
        j--;
        return;
    }
        
    if(key==1 && i<=8 && menu_flag==0)
    {
        if(add==100)
        {
            flag_count=1;
            add=0;
        } 
        save_flag=0;
        if(i==7)
            return;
        else if(i==8)
            i=1;
        else
            i++;       
        writing_eeprom();       
    }       
    else if(key==2 && i>1 && menu_flag==0)
    {
        if(add==100)
        {
            flag_count=1;
            add=0;
        } 
        save_flag=0;
        if(i==8)
            i=1;
        else
            i--;
        writing_eeprom();
    }
    else if(key==3 && menu_flag==0)
    {
        if(add==100)
        {
            flag_count=1;
            add=0;
        } 
        i=8;
        
        if(save_flag==0)
            writing_eeprom();
        save_flag++;
    }
          
    if(key==11 && menu_flag<2)
    {
        menu_flag++;
    }
    
    if(key==2 && k<3 && menu_flag==1)
    {
        if(add==100)
        {
            flag_count=1;
            add=0;
        }
        if(star==0)
        {
            star=1;
            flag++;
            return;
        }
        if(flag!=0)
            k++;
        flag++;
        if(k==3)
        {
            k=2;
            flag=3;
        }
    }
    else if(key==1 && k<3 && menu_flag==1)
    {
        if(add==100)
        {
            flag_count=1;
            add=0;
        }
        if(star==1)
        {
            star=0;
            flag--;
            return;
        }
        if(flag!=3)
            k--;
        flag--;
        if(k==-1)
        {
            k=0;
            flag=0;
        }           
    }
    if(menu_flag==2)
{
    if(star==0 && k==0)           // View log functionality
        view_flag=1;
    else if(star==1 && k==0)      // Clear log functionality (works as Set Time)
        settime_flag=1;
    else if(star==0 && k==1)      // Set time functionality (works as Clear log)
        clear_flag=1;
    else if(star==1 && k==1)      // Download log functionality
        download_flag=1;
    else if(star==0 && k==2)      // Download log functionality
        download_flag=1;
    else if(star==1 && k==2)      // Set time functionality (works as Clear log)
        clear_flag=1;
}
}

static void init_config(void)
{
    init_clcd();
    init_matrix_keypad();
    init_adc(); 
    init_i2c();
	init_ds1307();
    init_uart();
}

void main(void)
{
    unsigned short adc_reg_val;
    init_config();

    while (1)
    {
        check_matrix_keypad();
        adc_reg_val = read_adc(CHANNEL4);
        calculate_speed(adc_reg_val);        
        get_time();
        if(menu_flag==0)
        {
            CLEAR_DISP_SCREEN;
            clcd_print("  TIME    EV  SP", LINE1(0)); 
            clcd_print(time, LINE2(0));
            clcd_print(gear[i], LINE2(10)); 
            clcd_putch((speed % 10) + 48, LINE2(15));
            clcd_putch((speed / 10) + 48, LINE2(14));
        }
        else if(menu_flag==1)
        {
            clcd_print("                  ", LINE1(0));
            clcd_print("                  ", LINE2(0));
            if(star==0)
                clcd_putch('*', LINE1(0));
            else
                clcd_putch('*', LINE2(0));
            
            clcd_print(menu[k], LINE1(2));
            clcd_print(menu[k+1], LINE2(2));
        }
        else if(view_flag==1)
        {      
            if(add==0 && flag_count==0)
            {
                unsigned long long int delay=0;
                clcd_print(" LOGS ARE  ", LINE1(0));
                clcd_print(" ARE NOT FOUND : ", LINE2(0));
                while(delay++<300000);
                delay=0;
                menu_flag=1;
                k=0;
                star=0;
                view_flag=0;
            }
            else
            {
                reading_eeprom();
                clcd_print("                  ", LINE1(0));
                clcd_print("                  ", LINE2(0));
                clcd_print("# VIEW LOG : ", LINE1(0));
                clcd_print(detail[j], LINE2(0));     
            }
        }
        else if(clear_flag==1)
        {
            add=0;
            flag_count=0;
            unsigned long long int delay=0;
            CLEAR_DISP_SCREEN;
            clcd_print(" lOGS CLEARED ", LINE1(0));
            clcd_print(" SUCCESSFULLY ", LINE2(0));
            while(delay++<300000);
            delay=0;
            menu_flag=1;
            k=0;
            star=0;
            clear_flag=0;            
        }
        else if(download_flag==1)
        {
            unsigned long long int delay=0;
            CLEAR_DISP_SCREEN;           
            if(add==0 && flag_count==0)
            {
                clcd_print("HI NO LOGS", LINE1(0));
                clcd_print("TO DOWNLOAD :", LINE2(0));
                while(delay++<300000);
                delay=0;
                menu_flag=1;
                k=0;
                star=0;
                download_flag=0;
            }
            else
            {
                reading_eeprom();
                clcd_print("HEY DOWNLOAD", LINE1(0));
                clcd_print("IS COMPLETED..", LINE2(0));               
                for(int it=0;it<end;it++)
                {
                    puts(detail[it]);
                    puts("\n\r");
                }
                while(delay++<300000);
                delay=0;
                menu_flag=1;
                k=0;
                star=0;
                download_flag=0;
            }
        }
        else if(settime_flag==1)
        {
            if(time_flag==0)
            {
                hour = (time[0]-48)*10;
                hour += (time[1]-48);
                min = (time[3]-48)*10;
                min += (time[4]-48);
                sec = (time[6]-48)*10;
                sec += (time[7]-48);
                time_flag=1;
            }
            unsigned long long int delay=0;
            clcd_print("HH:MM:SS            ", LINE1(0));
            clcd_putch((hour/10) + '0', LINE2(0));
            clcd_putch((hour%10) + '0',LINE2(1));
            if(field==0)
            {
                while(delay++ < 5000);
                delay=0;
            }
            clcd_putch(':', LINE2(2));
            clcd_putch((min/10) + '0', LINE2(3));
            clcd_putch((min%10) + '0', LINE2(4));
            if(field==1)
            {
                while(delay++ < 5000);
                delay=0;
            }
            clcd_putch(':', LINE2(5));
            clcd_putch((sec/10) + '0', LINE2(6));
            clcd_putch((sec%10) + '0', LINE2(7));
            if(field==2)
            {
                while(delay++ < 5000);
                delay=0;
            }
            clcd_print("      ", LINE2(8)); 
            if(field==0)
            { 
                clcd_print("  ", LINE2(0));
                while(delay++ < 5000);
                delay=0;
            }
            else if(field==1)
            {
                clcd_print("  ", LINE2(3));
                while(delay++ < 5000);
                delay=0;
            }
            else if(field==2)
            {
                clcd_print("  ", LINE2(6));
                while(delay++ < 5000);
                delay=0;
            }
            
        }
        
    }
}