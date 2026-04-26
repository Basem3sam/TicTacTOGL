#include <windows.h>
#include <GL/gl.h>
#include <Gl/glut.h>

#include <cmath>
#include <cstdlib>

namespace
{

    const float W = 600.0f, H = 600.0f;
    const float L = 120.0f, B = 120.0f, C = 120.0f;

    char g[3][3] = {};
    char turn = 'X', over = 0;
    bool ai = true;
    bool modeChosen = false;

    void R(float x1, float y1, float x2, float y2, float r, float g2, float b2)
    {
        glColor3f(r, g2, b2);
        glBegin(GL_POLYGON);
        glVertex3f(x1, y1, 0.0f);
        glVertex3f(x2, y1, 0.0f);
        glVertex3f(x2, y2, 0.0f);
        glVertex3f(x1, y2, 0.0f);
        glEnd();
    }

    void O(float cx, float cy, float ro, float ri)
    {
        glColor3f(0.25f, 0.45f, 0.90f);
        glBegin(GL_POLYGON);
        for (int i = 0; i < 36; ++i)
        {
            float a = 6.2831853f * i / 36.0f;
            glVertex3f(cx + ro * std::cos(a), cy + ro * std::sin(a), 0);
        }
        glEnd();
        glColor3f(0.12f, 0.12f, 0.12f);
        glBegin(GL_POLYGON);
        for (int i = 0; i < 36; ++i)
        {
            float a = 6.2831853f * i / 36.0f;
            glVertex3f(cx + ri * std::cos(a), cy + ri * std::sin(a), 0);
        }
        glEnd();
    }

    void S(float x1, float y1, float x2, float y2, float t)
    {
        float dx = x2 - x1, dy = y2 - y1, len = std::sqrt(dx * dx + dy * dy);
        if (len < 0.001f)
            return;
        float px = -dy / len * (t * 0.5f), py = dx / len * (t * 0.5f);
        glColor3f(0.90f, 0.25f, 0.25f);
        glBegin(GL_POLYGON);
        glVertex3f(x1 + px, y1 + py, 0.0f);
        glVertex3f(x2 + px, y2 + py, 0.0f);
        glVertex3f(x2 - px, y2 - py, 0.0f);
        glVertex3f(x1 - px, y1 - py, 0.0f);
        glEnd();
    }

    void X(float cx, float cy, float s)
    {
        S(cx - s, cy - s, cx + s, cy + s, 12.0f);
        S(cx - s, cy + s, cx + s, cy - s, 12.0f);
    }

    void T(float x, float y, const char *s, float r, float g2, float b2)
    {
        glColor3f(r, g2, b2);
        glRasterPos2f(x, y);
        for (const char *p = s; *p; ++p)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *p);
    }

    void reset()
    {
        for (int r = 0; r < 3; ++r)
            for (int c = 0; c < 3; ++c)
                g[r][c] = 0;
        turn = 'X';
        over = 0;
    }

    char result()
    {
        for (int i = 0; i < 3; ++i)
        {
            if (g[i][0] && g[i][0] == g[i][1] && g[i][1] == g[i][2])
                return g[i][0];
            if (g[0][i] && g[0][i] == g[1][i] && g[1][i] == g[2][i])
                return g[0][i];
        }
        if (g[0][0] && g[0][0] == g[1][1] && g[1][1] == g[2][2])
            return g[0][0];
        if (g[0][2] && g[0][2] == g[1][1] && g[1][1] == g[2][0])
            return g[0][2];
        for (int r = 0; r < 3; ++r)
            for (int c = 0; c < 3; ++c)
                if (!g[r][c])
                    return 0;
        return 'D';
    }

    void place(int r, int c)
    {
        if (over || r < 0 || r > 2 || c < 0 || c > 2 || g[r][c])
            return;
        g[r][c] = turn;
        over = result();
        if (!over)
            turn = (turn == 'X') ? 'O' : 'X';
    }

    void aiMove()
    {
        if (!ai || over || turn != 'O')
            return;
        int rr[9], cc[9], n = 0;
        for (int r = 0; r < 3; ++r)
            for (int c = 0; c < 3; ++c)
                if (!g[r][c])
                {
                    rr[n] = r;
                    cc[n] = c;
                    ++n;
                }
        if (n)
            place(rr[std::rand() % n], cc[std::rand() % n]);
    }

    void draw()
    {
        glClear(GL_COLOR_BUFFER_BIT);

        if (!modeChosen)
        {
            R(124, 334, 286, 466, 0.95f, 0.95f, 0.95f);
            R(314, 334, 476, 466, 0.95f, 0.95f, 0.95f);
            R(130, 340, 280, 460, 0.20f, 0.60f, 0.20f);
            R(320, 340, 470, 460, 0.20f, 0.35f, 0.75f);

            X(175, 405, 18);
            O(230, 395, 20, 12);

            X(365, 405, 18);
            R(405, 375, 448, 418, 0.92f, 0.82f, 0.25f);
            R(421, 418, 432, 434, 0.92f, 0.82f, 0.25f);
            R(415, 392, 422, 399, 0.10f, 0.10f, 0.10f);
            R(431, 392, 438, 399, 0.10f, 0.10f, 0.10f);

            T(190, 500, "CHOOSE MODE", 1.0f, 1.0f, 1.0f);
            T(154, 352, "1: 2 PLAYER", 1.0f, 1.0f, 1.0f);
            T(350, 352, "2: VS AI", 1.0f, 1.0f, 1.0f);
            glFlush();
            return;
        }

        R(L, B, L + 3 * C, B + 3 * C, 0.15f, 0.15f, 0.15f);
        R(L + C - 3, B, L + C + 3, B + 3 * C, 1, 1, 1);
        R(L + 2 * C - 3, B, L + 2 * C + 3, B + 3 * C, 1, 1, 1);
        R(L, B + C - 3, L + 3 * C, B + C + 3, 1, 1, 1);
        R(L, B + 2 * C - 3, L + 3 * C, B + 2 * C + 3, 1, 1, 1);

        for (int r = 0; r < 3; ++r)
            for (int c = 0; c < 3; ++c)
            {
                float cx = L + (c + 0.5f) * C, cy = B + (2.5f - r) * C;
                if (g[r][c] == 'X')
                    X(cx, cy, 36);
                if (g[r][c] == 'O')
                    O(cx, cy, 40, 24);
            }

        if (over == 'X')
            X(550, 550, 20);
        else if (over == 'O')
            O(550, 550, 22, 13);
        else if (over == 'D')
        {
            X(530, 550, 14);
            O(570, 550, 16, 10);
        }

        glFlush();
    }

    void mouse(int button, int state, int x, int y)
    {
        if (button != GLUT_LEFT_BUTTON || state != GLUT_UP)
            return;
        float fx = (float)x, fy = H - (float)y;

        if (!modeChosen)
        {
            if (fx >= 130 && fx <= 280 && fy >= 340 && fy <= 460)
            {
                ai = false;
                modeChosen = true;
                reset();
            }
            else if (fx >= 320 && fx <= 470 && fy >= 340 && fy <= 460)
            {
                ai = true;
                modeChosen = true;
                reset();
            }
            draw();
            return;
        }

        if (fx < L || fx > L + 3 * C || fy < B || fy > B + 3 * C)
            return;
        int c = (int)((fx - L) / C), r = 2 - (int)((fy - B) / C);
        place(r, c);
        aiMove();
        draw();
    }

    void keyboard(unsigned char key, int, int)
    {
        if (key == 27)
            std::exit(0);

        if (!modeChosen)
        {
            if (key == '1')
            {
                ai = false;
                modeChosen = true;
                reset();
            }
            else if (key == '2')
            {
                ai = true;
                modeChosen = true;
                reset();
            }
            draw();
            return;
        }

        if (key == 'r' || key == 'R')
            reset();
        else if (key == 't' || key == 'T')
            ai = !ai;
        else if (key >= '1' && key <= '9')
        {
            int m = key - '1';
            place(m / 3, m % 3);
            aiMove();
        }
        draw();
    }

    void init()
    {
        glClearColor(0.08f, 0.08f, 0.08f, 0.0f);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0.0, W, 0.0, H, -1.0, 1.0);
    }

} // namespace

int main(int argc, char **argv)
{
    reset();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize((int)W, (int)H);
    glutInitWindowPosition(120, 80);
    glutCreateWindow("TicTacTOGL Minimal");
    init();
    glutDisplayFunc(draw);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMainLoop();
    return EXIT_SUCCESS;
}
