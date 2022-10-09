-- In 6.sql, write a SQL query to determine the average rating of all movies released in 2012.
SELECT AVG(rating) FROM movies
JOIN ratings ON ratings.movie_id = movies.id
WHERE year = 2012;

