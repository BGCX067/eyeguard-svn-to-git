/*
 * Filename : 	EgAbout.c
 * CreateDate: 	2012-08-22
 * 
 * 
 */
#include <EgSetting.h>

#define EG_COPYRIGHT	"版权公有，欢迎转载"
#define EG_COMMENTS		"本软件主要适用于长时间对着电脑的\n人士，有提示休息保护眼睛的功能。"
#define EG_WEBSITE		"http://hi.baidu.com/new/zaigelinux"
#define EG_WEBSITE_LABEL	"仔哥的linux空间"
#define EG_AUTHOR		"Ythink"



void
EgAbout_CBHandle(GtkDialog *dialog,
                 gint       response_id,
                 gpointer   user_data)
{	
	if(GTK_RESPONSE_CANCEL == response_id)
	{
		g_print("GTK_RESPONSE_CLOSE \n");
		gtk_widget_destroy(GTK_WIDGET(dialog));
	}
}

GtkWidget *
EgAbout_Create_Dialog()
{
	GtkWidget *adialog;
	GdkPixbuf *iconbuf;
	GtkWidget *iconwid;
	const gchar *authors[2] = {EG_AUTHOR,0};
	
	iconwid = gtk_image_new_from_file(EG_ICON32);
	iconbuf = gtk_image_get_pixbuf(GTK_IMAGE(iconwid));
	
	adialog = gtk_about_dialog_new();
	gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(adialog),EG_PRONAME);
	gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(adialog),EG_VERSION);
	gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(adialog),EG_COPYRIGHT);
	gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(adialog),EG_COMMENTS);
	gtk_about_dialog_set_license_type(GTK_ABOUT_DIALOG(adialog),GTK_LICENSE_LGPL_3_0);
	gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(adialog),EG_WEBSITE);
	gtk_about_dialog_set_website_label(GTK_ABOUT_DIALOG(adialog),EG_WEBSITE_LABEL);
	gtk_about_dialog_set_authors(GTK_ABOUT_DIALOG(adialog),authors);
	gtk_about_dialog_set_logo(GTK_ABOUT_DIALOG(adialog),iconbuf);

	g_signal_connect_swapped (adialog,
                         "response",
                         G_CALLBACK (EgAbout_CBHandle),
                         adialog);	
	return adialog;
}
