SELECT title FROM movies, people, stars, ratings
WHERE ratings.movie_id = movies.id AND stars.movie_id = movies.id AND stars.person_id = people.id AND people.name = "Chadwick Boseman"
ORDER BY ratings.rating DESC
LIMIT 5;