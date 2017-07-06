#include <iostream>
#include <vector>
#include <stdexcept>

class GameOfLife {
public:
    GameOfLife() { }

    GameOfLife(size_t w, size_t h) :
        height_(h + 2), width_(w + 2) {
        if(h && w == 0) {
            std::cerr << "Bad field size!" << std::endl;
            throw new std::logic_error("Bad field size");
        }

        field_.resize(height_ * width_, false);
    }

    GameOfLife(size_t w, size_t h, const std::vector<bool>& field) :
        height_(h + 2), width_(w + 2) {
        if((h * w != field_.size()) || (h && w == 0)) {
            std::cerr << "Bad field size!" << std::endl;
            throw new std::logic_error("Bad field size");
        } else {

        }
    }

    void doStep() {
        if(field_.size() == 0) {
            return; // nothing to do if the field is absent
        }

        std::vector<bool> newField;
        newField.resize(field_.size(), false);

        auto upperRow = field_.begin();
        auto currentRow = field_.begin() + width_;
        auto lowerRow = field_.begin() + width_ * 2;

        auto cur = newField.begin() + width_ + 1;

        for(size_t y = 0; y < height_; y++){
            for(size_t x = 0; x < width_; x++){
                int neighbours =
                        *upperRow + *(upperRow + 1) +   *(upperRow + 2) +
                        *currentRow +                   *(currentRow + 2) +
                        *lowerRow + *(lowerRow + 1) +   *(lowerRow + 2);
                /*
                 * It might seem that here we do a lot of calls to RAM,
                 * but actually after the first call to each of the iterators our data
                 * will be moved to L1 cache, so it should work pretty fast
                 */

                if(neighbours == 3) {
                    *cur = true;
                } else {
                    if(neighbours == 2) {
                        *cur = *(currentRow + 1);
                    }
                }
                // We've initialized newField with zeros so no need to set them here

                cur++;
                upperRow++;
                currentRow++;
                lowerRow++;
                // .... and thanks to x64 we've got enough register to store all the variables without stack usage
            }
            *(cur - width_ + 1) = *(cur - 1);
            *cur = *(cur - width_ + 2);
            cur += 2;
            upperRow += 2;
            currentRow += 2;
            lowerRow += 2;
        }
    }

protected:
    size_t height_ = 0;
    size_t width_ = 0;

    std::vector<bool> field_; // we will store the field in memory row-by-row
};

int main()
{
    return 0;
}
