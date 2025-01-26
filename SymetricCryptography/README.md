# Projekt na Przedmiot "Wprowadzenie do Cyberbezpieczeńśtwa"

# Temat 14 
Implementacja aplikacji szyfrującej i deszyfrującej pliki z wykorzystaniem kryptografii symetrycznej.

# Instrukcja uruchomienia
Wszystkie pliki należy umieścić w folderze, a następnie uruchomić skrypt inicjalizayjny 'setup.py', który zainstaluje wszystkie potrzebne
dependencje zawarte w pliku 'requirements.txt' zakładając, zę Python został zainstalowany ze wsparciem dla Tcl/Tk (potrzebne do działania tkinter, w którym jest napisane GUI), jeśli nie to można to naprawić pobierając jeszcze raz installer Pythona i zaznaczając tą opcję po kliknięciu w nim 'Modify'.
Następnie należy uruchomić skrypt 'main.py',aby otworzyć aplikację w trybie graficznym.

# Działanie
Aplikacja potrafi szyfrować i deszyforwać pliki w kilku trybach (ECB,CBC,CFB,OFB,CTR).
Aby rozpocząć pracę należy wybrać akcję, tryb oraz interesujący nas plik i kliknąć 'run'. Pliki pozwalające na sensowny ich podgląd będą wyświetlane w miejscu
okienek ze znakami zapytania.
W celach dydaktycznych jest możliwość ustawienia poziomu błędu w szyfrogramie w procentach zarówno podczas szyforwania jak i deszyforwania (losowe piksele otrzymują losowe wartości, wytwarzany jest sztuczny szum).

# Output
Po szyfrowaniu w folderze data tworzony jest plik z rozszerzeniem '.secret'.
Natomiast podczas każdej akcji tworzone są odpowiednio pliki odszyforwane z infixem '-dec' i zaszyfrowane '-enc'.
W przypadku symulowania błędu tworzony jest plik z infixem '-err', który przedstawia, które bajty zostały losowo przekłamane.

# Algorytm szyfrujący
Algorytmem jest AES (Advanced Encryption Standard). Używa 16-bajtowych bloków i 32-bajtowego klucza.
Oferuje dużą szybkość działania oraz niskie zużycie pamięci. Jest uważany za bezpieczny.

# ECB
W trybie szyfrowania Electronic Codebook każdy blok szyfrowany jest osobno przy użyciu klucza. Deszyfrowanie działa adekwatnie.
Bład w jednym bajcie bloku wpływa na cały blok wynikowy.
Ten tryb nie ukrywa wzorów danych co można zaobserwować w zaszyfrowanej bitmapie. Nie jest on rekomendowany do używania w ogóle.
Umożliwia współbieżne szyfrowanie i deszyfrowanie.

# CBC 
W trybie Cipher Block Chaining pierwszy blok złączany jest xorowaniem z 16-bajtowym wektorem inicializacyjnym (IV). Wynik szyfrowania danego bloku zostaje użyty jako wektor w następnym.
W deszyfrowaniu IV złączany jest z wynikiem deszyfracji bloku do właściwego wyniku deszyfracji. Dany blok zostaje użyty jako wektor do następnego bloku.
Bład w jednym bajcie bloku lub IV podczas szyfrowania wpływa na całość pliku. Podzas deszyfracji bład w jednym bajcie bloku wpływa na cały blok wynikowy.
Umożliwia współbieżne deszyfrowanie, ale nie szyfrowanie.

# CFB
W trybie Cipher Feedback na początku szyfrowany jest IV, wynik jest złączany z blokiem danych. To co wyjdzie zostaje użyte jako dane do szyfrowanie w następnym bloku.
Deszyfracja działa podobnie, deszyfrowany jest IV, wynik złączany z blokiem szyfru. Do następnego bloku brany jest blok szyfru.
Wpływ błędu podobnie jak CBC.
Umożliwia współbieżne deszyfrowanie, ale nie szyfrowanie.

# OFB
Tryb Output Feedback tym różni się od CFB, że podczas szyfrowania do następnego bloku brany jest wynik szyfrowania niezłączony z blokiem danych. 
W deszyfracji również do następnego bloku brany jest wynik algorytmu deszyfrującego. To sprawia, że błąd w jednym bajcie bajcie bloku danych
wpływa tylko na jeden bajt wyniku, a nie cały blok zarównu w szyfrowaniu jak i deszyfrowaniu.
Nie umożliwia współbieżności, jedynie współbieżne wykonanie ostatniego xorowania.

# CTR
W trybie Counter do algorytmu szyfrującego brany jest IV który dla każdego bloku zwiększany jest o 1. Błąd w jednym bajcie bajcie bloku wpływa tylko na jeden bajt wyniku.
Umożliwia współbieżne szyfrowanie i deszyfrowanie.
