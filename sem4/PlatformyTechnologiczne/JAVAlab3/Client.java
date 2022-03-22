package pt.lab3;

import java.net.*;
import java.io.*;
import java.util.Scanner;

public class Client {
    private Socket clientSocket;
    private ObjectOutputStream out;
    private BufferedReader in;

    public void startConnection(String ip, int port) {
        try{
            clientSocket = new Socket(ip, port);
            out = new ObjectOutputStream(clientSocket.getOutputStream());
            in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));

        }
        catch (java.io.IOException ex){
            System.out.println(ex);
        }


    }

    public String sendMessage() {
        String resp = "";
        int n = 0;
        int msg_n = 0;
        String msg_body = "";
        Scanner input = new Scanner(System.in);
        try{
           resp = in.readLine();
           if(resp.equals("ready")){
               System.out.println("n :");
               n = Integer.parseInt(input.nextLine());
               out.writeObject(n);
           }
           resp = in.readLine();
           if(resp.equals("ready for messages")){
               for(int i =0 ; i<n; i ++){
                   System.out.println("message n :");
                   msg_n = Integer.parseInt(input.nextLine());
                   System.out.println("message body :");
                   msg_body=input.nextLine();
                   out.writeObject(new pt.lab3.Message(msg_n, msg_body));
               }
           }
           resp = in.readLine();
        }
        catch (java.io.IOException ex){
            System.out.println(ex);

        }

        return resp;
    }

    public void stopConnection() {
        try{
            in.close();
            out.close();
            clientSocket.close();
        }
        catch (java.io.IOException ex) {
            System.out.println(ex);
        }
    }

    public static void main(String[] args){
        Client client = new Client();
        client.startConnection("localhost", 6666);
        String response = client.sendMessage();
        System.out.println(response);
    }
}