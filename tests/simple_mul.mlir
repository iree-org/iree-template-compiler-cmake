// This IREE compiler input represents a model as sourced from some frontend
// framework (JAX, pytorch, tflite, etc). Based on where the input comes from
// the compiler may require additional flags; refer to the documentation for
// frontend-specific instructions:
//   https://iree-org.github.io/iree/getting-started/

// Simple elementwise multiply:
//   %result = %lhs * %rhs
func.func @simple_mul(%lhs: tensor<4xf32>, %rhs: tensor<4xf32>) -> tensor<4xf32> {
  %result = arith.mulf %lhs, %rhs : tensor<4xf32>
  return %result : tensor<4xf32>
}
