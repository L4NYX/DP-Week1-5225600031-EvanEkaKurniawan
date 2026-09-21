# DP-Week1-5225600031-EvanEkaKurniawan
Repository mengenai tugas design pattern kolaborasi analisis Balatro


1. What is the invariant structure in your program?

Struktur invarian dalam program ini adalah urutan eksekusi fase di dalam method RunSession::run(), yaitu perulangan 3 ronde dengan 6 fase mutlak: (1) Generate input, (2) Compute base score, (3) Compute reward, (4) Update money, (5) Shop phase, dan (6) Advance round. Urutan dan batasan alur ini tidak boleh berubah apa pun variasi aturan mainnya.

2. Which parts are mutable?

Bagian yang mutable adalah implementasi konkret dari aturan perilaku (behavior): bagaimana input dihasilkan (IInputGenerator), bagaimana input dikonversi menjadi skor dasar (IScoringRule), formula perhitungan uang hasil skor (IRewardRule), dan mekanisme barang/transaksi di toko (ShopSystem).

3. When you replaced the InputGenerator, why didn’t RunSession change?

Karena RunSession menerapkan prinsip Dependency Inversion dan Polymorphism. RunSession hanya bergantung pada kontrak abstrak (IInputGenerator), bukan pada kelas konkret seperti FixedInputGenerator atau RandomInputGenerator. Selama kelas baru mengimplementasikan method generateInput() dan mengembalikan objek TurnInput, RunSession tidak peduli bagaimana data tersebut dihasilkan secara internal.

4. What would happen if scoring logic was placed inside RunSession?

Jika logika skor diletakkan di dalam RunSession, terjadi pelanggaran Single Responsibility Principle (SRP) dan Open/Closed Principle (OCP). RunSession akan terikat erat (tightly coupled) dengan aturan permainan tertentu. Setiap kali ada perubahan aturan skor (misalnya penambahan kartu Joker atau modifier kartu khas Balatro), kode RunSession harus diedit dan diuji ulang. Hal ini meningkatkan risiko rusaknya alur putaran (core loop) yang seharusnya tidak pernah berubah.
