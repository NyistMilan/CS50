SELECT title FROM movies, people, stars
WHERE stars.movie_id = movies.id AND stars.person_id = people.id AND people.name = "Johnny Depp" AND movies.id IN (
SELECT movies.id FROM movies, people, stars
WHERE stars.movie_id = movies.id AND stars.person_id = people.id AND people.name = "Helena Bonham Carter");