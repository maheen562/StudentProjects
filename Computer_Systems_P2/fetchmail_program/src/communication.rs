/**
*    Code written by Maheen Khan ,1193813 and Nhat Hoang Nguyen 1346324
*    for Comp30023 Project 2: Email Client. May 2024
*    This file consists of handling the respone/ string manipulation
*    of the queries sent to the server
*/



use std::io::{self, Write, BufRead, BufReader, ErrorKind};
use crate::ReadAndWrite;

/* 
* Purepose: Function sends command to the server
* stream: &mut Box<dyn ReadAndWrite> stream selected (tls or plain tcp) 
* command: &str command to be sent to the server
*/
pub fn send_command(stream:&mut Box<dyn ReadAndWrite>, command: &str) -> io::Result<()>{
    stream.write_all(command.as_bytes())?;
    stream.flush()
}

/* 
* Purepose: Function reads response from the server
* stream: &mut Box<dyn ReadAndWrite> Stream stream selected (tls or plain tcp) 
*/
pub fn read_response(stream: &mut Box<dyn ReadAndWrite>) -> io::Result<String> {
    let reader = BufReader::new(stream);
    let mut response = String::new();
    
    for line in reader.lines() {
        let line = line?;
        if line.starts_with('*') && (line.contains("BODY[]")){
            continue;
        }
        if line.starts_with("A0") && (line.contains("OK")) {        
            break; // Stop reading after the command completion status
        }
         
        else if line.starts_with("A0") && (line.contains("NO") || line.contains("BAD")){
            return Err(io::Error::new(ErrorKind::Other, "Command failed"));
        }
        response.push_str(&line);
        response.push_str("\r\n");
    }

    if response.ends_with(")\r\n") {
        // Remove the characters "\r\n"
        let len = response.len();
        response.truncate(len - 4);
    }
    Ok(response)

}