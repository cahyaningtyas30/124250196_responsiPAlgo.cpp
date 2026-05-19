#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

struct Bagasi {
    int flightTime;
    float berat;
};

// ================= STRUCT POHON TELUSUR BINER (PTB) =================
struct NodePTB {
    Bagasi data;
    NodePTB* left;
    NodePTB* right;
};

// ================= STRUCT QUEUE =================
struct NodeQueue {
    Bagasi data;
    NodeQueue* next;
};

struct Queue {
    NodeQueue* front;
    NodeQueue* rear;
};

// ================= STRUCT STACK =================
struct NodeStack {
    Bagasi data;
    NodeStack* next;
};

struct Stack {
    NodeStack* top;
};

// ================= UTILITY FUNCTIONS =================
void cetakBagasi(Bagasi b) {
    cout << "[Flight:" << setfill('0') << setw(4) << b.flightTime
         << " | Berat:" << fixed << setprecision(1) << b.berat << "kg]";
}

// Menekan tombol apa saja untuk melanjutkan
void pressAnyKey() {
    cout << "Press any key to continue..";
    cin.ignore();
    cin.get();
}

// ================= FUNGSI-FUNGSI PTB =================
NodePTB* createNodePTB(Bagasi b) {
    NodePTB* newNode = new NodePTB;
    newNode->data = b;
    newNode->left = nullptr;
    newNode->right = nullptr;
    return newNode;
}

bool isFlightTimeDuplicate(NodePTB* root, int flightTime) {
    if (root == nullptr) return false;
    if (root->data.flightTime == flightTime) return true;
    if (flightTime < root->data.flightTime)
        return isFlightTimeDuplicate(root->left, flightTime);
    else
        return isFlightTimeDuplicate(root->right, flightTime);
}

NodePTB* insertPTB(NodePTB* root, Bagasi b) {
    if (root == nullptr) return createNodePTB(b);
    if (b.flightTime < root->data.flightTime)
        root->left = insertPTB(root->left, b);
    else if (b.flightTime > root->data.flightTime)
        root->right = insertPTB(root->right, b);
    return root;
}


void inorderPTB(NodePTB* root) {
    if (root != nullptr) {
        inorderPTB(root->left);
        cout << " -> ";
        cetakBagasi(root->data);
        cout << "\n";
        inorderPTB(root->right);
    }
}

void clearPTB(NodePTB*& root) {
    if (root != nullptr) {
        clearPTB(root->left);
        clearPTB(root->right);
        delete root;
        root = nullptr;
    }
}

// ================= FUNGSI-FUNGSI QUEUE =================
void initQueue(Queue& q) {
    q.front = nullptr;
    q.rear = nullptr;
}

bool isQueueEmpty(Queue q) {
    return q.front == nullptr;
}

void enqueue(Queue& q, Bagasi b) {
    NodeQueue* newNode = new NodeQueue;
    newNode->data = b;
    newNode->next = nullptr;
    if (isQueueEmpty(q)) {
        q.front = newNode;
        q.rear = newNode;
    } else {
        q.rear->next = newNode;
        q.rear = newNode;
    }
}

Bagasi dequeue(Queue& q) {
    NodeQueue* temp = q.front;
    Bagasi b = temp->data;
    q.front = q.front->next;
    if (q.front == nullptr) {
        q.rear = nullptr;
    }
    delete temp;
    return b;
}

void cetakQueue(Queue q) {
    NodeQueue* curr = q.front;
    while (curr != nullptr) {
        cout << " -> ";
        cetakBagasi(curr->data);
        cout << "\n";
        curr = curr->next;
    }
}

void PTBtoQueue(NodePTB* root, Queue& q) {
    if (root != nullptr) {
        PTBtoQueue(root->left, q);
        enqueue(q, root->data);
        PTBtoQueue(root->right, q);
    }
}

// ================= FUNGSI-FUNGSI STACK =================
void initStack(Stack& s) {
    s.top = nullptr;
}

bool isStackEmpty(Stack s) {
    return s.top == nullptr;
}

void push(Stack& s, Bagasi b) {
    NodeStack* newNode = new NodeStack;
    newNode->data = b;
    newNode->next = s.top;
    s.top = newNode;
}

Bagasi pop(Stack& s) {
    NodeStack* temp = s.top;
    Bagasi b = temp->data;
    s.top = s.top->next;
    delete temp;
    return b;
}

void cetakStack(Stack s) {
    NodeStack* curr = s.top;
    while (curr != nullptr) {
        cout << " [TOP] ";
        cetakBagasi(curr->data);
        cout << "\n";
        curr = curr->next;
    }
}

void clearStack(Stack& s) {
    while (!isStackEmpty(s)) {
        pop(s);
    }
}

// ================= UTAMA / MENU PROGRAM =================
int main() {
    NodePTB* rootPTB = nullptr;
    Queue konveyorQueue;
    Stack overweightStack;

    initQueue(konveyorQueue);
    initStack(overweightStack);

    int pilihan;

    do {
        system("clear || cls"); 
        cout << "=== Sistem Keamanan Bandara ===\n";
        cout << "1. [PTB] Check-In Bagasi (Input Data)\n";
        cout << "2. [PTB] Lihat Database Bagasi (Cetak PTB)\n";
        cout << "3. [PTB -> Queue] Pindahkan Semua ke Konveyor (Hapus PTB)\n";
        cout << "4. [Queue] Lihat Antrian Konveyor (Cetak Queue)\n";
        cout << "5. [Queue -> Stack] Proses Scanning X-Ray\n";
        cout << "6. [Stack] Lihat Tumpukan Overweight (Cetak Stack)\n";
        cout << "7. [Stack Process] Pemeriksaan Manual (Pop Semua)\n";
        cout << "0. Keluar\n";
        cout << "Pilihan Anda: ";
        cin >> pilihan;

        switch (pilihan) {
            case 1: {
                system("clear || cls");
                cout << "--- Input Data Bagasi Baru ---\n";
                
                Bagasi b;
                cout << "Flight Time (Format HHMM, cth: 1430): ";
                cin >> b.flightTime;

                int jam = b.flightTime / 100;
                int menit = b.flightTime % 100;

                if (b.flightTime < 0 || b.flightTime > 2359 || jam > 23 || menit > 59) {
                    cout << "ERROR: Format jam tidak valid (0000-2359, menit 00-59).\n";
                    pressAnyKey();
                    break;
                }

                if (isFlightTimeDuplicate(rootPTB, b.flightTime)) {
                    cout << "ERROR: Jadwal penerbangan sudah ada (harus unik).\n";
                    pressAnyKey();
                    break;
                }

                cout << "Berat Bagasi (kg, harus > 0): ";
                cin >> b.berat;

                if (b.berat <= 0) {
                    cout << "ERROR: Berat harus positif.\n";
                    pressAnyKey();
                    break;
                }

                rootPTB = insertPTB(rootPTB, b);
                cout << "-> Data berhasil masuk ke Database PTB.\n";
                pressAnyKey();
                break;
            }
            case 2: {
                system("clear || cls");
                cout << "--- Isi Database Bagasi (PTB Inorder) ---\n";
                if (rootPTB == nullptr) {
                    cout << " (Database Kosong) \n";
                } else {
                    inorderPTB(rootPTB);
                }
                pressAnyKey();
                break;
            }
            case 3: {
                system("clear || cls");
                cout << "--- Memindahkan Data ke Konveyor ---\n";
                if (rootPTB == nullptr) {
                    cout << "GAGAL: PTB Kosong! Input data dulu di Menu 1.\n";
                } else {
                    PTBtoQueue(rootPTB, konveyorQueue);
                    clearPTB(rootPTB); 
                    cout << "-> SUKSES: Semua data dipindahkan dari PTB ke Queue Konveyor.\n";
                    cout << "-> INFO: Database PTB sekarang telah dikosongkan.\n";
                }
                pressAnyKey();
                break;
            }
            case 4: {
                system("clear || cls");
                cout << "--- Isi Antrian Konveyor (Queue) ---\n";
                if (isQueueEmpty(konveyorQueue)) {
                    cout << " (Queue Kosong) \n";
                } else {
                    cetakQueue(konveyorQueue);
                }
                pressAnyKey();
                break;
            }
            case 5: {
                system("clear || cls");
                cout << "--- Memulai Proses Scanning X-Ray ---\n";
                cout << "Aturan Filter: Berat > 9.3 kg -> OVERWEIGHT (masuk Stack)\n\n";
                
                if (isQueueEmpty(konveyorQueue)) {
                    cout << "GAGAL: Konveyor kosong! Load data dulu di Menu 3.\n";
                } else {
                    while (!isQueueEmpty(konveyorQueue)) {
                        Bagasi b = dequeue(konveyorQueue);
                        cout << "Scan: ";
                        cetakBagasi(b);
                        
                        if (b.berat <= 9.3) {
                            cout << " -> STATUS: AMAN (Lolos ke Pesawat)\n";
                        } else {
                            cout << " -> STATUS: OVERWEIGHT! (Push ke Stack)\n";
                            push(overweightStack, b);
                        }
                    }
                    cout << "\n-> Konveyor telah kosong. Proses scanning selesai.\n";
                }
                pressAnyKey();
                break;
            }
            case 6: {
                system("clear || cls");
                cout << "--- Isi Area Isolasi (Tumpukan Stack) ---\n";
                if (isStackEmpty(overweightStack)) {
                    cout << " (Stack Kosong) \n";
                } else {
                    cetakStack(overweightStack);
                }
                pressAnyKey();
                break;
            }
            case 7: {
                system("clear || cls");
                cout << "--- Pemeriksaan Manual (Pop Stack sampai habis) ---\n";
                if (isStackEmpty(overweightStack)) {
                    cout << "INFO: Stack kosong. Tidak ada bagasi overweight.\n";
                } else {
                    while (!isStackEmpty(overweightStack)) {
                        Bagasi b = pop(overweightStack);
                        cout << "Petugas memeriksa: ";
                        cetakBagasi(b);
                        cout << " -> SELESAI DIPERIKSA.\n";
                    }
                    cout << "\n-> Semua bagasi overweight telah diperiksa. Tumpukan kosong.\n";
                }
                pressAnyKey();
                break;
            }
            case 0: {
                system("clear || cls");
                clearPTB(rootPTB);
                clearStack(overweightStack);
                while(!isQueueEmpty(konveyorQueue)) {
                    dequeue(konveyorQueue);
                }
                cout << "Keluar dari program...\n";
                cout << "Terima kasih!\n";
                break;
            }
            default:
                cout << "Pilihan tidak valid! Silakan coba lagi.\n";
                pressAnyKey();
                break;
        }
    } while (pilihan != 0);

    return 0;
}
