# Image-Editor
Cucu Viorel-Cosmin 314CA

    Tema #3 - Editor de imagini

## MAIN
In main folosesc o stuctura repetitiva de tip while ce se opreste cand 
primeste comanda "EXIT". Citesc liniile introduse de la tastatura cu fgets
si cu ajutorul functiei "strtok" impart linia in subsiruri. Primul subsir 
reprezinta comanda introdusa si apoi urmatorul/urmatoarele reprezinta 
parametrii acelei comenzi. Daca comanda si vreun parametru introdus nu este
corect se va afisa "Invalid command".

 ## LOAD <nume_fisier>
Deschid fisierul cu numele dat, iar in caz ca nu exista afisez "Failed to 
load <nume_fisier>". Daca fisierul exista, citesc din el caracteristicile 
imaginii cu exceptia comentariului. Iar pixelii ii citesc cu ajutorul
functiilor "read_matrix_a" si "read_matrix_b" in functie de tipul imaginii.
inca din inceputul programului am definit o structura "image" ce contine
toate informatiile despre imagine, iar pixelii i-am retinut intr-o matrice 3d
pentru ca in cazul fotografiilor de tip PPM retin cele 3 canale de culoare pe
un pixel. Dupa ce am citit imaginea, fac selectia intregii imagini dandu-le 
valori coordonatelor "x1,y1,x2,y2" si fac variabila "load" 1 pentru a stii ca
imaginea s-a incarcat in memorie cu succes, apoi inchid fisierul cu "fclose" .

## SELECT <x1> <y1> <x2> <y2>
Daca imaginea nu este incarcata in memorie, afisez "No image loaded", iar
daca este incarcata, verific daca coordonatele date sunt corecte, apoi schimb
coordonatele imaginii cu cele date. Daca comanda este "SELECT ALL" atunci le 
dau coordonatelor imaginii valorile 0,0,latime_imagine,inaltime_imagine.

## HISTOGRAM <numar_stelute> <numar_intervale>
Daca imaginea nu este incarcata in memorie, afisez "No image loaded", iar
daca este incarcata, verific  parametrii. Daca sunt corecti, calculez 
histograma doar pentru imaginile de tip PGM. Daca imaginea este de tip PPM
afisez "Black and white image needed". Cei 2 parametrii reprezinta numarul 
maxim de stelute ce se vor afisa si numarul de intervale. Calculez intensitatea
tuturor pixelilor din fiecare intreval, aceasta valoare o pun in vectorul de
frecventa "frecv_grup". Apoi calculez maximul din vectorul de frecventa si 
numarul de stelute ce le voi afisa asa cum s-a specificat.

## CROP 
Daca imaginea nu este incarcata in memorie, afisez "No image loaded", iar
daca este incarcata, foc o noua imagine cu dimensiunile egale cu coordonatele
imaginii (x1,y1,x2,y2) si copiez pixelii din imaginea initiala in noua imagine.
Eliberez memoira pentru imaginea initiala si ii dau adresa imaginii noi si 
modific valorile coordonatelor imaginii.

## EQUALIZE
Daca imaginea nu este incarcata in memorie, afisez "No image loaded", iar
daca este incarcata, verific daca aceasta este de tip PGM, daca nu este afisez
"Black and white image needed". Calculez frecventa fiecarui pixel si o pun
intr-un vector "frecv". Calculez in vectorul "H" folosind formula data valoarea
noua a fiecarui pixel ce avea ca valoare pozitia la care se afla H. In final 
schimb valorile pixelilor din imaginea initiala cu cele din vectorul "H".

## SAVE <nume_fisier> (<ascii>)
Daca imaginea nu este incarcata in memorie, afisez "No image loaded", iar
daca este incarcata, verific daca parametrii sunt corecti. Poate sa fie doar 
un parametru acesta reprezentand numele fisierului in care se va salva imaginea
sau 2 parametrii reprezentand numele fisierului si "ascii"(dacă se doreste ca
imaginea sa fie salvata in format ASCII). Deschid fisierul cu numele dat si 
scriu in el caracteristicile imaginii, iar pixelii ii scriu cu ajutorul 
functiilor fprintf sau fwrite in functie de tipul imaginii. Apoi inchid fisierul
cu "fclose".

## APPLY <nume_efect>
Daca imaginea nu este incarcata in memorie, afisez "No image loaded", iar
daca este incarcata, verific daca imaginea este de tip PPM, daca nu este afisez
"Easy, Charlie Chaplin". Apoi verific numele efectului, daca este corect creez 
o noua imagine ce are dimensiunile cu imaginea selectata(cu coordonatele 
x1,y1,x2,y2). Verific daca toti pixelii au vecini, iar daca nu au ii las 
neschimbati. Aplic efectul dorit si apoi egalez valoarea pixelilor din imaginea
initiala cu cei din copie. Eliberez memoria pentru imaginea creata si 
afisez un mesaj de succes.

## ROTATE <unghi>
Daca imaginea nu este incarcata in memorie, afisez "No image loaded", iar
daca este incarcata, verific daca unghiul este corect, daca nu este afisez 
"Unsupported rotation angle". Apoi iau 2 cazuri: daca imaginea selectata este
cat imaginea initiala sau daca imaginea selectata este o parte din imaginea si
are forma de patrat. In ambele cazuri observ valoarea unghiului si in functie
de aceasta aplic rotatia dorita deoarece la fiecare rotatie imaginea se va roti
cu 90 de grade. In primul caz creez o imagine ce are dimensiunile imaginii pe 
dos si aplic rotatia. Eliberz memoria pentru imaginea initala si ii dau adresa
imaginii noi, totodata actualizez si caracteristicile imaginii. 
    In al doilea caz creez o imagine ce are dimensiunile cat acel patrat si ii
aplic rotatia. Ii dau imaginii initale valorile imaginii noi de pe acea 
portiune si eliberez memoria pentru imaginea noua.

## EXIT
Daca imaginea nu este incarcata in memorie, afisez "No image loaded", iar
daca este incarcata, eliberez memoria pentru imaginea initiala si inchid 
programul.
