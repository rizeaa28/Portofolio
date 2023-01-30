324CB - Ciuclea Alexia

Protocolul de nivel aplicatie:

    Protocolul folosit de mine implica adaugarea unor headere peste mesajele trimise intre client si server.
    Ceste headere sunt diferite intre mesajele trimise de client si cele trimise de server.

Mesajele trimise de server:

    Serverul trimite clientului mesaje doar cand a primit un mesaj udp ce trebuie trimis acelui client. Deci, pe 
langa continutul mesajului udp primit, headerul va contine datele clientului UDP de la care se primeste mesajul si
lungimea mesajului udp.
    Datele clientului UDP sunt necesare pentru printarea mesajului, iar lungimea lui este necesara pentru identificarea mesajelor
trunchiate sau concatenate.

Mesaje primite de client:
    Clientul primeste mesaje de la server in tipul celor de mai sus. Pentru a verifica daca sunt concatenate sau trunchiate se verifica urmatoarele:
1. Daca mesajul primit este fix de BUFLEN biti (maximul), atunci exista posibilitatea sa nu se fi trimis tot mesajul
2. Daca length-ul mesajului citit din header este mai mic decat lungimea mesajului primit
    => in aceste 2 cazuri se concateneaza cu ceea ce se da receive de pe socket pana cand se da rceive la mai putin de BUFLEN biti
        (practic cand se termina mesajul trimis, o sa se trimita intre 0 si BUFLEN-1 biti, pentru ca serverul a terminat de trimis)
    ** recunosc ca aceasta implementare nu acopera faptul ca pe un socket se pot primi constant BUFLEN biti, dar acesta nu cred ca e cazul
    pentru modul in care se trimit mesaje de la server la client

    In cazul concatenarii mesajelor intr-un buffer citit cu recv, se trateaza fiecare pe rand deoarece ii cunoastem lungimea din campul din header

Mesaje trimise de client:
    Clientul tirmite serverului doar 3 timpuri de mesaje:

1. Mesaje de tip "id"
    Cand clientul se conecteaza la server, el trimite fix dupa un mesaj de acest tip ce contine id-ul clientului

    Forma mesaj: "id<lungime_id><id>"
    -> lungimea id este intotdeauna pe un bit, deoarece este intre 1 si 6

2. Mesaje de tip "ss"
    Acestea reprezinta mesajele ce contin o comanda de tip subscribe.

    Forma mesaj: "ss<lungime_topic>!<sf><topic>
    Lungimea topicului si topicul trebuiesc separate de ! pentru ca lungimea topicului poate fi mai mare de 9.
    Bit-ul de SF este inclus in mesajul trimis, insa serverul nu are implementata aceasta functionalitate (sorry!). Totusi, protocolul
acopera aceasta informatie ce trebui trimisa.

3. Mesaje de tip "us"
    Acestea reprezinta mesajele ce contin o comanda de tip unsubscribe.

    Forma mesaj: "us<lungime_topic>!<topic>
    Lungimea topicului si topicul trebuiesc separate de ! pentru ca lungimea topicului poate fi mai mare de 9.

Mesaje primite de server:
    Serverul primeste mesaje ori de tipul "id", "ss" si "us"
    Nu am tratat in server trunchierea mesajelor deoarece un mesaj de la client intotdeauna o sa se
incadreze in cei BUFLEN (264) biti cititi din socket.
    Cazul de concatenarea este trata asemanator ca in client, folosit lenght-ul din header.

Last Notes:
    Sunt constienta ca modul in care trimit mesaje de la client este foarte diferit de modul in care trimit mesaje de la server. 
Am implementat mai intai clientul, la un moment in care sincer nu ma pricepeam asa de mult cu memcpy cu structuri si de aceea totul este un string.

    Local, imi trec toate testele in afara de c2_restart_sf si de quick_flow (primesc eroare la quick_flow).

    Am folosit o biblioteca externa de liste: 
Credits: https://gist.github.com/System-Glitch/59c9f0b5b3b31fdc4ea2108160f40276
