#include <c7app.hpp>
#include <c7json.hpp>
#include <c7nseq/push.hpp>
#include <c7utils/time.hpp>


using c7::p_;
using c7::P_;


struct Song: public c7::json_object {
    c7::json_str	title;
    c7::json_int	duration_s;
    c7::json_bin	audio;
    c7::json_bool	favorite;

    c7json_init(
	c7json_member(title),
	c7json_member(duration_s),
	c7json_member(audio),
	c7json_member(favorite),
	)
};


struct Album: public c7::json_object {
    c7::json_str	title;
    c7::json_str	artist;
    c7::json_usec	release;
    c7::json_array<Song> songs;

    c7json_init(
	c7json_member(title),
	c7json_member(artist),
	c7json_member(release),
	c7json_member(songs),
	)
};


struct Library: public c7::json_object {
    c7::json_array<Album> albums;

    c7json_init(
	c7json_member(albums)
	)
};


static void init_lib(Library& lib, const std::string& path)
{
    {
	Album a;
	a.title = "Perfect Strangers";
	a.artist = "Deep Purple";
	a.release = c7::make_usec().year(1984).make();
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
	{
	    Song s;
	    s.title = "Heat of the moment";
	    s.duration_s = 228;
 	    "heat of the moment" | c7::nseq::push_back(s.audio());
	    s.favorite   = true;
	    a.songs.push_back(std::move(s));
	}
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
