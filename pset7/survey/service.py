import csv

def save(students, database):
    with open(database, "w") as file:
        writer = csv.writer(file)
        for student in students:
            writer.writerow((student["name"], student["house"], student["player"]))

def getAllStudents(database):
    allStudents = []
    with open(database, "r") as file:
        reader = csv.reader(file, delimiter=',')
        for row in reader:
            allStudents.append({"name": row[0], "house": row[1], "player": row[2]})
    return allStudents