#include <c7app.hpp>
#include <c7json.hpp>
#include <c7nseq/push.hpp>
#include <c7utils/time.hpp>


using c7::p_;
using c7::P_;


struct Song: public c7::json_object {
    c7::json_str	title;
    c7::json_int	duration_m;
    c7::json_bin	audio;

    Song() {
	c7json_member(title);
	c7json_member(duration_m);
	c7json_member(audio);
    }
};


struct Album: public c7::json_object {
    c7::json_str	title;
    c7::json_str	author;
    c7::json_array<Song> songs;
    c7::json_int	price_yen;
    c7::json_usec	release;

    Album() {
	c7json_member(title);
	c7json_member(author);
	c7json_member(songs);
	c7json_member(price_yen);
	c7json_member(release);
    }
};


struct Library: public c7::json_object {
    c7::json_array<Album> albums;

    Library() {
	c7json_member(albums);
    }
};


static void init_lib(Library& lib, const std::string& path)
{
    {
	Album a;
	a.title = "Perfect Strangers";
	a.author = "Deep Purple";
	{
	    auto& s = a.songs.extend();
	    s.title = "Hungry Daze";
	    s.duration_m = 5;
	    s.audio = std::vector<uint8_t>{};
	}
	lib.albums.push_back(std::move(a));
    }

    {
	auto& a = lib.albums.extend();
	a.title = "Asia";
	a.author = "Asia";
	{
	    Song s;
	    s.title = "HOTM";
	    s.duration_m = 3;
	    s.audio = std::vector<uint8_t>{};
	    "heat of the moment" | c7::nseq::push_back(s.audio());
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
	p_("Hit! album: title:%{}, author:%{}, release:%{t%Y}", album, a.author(), a.release());
	for (auto& s: a.songs) {
	    if (s.title() != song) {
		continue;
	    }
	    p_("Hit! song: title:%{}, duration_m:%{}", s.title(), s.duration_m());
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
