#ifndef PLOT_H
#define PLOT_H

#define MAX_POINTS 2000
#define WINDOW_SIZE 20

typedef struct {
    float x[MAX_POINTS];
    float y[MAX_POINTS];
    int head;
    int tail;
    int num_points;
    float min_value;
    float max_value;
} PlotData;

void init_graph(PlotData *graph);
void draw_axes();
void add_point(PlotData *graph, float x, float y);
void detect_peaks(PlotData *graph, float *peaks_x, float *peaks_y, int *num_peaks);
void scrollLeft(PlotData *graph);

#endif /* PLOT_H */
