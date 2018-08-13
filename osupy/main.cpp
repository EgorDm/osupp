#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <beatmap_io.h>
#include <database.h>

namespace py = pybind11;
using namespace std;
using namespace osupp;

PYBIND11_MODULE(osupp, m) {
    m.doc() = "Library with tools for processing osu files";

    py::class_<OsuDB>(m, "OsuDB")
            .def(py::init<const std::string &>())
            .def("read", &OsuDB::read)
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

    py::class_<HitObject>(m, "HitObject")
            .def_readwrite("pos", &HitObject::pos)
            .def_readwrite("time", &HitObject::time)
            .def_readwrite("new_combo", &HitObject::new_combo);

    py::class_<HitCircle, HitObject>(m, "HitCircle");

    py::class_<Slider, HitObject>(m, "Slider")
            .def_readwrite("repeat", &Slider::repeat)
            .def_readwrite("pixel_length", &Slider::pixel_length)
            .def_readwrite("curve", &Slider::curve);

    py::class_<Spinner, HitObject>(m, "Spinner")
            .def_readwrite("end_time", &Spinner::end_time);

    py::class_<Curve>(m, "Curve")
            .def("position_at", &Curve::position_at)
            .def("length", &Curve::length)
            .def("get_type", &Curve::get_type)
            .def("get_points", &Curve::get_points)
            .def("set_points", &Curve::set_points);

    py::enum_<Curve::CurveType>(m, "CurveType")
            .value("None", Curve::CurveType::None)
            .value("Linear", Curve::CurveType::Linear)
            .value("Bezier", Curve::CurveType::Bezier)
            .value("Perfect", Curve::CurveType::Perfect)
            .value("Catmull", Curve::CurveType::Catmull)
            .export_values();


    m.def("read_beatmap", &read_beatmap,
          "Reads beatmap data using specified flags.",
          py::arg("file"),
          py::arg("read_flags") = ALL_SECTIONS
    );
}
