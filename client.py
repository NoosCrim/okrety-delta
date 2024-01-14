import platform
import ipaddress
import subprocess
import tkinter as tk
from tkinter import messagebox, Label, Entry, simpledialog

window_width = 960
window_height = 540
ip_address = "0.0.0.0"
port = "2137"


def launch():
    # print(launch_arguments)
    try:
        if platform.system() == "Windows":
            # messagebox.showinfo("Windows!", "Uruchamiam grę na systemie Windows.")
            p = subprocess.run(["okrety-delta.exe"] + launch_arguments, shell=True, check=False)
            returnProcess = p.returncode
            if returnProcess == 1:
                messagebox.showerror("Błąd", "Złe wejście")
            elif returnProcess == 2:
                messagebox.showerror("Błąd", "Nie udało się połączyć")

        else:
            # messagebox.showinfo("Linux!", "Uruchamiam grę na systemie Linux.")
            p = subprocess.run(["./okrety-delta"] + launch_arguments, shell=True, check=False)
            returnProcess = p.returncode
            if returnProcess == 1:
                messagebox.showerror("Błąd", "Złe wejście")
            elif returnProcess == 2:
                messagebox.showerror("Błąd", "Nie udało się połączyć")

    except subprocess.CalledProcessError as e:
        messagebox.showerror("Błąd", f"Wystąpił błąd podczas uruchamiania gry: {e}")


def show_help():
    messagebox.showinfo("Pomoc", "Gra w statki to klasyczna rozgrywka strategiczna, w której gracze rozmieszczają "
                                 "swoje statki na planszy i starają się trafić statki przeciwnika przez strategiczne "
                                 "i logiczne ruchy podzielone na tury.\n\nLauncher gry oferuje kilka opcji, umożliwiając"
                                 " dostosowanie doświadczenia rozgrywki. Możesz zmieniać rozmiar okna gry, przełączać "
                                 "między trybem pełnoekranowym a oknem, a także dostosowywać inne ustawienia, takie "
                                 "jak adres IP hosta dla gry wieloosobowej. Dostępne jest również zapoznanie się z "
                                 "elementami wizualnymi gry przed rozpoczęciem rozgrywki.")


def toggle_fullscreen(arguments):
    # Toggle pełnego ekranu
    if root.attributes("-fullscreen"):
        arguments.remove("-Fs")
    else:
        arguments.append("-Fs")

    root.attributes("-fullscreen", not root.attributes("-fullscreen"))


def toggle_option_buttons():
    # Ukryj/przywróć przyciski uruchamiania i pomocy
    start_button.pack_forget()
    help_button.pack_forget()
    options_button.pack_forget()
    exit_button.pack_forget()

    option_back_button.pack(side=tk.LEFT, padx=25)

    fullscreen_frame.pack(side=tk.LEFT, padx=25)


def toggle_help_buttons():
    # Ukryj/przywróć przyciski uruchamiania i pomocy
    start_button.pack_forget()
    help_button.pack_forget()
    options_button.pack_forget()
    exit_button.pack_forget()

    help_frame.pack(side=tk.BOTTOM, pady=25)
    gui_img_frame.pack(side=tk.LEFT, padx=25)
    gui_desc_frame.pack(side=tk.LEFT, padx=0)


def back_to_menu():
    # Przywróć przyciski menu głównego, ukryj gui pomocy/opcji
    option_back_button.pack_forget()
    fullscreen_frame.pack_forget()

    help_frame.pack_forget()
    gui_img_frame.pack_forget()
    gui_desc_frame.pack_forget()

    start_button.pack(pady=20)
    options_button.pack(pady=20)
    help_button.pack(pady=20)
    exit_button.pack(pady=20)


def set_background():
    label1.config(image=bg)
    label1.place(x=-5, y=-5)
    label1.config(width=2560, height=1440)


def change_button_color_hex(button, hex_color):
    button.config(bg=hex_color)


def update_dimensions():
    # Funkcja wywoływana po wciśnięciu przycisku "Aktualizuj"
    global window_width, window_height
    launch_arguments.remove("-Ws")
    launch_arguments.remove(str(window_width))
    launch_arguments.remove(str(window_height))
    launch_arguments.append("-Ws")
    window_width = int(width_var.get())
    window_height = int(height_var.get())
    launch_arguments.append(width_var.get())
    launch_arguments.append(height_var.get())
    print(launch_arguments)


def get_ip_address():
    global ip_address
    launch_arguments.remove("-H")
    launch_arguments.remove(str(ip_address))

    ip_address = simpledialog.askstring("IP", "Podaj adres IP hosta:")
    launch_arguments.append("-H")
    launch_arguments.append(ip_address)

    if ip_address and is_valid_ip(ip_address):
        launch()
    else:
        messagebox.showerror("Błąd", "Podany adres IP jest niepoprawny.")


def is_valid_ip(ip):
    try:
        # Sprawdzenie, czy adres IP jest poprawny
        ipaddress.IPv4Address(ip)
        return True
    except ipaddress.AddressValueError:
        return False


def exit():
    root.destroy()


if __name__ == "__main__":
    # Tworzenie głównego okna
    root = tk.Tk()
    root.title("Okręty Delta")

    # Początkowy rozmiar okna w rozdzielczości 16:9

    width_var = tk.StringVar()
    width_var.set(str(window_width))

    height_var = tk.StringVar()
    height_var.set(str(window_height))
    root.geometry(f"{window_width}x{window_height}")

    # Minimalne rozmiary okna
    root.minsize(width=320, height=280)

    # Ustawianie tła i ikony
    label1 = Label(root)
    bg = tk.PhotoImage(file=r"assets/launcherBg.png")
    set_background()
    icon = tk.PhotoImage(file=r"assets/skullMarker.png")
    root.tk.call('wm', 'iconphoto', root._w, icon)

    launch_arguments = ["-Ws", width_var.get(), height_var.get()]

    launch_arguments.append("-P")
    launch_arguments.append(str(port))

    launch_arguments.append("-H")
    launch_arguments.append(ip_address)

    # Nie mam dostępu do wielkości okna na bieżąco więc nawet nie próbuję tego dobrze padować
    # Przyciski do uruchamiania gry, pomocy i opcji
    start_button = tk.Button(root, text="Uruchom grę", command=lambda: (get_ip_address()), height=3, width=15)
    start_button.pack(pady=20)

    options_button = tk.Button(root, text="Opcje", command=toggle_option_buttons, height=3, width=15)
    options_button.pack(pady=20)

    help_button = tk.Button(root, text="Pomoc", command=toggle_help_buttons, height=3, width=15)
    help_button.pack(pady=20)

    exit_button = tk.Button(root, text="Wyjście", command=exit, height=3, width=15)
    exit_button.pack(pady=20)

    change_button_color_hex(start_button, "#67B7D1")
    change_button_color_hex(options_button, "#67B7D1")
    change_button_color_hex(help_button, "#67B7D1")
    change_button_color_hex(exit_button, "#67B7D1")

    # Przyciski powrotu i pełnego ekranu w ekranie opcji
    option_back_button = tk.Button(root, text="Powrót", command=back_to_menu, height=3, width=15)

    fullscreen_frame = tk.Frame(root, bg="#67B7D1")
    fullscreen_button = tk.Button(fullscreen_frame, text="Pełny ekran",
                                  command=lambda: toggle_fullscreen(launch_arguments), height=3, width=15, fg="#000040")
    fullscreen_button.pack(pady=5)

    change_button_color_hex(fullscreen_button, "#ADD8E6")
    change_button_color_hex(option_back_button, "#ADD8E6")

    # Pola Entry dla szerokości i wysokości okna
    label_text = tk.Label(fullscreen_frame, text="Szerokość okna gry", fg="#00004d", bg="#3ba1c2")
    label_text.pack(padx=10)
    width_entry = Entry(fullscreen_frame, width=10, textvariable=width_var)
    width_entry.pack()

    label_text = tk.Label(fullscreen_frame, text="Wysokość okna gry", fg="#00004d", bg="#3ba1c2")
    label_text.pack(padx=10)
    height_entry = Entry(fullscreen_frame, width=10, textvariable=height_var)
    height_entry.pack()

    update_button = tk.Button(fullscreen_frame, text="Aktualizuj", command=update_dimensions, height=2, width=15,
                              fg="#000040", bg="#ADD8E6")
    update_button.pack(pady=5)

    # Przycisk powrotu i opisu w ekranie pomocy
    help_frame = tk.Frame(root, bg="#67B7D1")
    help_back_button = tk.Button(help_frame, text="Powrót", command=back_to_menu, height=3, width=15)
    help_back_button.pack(side=tk.LEFT, padx=5, pady=5)
    desc_button = tk.Button(help_frame, text="Opis", command=show_help, height=3, width=15)
    desc_button.pack(side=tk.LEFT, padx=5, pady=5)

    change_button_color_hex(help_back_button, "#ADD8E6")
    change_button_color_hex(desc_button, "#ADD8E6")

    # Opisy gui
    gui_img_frame = tk.Frame(root, bg="#67B7D1")
    gui_desc_frame = tk.Frame(root, bg="#67B7D1")

    blue_square_img = tk.PhotoImage(file="assets/blueSquareMarker.png")
    black_square_img = tk.PhotoImage(file="assets/squareMarker.png")
    red_x_img = tk.PhotoImage(file="assets/crossMarker.png")
    blue_circle_img = tk.PhotoImage(file="assets/circleMarker.png")
    green_check_img = tk.PhotoImage(file="assets/checkMarker.png")

    # Dodawanie obrazów
    blue_square_label = tk.Label(gui_img_frame, image=blue_square_img, bg="#67B7D1")
    blue_square_label.pack(side=tk.TOP, pady=5)
    black_square_label = tk.Label(gui_img_frame, image=black_square_img, bg="#67B7D1")
    black_square_label.pack(side=tk.TOP, pady=5)
    red_x_label = tk.Label(gui_img_frame, image=red_x_img, bg="#67B7D1")
    red_x_label.pack(side=tk.TOP, pady=5)
    blue_circle_label = tk.Label(gui_img_frame, image=blue_circle_img, bg="#67B7D1")
    blue_circle_label.pack(side=tk.TOP, pady=5)
    green_check_label = tk.Label(gui_img_frame, image=green_check_img, bg="#67B7D1")
    green_check_label.pack(side=tk.TOP, pady=5)

    # Dodawanie opisów obrazów
    label_text = tk.Label(gui_desc_frame, text="Niebieski kwadrat oznacza stawiany statek", fg="#00004d", bg="#3ba1c2")
    label_text.pack(side=tk.TOP, pady=13)
    label_text = tk.Label(gui_desc_frame, text="Czarny kwadrat oznacza postawiony już statek", fg="#00004d",
                          bg="#3ba1c2")
    label_text.pack(side=tk.TOP, pady=14)
    label_text = tk.Label(gui_desc_frame, text="Czerwony X oznacza trafienie", fg="#00004d", bg="#3ba1c2")
    label_text.pack(side=tk.TOP, pady=14)
    label_text = tk.Label(gui_desc_frame, text="Niebieskie koło oznacza nietrafiony strzał", fg="#00004d", bg="#3ba1c2")
    label_text.pack(side=tk.TOP, pady=14)
    label_text = tk.Label(gui_desc_frame, text="Zielony checkmark służy do potwierdzania decyzji", fg="#00004d",
                          bg="#3ba1c2")
    label_text.pack(side=tk.TOP, pady=13)

    # Uruchomienie głównej pętli programu
    root.mainloop()
