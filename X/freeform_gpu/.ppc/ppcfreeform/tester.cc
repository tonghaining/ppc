#include "freeform.h"
#include "ppc.h"

int main(int argc, const char **argv) {
    const char *ppc_output = std::getenv("PPC_OUTPUT");
    int ppc_output_fd = 0;
    if (ppc_output) {
        ppc_output_fd = std::stoi(ppc_output);
    }
    if (ppc_output_fd <= 0) {
        ppc_output_fd = 1;
    }
    std::unique_ptr<ppc::fdostream> stream = std::unique_ptr<ppc::fdostream>(new ppc::fdostream(ppc_output_fd));

    argc--;
    argv++;
    if (argc != 1) {
        std::cerr << "Invalid usage" << std::endl;
        return 1;
    }

    std::ifstream input_file(argv[0]);
    if (!input_file) {
        std::cerr << "Failed to open input file" << std::endl;
        return 2;
    }

    int x;
    CHECK_READ(input_file >> x);
    CHECK_END(input_file);

    ppc::setup_cuda_device();
    ppc::perf timer;
    timer.start();
    int result = run(x);
    timer.stop();
    timer.print_to(*stream);
    ppc::reset_cuda_device();

    *stream << "result\tdone\n";
    *stream << "input\t" << x << '\n';
    *stream << "output\t" << result << '\n';
    *stream << std::endl;
}
