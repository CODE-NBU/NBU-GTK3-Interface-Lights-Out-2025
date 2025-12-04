#include <gtk/gtk.h>

GtkWidget *window = NULL;
GtkWidget *buttons[5][5];

void toggleAdjacent(int x, int y) {
	if(x < 0) {
		return;
	}
	if(y < 0) {
		return;
	}
	if(x >= 5) {
		return;
	}
	if(y >= 5) {
		return;
	}
	
	const char *label = gtk_button_get_label(GTK_BUTTON(buttons[x][y]));
	
	if(strcmp(label, ".") == 0) {
		gtk_button_set_label(GTK_BUTTON(buttons[x][y]),"*");
	} else if(strcmp(label, "*") == 0) {
		gtk_button_set_label(GTK_BUTTON(buttons[x][y]),".");
	}
}

static void toggleLight(int x, int y) {
	toggleAdjacent(x, y);
	toggleAdjacent(x-1, y);
	toggleAdjacent(x, y-1);
	toggleAdjacent(x+1, y);
	toggleAdjacent(x, y+1);
}

static void shuffleGame() {
	for(int i=0; i<2500; i++) {
		int x = rand() % 5;
		int y = rand() % 5;
		toggleLight(x, y);
	}
}

static int isGameOver() {
	for(int j=0; j<5; j++) {
		for(int i=0; i<5; i++) {
			const char *label = gtk_button_get_label(GTK_BUTTON(buttons[i][j]));
			if(strcmp(label, "*") == 0) {
				return 0;
			}
		}
	}
	
	return 1;
}

static void on_button_clicked(GtkWidget *widget, gpointer data) {
	for(int j=0; j<5; j++) {
		for(int i=0; i<5; i++) {
			if(buttons[i][j] != widget) {
				continue;
			}

			toggleLight(i, j);
			
			if( isGameOver() ) {
				gtk_window_set_title(GTK_WINDOW(window), "Game over!");
			}
			
			return;
		}
	}
}

int main(int argc, char *argv[]) {
	gtk_init(&argc, &argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	
	gtk_window_set_default_size(GTK_WINDOW(window), 300, 300);
	gtk_window_set_title(GTK_WINDOW(window), "Lights Out");
	
	GtkWidget *grid = gtk_grid_new();
	gtk_container_add(GTK_CONTAINER(window), grid);
	
	for(int j=0; j<5; j++) {
		for(int i=0; i<5; i++) {
			buttons[i][j] = gtk_button_new_with_label(".");
			g_signal_connect(buttons[i][j], "clicked",
 G_CALLBACK(on_button_clicked), NULL);
			gtk_grid_attach(GTK_GRID(grid), buttons[i][j], i, j, 1, 1);
		}
	}
	
	shuffleGame();
	
	gtk_widget_show_all(window);
	gtk_main();
	
	return 0;
}

