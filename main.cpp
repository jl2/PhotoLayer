#include <iostream>
#include <string>
#include <vector>

#include <boost/program_options.hpp>

#include "Magick++.h"

namespace po = boost::program_options;

bool parseArgs(int argc, char *argv[],
               std::string &outputFile, std::vector<std::string> &inputFiles) {

    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "print help message")
        ("output,o", po::value<std::string>(), "output file name")
        ("input,i", po::value< std::vector<std::string> >(), "input files");
    
    po::positional_options_description p;
    p.add("input", -1);
    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << "\n";
        return false;
    }
    bool hasInput = false;
    bool hasOutput = false;
    if (vm.count("output")) {
        outputFile = vm["output"].as<std::string>();
        hasOutput = true;
    }
    if (vm.count("input")) {
        inputFiles = vm["input"].as<std::vector<std::string>>();
        hasInput = true;
    }
    if (!hasInput || !hasOutput) {
        std::cout << desc << "\n";
        return false;
    }
    return true;
}


int main(int argc, char *argv[]) {
    Magick::InitializeMagick(*argv);

    std::string outputFileName;
    std::vector<std::string> inputFileNames;
    
    if (!parseArgs(argc, argv, outputFileName, inputFileNames)) {
        return 1;
    }

    Magick::Image base(inputFileNames[0]);
    for (size_t i = 1;i<inputFileNames.size(); ++i) {
        Magick::Image nextImage(inputFileNames[i]);
        Magick::Image tmp = base;

        // This (mostly) works for the case of a dark object moving in front of a bright background
        tmp.composite(nextImage,0,0, Magick::CompositeOperator::MinusCompositeOp);
        base.composite(tmp,0,0, Magick::CompositeOperator::MinusCompositeOp);
    }

    base.write(outputFileName);

    return 0;
}
