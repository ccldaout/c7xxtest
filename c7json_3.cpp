#include <c7app.hpp>
#include <c7json.hpp>
#include <c7nseq/io.hpp>
#include <c7nseq/push.hpp>
#include <c7nseq/transform.hpp>
#include <c7nseq/vector.hpp>
#include <c7utils/time.hpp>


using c7::p_;
using c7::P_;


struct SongID_tag {};
struct AlbumID_tag {};

/*[c7json:define]

// ID
SongID = tagged_int<SongID_tag>;
AlbumID = tagged_int<AlbumID_tag>;
//AlbumID { int v; }

*/


/*[c7json:define]
// 楽曲
Song! {
    SongID	id;		// 楽曲ID
    str		title;		// 曲名
    int		duration_s;	// 演奏時間
    bool	favorite;	// お気に入り
    bin		audio;		// オーディオデータ(バイト列)
}

// 楽曲
OldSong {
    SongID	id;		// 楽曲ID
    str		title;		// 曲名
    int		duration_s;	// 演奏時間
    bin		audio;		// オーディオデータ(バイト列)
    bool	favorite;	// お気に入り
}

// アルバム
Album {
    AlbumID	id;		// アルバムID
    str		title;		// アルバムタイトル
    str		artist;		// アーティスト名
    usec	release;	// リリース日時
    [SongID]	songs;		// 楽曲
}

// ライブラリ
Library {
    {SongID -> Song}	song_db;	// 楽曲DB
    {AlbumID -> Album}	album_db;	// アルバムDB
    {usec -> <AlbumID, SongID>}	history;
    {<AlbumID, SongID> -> int} price;
}

*/


//[c7json:begin]

using SongID = c7::json_tagged_int<SongID_tag>;

using AlbumID = c7::json_tagged_int<AlbumID_tag>;

//[c7json:end]

#if 0
namespace std {
template <>
struct hash<AlbumID> {
    size_t operator()(const AlbumID& s) const {
	return hash<decltype(s.v)>()(s.v);
    }
};
}
#endif

//[c7json:begin]

struct Song: public c7::json_struct {
    SongID id;
    c7::json_str title;
    c7::json_int duration_s;
    c7::json_bool favorite;
    c7::json_bin audio;

    using c7::json_struct::json_struct;

    template <typename T0,
              typename T1=c7::json_str,
              typename T2=c7::json_int,
              typename T3=c7::json_bool,
              typename T4=c7::json_bin>
    explicit Song(T0&& a_id,
                  T1&& a_title=T1(),
                  T2&& a_duration_s=T2(),
                  T3&& a_favorite=T3(),
                  T4&& a_audio=T4()):
	id(std::forward<T0>(a_id)),
	title(std::forward<T1>(a_title)),
	duration_s(std::forward<T2>(a_duration_s)),
	favorite(std::forward<T3>(a_favorite)),
	audio(std::forward<T4>(a_audio)) {}

    bool operator==(const Song& o) const {
        return (id == o.id &&
                title == o.title &&
                duration_s == o.duration_s &&
                favorite == o.favorite &&
                audio == o.audio);
    }

    bool operator!=(const Song& o) const { return !(*this == o); }

    c7json_init(
        c7json_member(id),
        c7json_member(title),
        c7json_member(duration_s),
        c7json_member(favorite),
        c7json_member(audio),
        )
};

struct OldSong: public c7::json_object {
    SongID id;
    c7::json_str title;
    c7::json_int duration_s;
    c7::json_bin audio;
    c7::json_bool favorite;

    using c7::json_object::json_object;

    template <typename T0,
              typename T1=c7::json_str,
              typename T2=c7::json_int,
              typename T3=c7::json_bin,
              typename T4=c7::json_bool>
    explicit OldSong(T0&& a_id,
                     T1&& a_title=T1(),
                     T2&& a_duration_s=T2(),
                     T3&& a_audio=T3(),
                     T4&& a_favorite=T4()):
	id(std::forward<T0>(a_id)),
	title(std::forward<T1>(a_title)),
	duration_s(std::forward<T2>(a_duration_s)),
	audio(std::forward<T3>(a_audio)),
	favorite(std::forward<T4>(a_favorite)) {}

    bool operator==(const OldSong& o) const {
        return (id == o.id &&
                title == o.title &&
                duration_s == o.duration_s &&
                audio == o.audio &&
                favorite == o.favorite);
    }

    bool operator!=(const OldSong& o) const { return !(*this == o); }

    c7json_init(
        c7json_member(id),
        c7json_member(title),
        c7json_member(duration_s),
        c7json_member(audio),
        c7json_member(favorite),
        )
};

struct Album: public c7::json_object {
    AlbumID id;
    c7::json_str title;
    c7::json_str artist;
    c7::json_usec release;
    c7::json_array<SongID> songs;

    using c7::json_object::json_object;

    template <typename T0,
              typename T1=c7::json_str,
              typename T2=c7::json_str,
              typename T3=c7::json_usec,
              typename T4=c7::json_array<SongID>>
    explicit Album(T0&& a_id,
                   T1&& a_title=T1(),
                   T2&& a_artist=T2(),
                   T3&& a_release=T3(),
                   T4&& a_songs=T4()):
	id(std::forward<T0>(a_id)),
	title(std::forward<T1>(a_title)),
	artist(std::forward<T2>(a_artist)),
	release(std::forward<T3>(a_release)),
	songs(std::forward<T4>(a_songs)) {}

    bool operator==(const Album& o) const {
        return (id == o.id &&
                title == o.title &&
                artist == o.artist &&
                release == o.release &&
                songs == o.songs);
    }

    bool operator!=(const Album& o) const { return !(*this == o); }

    c7json_init(
        c7json_member(id),
        c7json_member(title),
        c7json_member(artist),
        c7json_member(release),
        c7json_member(songs),
        )
};

struct Library: public c7::json_object {
    c7::json_dict<SongID, Song> song_db;
    c7::json_dict<AlbumID, Album> album_db;
    c7::json_dict<c7::json_usec, c7::json_pair<AlbumID, SongID>> history;
    c7::json_dict<c7::json_pair<AlbumID, SongID>, c7::json_int> price;

    using c7::json_object::json_object;

    template <typename T0,
              typename T1=c7::json_dict<AlbumID, Album>,
              typename T2=c7::json_dict<c7::json_usec, c7::json_pair<AlbumID, SongID>>,
              typename T3=c7::json_dict<c7::json_pair<AlbumID, SongID>, c7::json_int>>
    explicit Library(T0&& a_song_db,
                     T1&& a_album_db=T1(),
                     T2&& a_history=T2(),
                     T3&& a_price=T3()):
	song_db(std::forward<T0>(a_song_db)),
	album_db(std::forward<T1>(a_album_db)),
	history(std::forward<T2>(a_history)),
	price(std::forward<T3>(a_price)) {}

    bool operator==(const Library& o) const {
        return (song_db == o.song_db &&
                album_db == o.album_db &&
                history == o.history &&
                price == o.price);
    }

    bool operator!=(const Library& o) const { return !(*this == o); }

    c7json_init(
        c7json_member(song_db),
        c7json_member(album_db),
        c7json_member(history),
        c7json_member(price),
        )
};

//[c7json:end]


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
}

int main(int argc, char **argv)
{
    Library lb;
    init_data(lb);
}
