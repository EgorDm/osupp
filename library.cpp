#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "database.h"

namespace py = pybind11;
using namespace std;

PYBIND11_MODULE(osupp, m) {
    m.doc() = "Library with tools for processing osu files";

    py::class_<OsuDB>(m, "OsuDB")
            .def(py::init<const std::string &>())
            .def("read", &OsuDB::read)
            .def_readonly("entries", &OsuDB::entries);

    py::class_<BeatmapEntry>(m, "BeatmapEntry")
            .def(py::init<>())
            .def_readwrite("title", &BeatmapEntry::title);
}
