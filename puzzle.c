/*
    Author: Ondřej Šlampa, xslamp01@stud.fit.vutbr.cz
    Project: GUX proj2 - 8puzzle
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <gtk/gtk.h>

const int side_size=3;
const int number_of_stones=3*3;

char* number_strings[]={"0","1","2","3","4","5","6","7","8"};
int state[]={0,1,2,3,4,5,6,7,8};
GtkWidget* buttons[]={NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
GtkWidget* rows[]={NULL,NULL,NULL};

GtkWidget *window=NULL;

/*
    Returns true if stones are in winning combination, false otherwise.
*/
bool is_win(){
    for(int i=0;i<number_of_stones;i++){
        if(state[i]!=i){
            return false;
        }
    }
    return true;
}

/*
    Shuffles stones on board.
*/
void shuffle(){
    for (int i=0;i<number_of_stones-1;i++){
        size_t j=i+rand()/(RAND_MAX/(number_of_stones-i)+1);
        int t=state[j];
        state[j]=state[i];
        state[i]=t;
    }
    
    for(int i=0;i<number_of_stones;i++){
        gtk_button_set_label((GtkButton*)buttons[i], number_strings[state[i]]);
    }
    
    if(is_win()){
        shuffle();
    }
}

/*
    Returns index of button b in buttons array.
*/
int button_index(GtkWidget *b){
    for(int i=0;i<number_of_stones;i++){
        if(buttons[i]==b){
            return i;
        }
    }
    return -1;
}

/*
    Response to win dialog.
*/
static void popup_response(GtkDialog *dialog,
             gint       response_id,
             gpointer   user_data)
{
  gtk_widget_destroy(GTK_WIDGET(dialog));
}

/*
    Response to click on stone button.
*/
static void button_click(GtkWidget* widget, gpointer data){
    int index=button_index(widget);
    int empty_index=-1;
    
    for(int i=0;i<number_of_stones;i++){
        if(state[i]==0){
            empty_index=i;
            break;
        }
    }
    
    if(index==empty_index){
        return;
    }
    
    int diff=index-empty_index;
    int rem=index%side_size;
    if(!(diff==side_size || diff==-side_size || (rem!=0&&diff==1) || (rem!=side_size-1&&diff==-1))){
        return;
    }
    
    int tmp_int=state[index];
    state[index]=state[empty_index];
    state[empty_index]=tmp_int;
    gtk_button_set_label((GtkButton*)buttons[index], number_strings[state[index]]);
    gtk_button_set_label((GtkButton*)buttons[empty_index], number_strings[state[empty_index]]);
    
    if(is_win()){
        GtkWidget* popup=gtk_dialog_new_with_buttons("You won!",GTK_WINDOW(window),GTK_DIALOG_MODAL|GTK_DIALOG_DESTROY_WITH_PARENT,"OK",GTK_RESPONSE_OK,NULL);
        gtk_container_set_border_width(GTK_CONTAINER(popup), 10);
        gtk_window_set_resizable(GTK_WINDOW(popup), false);
        GtkWidget* content_area=gtk_dialog_get_content_area(GTK_DIALOG(popup));
        GtkWidget* label=gtk_label_new("Congratulations, you won!");
        gtk_container_add(GTK_CONTAINER(content_area), label);
        g_signal_connect(GTK_DIALOG(popup), "response", G_CALLBACK(popup_response), NULL);
        gtk_widget_show_all(popup);
    }
}

/*
    Response to click on stone.
*/
static void hello(GtkWidget *widget, gpointer data){
    shuffle();
}

/*
    Response to delete event on main window.
*/
static gboolean delete_event( GtkWidget *widget,
                              GdkEvent  *event,
                              gpointer   data)
{
    return FALSE;
}

/*
    Response to destroy signal on main window.
*/
static void destroy(GtkWidget *widget, gpointer data){
    gtk_main_quit();
}

/*
    Main function.
*/
int main(int argc, char *argv[]){
    gtk_init(&argc, &argv);
    
    //create main window
    window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "delete-event", G_CALLBACK(delete_event), NULL);
    g_signal_connect(window, "destroy", G_CALLBACK(destroy), NULL);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    
    GtkWidget* main_frame=gtk_frame_new(NULL);
    gtk_container_add(GTK_CONTAINER(window), main_frame);
    gtk_window_set_resizable(GTK_WINDOW(window), false);
    
    GtkWidget* main_box=gtk_vbox_new(FALSE, 1);
    gtk_container_add(GTK_CONTAINER(main_frame), main_box);
    
    GtkWidget* settings_box=gtk_hbox_new(FALSE, 1);
    gtk_container_add(GTK_CONTAINER(main_box), settings_box);
    
    GtkWidget *restart_button=gtk_button_new_with_label("      Shuffle      ");
    g_signal_connect(restart_button, "clicked", G_CALLBACK(hello), NULL);
    gtk_container_add(GTK_CONTAINER(settings_box), restart_button);
    
    //create stone box
    GtkWidget* stones_box=gtk_hbox_new(FALSE, 1);
    gtk_container_add(GTK_CONTAINER(main_box), stones_box);
    
    //create stones
    for(int i=0;i<side_size;i++){
        GtkWidget* row_box=gtk_vbox_new(FALSE, 1);
        for(int j=0;j<side_size;j++){
            int n=j*side_size+i;
            GtkWidget* stone=gtk_button_new_with_label(number_strings[n]);
            g_signal_connect(stone, "clicked", G_CALLBACK(button_click), NULL);
            gtk_container_add(GTK_CONTAINER(row_box), stone);
            buttons[n]=stone;
        }
        gtk_container_add(GTK_CONTAINER(stones_box), row_box);
        rows[i]=row_box;
    }
    
    //create menu bar, menus and menu items
    GtkWidget* menu_bar=gtk_menu_bar_new();
    GtkWidget* file_menu=gtk_menu_new();
    
    GtkWidget* file_mi=gtk_menu_item_new_with_mnemonic("_File");
    GtkWidget* shuffle_mi=gtk_menu_item_new_with_mnemonic("_Shuffle");
    GtkWidget* quit_mi=gtk_menu_item_new_with_mnemonic("_Quit");
    
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), shuffle_mi);
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), quit_mi);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_mi), file_menu);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), file_mi);
    gtk_box_pack_start(GTK_BOX(main_box), menu_bar, FALSE, FALSE, 0);
    
    //set menu responses to signals
    g_signal_connect(G_OBJECT(shuffle_mi), "activate", G_CALLBACK(hello), NULL);
    g_signal_connect(G_OBJECT(quit_mi), "activate", G_CALLBACK(gtk_main_quit), NULL);
    
    //show window and start main gtk program
    gtk_widget_show_all(window);
    gtk_main();
    
    return EXIT_SUCCESS;
}
