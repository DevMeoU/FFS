#delete file main.exe
rm main.exe
# generator main.exe
gcc -o main application/app_main.c middleware/mw_menu.c fatfs_drive/fatfs_drive.c hal/file_hal.c utilities/linked_list/linked_list.c utilities/timer/timer.c -I"./application" -I"./middleware" -I"./fatfs_drive" -I"./hal" -I"./utilities"