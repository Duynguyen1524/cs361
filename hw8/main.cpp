#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <string>
#include <fstream>
#include <cstdlib>
#include <algorithm>
struct Context {
    int n;                     ///< modulus n (odd, > 1)
    int length;                ///< sequence length = (n-1)/2 + 1
    int forbidden_value;       ///< ceil(n/2), cannot appear in the sequence
    std::mutex result_mutex;   ///< protects access to results
    std::vector<std::vector<int>> results; ///< all valid sequences found
};
void print_out(const std::vector<std::vector<int>>& results, const std::string& output_file);
void backtrack(Context* ctx, int pos, std::vector<int>& seq,
               std::vector<bool>& used_values,
               std::vector<bool>& used_diff);
void generate_sequences(Context* ctx);
int conflict_5(int x, int n) {
    int c = (1 - x) % n;
    if (c < 0) c += n;
    return c;
}//rule 5
int diff_pair(int d, int n) {
    int partner = (n - d) % n;
    return std::min(d, partner);
}//rule 6
int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << "./bin/sequence" << " <n> " <<"outputfile.txt"<< std::endl;
        return 1;
    }
    int n = std::stoi(argv[1]);
    std::string output_file = argv[2];
    if (n < 1 || n  % 2 == 0) {
        std::cerr << "Error: n must be a odd integer." << std::endl;
        return 1;
    }
    int valid_len = (n-1)/2+1;//rule 1
    
    int forbidden = (n + 1) / 2;

    
    Context context{n, valid_len, forbidden};
    

    generate_sequences(&context);
    print_out(context.results, output_file);
    
    return 0;
}
void backtrack(Context* ctx, int pos, std::vector<int>& seq,
               std::vector<bool>& used_values,
               std::vector<bool>& used_diff) {
    const int n = ctx->n;
    const int length = ctx->length;
    const int forbidden = ctx->forbidden_value;
    if (pos == length - 1) {
        // Ensure 1 hasn't already been used
        if (used_values[1]) {
            return;
        }

        int prev = seq[pos - 1];
        int d = (1 - prev + n) % n;
        if (d == 0) {
            return; // adjacent elements can't be equal; would give diff 0
        }
        int diff_rep = diff_pair(d, n);
        if (used_diff[diff_rep]) {
            return; // difference already used
        }
        seq[pos] = 1;
        used_values[1] = true;
        used_diff[diff_rep] = true;

        // Save result (copy) under lock.
        {
            std::lock_guard<std::mutex> lock(ctx->result_mutex);
            ctx->results.push_back(seq);
        }
          return;
    }
    int prev = seq[pos - 1];

    for (int x = 2; x < n; ++x) {
        // 0 and 1 are reserved for endpoints; we never place them in the middle.
        if (x == 0 || x == 1) {
            continue;
        }
        // Rule 4: cannot include forbidden_value = ceil(n/2).
        if (x == forbidden) {
            continue;
        }
        // Already used in sequence?
        if (used_values[x]) {
            continue;
        }
        // Rule 5: cannot have both x and (1 - x) mod n (for 1 < x < n).
        int cp = conflict_5(x, n);
        if (used_values[cp]) {
            continue;
        }

        // Check Rule 6 for the new adjacent difference.
        int d = (x - prev + n) % n;
        if (d == 0) {
            continue; // would imply duplicate values
        }
        int canon = diff_pair(d, n);
        if (used_diff[canon]) {
            continue;
        }

        // Choose x.
        seq[pos] = x;
        used_values[x] = true;
        used_diff[canon] = true;

        // Recurse to next position.
        backtrack(ctx, pos + 1, seq, used_values, used_diff);

        // Backtrack.
        used_diff[canon] = false;
        used_values[x] = false;
    }


}
void print_out(const std::vector<std::vector<int>>& results, const std::string& output_file){
    std::ofstream ofs(output_file);
    if (!ofs) {
        std::cerr << "Error: Could not open output file " << output_file << std::endl;
        return;
    }
    for (const auto& seq : results) {
        for (size_t i = 0; i < seq.size(); ++i) {
            ofs << seq[i];
            if (i != seq.size() - 1) {
                ofs << " ";
            }
        }
        ofs << "\n";
    }
    ofs.close();
}
void generate_sequences(Context* ctx) {
   
    std::vector<int> seq(ctx->length);
    seq[0] = 0;
    std::vector<bool> used_values(ctx->n, false);
    used_values[0] = true; // 0 is used at the start
    std::vector<bool> used_diff(ctx->n / 2, false);
    int threads_count = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    // Launch threads to explore different options for position 1
    

    for(int i= 2; i<= ctx->n; ++i){
        if(i == ctx->forbidden_value){
            continue;
        }
        if(threads.size() >= threads_count){
            break;
        }
        threads.emplace_back(backtrack, ctx, 1, std::ref(seq), std::ref(used_values), std::ref(used_diff));
    }

    for (auto& t : threads) {
        t.join();
    }
}



