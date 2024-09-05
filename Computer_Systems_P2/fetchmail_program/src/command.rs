/**
*    Code written by Maheen Khan ,1193813 and Nhat Hoang Nguyen 1346324
*    for Comp30023 Project 2: Email Client. May 2024
*    This file consists of handling the respone/ string manipulation
*    of the queries sent to the server
*/

use std::io::{self};
use crate::ReadAndWrite;


/* 
* Purpose: Function reponsible for email retrieval
* message_num: Option<String>  no. message selected
* stream: &mut Box<dyn ReadAndWrite>  stream selected (tls or plain tcp) 
* Return io::Result <()> Ok(()) if succeeds, else exit with code 3 
*/
pub fn retrieve(message_num:Option<String>, stream: &mut Box<dyn ReadAndWrite>) -> io::Result<()>{
    let fetch_command = if let Some(num) = message_num {
        format!("A03 FETCH {} BODY.PEEK[]\r\n", num)
    } else {
        "A03 FETCH 1 BODY.PEEK[]\r\n".to_string() // default to first if unable to determine last
    };

    //Communicate with the server
    super::communication::send_command(stream, &fetch_command)?;
    let fetch_response = super::communication::read_response(stream);
    

    match &fetch_response {
        Ok(response) => {
            println!("{}", response);
        },
        Err(_) => {
            println!("Message not found");
            std::process::exit(3);
        }
    }
    Ok(())
}

/* 
* Purpose: Function responsible for email parsing
* message_num: Option<String>  no. message selected
* stream: &mut Box<dyn ReadAndWrite>  stream selected (tls or plain tcp) 
* Return io::Result <()> Ok(()) if succeeds, else exit with code 3 
*/
pub fn parse(message_num:Option<String>, stream: &mut Box<dyn ReadAndWrite>) -> io :: Result <()>{
    let parse_command = if let Some(ref num) = message_num {
        format!("A03 FETCH {} BODY.PEEK[HEADER.FIELDS (FROM TO DATE SUBJECT)]\r\n", num)
    } else {
        "A03 FETCH 1 BODY.PEEK[HEADER.FIELDS (FROM TO DATE SUBJECT)]\r\n".to_string() // default to first if unable to determine last
    };


    super::communication::send_command(stream,&parse_command)?;
    let parse_response = super::communication::read_response(stream);
    match parse_response {
        Ok(s) => {
            super::utils::parse(s);
        },
        Err(_) => {
            println!("Message not found");
            std::process::exit(3);
        }
    }
    
    Ok(())
}

/* 
* Purpose: Function responsible for email listing
* stream: &mut Box<dyn ReadAndWrite>  stream selected (tls or plain tcp) 
* Return io::Result <()> Ok(()) if succeeds, else exit with code 3 
*/
pub fn list(stream: &mut Box<dyn ReadAndWrite>) -> io :: Result<()>{ 
    let list_command = "A06 FETCH 1:* BODY.PEEK[HEADER.FIELDS (SUBJECT)]\r\n";
    
    
    super::communication::send_command(stream,&list_command)?;
    let list_response = super::communication::read_response(stream);
    match list_response {
        Ok(e) => {
            super::utils::list(e);
        },
        Err(_) => {
            println!("Message not found");
            std::process::exit(3);
        }
    }
    
    Ok(())
}

/* 
* Purpose: Function responsible for email MIME
* message_num: Option<String>  no. message selected
* stream: &mut Box<dyn ReadAndWrite>  stream selected (tls or plain tcp) 
* Return io::Result <()> Ok(()) if succeeds, else exit with code 3 or 4
*/
pub fn mime(message_num:Option<String>,stream: &mut Box<dyn ReadAndWrite>) -> io :: Result<()>{
    let fetch_command = if let Some(ref num) = message_num {
        format!("A03 FETCH {} BODY.PEEK[]\r\n", num)
    } else {
        "A03 FETCH 1 BODY.PEEK[]\r\n".to_string() // default to first if unable to determine last
    };
    
    
    super::communication::send_command(stream, &fetch_command)?;
    let fetch_response = super::communication::read_response(stream);
    
    
    let response = match fetch_response {
        Ok(response) => response,
        Err(_) => {
            println!("Message not found");
            std::process::exit(3);
        }
    };

    //Identify the boundary in the email
    let boundary = match super::utils::extract_boundary(&response) {
        Some(boundary) => boundary,
        None => {
            println!("Failed to locate MIME boundary message");
            std::process::exit(4);
        }
    };

    //Find mime part according to the spec
    if let Err(_) = super::utils::process_mime_message(&response, &boundary) {
        println!("Failed to MIME message");
        std::process::exit(4);
    }
    Ok(())
}