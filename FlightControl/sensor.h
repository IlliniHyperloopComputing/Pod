class sensor{
    public:
        sensor();
        ~sensor();
        void update();
        void create_message(std::string & buff);
    
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

        uint8_t tick;
        const uint8_t x_timing = 1;       //every 5 ticks
        const uint8_t z_timing = 4;
        const uint8_t lev_timing = 3;      
        const uint8_t v_timing = 2;       
        const uint8_t a_timing = 1;
        const uint8_t brake_timing = 1;     
        const uint8_t esc_timing = 4;
        const uint8_t tot_timing = 4;
};
