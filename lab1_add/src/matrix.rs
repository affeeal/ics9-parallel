use std::cmp;
use std::sync::Mutex;
use std::thread;

use rand::Rng;

#[derive(Debug, PartialEq)]
pub struct SquareMatrix {
    pub data: Vec<i32>,
    pub size: usize,
}

impl SquareMatrix {
    const RAND_MIN: i32 = -128;
    const RAND_MAX: i32 = 128;

    pub fn new_random(size: usize) -> Self {
        let mut data = Vec::with_capacity(size * size);

        let mut rng = rand::thread_rng();
        for _ in 0..data.capacity() {
            data.push(rng.gen_range(Self::RAND_MIN..Self::RAND_MAX));
        }

        Self { data, size }
    }

    pub fn multiply_by_rows(&self, other: &Self) -> Self {
        debug_assert_eq!(self.size, other.size);

        let mut data = vec![0; self.size * self.size];

        for i in 0..self.size {
            for j in 0..self.size {
                for k in 0..self.size {
                    data[self.size * i + j] +=
                        self.data[self.size * i + k] * other.data[other.size * k + j];
                }
            }
        }

        Self { data, size: self.size }
    }

    pub fn multiply_by_columns(&self, other: &Self) -> Self {
        debug_assert_eq!(self.size, other.size);

        let mut data = vec![0; self.size * self.size];

        for i in 0..self.size {
            for j in 0..self.size {
                for k in 0..self.size {
                    data[self.size * j + i] +=
                        self.data[self.size * j + k] * other.data[other.size * k + i];
                }
            }
        }

        Self { data, size: self.size }
    }

    pub fn multiply_by_chunks(&self, other: &Self, chunk_size: usize) -> Self {
        debug_assert_eq!(self.size, other.size);

        let data = Mutex::new(vec![0; self.size * self.size]);

        thread::scope(|s| {
            let mut row = 0;
            while row < self.size {
                let mut column = 0;
                while column < self.size {
                    let data = &data;

                    s.spawn(move || {
                        let bottom_border = cmp::min(row + chunk_size, self.size);
                        let right_border = cmp::min(column + chunk_size, self.size);

                        for i in row..bottom_border {
                            for j in column..right_border {
                                let mut sum = 0;

                                for k in 0..self.size {
                                    sum += self.data[self.size * i + k]
                                        * other.data[other.size * k + j];
                                }

                                data.lock().unwrap()[self.size * i + j] = sum;
                            }
                        }
                    });
                    column += chunk_size;
                }
                row += chunk_size;
            }
        });

        Self {
            data: data.into_inner().unwrap(),
            size: self.size,
        }
    }
}
