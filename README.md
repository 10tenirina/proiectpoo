# Rule of Thirds Analyzer

Proiect realizat în cadrul cursului de **Programare Orientată pe Obiecte** (Facultatea de Matematică și Informatică, UniBuc).

## Descriere

**Rule of thirds** este una dintre cele mai folosite tehnici de compoziție în cinematografie și fotografie. Ideea e simplă: împarți cadrul în 9 părți egale cu două linii orizontale și două verticale, și plasezi elementele importante ale scenei pe aceste linii sau la intersecțiile lor — numite **power points**. Rezultatul e o imagine mai dinamică și mai naturală decât dacă ai centra subiectul.

Programul analizează cadre cinematografice și evaluează cât de bine respectă elementele din scenă rule of thirds.
Fiecare element focal (actor, recuzită, decor, sursă de lumină) este reprezentat printr-un bounding box, iar programul
calculează un scor de compoziție bazat pe distanța față de power points, ponderat cu importanța fiecărui element. Mai
mult, scorul poate fi reevaluat prin lentila unui **stil cinematografic** ales (Hollywood clasic, Wes Anderson,
documentar, cinematic neutru).

## Format fișiere de date

**`scena.txt`** — o scenă cu mai multe cadre:

```
<titlu_scena> <numar_cadre>
 
<titlu_cadru_1> <latime> <inaltime> <numar_subiecte>
<numar_subiecte> linii de subiecte (vezi mai jos)
 
<titlu_cadru_2> <latime> <inaltime> <numar_subiecte>
...
```

**`cadru_test.txt`** — un singur cadru, fără header de scenă, restul identic.

**Linie de subiect** (token-cu-token, separate prin spațiu):

```
<denumire> <x> <y> <latime> <inaltime> <Tip> <importanta> <camp_specific>
```

| Tip           | `camp_specific`                                           |
|---------------|-----------------------------------------------------------|
| `Actor`       | `stanga` / `dreapta` / `camera` (direcția privirii)       |
| `Recuzita`    | `static` / `dinamic` (mobilitate)                         |
| `Decor`       | `arhitectural` / `mobilier` / `vegetal` / ... (tip decor) |
| `SursaLumina` | `laterala` / `frontala` / `contra` (direcția luminii)     |

- `x`, `y` = colțul stânga-sus al bounding box-ului (în pixeli)
- `latime`, `inaltime` = dimensiunile bounding box-ului
- `importanta` = întreg în `[1, 10]`, ponderează subiectul în scorul cadrului
  Vezi `assets/scena.txt` și `assets/cadru_test.txt` pentru exemple complete.

## Mod interactiv

Programul citește comenzi din `std::cin`. Le poți tasta direct sau le pui în `tastatura.txt` și rulezi cu redirectare:

```sh
./oop < tastatura.txt
```

### Comenzi (o comandă pe linie)

| Comandă         | Efect                                                                                                                                  |
|-----------------|----------------------------------------------------------------------------------------------------------------------------------------|
| `1`             | Raport scena: scor și interpretare pe fiecare cadru, scor mediu, cadru recomandat.                                                     |
| `2`             | Clasament cadrelor ordonate descrescător după scor, cu tipul de compoziție.                                                            |
| `3 i j`         | Compară side-by-side cadrele `i` și `j` (scor, tip compoziție, echilibru vizual).                                                      |
| `4 i`           | Analiză detaliată a cadrului `i`: scorul fiecărui subiect + sfat, suprapuneri, tip compoziție, echilibru.                              |
| `5 i <subiect>` | Adaugă un subiect în cadrul `i`. `<subiect>` în același format ca în fișiere (vezi mai sus). Declanșează automat observatorii atașați. |
| `6 i`           | Evaluează cadrul `i` cu toate cele 4 stiluri cinematografice și afișează scorurile comparativ.                                         |
| `7 nume_stil`   | Schimbă stilul global al sesiunii. Stiluri disponibile: `cinematic`, `hollywood_clasic`, `wes_anderson`, `documentar`.                 |
| `8`             | Statistici peste scorurile cadrelor din scenă: număr, min, max, medie, mediană.                                                        |
| `0`             | Ieșire din modul interactiv.                                                                                                           |

**Exemplu pentru `5 i <subiect>`:**

```
5 0 Reflector 640 360 80 120 SursaLumina 6 laterala
```

**Observații:**

- Indecșii cadrelor încep de la `0`.
- Un index invalid produce un mesaj de eroare, nu oprește programul.
- La `stdin` gol, programul iese imediat (nu blochează CI-ul).
- Opțiunea `5` declanșează observatorii instalați la pornire: un logger pe `stderr` și un monitor de compoziție pe
  `stdout`.

### Exemplu de `tastatura.txt`

```
1
2
6 0
6 1
7 wes_anderson
8
5 0 Reflector 640 360 80 120 SursaLumina 6 laterala
4 0
0
```

## Voi primi notă pentru că am pus titlu și descriere

### Folosiți template-ul corespunzător grupei voastre!

| Laborant  | Link template                                |
|-----------|----------------------------------------------|
| Dragoș B  | https://github.com/Ionnier/oop-template      |
| Tiberiu M | https://github.com/MaximTiberiu/oop-template |
| Marius MC | https://github.com/mcmarius/oop-template     |

### Important!

Aveți voie cu cod generat de modele de limbaj la care nu ați contribuit semnificativ doar dacă documentați riguros acest proces.
Codul generat pus "ca să fie"/pe care nu îl înțelegeți se punctează doar pentru puncte bonus, doar în contextul
în care oferă funcționalități ajutătoare și doar dacă are sens.

Codul din proiect trebuie să poată fi ușor de înțeles și de modificat de către altcineva. Pentru detalii, veniți la ore.

O cerință nu se consideră îndeplinită dacă este realizată doar prin cod generat.

- **Fără cod de umplutură/fără sens!**
- **Fără copy-paste!**
- **Fără variabile globale!**
- **Fără atribute publice!**
- **Pentru T2 și T3, fără date în cod!** Datele vor fi citite din fișier, aveți exemple destule.
- **Obligatoriu** fișiere cu date mai multe din care să citiți, obligatoriu cu biblioteci externe: fișiere (local sau server) sau baze de date
- obligatoriu (TBD) să integrați cel puțin două biblioteci externe pe lângă cele pentru stocare

### Tema 0

- [ ] Nume proiect (poate fi schimbat ulterior)
- [ ] Scurtă descriere a temei alese, ce v-ați propus să implementați

## Tema 1

#### Cerințe
- [ ] definirea a minim **3-4 clase** folosind compunere cu clasele definite de voi; moștenirile nu se iau în considerare aici
- [ ] constructori de inițializare cu parametri pentru fiecare clasă
- [ ] pentru o aceeași (singură) clasă: constructor de copiere, `operator=` de copiere, destructor
<!-- - [ ] pentru o altă clasă: constructor de mutare, `operator=` de mutare, destructor -->
<!-- - [ ] pentru o altă clasă: toate cele 5 funcții membru speciale -->
- [ ] `operator<<` pentru **toate** clasele pentru afișare (`std::ostream`) folosind compunere de apeluri cu `operator<<`
- [ ] cât mai multe `const` (unde este cazul) și funcții `private`
- [ ] implementarea a minim 3 funcții membru publice pentru funcționalități netriviale specifice temei alese, dintre care cel puțin 1-2 funcții mai complexe
  - nu doar citiri/afișări sau adăugat/șters elemente într-un/dintr-un vector
- [ ] scenariu de utilizare **cu sens** a claselor definite:
  - crearea de obiecte și apelarea tuturor funcțiilor membru publice în main
  - vor fi adăugate în fișierul `tastatura.txt` DOAR exemple de date de intrare de la tastatură (dacă există); dacă aveți nevoie de date din fișiere, creați alte fișiere separat
- [ ] minim 52-60% din codul propriu să fie C++, `.gitattributes` configurat corect
- [ ] tag de `git`: de exemplu `v0.1`
- [ ] serviciu de integrare continuă (CI) cu **toate bifele**; exemplu: GitHub Actions
- [ ] code review #1 2 proiecte

## Tema 2

#### Cerințe
- [ ] separarea codului din clase în `.h` (sau `.hpp`) și `.cpp`
- [ ] moșteniri:
  - minim o clasă de bază și **3 clase derivate** din aceeași ierarhie; cele 3 derivate moștenesc aceeași clasă de bază
  - ierarhia trebuie să fie cu bază proprie, nu derivată dintr-o clasă predefinită
  - [ ] funcții virtuale (pure) apelate prin pointeri de bază din clasa care conține atributul de tip pointer de bază
    - minim o funcție virtuală va fi **specifică temei** (i.e. nu simple citiri/afișări sau preluate din biblioteci i.e. draw/update/render)
    - constructori virtuali (clone): sunt necesari, dar nu se consideră funcții specifice temei
    - afișare virtuală, interfață non-virtuală
  - [ ] apelarea constructorului din clasa de bază din constructori din derivate
  - [ ] clasă cu atribut de tip pointer la o clasă de bază cu derivate; aici apelați funcțiile virtuale prin pointer de bază, eventual prin interfața non-virtuală din bază
    - [ ] suprascris cc/op= pentru copieri/atribuiri corecte, copy and swap
    - [ ] `dynamic_cast`/`std::dynamic_pointer_cast` pentru downcast cu sens
    - [ ] smart pointers (recomandat, opțional)
- [ ] excepții
  - [ ] ierarhie proprie cu baza `std::exception` sau derivată din `std::exception`; minim **3** clase pentru erori specifice distincte
    - clasele de excepții trebuie să trateze categorii de erori distincte (exemplu de erori echivalente: citire fișiere cu diverse extensii)
  - [ ] utilizare cu sens: de exemplu, `throw` în constructor (sau funcție care întoarce un obiect), `try`/`catch` în `main`
  - această ierarhie va fi complet independentă de ierarhia cu funcții virtuale
- [ ] funcții și atribute `static`
- [ ] STL
- [ ] cât mai multe `const`
- [ ] funcții *de nivel înalt*, de eliminat cât mai mulți getters/setters/funcții low-level
- [ ] minim 75-78% din codul propriu să fie C++
- [ ] la sfârșit: commit separat cu adăugarea unei noi clase derivate fără a modifica restul codului, **pe lângă cele 3 derivate deja adăugate** din aceeași ierarhie
  - noua derivată nu poate fi una existentă care a fost ștearsă și adăugată din nou
  - noua derivată va fi integrată în codul existent (adică va fi folosită, nu adăugată doar ca să fie)
- [ ] tag de `git` pe commit cu **toate bifele**: de exemplu `v0.2`
- [ ] code review #2 2 proiecte

## Tema 3

#### Cerințe
- [ ] 2 șabloane de proiectare (design patterns)
- [ ] o clasă șablon cu sens; minim **2 instanțieri**
  - [ ] preferabil și o funcție șablon (template) cu sens; minim 2 instanțieri
- [ ] minim 80-90% din codul propriu să fie C++
<!-- - [ ] o specializare pe funcție/clasă șablon -->
- [ ] tag de `git` pe commit cu **toate bifele**: de exemplu `v0.3` sau `v1.0`
- [ ] code review #3 2 proiecte

## Instrucțiuni de compilare

Proiectul este configurat cu CMake.

Instrucțiuni pentru terminal:

1. Pasul de configurare
```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
# sau ./scripts/cmake.sh configure
```

Sau pe Windows cu GCC folosind Git Bash:
```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -G Ninja
# sau ./scripts/cmake.sh configure -g Ninja
```

Pentru a configura cu ASan, avem opțiunea `-DUSE_ASAN=ON` (nu merge pe Windows cu GCC):
```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DUSE_ASAN=ON
# sau ./scripts/cmake.sh configure -e "-DUSE_ASAN=ON"
```


La acest pas putem cere să generăm fișiere de proiect pentru diverse medii de lucru.


2. Pasul de compilare
```sh
cmake --build build --config Debug --parallel 6
# sau ./scripts/cmake.sh build
```

Cu opțiunea `parallel` specificăm numărul de fișiere compilate în paralel.


3. Pasul de instalare (opțional)
```sh
cmake --install build --config Debug --prefix install_dir
# sau ./scripts/cmake.sh install
```

Vezi și [`scripts/cmake.sh`](scripts/cmake.sh).

Observație: folderele `build/` și `install_dir/` sunt adăugate în fișierul `.gitignore` deoarece
conțin fișiere generate și nu ne ajută să le versionăm.


## Instrucțiuni pentru a rula executabilul

Există mai multe variante:

1. Din directorul de build (implicit `build`). Executabilul se află la locația `./build/oop` după ce a fost rulat pasul de compilare al proiectului (`./scripts/cmake.sh build` - pasul 2 de mai sus).

```sh
./build/oop
```

2. Din directorul `install_dir`. Executabilul se află la locația `./install_dir/bin/oop` după ce a fost rulat pasul de instalare (`./scripts/cmake.sh install` - pasul 3 de mai sus).

```sh
./install_dir/bin/oop
```

3. Rularea programului folosind Valgrind se poate face executând script-ul `./scripts/run_valgrind.sh` din rădăcina proiectului. Pe Windows acest script se poate rula folosind WSL (Windows Subsystem for Linux). Valgrind se poate rula în modul interactiv folosind: `RUN_INTERACTIVE=true ./scripts/run_valgrind.sh`

Implicit, nu se rulează interactiv, iar datele pentru `std::cin` sunt preluate din fișierul `tastatura.txt`.

```sh
RUN_INTERACTIVE=true ./scripts/run_valgrind.sh
# sau
./scripts/run_valgrind.sh
```

4. Pentru a rula executabilul folosind ASan, este nevoie ca la pasul de configurare (vezi mai sus) să fie activat acest sanitizer. Ar trebui să meargă pe macOS și Linux. Pentru Windows, ar merge doar cu MSVC (nerecomandat).

Comanda este aceeași ca la pasul 1 sau 2. Nu merge combinat cu Valgrind.

```sh
./build/oop
# sau
./install_dir/bin/oop
```

## License

The project is licensed under [AGPLv3](LICENSE).

The [template repository](https://github.com/mcmarius/oop-template) itself is licensed under [Unlicense](LICENSE.template).

## Resurse

- adăugați trimiteri **detaliate** către resursele externe care v-au ajutat sau pe care le-ați folosit
