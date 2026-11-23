#include "raytracing/test_helpers.h"
#include "raytracing/Canvas.h"

#include <filesystem>
#include <fstream>
#include <sstream>

using namespace testutil;
namespace fs = std::filesystem;

TEST(Canvas, Creating) {
    Canvas c(10, 20);
    EXPECT_EQ(c.width, 10u);
    EXPECT_EQ(c.height, 20u);
    ASSERT_EQ(c.pixels.size(), 20u);
    for (const auto& row : c.pixels) {
        ASSERT_EQ(row.size(), 10u);
        for (const Color& px : row)
            EXPECT_COLOR_EQ(px, (Color{ 0, 0, 0 }));
    }
}

TEST(Canvas, WritingPixels) {
    Canvas c(10, 20);
    Color red{ 1, 0, 0 };
    c(2, 3) = red;
    EXPECT_COLOR_EQ(c(2, 3), red);
}

TEST(Canvas, ConstAccessReadsWhatWasWritten) {
    Canvas c(10, 20);
    c(2, 3) = Color{ 0, 1, 0 };
    const Canvas& cc = c;
    EXPECT_COLOR_EQ(cc(2, 3), (Color{ 0, 1, 0 }));
}

class CanvasPpm : public ::testing::Test {
protected:
    fs::path originalDir_;
    fs::path tempDir_;

    void SetUp() override {
        originalDir_ = fs::current_path();
        tempDir_ = fs::temp_directory_path() /
            (std::string("raytracer_canvas_") +
                ::testing::UnitTest::GetInstance()->current_test_info()->name());
        fs::remove_all(tempDir_);
        fs::create_directories(tempDir_);
        fs::current_path(tempDir_);
    }

    void TearDown() override {
        fs::current_path(originalDir_);
        std::error_code ec;
        fs::remove_all(tempDir_, ec);
    }

    std::string ppmOf(const Canvas& c) {
        c.toPPM();
        std::ifstream in{ "image.ppm" }; 
        EXPECT_TRUE(in.is_open()) << "toPPM() did not create image.ppm";
        std::ostringstream ss;
        ss << in.rdbuf();
        return ss.str();
    }
};

TEST_F(CanvasPpm, WritesImagePpmInTheCurrentDirectory) {
    Canvas c(5, 3);
    c.toPPM();
    EXPECT_TRUE(fs::exists("image.ppm"));
}

TEST_F(CanvasPpm, ConstructingThePpmHeader) {
    Canvas c(5, 3);
    auto lines = splitLines(ppmOf(c));
    ASSERT_GE(lines.size(), 3u);
    EXPECT_EQ(lines[0], "P3");
    EXPECT_EQ(lines[1], "5 3");
    EXPECT_EQ(lines[2], "255");
}

TEST_F(CanvasPpm, ConstructingThePpmPixelData) {
    Canvas c(5, 3);
    c.pixels[0][0] = Color{ 1.5, 0, 0 };
    c.pixels[1][2] = Color{ 0, 0.5, 0 };
    c.pixels[2][4] = Color{ -0.5, 0, 1 };

    auto lines = splitLines(ppmOf(c));
    ASSERT_GE(lines.size(), 6u);
    EXPECT_EQ(lines[3], "255 0 0 0 0 0 0 0 0 0 0 0 0 0 0");
    EXPECT_EQ(lines[4], "0 0 0 0 0 0 0 128 0 0 0 0 0 0 0");
    EXPECT_EQ(lines[5], "0 0 0 0 0 0 0 0 0 0 0 0 0 0 255");
}

TEST_F(CanvasPpm, SplittingLongLinesInPpmFiles) {
    Canvas c(10, 2);
    for (auto& row : c.pixels)
        for (auto& px : row)
            px = Color{ 1, 0.8, 0.6 };

    auto lines = splitLines(ppmOf(c));
    ASSERT_GE(lines.size(), 7u);
    EXPECT_EQ(lines[3], "255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204");
    EXPECT_EQ(lines[4], "153 255 204 153 255 204 153 255 204 153 255 204 153");
    EXPECT_EQ(lines[5], "255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204");
    EXPECT_EQ(lines[6], "153 255 204 153 255 204 153 255 204 153 255 204 153");
    for (const auto& l : lines)
        EXPECT_LE(l.size(), 70u);
}

TEST_F(CanvasPpm, PpmFilesAreTerminatedByANewline) {
    Canvas c(5, 3);
    std::string ppm = ppmOf(c);
    ASSERT_FALSE(ppm.empty());
    EXPECT_EQ(ppm.back(), '\n');
}
