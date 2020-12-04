#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_STRING 256
#define NUMBER_CONTAINER 4
#define TABLE_NUMBER 25
#define ROOM_PLAYER 5
#define GLADE_FILE_NAME "glade/test.glade"
#define GLADE_CSS "style.css"
#define WINDOW_NAME "window_main"
#define WINDOW_ERR "window_error"
#define WINDOW_BINGO "window_bingo"
#define STACK_CONTAINER "stack"
#define LBL_USER "lbl_username"
#define LBL_ERR "lbl_error"
#define LBL_BINGO "lbl_bingo"
#define ENTRY_USER "entry_username"
#define ENTRY_SEARCH_ROOM "entry_search_room"

#define MES_ERR_USER "Your username is error!"
#define MES_NO_USER "Please enter your username first!"
#define MES_CUR_USER "Your name is: "
#define MES_BINGO1 "!!! YOU WON !!!"
#define MES_BINGO2 "Won!"

typedef struct {
    int currentWindow;
    char* currUser;
    GtkEntry *w_entry_username, *w_entry_search_room;
    GtkLabel *w_lbl_username, *w_lbl_err, *w_lbl_bingo;
    GtkStack *w_stack;
    GtkWidget *w_container_list[NUMBER_CONTAINER], *w_err_window, *w_bingo_window;
    GtkToggleButton *w_tog_btn_table[TABLE_NUMBER], *w_tog_btn_player[ROOM_PLAYER];
} app_widgets;

char container_name[NUMBER_CONTAINER][MAX_STRING] = {"container_menu", "container_lobby", "container_room", "container_playing"};
char tog_btn_table_name[TABLE_NUMBER][MAX_STRING] = {"tog_btn_table_0", "tog_btn_table_1", "tog_btn_table_2", "tog_btn_table_3", "tog_btn_table_4"
                                                    , "tog_btn_table_5", "tog_btn_table_6", "tog_btn_table_7", "tog_btn_table_8", "tog_btn_table_9"
                                                    , "tog_btn_table_10", "tog_btn_table_11", "tog_btn_table_12", "tog_btn_table_13", "tog_btn_table_14"
                                                    , "tog_btn_table_15", "tog_btn_table_16", "tog_btn_table_17", "tog_btn_table_18", "tog_btn_table_19"
                                                    , "tog_btn_table_20", "tog_btn_table_21", "tog_btn_table_22", "tog_btn_table_23", "tog_btn_table_24"};
char tog_btn_player_name[ROOM_PLAYER][MAX_STRING] = {"tog_btn_player0", "tog_btn_player1", "tog_btn_player2", "tog_btn_player3", "tog_btn_player4", };
GtkBuilder *builder; 
GtkWidget  *window;

static void load_css(void);
bool checkString(char *string);
int showError(char* mesError, GtkLabel *lbl_err, GtkWidget *err_window);
int showBingo(char* mesBingo, GtkLabel *lbl_bingo, GtkWidget *bingo_window);
gboolean on_window_error_delete_event(GtkWidget *widget, GdkEvent *event, gpointer data);

int main(int argc, char *argv[]) {
    app_widgets *widgets = g_slice_new(app_widgets);
    gtk_init(&argc, &argv);
    load_css();
    builder = gtk_builder_new_from_file(GLADE_FILE_NAME);

    //Get window
    window = GTK_WIDGET(gtk_builder_get_object(builder, WINDOW_NAME));
    widgets->w_err_window = GTK_WIDGET(gtk_builder_get_object(builder, WINDOW_ERR));
    widgets->w_bingo_window = GTK_WIDGET(gtk_builder_get_object(builder, WINDOW_BINGO));
    gtk_window_set_transient_for(GTK_WINDOW(widgets->w_err_window), GTK_WINDOW(window));
    gtk_window_set_transient_for(GTK_WINDOW(widgets->w_bingo_window), GTK_WINDOW(window));
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_position(GTK_WINDOW(widgets->w_err_window), GTK_WIN_POS_CENTER_ON_PARENT);
    gtk_window_set_position(GTK_WINDOW(widgets->w_bingo_window), GTK_WIN_POS_CENTER_ON_PARENT);

    //Get stack and child
    widgets->w_stack = GTK_STACK(gtk_builder_get_object(builder, STACK_CONTAINER));
    for(int i = 0; i < NUMBER_CONTAINER; i++) {
        widgets->w_container_list[i] = GTK_WIDGET(gtk_builder_get_object(builder, container_name[i]));
    }

    //Get entry
    widgets->w_entry_username = GTK_ENTRY(gtk_builder_get_object(builder, ENTRY_USER));
    widgets->w_entry_search_room = GTK_ENTRY(gtk_builder_get_object(builder, ENTRY_SEARCH_ROOM));

    //Get label
    widgets->w_lbl_username = GTK_LABEL(gtk_builder_get_object(builder, LBL_USER));
    widgets->w_lbl_err = GTK_LABEL(gtk_builder_get_object(builder, LBL_ERR));
    widgets->w_lbl_bingo = GTK_LABEL(gtk_builder_get_object(builder, LBL_BINGO));

    //Get toggle button
    for (int i = 0; i < TABLE_NUMBER; i++) {
        widgets->w_tog_btn_table[i] = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, tog_btn_table_name[i]));
        gtk_style_context_add_class(gtk_widget_get_style_context(GTK_WIDGET(widgets->w_tog_btn_table[i])), "tog_btn_table");
    }
    for (int i = 0; i < ROOM_PLAYER; i++) {
        widgets->w_tog_btn_player[i] = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, tog_btn_player_name[i]));
        gtk_style_context_add_class(gtk_widget_get_style_context(GTK_WIDGET(widgets->w_tog_btn_player[i])), "tog_btn_player");
    }

    widgets->currentWindow = 0;
    widgets->currUser = (char*)malloc(sizeof(char)*MAX_STRING);
    widgets->currUser = "\0";
    gtk_builder_connect_signals(builder, widgets);
    g_object_unref(builder);

    gtk_widget_show(window);
    gtk_main();
    g_slice_free(app_widgets, widgets);

    return 0;
}

void on_btn_play_clicked(GtkButton *button, app_widgets *app_wdgts) {
    if (strcmp(app_wdgts->currUser, "\0") != 0) {
        app_wdgts->currentWindow = app_wdgts->currentWindow + 1;
        gtk_stack_set_visible_child(app_wdgts->w_stack, app_wdgts->w_container_list[app_wdgts->currentWindow]);
        } else {
        gtk_label_set_text(app_wdgts->w_lbl_username, MES_NO_USER);
        showError(MES_NO_USER, app_wdgts->w_lbl_err, app_wdgts->w_err_window);
    }
    return;
}

void on_btn_details_clicked() {
    return;
}

void on_btn_enter_user_clicked(GtkButton *button, app_widgets *app_wdgts) {
    char username[MAX_STRING], tmp[MAX_STRING] = "\0";
    strcpy(username, gtk_entry_get_text(app_wdgts->w_entry_username));
    if (!checkString(username)) showError(MES_ERR_USER, app_wdgts->w_lbl_err, app_wdgts->w_err_window);
    else {
        strcat(tmp, MES_CUR_USER);
        strcat(tmp, username);
        gtk_label_set_text(app_wdgts->w_lbl_username, tmp);
        app_wdgts->currUser = strdup(username);
    }
    return;
}

void on_btn_back_clicked(GtkButton *button, app_widgets *app_wdgts) {
    if (app_wdgts->currentWindow == NUMBER_CONTAINER - 1) app_wdgts->currentWindow = app_wdgts->currentWindow - 2;
    else app_wdgts->currentWindow = app_wdgts->currentWindow - 1;
    gtk_stack_set_visible_child(app_wdgts->w_stack, app_wdgts->w_container_list[app_wdgts->currentWindow]);
}

void on_btn_ok_quit_clicked(GtkButton *button, app_widgets *app_wdgts) {
    gtk_widget_hide(app_wdgts->w_err_window);
}

void on_btn_bingo_ok_quit_clicked(GtkButton *button, app_widgets *app_wdgts) {
    gtk_widget_hide(app_wdgts->w_bingo_window);
}

void on_btn_create_room_clicked(GtkButton *button, app_widgets *app_wdgts) {
    app_wdgts->currentWindow = app_wdgts->currentWindow + 1;
    gtk_stack_set_visible_child(app_wdgts->w_stack, app_wdgts->w_container_list[app_wdgts->currentWindow]);
    return;
}

void on_btn_quick_join_clicked(GtkButton *button, app_widgets *app_wdgts) {
    app_wdgts->currentWindow = app_wdgts->currentWindow + 1;
    gtk_stack_set_visible_child(app_wdgts->w_stack, app_wdgts->w_container_list[app_wdgts->currentWindow]);
    return;
}

void on_btn_join_room_clicked(GtkButton *button, app_widgets *app_wdgts) {
    char id[MAX_STRING] = "\0";
    strcpy(id, gtk_entry_get_text(app_wdgts->w_entry_search_room));
    app_wdgts->currentWindow = app_wdgts->currentWindow + 1;
    gtk_stack_set_visible_child(app_wdgts->w_stack, app_wdgts->w_container_list[app_wdgts->currentWindow]);
    return;
}

void on_btn_start_clicked(GtkButton *button, app_widgets *app_wdgts) {
    app_wdgts->currentWindow = app_wdgts->currentWindow + 1;
    gtk_stack_set_visible_child(app_wdgts->w_stack, app_wdgts->w_container_list[app_wdgts->currentWindow]);
    return;
}

void on_btn_bingo_check_clicked(GtkButton *button, app_widgets *app_wdgts) {
    showBingo(MES_BINGO1, app_wdgts->w_lbl_bingo, app_wdgts->w_bingo_window);
}

void on_tog_btn_table_toggled(GtkToggleButton *toggle_button, app_widgets *app_wdgts) {
    printf("ok\n");
}

gboolean on_window_error_delete_event(GtkWidget *widget, GdkEvent *event, gpointer data) {
    gtk_widget_hide(widget);
    return TRUE;
}

gboolean on_window_bingo_delete_event(GtkWidget *widget, GdkEvent *event, gpointer data) {
    gtk_widget_hide(widget);
    return TRUE;
}

void on_window_main_destroy() {
    gtk_main_quit();
}

int showError(char* mesError, GtkLabel *lbl_err, GtkWidget *err_window) {
    gtk_label_set_text(GTK_LABEL(lbl_err), mesError);
    gtk_widget_show(err_window);
    return 1;
}

int showBingo(char* mesBingo, GtkLabel *lbl_bingo, GtkWidget *bingo_window) {
    gtk_label_set_text(GTK_LABEL(lbl_bingo), mesBingo);
    gtk_widget_show(bingo_window);
    return 1;
}

static void load_css(void) {
    GtkCssProvider *provider;
    GdkDisplay *display;
    GdkScreen *screen;

    GFile *css_fp = g_file_new_for_path(GLADE_CSS);
    GError *error = 0;

    provider = gtk_css_provider_new();
    display = gdk_display_get_default();
    screen = gdk_display_get_default_screen(display);

    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_SETTINGS);
    gtk_css_provider_load_from_file(provider, css_fp, &error);

    g_object_unref(provider);
}