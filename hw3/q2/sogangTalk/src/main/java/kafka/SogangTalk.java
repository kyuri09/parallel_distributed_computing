package kafka;

import java.util.*;
import java.time.Duration;

import org.apache.kafka.clients.consumer.ConsumerConfig;
import org.apache.kafka.clients.consumer.ConsumerRecord;
import org.apache.kafka.clients.consumer.ConsumerRecords;
import org.apache.kafka.clients.consumer.KafkaConsumer;
import org.apache.kafka.clients.producer.KafkaProducer;
import org.apache.kafka.clients.producer.ProducerConfig;
import org.apache.kafka.clients.producer.ProducerRecord;
import org.apache.kafka.common.PartitionInfo;

public class SogangTalk {
    static String id;
    static String roomListFile;
    //static List<String> idList = new ArrayList<String>();
    //static List<String> roomList;
    static Scanner in = new Scanner(System.in);
    static Properties config_prod;
    static Properties config_cons;
    static KafkaProducer<String, String> producer;
    static KafkaConsumer<String, String> consumer;
    static ArrayList<String> roomList = new ArrayList<String>();

    public static boolean login(){
        String select;
        while(true){
            System.out.println("Welcome to SogangTalk");
            System.out.println("1. Login");
            System.out.println("2. Exit\n");
            System.out.print("SogangTalk> ");
            select = in.nextLine();
            if(select.equals(("1"))) {
                System.out.print("SogangTalk> ID: ");
                id = in.nextLine();
                System.out.println(id + "'s Chatting");
                return true;
            }
            else if(select.equals("2")){
                System.out.println("Bye!");
                return false;
            }
            else{
                System.out.println("Error");
            }
        }
    }
    public static void chatting(){
        Scanner in = new Scanner(System.in);;
        String select;
        String roomName;
        String joinRoom;
        Map<String, List<PartitionInfo>> topics;
        while(true){
            System.out.println("Chatting Window");
            System.out.println("1. List");
            System.out.println("2. Make");
            System.out.println("3. Join");
            System.out.println("4. Log out\n");
            System.out.print("SogangTalk> ");
            select = in.nextLine();
            if(select.equals(("1"))) {
                if(roomList.isEmpty()){
                    System.out.println("There is no room.\n");
                }
                else{
                    for(int i=0; i<roomList.size(); i++){
                        System.out.println(roomList.get(i));
                    }
                    System.out.print("\n");
                }
            }
            else if(select.equals(("2"))) {
                System.out.print("SogangTalk> Chat room name: ");
                roomName = in.nextLine();
                if(roomList.contains(roomName)){
                    System.out.println("Error: Room name already exist.");
                }
                else{
                   /* String value = id + " joined the chat room.";
                    ProducerRecord<String, String> record_prod = new ProducerRecord<>(roomName, value);
                    producer.send(record_prod);
                    */
                   // ProducerRecord<String, String> record_prod = new ProducerRecord<>(roomName, value);
                   
                    try{
                        roomList.add(roomName);                   
                    }catch(Exception e){
                        System.out.println("Error");
                    }
                    System.out.printf("\"%s\" is created!\n\n", roomName);
                }
            }
            else if(select.equals(("3"))) {
                System.out.print("Chat room name: ");
                roomName = in.nextLine();
                topics = consumer.listTopics();
                if(!roomList.contains(roomName)){
                    System.out.println("Error: Room does not exist.\n");
                }
                else{
                    joinRoom = roomName;
                    chatRoom(joinRoom);
                }
            }
            else if(select.equals(("4"))) {
                System.out.println("Return to Login Window Menu\n");
                break;
            }
            else{
                System.out.println("Error");
            }
        }
    }

    public static void chatRoom(String room){
        while(true) {
            String select;
            String value;

            System.out.println(room);
            System.out.println("1. Read");
            System.out.println("2. Write");
            System.out.println("3. Reset");
            System.out.println("4. Exit\n");
            System.out.print("SogangTalk> ");
            select = in.nextLine();


            if (select.equals(("1"))) {
                consumer.subscribe(Collections.singletonList(room));
                
                ConsumerRecords<String, String> records_cons = consumer.poll(Duration.ofMillis(1000));
                consumer.commitSync();
                for (ConsumerRecord<String, String> record_cons : records_cons) {
                    System.out.println(record_cons.value());
                }
                System.out.print("\n");
            }
            else if (select.equals(("2"))) {
                System.out.print("SogangTalk> Text: ");
                value = in.nextLine();
                value = id + ": " + value;
                ProducerRecord<String, String> record_prod = new ProducerRecord<>(room, value);
                producer.send(record_prod);
            }
            else if (select.equals(("3"))) {
                System.out.println("Read from the beginning\n");
                consumer.seekToBeginning(consumer.assignment());
            }
            else if (select.equals(("4"))) {
                System.out.println("Return to Chatting Window Menu\n");
                break;
            }
            else {
                System.out.println("Error");
            }
        }
    }
    public static void init(){
        config_prod = new Properties();
        config_cons = new Properties();

        config_prod.put(ProducerConfig.BOOTSTRAP_SERVERS_CONFIG, "localhost:9092");
        config_prod.put(ProducerConfig.CLIENT_ID_CONFIG, "id1");
        config_prod.put(ProducerConfig.KEY_SERIALIZER_CLASS_CONFIG, "org.apache.kafka.common.serialization.StringSerializer");
        config_prod.put(ProducerConfig.VALUE_SERIALIZER_CLASS_CONFIG, "org.apache.kafka.common.serialization.StringSerializer");
        config_prod.put(ProducerConfig.LINGER_MS_CONFIG, 1);

        config_cons.put(ConsumerConfig.GROUP_ID_CONFIG, id);
        config_cons.put(ConsumerConfig.BOOTSTRAP_SERVERS_CONFIG, "localhost:9092");
        config_cons.put(ConsumerConfig.KEY_DESERIALIZER_CLASS_CONFIG, "org.apache.kafka.common.serialization.StringDeserializer");
        config_cons.put(ConsumerConfig.VALUE_DESERIALIZER_CLASS_CONFIG, "org.apache.kafka.common.serialization.StringDeserializer");
        config_cons.put(ConsumerConfig.AUTO_OFFSET_RESET_CONFIG,"earliest");
        config_cons.put(ConsumerConfig.ENABLE_AUTO_COMMIT_CONFIG, "true");

        producer = new KafkaProducer<>(config_prod);
        consumer = new KafkaConsumer<>(config_cons);
    }

    public static void main(String[] args) {
        boolean logined;
        while(true){
            logined = login();
            if(!logined) break;
            init();
            chatting();
            producer.close();
            consumer.close();
        }
        in.close();
    }
}
