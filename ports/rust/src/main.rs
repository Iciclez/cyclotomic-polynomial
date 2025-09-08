use num::complex::Complex;

fn gcd(mut a: u64, mut b: u64) -> u64 {
    while b != 0 {
        let remainder = a % b;
        a = b;
        b = remainder;
    }

    a
}

fn subtract(p: &Vec<i64>, q: &Vec<i64>) -> Vec<i64> {
    let mut vec = p.clone();
    vec.resize(std::cmp::max(p.len(), q.len()), 0);

    for i in 0..q.len() {
        let idx = q.len() - 1 - i;
        vec[idx] -= q[idx];
    }

    while vec.len() > 0 && *vec.last().unwrap() == 0 {
        vec.pop();
    }

    vec
}

fn multiply_v1(p: &Vec<i64>, q: &Vec<i64>) -> Vec<i64> {
    let mut vec = std::vec![];
    vec.resize(p.len() + q.len(), 0);

    for i in 0..p.len() {
        for j in 0..q.len() {
            vec[i + j] += p[i] * q[j];
        }
    }

    while vec.len() > 0 && *vec.last().unwrap() == 0 {
        vec.pop();
    }

    vec
}

fn divide_v1(mut p: Vec<i64>, q: &Vec<i64>) -> Vec<i64> {
    let mut vec = std::vec![];
    vec.resize(std::cmp::max(p.len(), q.len()) + 1, 0);

    while (p.len() == q.len() && p.last() == q.last()) || (p.len() > q.len() || p.last() > q.last())
    {
        let mut v = std::vec![];
        v.resize(p.len() - q.len() + 1, 0);

        let l_idx = v.len() - 1;
        vec[l_idx] = *p.last().unwrap() / *q.last().unwrap();
        v[l_idx] = vec[l_idx];

        let _fn = if p.len() == q.len() {
            multiply
        } else {
            multiply_v1
        };
        p = subtract(&p, &_fn(&q, &v));

        if p.len() == 0 {
            break;
        }
    }

    while vec.len() > 0 && *vec.last().unwrap() == 0 {
        vec.pop();
    }

    vec
}

fn as_complex(vec: &Vec<i64>) -> Vec<Complex<f64>> {
    vec.iter().map(|x| Complex::new(*x as f64, 0.0)).collect()
}

fn fft(v: &Vec<Complex<f64>>, n: i32, k: i32) -> Vec<Complex<f64>> {
    if n.abs() == k.abs() {
        return vec![v.iter().copied().sum()];
    }

    let mut u = [vec![], vec![]];
    u[0].reserve_exact(v.len() + 1);
    u[1].reserve_exact(v.len() + 1);

    for i in 0..v.len() {
        u[i % 2].push(v[i]);
    }

    let w0 = fft(&u[0], n, k << 1);
    let w1 = fft(&u[1], n, k << 1);

    let mut res = vec![];
    res.resize(usize::try_from((n / k).abs()).unwrap(), 0.0.into());

    let mut i = 0;
    let j = res.len() >> 1;

    while i < j {
        let m = ((2.0 * std::f64::consts::PI * (k as f64) * (i as f64) / (n as f64))
            * Complex::new(0.0, 1.0))
        .exp();

        res[i + j] = m * w1[i];
        res[i] = w0[i] + res[i + j];
        res[i + j] = w0[i] - res[i + j];

        i += 1;
    }

    res
}

fn multiply_v2(p: &Vec<i64>, q: &Vec<i64>) -> Vec<i64> {
    let n = 1u64 << ((((p.len() + q.len() - 2) as f64).log2() + 1.0) as u64);

    let _p = as_complex(p);
    let _q = as_complex(q);
    let a = fft(&_p, i32::try_from(n).unwrap(), 1);
    let b = fft(&_q, i32::try_from(n).unwrap(), 1);
    let mut c = vec![];
    let mut d = vec![];

    for i in 0u64..n {
        let idx = usize::try_from(i).unwrap();
        c.push(a[idx] * b[idx]);
    }

    for m in fft(&c, i32::try_from(n).unwrap(), -1) {
        d.push((m / Complex::new(n as f64, 0.0)).re.round() as i64)
    }

    while d.len() > 0 && *d.last().unwrap() == 0 {
        d.pop();
    }

    d
}

fn multiply(p: &Vec<i64>, q: &Vec<i64>) -> Vec<i64> {
    multiply_v2(&p, &q)
}

fn divide(p: &Vec<i64>, q: &Vec<i64>) -> Vec<i64> {
    divide_v1(p.clone(), q)
}

fn is_prime(n: u64) -> bool {
    if n <= 1 {
        return false;
    }

    if n <= 3 {
        return true;
    }

    if n % 2 == 0 {
        return false;
    }

    // let m = n.isqrt() + 1;
    let m = (n as f64).sqrt() as u64 + 1;
    for i in (3..=m).step_by(2) {
        if n % i == 0 {
            return false;
        }
    }

    true
}

// Returns true if n is a power of m.
fn is_power_of(mut n: u64, m: u64) -> bool {
    while n % m == 0 && n > 1 {
        n /= m;
    }

    n == 1
}

fn cyclotomic_polynomial(n: u64) -> Vec<i64> {
    if n == 1 {
        return std::vec![1, -1];
    }

    let mut vec = std::vec![];

    if is_prime(n) {
        vec.resize(usize::try_from(n).unwrap(), 1);
    } else if (n % 2 == 0) && ((n / 2) % 2 != 0) && is_prime(n / 2) {
        let m = n / 2;
        vec.reserve_exact(usize::try_from(m).unwrap());
        for i in 0..m {
            vec.push(if i % 2 == 0 { 1 } else { -1 });
        }
    } else if n > 1 && is_power_of(n, 2) {
        vec.resize(usize::try_from(n / 2 + 1).unwrap(), 0);

        vec[0] = 1;
        *vec.last_mut().unwrap() = 1;
    } else if ((n % 12 == 0) && is_power_of(n / 12, 2)) || ((n % 18 == 0) && is_power_of(n / 18, 2))
    {
        vec.resize(usize::try_from(n / 3 + 1).unwrap(), 0);

        let m_idx = vec.len() / 2;

        vec[0] = 1;
        vec[m_idx] = -1;
        *vec.last_mut().unwrap() = 1;
    } else if gcd(n, 9) == 9 && is_power_of(n, 3) {
        vec.resize((n as f64 / 1.5).ceil() as usize + 1, 0);

        let m_idx = vec.len() / 2;

        vec[0] = 1;
        vec[m_idx] = 1;
        *vec.last_mut().unwrap() = 1;
    } else {
        let mut p = std::vec![1];

        // factors of N
        let m = n / 2;
        for i in 1..=m {
            if n % i == 0 {
                let mut q = cyclotomic_polynomial(i);
                q.reverse();
                p = multiply(&p, &q);
            }
        }

        let mut q = std::vec![];
        q.resize(usize::try_from(n + 1).unwrap(), 0);
        q[0] = -1;
        *q.last_mut().unwrap() = 1;

        vec = divide(&q, &p);
        vec.reverse();
    }

    vec
}

fn main() {
    std::env::set_var("RUST_BACKTRACE", "1");

    let start = std::time::Instant::now();
    for i in 1..=500 {
        println!("{}: {:?}", i, cyclotomic_polynomial(i));
    }

    let duration = start.elapsed();

    println!("-> {:?}", duration);
}

#[cfg(test)]
mod test {
    use super::*;

    #[test]
    fn test_cyclotomic_polynomial() {
        std::env::set_var("RUST_BACKTRACE", "1");

        let solutions = std::collections::HashMap::from([
            (1, vec![1, -1]),
            (2, vec![1, 1]),
            (3, vec![1, 1, 1]),
            (4, vec![1, 0, 1]),
            (5, vec![1, 1, 1, 1, 1]),
            (6, vec![1, -1, 1]),
            (7, vec![1, 1, 1, 1, 1, 1, 1]),
            (8, vec![1, 0, 0, 0, 1]),
            (9, vec![1, 0, 0, 1, 0, 0, 1]),
            (10, vec![1, -1, 1, -1, 1]),
            (11, vec![1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1]),
            (12, vec![1, 0, -1, 0, 1]),
            (13, vec![1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1]),
            (14, vec![1, -1, 1, -1, 1, -1, 1]),
            (15, vec![1, -1, 0, 1, -1, 1, 0, -1, 1]),
            (16, vec![1, 0, 0, 0, 0, 0, 0, 0, 1]),
            (17, vec![1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1]),
            (18, vec![1, 0, 0, -1, 0, 0, 1]),
            (
                19,
                vec![1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
            ),
            (20, vec![1, 0, -1, 0, 1, 0, -1, 0, 1]),
            (21, vec![1, -1, 0, 1, -1, 0, 1, 0, -1, 1, 0, -1, 1]),
            (22, vec![1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1]),
            (
                23,
                vec![
                    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                ],
            ),
            (24, vec![1, 0, 0, 0, -1, 0, 0, 0, 1]),
            (
                25,
                vec![
                    1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1,
                ],
            ),
            (26, vec![1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1]),
            (
                27,
                vec![1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1],
            ),
            (28, vec![1, 0, -1, 0, 1, 0, -1, 0, 1, 0, -1, 0, 1]),
            (
                29,
                vec![
                    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                    1, 1, 1,
                ],
            ),
            (30, vec![1, 1, 0, -1, -1, -1, 0, 1, 1]),
            (
                105,
                vec![
                    1, 1, 1, 0, 0, -1, -1, -2, -1, -1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, -1, 0, -1, 0,
                    -1, 0, -1, 0, -1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, -1, -1, -2, -1, -1, 0, 0, 1, 1,
                    1,
                ],
            ),
        ]);

        for (k, v) in solutions {
            assert_eq!(v, cyclotomic_polynomial(k));
        }
    }
}
