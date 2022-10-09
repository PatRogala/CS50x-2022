-- In 10.sql, write a SQL query to list the names of all people who have directed a movie that received a rating of at least 9.0.
SELECT name FROM people
JOIN directors ON people.id = directors.person_id
JOIN movies ON directors.movie_id = movies.id
JOIN ratings ON movies.id = ratings.movie_id
WHERE rating >= 9.0
