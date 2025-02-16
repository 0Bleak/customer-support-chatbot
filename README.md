This project implements a Customer Support Chatbot that integrates multiple backend systems using Enterprise Integration Patterns (EIPs). The chatbot acts as a mediator and orchestrator, fetching data from various systems, enriching it, and providing a seamless experience to the end user. The backend systems include a CRM (Customer Relationship Management) system, a Ticketing System, and an FAQ Database.
Fake API for Simulation

To facilitate learning and development, the backend systems are implemented as a fake API using Flask in Python. This fake API simulates real-world systems by providing static responses to predefined queries. For example:

    The CRM API returns fake customer details (e.g., name, email) for specific user IDs.

    The Ticketing API returns fake ticket statuses (e.g., resolved, in progress) for specific issues.

    The FAQ API returns predefined answers to common questions (e.g., password reset, contact support).

The use of a fake API allows me to focus on understanding and implementing EIPs without the complexity of integrating real backend systems. It also ensures that the project remains lightweight and easy to set up, making it ideal for educational purposes.
Key Features

    Static Responses: Predefined responses for specific inputs.

    Scalability: Easily add new endpoints and responses.

    Educational Focus: Learn EIPs and system integration in a controlled environment.

