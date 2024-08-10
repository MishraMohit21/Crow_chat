-- Insert test users
INSERT INTO Users (user_name, user_password, user_email) VALUES ('user1', 'password1', 'user1@example.com');
INSERT INTO Users (user_name, user_password, user_email) VALUES ('user2', 'password2', 'user2@example.com');
INSERT INTO Users (user_name, user_password, user_email) VALUES ('user3', 'password3', 'user3@example.com');


-- Insert test chats
INSERT INTO Chats (chat_name) VALUES ('General Chat');
INSERT INTO Chats (chat_name) VALUES ('Project Discussion');
INSERT INTO Chats (chat_name) VALUES ('Random');


-- Assuming chat IDs are 1, 2, and 3 respectively for the above chats
-- and user IDs are 1, 2, and 3 respectively for the test users

-- General Chat (chat_id = 1)
INSERT INTO UserChats (user_id, chat_id) VALUES (1, 1);
INSERT INTO UserChats (user_id, chat_id) VALUES (2, 1);

-- Project Discussion (chat_id = 2)
INSERT INTO UserChats (user_id, chat_id) VALUES (1, 2);
INSERT INTO UserChats (user_id, chat_id) VALUES (3, 2);

-- Random (chat_id = 3)
INSERT INTO UserChats (user_id, chat_id) VALUES (2, 3);
INSERT INTO UserChats (user_id, chat_id) VALUES (3, 3);


-- Insert test messages for General Chat (chat_id = 1)
INSERT INTO Messages (chat_id, sender_id, message_text) VALUES (1, 1, 'Hello everyone!');
INSERT INTO Messages (chat_id, sender_id, message_text) VALUES (1, 2, 'Hi user1!');

-- Insert test messages for Project Discussion (chat_id = 2)
INSERT INTO Messages (chat_id, sender_id, message_text) VALUES (2, 1, 'How is the project going?');
INSERT INTO Messages (chat_id, sender_id, message_text) VALUES (2, 3, 'It’s going well, thanks for asking.');

-- Insert test messages for Random (chat_id = 3)
INSERT INTO Messages (chat_id, sender_id, message_text) VALUES (3, 2, 'Any weekend plans?');
INSERT INTO Messages (chat_id, sender_id, message_text) VALUES (3, 3, 'Just relaxing and watching movies.');
