package pt.lab3;

import java.net.*;
import java.io.*;

public class Server {
    private ServerSocket serverSocket;
    private Socket client;

    public void start(int port) {
        try {
            serverSocket = new ServerSocket(port);
        } catch (java.io.IOException ex) {
            System.out.println(ex);
        }
        while(true){
            try {
                client = serverSocket.accept();
                System.out.println("Client connected to server");
                new ClientHandler(client).start();
            }catch (java.io.IOException ex) {
                System.out.println(ex);
            }
        }

    }

    public void stop() {
        try {
            serverSocket.close();
        } catch (java.io.IOException ex) {
            System.out.println(ex);
        }
    }

    private static class ClientHandler extends Thread {
        private Socket clientSocket;
        private PrintWriter out;
        private ObjectInputStream  in;

        public ClientHandler(Socket socket) {
            this.clientSocket = socket;
        }

        public void run() {
            try{
                out = new PrintWriter(clientSocket.getOutputStream(), true);
                in = new ObjectInputStream (clientSocket.getInputStream());

                out.println("ready");
                int n = 0;
                try{
                    n = (Integer)in.readObject();
                }
                catch (java.lang.ClassNotFoundException ex){
                    System.out.println(ex);
                }
                out.println("ready for messages");
                Message m;
                for(int i=0; i<n; i++){
                    try{
                        m = (Message)in.readObject();
                        System.out.println(m + "\n");
                    }
                    catch(java.lang.ClassNotFoundException ex){
                        System.out.println(ex);
                    }
                }

                out.println("finished");

                in.close();
                out.close();
                clientSocket.close();}

             catch (java.io.IOException ex) {
                System.out.println(ex);
            }

        }
    }
    public static void main(String[] args) {
        Server server = new Server();
        server.start(6666);
    }
}