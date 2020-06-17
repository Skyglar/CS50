import cs50

from flask import Flask, jsonify, redirect, render_template, request
from service import save, getAllStudents

# Configure application
app = Flask(__name__)

# Reload templates when they are changed
app.config["TEMPLATES_AUTO_RELOAD"] = True

database = 'survey.csv'
students = []

@app.after_request
def after_request(response):
    """Disable caching"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET"])
def get_index():
    return redirect("/form")


@app.route("/form", methods=["GET"])
def get_form():
    return render_template("form.html")


@app.route("/form", methods=["POST"])
def post_form():

    if not request.form.get("name"):
        return render_template("error.html", message="Name cannot be empty")
    elif not request.form.get("house"):
        return render_template("error.html", message="House has not been chosen")
    elif not request.form.get("player"):
        return render_template("error.html", message="Position has not been chosen")

    students.append({"name" : request.form.get("name"), "house" : request.form.get("house"), "player": request.form.get("player")})
    save(students, database)
    #return render_template("error.html", message="TODO")
    return render_template("sheet.html", students = getAllStudents(database))


@app.route("/sheet", methods=["GET"])
def get_sheet():
    #return render_template("error.html", message="TODO")
    return render_template("sheet.html", students = getAllStudents(database))
    




