<h1>C++ Structure Lister</h1>

<h2>Descrizione</h2>

Questo programma analizza un singolo file C++ (`inputfile.txt`) e ne ricostruisce la struttura,
**replicando il funzionamento del Solution Explorer di Visual Studio.**

<h2>Funzionalità</h2>

Estrae:
+ Macro anche complesse purché **definite nello stesso file**
+ Namespace
+ Classi e **classi template**
+ Funzioni e **funzioni template** (distinguendole da inizializzazioni per copia)
+ Variabili globali senza il loro valore di inizializzazione
+ `enum` ed `enum class` includendo gli elementi
+ strutture e **strutture template**

<h2>Funzionamento</h2>

Non è necessario alcun compilatore specifico (a parte quello per compilare questo programma stesso): l’analisi è implementata da zero.

<h3>Passaggi principali:</h3>

1) Rimozione di spazi inutili, commenti, e caratteri \ per righe spezzate.
2) Eliminazione dei commenti, distinguendoli da stringhe/char usando la parità di virgolette e apostrofi.
3) Rimozione di virgolette e apostrofi, mantenendo eventualmente il carattere L per stringhe wide.
4) Estrazione e salvataggio delle definizioni di macro; rimozione di tutte le direttive rimanenti.
5) Eliminazione delle macro senza valore.
6) Separazione accurata delle stringhe delle macro per facilitare la sostituzione.
7) Sostituzione delle macro nel testo.
8) Pulizia da spazi bianchi superflui e rimozione di `__pragma`.
9) Eliminazione del contenuto delle funzioni, riconosciute tramite parentesi tonde.
    + Rimozione delle inizializzazioni per copia nei costruttori.
    + Rimozione delle inizializzazioni delle variabili globali.
10) Scrittura della lista delle macro su file.
11) Tokenizzazione del testo su `;` e `{}`.
12) Espansione delle inizializzazioni multiple su una riga singola.
13) Rimozione dell’inizializzazione uniforme (`{}`).
14) *(In fase di sviluppo: ulteriori analisi e raffinamenti)*

<h2>Limitazioni</h2>

+ Il programma può crashare se il file non è stato compilato prima per verificare l'assenza di errori sintattici.
+ **Lo sviluppo non è ancora completo.**

<h2>Autore</h2>

Creato da **Emanuele Cantarelli**
