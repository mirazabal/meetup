#ifndef BCN_CPP_QUEUE_H
#define BCN_CPP_QUEUE_H

#include <memory>
#include <iterator>
#include <iostream>

namespace bcn_cpp{


  template <typename T> 
    // T is Semiregular or Regular or TotallyOrdered
    class queue
    {

      public:
        queue() : elements_{0} {}                                    // could be implicitly declared sometimes
        ~queue() = default;        
        // Semiregular:
        queue(const queue& x)	
        {
          if(x.head_ == nullptr) return;

          head_ = std::make_unique<node>( x.head_.get()->val_);

          node* node_this = this->head_.get();	
          node* node_x = x.head_.get()->next.get();

          while(node_x != nullptr ){
            node_this->next = std::make_unique<node>(node_x->val_);
            node_this->next->previous_view = node_this;

            node_this = node_this->next.get();
            node_x = node_x->next.get();
          }	
        } //: value(x.value) {} // could be implicitly declared

        queue& operator=(queue const& x)
        {        // could be implicitly declared 
          queue temp(x);
          swap(*this,temp);
          return *this;
        }

        friend 
          void swap(queue& first, queue& second) // nothrow copy and swap idiom? why we should implement swap?
          {
            // enable ADL (not necessary in our case, but good practice)
            using std::swap;

            // by swapping the members of two objects,
            // the two objects are effectively swapped
            swap(first.elements_, second.elements_);
            swap(first.tail_view_, second.tail_view_);
            swap(first.head_, second.head_);

          }

        // Regular
        template <typename U>
          friend
          bool operator==(const queue<U>& x, const queue<U>& y) = delete; // why public and deleted and no privately implemented?
        template <typename U>
          friend
          bool operator!=(const queue<U>& x, const queue<U>& y) = delete; //{ return !(x==y);}; 

        // TotallyOrdered
        template <typename U>
          friend
          bool operator<(const queue<U>& x, const queue<U>& y) = delete; 

        template <typename U>
          friend
          bool operator>(const queue<U>& x, const queue<U>& y) { return y < x; }; 

        template <typename U>
          friend
          bool operator<=(const queue<U>& x, const queue<U>& y) {	return !(y < x); }

        template <typename U>
          friend
          bool operator>=(const queue<U>& x, const queue<U>& y) {	return !(x < y); } 


        void push(T const& val) & // why they do exist push and emplace? which is the difference between pass by const ref (T const&) and by RValue ref ?
        {
          if(head_ == nullptr){
            head_ = std::make_unique<node>( val );
            tail_view_ = head_.get();
            ++elements_;
            return;
          }

          tail_view_->next = std::make_unique<node>( val );
          tail_view_->next->previous_view = tail_view_; 
          tail_view_ = tail_view_->next.get();
          ++elements_;
        }

        void emplace(T&& val) &
        {
          if(head_ == nullptr){
            head_ = std::make_unique<node>( std::move(val) );
            tail_view_ = head_.get();
            ++elements_;
            return;
          }

          tail_view_->next = std::make_unique<node>( std::move(val) );
          tail_view_->next->previous_view = tail_view_; 
          tail_view_ = tail_view_->next.get();
          ++elements_;
        }

        T const& back() const & // why there are 2 functions top() + pop() instead of one?
        {
          return tail_view_->val_;
        }

        T const& front() const &
        {
          return head_.get()->val_;	
        }

        void pop() &
        {
          if(elements_ == 0) return;

          head_ = std::move(head_->next);
          head_->previous_view = nullptr;

          //tail_view_ = tail_view_->previous_view;
          //tail_view_->next = nullptr;
        }

        bool empty() const &
        {
          return static_cast<bool>( size() ); // why static cast? how many casts types do exist in c++17?	
        }

        size_t size() const &
        {
          return elements_;
        }


        std::iterator<T> begin( )
        {
          return iterator( *head.get() );
        }

        // const_iterator begin( ) const
        //				      { return const_iterator( _head->_next ); }

        // Return iterator representing endmarker of Linkedlist.
        //     // Mutator version is first, then accessor version.
        std::iterator<T> end( )
        {
          return iterator(tail_view_);
        }
        //	               { return iterator( _tail ); }

        //                   const_iterator end( ) const
        //                       { return const_iterator( _tail ); }
        //	           std::iterator<std::input_iterator_tag, T> begin()
        //	{
        //		return 	
        //	}

        //	std::iterator<std::input_iterator_tag, T> end() {

        //	}	


      private:

        std::unique_ptr<node> head_; // what is a unique_ptr?
        node* tail_view_; // why is this variable named with the _view attributte?
        size_t elements_; // what is size_t and when to use it?

        struct node
        {
          explicit node(T val) : val_{std::move(val)}, next{nullptr}, 
                   previous_view{nullptr} { }; // why explicit? why initialization-list vs 

          ~node() = default; // which is the meaning of default?

          //node& node(const node&) = default;
          //node& operator=(const node&) = default;
          //node& node(node&&) = delete;
          //node& operator=(node&&) = delete;

          T val_;
          std::unique_ptr<node> next;	
          node* previous_view; 
        };



        /* 

           typedef T value_type;
           T value;
        // Conversions from T and to T:
        explicit queue(const T& x) : value(x) {}
        explicit operator T() const { return value; }
        template <typename U>
        queue(const queue<U>& x) : value(x.value) {}
        // Write conversions from T to queue<T> and queue<T> to T.

        // Semiregular:
        queue(const queue& x) : value(x.value) {} // could be implicitly declared
        queue() {}                                    // could be implicitly declared sometimes
        ~queue() {}                                   // could be implicitly declared 
        queue& operator=(const queue& x) {        // could be implicitly declared 
        value = x.value;
        return *this;
        }
        // Regular
        friend
        bool operator==(const queue& x, const queue& y) {
        return x.value == y.value;
        }
        friend
        bool operator!=(const queue& x, const queue& y) {
        return !(x == y);
        }
        // TotallyOrdered
        friend
        bool operator<(const queue& x, const queue& y) { 
        return x.value < y.value;
        }
        friend
        bool operator>(const queue& x, const queue& y) {
        return y < x;
        }
        friend
        bool operator<=(const queue& x, const queue& y) {
        return !(y < x);
        }
        friend
        bool operator>=(const queue& x, const queue& y) {
        return !(x < y);
        } 

*/

    };
}

#endif

