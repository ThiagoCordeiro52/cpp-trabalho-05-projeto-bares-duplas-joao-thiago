#ifndef _STACK_H_
#define _STACK_H_

#include <string>   // std::string
#include <iostream> // std::ostream

#define MAX 1000

/// Represents a stack.
/**
 * @brief This structure represents a stack, which is just a set of stacked data,
 * where we only work with its top.
 * We will use the stack data structure to store the received expression and be
 * able to calculate it.
 * The implementation was inspired by the website:
 * https://www.geeksforgeeks.org/stack-data-structure-introduction-program/.
 * 
 * @tparam T the type of stack.
 */
template <typename T>
struct Stack
{
	T A[MAX]; // Maximum size of Stack

	Stack() { top_idx = -1; }

	/**
	 * @brief Adds an item in the stack. If the stack is full, then it is said 
	 * to be an Overflow condition.
	 * @param element the element that will be add on the stack.
	 * @return true if pushed successfully.
	 * @return false otherwise.
	 */
	bool push(T element)
	{
		if (top_idx >= (MAX - 1)) {
			std::cout << "Stack Overflow";
			return false;
		}
		else {
			A[++top_idx] = element;
			std::cout << element << " pushed into stack\n";
			return true;
		}
	};
	
	/**
	 * @brief Removes an item from the stack. The items are popped in the
	 * reversed order in which they are pushed. If the stack is empty,
	 * then it is said to be an Underflow condition.
	 * @return T the element that was removed from the stack.
	 */
	T pop()
	{
		if (top_idx < 0) {
			std::cout << "Stack Underflow";
			return 0;
		}
		else {
			T element = A[top_idx--];
			return element;
		}
	};

	/**
	 * @brief Get the top element of the stack.
	 * @return T Returns top element of stack.
	 */
	T top()
	{
		if (top_idx < 0) {
			std::cout << "Stack is Empty";
			return 0;
		}
		else {
			T element = A[top_idx];
			return element;
		}
	};

	/**
	 * @brief Check if the stack is empty.
	 * @return true Returns true if stack is empty.
	 * @return false otherwise.
	 */
	bool empty()
	{
		return (top_idx < 0);
	};

	private:
		size_t top_idx; // The index of the element on the top of the stack. 
};

#endif
