/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * main.c
 * Copyright (C) 2012 ythink <ythink@gmail.com>
 * 
 * EyeGuard is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * EyeGuard is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <eyeguard.h>




/* Signal handlers */
/* Note: These may not be declared static because signal autoconnection
 * only works with non-static methods
 */

gchar*
EG_Get_Message(void)
{
	static struct tm *loctime;
	static time_t curtime;
	static gchar countdown[100];
	static gchar message[1024];
	static gchar *buffer ="\n休息时间到，请注意保护眼睛。\n劳逸结合，健康工作！\n";

	memset(message,0,sizeof(message));
	
	curtime = time(NULL);
	
	loctime = localtime(&curtime);
	strftime(message, 256, "%T", loctime);
	strcat(message,buffer);

	sprintf(countdown,"\n%02d:%02d 后将解锁\n",
	        egMain->tcounter/60,egMain->tcounter%60);
	strcat(message,countdown);

	return message;
}

void
EG_CBHandle_BtnSetting()
{
	g_print("print something..\n");
	//取得用户设置的时间
	if(TRUE == egSetting->flag_change )
	{
		egSetting->flag_change = FALSE;
		EG_GetSettingInfo();
	}
}

void
EG_BtnToolSetting(GtkButton *button,gpointer user_data) 
{
	GtkWidget *settingdialog;
	settingdialog = EgSetting_Create_Dialog ();
	
	g_signal_connect(G_OBJECT(settingdialog),"response",
	                 G_CALLBACK(EG_CBHandle_BtnSetting),NULL);	
}



void
EG_BtnToolAbout(GtkButton *button,gpointer user_data) 
{
	GtkWidget *aboutdialog;
	aboutdialog = EgAbout_Create_Dialog();
	
	gtk_widget_show(aboutdialog);
}

//void
//EG_BtnToolHelp(GtkButton *button,gpointer user_data) 
//{
//	system("gedit README");
//}

void EG_CBHandle_BtnPicker( GtkDialog *dialog,
                 			gint       response_id,
                 			gpointer   user_data)
{
	GtkWidget *colorselect;
	if(GTK_RESPONSE_OK == response_id)
	{
		g_print("GTK_RESPONSE_OK\n");
		colorselect = gtk_color_selection_dialog_get_color_selection(
						GTK_COLOR_SELECTION_DIALOG(dialog));
		gtk_color_selection_get_current_rgba(
						GTK_COLOR_SELECTION(colorselect),&egMain->color);
		EgSetting_PickerApply(&egMain->color);//写入配置文件
	}
	else if(GTK_RESPONSE_CANCEL == response_id)
	{
		g_print("GTK_RESPONSE_CANCEL\n");
	}
	gtk_widget_destroy(GTK_WIDGET(dialog));
}

void
EG_BtnToolPicker(GtkButton *button,gpointer user_data)
{
	GtkWidget *colordialog;
	colordialog = gtk_color_selection_dialog_new("选择背景颜色");
	g_signal_connect_swapped (colordialog,
                             "response",
                             G_CALLBACK (EG_CBHandle_BtnPicker),
                             colordialog);	
	gtk_widget_show(colordialog);
}


static GtkWidget*
EG_Create_ToolBar(void)
{
	GtkWidget *toolbar;
	GtkToolItem *set,*picker,*about,*sep,*exit;//,*help
	
	toolbar = gtk_toolbar_new();
	gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_ICONS);

	gtk_container_set_border_width(GTK_CONTAINER(toolbar), 2);

	set = gtk_tool_button_new_from_stock(GTK_STOCK_PREFERENCES);
	picker = gtk_tool_button_new_from_stock(GTK_STOCK_COLOR_PICKER);
	about = gtk_tool_button_new_from_stock(GTK_STOCK_ABOUT);
	//help = gtk_tool_button_new_from_stock(GTK_STOCK_HELP);
	sep = gtk_separator_tool_item_new();
	exit = gtk_tool_button_new_from_stock(GTK_STOCK_QUIT);

	gtk_widget_set_tooltip_text(GTK_WIDGET(set),"Setting");
	gtk_widget_set_tooltip_text(GTK_WIDGET(picker),"Choose Color");
	gtk_widget_set_tooltip_text(GTK_WIDGET(about),"About");
	//gtk_widget_set_tooltip_text(GTK_WIDGET(help),"Help");
	gtk_widget_set_tooltip_text(GTK_WIDGET(exit),"Exit");
	
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), set, -1);	
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), picker, -1);	
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), about, -1);	
	//gtk_toolbar_insert(GTK_TOOLBAR(toolbar), help, -1);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), sep, -1);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), exit, -1);

	g_signal_connect(G_OBJECT(exit), "clicked", 
	                 G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(G_OBJECT(picker),"clicked",
	                 G_CALLBACK(EG_BtnToolPicker),NULL);	
	g_signal_connect(G_OBJECT(set),"clicked",
	                 G_CALLBACK(EG_BtnToolSetting),NULL);
	g_signal_connect(G_OBJECT(about),"clicked",
	                 G_CALLBACK(EG_BtnToolAbout),NULL);
	//g_signal_connect(G_OBJECT(help),"clicked",
	//                 G_CALLBACK(EG_BtnToolHelp),NULL);	
	return toolbar;
}

static GtkWidget*
EG_Create_InfoText(void)
{
	GtkWidget *view;
	GtkTextBuffer *buffer;
	const gchar *text="\n\t欢迎使用护眼提示! ^_^ \n\t请设置好休息时间，并按下『开始』计时。\n\t温馨提示：默认时间是45分钟。\n";
	
	buffer = gtk_text_buffer_new(NULL);
	gtk_text_buffer_set_text(buffer,text,strlen(text));
	view = gtk_text_view_new_with_buffer(buffer);
	gtk_text_view_set_editable(GTK_TEXT_VIEW(view),FALSE);
	return view;
}

void
EG_CountDown_SetBuf(GtkTextBuffer *buffer)
{
	static gchar cbtext[1024];
	static gchar min,sec;
	static const gchar *text="倒计时：";

	min = egMain->worktime/60;
	sec = egMain->worktime%60;
	memset(cbtext,0,sizeof(cbtext));
	sprintf(cbtext,"\t%s%02d:%02d",text,min,sec);
			
	gtk_text_buffer_set_text(buffer,cbtext,strlen(cbtext));
}

//倒计时文本
static GtkWidget*
EG_Create_CountDownText(void)
{
	GtkWidget *view;
	
	egMain->cdbuf = gtk_text_buffer_new(NULL);
	view = gtk_text_view_new_with_buffer(egMain->cdbuf);
	EG_CountDown_SetBuf(egMain->cdbuf);
	gtk_text_view_set_editable(GTK_TEXT_VIEW(view),FALSE);
	return view;	
}

//工作时间倒计时 回调函数
gboolean            
EG_TimeHandler_Work(gpointer user_data)
{
	if(FALSE == egMain->flag_time_work)//关闭定时器
	{
		return FALSE;
	}
	
	if(egMain->worktime == 0)
	{
		EG_CBHandle_BtnNow(NULL,NULL);
		return FALSE;
	}
	EG_CountDown_SetBuf(egMain->cdbuf);
	egMain->worktime--;
	//gtk_widget_show(egMain->cdview);
	return TRUE;
}


static gboolean
EG_TimeHandler_Rest(GtkWidget *widget)
{
	if(FALSE == egMain->flag_time_rest)//关闭定时器
	{
		return FALSE;
	}

	if((egSetting->bakrtime - egMain->tcounter) == 60)
	{
		gtk_dialog_add_button(GTK_DIALOG(egMain->dialog),
		                      "解锁",GTK_RESPONSE_NONE);
	}
	
	if(0 == egMain->tcounter)
	{
		//Lock Screen
		g_print("\nLock Screen Now!\n");
		//关闭对话框并重新初始化
		EG_LockDialog_Close(NULL,0,NULL);
		return FALSE;
	}
	egMain->tcounter--;
	gtk_label_set_text(GTK_LABEL(egMain->label),EG_Get_Message());
	gtk_widget_queue_draw(widget);
	return TRUE;
}

void
EG_LockDialog_Close(GtkDialog *dialog,gint response_id,gpointer user_data)
{
	g_print("关闭对话框\n");
	//关闭对话框
	gtk_widget_destroy(GTK_WIDGET(egMain->dialog));
	//关闭定时器
	egMain->flag_time_rest = FALSE;
	egMain->dialog = NULL;
	egMain->delaynum = 3;
	gtk_widget_set_sensitive(egMain->btndelay,TRUE);
	//获取配置信息
	EG_GetSettingInfo();
	
	//设置工作时间倒计时
	if(FALSE == egMain->flag_time_work)
	{
		egMain->flag_time_work = TRUE;
		g_timeout_add_seconds(1, (GSourceFunc)EG_TimeHandler_Work, NULL);
	}	
}

//置顶锁屏界面 100ms一次
static gboolean
EG_TimeHandler_KeepAbove(GtkWidget *widget)
{
	if(NULL == egMain->dialog)
	{
		return FALSE;
	}
	gtk_window_set_keep_above(GTK_WINDOW(egMain->dialog),TRUE);
	return TRUE;
}

//屏蔽锁屏界面的按键
gboolean 
EG_LockDialog_ShieldKey(GtkWidget *widget,
                        GdkEvent  *event,
                        gpointer   user_data)
{
	g_print("EG_LockDialog_ShieldKey = %s\n",(gchar *)user_data);
	return TRUE;//TRUE to stop other handlers from being invoked for the event
}

void
EG_LockDialog_Create()
{
	static GtkWidget *content_area;
	static GtkWidget *dialog;
	static GdkRGBA  color;  

	dialog = gtk_dialog_new ();
	gtk_window_fullscreen(GTK_WINDOW(dialog));

	
	gtk_widget_override_background_color(dialog,GTK_STATE_FLAG_NORMAL,&egMain->color);
    color.red = 0.0;  
    color.green = 0.0;  
    color.blue = 1.0;
	color.alpha = 1.0;
	gtk_widget_override_color(dialog,GTK_STATE_FLAG_NORMAL,&color);
	
	/* Add the label, and show everything we've added to the dialog */
	content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));

	egMain->label = gtk_label_new (NULL);
	gtk_widget_set_valign(content_area,GTK_ALIGN_CENTER);
	gtk_widget_set_halign(content_area,GTK_ALIGN_CENTER);
	gtk_container_add (GTK_CONTAINER (content_area), egMain->label);

	//设置休息时间倒计时
	if(FALSE == egMain->flag_time_rest)
	{
		egMain->flag_time_rest = TRUE;
		g_timeout_add_seconds(1, (GSourceFunc)EG_TimeHandler_Rest, dialog);
	}

	g_timeout_add(100,(GSourceFunc)EG_TimeHandler_KeepAbove,dialog);

	g_signal_connect(G_OBJECT(dialog),"response",
             		G_CALLBACK (EG_LockDialog_Close),NULL);

	
	g_signal_connect(G_OBJECT(dialog),"key-release-event",
         			G_CALLBACK (EG_LockDialog_ShieldKey),"1");//对应esc键
	g_signal_connect(G_OBJECT(dialog),"key-press-event",
         			G_CALLBACK (EG_LockDialog_ShieldKey),"2");//对应esc键
	
	//设置成模态窗口
	gtk_window_set_modal(GTK_WINDOW(dialog),TRUE);
	gtk_widget_show_all (dialog);
	
	egMain->dialog = dialog;
}

//立即休息 按钮处理
void
EG_CBHandle_BtnNow(GtkButton *button,gpointer user_data) 
{
	g_print("\nRest Now\n");
	//获取配置信息
	EG_GetSettingInfo();
	//关闭主界面工作定时器
	egMain->flag_time_work = FALSE ;
	//使能工具栏『开始』按钮
	//gtk_widget_set_sensitive(GTK_WIDGET(egMain->itemstart),TRUE);
	EG_LockDialog_Create();
}

//推迟休息 按钮处理
void
EG_CBHandle_BtnDelay(GtkButton *button,gpointer user_data) 
{
	static gint delaytime = 0;
	if(egMain->delaynum == 1)
	{
		//关闭按钮
		gtk_widget_set_sensitive(egMain->btndelay,FALSE);
	}
	egMain->delaynum--;

	if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(egMain->delay1min))==TRUE)
	{
		delaytime = 60*1;
	}
	else if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(egMain->delay3min))==TRUE)
	{
		delaytime = 60*3;
	}
	else if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(egMain->delay8min))==TRUE)
	{
		delaytime = 60*8;
	}
	g_print("\ndelaytime = %d\n",delaytime);
	egMain->worktime += delaytime;
}

//获取设置值
void
EG_GetSettingInfo(void)
{
	EgSetting_GetInfo();//从设置对话框中读取值
	egMain->worktime = egSetting->bakwtime;
	egMain->tcounter = egSetting->bakrtime;
	egMain->color.red = egSetting->bakcolor.red;
	egMain->color.green = egSetting->bakcolor.green;
	egMain->color.blue = egSetting->bakcolor.blue;
	egMain->color.alpha = egSetting->bakcolor.alpha;
}

//处理开始按钮 开始倒计时
void
EG_CBHandle_BtnBegin(GtkButton *button,gpointer user_data) 
{
	gtk_widget_set_sensitive(egMain->btndelay,TRUE);

	//设置工作时间倒计时
	if(FALSE == egMain->flag_time_work)
	{
		egMain->flag_time_work = TRUE;
		g_timeout_add_seconds(1, (GSourceFunc)EG_TimeHandler_Work, NULL);
	}
	
}

static GtkWidget*
EG_Create_Button(void)
{
	GtkWidget *btnbox;
	GtkWidget *label;
	
	btnbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL,10);

	//功能按钮
	egMain->btnnow = gtk_button_new_with_label("立即休息");
	egMain->btndelay = gtk_button_new_with_label("推迟休息");

	g_signal_connect(egMain->btnnow,"clicked",
	                 G_CALLBACK(EG_CBHandle_BtnNow),NULL);
	g_signal_connect(egMain->btndelay,"clicked",
	                 G_CALLBACK(EG_CBHandle_BtnDelay),NULL);
	
	egMain->delay1min = gtk_radio_button_new(NULL);
	label = gtk_label_new("1分钟");
	gtk_container_add (GTK_CONTAINER(egMain->delay1min),label);
	egMain->delay3min = gtk_radio_button_new_with_label_from_widget(
							GTK_RADIO_BUTTON(egMain->delay1min),"3分钟");
	egMain->delay8min = gtk_radio_button_new_with_label_from_widget(
	                        GTK_RADIO_BUTTON(egMain->delay3min),"8分钟");
	
	gtk_box_pack_start (GTK_BOX(btnbox),egMain->btnnow,FALSE,FALSE,10);
	gtk_box_pack_start (GTK_BOX(btnbox),egMain->btndelay,FALSE,FALSE,0);
	gtk_box_pack_start (GTK_BOX(btnbox),egMain->delay1min,FALSE,FALSE,0);
	gtk_box_pack_start (GTK_BOX(btnbox),egMain->delay3min,FALSE,FALSE,0);
	gtk_box_pack_start (GTK_BOX(btnbox),egMain->delay8min,FALSE,FALSE,0);

	gtk_widget_set_sensitive(egMain->btndelay,FALSE);
	
	return btnbox;
}

static GList*
EG_Set_Icons(void)
{
	GdkPixbuf *iconbuf[4];// 16x16, 32x32, (64x64, 128x128)
	GtkWidget *iconwid[4];
	gchar *iconfile[4]={EG_ICON16,
						EG_ICON32,
						EG_ICON64,
						EG_ICON128};
	guchar i;
	GList *iconlist=NULL;
	
	for(i=0;i<4;i++)
	{
		iconwid[i] = gtk_image_new_from_file(iconfile[i]);
		iconbuf[i] = gtk_image_get_pixbuf(GTK_IMAGE(iconwid[i]));
		iconlist = g_list_append(iconlist,iconbuf[i]);
	}
	
	return iconlist;
}

static GtkWidget*
EG_Create_Window(void)
{
	GtkWidget *window;
	GtkWidget *box;
	GtkWidget *toolbar;	
	GtkWidget *infoview,*cdview;
	GtkWidget *btnbox;
	GList *iconlist;
//	GError **err=NULL;
	
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (window), EG_PRONAME);

	iconlist = EG_Set_Icons();
	gtk_window_set_icon_list(GTK_WINDOW (window),iconlist);
	
	//gtk_window_resize (GTK_WINDOW(window),250,200);
	/* Exit when the window is closed */
	g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

	
	box = gtk_box_new (GTK_ORIENTATION_VERTICAL,0);
	gtk_container_add(GTK_CONTAINER(window),box);	
	
	
	toolbar = EG_Create_ToolBar();
	gtk_box_pack_start (GTK_BOX(box),toolbar,FALSE,FALSE,3);

	infoview = EG_Create_InfoText();
	gtk_box_pack_start (GTK_BOX(box),infoview,FALSE,FALSE,3);
	
	cdview = EG_Create_CountDownText();
	gtk_box_pack_start (GTK_BOX(box),cdview,FALSE,FALSE,0);
	egMain->cdview = cdview;
	
	btnbox = EG_Create_Button();
	gtk_box_pack_start (GTK_BOX(box),btnbox,FALSE,FALSE,10);	
	
	return window;	
}



gboolean
EG_Init()
{
	EgSetting_Init();
	egMain = g_malloc(sizeof(EyeGuard_Main));
	if(egMain == NULL)
	{
		g_print("egMain = NULL!");
		return FALSE;
	}
	memset(egMain,0,sizeof(EyeGuard_Main));
	
	EG_GetSettingInfo();
	egMain->delaynum = 3;
	return TRUE;
}

int
main (int argc, char *argv[])
{	
	int ret;

#ifdef ENABLE_NLS
	bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
	textdomain (GETTEXT_PACKAGE);
#endif

	gtk_init (&argc, &argv);
	
	ret = EG_Init();
	
		
	egMain->window = EG_Create_Window();	
	gtk_widget_show_all (egMain->window);
	
	EG_CBHandle_BtnBegin(NULL,NULL);
	
	gtk_main ();
	return ret;
}
