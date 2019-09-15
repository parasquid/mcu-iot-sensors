class HoneywellHPM {
  private:
    Stream* sensor;
  public:
    HoneywellHPM (Stream* sensor_param);
    bool startFan();
    bool stopFan();

    bool startAutoSend();
    bool stopAutoSend();
    int available();
    int read();
};
