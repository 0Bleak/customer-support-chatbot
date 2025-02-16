from flask import Flask, request, jsonify
import logging

app = Flask(__name__)

# Configure logging
logging.basicConfig(level=logging.INFO)

# Fake CRM Data
CRM_DATA = {
    "1": {"name": "John Doe", "email": "john@example.com"},
    "2": {"name": "Jane Smith", "email": "jane@example.com"}
}

# Fake Ticketing Data
TICKET_DATA = {
    "password_reset": {"status": "Resolved", "ticket_id": "12345"},
    "login_issue": {"status": "In Progress", "ticket_id": "67890"}
}

# Fake FAQ Data
FAQ_DATA = {
    "reset_password": {"answer": "Visit https://example.com/reset-password to reset your password."},
    "contact_support": {"answer": "Call +1-800-123-4567 for immediate assistance."}
}

# CRM Endpoint
@app.route('/crm', methods=['GET'])
def crm():
    user_id = request.args.get('user_id')
    logging.info(f"CRM request received for user_id: {user_id}")
    if user_id in CRM_DATA:
        return jsonify(CRM_DATA[user_id])
    logging.warning(f"User not found: {user_id}")
    return jsonify({"error": "User not found"}), 404

# Ticketing Endpoint
@app.route('/ticketing', methods=['GET'])
def ticketing():
    query = request.args.get('query')
    logging.info(f"Ticketing request received for query: {query}")
    if query in TICKET_DATA:
        return jsonify(TICKET_DATA[query])
    logging.warning(f"Ticket not found: {query}")
    return jsonify({"error": "Ticket not found"}), 404

# FAQ Endpoint
@app.route('/faq', methods=['GET'])
def faq():
    question = request.args.get('question')
    logging.info(f"FAQ request received for question: {question}")
    if question in FAQ_DATA:
        return jsonify(FAQ_DATA[question])
    logging.warning(f"FAQ not found: {question}")
    return jsonify({"error": "FAQ not found"}), 404

# Human Agent Endpoint
@app.route('/human_agent', methods=['POST'])
def human_agent():
    data = request.json
    logging.info(f"Human agent request received: {data}")
    return jsonify({"status": "Request logged", "data": data})

if __name__ == '__main__':
    app.run(debug=True)