class sensor{
    public:
        sensor();
        ~sensor();
        void update();
    
        double * get_x();
        double * get_z();
        double * get_lev();
        double * get_v();
        double * get_a();
        double * get_brake();
        double * get_esc();
        double * get_tot();

    private:
        double x, z;
        double * lev;           //levf, levb
        double * v;             //vx, vy, vz;
        double * a;             //ax, ay, az;
        double brake_pressure;  //
        double * esc;           //fl, fr, bl, br; 
        double * tot;           //cu, am, b1, b2;
};
