#include <iostream>
#include <fstream>
#include <string>
using namespace std;
class HashTable {
  const int FREE = 0; // свободная ячейка
  const int BUSY = 1; // занятая ячейка
  const int REMOVED = 2; // удалённая ячейка
  struct Node {
    int value; // значение элемента
    string key; // ключ элемента
    int state; // состояние ячейки
    int count; // количество обращений к ячейке
  };
  int capacity; // максимальная вместимость таблицы
  int size;
  Node *segments; // массив списков - сегменты таблицы
  int (*h)(const string& key); // указатель на хеш-функцию
  public:
    HashTable(int n, int (*function)(const string& key)); // конструктор из размера таблицы и указателя на хеш функцию
    void Insert(const string& key, int value); // добавление элемента в таблицу
    void Print(); // вывод на экран содержимого таблицы
    bool Remove(const string& key); // удаление элемента по ключу
    void FindByKey(const string& key); // поиск и вывод элементов по ключу
    void FindBySegment(int segment); // поиск и вывод элементов по сегменту
    void Save(const string& path); // сохранение таблицы в файл
    ~HashTable(); // деструктор таблицы (очистка памяти)
};
HashTable::HashTable(int n, int (*function)(const string& key)) {
  capacity = n;
  size = 0;
  segments = new Node[n]; // выделяем память под элементы
  // обнуляем все списки
  for (int i = 0; i < n; i++) {
    segments[i].state = FREE;
    segments[i].count = 0;
  }
  h = function;
}
void HashTable::Insert(const string& key, int value) {
  int sequenceLength = 0; // начальная длина пробной последовательности равна нулю
  int hash = h(key); // получаем хеш от ключа
  while (sequenceLength < capacity) {
    int index = (hash + sequenceLength * 1) % capacity;
    segments[index].count++; // увеличиваем число обращений на 1
    if (segments[index].state != BUSY) { // если нашли незанятую ячейку
      segments[index].key = key; // сохраняем ключ
      segments[index].value = value; // сохраняем значение
      segments[index].state = BUSY; // ячейка становится занятой
      size++; // увеличиваем счётчик числа элементов
      return; // выходим
      }
    sequenceLength++; // увеличиваем длину пробной последовательности
    }
  // прошли всю таблицу
  throw string("Unable to insert value with this key"); // бросаем исключение
  }
// вывод таблицы на экран
void HashTable::Print() {
  for (int i = 0; i < capacity; i++) {
    if (segments[i].state != BUSY) // если нет элемента
    continue; // то пропускаем
    cout << "[" << i << "]: "; // выводим номер ячейки
    cout << segments[i].value << "(" << segments[i].key << ") "; // выводим
    содержимое ячейки
    cout << endl; // переходим на новую строку
  }
}
// удаление элемента по ключу
bool HashTable::Remove(const string& key) {
  int sequenceLength = 0; // начальная длина пробной последовательности равна нулю
  int hash = h(key); // получаем хеш от ключа
  while (sequenceLength < capacity) {
    int index = (hash + sequenceLength * 1) % capacity;
    segments[index].count++; // увеличиваем число обращений на 1
    // если нашли занятую нужным ключом ячейку
    if (segments[index].state == BUSY && segments[index].key == key) {
      segments[index].state = REMOVED; // помечаем её как удалённую
      size--; // уменьшаем счётчик числа элементов
      return true; // возвращаем истину
      }
    // если нашли свободную ячейку
    if (segments[index].state == FREE)
      return false; // возвращаем ложь
    sequenceLength++; // увеличиваем длину пробной последовательности
  }
  return false; // не нашли во всей таблице, возвращаем ложь
}
// поиск и вывод элементов по ключу
void HashTable::FindByKey(const string& key) {
  int sequenceLength = 0; // начальная длина пробной последовательности равна нулю
  int hash = h(key); // получаем хеш от ключа
  while (sequenceLength < capacity) {
    int index = (hash + sequenceLength * 1) % capacity; // ищем индекс элемента
    segments[index].count++; // увеличиваем число обращений на 1
    // если нашли занятую клетку с нужным ключом
    if (segments[index].state == BUSY && segments[index].key == key) {
      cout << "Value by key '" << key << "': " << segments[index].key << " - "
      << segments[index].value << endl;
      return; // значит нашли
      }
    // если нашли свободную ячейку
    if (segments[index].state == FREE)
      return; // значит нет элемента
    sequenceLength++; // увеличиваем длину пробной последовательности
    }
}
// поиск и вывод элементов по сегменту
void HashTable::FindBySegment(int segment) {
  // если сегмент задан некорректно
  if (segment < 0 || segment >= capacity) {
    cout << "Incorrect segment!" << endl; // сообщаем об ошибке
    return; // и выходим
    }
  // если элемента в сегменте нет
  if (segments[segment].state != BUSY) {
    cout << "No value in this segment" << endl; // сообщаем, что нет элементов
    return;
    }
  // выводим элемент в данном сегменте
  cout << "Value by segment " << segment << ": " << segments[segment].key << " - " <<
  segments[segment].value << endl;
}
// сохранение таблицы в файл
void HashTable::Save(const string& path) {
  ofstream f;
  f.open(path.c_str()); // создаём файл и ткрываем его на запись
  for (int i = 0; i < capacity; i++) {
    if (segments[i].state != BUSY) {
      f << "" << endl;
      continue;
    }
    f << i << ";" << segments[i].count << endl; // выводим количество обращений
    к ячейке
    }
  f.close(); // закрываем файл
}
// деструктор таблицы (очистка памяти)
HashTable::~HashTable() {
  delete[] segments; // освобождаем память от массива сегментов
}
// функция, получающая хеш по ключу
int getHash(const string& key) {
  int hash = 0;
  for (int i = 0; i < 6; i++)
  hash += key[i] * key[i];
  return hash % 2000;
  }
// проверка ключа на корректность
bool checkKey(const string& key) {
  if (key.length() != 6)
    return false;
  if (key[0] < '0' || key[0] > '9')
    return false;
  if (key[1] < '0' || key[1] > '9')
    return false;
  if (key[2] < '0' || key[2] > '9')
    return false;
  if (key[3] < 'A' || key[3] > 'Z')
    return false;
  if (key[4] < '0' || key[4] > '9')
    return false;
  if (key[5] < '0' || key[5] > '9')
    return false;
  return true;
}
string getRandomKey() {
  string key = "";
  key += '0' + rand() % ('9' - '0' + 1);
  key += '0' + rand() % ('9' - '0' + 1);
  key += '0' + rand() % ('9' - '0' + 1);
  key += 'A' + rand() % ('Z' - 'A' + 1);
  key += '0' + rand() % ('9' - '0' + 1);
  key += '0' + rand() % ('9' - '0' + 1);
  return key;
}
void PrintTable(HashTable& table) {
  table.Print();
}
void Insert(HashTable& table) {
  string key;
  int value;
  cout << "Enter key and value: ";
  cin >> key >> value;
  while (!checkKey(key)) {
    cout << "Incorrect key. Try again: ";
    cin >> key;
  }
  try {
    table.Insert(key, value);
    cout << "Inserted!" << endl;
  }
  catch (string error) {
    cout << error << endl;
  }
}
void Remove(HashTable& table) {
  string key;
  cout << "Enter key: ";
  cin >> key;
  while (!checkKey(key)) {
    cout << "Incorrect key. Try again: ";
    cin >> key;
  }
  if (table.Remove(key)) {
    cout << "Removed!" << endl;
  }
  else {
    cout << "No value with key '" << key << "'" << endl;
  }
}
void FindByKey(HashTable& table) {
  string key;
  cout << "Enter key: ";
  cin >> key;
  while (!checkKey(key)) {
    cout << "Incorrect key. Try again: ";
    cin >> key;
  }
  table.FindByKey(key);
}
void FindBySegment(HashTable& table) {
  int segment;
  cout << "Enter segment: ";
  cin >> segment;
  table.FindBySegment(segment);
}
void SaveToFile(HashTable& table) {
  string path;
  cout << "Enter path to file: ";
  cin >> path;
  table.Save(path);
  cout << "Saved!" << endl;
}
void FillRandom(HashTable& table) {
  int n;
  cout << "Enter n: ";
  cin >> n;
  try {
    for (int i = 0; i < n; i++) {
      string key = getRandomKey();
      table.Insert(key, rand() % 10000);
    }
  }
  catch (string error) {
    cout << error << endl;
  }
}
int main() {
  HashTable table(2000, getHash); // создаём таблицу с заданной функцией
  хеширования
   int item; // выбираемый пункт меню
   do {
     system("cls"); // очищаем экран
     // выводим меню
     cout << "What do you want to do?" << endl;
     cout << "1. Print table" << endl;
     cout << "2. Insert" << endl;
     cout << "3. Remove" << endl;
     cout << "4. Find by key" << endl;
     cout << "5. Find by segment" << endl;
     cout << "6. Save to file" << endl;
     cout << "7. Fill random" << endl;
     cout << "8. Quit" << endl;
     cout << ">";
     cin >> item; // считываем пункт меню
     // обрабатываем некорректный ввод
     while(item < 1 || item > 8) {
       cout << "Incorrect item. Try again: ";
       cin >> item; // считываем пункт меню заново
     }
     switch (item) {
       case 1:
       PrintTable(table);
       break;
     case 2:
       Insert(table);
       break;
     case 3:
       Remove(table);
       break;
     case 4:
       FindByKey(table);
       break;
     case 5:
       FindBySegment(table);
       break;
     case 6:
       SaveToFile(table);
       break;
     case 7:
       FillRandom(table);
       break;
     }
     if (item != 8) {
       system("pause"); // задерживаем экран
     }
   } while (item != 8); // повторяем, пока не решим выйти
   return 0;
}
