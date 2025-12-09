#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#define max 40

typedef struct mahasiswa{
    int no_absen, nim;
    char nama[50];
    int vote;
}mahasiswa;


typedef struct kandidat{
    int cid;
    int votes;
    char k_nama[50];
    char visi[100];
    char misi[100];
    int top_voter;
    mahasiswa stack_voter[max];
}kandidat;

//-----Global Variabel---------
mahasiswa pemilih[max];
char kelas[5];
int jumlah_kandidat;
int jumlah_pemilih;
kandidat kandidat_array[5];
int total_voted;
int index, y;// index untuk array pemilih dan y untuk bantuan index kandidat
//----------------------------------------------------------------

//Store file 2KS3 ke dalam struct mahasiswa
void getfield(char field[], int nomor) //Fungsi untuk memisahkan no absen, nim, nama, dan kelas yang dibatasi oleh ";"
{
    char* token;
    token = strtok(field, ";");

    int fieldno = 0;
    while( token != NULL )
    {
        if(fieldno == 0){
            pemilih[nomor].no_absen = atoi(token);
        }

      	else if(fieldno == 1) {
            strcpy(pemilih[nomor].nama, token);
        }

      	else if(fieldno == 2) {
            pemilih[nomor].nim = atoi(token);
        }

        fieldno++;
        token = strtok(NULL, ";");
  }
}

void read_pemilih() //Fungsi untuk store file ke struct mahasiswa
{
    FILE* data;
    char filename[20];
    sprintf(filename, "%s.txt", kelas);
    data = fopen(filename, "r");

    char field[100];
    int nomor = 0;
    while(fgets(field, 100, data))
    {
        char* tmp = strdup(field);
        getfield(tmp, nomor);

        free(tmp);
        nomor++;
    }
    jumlah_pemilih = nomor;
    fclose(data);
}

//Mengecek apakah Nim tersebut ada pada data kelas tersebut
int isValid(int NIM, mahasiswa pemilih[]){
    int tmp = NIM;
    int digit = 0;
 	do {
    	tmp /= 10;
   		++digit;
 	}while (tmp != 0);
  	if(digit != 9)
        return 0;
    for (int i = 0; i < jumlah_pemilih; i++){
        if (NIM == pemilih[i].nim)
            return 1;
    }
    return 0;
}

//Login Admin (username = admin & password = admin)
int authenticateAdmin(){
    char username[15], password[6];
    printf("\n\n\n\n\n\n");
    printf("\t\t\t\t\t      =================================\n");
    printf("\t\t\t\t\t      .__          .  .__.  .         \n");
    printf("\t\t\t\t\t      [__) _.._  _ |  [__] _|._ _ *._ \n");
    printf("\t\t\t\t\t      |   (_][ )(/,|  |  |(_][ | )|[ )\n");
    printf("\t\t\t\t\t      =================================\n");
    printf("\t\t\t\t\t      Username : ");
    scanf("%s",username);
    if((strcmp(username,"admin")) !=0 ){
        return 0;
    }
    else{
        printf("\t\t\t\t\t      Password : ");
        fflush(stdin);
        int i=0;
        for(i=0;i<5;i++)
        {
            password[i]=getch();
            printf("%c",'*');
        }
        password[i]='\0';
        if((strcmp(password,"admin")) !=0 )
            return 0;
    }
    return 1;
}

//push struct mahasiswa ke dalam array stack pemilih
void push(mahasiswa stack_voter[], int idx, int id){
    int top = kandidat_array[id].top_voter;
    // printf("\n%d\n", kandidat_array[id].top_voter);
    // getch();
    if(top == max-1){
        printf("\n error full");
        return;
    }
    else{
        stack_voter[top].no_absen = pemilih[idx].no_absen;
        stack_voter[top].nim = pemilih[idx].nim;
        stack_voter[top].vote = pemilih[idx].vote;
        strcpy(stack_voter[top].nama, (pemilih[idx].nama));
        kandidat_array[id].top_voter++;

    }
}

//pop menghilangkan satu stuct mahasiswa pada stack pemilih serta menampilkan data yang di pop
void pop(mahasiswa stack_voter[], int id){
   int top = kandidat_array[id].top_voter;
    if (top == -1){
        printf("\n");
    }
    else{
         printf("    Pemilih ke %d : %s (%d) \n", top+1, kandidat_array[id].stack_voter[top].nama, kandidat_array[id].stack_voter[top].no_absen);
         kandidat_array[id].top_voter--;
    }
}

// Membuat File Kandidat yang telah dibuat
void create_file_kandidat(){
    printf("\n\t\t\t\t Membuat File Kandidat...");
    FILE *fp;
    char filename[20];
    for (int i = 1; i <= jumlah_kandidat; i++)
    {
        sprintf(filename, "candidate%d.txt", i);
        fp = fopen(filename, "w");
        fprintf(
            fp, "%d\n%d\n%s\n%s\n%s",
            kandidat_array[i-1].votes,
            kandidat_array[i-1].cid,
            kandidat_array[i-1].k_nama,
            kandidat_array[i-1].visi,
            kandidat_array[i-1].misi
        );
        fclose(fp);
    }
    printf("\n\t\t\t\t Kandidat File Berhasil dibuat");
    getch();
}

//Membuat Sistem voting Ketua kelas baru
void initiateNewElection(){
    int NIM;
    int id=1;
    // char visi[100], misi[100];
    printf("\n\t\t\t\tPemilihan Ketua Kelas Baru\n");
    printf("\n\t\t\t\tMasukkan Kelas: ");
    getchar();
    gets(kelas);
    strupr(kelas);
    fflush(stdin);
    read_pemilih();
    printf("\t\t\t\tMasukkan Jumlah Kandidat: ");
    scanf("%d",&jumlah_kandidat);
    for (int i = 0;i < jumlah_kandidat; i++){
        int tmp;
        cek_nim:
            printf("\n\t\t\t\tMasukkan NIM Kandidat %d: ", id);
            scanf("%d", &NIM);
        while (isValid(NIM, pemilih) != 1){
            printf("\t\t\t\tNIM tidak valid atau tidak berada di kelas ini");
            printf("\n\t\t\t\t           Tekan enter");
            getch();
            goto cek_nim;
        }
        for (int j = 0; j < jumlah_pemilih; j++){
            if (NIM == pemilih[j].nim){
                tmp = j;
                break;
            }
        }
        kandidat_array[i].cid = id;
        kandidat_array[i].top_voter = -1;
        kandidat_array[i].votes = 0;
        strcpy(kandidat_array[i].k_nama, pemilih[tmp].nama);
        printf("\n\t\t\t\tKandidat %d : %s\n", id, kandidat_array[i].k_nama);
        printf("\t\t\t\tMasukkan Visi Kandidat %d: ", id);
        getchar();
        scanf("%[^\n]", kandidat_array[i].visi);
        printf("\t\t\t\tMasukkan Misi Kandidat %d: ", id);
        // fflush(stdin);
        getchar();
        // fflush(stdin);
        scanf(" %[^\n]", kandidat_array[i].misi);
        id++;
        }
    return;
}

// Menyimpan data info pemilihan kedalam file agar dapat di load kembali
void save_election_file(){
    system("cls");
    printf("\n\n\n\n\n\n\n\n\n");
    printf("\t\t\t\t Menyimpan Info Pemilihan dalam File...\n");
    FILE *fp = fopen("ElectionInfo.txt", "w");
    if(fp==NULL)
    {
        printf("\n\t\t\t\t\tError dalam membuat file\n");
        fclose(fp);
        return;
    }
    fprintf(
        fp,"%s\n%d",
        kelas,
        jumlah_kandidat
    );
    fclose(fp);
    printf("\t\t\t\t Info Pemilihan Berhasil di Simpan\n");
    getch();
}

//Pengecekan apakah NIM tersebut telah memilih atau belum
int isVoted(int NIM, mahasiswa pemilih[]){
    int i;
    if (pemilih[index].vote != 0){
        return 1;
    }
    return 0;
}

//Menyimpan data vote kedalam varibel pada struct kandidat serta mahasiswa
// dan juga meyimpan data absen pemilih ke dalam file kandidat yang telah dibuat
void saveVote(int NIM, int voteinput){
    char filename[20];
    sprintf(filename,"candidate%d.txt",voteinput);
    FILE *fp = fopen(filename,"r+");//search file
    if (fp == NULL)
        printf("\n\t\t\t\t\tFile Kandidat tidak tersedia");
    kandidat_array[voteinput-1].votes++;
    pemilih[index].vote = voteinput;
    total_voted++;
    kandidat_array[voteinput-1].top_voter++;
    push(kandidat_array[voteinput-1].stack_voter, index, voteinput-1);
    kandidat_array[voteinput-1].top_voter--;
    fseek(fp, 0, SEEK_SET);// pointer di posisi awal file
    fprintf(fp,"%d",kandidat_array[voteinput-1].votes);
    fseek(fp, 0, SEEK_END); //pointer di posisi di akhir file
    fprintf(fp,"\n%d", pemilih[index].no_absen);
    fclose(fp);
}

//Mendapatkan id(nomor urut) kandidat yang menjadi pemenang
int getwinner(){
    int top = -1;
    int winnercid;
    if (total_voted < jumlah_pemilih/2){
        return 0;
    }
    for (int i = 0; i < jumlah_kandidat; i++){
        if(kandidat_array[i].votes > top){
            winnercid = kandidat_array[i].cid;
            top = kandidat_array[i].votes;
        }
        else if (kandidat_array[i].votes == top)
            return -1;
    }
    return winnercid;
}

//Load file elcection dan informasi pada file kandidat yang telah dibuat sebelumnya
void load_election_info(){
    FILE *f1, *f2;
    f1 = fopen("ElectionInfo.txt", "r");
    if (f1 == NULL){
        printf("\n\t\t\t\t\t\t File Election Tidak Tersedia");
        return;
    }
    fscanf(f1, "%s", kelas);
    fseek(f1, 2, SEEK_CUR);
    fscanf(f1, "%d", &jumlah_kandidat);
    fclose(f1);
    read_pemilih();

    for (int i=1; i<=jumlah_kandidat; i++){
        char filename[20];
        sprintf(filename,"candidate%d.txt",i);
        f2 = fopen(filename,"r+");//search file
        if (f2 == NULL){
            printf("\n\t\t\t\t\tFile Kandidat tidak tersedia");
            return;
        }
        fscanf(f2, "%d", &kandidat_array[i-1].votes);
        fseek(f2, 2, SEEK_CUR);
        fscanf(f2, "%d", &kandidat_array[i-1].cid);
        fseek(f2, 2, SEEK_CUR);
        fscanf(f2, "%[^\n]", kandidat_array[i-1].k_nama);
        fseek(f2, 2, SEEK_CUR);
        fflush(stdin);
        fscanf(f2, "%[^\n]", kandidat_array[i-1].visi);
        fseek(f2, 2, SEEK_CUR);
        fflush(stdin);
        fscanf(f2, "%[^\n]", kandidat_array[i-1].misi);
        kandidat_array[i-1].top_voter = -1;

        while (!feof(f2)){
            int absen;
            fscanf(f2, "%d", &absen);
            pemilih[absen-1].vote = i;
            push(kandidat_array[i-1].stack_voter, absen-1, i);
            kandidat_array[i-1].top_voter++;
        }
        total_voted+=kandidat_array[i-1].votes;
        fclose(f2);
    }
    system("cls");
    printf("\n\n\n\n");
    printf("  Load Election Berhasil ");
    getch();
}


void adminPanel()
{
    while(1){
        if(authenticateAdmin()!= 1){
            printf("\n\n\n\n\n\n");
            printf("\t\t\t\t\t Username atau Password salah \n");
            break;
        }
        printf("\n\n\t\t\t\t\t\tLOGGED IN BERHASIL");
        printf("\n\t\t\t\t\t\t(Press Enter)");
		getch();

        while(1){
            system("cls");
            int inputNIM[9];
            char input;
            int WinnerCid, total_voted=0; ;
            float hasil_kandidat, hasil_total;
            printf("\t\t\t\t\t      =================================\n");
            printf("\t\t\t\t\t      .__          .  .__.  .         \n");
            printf("\t\t\t\t\t      [__) _.._  _ |  [__] _|._ _ *._ \n");
            printf("\t\t\t\t\t      |   (_][ )(/,|  |  |(_][ | )|[ )\n");
            printf("\t\t\t\t\t      =================================");
            printf("\n\t\t\t\t\t      1. Buat Pemilihan Baru");
            printf("\n\t\t\t\t\t      2. Lanjutkan Pemilihan");
            printf("\n\t\t\t\t\t      3. Tampilkan Hasil");
            printf("\n\t\t\t\t\t      4. Daftar Pemilih Resmi");
            printf("\n\t\t\t\t\t      5. Info Kandidat");
            printf("\n\t\t\t\t\t      6. Keluar ");
            printf("\n\t\t\t\t\t      Pilihan: ");
			scanf(" %c",&input);
            switch(input)
            {
                case '1':
                    initiateNewElection();
                    save_election_file();
                    create_file_kandidat();
                    break;
                case '2':
                    load_election_info();
                    printf("\n       Kelas Saat ini  : %s", kelas);
                    printf("\n       Jumlah Kandidat : %d", jumlah_kandidat);
                    read_pemilih();
                    printf("\n      =============================================\n");
                    printf("      Kandidat Ketua Kelas: \n");
                    for (int i = 1; i <= jumlah_kandidat; i++)
                    {
                    printf("      =============================================");
                    printf("\n      %d. %s",i,kandidat_array[i-1].k_nama);
                    printf("\n      Visi : %s", kandidat_array[i-1].visi);
                    printf("\n      Misi : %s\n", kandidat_array[i-1].misi);
                    }
                    printf("\n\t\t Tekan Enter untuk kembali");
                    getch();
                    break;
                case '3':
                    WinnerCid = getwinner();
                    printf("\n\t\t\t\t\t      =================================");
                    for(int i=0; i<jumlah_kandidat; i++){
                        total_voted+=kandidat_array[i].votes;
                        hasil_kandidat = (kandidat_array[i].votes*100)/jumlah_pemilih;
                        printf("\n\t\t\t\t\t%d. %s -> %.2f%%",kandidat_array[i].cid,kandidat_array[i].k_nama,hasil_kandidat);
                    }
                    if(WinnerCid == 0 ){
                        printf("\n\n\t\t\t\t\tJumlah pemilih saat ini masih dibawah 50%% jumlah kelas");
                    }
                    else if (WinnerCid == -1){
                        printf("\n\t\t\t\t\tHasil Perolehan Suara Seri");
                    }
                    else{
                        printf("\n\t\t\t\t\tPemenang adalah %s dengan perolehan %d suara\n",kandidat_array[WinnerCid-1].k_nama,kandidat_array[WinnerCid-1].votes);
                    }

                    //Tampilkan Hasil Voting Sementara
                    printf("\n\n\t\t\t\t\tHasil Keseluruhan : ");
                    printf("\n\t\t\t\t\tJumlah Pemilih saat ini : %d\n", total_voted);
                    hasil_total = (total_voted*100)/jumlah_pemilih;
                    printf("\t\t\t\t\t%s -> Voting Percentage saat ini : %.2f%%", kelas, hasil_total);
                    printf("\n\n\t\t\t\t\t Tekan Enter untuk kembali");
                    getch();
                    break;
                case '4':
                    system("cls");
                    printf("\n\n\t===========================================");
                    printf("\n\t  Daftar Pemilih Resmi kelas %s : ", kelas);
                    printf("\n\t===========================================");
                    for (int i = 0; i < jumlah_pemilih; i++){
                        printf("\n\t %d. %d  %s", pemilih[i].no_absen, pemilih[i].nim, pemilih[i].nama);
                    }
                    printf("\n\t\t Tekan Enter untuk kembali");
                    getch();
                    break;
                case '5':
                    system("cls");
                    printf("\n    Kelas Saat ini  : %s", kelas);
                    printf("\n    Jumlah Kandidat : %d", jumlah_kandidat);
                    printf("\n    =============================================\n");
                    printf("    Kandidat Ketua Kelas: \n");
                    for (int i = 1; i <= jumlah_kandidat; i++){
                        printf("    =============================================");
                        printf("\n    %d. %s",i,kandidat_array[i-1].k_nama);
                        printf("\n    Visi : %s", kandidat_array[i-1].visi);
                        printf("\n    Misi : %s\n", kandidat_array[i-1].misi);
                        printf("    =============================================");
                        printf("\n    Voters: \n");
                        int idx = i-1;
                        int reset_top = kandidat_array[idx].top_voter;
                        for (int j= kandidat_array[idx].top_voter; j>=0 ; j--){
                            pop(kandidat_array[idx].stack_voter, idx);
                        }
                        // printf("\n%d", kandidat_array[id].top_voter);
                        // getch();
                        kandidat_array[idx].top_voter = reset_top;
                    }
                    printf("\n\t\t Tekan Enter untuk kembali");
                    getch();
                    break;
                case '6':
                    return;
                default:
                    printf("\n\t\t\t\t\tPilihan Tidak Sesuai");
					getch();
            }
        }
    }
};


void studentPanel()
{
    int NIM;
    int voteInput;
    while(1)
	{
        system("cls");
        printf("\n\n\n");
        printf("\t\t\t\t      =============================================\n");
        printf("\t\t\t\t      .__          .  .   .   .                    \n");
        printf("\t\t\t\t      [__) _.._  _ |  |'v'| _.|_  _. __* __. _ . _.\n");
        printf("\t\t\t\t      |   (_][ )(/,|  |   |(_][ )(_]_) |_)  V V (_]\n");
        printf("\t\t\t\t      =============================================\n");
		printf("\t\t\t\t                  Tekan 0 untuk kembali            \n");
        printf("\t\t\t\t      =============================================\n\n");
        printf("\t\t\t\t      Masukkan NIM: ");
        scanf("%d", &NIM);
        for (int i = 0; i < jumlah_pemilih; i++){
            if (NIM == pemilih[i].nim){
                index = i;
                break;
            }
        }
        if(NIM == 0)
			return;
        if(isValid(NIM, pemilih) != 1){
            printf("\t\t\t\t\t       NIM tidak Valid \n");
            printf("\t\t\t\t\t        (Press Enter)  \n");
            getch();
            continue;
        }
        if(isVoted(NIM, pemilih) != 0){
            printf("\n\t\t\t\t     Nomor Induk Mahasiswa ini telah memilih\n");
            printf("\n\t\t\t\t         Hubungi Admin Untuk Bantuan         \n");
            getch();
            continue;
        }
        system("cls");
        printf("\n\n\n\n\n\n\n\n\n");
        printf("\n\n\n\n\t\t\t\t\t       SELAMAT DATANG        ");
        printf("\n\t\t\t\t\t       %s", pemilih[index].nama);
        getch();

        char vote;
        vote:
        system("cls");
        printf("\n\n\n");
        printf("      =============================================\n");
        printf("     .__          .  .   .   .                    \n");
        printf("     [__) _.._  _ |  |'v'| _.|_  _. __* __. _ . _.\n");
        printf("     |   (_][ )(/,|  |   |(_][ )(_]_) |_)  V V (_]\n");
        printf("      =============================================\n");
		printf("                   Tekan 0 untuk kembali           \n");
        printf("      =============================================\n");
        printf("      Kandidat Ketua Kelas: ");
		for (int i = 1; i <= jumlah_kandidat; i++)
        {
            printf("\n      =============================================\n");
            printf("      %d. %s",i,kandidat_array[i-1].k_nama);
            printf("\n      Visi : %s", kandidat_array[i-1].visi);
            printf("\n      Misi : %s", kandidat_array[i-1].misi);
        }
            printf("\n\n      Pilihan Anda (Masukkan Nomor Kandidat):");
            voteInput=getch();
            printf("*");
            voteInput-=48;
        if (voteInput < 1 || voteInput > jumlah_kandidat)
        {
            printf("\n\t\t\t Pilihan Tidak Valid");
            printf("\n\t\t\t     Coba lagi      ");
            getch();
            continue;
        }
        printf("\n      Konfirmasi Pilihan Anda (Y/N): ");
        vote=getch();
        if (vote == 'y' || vote == 'Y'){
            saveVote(NIM, voteInput);
        }
        else{
            goto vote;
        }
        system("cls");
        printf("\n\n\n\n\n\n\n\n\n");
        printf("\n\n\t\t\t\t\tTerima Kasih atas vote Anda");
        printf("\n\t\t\t\t\t        (Press Enter)        ");
        getch();
    }
};


void tentangprogram()
{
    printf("\n\n\n\n");
    printf("\t\t\t      =====================================================\n");
    printf("\t\t\t      .___.       ,            .__                      \n");
    printf("\t\t\t        |   _ ._ -+- _.._  _   [__)._. _  _ ._. _.._ _  \n");
    printf("\t\t\t        |  (/,[ ) | (_][ )(_]  |   [  (_)(_][  (_][ | ) \n");
    printf("\t\t\t                          ._|            ._|            \n");
    printf("\t\t\t      =====================================================\n\n");
    printf("\t\t\t      Mini Voting Sistem \n");
    printf("\t\t\t      Oleh Kelompok 5: \n");
    printf("\t\t\t      -> (14) Eliana Mardiyaningtyas            222112011\n");
    printf("\t\t\t      -> (17) Gilang Abdul Jabbar               222112076\n");
    printf("\t\t\t      -> (19) Inggid Utami                      222112111\n");
    printf("\t\t\t      -> (31) Pratiwi                           222112286\n");
    printf("\t\t\t      -> (32) Rahadian Eka Bagus Indra Rinangku 222112300\n");
    printf("\t\t\t      -> (33) Regita Pramiswari Hadi Maharani   222112311\n\n");
    printf("\t\t\t      Penggunaan Program : \n");
    printf("\t\t\t      1. Masuk Panel Admin dengan memasukkan username : 'admin' dan password : 'admin'\n");
    printf("\t\t\t      2. Pilih Menu Buat Pemilihan Baru \n");
    printf("\t\t\t      3. Masukkan kelas, jumlah kandidat, dan data diri kandidat\n");
    printf("\t\t\t         (Kandidat harus berasal dari kelas yang dipilih) \n");
    printf("\t\t\t      4. Jika telah selesai menginput kandidat ketua kelas, keluar dari panel admin \n");
    printf("\t\t\t      5. Lalu masuk ke panel mahasiswa dan program sudah siap utuk digunakan untuk pengumpulan voting \n");
    printf("\t\t\t                         ~Selamat Menggunakan~                        \n");
}

int main(){
//     strcpy(kelas, "2KS3");
//     read_pemilih();
//     jumlah_kandidat =3;
//     kandidat_array[0].cid = 1;
//     strcpy(kandidat_array[0].k_nama, "Dhymas Adhyza Rayhan");
//     strcpy(kandidat_array[0].visi, "Mewujudkan kelas 2KS3 yang peduli antar sesama dan dapat maju bersama ");
//     strcpy(kandidat_array[0].misi, "Mengadakan Acara Silaturahmi bersama, Mengumpulkan piket kelas dan kerja bakti kampus");
//     kandidat_array[0].top_voter = -1;
//
//     kandidat_array[1].cid = 2;
//     strcpy(kandidat_array[1].k_nama, "Danang Wisnu Prabowo");
//     strcpy(kandidat_array[1].visi, "Mewujudkan kelas yang kreatif, aktif, bertanggungjawab, dan dilandasi dengan iman");
//     strcpy(kandidat_array[1].misi, "Menumbuhkan rasa kekeluargaan antar anggota kelas dan Menegakan Peraturan kelas");
//     kandidat_array[1].top_voter = -1;
//
//     kandidat_array[2].cid = 3;
//     strcpy(kandidat_array[2].k_nama, "Rohmad Ali Fatur Rizki");
//     strcpy(kandidat_array[2].visi, "Membangun kelas yang memiliki anggota berkualitas, bermoral, dan kreatif");
//     strcpy(kandidat_array[2].misi, "Mengadakan Piket kelas dan Membuat program belajar bersama");
//     kandidat_array[2].top_voter = -1;
//     create_file_kandidat();
//
//     for (int i = 0; i<10; i++){
//         index = i;
//         saveVote(pemilih[index].nim, 1);
//     }
//
//     for (int i = 10; i<25;i++){
//         index = i;
//         saveVote(pemilih[index].nim, 2);
//     }
//     for (int i = 25; i<jumlah_pemilih;i++){
//         index = i;
//         saveVote(pemilih[index].nim, 3);
//     }
    while(1){
        system("cls");
        system ("color b0");
        printf("\n\n\n\n\n\n");
        printf("\t\t\t\t\t      .__ .__ .__..__ .__ .__..   .\n");
        printf("\t\t\t\t\t      [__)[__)|  |[ __[__)[__]|'v'|\n");
        printf("\t\t\t\t\t      |   |  ||__|[_./|  ||  ||   |\n");
        printf("\t\t\t\t\t    .__ .__ .  .. _ .    _ .  ..__..__.\n");
        printf("\t\t\t\t\t    [__)[__ |'v'| | |    | |__|[__]|  |\n");
        printf("\t\t\t\t\t    |   [__ |   | | |___ | |  ||  ||  |\n");
        printf("\t\t\t\t\t.  ..__.___..  ..__.  .  ..__ .   .__. __.\n");
        printf("\t\t\t\t\t|_/ [__  |  |  |[__]  |_/ [__ |   [__](__ \n");
        printf("\t\t\t\t\t| | [__  |  |__||  |  | | [__ |___|  | __)\n");
        printf("\n\t\t\t\t\tPILIHAN MENU : ");
        printf("\n\t\t\t\t\t 1. PANEL MAHASISWA \n\t\t\t\t\t 2. PANEL ADMIN \n\t\t\t\t\t 3. TENTANG PROGRAM \n\t\t\t\t\t 4. KELUAR");
        printf("\n\t\t\t\t\t PILIHAN ANDA : ");
		char input;
        scanf(" %c",&input);
        fflush(stdin);
        switch(input){
            case '1':
                system("cls");
                studentPanel();
                break;
            case '2':
                system("cls");
                adminPanel();
                getchar();
                break;
            case '3':
                system("cls");
                tentangprogram();
                getchar();
                break;
            case '4':
                save_election_file();
                return 0;
            default:
                printf("\nInvalid option");
                getch();
        }
    }
    return 0;
}
