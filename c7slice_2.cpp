#include <c7app.hpp>
#include <c7nseq/range.hpp>
#include <c7nseq/vector.hpp>
#include <c7nseq/zip.hpp>
#include <c7slice.hpp>


using c7::p_;
using c7::P_;
namespace ns = c7::nseq;


template <typename C>
static void put(const C& c)
{
    for (const auto& v: c) {
	P_(" %{}", v);
    }
    p_("");
}


static void test(const std::vector<int>& base,
		 ptrdiff_t off, ptrdiff_t gap, size_t n,
		 const std::vector<int>& expect)
{
    auto cond = c7::format("off:%{}, gap:%{}, n:%{}", off, gap, n);
    auto c = c7::make_slice(base, off, gap, n);

    auto v = c|ns::to_vector();
    bool ok = (expect == v);

    P_(  "  %{}: expect:", cond);
    put(expect);
    P_("%{c} %{*c}:       :", ok ? ' ' : 'X', cond.size(), ' ');
    put(c);
}


int main(int argc, char **argv)
{
    std::vector<int> base = ns::range(10) | ns::to_vector();

    //                     {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}
    test(base,  0,  2,  4, {0,    2,    4,    6         });
    test(base,  0,  2,  5, {0,    2,    4,    6,    8   });
    test(base,  0,  2,  6, {0,    2,    4,    6,    8   });
    test(base,  0,  3,  3, {0,       3,       6         });
    test(base,  0,  3,  4, {0,       3,       6,       9});
    test(base,  0,  3,  5, {0,       3,       6,       9});
    test(base,  0,  5,  0, {                            }); 
    test(base,  0,  5,  1, {0                           }); 
    test(base,  0,  5,  2, {0,             5            }); 
    test(base,  0,  5,  3, {0,             5            }); 
    test(base,  0, 10,  1, {0,                          }); 
    test(base,  3,  2,  3, {         3,    5,    7,     });
    test(base,  3,  2,  4, {         3,    5,    7,    9});
    test(base,  3,  2,  4, {         3,    5,    7,    9});
    test(base,  3,  4,  1, {         3,                 });
    test(base,  3,  4,  2, {         3,          7,     });
    test(base,  3,  4,  3, {         3,          7,     });

    //                     {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}
    //                    -10 -9 -8 -7 -6 -5 -4 -3 -2 -1
    test(base, -7,  2,  3, {         3,    5,    7,     });
    test(base, -7,  2,  4, {         3,    5,    7,    9});
    test(base, -7,  2,  4, {         3,    5,    7,    9});

    //                     {9, 8, 7, 6, 5, 4, 3, 2, 1, 0});
    test(base,  8, -1,  4, {   8, 7, 6, 5               });
    test(base,  8, -1,  8, {   8, 7, 6, 5, 4, 3, 2, 1   });
    test(base,  8, -1,  9, {   8, 7, 6, 5, 4, 3, 2, 1, 0});
    test(base,  8, -1, 10, {   8, 7, 6, 5, 4, 3, 2, 1, 0});
    test(base,  8, -4,  2, {   8,          4            });
    test(base,  8, -4,  3, {   8,          4,          0});
    test(base,  8, -4,  4, {   8,          4,          0});

    //                     {9, 8, 7, 6, 5, 4, 3, 2, 1, 0});
    //                     -1 -2 -3 -4 -5 -6 -7 -8 -9 -10
    test(base, -2, -1,  4, {   8, 7, 6, 5               });
    test(base, -2, -1,  8, {   8, 7, 6, 5, 4, 3, 2, 1   });
    test(base, -2, -1,  9, {   8, 7, 6, 5, 4, 3, 2, 1, 0});
    test(base, -2, -1, 10, {   8, 7, 6, 5, 4, 3, 2, 1, 0});
    test(base, -2, -4,  2, {   8,          4            });
    test(base, -2, -4,  3, {   8,          4,          0});
    test(base, -2, -4,  4, {   8,          4,          0});

    test(base, -1, -1,  5, {9, 8, 7, 6, 5               });
    test(base, -1, -1,  9, {9, 8, 7, 6, 5, 4, 3, 2, 1   });
    test(base, -1, -1, 10, {9, 8, 7, 6, 5, 4, 3, 2, 1, 0});
    test(base, -1, -1, 11, {9, 8, 7, 6, 5, 4, 3, 2, 1, 0});

}
