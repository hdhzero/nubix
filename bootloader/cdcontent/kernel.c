void main() {
    #define abs(a) (a) > 0 ? (a) : -(a)
    char* video = (char*) 0xA0000;//0xb8000;
    /*int i;

    for (i = 0; i < 320 * 200; ++i) {
        video[i] = 12;
    }*/

    int x0 = 0, y0 = 0, x1 = 100, y1 = 100;
    int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
    int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1; 
    int err = (dx>dy ? dx : -dy)/2, e2;

    for(;;){
        video[y0 * 320 + x0] = 9;
        if (x0==x1 && y0==y1) break;
        e2 = err;
        if (e2 >-dx) { err -= dy; x0 += sx; }
        if (e2 < dy) { err += dx; y0 += sy; }
    }

}

