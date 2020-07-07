BEGIN TRANSACTION;
CREATE TABLE IF NOT EXISTS artist(
    artist_id   INTEGER  NOT NULL PRIMARY KEY,
    artist_name TEXT    NOT NULL,
    artist_mbid TEXT
);
CREATE TABLE IF NOT EXISTS album(
    album_id    INTEGER NOT NULL PRIMARY KEY,
    album_name  TEXT    NOT NULL,
    album_mbid  TEXT,
    num_discs   INTEGER NOT NULL,
    num_tracks  INTEGER NOT NULL,
    cover_art   BLOB
);
CREATE TABLE IF NOT EXISTS track(
    track_id        INTEGER NOT NULL PRIMARY KEY,
    track_path      TEXT    NOT NULL,
    track_title     TEXT    NOT NULL,
    track_album     INTEGER REFERENCES album(album_id) ON UPDATE CASCADE ON DELETE SET NULL,
    track_artist    INTEGER REFERENCES artist(artist_id) ON UPDATE CASCADE ON DELETE SET NULL,
    track_mbid      TEXT,
    disc_num        INTEGER NOT NULL,
    track_num       INTEGER NOT NULL
);
COMMIT TRANSACTION;