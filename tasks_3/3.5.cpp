class Vector {
  double* data_;
  size_t size_;
  bool is_original_;  //true -- не нужно копировать при изменении, false -- нужно

public:
  Vector(size_t size, double* data) {
    data_ = data;
    size_ = size;
    is_original_ = false;
  }

  Vector(const Vector& other) {
    data_ = other.data_;
    size_ = other.size_;
    is_original_ = false;
  }

  Vector(const Vector&& other, size_t offset, size_t len) {
    data_ = other.data_ + offset;
    size_ = len;
    is_original_ = false;
  }

  ~Vector() {
    size_ = 0;
    delete[] data_;
  }

  Vector& operator=(const Vector& other) {
    if (this != &other) {
      delete[] data_;
      data_ = other.data_;
      size_ = other.size_;
      is_original_ = false;
    }
  }

  double& operator[](size_t index) {
    if (index >= size_) {
      return cError;
    }
    if (!is_original_) {
      double* new_data = new double[size_];
      for (size_t i = 0; i < size_; i++) {
        new_data[i] = data_[i];
      }
      data_ = new_data;
    }
    return data_[index];
  }

  double* AquireBuffer() {
    return data_;
  }
};