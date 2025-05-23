#include <c7app.hpp>
#include <c7nseq/io.hpp>
#include <c7nseq/push.hpp>
#include <c7nseq/transform.hpp>
#include <c7nseq/vector.hpp>
#include <c7path.hpp>
#include <c7utils/time.hpp>
#include "c7json_3.hpp"


//[c7json:begin]

//[c7json:end]

//[c7json:begin]

bool Song::operator==(const Song& o) const
{
    return (id == o.id &&
                title == o.title &&
                duration_s == o.duration_s &&
                favorite == o.favorite &&
                audio == o.audio);
}

c7json_init_implement(
    Song,
    c7json_member(id),
    c7json_member(title),
    c7json_member(duration_s),
    c7json_member(favorite),
    c7json_member(audio),
)

bool Album::operator==(const Album& o) const
{
    return (id == o.id &&
                title == o.title &&
                artist == o.artist &&
                release == o.release &&
                songs == o.songs);
}

c7json_init_implement(
    Album,
    c7json_member(id),
    c7json_member(title),
    c7json_member(artist),
    c7json_member(release),
    c7json_member(songs),
)

bool Library::operator==(const Library& o) const
{
    return (song_db == o.song_db &&
                album_db == o.album_db &&
                history == o.history &&
                price == o.price &&
                map == o.map);
}

c7json_init_implement(
    Library,
    c7json_member(song_db),
    c7json_member(album_db),
    c7json_member(history),
    c7json_member(price),
    c7json_member(map),
)

//[c7json:end]


using c7::p_;
using c7::P_;


static int duration(int min, int sec)
{
    return min * 60 + sec;
}


static c7::usec_t release(int year)
{
    return c7::make_usec().year(year).make();
}


static c7::usec_t at_time()
{
    static uint32_t n = 100;
    return c7::make_usec().now().make() - (n--) * 789 * C7_TIME_S_us;
}


static void init_data(Library& lb)
{
    // {SongID -> Song}	song_db;	// 楽曲DB

    lb.song_db
	.insert_or_assign( 0, Song{0, "Child's Anthem", duration(2, 45), false})	//  0: Toto 1
	.insert_or_assign( 1, Song{1, "Rosana", duration(5, 31), false})		//  1: Toto IV
	.insert_or_assign( 2, Song{2, "Africa", duration(4, 55), true})			//  2: ,,
	.insert_or_assign( 3, Song{3, "Pamela", duration(5, 10), true})			//  3: The Seventh One
	.insert_or_assign( 4, Song{4, "Heat Of The Moment", duration(3, 48), true})	//  4: Asia
	.insert_or_assign( 5, Song{5, "Only Time Will Tell", duration(4, 45), true})	//  5: ,,
	.insert_or_assign( 6, Song{6, "Don't Cry", duration(3, 30), true})		//  6: Alpha
	.insert_or_assign( 7, Song{7, "Open Your Eyes", duration(6, 24), true})		//  7: ,,
	.insert_or_assign( 8, Song{8, "Go", duration(4, 7), false})			//  8: Astra
	.insert_or_assign( 9, Song{9, "Rock and Roll Dream", duration(6, 49), true})	//  9: ,,
	.insert_or_assign(10, Song{10, "After the War", duration(5, 8), false})		// 10: ,,
	.insert_or_assign(11, Song{11, "Never Again", duration(4, 56), false})		// 11: Phoenix
	.insert_or_assign(12, Song{12, "An Extraordinay Life", duration(4, 57), true})	// 12: ,,
	;

    // {AlbumID -> Album}	album_db;	// アルバムDB

    lb.album_db
	.insert_or_assign(200, Album{200, "Toto", "Toto", release(1978), {SongID(0)}})
	.insert_or_assign(201, Album{201, "Toto IV", "Toto", release(1982), {SongID(1), SongID(2)}})
	.insert_or_assign(202, Album{202, "The Seventh One", "Toto", release(1988), {SongID(3)}})
	.insert_or_assign(203, Album{203, "Asia", "Asia", release(1982), {SongID(4), SongID(5)}})
	.insert_or_assign(203, Album{203, "Alpha", "Asia", release(1983), {SongID(6), SongID(7)}})
	.insert_or_assign(204, Album{204, "Astra", "Asia", release(1985), {SongID(8), SongID(9), SongID(10)}})
	.insert_or_assign(205, Album{205, "Phoenix", "Asia", release(2008), {SongID(11), SongID(12)}})
	;

    // {usec -> <AlbumID, SongID>}	history;

    using alsong = c7::json_pair<AlbumID, SongID>;

    lb.history
	.insert_or_assign(at_time(), alsong(200, 0))
	.insert_or_assign(at_time(), alsong(204, 10))
	;

    // {<AlbumID, SongID> -> int} price;
    
    lb.price
	.insert_or_assign(alsong(200, 0), 200)
	.insert_or_assign(alsong(201, 1), 200)
	.insert_or_assign(alsong(201, 2), 250)
	;

    // {Location -> SongID} map;
    lb.map
	.insert_or_assign(Location{1, 2, 3}, 9)
	.insert_or_assign(Location{7, 3, 5}, 3)
	;
}


static std::pair<std::string, std::string>
find_music(const Library& lib, AlbumID aid, SongID sid)
{
    std::string album = "unknown";
    std::string song  = "unknown";
    if (auto it = lib.album_db.find(aid); it != lib.album_db.end()) {
	album = (*it).second.title;
    }
    if (auto it = lib.song_db.find(sid); it != lib.song_db.end()) {
	song = (*it).second.title;
    }
    return {album, song};
}


static SongID
find_by_location(const Library& lib, int x, int y, int z)
{
    if (auto it = lib.map.find(Location(x, y, z)); it != lib.map.end()) {
	return (*it).second;
    } else {
	return SongID{-1};
    }
}


static void show_history(const Library& lib)
{
    for (auto [tm, id_pair]: lib.history) {
	auto [aid, sid] = id_pair();
	auto [album, song] = find_music(lib, aid, sid);
	p_("%{t%Y %m/%d %H:%M:%S} %{}:%{} %{}:%{}", tm, aid, album, sid, song);
    }
}


int main(int argc, char **argv)
{
    auto db_path = c7::path::untildize("~/tmp/aa.json");

    Library lib;
    if (auto res = c7::json_load(lib, db_path); !res && !res.has_what(ENOENT)) {
	c7error(res);
    }
    if (lib.song_db.empty()) {
	init_data(lib);
	if (auto res = c7::json_dump(lib, db_path, 2); !res) {
	    c7error(res);
	}
	p_("initialized: %{}", db_path);
    }

    show_history(lib);

    if (argc == 3) {
	AlbumID aid = std::stol(argv[1]);
	SongID  sid = std::stol(argv[2]);
	auto [album, song] = find_music(lib, aid, sid);
	if (album.empty() || song.empty()) {
	    c7exit("not found");
	}

	using alsong = c7::json_pair<AlbumID, SongID>;
	lib.history.insert_or_assign(c7::time_us(), alsong(aid, sid));
	
	if (auto res = c7::json_dump(lib, db_path, 2); !res) {
	    c7error(res);
	}
	p_("updated");
    } else if (argc == 4) {
	int x = std::stol(argv[1]);
	int y = std::stol(argv[2]);
	int z = std::stol(argv[3]);
	auto id = find_by_location(lib, x, y, z);
	p_("(%{}, %{}, %{}) -> SongID: %{}", x, y, z, id);
    }
}
