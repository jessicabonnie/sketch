#include "python/pysketch.h"
#include "sketch/setsketch.h"

PYBIND11_MODULE(sketch_ss, m) {
    m.doc() = "SetSketch python bindings"; // optional module docstring
    py::class_<EShortSetS> (m, "ShortSetSketch")
        .def(py::init<size_t>())
        .def(py::init<std::string>())
        .def("clear", &EShortSetS::clear, "Clear all entries.")
        .def("report", &EShortSetS::harmean, "Emit estimated cardinality.")
        .def("add", [](EShortSetS &h1, uint64_t v) {h1.add(v);}, "Add a (hashed) value to the sketch.")
        .def("addh", [](EShortSetS &h1, py::object p) {
            auto hv = py::hash(p);
            h1.addh(hv);
        }, "Hash a python object and add it to the sketch.")
        .def("jaccard_index", [](EShortSetS &h1, EShortSetS &h2) {
            auto triple = h1.alpha_beta_mu(h2);
            return std::max(0., (1. - std::get<0>(triple) - std::get<1>(triple)));
        })
        .def("union", [](const EShortSetS &h1, const EShortSetS &h2) {return h1 + h2;})
        //.def("union_size", [](const EShortSetS &h1, const EShortSetS &h2) {return h1.union_size(h2);})
        .def("__ior__", [](EShortSetS &lh, const EShortSetS &rh) {lh += rh; return lh;})
        .def("__or__", [](const EShortSetS &lh, const EShortSetS &rh) {return lh + rh;})
        .def("__eq__", [](const sketch::EShortSetS &h, const sketch::EShortSetS &h2) {
            return h == h2;
        }).def("__neq__", [](const sketch::EShortSetS &h, const sketch::EShortSetS &h2) {
            return !(h == h2);
        }).def("write", [](const sketch::EShortSetS &h, std::string path) {
            h.write(path);
        }).def("to_numpy", [](const sketch::EShortSetS &h) {
            return setsketch2np(h);
        });
    m.def("jaccard_index", [](EShortSetS &h1, EShortSetS &h2) {
            auto triple = h1.alpha_beta_mu(h2);
            return std::max(0., (1. - std::get<0>(triple) - std::get<1>(triple)));
        }, "Calculates jaccard indexes between two sketches")
    .def("ess_from_np", [](const py::array_t<uint32_t> &input, size_t ss=10, long double b=1.0006, long double a=.001) {
         EShortSetS ret(ss);
         auto ptr = input.data();
         for(ssize_t i = 0; i < input.size();ret.add(ptr[i++]));
         ret.getcard();
         return ret;
    }, py::arg("array"), py::arg("sketchsize") = 10, py::arg("b") = 1.0006, py::arg("a") = .001,
        py::return_value_policy::take_ownership, "Creates a SetSketch with 16-bit registers from a numpy array of 32-bit hashes.")
    .def("ess_from_np", [](const py::array_t<uint64_t> &input, size_t ss=10, long double b=1.0006, long double a=.001) {
         EShortSetS ret(ss);
         auto ptr = input.data();
         for(ssize_t i = 0; i < input.size();ret.add(ptr[i++]));
         ret.getcard();
         return ret;
     }, py::arg("array"), py::arg("sketchsize") = 10, py::arg("b") = 1.0006, py::arg("a") = .001,
        py::return_value_policy::take_ownership, "Creates a SetSketch with 16-bit registers from a numpy array of (unhashed) 64-bit integers");
    py::class_<EByteSetS> (m, "ByteSetSketch")
        .def(py::init<size_t>())
        .def(py::init<std::string>())
        .def("clear", &EByteSetS::clear, "Clear all entries.")
        .def("report", &EByteSetS::harmean, "Emit estimated cardinality.")
        .def("add", [](EByteSetS &h1, uint64_t v) {h1.add(v);}, "Add a (hashed) value to the sketch.")
        .def("addh", [](EByteSetS &h1, py::object p) {
            auto hv = py::hash(p);
            h1.addh(hv);
        }, "Hash a python object and add it to the sketch.")
        .def("jaccard_index", [](EByteSetS &h1, EByteSetS &h2) {
            auto triple = h1.alpha_beta_mu(h2);
            return std::max(0., (1. - std::get<0>(triple) - std::get<1>(triple)));
        })
        .def("union", [](const EByteSetS &h1, const EByteSetS &h2) {return h1 + h2;})
        //.def("union_size", [](const EByteSetS &h1, const EByteSetS &h2) {return h1.union_size(h2);})
        .def("__ior__", [](EByteSetS &lh, const EByteSetS &rh) {lh += rh; return lh;})
        .def("__or__", [](const EByteSetS &lh, const EByteSetS &rh) {return lh + rh;})
        .def("__eq__", [](const sketch::EByteSetS &h, const sketch::EByteSetS &h2) {
            return h == h2;
        }).def("__neq__", [](const sketch::EByteSetS &h, const sketch::EByteSetS &h2) {
            return !(h == h2);
        }).def("write", [](const sketch::EByteSetS &h, std::string path) {
            h.write(path);
        }).def("calc_card", [](const sketch::EByteSetS &h) {h.getcard();});
    m.def("jaccard_index", [](EByteSetS &h1, EByteSetS &h2) {
            auto triple = h1.alpha_beta_mu(h2);
            return std::max(0., (1. - std::get<0>(triple) - std::get<1>(triple)));
        }, "Calculates jaccard indexes between two sketches")
    .def("ebs_from_np", [](const py::array_t<uint32_t> &input, size_t ss=10, long double b=1.09, long double a=.08) {
         EByteSetS ret(ss);
         auto ptr = input.data();
         for(ssize_t i = 0; i < input.size();ret.add(ptr[i++]));
         ret.getcard();
         return ret;
    }, py::arg("array"), py::arg("sketchsize") = 10, py::arg("b") = 1.09, py::arg("a") = .08,
        py::return_value_policy::take_ownership, "Creates a SetSketch with 8-bit registers from a numpy array of 32-bit hashes.")
    .def("ebs_from_np", [](const py::array_t<uint64_t> &input, size_t ss=10, long double b=1.09, long double a=.08) {
         EByteSetS ret(ss);
         auto ptr = input.data();
         for(ssize_t i = 0; i < input.size();ret.add(ptr[i++]));
         ret.getcard();
         return ret;
     }, py::arg("array"), py::arg("sketchsize") = 10, py::arg("b") = 1.09, py::arg("a") = .08,
        py::return_value_policy::take_ownership, "Creates a SetSketch with 8-bit registers from a numpy array of (unhashed) 64-bit integers")
     .def("to_numpy", [](const sketch::EByteSetS &h) {
            return setsketch2np(h);
     });
    py::class_<CSetSketch<double>> (m, "CSetSketch")
        .def(py::init<size_t>())
        .def(py::init<std::string>())
        .def("clear", &CSetSketch<double>::clear, "Clear all entries.")
        .def("report", &CSetSketch<double>::cardinality, "Emit estimated cardinality.")
        .def("add", [](CSetSketch<double> &h1, uint64_t v) {h1.add(v);}, "Add a (hashed) value to the sketch.")
        .def("addh", [](CSetSketch<double> &h1, py::object p) {
            auto hv = py::hash(p);
            h1.addh(hv);
        }, "Hash a python object and add it to the sketch.")
        .def("jaccard_index", [](CSetSketch<double> &h1, CSetSketch<double> &h2) {
            return h1.jaccard_index(h2);
        })
        .def("union", [](const CSetSketch<double> &h1, const CSetSketch<double> &h2) {return h1 + h2;})
        .def("nmatch", [](const CSetSketch<double> &h, const CSetSketch<double> &o) {
            return h.shared_registers(o);
        })
        //.def("union_size", [](const CSetSketch<double> &h1, const CSetSketch<double> &h2) {return h1.union_size(h2);})
        .def("__ior__", [](CSetSketch<double> &lh, const CSetSketch<double> &rh) {lh += rh; return lh;})
        .def("__or__", [](const CSetSketch<double> &lh, const CSetSketch<double> &rh) {return lh + rh;})
        .def("__eq__", [](const sketch::CSetSketch<double> &h, const sketch::CSetSketch<double> &h2) {
            return h == h2;
        }).def("__neq__", [](const sketch::CSetSketch<double> &h, const sketch::CSetSketch<double> &h2) {
            return !(h == h2);
        }).def("write", [](const sketch::CSetSketch<double> &h, std::string path) {
            h.write(path);
        }).def("to_numpy", [](const sketch::CSetSketch<double> &h) {
            return setsketch2np(h);
        }).def("calc_card", [](const sketch::CSetSketch<double> &h) {h.getcard();});
    m.def("jaccard_index", [](CSetSketch<double> &h1, CSetSketch<double> &h2) {
            return h1.jaccard_index(h2);
        }, "Calculates jaccard indexes between two sketches")
    .def("css_from_np", [](const py::array_t<uint32_t> &input, size_t ss) {
         CSetSketch<double> ret(ss);
         auto ptr = input.data();
         for(ssize_t i = 0; i < input.size();ret.add(ptr[i++]));
         ret.getcard();
         return ret;
    }, py::arg("array"), py::arg("sketchsize") = 10,
        py::return_value_policy::take_ownership, "Creates a CSetSketch with double precision registers from a numpy array of 32-bit hashes.")
    .def("css_from_np", [](const py::array_t<uint64_t> &input, size_t ss) {
         CSetSketch<double> ret(ss);
         auto ptr = input.data();
         for(ssize_t i = 0; i < input.size();ret.add(ptr[i++]));
         ret.getcard();
         return ret;
     }, py::arg("array"), py::arg("sketchsize") = 10,
        py::return_value_policy::take_ownership, "Creates a CSetSketch with double precision registers from a numpy array of (unhashed) 64-bit integers");

    py::class_<CSetSketch<float>> (m, "FSetSketch")
        .def(py::init<size_t>())
        .def(py::init<std::string>())
        .def("clear", &CSetSketch<float>::clear, "Clear all entries.")
        .def("report", &CSetSketch<float>::cardinality, "Emit estimated cardinality.")
        .def("add", [](CSetSketch<float> &h1, uint64_t v) {h1.add(v);}, "Add a (hashed) value to the sketch.")
        .def("addh", [](CSetSketch<float> &h1, py::object p) {
            auto hv = py::hash(p);
            h1.addh(hv);
        }, "Hash a python object and add it to the sketch.")
        .def("jaccard_index", [](CSetSketch<float> &h1, CSetSketch<float> &h2) {
            return h1.jaccard_index(h2);
        })
        .def("union", [](const CSetSketch<float> &h1, const CSetSketch<float> &h2) {return h1 + h2;})
        .def("nmatch", [](const CSetSketch<float> &h, const CSetSketch<float> &o) {
            return h.shared_registers(o);
        })
        //.def("union_size", [](const CSetSketch<double> &h1, const CSetSketch<double> &h2) {return h1.union_size(h2);})
        .def("__ior__", [](CSetSketch<float> &lh, const CSetSketch<float> &rh) {lh += rh; return lh;})
        .def("__or__", [](const CSetSketch<float> &lh, const CSetSketch<float> &rh) {return lh + rh;})
        .def("__eq__", [](const sketch::CSetSketch<float> &h, const sketch::CSetSketch<float> &h2) {
            return h == h2;
        }).def("__neq__", [](const sketch::CSetSketch<float> &h, const sketch::CSetSketch<float> &h2) {
            return !(h == h2);
        }).def("write", [](const sketch::CSetSketch<float> &h, std::string path) {
            h.write(path);
        }).def("to_numpy", [](const sketch::CSetSketch<float> &h) {
            return setsketch2np(h);
        }).def("calc_card", [](const sketch::CSetSketch<float> &h) {h.getcard();});
    m.def("jaccard_index", [](CSetSketch<float> &h1, CSetSketch<float> &h2) {
            return h1.jaccard_index(h2);
        }, "Calculates jaccard indexes between two sketches")
    .def("csf_from_np", [](const py::array_t<uint32_t> &input, size_t ss=10) {
         CSetSketch<float> ret(ss);
         auto ptr = input.data();
         for(ssize_t i = 0; i < input.size();ret.add(ptr[i++]));
         ret.getcard();
         return ret;
    }, py::arg("array"), py::arg("sketchsize") = 10,
        py::return_value_policy::take_ownership, "Creates an f32 CSetSketch from a numpy array of 32-bit hashes.")
    .def("csf_from_np", [](const py::array_t<uint64_t> &input, size_t ss=10) {
         CSetSketch<float> ret(ss);
         auto ptr = input.data();
         for(ssize_t i = 0; i < input.size();ret.add(ptr[i++]));
         ret.getcard();
         return ret;
     }, py::arg("array"), py::arg("sketchsize") = 10,
        py::return_value_policy::take_ownership, "Creates a f32 CSetSketch from a numpy array of (unhashed) 64-bit integers");
} // pybind11 module
