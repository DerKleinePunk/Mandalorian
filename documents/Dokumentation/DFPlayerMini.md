source https://wolles-elektronikkiste.de/dfplayer-mini-ansteuerung-mit-dem-arduino

1. Standardverzeichnisse
Man darf 99 Standardverzeichnisse erstellen, die nach dem Schema 01, 02, 03 …. 99 benannt werden müssen. In jedes Verzeichnis passen bis zu 3000 Dateien, allerdings lassen sich nur die ersten 255 direkt über die seriellen Kommandos ansprechen. Das heißt, dass ein Kommando wie „spiel Datei Nummer 627“ in den Standardverzeichnissen nicht möglich ist und man sich mit „next“-Anweisungen durchhangeln müsste. Die Dateinamen müssen die Struktur 001xxxxx.mp3 (bzw. wav), 002xxxxx.mp3, ….. 255xxxxx.mp3 haben. Dabei ist „xxxxx“ frei wählbar und kann mehr als die hier angedeuteten 5 Stellen haben. Auch Leerzeichen sind erlaubt. 

2. Sonderverzeichnis „MP3“
Dieses Verzeichnis muss exakt als „MP3“ benannt werden.  Hier passen bis zu 3000 Dateien mit dem Namensschema 0001.mp3 (bzw. wav), 0002.mp3, ….. 3000.mp3 hinein. Im Gegensatz zum Hauptverzeichnis sind alle Dateien direkt ansprechbar und es gibt auch keine Probleme mit der Reihenfolge. 

2. Sonderverzeichnis „ADVERT“
Es gelten dieselben Regeln wie für das MP3 Verzeichnis. Das ADVERT Verzeichnis hat eine besondere Funktion. Spielt man eine Datei aus einem der anderen Verzeichnisse ab und ruft währenddessen eine Datei aus dem ADVERT Verzeichnis auf, so wird das Abspielen der ursprünglichen Datei lediglich unterbrochen. Nach dem Beenden der ADVERT Einspielung wird das ursprüngliche Stück fortgesetzt. Es ist wie eine Werbeeinblendung und so deutet es der Name des Verzeichnisses ja auch schon an.