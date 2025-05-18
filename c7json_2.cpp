#include <c7app.hpp>
#include <c7json.hpp>
#include <c7nseq/io.hpp>
#include <c7nseq/push.hpp>
#include <c7nseq/transform.hpp>
#include <c7nseq/vector.hpp>
#include <c7utils/time.hpp>


using c7::p_;
using c7::P_;


/*[c7json:define]

// 楽曲
Song {
    str		title;		// 曲名
    int		duration_s;	// 演奏時間
    bin		audio;		// オーディオデータ(バイト列)
    bool	favorite;	// お気に入り
}

// アルバム
Album {
    str		title;		// アルバムタイトル
    str		artist;		// アーティスト名
    usec	release;	// リリース日時
    [Song]	songs;		// 楽曲
}

// ライブラリ
Library {
    [Album]	albums;		// アルバムリスト
    {usec -> str} history;
}

*/


//[c7json:begin]

struct Song: public c7::json_object {
    c7::json_str title;
    c7::json_int duration_s;
    c7::json_bin audio;
    c7::json_bool favorite;

    using c7::json_object::json_object;

    template <typename T0,
              typename T1=c7::json_int,
              typename T2=c7::json_bin,
              typename T3=c7::json_bool>
    explicit Song(T0&& a_title,
                  T1&& a_duration_s=T1(),
                  T2&& a_audio=T2(),
                  T3&& a_favorite=T3()):
	title(std::forward<T0>(a_title)),
	duration_s(std::forward<T1>(a_duration_s)),
	audio(std::forward<T2>(a_audio)),
	favorite(std::forward<T3>(a_favorite)) {}

    c7json_init(
        c7json_member(title),
        c7json_member(duration_s),
        c7json_member(audio),
        c7json_member(favorite),
        )
};

struct Album: public c7::json_object {
    c7::json_str title;
    c7::json_str artist;
    c7::json_usec release;
    c7::json_array<Song> songs;

    using c7::json_object::json_object;

    template <typename T0,
              typename T1=c7::json_str,
              typename T2=c7::json_usec,
              typename T3=c7::json_array<Song>>
    explicit Album(T0&& a_title,
                   T1&& a_artist=T1(),
                   T2&& a_release=T2(),
                   T3&& a_songs=T3()):
	title(std::forward<T0>(a_title)),
	artist(std::forward<T1>(a_artist)),
	release(std::forward<T2>(a_release)),
	songs(std::forward<T3>(a_songs)) {}

    c7json_init(
        c7json_member(title),
        c7json_member(artist),
        c7json_member(release),
        c7json_member(songs),
        )
};

struct Library: public c7::json_object {
    c7::json_array<Album> albums;
    c7::json_dict<c7::json_usec, c7::json_str> history;

    using c7::json_object::json_object;

    template <typename T0,
              typename T1=c7::json_dict<c7::json_usec, c7::json_str>>
    explicit Library(T0&& a_albums,
                     T1&& a_history=T1()):
	albums(std::forward<T0>(a_albums)),
	history(std::forward<T1>(a_history)) {}

    c7json_init(
        c7json_member(albums),
        c7json_member(history),
        )
};

//[c7json:end]


static void init_lib(Library& lib, const std::string& path)
{
    {
	Album a{
	    "Perfect Strangers",
	    "Deep Purple",
	    c7::make_usec().year(1984).make(),
	};
	{
	    auto& s = a.songs.extend();
	    s.title = "Hungry Daze";
 	    "hungry daze" | c7::nseq::push_back(s.audio());
	    s.duration_s = 501;
	    s.favorite   = false;
	}
	lib.albums.push_back(std::move(a));
    }

    {
	auto& a = lib.albums.extend();
	a.title = "Asia";
	a.artist = "Asia";
	a.release = c7::make_usec().year(1982).make();
#if 0
	{
	    Song s;
	    s.title = "Heat of the moment";
	    s.duration_s = 228;
# if 1
 	    "heat of the moment" | c7::nseq::push_back(s.audio());
	    c7::nseq::mmap_r<uint8_t>("hotm.wav") | c7::nseq::push_back(s.audio());
# else
	    s.audio = c7::nseq::mmap_r<uint8_t>("hotm.wav") | c7::nseq::to_vector();
# endif
	    s.favorite   = true;
	    a.songs.push_back(std::move(s));
	}
#else
	a.songs.push_back(Song{
		"Heat of the moment",
		    228,
# if 1
		    "heat of the moment"
		    | c7::nseq::transform([](auto c){ return static_cast<uint8_t>(c); })
		    | c7::nseq::to_vector(),
# else
		    c7::nseq::mmap_r<uint8_t>("hotm.wav") | c7::nseq::to_vector(),
# endif
		    true});
#endif
    }

    if (auto res = c7::json_dump(lib, path, 2); !res) {
	c7error(res);
    }
}


int main(int argc, char **argv)
{
    // prog lib_path album_name song_name add_data
    if (argc != 5) {
	c7exit("Usage: %{} Path Album Song Append_Data", c7::app::progname);
    }
    std::string path  = *++argv;
    std::string album = *++argv;
    std::string song  = *++argv;
    std::string data  = *++argv;

    Library lib;

    if (auto res = c7::json_load(lib, path); !res) {
	if (!res.has_what(ENODATA)) {
	    c7error(res);
	}
	init_lib(lib, path);
    } else {
	if (lib.albums.empty()) {
	    init_lib(lib, path);
	}
    }

    for (const auto& [t, n]: lib.history) {
	p_("history: %{t%Y %m/%d %H:%M:%S}, %{}", t, n);
    }

    for (auto& a: lib.albums) {
	if (a.title() != album) {
	    continue;
	}
	p_("Hit! album: title:%{}, artist:%{}, release:%{t%Y}", album, a.artist, a.release);
	for (auto& s: a.songs) {
	    if (s.title() != song) {
		continue;
	    }
	    p_("Hit! song: title:%{}, duration_s:%{}", s.title, s.duration_s);
	    auto& d = s.audio();
	    for (auto c: data) {
		d.push_back(c);
	    }
	    if (auto res = c7::json_dump(lib, path, 2); !res) {
		c7error(res);
	    }
	    c7exit("updated");
	}
    }
}
