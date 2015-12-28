#ifndef __EGSETTING_H_
#define __EGSETTING_H_

#include <stdlib.h>
#include <stdio.h>
#include <config.h>
#include <gtk/gtk.h>
#include <glib/gi18n.h>

/**********宏定义************/
#define EG_PRONAME		"护眼卫士"
#define EG_VERSION		"V0.7"

#define SET_FILENAME 	"usr.eg"
#define EG_ICON16		"/usr/local/eyeguard/icon/icon16.png"
#define EG_ICON32		"/usr/local/eyeguard/icon/icon32.png"
#define EG_ICON64		"/usr/local/eyeguard/icon/icon64.png"
#define EG_ICON128		"/usr/local/eyeguard/icon/icon128.png"


//默认配置
#define DEF_WORKTIME	(45*60)
#define DEF_RESTTIME	(2*60)
#define DEF_COLOR_R 	(0.36)
#define DEF_COLOR_G	 	(0.52)
#define DEF_COLOR_B		(0.15)
#define DEF_COLOR_A		(1.0)

/**********函数声明************/
//EgSetting.c
GtkWidget* EgSetting_Create_Dialog (void);
gboolean EgSetting_Init(void);
gulong EgSetting_GetInfo(void); 
void EgSetting_PickerApply (GdkRGBA *color);
//EgAbout.c
GtkWidget* EgAbout_Create_Dialog(void);


typedef struct EYEGUARD_SETTING_
{
	gulong 		bakwtime;//用于备份worktime
	guchar 		bakrtime;//用于备份resttime
	GdkRGBA  	bakcolor;
	
	
	GtkWidget *btnworkm;//输入分钟
	GtkWidget *btnworks;//输入秒钟

	GtkWidget *btnrestm;//输入分钟
	
	GtkWidget *btndef;
	GtkWidget *btnapply;//应用

	gboolean  flag_change;
	
}EyeGuard_Setting;

EyeGuard_Setting *egSetting;

#endif
