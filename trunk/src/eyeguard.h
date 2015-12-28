#ifndef __EYEGUARD_H_
#define __EYEGUARD_H_

//#include <stdlib.h>
#include <EgSetting.h>





/* For testing propose use the local (not installed) ui file */
/* #define UI_FILE PACKAGE_DATA_DIR"/ui/eyeguard.ui" */
#define UI_FILE			"src/eyeguard.ui"
#define TOP_WINDOW  	"window"





void EG_CBHandle_BtnBegin(GtkButton *button,gpointer user_data) ;
gboolean EG_Countdown_Handler(gpointer user_data);
void EG_GetSettingInfo(void);
void EG_CBHandle_BtnNow(GtkButton *button,gpointer user_data);
void EG_LockDialog_Create(void);
void EG_LockDialog_Close(GtkDialog *dialog,gint response_id,gpointer user_data);


typedef struct EYEGUARD_MAIN_
{
	gulong worktime;
	gint tcounter;
	gchar delaynum;//延迟休息次数，最多三次
	
	GtkWidget *window;
	GtkWidget *dialog;
	GtkWidget *btnbegin;
	GtkWidget *btnsetting;	
	GtkWidget *btnnow;
	GtkWidget *btndelay;
	GtkWidget *infoview,*cdview;
	
	GtkWidget *delay1min;
	GtkWidget *delay3min;
	GtkWidget *delay8min;

	//GtkToolItem *itemstart;
	
	GtkWidget *label;
	GdkRGBA  color; 	
	
	GtkTextBuffer *cdbuf;;//倒计时文本

	//标志位
	gboolean flag_time_work;//工作时间定时器标志位
	gboolean flag_time_rest;//休息时间定时器标志位

}EyeGuard_Main;

static EyeGuard_Main *egMain;

#endif
