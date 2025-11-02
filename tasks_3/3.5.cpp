#include <iostream>
#include <cassert>

class Vector {
  /*
   * Пишу отдельную структуру для хранения данных,
   * чтобы удобнее было получать доступ к ref_count
   * из любого конструктора, деструктора и прочих мест
  */
  struct Data {
    double* data;
    size_t capacity;
    unsigned ref_count;

    void Print() const {
      std::cout << "[";
      for (size_t i = 0; i < capacity; i++) {
        std::cout << data[i] << ", ";
      }
      std::cout << "]\n";
      std::cout << "ref_count: " << ref_count << "\n\n";
    }
  };

  Data* data_;
  size_t size_;     //Собственный размер. Может не совпадать с data_.capacity
  size_t offset_;
  bool is_blocked;  //true -- буфер занят через AcquireBuffer, false -- свободен

  void CleanData() {
    std::cout << "Data cleaned";
    --data_->ref_count;
    if (data_->ref_count == 0) {
      delete data_->data;
      delete data_;
      std::cout << ", deleted";
    }
    std::cout << "\n";
  }

  void CopyData() {
    if (data_->ref_count > 1) {
      Data* new_data = new Data{new double[size_], size_, 1};
      --data_->ref_count;

      for (unsigned i = 0; i < size_; ++i) {
        new_data->data[i] = data_->data[i + offset_];
      }

      data_ = new_data;
      offset_ = 0;
      std::cout << "Data copied\n";
    }
  }

 public:
  Vector(size_t size) {
    data_ = new Data{new double[size], size, 1};
    size_ = size;
    offset_ = 0;
    is_blocked = false;
    std::cout << "Empty vector created\n";
  }

  /*
   * Когда вызывается конструктор от динамического массива,
   * предполагается, что этот массив ни кем не используется.
   * Не уверена, может быть вообще не стоит создавать такой конструктор
  */
  Vector(size_t size, double* data) {
    data_ = new Data{data, size, 1};
    size_ = size;
    offset_ = 0;
    is_blocked = false;
    std::cout << "Vector created\n";
  }

  Vector(const Vector& other) {
    data_ = other.data_;
    ++data_->ref_count;
    size_ = other.size_;
    offset_ = other.offset_;
    is_blocked = false;
    std::cout << "Vector copy created\n";
  }

  Vector(const Vector& other, size_t offset, size_t len) {
    data_ = other.data_;
    ++data_->ref_count;
    size_ = len;
    offset_ = other.offset_ + offset;
    is_blocked = false;
    std::cout << "Subvector created\n";
  }

  ~Vector() {
    CleanData();
    std::cout << "Vector destroyed\n";
  }

  Vector& operator=(const Vector& other) {
    std::cout << "Copy assignment ";
    if (this->data_ != other.data_) {
      std::cout << "with a different data buffer\n";
      CleanData();
      data_ = other.data_;
      ++data_->ref_count;
    } else {
      std::cout << "with a same data buffer\n";
    }
    size_ = other.size_;
    offset_ = other.offset_;
    is_blocked = false;
    return *this;
  }

  double& operator[](size_t index) {
    assert(index < size_ && "Index out of range");
    assert(!is_blocked && "Cannot change blocked data!");
    CopyData();
    return data_->data[index];
  }

  const double& operator[] (size_t index) const {
    assert(index < size_ && "Index out of range");
    return data_->data[index];
  }


  class Buffer {
    Vector* vec_;

   public:
    Buffer(Vector* vec) {
      std::cout << "Buffer created\n";
      vec->CopyData();
      vec_ = vec;
    }

    ~Buffer() {
      vec_->is_blocked = false;
    }

    double* AcquireBuffer() {
      assert(!vec_->is_blocked && "Cannot acquire buffer of a blocked vector!");
      std::cout << "Buffer acquired\n";
      vec_->is_blocked = true;
      return vec_->data_->data;
    }

    void ReleaseBuffer() {
      std::cout << "Buffer released\n";
      vec_->is_blocked = false;
    }
  };

  class ConstBuffer {
    Vector* vec_;

   public:
    ConstBuffer(Vector* vec) {
      std::cout << "Buffer created\n";
      vec_ = vec;
    }

    ~ConstBuffer() {
      vec_->is_blocked = false;
    }

    const double* AcquireBuffer() const {
      assert(!vec_->is_blocked && "Cannot acquire const buffer of a blocked vector!");
      vec_->is_blocked = true;
      return vec_->data_->data;
    }

    void ReleaseBuffer() const {
      vec_->is_blocked = false;
    }
  };

  void Print() {
    std::cout << "----------- Vector -----------\nSize: "
              << size_
              << ", Offset: "
              << offset_
              << "\nIs blocked: "
              << is_blocked
              << '\n';
    data_->Print();
  }
};


int main() {
  double* d = new double[10] {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  Vector vec = Vector(10, d);
  vec.Print();

  Vector vec2 = vec;
  vec2.Print();

  Vector vec3 = Vector(vec2);
  vec3.Print();

  Vector vec4 = Vector(vec3, 3, 4);
  vec4.Print();

  vec2 = vec3;
  vec2.Print();

  vec2[2] = 11;
  vec2.Print();
  vec.Print();

  std::cout << "------------------------------------ AcquireBuffer \n";
  Vector::Buffer buffer = Vector::Buffer(&vec);
  vec.Print();

  double* d1 = buffer.AcquireBuffer();
  d1[0] = -10;
  vec.Print();
  // vec[3] = 1; -- assert
  buffer.ReleaseBuffer();
  vec.Print();

  std::cout << "------------------------------------ AcquireConstBuffer \n";
  Vector::ConstBuffer cbuffer = Vector::ConstBuffer(&vec);
  vec.Print();

  const double* d2 = cbuffer.AcquireBuffer();
  // d2[0] = -10; -- error
  vec.Print();
  // vec[3] = 1; -- assert
  buffer.ReleaseBuffer();
}