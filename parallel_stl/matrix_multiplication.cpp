//#include <experimental/a>
//#include <experimental/coordinate>
//#include <random>
//
//using namespace std::experimental::D4087;
//
//template<typename F>
//void measure_time(F&& f, const char* name)
//{
//	using namespace std::chrono;
//
//	auto begin = high_resolution_clock::now();
//	f();
//	auto end = high_resolution_clock::now();
//
//	printf("%s %llu.%03llu seconds\n", name,
//		duration_cast<seconds>(end - begin).count(),
//		duration_cast<milliseconds>(end - begin).count() % 1000);
//}
//
//void multiply_element(const index<2>& idx, const std::vector<double>& vA, const std::vector<double>& vB, std::vector<double>& vResult, int N)
//{
//	auto row = idx[0];
//	auto col = idx[1];
//
//	double sum = 0;
//
//	for (auto i = 0; i < N; i++)
//		sum += vA[row * N + i] * vB[i * N + col];
//
//	vResult[row * N + col] = sum;
//}
//
//void test_matrix_multiplication(int N)
//{
//	// Create a random number generator.
//	std::random_device rd;
//	std::mt19937 generator(rd());
//
//	printf("\nTesting matrix multiplication of %d X %d doubles:\n", N, N);
//
//	std::vector<double> vA(N * N);
//	std::vector<double> vB(N * N);
//
//	std::uniform_real_distribution<double> dist(-100, 100);
//
//	// Generate random numbers
//	std::generate(std::begin(vA), std::end(vA), [&generator, &dist]() { return dist(generator); }); //double distribution
//	std::generate(std::begin(vB), std::end(vB), [&generator, &dist]() { return dist(generator); });
//
//	measure_time([&]() mutable
//		{
//			bounds<2> bnd{ N, N };
//			std::vector<double> vResult(bnd.size());
//
//			// Using serial implementation
//			std::for_each(std::begin(bnd), std::end(bnd), [&](index<2> idx) {
//				multiply_element(idx, vA, vB, vResult, N);
//				});
//		}, "serial:       ");
//
//	measure_time([&]() mutable
//		{
//			bounds<2> bnd{ N, N };
//			std::vector<double> vResult(bnd.size());
//
//			// Using Parallel STL implementation
//			std::experimental::parallel::for_each(std::experimental::parallel::par, std::begin(bnd), std::end(bnd), [&](index<2> idx) {
//				multiply_element(idx, vA, vB, vResult, N);
//				});
//		}, "parallel STL: ");
//}
//
//int _tmain(int /* argc */, _TCHAR* /* argv */[])
//{
//	test_matrix_multiplication(100);
//	test_matrix_multiplication(200);
//	test_matrix_multiplication(500);
//	test_matrix_multiplication(750);
//	test_matrix_multiplication(1000);
//	return 0;
//}
//
