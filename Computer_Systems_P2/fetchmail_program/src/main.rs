/**
*    Code written by Maheen Khan ,1193813 and Nhat Hoang Nguyen 1346324
*    for Comp30023 Project 2: Email Client. May 2024
*   
*/

use std::env;
use std::net::TcpStream;
use openssl::ssl::{SslMethod, SslConnector};
use std::io::{self};
use std::boxed::Box;
use std::io::{Write as IoWrite, Read as IoRead};


mod communication;
mod command;
mod utils;

const TCP_PORT: &str = "143";
const TLS_PORT: &str = "993";


#[allow(unused)]

/*Create a class that has read and write. This is used for Tcp/Tls read a write*/
trait ReadAndWrite: IoRead + IoWrite {
}

impl <T: IoRead + IoWrite> ReadAndWrite for T {
}

fn main() -> io::Result<()> {
    //Prevent injection
    let args: Vec<String> = env::args().map(|arg| arg.replace("\r\n", "")).collect();
    
    /* Parse command - line */
    if args.len() < 6 {
        eprintln!("Usage: fetchmail -u <username> -p <password> [-f <folder>] [-n <messageNum>] [-t] <command> <server_name>");
        std::process::exit(1);
    }

    
    let mut username = String::new();
    let mut password = String::new();
    let mut folder = String::from("INBOX");
    let mut message_num: Option<String> = None;
    let mut command = String::new();
    let mut server_name = String::new();

    let mut args_iter = args.iter().skip(1);
    while let Some(arg) = args_iter.next() {
        match arg.as_str() {
            "-u" => username = args_iter.next().unwrap_or_else(|| panic!("Expected username")).clone(),
            "-p" => password = args_iter.next().unwrap_or_else(|| panic!("Expected password")).clone(),
            "-f" => folder = args_iter.next().unwrap_or_else(|| panic!("Expected folder")).to_string(),
            "-n" => message_num = args_iter.next().map(String::from),
            "retrieve" | "list" | "mime" | "parse" => command = arg.clone(),
            _ =>{
                if arg != "-t"{
                    server_name = arg.clone();
                }
            }
        }
    }
    if command.is_empty() || server_name.is_empty() {
        eprintln!("Command and server name are required.");
        std::process::exit(1);
    }


    /* Establish connection code references
    * https://docs.rs/openssl/latest/openssl/
    * https://doc.rust-lang.org/std/boxed/
    * https://stackoverflow.com/questions/68316251/rust-use-common-trait-type-for-different-streams-tcp-stream-tls-stream    
    */
    
    let mut port = TCP_PORT.to_string();
    let mut is_tls : bool = false;
    
    
    if args.contains(&"-t".to_string()){
        port = TLS_PORT.to_string();
        is_tls = true;

    }

    let server_with_port = format!("{}:{}", server_name, port);
      
    //Attempt to connect to the server  
    let stream = match TcpStream::connect(&server_with_port){
        Ok(stream) => stream,
        Err(_) => {
            println!("Server connection failure");
            std::process::exit(1);
        }
    };
    let mut stream = establish_tls_connection(is_tls,server_name,stream);



    // If connection established, then login into the user's account on the server.
    communication::send_command(&mut stream, &format!("A01 LOGIN {} {}\r\n", username, password))?;
    if let Err(_) = communication::read_response(&mut stream) {
        println!("Login failure");
        std::process::exit(3);
    }
    
    // Send SELECT folder command to the server 
    communication::send_command(&mut stream, &format!("A02 SELECT \"{}\"\r\n", folder))?;
    let select_response = communication::read_response(&mut stream);
    match &select_response {
        Ok(_response) => {
            //If response is OK, does nothing
        },
        Err(_) => {
            println!("Folder not found");
            std::process::exit(3);
        }
    }
    
    // If message num is not provided, choose the lastest message
    if message_num.is_none() {
        if let Ok(response) = &select_response {
            if let Some(exists_msg) = response.split_whitespace().find(|&s| s.chars().all(char::is_numeric)) {
                message_num = Some(exists_msg.to_string());
            }
        }
    }
    // If message num is not an integer
    else if let Some(ref num) = message_num {
        if num.parse::<i32>().is_err() {
            println!("Message not found");
            std::process::exit(3);
        }
    }

    //Send commands to server
    if command.contains("retrieve"){
        command::retrieve(message_num, &mut stream)?;
    }
    else if command.contains("parse"){
        command::parse(message_num, &mut stream)?;        
    }
    else if command.contains("list"){
        command::list(&mut stream)?;
        
    }
    else if command.contains("mime"){
        command::mime(message_num, &mut stream)?;
    }
    else {
        println!("What command is that? Bye bye");
        std::process::exit(1);
    }
    Ok(())
}

/* If connection type is tls, it creates a tls connection, else returns the tcp string as a box object*/
fn establish_tls_connection(is_tls :bool, server_name: String,tcp: TcpStream)->Box<dyn ReadAndWrite>{

    if is_tls {
        let connector = SslConnector::builder(SslMethod::tls()).unwrap().build();        
        let tls = connector.connect(&server_name, tcp).unwrap();
        
        Box::new(tls)
    
    } else {
        Box::new(tcp)
    }
}