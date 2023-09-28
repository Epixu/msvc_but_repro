#define CATCH_CONFIG_RUNNER
#include <catch2/catch.hpp>
#include <cstdint>
#include <immintrin.h>

int main(int argc, char* argv[]) {
   Catch::Session session;
   return session.run(argc, argv);
}

/// FAILS                                                               
TEST_CASE("Testing with GIVEN", "[bug]") {
   const int64_t values[4] {11, 22, 33, 0};

   GIVEN("a1") {
      const __m256i a1 = _mm256_setr_epi64x(
         values[0],
         values[1],
         values[2],
         0 // if i substitute this with values[3] it works fine
      );

      WHEN("a1 is stored") {
         alignas(32) int64_t b1[4];
         _mm256_store_si256(reinterpret_cast<__m256i*>(b1), a1);

         THEN("The result should be correct") {
            REQUIRE(b1[0] == values[0]);
            REQUIRE(b1[1] == values[1]);
            REQUIRE(b1[2] == values[2]); // always fails here
            REQUIRE(b1[3] == values[3]);
         }
      }
   }
}

/// WORKS                                                               
TEST_CASE("Testing without GIVEN", "[bug]") {
   const int64_t values[4] {11, 22, 33, 0};
   //GIVEN("a1") {
   const __m256i a1 = _mm256_setr_epi64x(
      values[0],
      values[1],
      values[2],
      0 // if i substitute this with values[3] it works fine
   );

   WHEN("a1 is stored") {
      alignas(32) int64_t b1[4];
      _mm256_store_si256(reinterpret_cast<__m256i*>(b1), a1);

      THEN("The result should be correct") {
         REQUIRE(b1[0] == values[0]);
         REQUIRE(b1[1] == values[1]);
         REQUIRE(b1[2] == values[2]); // always fails here
         REQUIRE(b1[3] == values[3]);
      }
   }
   //}
}

/// FAILS                                                               
TEST_CASE("Testing without GIVEN, multiple loops", "[bug]") {
   const int64_t values[4] {11, 22, 33, 0};

   for (int i = 0; i < 100; ++i) {
      //GIVEN("a1") {
      const __m256i a1 = _mm256_setr_epi64x(
         values[0],
         values[1],
         values[2],
         0 // if i substitute this with values[3] it works fine
      );

      WHEN("a1 is stored") {
         alignas(32) int64_t b1[4];
         _mm256_store_si256(reinterpret_cast<__m256i*>(b1), a1);

         THEN("The result should be correct") {
            REQUIRE(b1[0] == values[0]);
            REQUIRE(b1[1] == values[1]);
            REQUIRE(b1[2] == values[2]); // always fails here
            REQUIRE(b1[3] == values[3]);
         }
      }
      //}
   }
}

#if _MSC_VER
   #define NOINLINE __declspec(noinline)
#else
   #define NOINLINE __attribute__((noinline))
#endif

NOINLINE bool externaltest() {
   const int64_t values[4] {11, 22, 33, 0};

   const __m256i a1 = _mm256_setr_epi64x(
      values[0],
      values[1],
      values[2],
      0 // if i substitute this with values[3] it works fine
   );

   alignas(32) int64_t b1[4];
   _mm256_store_si256(reinterpret_cast<__m256i*>(b1), a1);

   return b1[0] == values[0]
       && b1[1] == values[1]
       && b1[2] == values[2]
       && b1[3] == values[3];
}

/// WORKS                                                               
TEST_CASE("Testing without GIVEN, multiple loops, external function", "[bug]") {
   for (int i = 0; i < 100; ++i) {
      REQUIRE(externaltest());
   }
}
