SELECT name FROM movies, people, stars
WHERE stars.person_id = people.id AND stars.movie_id = movies.id AND people.name != "Kevin Bacon" AND movies.title IN (
SELECT title FROM movies, people, stars
WHERE stars.person_id = people.id AND stars.movie_id = movies.id AND people.name = "Kevin Bacon" AND people.birth = 1958);