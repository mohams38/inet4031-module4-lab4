
Problem Statement
This project is a seating reservation system for Colossus Airlines. The plane has 24 seats and does two flights each day, one outbound and one inbound. In this version, the program also saves the reservation data to a file so the data is still there when the program starts again.

Describe the Solution
I used an array of structures to store seat information for both flights. Each seat stores the seat number, whether the seat is assigned, and the passenger’s first and last name. The program starts by checking for a saved file and loading the reservation data if it exists. If there is no file yet, it starts with empty seats. When a seat is assigned or deleted, the program saves the updated data to the file.



Pros
- It works for both outbound and inbound flights
- All functions works and can assign seats

Cons
- The data is only saved in a local file
- It is still a basic program and not made for multiple users



