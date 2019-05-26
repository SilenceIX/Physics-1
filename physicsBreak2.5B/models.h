#ifndef MODELS_H
#define MODELS_H
#include <Qt3DExtras>
#include <Qt3DRender>
#include <Qt3DCore>
#include <QVBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QPushButton>
#include <cmath>
#include "plot.h"
#include "model4/measurer.h"

Qt3DCore::QEntity *addObject(Qt3DCore::QEntity *, QString, QString);


const double PI = M_PI;
const double e = std::exp(1.);
const double g = 9.8066;
const double toGrad = 180. / PI;
const int timesPrint = 10;

class Model
{
public:
    virtual void Init() = 0;
    virtual void Update(double) = 0;
    virtual Qt3DCore::QEntity *GetEntity() = 0;
    virtual QVBoxLayout *GetSet() = 0;
    virtual QVBoxLayout *GetInf() = 0;
    virtual void lock(bool) = 0;
    virtual void GetMenu(QMenu *) = 0;
    virtual QString GetName() = 0;
    virtual ~Model() = default;
};


class Model1 : public virtual Model, QObject
{
private:
    double A0, beta, omega0,omega, angle, t, r, c, k, m, R, J, A;
    Qt3DCore::QEntity *ent;
    QVBoxLayout *set, *inf;
    QLabel *i1, *i2, *k1, *k2, *k3, *k4, *k5, *k6;
    QSlider *s1, *s2, *s4, *s5, *s6;
    QCheckBox *cGraf;
    QSlider *sGraf;
    Qt3DCore::QTransform *tr1, *tr2, *tr3;
    QList<Plot *> plots;
public:
    Model1();
    void Init();
    void Compute();
    void Transform();
    void Update(double);
    void CreatePlot(int);
    double GetA();
    double GetBeta();
    double GetOmega();
    double GetAngle();
    double GetTime();
    double GetEp(){return 0.5*(k * pow(A*cos(omega0 * t),2.));}
    double GetEk(){return 0.5*(J * pow(A * (-beta * cos(omega0 * t) - omega0 * sin(omega0 * t)), 2));}
    void GetMenu(QMenu *);
    Qt3DCore::QEntity *GetEntity();
    QVBoxLayout *GetSet();
    QVBoxLayout *GetInf();
    QString GetName() {return "Колебания торсионного маятника";}
    void lock(bool);
    ~Model1(){}
    void Update_plot(double dt, int maxtime);
};

class Model2 : public virtual Model, QObject
{
private:
    double I_psi, I0;
    double mass, radius, length;
    double psi, psi_dot;
    double phi, phi_dot;
    double theta, theta_dot;
    double L_psi, L_phi;
    double time;
    void CalculateConstants();
    void Transform();
    void LoadModel();
    Qt3DCore::QEntity *ent;
    QVBoxLayout *set, *inf;
    QLabel *i1, *i2;
    Qt3DCore::QTransform *tr1, *tr2, *tr3, *tr4;
    QSlider *s1, *s2, *s3, *s4, *s5, *s6;
    QList<Plot *> plots;
    double dy1(double arg);
    double dy2(double arg);
    double dy3(double arg);
    double dy4(double arg);
    QQuaternion rotation, nutation, precession;
    QElapsedTimer *t;
public:
    Model2();
    void Init();
    void Compute(double);
    void Update(double);
    void CreatePlot(int);
    double GetPsi() { return psi; }
    double GetPhi() { return phi; }
    double GetTheta() { return theta; }
    double GetTime() { return time; }
    Qt3DCore::QEntity *GetEntity(){return ent; }
    QVBoxLayout *GetSet(){return set; }
    QVBoxLayout *GetInf() {return inf; }
    void GetMenu(QMenu *) {return;}
    QString GetName() {return "Моделирование прецессии и нутации гироскопа";}
    ~Model2(){}
    void SetTransform();
    void lock(bool);
};

class Model3 : public virtual Model, QObject
{
private:
    const double pl = 1.;
    const double Cx = 0.3;
    double M, L, A, W, S, E, Ek, Ep, r, time;
    double sM, sL, sA, sW, sr;
    double YSize;
    void Transform();
    void LoadModel();
    Qt3DCore::QEntity *ent;
    QVBoxLayout *set, *inf;
    QLabel *i1, *i2, *i3, *i4, *k1, *k2, *k3, *k4, *k5;
    QSlider *s1, *s2, *s3, *s4, *s5;
    QCheckBox *cGraf;
    QSlider *sGraf;
    Qt3DCore::QTransform *tr1, *tr2, *tr3, *tr4;
    QList<Plot *> plots;
    double func(double axis, double speed);
public:
    Model3();
    void Init();
    void Compute(double);
    void Update(double);
    void CreatePlot(int);
    double GetA() {return A;}
    double GetW() {return W;}
    double GetEk() {return Ek;}
    double GetEp() {return Ep;}
    double GetEnergy() {return Ek + Ep;}
    double GetTime() {return time;}
    Qt3DCore::QEntity *GetEntity(){return ent; }
    QVBoxLayout *GetSet(){return set; }
    QVBoxLayout *GetInf() {return inf; }
    void GetMenu(QMenu *);
    QString GetName() {return "Колебания жесткого математического маятника с большими амплитудами";}
    ~Model3(){}
    void Update_plot(double dt, int maxtime);
    void lock(bool);
};

class Model4 : public virtual Model, QObject
{
private:
    Qt3DCore::QEntity       *root     = nullptr;
    physics ::object        *lab      = nullptr;
    physics ::bullet        *bullet   = nullptr;
    physics ::pendulum      *pendulum = nullptr;
    physics ::measurer      *measurer = nullptr;
    QPropertyAnimation      *P        = nullptr;
    QPropertyAnimation      *M        = nullptr;
    QParallelAnimationGroup *group    = nullptr;

    QVBoxLayout *opt                  = nullptr;
    QVBoxLayout *inf                  = nullptr;
    QSlider     *bullet_mass_slider   = nullptr;
    QSlider     *pendulum_mass_slider = nullptr;
    QSlider     *k_slider             = nullptr;
    QSlider     *b_slider             = nullptr;
    QLabel      *bullet_mass_label    = nullptr;
    QLabel      *pendulum_mass_label  = nullptr;
    QLabel      *k_label              = nullptr;
    QLabel      *b_label              = nullptr;
    QLabel      *move_label           = nullptr;
    QLabel      *speed_label          = nullptr;

public:
    Model4();
    void start() const;

    float get_l() const;
    float get_g() const;
    float get_k() const;
    float get_b() const;
    float get_T() const;
    float get_omega() const;
    float get_time()  const;
    QVBoxLayout *get_opt() const;
    QVBoxLayout *get_inf() const;
    physics::bullet   *get_bullet() const;
    physics::pendulum *get_pendulum() const;
    physics::measurer *get_measurer() const;

    void Init();
    void Update(double){return;}
    Qt3DCore::QEntity *GetEntity() {return root;}

    QVBoxLayout *GetSet(){return get_opt();}
    QVBoxLayout *GetInf(){return get_inf();}
    void GetMenu(QMenu *) {return;}
    QString GetName(){return "Измерение скорости пули с помощью баллистического маятника";}
    ~Model4(){}
    void lock(bool);
};

class Model5 : public virtual Model, QObject
{
private:

    void Transform();
    void LoadModel();
    Qt3DCore::QEntity *ent;
    QVBoxLayout *set, *inf;
    QLabel *i1;
    QSlider *s1, *s2, *s3, *s4, *s5;
    Qt3DCore::QTransform *tr1, *tr2, *pruz;
    QList<Plot *> plots;
    double  k;              //коэффициент жесткости пружины, Н/м
    double l=0.5;             //длина нити, м
    double m=0.2;             //масса маятника (шара), кг
    double w1;     //1 собственная частота, 1/с^2
    double Start_angle1, Start_angle2;
    double D;
    double w2;                      //2 собственная частота, 1/с^2
    const double rad=M_PI/180.;     //перевод градусов в радианы
    double d, t, angle1, angle2, W, Wm, E1, E2;
public:
    Model5();
    void Init();
    void Compute(double);
    void Update(double);
    void CreatePlot(int);
    Qt3DCore::QEntity *GetEntity(){return ent; }
    QVBoxLayout *GetSet(){return set; }
    QVBoxLayout *GetInf() {return inf; }
    void GetMenu(QMenu *) {return;}
    QString GetName() {return "Колебания связанных физических маятников";}
    ~Model5(){}
    void Func(double);
    double FuncW2();
    double Get_Angle1();
    double Get_Angle2();
    void Get_W_Wm();
    double Get_E1();
    double Get_E2();
    void lock(bool){}
};

class Model6 : public virtual Model, QObject
{
private:

    void Transform();
    void LoadModel();
    Qt3DCore::QEntity *ent;
    QVBoxLayout *set, *inf;
    QLabel *i1, *k1;
    QSlider *s1;
    Qt3DCore::QTransform *tr1;
    QList<Plot *> plots;



public:
    Model6();
    void Init();
    void Compute(double);
    void Update(double);
    void CreatePlot(int);
    Qt3DCore::QEntity *GetEntity(){return ent; }
    QVBoxLayout *GetSet(){return set; }
    QVBoxLayout *GetInf() {return inf; }
    void GetMenu(QMenu *) {return;}
    QString GetName() {return "Колебания оборотного маятника";}
    ~Model6(){}
    void lock(bool){}
};

class Model7 : public virtual Model, QObject
{
private:

    void Transform();
    void LoadModel();
    Qt3DCore::QEntity *ent;
    QVBoxLayout *set, *inf;
    QLabel *i1, *k1;
    QSlider *s1;
    Qt3DCore::QTransform *tr1, *g1, *g2, *gq;
    QList<Plot *> plots;
    const double l = 1.0;
    double scr = 0;



    double A0, h, m, t;
    double angle, W, W1, k, velocity, Ek, Ep, E, x, y, t1, t2, T_left, T_right;

    void Compute_left();
    void Compute_right();
public:
    Model7();
    void Init();
    void Compute(double);
    void Update(double);
    void CreatePlot(int);
    Qt3DCore::QEntity *GetEntity(){return ent; }
    QVBoxLayout *GetSet(){return set; }
    QVBoxLayout *GetInf() {return inf; }
    void GetMenu(QMenu *) {return;}
    QString GetName() {return "Колебания маятника Галилея";}
    ~Model7(){}
    void lock(bool){}
};




#endif // MODELS_H
