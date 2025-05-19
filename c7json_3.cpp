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
    c7::json_bin audio;
    c7::json_bool favorite;

    using c7::json_struct::json_struct;

    template <typename T0,
              typename T1=c7::json_str,
              typename T2=c7::json_int,
              typename T3=c7::json_bin,
              typename T4=c7::json_bool>
    explicit Song(T0&& a_id,
                  T1&& a_title=T1(),
                  T2&& a_duration_s=T2(),
                  T3&& a_audio=T3(),
                  T4&& a_favorite=T4()):
	id(std::forward<T0>(a_id)),
	title(std::forward<T1>(a_title)),
	duration_s(std::forward<T2>(a_duration_s)),
	audio(std::forward<T3>(a_audio)),
	favorite(std::forward<T4>(a_favorite)) {}

    bool operator==(const Song& o) const {
        return (id == o.id &&
                title == o.title &&
                duration_s == o.duration_s &&
                audio == o.audio &&
                favorite == o.favorite);
    }

    bool operator!=(const Song& o) const { return !(*this == o); }

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


int main(int argc, char **argv)
{
    Library lb;
    AlbumID a{1};
    SongID s{20};
    auto it = lb.price.find(c7::json_pair<AlbumID, SongID>(a, s));
    it == lb.price.end();
}
