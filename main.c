#include <gtk/gtk.h>

#include <stdio.h>
#include <locale.h>
#include <wchar.h>
#include <string.h>

GtkWidget *entry;
GtkWidget *label;

static char a[] = "QWERTYUIOP{}ASDFGHJKL:\"ZXCVBNM<>qwertyuiop[]asdfghjkl;'zxcvbnm,.&";
wchar_t c[] =    L"ЙЦУКЕНГШЩЗХЇФІВАПРОЛДЖЄЯЧСМИТЬБЮйцукенгшщзхїфівапролджєячсмитьбю?";

static gboolean entry_key_pressed(GtkWidget *widget, GdkEventKey *event, gpointer user_data);
static void button_clicked(GtkWidget *widget, gpointer user_data);
static void activate (GtkApplication* app, gpointer user_data);

int main(int argc, char **argv){
    setlocale(LC_ALL, "uk_UA.UTF-8");
    GtkApplication *app;
    int status;
    app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK (activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}

static gboolean entry_key_pressed(GtkWidget *widget, GdkEventKey *event, gpointer user_data) {
    if (event->keyval == GDK_KEY_Return || event->keyval == GDK_KEY_KP_Enter) {
        // Вызываем обработчик кнопки "Enter"
        button_clicked(NULL, user_data);
        return TRUE; // Отмечаем событие как обработанное
    }
    return FALSE; // Пропускаем другие клавиши
}

static void button_clicked(GtkWidget *widget, gpointer user_data) {
    const gchar *text = gtk_entry_get_text(GTK_ENTRY(entry));
    g_print("По-бородянськи: %s\n", text);

    // Преобразуем text в широкий формат
    wchar_t processed_text[254];
    memset(processed_text, 0, sizeof(processed_text));
    mbstowcs(processed_text, text, sizeof(processed_text));

    int size = wcslen(processed_text);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < strlen(a); j++) {
            if (processed_text[i] == a[j]) {
                processed_text[i] = c[j];
                break;
            }
        }
    }

    // Преобразуем processed_text в строку UTF-8
    char utf8_processed_text[254];
    wcstombs(utf8_processed_text, processed_text, sizeof(utf8_processed_text));
    g_print("Людською мовою: %s\n", utf8_processed_text);

    gtk_label_set_text(GTK_LABEL(label), utf8_processed_text);
}


static void activate (GtkApplication* app, gpointer user_data){
    GtkWidget *window;
    GtkWidget *button_box;
    GtkWidget *button;
    GtkWidget *box;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Borodyanskiy_surgyk");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), box);

    entry = gtk_entry_new();
    gtk_container_add(GTK_CONTAINER(box), entry);
    g_signal_connect(entry, "key-press-event", G_CALLBACK(entry_key_pressed), NULL);

    //button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    //gtk_container_add(GTK_CONTAINER(window), button_box);
    
    button = gtk_button_new_with_label("Перекласти цю хуйню");
    g_signal_connect(button, "clicked", G_CALLBACK(button_clicked), NULL);
    gtk_container_add(GTK_CONTAINER(box), button);

    label = gtk_label_new("");
    gtk_container_add(GTK_CONTAINER(box), label);

    gtk_widget_show_all(window);
}
