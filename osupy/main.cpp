#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <beatmap_io.h>
#include <database.h>

namespace py = pybind11;
using namespace std;
using namespace osupp;

PYBIND11_MODULE(osupy, m) {
    m.doc() = R"pbdoc(
        osupp
        -----------------------

        .. currentmodule:: osupy

        .. autosummary::
           :toctree: _generate

           read_beatmap
           Curve
           HitObject
    )pbdoc";

    py::class_<OsuDB>(m, "OsuDB")
            .def(py::init<const std::string &>())
            .def("read", &OsuDB::read)
            .def_readonly("version", &OsuDB::version)
            .def_readonly("user", &OsuDB::user)
            .def_readonly("entries", &OsuDB::entries);

    py::class_<BeatmapEntry>(m, "BeatmapEntry")
            .def(py::init<>())
            .def_readwrite("title", &BeatmapEntry::title)
            .def_readwrite("artist", &BeatmapEntry::artist)
            .def_readwrite("creator", &BeatmapEntry::creator)
            .def_readwrite("version", &BeatmapEntry::version)
            .def_readwrite("audio_file", &BeatmapEntry::audio_file)
            .def_readwrite("osu_file", &BeatmapEntry::osu_file)
            .def_readwrite("folder_name", &BeatmapEntry::folder_name)
            .def_readwrite("mode", &BeatmapEntry::mode)
            .def_readwrite("ranked", &BeatmapEntry::ranked)
            .def_readwrite("id", &BeatmapEntry::id)
            .def_readwrite("set_id", &BeatmapEntry::set_id)
            .def_readwrite("ar", &BeatmapEntry::ar)
            .def_readwrite("cs", &BeatmapEntry::cs)
            .def_readwrite("hp", &BeatmapEntry::hp)
            .def_readwrite("od", &BeatmapEntry::od)
            .def_readwrite("std_diffs", &BeatmapEntry::std_diffs)
            .def_readwrite("taiko_diffs", &BeatmapEntry::taiko_diffs)
            .def_readwrite("ctb_diffs", &BeatmapEntry::ctb_diffs)
            .def_readwrite("mania_diffs", &BeatmapEntry::mania_diffs)
            .def_readwrite("time_drain", &BeatmapEntry::time_drain)
            .def_readwrite("time_total", &BeatmapEntry::time_total)
            .def_readwrite("timingpoints", &BeatmapEntry::timingpoints)
            .def("get_path", &BeatmapEntry::get_path);

    py::class_<Beatmap>(m, "Beatmap")
            .def(py::init<>())
            .def_readwrite("title", &Beatmap::title)
            .def_readwrite("artist", &Beatmap::artist)
            .def_readwrite("creator", &Beatmap::creator)
            .def_readwrite("version", &Beatmap::version)
            .def_readwrite("id", &Beatmap::id)
            .def_readwrite("set_id", &Beatmap::set_id)
            .def_readwrite("ar", &Beatmap::ar)
            .def_readwrite("cs", &Beatmap::cs)
            .def_readwrite("hp", &Beatmap::hp)
            .def_readwrite("od", &Beatmap::od)
            .def_readwrite("slider_multiplayer", &Beatmap::slider_multiplayer)
            .def_readwrite("slider_tick_rate", &Beatmap::slider_tick_rate)
            .def_readwrite("timingpoints", &Beatmap::timingpoints)
            .def_readwrite("hitobjects", &Beatmap::hitobjects);

    py::class_<TimingPoint>(m, "TimingPoint")
            .def(py::init<>())
            .def_readwrite("offset", &TimingPoint::offset)
            .def_readwrite("meter", &TimingPoint::meter)
            .def_readwrite("slider_multiplayer", &TimingPoint::slider_multiplayer);

    py::class_<KeyTimingPoint, TimingPoint>(m, "KeyTimingPoint")
            .def(py::init<>())
            .def_readwrite("mpb", &KeyTimingPoint::mpb);

    py::class_<InheritedTimingPoint, TimingPoint>(m, "InheritedTimingPoint")
            .def(py::init<>())
            .def_readwrite("parent", &InheritedTimingPoint::parent);

    py::class_<Coordinate>(m, "Coordinate")
            .def(py::init<>())
            .def_readwrite("x", &Coordinate::x)
            .def_readwrite("y", &Coordinate::y);

    py::class_<HitObject>(m, "HitObject", R"pbdoc(
             Base hitobject class
             )pbdoc")
            .def_readwrite("pos", &HitObject::pos, R"pbdoc(
                Position of the histobject.

                x ranges from 0 to 512 pixels, inclusive, and y ranges from 0 to 384 pixels, inclusive.
            )pbdoc")
            .def_readwrite("time", &HitObject::time, R"pbdoc(
                time is an integral number of milliseconds from the beginning of the song, and specifies when the hit begins.
            )pbdoc")
            .def_readwrite("new_combo", &HitObject::new_combo, R"pbdoc(
                Wether hitobject starts new combo
            )pbdoc");

    py::class_<HitCircle, HitObject>(m, "HitCircle");

    py::class_<Slider, HitObject>(m, "Slider")
            .def_readwrite("repeat", &Slider::repeat)
            .def_readwrite("pixel_length", &Slider::pixel_length)
            .def_readwrite("curve", &Slider::curve);

    py::class_<Spinner, HitObject>(m, "Spinner")
            .def_readwrite("end_time", &Spinner::end_time);

    py::class_<Curve>(m, "Curve", R"pbdoc(
             Curve class that contains the curve information and helper methods to extract the curve properties and
             interpolated positions.
             )pbdoc")
            .def(py::init<Curve::CurveType, const std::vector<Coordinate> &, double>(),
                 py::arg("type"), py::arg("points"), py::arg("length"))
            .def("position_at", &Curve::position_at, R"pbdoc(
                Gets slider position at a normalized position between 0 and 1
            )pbdoc")
            .def("length", &Curve::length, R"pbdoc(
                Gets slider length in pixels. (Repeats are not included)
            )pbdoc")
            .def("get_type", &Curve::get_type, R"pbdoc(
                Gets the slider type
            )pbdoc")
            .def("get_points", &Curve::get_points, R"pbdoc(
                Gets a list of base points on the slider.
            )pbdoc")
            .def("set_points", &Curve::set_points, R"pbdoc(
                Sets a list of base points on the slider.
            )pbdoc");

    py::enum_<Curve::CurveType>(m, "CurveType")
            .value("None", Curve::CurveType::None)
            .value("Linear", Curve::CurveType::Linear)
            .value("Bezier", Curve::CurveType::Bezier)
            .value("Perfect", Curve::CurveType::Perfect)
            .value("Catmull", Curve::CurveType::Catmull)
            .export_values();


    m.attr("META_SECTION") = py::int_(META_SECTION);
    m.attr("DIFFICULTY_SECTION") = py::int_(DIFFICULTY_SECTION);
    m.attr("TIMING_SECTION") = py::int_(TIMING_SECTION);
    m.attr("HITOBJECT_SECTION") = py::int_(HITOBJECT_SECTION);
    m.attr("ALL_SECTIONS") = py::int_(ALL_SECTIONS);

    m.def("read_beatmap", &read_beatmap,
          R"pbdoc(
                Reads beatmap data using specified flags.

                Use following flags to specify reading mode:

                * META_SECTION
                * DIFFICULTY_SECTION
                * TIMING_SECTION
                * HITOBJECT_SECTION
                * ALL_SECTIONS

                Example code for reading only meta and timing of a beatmap.

                .. code-block:: python

                   beatmap = osupy.read_beatmap("./Cool Songs.osu", META_SECTION | DIFFICULTY_SECTION | TIMING_SECTION)

          )pbdoc",
          py::arg("file"),
          py::arg("read_flags") = ALL_SECTIONS
    );

#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}
