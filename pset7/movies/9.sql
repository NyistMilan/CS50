SELECT DISTINCT name FROM people, movies, stars
WHERE stars.movie_id = movies.id AND stars.person_id = people.id AND movies.year = 2004
ORDER BY birth;