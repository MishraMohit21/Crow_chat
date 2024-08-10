-- Table for storing user information
CREATE TABLE IF NOT EXISTS Users (
    user_id INTEGER PRIMARY KEY AUTOINCREMENT,
    user_name TEXT UNIQUE NOT NULL,
    user_password TEXT NOT NULL,
    user_email TEXT NOT NULL
);

-- Table for storing chat information
CREATE TABLE IF NOT EXISTS Chats (
    chat_id INTEGER PRIMARY KEY AUTOINCREMENT,
    chat_name TEXT,
    last_message_id INTEGER,
    last_message_text TEXT,
    last_message_timestamp TIMESTAMP,
    is_group BOOLEAN DEFAULT 0,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Table for storing messages in chats
CREATE TABLE IF NOT EXISTS Messages (
    message_id INTEGER PRIMARY KEY AUTOINCREMENT,
    chat_id INTEGER NOT NULL,
    sender_id INTEGER NOT NULL,
    message_text TEXT NOT NULL,
    message_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (chat_id) REFERENCES Chats(chat_id),
    FOREIGN KEY (sender_id) REFERENCES Users(user_id)
);

-- Table to manage the many-to-many relationship between users and chats
CREATE TABLE IF NOT EXISTS UserChats (
    user_id INTEGER NOT NULL,
    chat_id INTEGER NOT NULL,
    PRIMARY KEY (user_id, chat_id),
    FOREIGN KEY (user_id) REFERENCES Users(user_id),
    FOREIGN KEY (chat_id) REFERENCES Chats(chat_id)
);

