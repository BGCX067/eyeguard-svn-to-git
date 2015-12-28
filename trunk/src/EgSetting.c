/*
 * Filename : 	EgSetting.c
 * CreateDate: 	2012-08-20
 * 
 * 可设置内容：
 * 1、设置工作时间、休息时间
 * 2、设置锁屏背景颜色（用调色板）
 * 
 * 
 */
#include <EgSetting.h>

//解析配置信息
void
EgSetting_ResolveInfo(gchar *filedata,gulong lenth)
{
	guchar i,j;
	guchar site[10];
	gchar temp[10];
	
	for(i=0,j=0;i<lenth;i++)
	{
		if(filedata[i] == ',')
			site[j++] = i;
	}
	
	memset(temp,0,sizeof(temp));
	memcpy(temp,&filedata[0],site[0]);
	egSetting->bakwtime = atoi(temp);

	memset(temp,0,sizeof(temp));
	memcpy(temp,&filedata[site[0]+1],site[1]-site[0]-1);
	egSetting->bakrtime = atoi(temp);

	memset(temp,0,sizeof(temp));
	memcpy(temp,&filedata[site[1]+1],site[2]-site[1]-1);
	egSetting->bakcolor.red = atof(temp);

	memset(temp,0,sizeof(temp));
	memcpy(temp,&filedata[site[2]+1],site[3]-site[2]-1);
	egSetting->bakcolor.green = atof(temp);

	memset(temp,0,sizeof(temp));
	memcpy(temp,&filedata[site[3]+1],site[4]-site[3]-1);
	egSetting->bakcolor.blue = atof(temp);

	memset(temp,0,sizeof(temp));
	memcpy(temp,&filedata[site[4]+1],lenth-site[4]-1);
	egSetting->bakcolor.alpha = atof(temp);	

}

//获取配置文件信息
gulong 
EgSetting_GetInfo()
{
	FILE *setfile;
	gchar filedata[100];
	gulong lenth;
	
	setfile = fopen(SET_FILENAME,"r");
	if(NULL == setfile)
	{
		return 0;
	}
	memset(filedata,0,sizeof(filedata));
	fseek(setfile, 0, SEEK_SET);
	if(fgets(filedata,100,setfile) ==NULL)
	{
		return 0;
	}
	lenth = strlen(filedata);
	if(0 == lenth)
	{
		return 0;
	}
	EgSetting_ResolveInfo(filedata,lenth);
	g_print("lenth = %ld\n",lenth);
	g_print("filedata = %s\n",filedata);	
	fclose (setfile);
	return lenth;
}

//打包配置信息
gchar*
EgSetting_PackInfo(void)
{
	static gchar filedata[100];
	
	sprintf(filedata,"%ld,%d,%lf,%lf,%lf,%lf",
	        egSetting->bakwtime,egSetting->bakrtime,
	        egSetting->bakcolor.red,
	        egSetting->bakcolor.green,
	        egSetting->bakcolor.blue,
	        egSetting->bakcolor.alpha);
	
	return filedata;
}

//修改配置文件
void
EgSetting_OverrideInfo()
{
	gchar *info;
	FILE *setfile;
	
	info = EgSetting_PackInfo();
	setfile = fopen(SET_FILENAME,"w");
	fseek(setfile, 0, SEEK_SET);
	fputs(info,setfile);	
	fclose (setfile);
}

void 
EgSetting_SetDefault(void)
{
	egSetting->bakwtime = DEF_WORKTIME;
	egSetting->bakrtime = DEF_RESTTIME; 
	egSetting->bakcolor.red = DEF_COLOR_R;
	egSetting->bakcolor.green = DEF_COLOR_G;
	egSetting->bakcolor.blue = DEF_COLOR_B;
	egSetting->bakcolor.alpha = DEF_COLOR_A;
}

//配置文件初始化
void
EgSetting_FileInit()
{
	gulong lenth;

	lenth = EgSetting_GetInfo();//lenth 不等于0 则会赋值给egSetting
	if(0 == lenth)
	{
		//没有内容，创建
		EgSetting_SetDefault();
		EgSetting_OverrideInfo();
	}
}

//初始化
gboolean
EgSetting_Init()
{	
	egSetting = g_malloc(sizeof(EyeGuard_Setting));
	if(egSetting == NULL)
	{
		g_print("egSetting = NULL!");
		return FALSE;
	}
	memset(egSetting,0,sizeof(EyeGuard_Setting));

	//从文件中获取配置
	EgSetting_FileInit();
	return TRUE;	
}

//检测按钮值 是否改变
void
EgSetting_ChangValue(GtkSpinButton *spinbutton,
                     GtkScrollType  arg1,
                     gpointer       user_data) 
{
	//改变则 高亮『应用』按钮
	if( FALSE == gtk_widget_get_sensitive(egSetting->btnapply))
		gtk_widget_set_sensitive(egSetting->btnapply,TRUE);
}

static GtkWidget *
EgSetting_Create_Text (void)
{
	GtkWidget *textbox,*workbox,*restbox;
	GtkAdjustment *adjustment;
	GtkWidget *view1,*view2;
	GtkTextBuffer *buffer1,*buffer2;
	gchar *work_text = "工作时间：";
	gchar *rest_text = "休息时间：";
	gchar min,sec;

	textbox = gtk_box_new (GTK_ORIENTATION_VERTICAL,10);
	workbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL,10);
	restbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL,10);

	gtk_box_pack_start (GTK_BOX(textbox),workbox,FALSE,FALSE,10);
	gtk_box_pack_start (GTK_BOX(textbox),restbox,FALSE,FALSE,0);	

	
	//工作时间
	buffer1 = gtk_text_buffer_new(NULL);
	gtk_text_buffer_set_text(buffer1,work_text,strlen(work_text));
	view1 = gtk_text_view_new_with_buffer(buffer1);
	
	min = egSetting->bakwtime/60;
	sec = egSetting->bakwtime%60;
	//work_min
	adjustment = gtk_adjustment_new ((gdouble)min, 1.0, 60.0, 1.0, 5.0, 0.0);
	egSetting->btnworkm = gtk_spin_button_new (adjustment, 1.0, 0);	
	//work_sec
	adjustment = gtk_adjustment_new ((gdouble)sec, 0.0, 60.0, 1.0, 5.0, 0.0);
	egSetting->btnworks = gtk_spin_button_new (adjustment, 1.0, 0);	

	//休息时间	
	buffer2 = gtk_text_buffer_new(NULL);
	gtk_text_buffer_set_text(buffer2,rest_text,strlen(rest_text));
	view2 = gtk_text_view_new_with_buffer(buffer2);
	//rest_min
	min = egSetting->bakrtime/60;//
	adjustment = gtk_adjustment_new ((gdouble)min, 1.0, 60.0, 1.0, 5.0, 0.0);
	egSetting->btnrestm = gtk_spin_button_new (adjustment, 1.0, 0);	

	//布局
	gtk_box_pack_start (GTK_BOX(workbox),view1,FALSE,FALSE,12);
	gtk_box_pack_start (GTK_BOX(workbox),egSetting->btnworkm,FALSE,FALSE,10);
	gtk_box_pack_start (GTK_BOX(workbox),egSetting->btnworks,FALSE,FALSE,0);
	gtk_box_pack_start (GTK_BOX(restbox),view2,FALSE,FALSE,12);
	gtk_box_pack_start (GTK_BOX(restbox),egSetting->btnrestm,FALSE,FALSE,10);

	//连接信号
	g_signal_connect(G_OBJECT(egSetting->btnworkm), "value-changed", 
                 G_CALLBACK(EgSetting_ChangValue), NULL);
	g_signal_connect(G_OBJECT(egSetting->btnworks), "value-changed", 
                 G_CALLBACK(EgSetting_ChangValue), NULL);	
	g_signal_connect(G_OBJECT(egSetting->btnrestm), "value-changed", 
                 G_CALLBACK(EgSetting_ChangValue), NULL);	
	return textbox;
}

void
EgSetting_PickerApply (GdkRGBA *color)
{
	egSetting->bakcolor.red = color->red;
	egSetting->bakcolor.green = color->green;
	egSetting->bakcolor.blue = color->blue;
	egSetting->bakcolor.alpha = color->alpha;
	//写入配置文件
	EgSetting_OverrideInfo();	
}

//『应用』 按钮回调函数
void
EgSetting_BtnApply(GtkButton *button,gpointer user_data) 
{
	gint min,sec;
	
	gtk_widget_set_sensitive(egSetting->btnapply,FALSE);
	
	min = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(egSetting->btnworkm));
	sec = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(egSetting->btnworks));
	egSetting->bakwtime = min*60 + sec;

	min = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(egSetting->btnrestm));
	egSetting->bakrtime = min*60; 

	//写入配置文件
	EgSetting_OverrideInfo();	
	//设置标志位
	egSetting->flag_change = TRUE;
}

//『恢复默认』 按钮回调函数
void
EgSetting_BtnDefault(GtkButton *button,gpointer user_data) 
{
	gtk_widget_set_sensitive(egSetting->btnapply,FALSE);
	
	EgSetting_SetDefault();
	//写入配置文件
	EgSetting_OverrideInfo();
	
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(egSetting->btnworkm),
	                          egSetting->bakwtime/60);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(egSetting->btnworks),
	                          egSetting->bakwtime%60);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(egSetting->btnrestm),
	                          egSetting->bakrtime/60);
	//gtk_spin_button_set_value(egSetting->btnworkm,egSetting->bakcolor);
	//设置标志位
	egSetting->flag_change = TRUE;	
}

static GtkWidget *
EgSetting_Create_Button (GtkWidget *dialog)
{
	GtkWidget *btnbox;
	GtkWidget *btnapply,*btndef;
	
	btnbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL,10);
	
	btnapply = gtk_button_new_with_label ("应用");
	btndef = gtk_button_new_with_label ("恢复默认");

	
	gtk_box_pack_end (GTK_BOX(btnbox),btnapply,FALSE,FALSE,0);
	gtk_box_pack_end (GTK_BOX(btnbox),btndef,FALSE,FALSE,0);
	
	g_signal_connect(G_OBJECT(btnapply), "clicked", 
	                 G_CALLBACK(EgSetting_BtnApply), NULL);
	g_signal_connect(G_OBJECT(btndef), "clicked", 
	                 G_CALLBACK(EgSetting_BtnDefault), NULL);

	gtk_widget_set_sensitive(btnapply,FALSE);
	egSetting->btnapply = btnapply;
	egSetting->btndef = btndef;
	return btnbox;
}

GtkWidget *
EgSetting_Create_Dialog (void)
{
	GtkWidget *content_area;
	GtkWidget *dialog;
	GtkWidget *box,*textbox,*btnbox;
	
	dialog = gtk_dialog_new ();
	gtk_window_set_title (GTK_WINDOW(dialog),"设置");
	
	content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
	box = gtk_box_new (GTK_ORIENTATION_VERTICAL,10);
	gtk_container_add (GTK_CONTAINER (content_area), box);
	
	//设置工作时间、休息时间
	textbox = EgSetting_Create_Text();
	//功能键button
	btnbox = EgSetting_Create_Button(dialog);
	
	gtk_box_pack_start (GTK_BOX(box),textbox,FALSE,FALSE,0);
	gtk_box_pack_start (GTK_BOX(box),btnbox,FALSE,FALSE,0);
	
	//设置成模态窗口
	gtk_window_set_modal(GTK_WINDOW(dialog),TRUE);
	gtk_widget_show_all (dialog);	
	return dialog;
}




