mod matrix;

use std::time::Instant;

use clap::Parser;

use matrix::SquareMatrix;

#[derive(Parser)]
struct Cli {
    matrix_size: usize,
    chunk_size: usize,
}

fn main() {
    let args = Cli::parse();

    let m1 = SquareMatrix::new_random(args.matrix_size);
    let m2 = SquareMatrix::new_random(args.matrix_size);

    let start = Instant::now();
    let result_by_rows = m1.multiply_by_rows(&m2);
    let duration = start.elapsed().as_secs_f32();
    println!("Time elapsed multiplying by rows: {:?}", duration);

    let start = Instant::now();
    let result_by_columns = m1.multiply_by_columns(&m2);
    let duration = start.elapsed().as_secs_f32();
    println!("Time elapsed multiplying by columns: {:?}", duration);

    assert_eq!(result_by_rows, result_by_columns);

    let start = Instant::now();
    let result_by_chunks = m1.multiply_by_chunks(&m2, args.chunk_size);
    let duration = start.elapsed().as_secs_f32();
    println!("Time elapsed multiplying by chunks: {:?}", duration);

    assert_eq!(result_by_chunks, result_by_rows);
}
