#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../../protocol.h"
#include "../../helper.h"
#include "../../client_handle.h"
#include <pthread.h>
#define BUFF_SIZE 255

#define MAX_STRING 256
#define NUMBER_CONTAINER 4
#define TABLE_NUMBER 25
#define ROOM_PLAYER 5
#define GAME_RULE_FILE "Client/res/GameRule.txt"
#define ABOUT_FILE "Client/res/AboutUs.txt"
#define GLADE_FILE_NAME "Client/glade/test.glade"
#define GLADE_CSS "Client/style.css"
//ID Window
#define WINDOW_NAME "window_main"
#define WINDOW_ERR "window_error"
#define WINDOW_BINGO "window_bingo"
#define WINDOW_INVITE "window_invite"
#define WINDOW_LOG "container_menu_log"
#define WINDOW_REG "container_menu_reg"
#define WINDOW_READER "window_reader"
//ID Stack
#define STACK_CONTAINER "stack_container"
#define STACK_MENU "stack_menu"
#define STACK_ROOM "stack_room"
#define STACK_PLAYING "stack_playing"
//ID Label
#define LBL_MENU_LOG "lbl_menu_log"
#define LBL_MENU_REG "lbl_menu_reg"
#define LBL_ERR "lbl_error"
#define LBL_BINGO "lbl_bingo"
#define LBL_INVITE "lbl_invite"
#define LBL_MENU_TITLE "lbl_menu_title"
//ID Entry
#define ENTRY_MENU_LOG_USER "entry_menu_log_user"
#define ENTRY_LOBBY_SEARCH "entry_lobby_search"
#define ENTRY_ROOM_SEARCH "entry_room_search"
#define ENTRY_MENU_LOG_PAS "entry_menu_log_pas"
#define ENTRY_MENU_REG_USER "entry_menu_reg_user"
#define ENTRY_MENU_REG_PAS "entry_menu_reg_pas"
#define ENTRY_MENU_REG_CON_PAS "entry_menu_reg_con_ pas"
//ID Button
#define BTN_ROOM_START "btn_room_start"
#define BTN_ROOM_READY "btn_room_ready"
#define BTN_ROOM_KICK "btn_room_kick"
#define BTN_ROOM_INVITE "btn_room_invite"
#define BTN_PLAYING_QUIT "btn_playing_quit"
#define BTN_PLAYING_BACK "btn_playing_back"

//ID Clickable Button
#define BTN_MENU_REG "btn_menu_log_reg"
#define BTN_MENU_LOG "btn_menu_reg_log"
#define BTN_MENU_DETAILS "btn_menu_details"

//ID Text Buffer
#define TXT_BUF "txt_buf"
#define MES_ERR_USER "Your user is error!"
#define MES_NO_KICK "Pick someone to kick!"
#define MES_BINGO1 "!!! YOU WON !!!"
#define MES_BINGO2 "Won!"
#define MES_INVITE "You have been invited to a game!"
#define READY "Ready"
#define NOT_READY "Not Ready"

typedef struct
{
    int currentWindow;
    char *currUser;
    int serverfd;
    GtkEntry *w_entry_menu_log_user, *w_entry_lobby_search, *w_entry_room_user, *w_entry_menu_log_pas, *w_entry_menu_reg_user, *w_entry_menu_reg_pas, *w_entry_menu_reg_con_pas;
    GtkLabel *w_lbl_err, *w_lbl_bingo, *w_lbl_invite;
    GtkStack *w_stack_container, *w_stack_room, *w_stack_playing, *w_stack_menu;
    GtkWidget *w_container_list[NUMBER_CONTAINER], *w_read_window, *w_err_window, *w_bingo_window, *w_invite_window, *w_container_menu_log, *w_container_menu_reg;
    GtkToggleButton *w_tog_btn_table[TABLE_NUMBER], *w_tog_btn_player[ROOM_PLAYER];
    GtkButton *w_btn_room_start, *w_btn_room_ready, *w_btn_room_kick, *w_btn_room_invite, *w_btn_playing_quit, *w_btn_playing_back;
    GtkTextBuffer *w_txt_buf;
} app_widgets;

char container_name[NUMBER_CONTAINER][MAX_STRING] = {"container_menu", "container_lobby", "container_room", "container_playing"};
char tog_btn_table_name[TABLE_NUMBER][MAX_STRING] = {"tog_btn_table_0", "tog_btn_table_1", "tog_btn_table_2", "tog_btn_table_3", "tog_btn_table_4", "tog_btn_table_5", "tog_btn_table_6", "tog_btn_table_7", "tog_btn_table_8", "tog_btn_table_9", "tog_btn_table_10", "tog_btn_table_11", "tog_btn_table_12", "tog_btn_table_13", "tog_btn_table_14", "tog_btn_table_15", "tog_btn_table_16", "tog_btn_table_17", "tog_btn_table_18", "tog_btn_table_19", "tog_btn_table_20", "tog_btn_table_21", "tog_btn_table_22", "tog_btn_table_23", "tog_btn_table_24"};
char tog_btn_player_name[ROOM_PLAYER][MAX_STRING] = {
    "tog_btn_player0",
    "tog_btn_player1",
    "tog_btn_player2",
    "tog_btn_player3",
    "tog_btn_player4",
};
char btn_all_name[][MAX_STRING] = {"btn_bingo_ok", "btn_error_ok", "btn_menu_login", "btn_lobby_log_out", "btn_lobby_create", "btn_lobby_quick_join", "btn_lobby_join", "btn_room_back", "btn_playing_bingo", "btn_invite_accept", "btn_invite_decline", BTN_ROOM_START, BTN_ROOM_READY, BTN_ROOM_KICK, BTN_ROOM_INVITE, BTN_PLAYING_QUIT, BTN_PLAYING_BACK, "btn_menu_reg"};
char entry_all_name[][MAX_STRING] = {ENTRY_MENU_LOG_USER, ENTRY_LOBBY_SEARCH, ENTRY_ROOM_SEARCH, ENTRY_MENU_LOG_PAS, ENTRY_MENU_REG_PAS, ENTRY_MENU_REG_USER, ENTRY_MENU_REG_CON_PAS};
char label_all_name[][MAX_STRING] = {"lbl_menu_log_user", "lbl_menu_log_pas", "lbl_menu_reg_user", "lbl_menu_reg_pas", "lbl_menu_reg_con_pas", "lbl_room_id", "lbl_menu_reg", "lbl_playing_info", LBL_ERR, LBL_BINGO, LBL_INVITE};
char window_all_name[][MAX_STRING] = {WINDOW_READER, WINDOW_INVITE, WINDOW_NAME, WINDOW_BINGO, WINDOW_ERR, WINDOW_LOG, WINDOW_REG};

GtkBuilder *builder;
GtkWidget *window;
app_widgets *widgets;
int number_button = sizeof(btn_all_name) / (sizeof(char) * MAX_STRING),
    number_entry = sizeof(entry_all_name) / (sizeof(char) * MAX_STRING),
    number_lable = sizeof(label_all_name) / (sizeof(char) * MAX_STRING),
    number_window = sizeof(window_all_name) / (sizeof(char) * MAX_STRING);

static void load_css(void);
void *recv_handler(void *void_sockfd);
int showError(char *mesError, GtkLabel *lbl_err, GtkWidget *err_window);
int showBingo(char *mesBingo, GtkLabel *lbl_bingo, GtkWidget *bingo_window);
int showInvite(char *mesInvite, GtkLabel *lbl_invite, GtkWidget *invite_window);

int clientGUI(int serverfd)
{
    gtk_init(NULL, NULL);
    load_css();
    builder = gtk_builder_new_from_file(GLADE_FILE_NAME);
    widgets = g_slice_new(app_widgets);
    //Get window
    window = GTK_WIDGET(gtk_builder_get_object(builder, WINDOW_NAME));
    widgets->w_err_window = GTK_WIDGET(gtk_builder_get_object(builder, WINDOW_ERR));
    widgets->w_bingo_window = GTK_WIDGET(gtk_builder_get_object(builder, WINDOW_BINGO));
    widgets->w_invite_window = GTK_WIDGET(gtk_builder_get_object(builder, WINDOW_INVITE));
    widgets->w_container_menu_log = GTK_WIDGET(gtk_builder_get_object(builder, WINDOW_LOG));
    widgets->w_container_menu_reg = GTK_WIDGET(gtk_builder_get_object(builder, WINDOW_REG));
    widgets->w_read_window = GTK_WIDGET(gtk_builder_get_object(builder, WINDOW_READER));
    widgets->serverfd = serverfd; //chuyen cong ket noi vao ben gtk
    gtk_window_set_transient_for(GTK_WINDOW(widgets->w_err_window), GTK_WINDOW(window));
    gtk_window_set_transient_for(GTK_WINDOW(widgets->w_bingo_window), GTK_WINDOW(window));
    gtk_window_set_transient_for(GTK_WINDOW(widgets->w_invite_window), GTK_WINDOW(window));
    gtk_window_set_transient_for(GTK_WINDOW(widgets->w_read_window), GTK_WINDOW(window));
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_position(GTK_WINDOW(widgets->w_err_window), GTK_WIN_POS_CENTER_ON_PARENT);
    gtk_window_set_position(GTK_WINDOW(widgets->w_bingo_window), GTK_WIN_POS_CENTER_ON_PARENT);
    gtk_window_set_position(GTK_WINDOW(widgets->w_invite_window), GTK_WIN_POS_CENTER_ON_PARENT);
    gtk_window_set_position(GTK_WINDOW(widgets->w_read_window), GTK_WIN_POS_CENTER_ON_PARENT);
    for (int i = 0; i < number_window; i++)
        gtk_style_context_add_class(gtk_widget_get_style_context(GTK_WIDGET(gtk_builder_get_object(builder, window_all_name[i]))), "window");

    //Get stack
    widgets->w_stack_container = GTK_STACK(gtk_builder_get_object(builder, STACK_CONTAINER));
    widgets->w_stack_room = GTK_STACK(gtk_builder_get_object(builder, STACK_ROOM));
    widgets->w_stack_playing = GTK_STACK(gtk_builder_get_object(builder, STACK_PLAYING));
    widgets->w_stack_menu = GTK_STACK(gtk_builder_get_object(builder, STACK_MENU));

    //Get container
    for (int i = 0; i < NUMBER_CONTAINER; i++)
    {
        widgets->w_container_list[i] = GTK_WIDGET(gtk_builder_get_object(builder, container_name[i]));
    }

    //Get entry
    widgets->w_entry_menu_log_user = GTK_ENTRY(gtk_builder_get_object(builder, ENTRY_MENU_LOG_USER));
    widgets->w_entry_lobby_search = GTK_ENTRY(gtk_builder_get_object(builder, ENTRY_LOBBY_SEARCH));
    widgets->w_entry_room_user = GTK_ENTRY(gtk_builder_get_object(builder, ENTRY_ROOM_SEARCH));
    widgets->w_entry_menu_log_pas = GTK_ENTRY(gtk_builder_get_object(builder, ENTRY_MENU_LOG_PAS));
    widgets->w_entry_menu_reg_user = GTK_ENTRY(gtk_builder_get_object(builder, ENTRY_MENU_REG_USER));
    widgets->w_entry_menu_reg_pas = GTK_ENTRY(gtk_builder_get_object(builder, ENTRY_MENU_REG_PAS));
    widgets->w_entry_menu_reg_con_pas = GTK_ENTRY(gtk_builder_get_object(builder, ENTRY_MENU_REG_CON_PAS));
    for (int i = 0; i < number_entry; i++)
        gtk_style_context_add_class(gtk_widget_get_style_context(GTK_WIDGET(gtk_builder_get_object(builder, entry_all_name[i]))), "entry");

    //Get label
    widgets->w_lbl_err = GTK_LABEL(gtk_builder_get_object(builder, LBL_ERR));
    widgets->w_lbl_bingo = GTK_LABEL(gtk_builder_get_object(builder, LBL_BINGO));
    widgets->w_lbl_invite = GTK_LABEL(gtk_builder_get_object(builder, LBL_INVITE));
    gtk_style_context_add_class(gtk_widget_get_style_context(GTK_WIDGET(gtk_builder_get_object(builder, LBL_MENU_LOG))), "title2");
    gtk_style_context_add_class(gtk_widget_get_style_context(GTK_WIDGET(gtk_builder_get_object(builder, LBL_MENU_REG))), "title2");
    gtk_style_context_add_class(gtk_widget_get_style_context(GTK_WIDGET(gtk_builder_get_object(builder, LBL_MENU_TITLE))), "title1");
    for (int i = 0; i < number_lable; i++)
        gtk_style_context_add_class(gtk_widget_get_style_context(GTK_WIDGET(gtk_builder_get_object(builder, label_all_name[i]))), "label");

    //Get toggle button
    for (int i = 0; i < TABLE_NUMBER; i++)
    {
        widgets->w_tog_btn_table[i] = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, tog_btn_table_name[i]));
        gtk_style_context_add_class(gtk_widget_get_style_context(GTK_WIDGET(widgets->w_tog_btn_table[i])), "tog_btn_table");
    }
    for (int i = 0; i < ROOM_PLAYER; i++)
    {
        widgets->w_tog_btn_player[i] = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, tog_btn_player_name[i]));
        gtk_style_context_add_class(gtk_widget_get_style_context(GTK_WIDGET(widgets->w_tog_btn_player[i])), "tog_btn_player");
    }

    //Get button
    widgets->w_btn_room_ready = GTK_BUTTON(gtk_builder_get_object(builder, BTN_ROOM_READY));
    gtk_button_set_label(widgets->w_btn_room_ready, READY);
    widgets->w_btn_room_start = GTK_BUTTON(gtk_builder_get_object(builder, BTN_ROOM_START));
    widgets->w_btn_room_kick = GTK_BUTTON(gtk_builder_get_object(builder, BTN_ROOM_KICK));
    widgets->w_btn_room_invite = GTK_BUTTON(gtk_builder_get_object(builder, BTN_ROOM_INVITE));
    widgets->w_btn_playing_back = GTK_BUTTON(gtk_builder_get_object(builder, BTN_PLAYING_BACK));
    widgets->w_btn_playing_quit = GTK_BUTTON(gtk_builder_get_object(builder, BTN_PLAYING_QUIT));
    for (int i = 0; i < number_button; i++)
        gtk_style_context_add_class(gtk_widget_get_style_context(GTK_WIDGET(gtk_builder_get_object(builder, btn_all_name[i]))), "button");

    //Get clickable button
    gtk_style_context_add_class(gtk_widget_get_style_context(GTK_WIDGET(gtk_builder_get_object(builder, BTN_MENU_REG))), "cb");
    gtk_style_context_add_class(gtk_widget_get_style_context(GTK_WIDGET(gtk_builder_get_object(builder, BTN_MENU_LOG))), "cb");
    gtk_style_context_add_class(gtk_widget_get_style_context(GTK_WIDGET(gtk_builder_get_object(builder, BTN_MENU_DETAILS))), "cb");

    //Get text buffer
    widgets->w_txt_buf = GTK_TEXT_BUFFER(gtk_builder_get_object(builder, TXT_BUF));

    widgets->currentWindow = 0;
    widgets->currUser = (char *)malloc(sizeof(char) * MAX_STRING);
    widgets->currUser = "\0";
    gtk_builder_connect_signals(builder, widgets);
    g_object_unref(builder);
    pthread_t tid;
    pthread_create(&tid, NULL, &recv_handler, (void *)&serverfd);

    gtk_widget_show(window);
    gtk_main();
    g_slice_free(app_widgets, widgets);

    return 0;
}

void on_btn_menu_login_clicked(GtkButton *button, app_widgets *app_wdgts)
{
    // Request *req = (Request *)malloc(sizeof(Request));
    // Response *res = (Response *)malloc(sizeof(Response));
    char username[MAX_STRING], password[MAX_STRING];
    strcpy(username, gtk_entry_get_text(app_wdgts->w_entry_menu_log_user));
    strcpy(password, gtk_entry_get_text(app_wdgts->w_entry_menu_log_pas));
    // memset(sendbuff, '\0', BUFF_SIZE); //clear buffer
    // memset(res->data, '\0', BUFF_SIZE); //clear buff in res->data
    // memset(res->message, '\0', BUFF_SIZE); //clear buff in res->message
    login(app_wdgts->serverfd, username, password);
    // int check = login(app_wdgts->serverfd, req, res);
    // if (check < 0 || check == 4 || check == 7 || check == 8)
    //     showError(res->message, app_wdgts->w_lbl_err, app_wdgts->w_err_window);
    // else {
    //     app_wdgts->currUser = strdup(username);
    //     app_wdgts->currentWindow = app_wdgts->currentWindow + 1;
    //     gtk_stack_set_visible_child(app_wdgts->w_stack_container, app_wdgts->w_container_list[app_wdgts->currentWindow]);
    // }
    return;
}

void on_btn_menu_reg_clicked(GtkButton *button, app_widgets *app_wdgts)
{
    Request *req = (Request *)malloc(sizeof(Request));
    Response *res = (Response *)malloc(sizeof(Response));
    char username[MAX_STRING], password[MAX_STRING], con_pas[MAX_STRING];
    strcpy(username, gtk_entry_get_text(app_wdgts->w_entry_menu_reg_user));
    strcpy(password, gtk_entry_get_text(app_wdgts->w_entry_menu_reg_pas));
    strcpy(con_pas, gtk_entry_get_text(app_wdgts->w_entry_menu_reg_con_pas));
    createRegisterRequest("REGISTER", username, password, con_pas, req);
    int check = registerAccount(app_wdgts->serverfd, req, res);
    if (check < 0 || check == 1 || check == 2) //Wrong input
        showError(res->message, app_wdgts->w_lbl_err, app_wdgts->w_err_window);
    else
    {
        app_wdgts->currUser = strdup(username);
        app_wdgts->currentWindow = app_wdgts->currentWindow + 1;
        gtk_stack_set_visible_child(app_wdgts->w_stack_container, app_wdgts->w_container_list[app_wdgts->currentWindow]);
    }
    return;
}

void on_btn_menu_log_reg_clicked(GtkButton *button, app_widgets *app_wdgts)
{
    gtk_stack_set_visible_child(app_wdgts->w_stack_menu, app_wdgts->w_container_menu_reg);
    return;
}

void on_btn_menu_reg_log_clicked(GtkButton *button, app_widgets *app_wdgts)
{
    gtk_stack_set_visible_child(app_wdgts->w_stack_menu, app_wdgts->w_container_menu_log);
    return;
}

void on_btn_menu_details_clicked(GtkButton *button, app_widgets *app_wdgts)
{
    gchar *file_contents = NULL;
    gboolean file_success = FALSE;

    gtk_widget_show(app_wdgts->w_read_window);
    file_success = g_file_get_contents(GAME_RULE_FILE, &file_contents, NULL, NULL);
    if (file_success)
        gtk_text_buffer_set_text(app_wdgts->w_txt_buf, file_contents, -1);
    g_free(file_contents);
    return;
}

void on_btn_reader_details_clicked(GtkButton *button, app_widgets *app_wdgts)
{
    // Request *req = (Request *)malloc(sizeof(Request));
    // Response *res = (Response *)malloc(sizeof(Response));
    gchar *file_contents = NULL;
    gboolean file_success = FALSE;

    file_success = g_file_get_contents(GAME_RULE_FILE, &file_contents, NULL, NULL);
    if (file_success)
        gtk_text_buffer_set_text(app_wdgts->w_txt_buf, file_contents, -1);
    g_free(file_contents);
    return;
}

void on_btn_reader_about_clicked(GtkButton *button, app_widgets *app_wdgts)
{
    gchar *file_contents = NULL;
    gboolean file_success = FALSE;

    file_success = g_file_get_contents(ABOUT_FILE, &file_contents, NULL, NULL);
    if (file_success)
        gtk_text_buffer_set_text(app_wdgts->w_txt_buf, file_contents, -1);
    g_free(file_contents);
    return;
}

void on_btn_back_clicked(GtkButton *button, app_widgets *app_wdgts)
{
    app_wdgts->currentWindow = app_wdgts->currentWindow - 1;
    gtk_stack_set_visible_child(app_wdgts->w_stack_container, app_wdgts->w_container_list[app_wdgts->currentWindow]);
}

void on_btn_error_ok_clicked(GtkButton *button, app_widgets *app_wdgts)
{
    gtk_widget_hide(app_wdgts->w_err_window);
}

void on_btn_bingo_ok_clicked(GtkButton *button, app_widgets *app_wdgts)
{
    gtk_widget_hide(app_wdgts->w_bingo_window);
}

void on_btn_invite_accept_clicked(GtkButton *button, app_widgets *app_wdgts)
{
    gtk_widget_hide(app_wdgts->w_invite_window);
}

void on_btn_invite_decline_clicked(GtkButton *button, app_widgets *app_wdgts)
{
    gtk_widget_hide(app_wdgts->w_invite_window);
}

void on_btn_lobby_log_out_clicked(GtkButton *button, app_widgets *app_wdgts)
{
    Request *req = (Request *)malloc(sizeof(Request));
    Response *res = (Response *)malloc(sizeof(Response));
    createLogOutRequest("LOGOUT", req);
    logOut(app_wdgts->serverfd, req, res);
    app_wdgts->currentWindow = app_wdgts->currentWindow - 1;
    gtk_stack_set_visible_child(app_wdgts->w_stack_container, app_wdgts->w_container_list[app_wdgts->currentWindow]);
    gtk_stack_set_visible_child(app_wdgts->w_stack_menu, app_wdgts->w_container_menu_log);
    return;
}

void on_btn_lobby_create_clicked(GtkButton *button, app_widgets *app_wdgts)
{
    app_wdgts->currentWindow = app_wdgts->currentWindow + 1;
    gtk_stack_set_visible_child(app_wdgts->w_stack_container, app_wdgts->w_container_list[app_wdgts->currentWindow]);
    gtk_stack_set_visible_child(app_wdgts->w_stack_room, GTK_WIDGET(app_wdgts->w_btn_room_start));
    gtk_widget_set_visible(GTK_WIDGET(app_wdgts->w_btn_room_kick), TRUE);
    gtk_widget_set_visible(GTK_WIDGET(app_wdgts->w_btn_room_invite), TRUE);
    gtk_widget_set_visible(GTK_WIDGET(app_wdgts->w_entry_room_user), TRUE);
    return;
}

void on_btn_lobby_quick_join_clicked(GtkButton *button, app_widgets *app_wdgts)
{
    app_wdgts->currentWindow = app_wdgts->currentWindow + 1;
    gtk_stack_set_visible_child(app_wdgts->w_stack_container, app_wdgts->w_container_list[app_wdgts->currentWindow]);
    gtk_stack_set_visible_child(app_wdgts->w_stack_room, GTK_WIDGET(app_wdgts->w_btn_room_ready));
    gtk_widget_set_visible(GTK_WIDGET(app_wdgts->w_btn_room_kick), FALSE);
    gtk_widget_set_visible(GTK_WIDGET(app_wdgts->w_btn_room_invite), FALSE);
    gtk_widget_set_visible(GTK_WIDGET(app_wdgts->w_entry_room_user), FALSE);
    return;
}

void on_btn_lobby_join_clicked(GtkButton *button, app_widgets *app_wdgts)
{
    char id[MAX_STRING] = "\0";
    strcpy(id, gtk_entry_get_text(app_wdgts->w_entry_lobby_search));
    app_wdgts->currentWindow = app_wdgts->currentWindow + 1;
    gtk_stack_set_visible_child(app_wdgts->w_stack_container, app_wdgts->w_container_list[app_wdgts->currentWindow]);
    gtk_stack_set_visible_child(app_wdgts->w_stack_room, GTK_WIDGET(app_wdgts->w_btn_room_ready));
    gtk_widget_set_visible(GTK_WIDGET(app_wdgts->w_btn_room_kick), FALSE);
    gtk_widget_set_visible(GTK_WIDGET(app_wdgts->w_btn_room_invite), FALSE);
    gtk_widget_set_visible(GTK_WIDGET(app_wdgts->w_entry_room_user), FALSE);
    return;
}

void on_btn_room_kick_clicked(GtkButton *button, app_widgets *app_wdgts)
{
    gboolean check = FALSE;
    char tmp[MAX_STRING];
    for (int i = 0; i < ROOM_PLAYER; i++)
    {
        if (gtk_toggle_button_get_active(app_wdgts->w_tog_btn_player[i]))
        {
            check = TRUE;
            strcpy(tmp, gtk_button_get_label(GTK_BUTTON(app_wdgts->w_tog_btn_player[i])));
            // GOT USERNAME AND KICK
            gtk_button_set_label(GTK_BUTTON(app_wdgts->w_tog_btn_player[i]), "Empty");
        }
        gtk_toggle_button_set_active(app_wdgts->w_tog_btn_player[i], FALSE);
    }
    if (check == FALSE)
        showError(MES_NO_KICK, app_wdgts->w_lbl_err, app_wdgts->w_err_window);
    return;
}

void on_btn_room_invite_clicked(GtkButton *button, app_widgets *app_wdgts)
{
    showInvite(MES_INVITE, app_wdgts->w_lbl_invite, app_wdgts->w_invite_window);
}

void on_btn_room_ready_clicked(GtkButton *button, app_widgets *app_wdgts)
{
    char tmp[MAX_STRING] = "\0";
    strcpy(tmp, gtk_button_get_label(app_wdgts->w_btn_room_ready));
    if (strcmp(tmp, READY) == 0)
    {
        gtk_button_set_label(app_wdgts->w_btn_room_ready, NOT_READY);
    }
    else
    {
        gtk_button_set_label(app_wdgts->w_btn_room_ready, READY);
    }
}

void on_btn_room_start_clicked(GtkButton *button, app_widgets *app_wdgts)
{
    app_wdgts->currentWindow = app_wdgts->currentWindow + 1;
    gtk_stack_set_visible_child(app_wdgts->w_stack_container, app_wdgts->w_container_list[app_wdgts->currentWindow]);
    gtk_stack_set_visible_child(app_wdgts->w_stack_playing, GTK_WIDGET(app_wdgts->w_btn_playing_quit));
    return;
}

void on_btn_playing_bingo_clicked(GtkButton *button, app_widgets *app_wdgts)
{
    showBingo(MES_BINGO1, app_wdgts->w_lbl_bingo, app_wdgts->w_bingo_window);
    gtk_stack_set_visible_child(app_wdgts->w_stack_playing, GTK_WIDGET(app_wdgts->w_btn_playing_back));
}

void on_btn_playing_quit_clicked(GtkButton *button, app_widgets *app_wdgts)
{
    app_wdgts->currentWindow = app_wdgts->currentWindow - 2;
    gtk_stack_set_visible_child(app_wdgts->w_stack_container, app_wdgts->w_container_list[app_wdgts->currentWindow]);
}

void on_tog_btn_table_toggled(GtkToggleButton *toggle_button, app_widgets *app_wdgts)
{
    if (!gtk_toggle_button_get_active(toggle_button))
        gtk_toggle_button_set_active(toggle_button, TRUE);
}

gboolean on_window_error_delete_event(GtkWidget *widget, GdkEvent *event, gpointer data)
{
    gtk_widget_hide(widget);
    return TRUE;
}

gboolean on_window_bingo_delete_event(GtkWidget *widget, GdkEvent *event, gpointer data)
{
    gtk_widget_hide(widget);
    return TRUE;
}

gboolean on_window_invite_delete_event(GtkWidget *widget, GdkEvent *event, gpointer data)
{
    gtk_widget_hide(widget);
    return TRUE;
}

gboolean on_window_reader_delete_event(GtkWidget *widget, GdkEvent *event, gpointer data)
{
    gtk_widget_hide(widget);
    return TRUE;
}

void on_window_main_destroy()
{
    gtk_main_quit();
}

int showError(char *mesError, GtkLabel *lbl_err, GtkWidget *err_window)
{
    gtk_label_set_text(GTK_LABEL(lbl_err), mesError);
    gtk_widget_show(err_window);
    return 1;
}

int showBingo(char *mesBingo, GtkLabel *lbl_bingo, GtkWidget *bingo_window)
{
    gtk_label_set_text(GTK_LABEL(lbl_bingo), mesBingo);
    gtk_widget_show(bingo_window);
    return 1;
}

int showInvite(char *mesInvite, GtkLabel *lbl_invite, GtkWidget *invite_window)
{
    gtk_label_set_text(GTK_LABEL(lbl_invite), mesInvite);
    gtk_widget_show(invite_window);
    return 1;
}

static void load_css(void)
{
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

void *recv_handler(void *void_sockfd)
{
    int serverfd;
    int rcvBytes;
    Response *res = (Response *)malloc(sizeof(Response));
    pthread_detach(pthread_self());
    serverfd = *(int *)void_sockfd;

    while (1)
    {
        rcvBytes = recvRes(serverfd, res, sizeof(Response), 0);
        if (rcvBytes < 0)
        {
            perror("\nError: ");
            break;
        }
        switch (res->code)
        {
        case LOGIN_SUCCESS:
            // app_wdgts->currUser = strdup(username);
            widgets->currentWindow = widgets->currentWindow + 1;
            gtk_stack_set_visible_child(widgets->w_stack_container, widgets->w_container_list[widgets->currentWindow]);
            break;

            default:
            break;
        }
    }
    close(serverfd);
}