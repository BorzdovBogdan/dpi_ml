//
// Created by Bogdan on 21.04.18.
//

#include <string>
#include <iostream>
/*#include "tensorflow/core/public/session.h"
#include "tensorflow/core/platform/env.h"*/
#include "tensorflow/cc/client/client_session.h"
#include "tensorflow/cc/ops/standard_ops.h"
#include "tensorflow/core/framework/tensor.h"


using namespace tensorflow;
using namespace tensorflow::ops;

int main(int argc, char* argv[]) {

    // Initialize a tensorflow session
    std::cout<<"Hello"<<std::endl;

    Scope root = Scope::NewRootScope();
    // Matrix A = [3 2; -1 0]
    auto A = Const(root, { {3.f, 2.f}, {-1.f, 0.f} });
    // Vector b = [3 5]
    auto b = Const(root, { {3.f, 5.f} });
    // v = Ab^T
    auto v = MatMul(root.WithOpName("v"), A, b, MatMul::TransposeB(true));
    std::vector<Tensor> outputs;
    ClientSession session(root);
    // Run and fetch v
    TF_CHECK_OK(session.Run({v}, &outputs));
    // Expect outputs[0] == [19; -3]
    LOG(INFO) << outputs[0].matrix<float>();


    /*Session* session;
    Status status = NewSession(SessionOptions(), &session);
    if (!status.ok()) {
        std::cout << status.ToString() << "\n";
        return 1;
    }*/

    // Read in the protobuf graph we exported
    // (The path seems to be relative to the cwd. Keep this in mind
    // when using `bazel run` since the cwd isn't where you call
    // `bazel run` but from inside a temp folder.)
    /*GraphDef graph_def;
    status = ReadBinaryProto(Env::Default(), "mlp.pb", &graph_def);
    if (!status.ok()) {
        std::cout << status.ToString() << "\n";
        return 1;
    }

    // Add the graph to the session
    status = session->Create(graph_def);
    if (!status.ok()) {
        std::cout << status.ToString() << "\n";
        return 1;
    }

    // Setup inputs and outputs:

    // Our graph doesn't require any inputs, since it specifies default values,
    // but we'll change an input to demonstrate.
    Tensor a(DT_FLOAT, TensorShape());
    a.scalar<float>()() = 3.0;

    Tensor b(DT_FLOAT, TensorShape());
    b.scalar<float>()() = 2.0;

    std::vector<std::pair<string, tensorflow::Tensor>> inputs = {
            { "a", a },
            { "b", b },
    };

    // The session will initialize the outputs
    std::vector<tensorflow::Tensor> outputs;

    // Run the session, evaluating our "c" operation from the graph
    status = session->Run(inputs, {"c"}, {}, &outputs);
    if (!status.ok()) {
        std::cout << status.ToString() << "\n";
        return 1;
    }

    // Grab the first output (we only evaluated one graph node: "c")
    // and convert the node to a scalar representation.
    auto output_c = outputs[0].scalar<float>();

    // (There are similar methods for vectors and matrices here:
    // https://github.com/tensorflow/tensorflow/blob/master/tensorflow/core/public/tensor.h)

    // Print the results
    std::cout << outputs[0].DebugString() << "\n"; // Tensor<type: float shape: [] values: 30>
    std::cout << output_c() << "\n"; // 30

    // Free any resources used by the session
    session->Close();*/
    return 0;
}