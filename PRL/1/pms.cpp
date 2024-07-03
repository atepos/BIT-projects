/*
 * Projekt: Projekt 1 do predmetu PRL - Parallel Merge sort
 * Autor: Petr Kaška, xkaska01@stud.fit.vutbr.cz
 * Datum: 21.2.2024
 */

/*
 * The creation of this program is based on the Parallel merge sort algorithm from the book:
 * Akl, S.G., 2014. Parallel sorting algorithms (Vol. 12). Academic press.
 * url: https://books.google.cz/books?id=jhHjBQAAQBAJ&lpg=PA48&ots=uUQZmC5MdA&dq=pipeline+merge+sort&hl=cs&pg=PA48#v=onepage&q&f=false
 */

#include <mpi.h>
#include <stdio.h>
#include <vector>
#include <cmath>
#include <queue>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <time.h>

using namespace std;

// if true, debugging informations are printed on standart output
// #define TESTING true

// if true, desired output format is printed
#define FINAL true

#define TAG_QUEUE_1 0
#define TAG_QUEUE_2 1

/**
 * @brief Checks the entry conditions for the parallel sorting algorithm.
 *
 * @param size The total number of processes.
 * @param input_size The size of the input array.
 */
void entry_condition(int size, int input_size)
{
    int required_size = static_cast<int>(pow(2, size - 1));
    if (required_size != input_size || input_size == 1)
    {
        cerr << "Error: The number of processes must be equal to 2^(size - 1)." << endl;
        cerr << "       Expected processes: " << required_size << endl;
        cerr << "       Actual processes: " << size << endl;
        MPI_Abort(MPI_COMM_WORLD, MPI_ERR_OTHER);
    }
}

/**
 * @brief Prints the elements of a input vector with spaces between them.
 *
 * @param vec The vector to be printed.
 */
void print_input_vector(const vector<unsigned char> &vec)
{
    auto it = vec.begin();
    cout << static_cast<int>(*it++);

    while (it != vec.end())
    {
        cout << ' ' << static_cast<int>(*it++);
    }
    cout << endl;
}

/**
 * @brief Prints the elements of an output vector each on a separate line.
 *
 * @param vec The vector to be printed.
 */
void print_output_vector(const std::vector<unsigned char> &vec)
{
    for (auto element : vec)
    {
        std::cout << static_cast<int>(element) << std::endl;
    }
}

/**
 * @brief Reads numbers from a file and performs entry condition check.
 *
 * @param size The total number of processes.
 * @return Vector of unsigned char containing the read numbers.
 */
vector<unsigned char> read_numbers_from_input_file(int size)
{
    vector<unsigned char> numbers;
    ifstream file("numbers", ios::binary);

    if (!file.is_open())
    {
        cerr << "Could not open file: numbers" << endl;
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    unsigned char num;
    while (file.read(reinterpret_cast<char *>(&num), sizeof(unsigned char)))
    {
        numbers.push_back(num);
    }
    file.close();

    entry_condition(size, numbers.size());
#ifdef FINAL
    print_input_vector(numbers);
#endif
    return numbers;
}

/**
 * @brief Get the front element of a queue, remove it, and return it.
 *
 * @param q The queue from which to retrieve the front element.
 *
 * @return The front element of the queue.
 */
unsigned char get_front_pop(queue<unsigned char> &q)
{
    unsigned char value = q.front();
    q.pop();
    return value;
}

/**
 * @brief Compare the front elements of two queues and return the smaller one.
 *
 * @param q1 The first queue.
 * @param q2 The second queue.
 *
 * @return The smaller of the front elements of the two queues, or 0 if both queues are empty.
 */
unsigned char compare_third_step(queue<unsigned char> &q1, queue<unsigned char> &q2)
{
    if (!q1.empty() && q2.empty())
    {
        return get_front_pop(q1);
    }
    else if (q1.empty() && !q2.empty())
    {
        return get_front_pop(q2);
    }
    else if (q1.front() < q2.front())
    {
        return get_front_pop(q1);
    }
    else
    {
        return get_front_pop(q2);
    }
}

/**
 * @brief Get the front element of a queue, remove it, and decrement a counter.
 *
 * @param q The queue from which to retrieve the front element.
 * @param counter A reference to an integer counter that should be decremented after the element is retrieved.
 *
 * @return The front element of the queue.
 */
unsigned char get_front_pop(queue<unsigned char> &q, int &counter)
{
    unsigned char value = q.front();
    q.pop();
    counter--;
    return value;
}

/**
 * @brief Compare the front elements of two queues and return the smaller one.
 *
 * @param q1 The first queue.
 * @param q2 The second queue.
 * @param a Reference to an integer indicating if the first queue is empty (1 if empty, 0 otherwise).
 * @param b Reference to an integer indicating if the second queue is empty (1 if empty, 0 otherwise).
 *
 * @return The smaller of the front elements of the two queues, or 0 if both queues are empty.
 */
unsigned char compare_second_step(queue<unsigned char> &q1, queue<unsigned char> &q2, int &a, int &b)
{
    if (a == 0)
    {
        return get_front_pop(q2, b);
    }
    else if (b == 0)
    {
        return get_front_pop(q1, a);
    }
    if (!q1.empty() && q2.empty())
    {
        return get_front_pop(q2, b);
    }
    else if (q1.empty() && !q2.empty())
    {
        return get_front_pop(q1, a);
    }
    else if (q1.front() < q2.front())
    {
        return get_front_pop(q1, a);
    }
    else
    {
        return get_front_pop(q2, b);
    }
}

/**
 * @brief Calculates the length of subsequence.
 *
 * @param size The total number of processes.
 * @return The length of subsequence for the current process.
 */
int get_subsequence_length(int size)
{
    return pow(2, size - 1);
}

/**
 * @brief Sends elements of a vector to the next process.
 *
 * @param rank The rank of the current process.
 * @param size The total number of processes.
 * @param input The vector containing the elements to be sent.
 */
void first_step(int rank, int size, std::vector<unsigned char> &input)
{
    for (int j = 0; j < input.size(); j++)
    {
        MPI_Send(&input[j],         // Pointer to the data to send
                 1,                 // Number of data items to send
                 MPI_UNSIGNED_CHAR, // MPI data type of the items
                 rank + 1,          // Rank of the destination process (next process in the algorithm)
                 j % 2,             // Tag for message identification (alternating between 0 and 1)
                 MPI_COMM_WORLD);   // Communicator over which to send the message
    }
}

/**
 * @brief Receive an unsigned char value from a specified MPI process.
 *
 * @param rank The rank of the MPI process from which to receive the value.
 *
 * @return A pair containing the received value and its MPI status.
 */
pair<unsigned char, MPI_Status> receive_value(int rank)
{
    unsigned char value;
    MPI_Status recv_status;
    MPI_Recv(&value, 1, MPI_UNSIGNED_CHAR, rank - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &recv_status);
    return make_pair(value, recv_status);
}

/**
 * @brief Receive an unsigned char value from a specified MPI process and push it into one of the two queues.
 *
 * @param rank The rank of the MPI process from which to receive the value.
 * @param q1 Reference to the first queue.
 * @param q2 Reference to the second queue.
 * @param recv_numbers Reference to the number of received values.
 * @param size The size of the MPI communicator.
 */
void receive_value(int rank, queue<unsigned char> &q1, queue<unsigned char> &q2, int &recv_numbers, int size)
{
    if (recv_numbers < size)
    {

#ifdef TESTING
        cout << "dostal jsem se do teto funkce " << recv_numbers << endl;
#endif
        pair<unsigned char, MPI_Status> received_data = receive_value(rank);
        if (received_data.second.MPI_TAG == TAG_QUEUE_1)
        {
            q1.push(received_data.first);
        }
        else
        {
            q2.push(received_data.first);
        }

        recv_numbers++;
    }
}

/**
 * @brief Check the sorting condition based on the sizes of two queues and a boolean condition.
 *
 * @param q1 Reference to the first queue.
 * @param q2 Reference to the second queue.
 * @param rank The rank used in the sorting condition calculation.
 * @param sort_condition The boolean condition used in the sorting condition calculation.
 *
 * @return True if the sorting condition is satisfied, false otherwise.
 */
bool sort_condition_check(queue<unsigned char> &q1, queue<unsigned char> &q2, int rank, bool sort_condition)
{
    return (!sort_condition && q1.size() == pow(2, rank - 1) && q2.size() == 1);
}

/**
 * @brief Perform sorting operation based on the values in two queues and update control variables.
 *
 * @param q1 first queue.
 * @param q2 second queue.
 * @param a  variable representing the number of elements in queue 1.
 * @param b  variable representing the number of elements in queue 2.
 * @param rank The rank of the MPI process.
 * @param m  variable representing the current phase of the sorting operation.
 * @param j  variable representing the current tag for MPI communication.
 * @param k  variable representing the number of elements processed so far.
 */
void perform_sorting(queue<unsigned char> &q1, queue<unsigned char> &q2, int &a, int &b, int rank, int &m, unsigned char &j, int &k)
{
    unsigned char buff = compare_second_step(q1, q2, a, b);
    MPI_Send(&buff, 1, MPI_UNSIGNED_CHAR, rank + 1, j, MPI_COMM_WORLD);
    m++;

    if (m > pow(2, rank))
    {
        m = 1;
        j = (j + 1) % 2;
        k += pow(2, rank);
        a = pow(2, rank) / 2;
        b = pow(2, rank) / 2;
    }
}

/**
 * @brief Perform the second step of the sorting algorithm.
 *
 * @param rank The rank of the MPI process.
 * @param size The size of the MPI communicator.
 */
void second_step(int rank, int size)
{
    queue<unsigned char> q1;
    queue<unsigned char> q2;

    int recv_numbers = 0;
    bool sort_condition = false;
    int m = 1;

    int a = pow(2, rank) / 2;
    int b = pow(2, rank) / 2;

    unsigned char j = 0;
    int k = 1;

    while (k <= size)
    {

        receive_value(rank, q1, q2, recv_numbers, size);

        if (sort_condition_check(q1, q2, rank, sort_condition))
        {
            sort_condition = true;
        }

        if (sort_condition)
        {
            perform_sorting(q1, q2, a, b, rank, m, j, k);
        }
    }
}

/**
 * @brief Compare the front elements of two queues and push the smaller one into a vector.
 *
 * @param q1  first queue.
 * @param q2  second queue.
 * @param output  vector where the result will be stored.
 * @param m  variable representing the number of comparisons made.
 */
void compare_and_push(queue<unsigned char> &q1, queue<unsigned char> &q2, vector<unsigned char> &output, int &m)
{
    unsigned char x = compare_third_step(q1, q2);
    output.push_back(x);
    m++;
}

/**
 * @brief Perform the third step of the sorting algorithm.
 *
 * @param rank The rank of the MPI process.
 * @param size The size of the MPI communicator.
 */
void third_step(int rank, int size)
{
    queue<unsigned char> q1;
    queue<unsigned char> q2;
    vector<unsigned char> output;
    bool sort_condition = false;
    int recv_numbers = 0;
    unsigned char x;

    int m = 1;

    while (m <= size)
    {
        receive_value(rank, q1, q2, recv_numbers, size);

        if (sort_condition_check(q1, q2, rank, sort_condition))
        {
            sort_condition = true;
        }

        if (sort_condition)
        {
            compare_and_push(q1, q2, output, m);
        }
    }
#ifdef FINAL
    print_output_vector(output);
#endif

#ifdef TESTING
    cout << "Velikost výstupu <" << output.size() << ">" << endl;
#endif
}

/**
 * @brief Main function of the program.
 *
 * This function initializes MPI, retrieves the rank and size of the current process,
 * reads input numbers from a file (if the rank is 0), performs the necessary steps
 * of the parallel sorting algorithm based on the process rank, and finalizes MPI.
 *
 * @param argc The number of command-line arguments.
 * @param argv An array of C-style strings representing the command-line arguments.
 * @return 0 on successful completion of the program.
 */
int main(int argc, char *argv[])
{
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

#ifdef TESTING
    cout << "rank --<" << rank << "> a size <" << size << ">" << endl;
#endif
    if (rank == 0)
    {
        vector<unsigned char> input = read_numbers_from_input_file(size);

        first_step(rank, size, input);
    }
    else
    {
        if (rank != (size - 1))
            second_step(rank, get_subsequence_length(size));
        else
            third_step(rank, get_subsequence_length(size));
    }

    MPI_Finalize();
    return 0;
}