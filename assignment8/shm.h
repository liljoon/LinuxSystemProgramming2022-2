#define	MY_ID		34
#define	SHM_KEY		(0x9000 + MY_ID)
#define	SHM_SIZE	1024
#define	SHM_MODE	(SHM_R | SHM_W | IPC_CREAT)

#define	SEM_KEY_1	(0x5000 + MY_ID)
#define	SEM_KEY_2	(0x6000 + MY_ID)
