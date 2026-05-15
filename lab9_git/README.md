# Lab 9
На оценку 3
<img width="771" height="442" alt="1" src="https://github.com/user-attachments/assets/9ba4a337-b4e3-43af-9eac-db9207d34453" />
1) Использовали git status, чтобы узнать на какой ветке находимся
2) Создали файл sort.c и вставили туда пузырьковую функцию

<img width="807" height="863" alt="3" src="https://github.com/user-attachments/assets/3bf8e1ca-4bee-40ee-a8ef-80e7136e00fd" />

4) Далее проверили как сейчас выглядит git status
5) Добавили файл в область add
6) Снова проверили проверили git status
7) Закоментили файл в репозиторий
8) Проверили как сейчас выглядит git status
9) Проверили log
10) Добавили комментарий // First modification в sort.c
<img width="773" height="814" alt="4" src="https://github.com/user-attachments/assets/97fbbc30-5254-40ff-96ed-a4619f1a3d51" />
11) Далее снова делаем git add sort.c и git commit -m "Add first comment", чтобы история была чистой и понятной
<img width="762" height="800" alt="5" src="https://github.com/user-attachments/assets/42160453-f919-4a90-9453-9adf17e243ce" />
12) Проверили как сейчас выглядит status и журнал log
13) Доюавили в stage и закоммители последние изменения
14) Запушили на удаленный репо
15) Создали новую ветку mybranch
16) С помощью git switch mybranch, чтобы переключиться на новую ветку
17) Создали файл с именем file1.txt и своим именем
18) Добавили файл и закомитили его изменения
<img width="822" height="844" alt="6" src="https://github.com/user-attachments/assets/91b2dca9-80a7-4ac6-afc1-1f5401344885" />
19) Использовали git log --oneline --graph, чтобы увидеть, что наша ветка указывает на новый коммит
20) Вернулись к ветке main
21) Создали файл file2.txt и закоммитили его
22) Переключились на ветку mybranch
23) Файл file2.txt пропал
24) Использовали git diff mybranch main, чтобы увидеть разницу между двумя ветками
<img width="710" height="798" alt="7" src="https://github.com/user-attachments/assets/72439a77-dfa1-421b-ada9-095686bb2b35" />
25) Закоммитили и запушили на удаленный репо ветку mybranch
.
.
.
.
На оценку 4
.
.
<img width="588" height="700" alt="8(на 4)" src="https://github.com/user-attachments/assets/601bdcab-9f1b-4363-af11-a6501b9e0f3c" />
1) Переключаемся на ветку mybranch
2) Перезапишем содержимое в sort.c, добавив функцию main(), в которой будет объявлен массив из нескольких чисел и вызвана функция сортировки для массива
<img width="519" height="868" alt="9" src="https://github.com/user-attachments/assets/e4acf744-5ddc-4d16-b304-f2ecba966273" />
3) Смотрим, что говорит git diff
<img width="559" height="904" alt="10" src="https://github.com/user-attachments/assets/5c302d04-613a-4de4-b196-4b589a08358e" />
4) Проверяем git diff --staged
<img width="574" height="901" alt="11" src="https://github.com/user-attachments/assets/8cad8977-c89e-42d2-835b-1aebf7d1dd5c" />
5) Удаляем любое из чисел в массиве sort.c
6) Проверяем git diff, команда показывает новые изменения, которые сделали после добавления файла в staging
7) Далее проверяем git diff --staged, эта команда по-прежнему показывает старые изменения (с тремя числами), потому что мы не обновляли staging после последнего изменения
<img width="704" height="853" alt="12" src="https://github.com/user-attachments/assets/6dad925a-4dc9-424e-aa64-f3188c0325a8" />
8) Запустили git status и обратим внимание, что sort.c присутсвует дважды в выводе
9) Запустили git restore --staged sort.c, чтобы отменить индексацию изменения
10) git status говорит, что все изменения находятся только в рабочей директории
11) Индексировали изменения и сделали коммит
<img width="682" height="883" alt="13" src="https://github.com/user-attachments/assets/8328e0bc-be24-453b-a354-f93e3613d8f5" />

12) Проверили как выглядит журнал
13) Добавили в sort.c в main() printf(“hello git\n”)

<img width="691" height="840" alt="14" src="https://github.com/user-attachments/assets/87d17874-6aa4-43d5-b773-0ef15db407be" />

14) git status показал, что файл sort.c модифицирован, но не добавлен в staging
15) Запустили git restore sort.c
16) printf(“hello git\n”) пропал и файл вернулся к состоянию, которое было зафиксировано в последнем коммите

<img width="804" height="867" alt="15" src="https://github.com/user-attachments/assets/eddd399d-2a3f-4f9c-b9f0-6d30a95f1f4d" />

18) git status показал nothing to commit, working tree clean, так как все изменения отменены
19) Запушили на удаленную репо ветку

20) Создали файл greeting.txt, проиндексировали его и закоммитили с 
сообщением “Add file greeting.txt”
21) Добавили слово hello, индексировали его и коммитили его с текстом "Add content to greeting.txt"
22) Создаем ветку с именем feature/uppercase
23) Переключаемся на эту ветку
24) Отредактировали greeting.txt, чтобы он содержал приветствие в верхнем 
регистре (HELLO)


<img width="826" height="498" alt="16" src="https://github.com/user-attachments/assets/1a8a821d-7472-4b9f-8bff-2f22719bed9c" />

25) Добавили файл greeting.txt и закоммитили его
26) git branch показывает три ветки: feature/uppercase, main и mybranch. Звездочка (*) указывает на текущую ветку
27) переключились на главную ветку main

<img width="708" height="514" alt="17" src="https://github.com/user-attachments/assets/3293adef-bc16-4875-ad5c-976daa1b44c1" />

28) Использовали cat, чтобы увидеть содержимое файла greetings.txt
29) Сравнили ветки
30) Объединили ветки
31) Удалили ветку с заглавными буквами (feature/uppercase)
32) Смержили ветку mybranch
33) git log --oneline --graph –all показывает, как развивались ветки и когда они были смержены
34) Запушили изменения ветки на удаленный репо
35) запушили документ с результатами работы
<img width="591" height="244" alt="18" src="https://github.com/user-attachments/assets/e04c1f18-1437-4a14-8123-98fd342d686c" />
.
.
.
.
На оценку 5
.
.
.

<img width="838" height="888" alt="19(на 5)" src="https://github.com/user-attachments/assets/47530204-53a8-4cf6-8fea-35ac505dc5e4" />

1) Создали ветку branch1 и переключились на нее
2) Выполнили команду echo "This is a relevant fact" > file.txt
3) Закоммитили изменение
4) Переключились на главную ветку и выполнили команду echo "This is an indispensable truth!" > file.txt
5) Закоммитили изменения
6) git log --oneline --graph –all  показывает, что у нас есть две ветки (main и branch1) с разными коммитами, и что они имеют общего предка
7) Использовали команду git merge чтобы смержить ветку branch1 в master(получили конфликт)

<img width="789" height="511" alt="20" src="https://github.com/user-attachments/assets/f5f9df0b-ef64-4902-aabe-668111a4415d" />

8) git status сообщает, что у нас есть незавершенные пути и что файл file.txt имеет конфликт типа both added. Это означает, что обе ветки создали этот файл.
9) Посмотреть содержимое файла file.txt и в текстовом 
редакторе исправили конфликт

<img width="852" height="864" alt="21" src="https://github.com/user-attachments/assets/7056f6e5-c2af-426f-8c6e-6bd372111374" />

10) git log --oneline –graph показывает какие коммиты уже есть в ветке main
11) запушили изменения
12) Находясь в ветке main создали файл mergesort.py с содержимым из 
base.py 
13) Проиндексировали файл и закоммитили его
14) Создали новую ветку Mergesort-Impl и переключились на нее

<img width="841" height="858" alt="22" src="https://github.com/user-attachments/assets/35a83141-5492-411c-ad06-18489d5a1752" />

15) Содержимое файла mergesort.py заменили на код из righty.py
16) Коммитим изменения
17) Переключились на main и меняем все содержимое mergesort.py на 
lefty.py
18) Коммитим изменения
19) git log --oneline –graph показывает, что у есть две ветки (main и Mergesort-Impl) с разными коммитами, и что они имеют общего предка
20) смержили Mergesort-Impl в master
11) После исправления всех merge конфликтов запушили в main
изменения
12) Запушили документ с результатами работы

<img width="902" height="510" alt="23" src="https://github.com/user-attachments/assets/ba2314a4-004f-4fbf-bf5a-0b2b8c6333cd" />

