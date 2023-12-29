import platform
import subprocess
import tkinter as tk
from tkinter import messagebox, Label

def launch():
    #Tymczasowe dummy argumenty
    try:
        if platform.system() == "Windows":
            messagebox.showinfo("Windows!", "Uruchamiam grę na systemie Windows.")
            subprocess.run(["gra.exe", "argument1", "argument2"], shell=True, check=True)
        else:
            messagebox.showinfo("Linux!", "Uruchamiam grę na systemie Linux.")
            subprocess.run(["./gra", "argument1", "argument2"], shell=True, check=True)
    except subprocess.CalledProcessError as e:
        messagebox.showerror("Błąd", f"Wystąpił błąd podczas uruchamiania gry: {e}")

def show_help():
    messagebox.showinfo("Pomoc", "Tu pojawi się pomoc dotycząca gry.")

def toggle_fullscreen():
    # Toggle pełnego ekranu
    root.attributes("-fullscreen", not root.attributes("-fullscreen"))

def toggle_buttons():
    # Ukryj/przywróć przyciski uruchamiania i pomocy
    start_button.pack_forget()
    help_button.pack_forget()
    options_button.pack_forget()

    back_button.pack(side=tk.LEFT, padx=10)

    fullscreen_frame.pack(side=tk.LEFT, padx=30)

def back_to_menu():
    # Przywróć przyciski uruchamiania i pomocy, ukryj przyciski powrotu i pełnego ekranu
    back_button.pack_forget()
    fullscreen_frame.pack_forget()

    start_button.pack(pady=10)
    options_button.pack(pady=10)
    help_button.pack(pady=10)

def set_background():
    label1.config(image=bg)
    label1.place(x=-5, y=-5)
    label1.config(width=2560, height=1440)

def change_button_color_hex(button, hex_color):
    button.config(bg=hex_color)

if __name__ == "__main__":
    # Tworzenie głównego okna
    root = tk.Tk()
    root.title("Okręty Delta")

    # Początkowy rozmiar okna w rozdzielczości 16:9
    window_width = 960
    window_height = 540
    root.geometry(f"{window_width}x{window_height}")

    # Minimalne rozmiary okna
    root.minsize(width=320, height=280)

    # Ustawianie tła
    label1 = Label(root)
    bg = tk.PhotoImage(file=r"assets/launcherBg.png")
    set_background()

    # Nie mam dostępu do wielkości okna na bieżąco więc nawet nie próbuję tego dobrze padować
    # Przyciski do uruchamiania gry, pomocy i opcji
    start_button = tk.Button(root, text="Uruchom grę", command=launch, height=3, width=15)
    start_button.pack(pady=10)

    options_button = tk.Button(root, text="Opcje", command=toggle_buttons, height=3, width=15)
    options_button.pack(pady=10)

    help_button = tk.Button(root, text="Pomoc", command=show_help, height=3, width=15)
    help_button.pack(pady=10)

    change_button_color_hex(start_button, "#67B7D1")
    change_button_color_hex(options_button, "#67B7D1")
    change_button_color_hex(help_button, "#67B7D1")

    # Przyciski powrotu i pełnego ekranu (na razie ukryte)
    back_button = tk.Button(root, text="Powrót", command=back_to_menu, height=3, width=15)

    fullscreen_frame = tk.Frame(root)
    fullscreen_button = tk.Button(fullscreen_frame, text="Pełny ekran", command=toggle_fullscreen, height=3, width=15)
    fullscreen_button.pack()

    change_button_color_hex(fullscreen_button, "#ADD8E6")
    change_button_color_hex(back_button, "#ADD8E6")

    # Uruchomienie głównej pętli programu
    root.mainloop()
