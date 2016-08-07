class CuadranteClass{
  public:
    int posicion_x;
    int posicion_y;
    int vecino_adelante;
    int vecino_izq;
    int vecino_der;
    int vecino_atras;

  void init(int pos_x, int pos_y, int dx, int dy){
    posicion_x = pos_x;
    posicion_y = pos_y;
    vecino_adelante = pos_x + dx;
    vecino_izq = pos_y + dy;
    vecino_der = pos_y - dy;
    vecino_atras = pos_x - dx;
  }
};