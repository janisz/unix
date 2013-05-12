Cjk
=====
#Napisać grę w stylu Rouge, składającą się z dwóch programów — klienta i serwera.

Serwer w momencie startu wczytuje mapę jaskiń z pliku postaci:

		10 5
		XXXXHXXCXC
		XXHCCCCCCC
		HCCXXXXHXC
		CXCCCCCCCC
		CCCHHXXXXX

W pierwszej linii znajdują się wymiary mapy `X Y` oddzielone spacją. Następnie następuje `Y` linii po `X` znaków o znastępującym znaczeniu:
* `X` ściana – pole na które nie ma wstępu
* `C` korytarz – pole na którym może znajdować się co najwyżej jeden gracz na raz
* `H` pokój,grota – duże pomieszczenie w którym może znajdować się dowolnie dużo graczy

Po planszy będą poruszać się gracze. Każdy z graczy jest człowiekiem renesansu i w związku z tym ma jeden atrybut określający po równo wszystkie jego umiejętności: siłę, zręczność, inteligencję, magię i zdolności artystyczne. Początkowa wartość atrybutu wynosi 1 i jest zwiększana w tracie gry.

Gra odbywa się w czasie rzeczywistym, bez podziału na kolejki. Gracze po mapie poruszają się tylko o jedno pole wzdłuż osi – północ, południe, wschód, zachód (nie ma ruchów na skos), nie mogą się poruszyć na pole, gdzie jest ściana oraz na pole korytarza, jeśli jest zajęte przez innego gracza.

Możliwa jest walka dwóch graczy. Jeśli A atakuje gracza B, to serwer z prawdopodobieństwem att(A)/(att(A)+att(B)) uśmierca gracza B, w przeciwnym wypadku gracza A. att(A) to wartość atrybutu gracza. Zwycięzca w nagrodę otrzymuje jeden punkt atrybutu. Przegrany jest odłączany od serwera.

Klient Po uruchomieniu klient pyta gracza o nick i podłącza się do serwera. Jeśli na serwerze jest już gracz o tym samym nicku, połączenie jest odrzucane. W przeciwnym przypadku serwer tworzy postać gracza i umieszcza w dowolnej grocie. Klient wypisuje na ekran informacje o aktualnym położeniu gracza i jego ew. zmianach, przyjmuje z klawiatury komendy wykonujące akcje

##Gracz może wykonywać następujące akcje:

1. Poruszyć się w jednym z 4 kierunków
2. Zaatakować innego gracza, będącego w tym samym pomieszczeniu
3. Efekty działania każdej z akcji gracza natychmiast pojawiają się na ekranie wszystkich graczy w pomieszczeniu (wejście, wyjście gracza, rezultat walki).
4. Ponadto gracz może rozejrzeć się po pomieszczeniu i dowiedzieć się w jakich kierunkach można się poruszyć oraz kto znajduje się w tym pokoju

Cała logika gry jest obsługiwana po stronie serwera. Klient jedynie wyświetla graczowi informacje od serwera i przesyła akcje do serwera. Serwer nie przechowuje stanu gry na dysku, po ponownym uruchomieniu rozpoczyna się całkowicie nowa gra.
