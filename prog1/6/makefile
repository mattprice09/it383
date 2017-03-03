cc=gcc

all: shm_fileuploader shm_filedownloader

shm_fileuploader: shm_fileuploader.c mrprice_filesharing.h
	${cc} shm_fileuploader.c -o shm_fileuploader

shm_filedownloader: shm_filedownloader.c mrprice_filesharing.h
	${cc} shm_filedownloader.c -o shm_filedownloader
