CREATE TABLE IF NOT EXISTS Users (
    user_id INTEGER PRIMARY KEY AUTOINCREMENT,
    user_name TEXT UNIQUE NOT NULL,
    user_password TEXT NOT NULL,
    user_email TEXT NOT NULL
);

CREATE TABLE IF NOT EXISTS Chats (
    chat_id INTEGER PRIMARY KEY AUTOINCREMENT,
    chat_name TEXT,
    last_message_id INTEGER,
    last_message_text TEXT,
    last_message_timestamp TIMESTAMP,
    is_group BOOLEAN DEFAULT 0,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE IF NOT EXISTS Messages (
    message_id INTEGER PRIMARY KEY AUTOINCREMENT,
    chat_id INTEGER NOT NULL,
    sender_id INTEGER NOT NULL,
    message_text TEXT NOT NULL,
    message_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (chat_id) REFERENCES Chats(chat_id),
    FOREIGN KEY (sender_id) REFERENCES Users(user_id)
);



INSERT INTO Users (user_name, user_password, user_email) VALUES ('user1', 'password1', 'user1hai@example.com');
INSERT INTO Users (user_name, user_password, user_email) VALUES ('user2', 'password2', 'user2hai@example.com');


INSERT INTO Chats (chat_name) VALUES ('Test_chat');

SQLite version 3.37.2 2022-01-06 13:25:41
Enter ".help" for usage hints.
[?2004hsqlite> ^C[?2004l
[?2004h^C[?2004l
[?2004h^C[?2004l
