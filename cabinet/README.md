# CABINET 
berisikan program Arduino dan skema Godong beserta dokumentasi kecil lainya

## **LOG**
(Rabu 29 juni 2022) hasil yang didapat oleh Ahsanu Amala(2019) stevanus bintang(2019), pada periode magang 5-6 2022
- **terdapat ppt pada folder doc hasil periode kami.**
- penambahan FREERTOS untuk otomasi scheduler 
- setting waktu lampu on/off, aerator on/off, waktu mist on/off, setting ppm masing-masing rak sudah tersimpan pada EEPROM
- kontrol lampu dan aerator dapat diatur melalui komunikasi serial pc dengan arduino
- Terdapat menu untuk melihat status dan setting dari cabinet

***Tambahkan Log periode anda....***


## TO DO...
- TDS masih belum stabil
- DHT belum dipakai
- Kompensasi suhu untuk tds belum dimasukan
- Mist belum jalan
- Waterflow masih belum jadi programnya
- Data hanya tertampil di local server bukan server(sambungkan ke server). lihat 'https://github.com/johandiMIS/Project'
- Bak air baku tidak ada
- Rak 0 terlalu pendek sehingga sedikit susah untuk memasang selang, kabel, dan mengganti air
- Kabel duck terlalu kecil. Saran mungkin bisa diganti yang agak sedikit lebih besar supaya kabel bisa lebih tersusun rapih
- Agar lebih rapih, bisa ditambahkan kabel spiral agar kabel yang keluar dari jalur bisa jadi satu jalur dengan menggunakan kabel spiral
- Ditambahkan log/catatan setiap periode magang agar perkembangan terlihat jelas
- Jika kebanyakan air maka lampu agak sedikit longgar. Saran besi untuk menahan air agar di kuatkan atau di bikin lebih tebal.




## Pesan

## catatan program
- pada void taskPenstabilPpm() program senaja tidak memakai for dan array, dikarenakan ***entah kenapa*** ketika memakai array dan for program tidak mau berganti task lain (ke task kontroller lampu dan aerator)
- program untuk menampilkan tabel pada local server (yang terlihat dalam ppt) masih belum diupload ke repo, hubungi Ahsanu Amala(2019) atau stevanus bintang(2019) jika ingin meminta program. alasan program tidak diupload ke repo dikarenakan program nodeMCU lebih difokuskan ke web dan API server, sehingga local server tidak perlu dikembangkan. 

## Cara Install 
**menggunakan git**
1. `cd` ke direktori yang ingin dipakai (contoh `d:/magang/godong/`)
2. di folder tersebut panggil `git clone https://github.com/ahsanu123/cabinet.git` (atau clone dari main repository)
3. install platform io dengan mengikuti instruksi pada link ini: `https://platformio.org/install`
4. coba jalankan plaform io dengan command `pio run` lalu klik enter (apabila command `pio` tidak terdeteksi, cek **environment variable**)
5. penggunaan lebih lanjut platformIO `https://docs.platformio.org/en/latest/core/quickstart.html`

# godong
