#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>

class Solution {
public:
    bool isAttacked(const int n, const std::vector<std::vector<int>>& board, int row, int col) {
        int checkRow = row - 1;
        int checkCol = col;
        while (checkRow >= 0) {
            if (board[checkRow][checkCol] == 1) return true;
            checkRow--;
        }

        checkRow = row - 1;
        checkCol = col - 1;
        while (checkRow >= 0 && checkCol >= 0) {
            if (board[checkRow][checkCol] == 1) return true;
            checkRow--;
            checkCol--;
        }

        checkRow = row - 1;
        checkCol = col + 1;
        while (checkRow >= 0 && checkCol < n) {
            if (board[checkRow][checkCol] == 1) return true;
            checkRow--;
            checkCol++;
        }

        return false;
    }

    std::vector<std::string> getConf(const std::vector<std::vector<int>>& board) {
        std::vector<std::string> conf;
        for (const auto& row : board) {
            std::string rowConf;
            for (const auto& cell : row) {
                if (cell == 0) rowConf += ".";
                else rowConf += "Q";
            }
            conf.push_back(rowConf);
        }
        return conf;
    }

    void visualizeBoard(const std::vector<std::vector<int>>& board, GLFWwindow* window, bool success = false) {
        glClear(GL_COLOR_BUFFER_BIT);
        int n = static_cast<int>(board.size());
        float squareSize = 2.0f / n;

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                float x = -1.0f + j * squareSize;
                float y = 1.0f - (i + 1) * squareSize;
                if ((i + j) % 2 == 0)
                    glColor3f(1.0f, 1.0f, 1.0f); // White
                else
                    glColor3f(0.0f, 0.0f, 0.0f); // Black
                glBegin(GL_QUADS);
                glVertex2f(x, y);
                glVertex2f(x + squareSize, y);
                glVertex2f(x + squareSize, y + squareSize);
                glVertex2f(x, y + squareSize);
                glEnd();

                if (board[i][j] == 1) {
                    glColor3f(1.0f, 0.0f, 0.0f); // Red
                    glBegin(GL_TRIANGLES);
                    glVertex2f(x + squareSize / 2, y + 0.1f);
                    glVertex2f(x + 0.1f, y + squareSize - 0.1f);
                    glVertex2f(x + squareSize - 0.1f, y + squareSize - 0.1f);
                    glEnd();
                }
            }
        }

        if (success) {
            glColor3f(0.0f, 1.0f, 0.0f); // Green
            glBegin(GL_QUADS);
            glVertex2f(-1.0f, -1.0f);
            glVertex2f(1.0f, -1.0f);
            glVertex2f(1.0f, 1.0f);
            glVertex2f(-1.0f, 1.0f);
            glEnd();
            glFlush();
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        glFlush();
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Add a delay to slow down visualization
    }

    void solve(int n, std::vector<std::vector<std::string>>& result, std::vector<std::vector<int>>& board, int row, GLFWwindow* window, int& solutionCount) {
        if (row == n) {
            std::vector<std::string> currConf = getConf(board);
            result.push_back(currConf);
            solutionCount++;
            visualizeBoard(board, window, true);
            glfwSwapBuffers(window);
            return;
        }

        for (int col = 0; col < n; col++) {
            if (!isAttacked(n, board, row, col)) {
                board[row][col] = 1;
                visualizeBoard(board, window);
                glfwSwapBuffers(window);
                glfwPollEvents();
                solve(n, result, board, row + 1, window, solutionCount);
                board[row][col] = 0;
                visualizeBoard(board, window);
                glfwSwapBuffers(window);
                glfwPollEvents();
            }
        }
    }

    std::vector<std::vector<std::string>> solveNQueens(int n, GLFWwindow* window, int& solutionCount) {
        std::vector<std::vector<std::string>> result;
        std::vector<std::vector<int>> board(n, std::vector<int>(n, 0));
        solve(n, result, board, 0, window, solutionCount);
        return result;
    }
};

void renderAllConfigurations(GLFWwindow* window, const std::vector<std::vector<std::string>>& solutions, int& currentSolutionIndex) {
    glClear(GL_COLOR_BUFFER_BIT);
    int n = static_cast<int>(solutions[currentSolutionIndex].size());
    float squareSize = 2.0f / n;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            float x = -1.0f + j * squareSize;
            float y = 1.0f - (i + 1) * squareSize;
            if ((i + j) % 2 == 0)
                glColor3f(1.0f, 1.0f, 1.0f); // White
            else
                glColor3f(0.0f, 0.0f, 0.0f); // Black
            glBegin(GL_QUADS);
            glVertex2f(x, y);
            glVertex2f(x + squareSize, y);
            glVertex2f(x + squareSize, y + squareSize);
            glVertex2f(x, y + squareSize);
            glEnd();

            if (solutions[currentSolutionIndex][i][j] == 'Q') {
                glColor3f(1.0f, 0.0f, 0.0f); // Red
                glBegin(GL_TRIANGLES);
                glVertex2f(x + squareSize / 2, y + 0.1f);
                glVertex2f(x + 0.1f, y + squareSize - 0.1f);
                glVertex2f(x + squareSize - 0.1f, y + squareSize - 0.1f);
                glEnd();
            }
        }
    }

    glFlush();
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 800, "N-Queens Visualizer", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glOrtho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);

    Solution solution;
    int solutionCount = 0;
    std::vector<std::vector<std::string>> solutions = solution.solveNQueens(5, window, solutionCount);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
            static int currentSolutionIndex = 0;
            renderAllConfigurations(window, solutions, currentSolutionIndex);
            glfwSwapBuffers(window);

            if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
                currentSolutionIndex = (currentSolutionIndex + 1) % solutions.size();
                std::this_thread::sleep_for(std::chrono::milliseconds(200)); // Prevent rapid changes
            }
            else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
                currentSolutionIndex = (currentSolutionIndex - 1 + solutions.size()) % solutions.size();
                std::this_thread::sleep_for(std::chrono::milliseconds(200)); // Prevent rapid changes
            }
        }
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
