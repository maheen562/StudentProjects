/**
*    Code written by Maheen Khan ,1193813 and Nhat Hoang Nguyen 1346324
*    for Comp30023 Project 2: Email Client. May 2024
*    This file consists of handling the respone/ string manipulation
*    of the queries sent to the server
*/


/* 
* Purpose: Function performs email parsing
* parse_response : String response from server
*/
pub fn parse(parse_response : String){
    if !parse_response.contains("BAD Error"){

        let modified_response = parse_response.replace("\r","");
        let mut response_lines: Vec<&str> = modified_response.split('\n').collect();

        response_lines.remove(0);
        response_lines.retain(|&line| line != "");        

        let mut date = String::from("Date: ");
        let mut from = String::from("From: ");
        let mut to = String::from("To: ");
        let mut subject = String::new();
        
        let mut index = 0;

        while index< response_lines.len(){
            let  line = response_lines[index];
                       
            if line.to_lowercase().contains("date"){
                date.push_str(&line[6..]);
            }
            else if line.to_lowercase().contains("to"){
                to.push_str(&line[4..]);
            }
            else if line.to_lowercase().contains("from"){
                from.push_str(&line[6..])
            }
            else{
                if line.to_lowercase().contains("subject"){
                    subject = "Subject: ".to_string() + &line[9..].to_string().trim_start().trim_end();
                }
                else{
                    subject.push_str(" ");
                    subject.push_str(line.trim_start().trim_end());                

                }
            }

            index+=1;

        }

        if subject.is_empty() {
            subject = "Subject: <No subject>".to_string();
        }
    
        println!("{}\n{}\n{}\n{}",from.trim_end(),to.trim_end(),date.trim_end(),subject.trim_end());

    }
}

/* 
* Purpose: Function performs email listing
* list_response : String response from server
*/
pub fn list (list_response:String){
    if !list_response.contains("BAD Error"){  
        let modified_response = list_response.replace("\r","");
        let mut _lines: Vec<&str> = modified_response.split('*').collect();

        let mut index:i32 = 0;
        if _lines.len()>0{
            for each_mail in _lines{
                if each_mail.contains("FETCH"){
                    let mut each_heading : Vec<&str> = each_mail.split('\n').collect();
                    each_heading.remove(0);
                    each_heading.retain(|&line| line != "(" && line != ")");
    
                    let mut concat_heading = String :: new();
                    for each_line in each_heading{
                        if each_line.to_lowercase().contains("subject"){
                            concat_heading.push_str(&each_line[9..]);
                        }
                        else{
                            concat_heading.push_str(each_line);
                        }
                    }
    
                    index+=1;
                    if concat_heading == ""{
                        print!("{}: <No subject>\n",index);
                    }
                    else{
                        print!("{}: {}\n",index,concat_heading);
                    }
                }
             }
        
        }else{
            std::process::exit(0);

        }
    }
}

/* 
* Purpose: Function extracts email MIME boundary
* content: &str : String response from server
* Return Option<String> if boundary is found, Some<String> is returned, else NONE returned
*/
pub fn extract_boundary(content: &str) -> Option<String> {
    let mut unfolded_lines = Vec::new();
    let mut current_line = String::new();

    // Unfold the headers first
    for line in content.lines() {
        if line.starts_with(' ') || line.starts_with('\t') {
            // It's a continuation of the previous line
            current_line += line.trim();
        } else {
            if !current_line.is_empty() {
                unfolded_lines.push(current_line.clone());
            }
            current_line = line.to_string();
        }
    }
    if !current_line.is_empty() {
        unfolded_lines.push(current_line);
    }

    //Search for the boundary in the unfolded content
    for line in unfolded_lines {
        if line.to_lowercase().contains("content-type:") && line.to_lowercase().contains("boundary=") {
            let start_index = line.to_lowercase().find("boundary=").unwrap() + "boundary=".len();
            let boundary_start = line[start_index..].trim_start_matches(' ');
            let boundary_value = if let Some(idx) = boundary_start.find(';') {
                &boundary_start[..idx]
            } else {
                boundary_start
            };

            return Some(boundary_value.trim_matches(|c: char| c == '"' || c.is_whitespace()).to_string());
        }
    }
    None
}


/*processes the mime message from the response*/
pub fn process_mime_message(content: &str, boundary: &str) -> Result<(), String> {
    let bound = format!("--{}", boundary);
    let end_bound = format!("--{}--", boundary);
    let mut in_text_plain_utf8_section = false;
    let mut result = String::new();
    let mut found_content_type = false;
    let mut found_charset = false;

    let mut lines = content.lines().peekable();
    while let Some(line) = lines.next() {
        if line.trim() == bound || line.trim() == end_bound {
            // We are at the start or end of a section
            if in_text_plain_utf8_section {
                // If we were collecting text, stop as we hit the end of a section
                break;
            }
            for _ in 0..3 { // Peek the next three lines
                if let Some(next_line) = lines.next() {
                    if next_line.to_lowercase().contains("content-type: text/plain") && next_line.to_lowercase().contains("charset=utf-8") {
                        in_text_plain_utf8_section = true; // Start capturing lines on next iterations
                        lines.next();
                        break;
                    }
                    if next_line.to_lowercase().contains("content-type: text/plain") {
                        found_content_type = true;
                    }
                    if next_line.to_lowercase().contains("charset=utf-8") {
                        found_charset = true;
                    }
                }
            }
            if found_content_type && found_charset {
                in_text_plain_utf8_section = true; // Start capturing lines on next iterations
                lines.next();
            }
        }
        //Collecting text 
        else if in_text_plain_utf8_section {
                result.push_str(line);
                result.push_str("\r\n");
        }
    }
    if result.ends_with("\r\n") {
        // Remove the last characters "\r\n" 
        let len = result.len();
        result.truncate(len - 3);
    }
    

    if result.is_empty() {
        Err("No matching text/plain part found".to_string())
    } else {
        println!("{}", result);
        Ok(())
    }
}