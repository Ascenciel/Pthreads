import java.io.*;
import java.nio.file.*;
 
public class ThreadDemo {
    static class SharedData {
        private int completedTasks = 0;
 
        public synchronized void taskCompleted(String taskName) {
            completedTasks++;
            System.out.println("[SYNC] " + taskName +
                    " selesai. Total tugas selesai: " + completedTasks);
        }
 
        public synchronized int getCompletedTasks() {
            return completedTasks;
        }
    }
 
    static class FactorialTask implements Runnable {
        private final SharedData shared;
 
        FactorialTask(SharedData shared) {
            this.shared = shared;
        }
 
        @Override
        public void run() {
            long n = 10;
            long result = 1;
 
            for (int i = 1; i <= n; i++) {
                result *= i;
            }
 
            System.out.println("[" + Thread.currentThread().getName() +
                    "] Faktorial " + n + " = " + result);
            shared.taskCompleted("Faktorial");
        }
    }
 
    static class FibonacciTask implements Runnable {
        private final SharedData shared;
 
        FibonacciTask(SharedData shared) {
            this.shared = shared;
        }
 
        @Override
        public void run() {
            int count = 10;
            long a = 0, b = 1;
 
            System.out.print("[" + Thread.currentThread().getName() +
                    "] Fibonacci " + count + " suku: ");
 
            for (int i = 0; i < count; i++) {
                System.out.print(a + (i == count - 1 ? "\n" : " "));
                long next = a + b;
                a = b;
                b = next;
            }
 
            shared.taskCompleted("Fibonacci");
        }
    }
 
    static class FileTask implements Runnable {
        private final SharedData shared;
 
        FileTask(SharedData shared) {
            this.shared = shared;
        }
 
        @Override
        public void run() {
            Path path = Paths.get("data.txt");
 
            try {
                if (Files.notExists(path)) {
                    Files.writeString(path,
                            "Baris pertama file teks.\n" +
                            "Baris kedua dibaca oleh thread.\n");
                }
 
                System.out.println("[" + Thread.currentThread().getName() +
                        "] Isi file:");
 
                for (String line : Files.readAllLines(path)) {
                    System.out.println("  " + line);
                }
 
                shared.taskCompleted("Baca File");
            } catch (IOException e) {
                System.out.println("Gagal membaca file: " + e.getMessage());
            }
        }
    }
 
    public static void main(String[] args) throws InterruptedException {
        SharedData shared = new SharedData();
 
        Thread t1 = new Thread(new FactorialTask(shared), "Thread-Faktorial");
        Thread t2 = new Thread(new FibonacciTask(shared), "Thread-Fibonacci");
        Thread t3 = new Thread(new FileTask(shared), "Thread-File");
 
        System.out.println("=== Program Multithreading Java ===");
 
        t1.start();
        t2.start();
        t3.start();
 
        t1.join();
        t2.join();
        t3.join();
 
        System.out.println("Semua thread selesai.");
        System.out.println("Total tugas selesai = " + shared.getCompletedTasks());
    }
}