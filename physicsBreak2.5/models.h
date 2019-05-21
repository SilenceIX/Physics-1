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

class Model
{
public:
    virtual void Init() = 0;
    virtual void Update(double) = 0;
    virtual Qt3DCore::QEntity *GetEntity() = 0;
    virtual QVBoxLayout *GetSet() = 0;
    virtual QVBoxLayout *GetInf() = 0;
    virtual QString GetName() = 0;
    virtual ~Model() = default;
};


class Model1 : public virtual Model, QObject
{
private:
    double A0, beta, omega, angle, t, r, c, k, m;
    Qt3DCore::QEntity *ent;
    QVBoxLayout *set, *inf;
    QLabel *i1, *i2, *k1, *k2, *k3, *k4, *k5, *k6;
    QSlider *s1, *s2, *s3, *s4, *s5, *s6;
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
    Qt3DCore::QEntity *GetEntity();
    QVBoxLayout *GetSet();
    QVBoxLayout *GetInf();
    QString GetName() {return "Торсионный маятник";}
    ~Model1(){}
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
    QLabel *i1, *i2, *k1, *k2, *k3;
    QSlider *s1, *s2, *s3;
    Qt3DCore::QTransform *tr1, *tr2, *tr3, *tr4;
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
    QString GetName() {return "Моделирование прецессии и нутации гироскопа";}
    ~Model2(){}
};

class Model3 : public virtual Model, QObject
{
private:
    const double pl = 1.;
    const double Cx = 0.3;
    double M, L, A, W, S, E, Ek, Ep, r, time;
    double sM, sL, sA, sW, sr;
    void Transform();
    void LoadModel();
    Qt3DCore::QEntity *ent;
    QVBoxLayout *set, *inf;
    QLabel *i1, *i2, *i3, *i4, *k1, *k2, *k3, *k4, *k5;
    QSlider *s1, *s2, *s3, *s4, *s5;
    Qt3DCore::QTransform *tr1, *tr2;
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
    QString GetName() {return "Колебания жесткого математического маятника с большими амплитудами";}
    ~Model3(){}
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
    QVBoxLayout *box                  = nullptr;
    QSlider     *bullet_mass_slider   = nullptr;
    QSlider     *pendulum_mass_slider = nullptr;
    QSlider     *k_slider             = nullptr;
    QSlider     *b_slider             = nullptr;
    QLabel      *bullet_mass_Label    = nullptr;
    QLabel      *pendulum_mass_Label  = nullptr;
    QLabel      *k_label              = nullptr;
    QLabel      *b_label              = nullptr;

public:
    Model4();
    void start() const;

    float get_l() const;
    float get_g() const;
    float get_k() const;
    float get_b() const;
    float get_T() const;
    float get_omega() const;
    physics::bullet   *get_bullet() const;
    physics::pendulum *get_pendulum() const;
    physics::measurer *get_measurer() const;

    void Init();
    void Update(double){return;}
    Qt3DCore::QEntity *GetEntity() {return root;}
    QVBoxLayout *GetSet(){return box;}
    QVBoxLayout *GetInf(){return new QVBoxLayout();}
    QString GetName(){return "Балистический маятник";}
    ~Model4(){}
};


class Model5 : public virtual Model, QObject
{
private:

    void Transform();
    void LoadModel();
    Qt3DCore::QEntity *ent;
    QVBoxLayout *set, *inf;
    QLabel *i1, *k1;
    QSlider *s1;
    Qt3DCore::QTransform *tr1, *tr2, *pruz;
    QList<Plot *> plots;

    const int  k=100;               //коэффициент жесткости пружины, Н/м
    const double l=0.5;             //длина нити, м
    const double m=0.2;             //масса маятника (шара), кг
    const double w1 =sqrt(g/l);     //1 собственная частота, 1/с^2
    double d, st_angle1, st_angle2, t, angle1, angle2, Ep1, Ep2, Ek1, Ek2;
    double D = 0.1, Start_angle1 = PI / 6, Start_angle2 = 0.0;
public:
    Model5();
    void Init();
    void Compute(double);
    void Update(double);
    void CreatePlot(int);
    Qt3DCore::QEntity *GetEntity(){return ent; }
    QVBoxLayout *GetSet(){return set; }
    QVBoxLayout *GetInf() {return inf; }
    QString GetName() {return "Связанные физические маятники";}
    ~Model5(){}
    double FuncW2();
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
    QString GetName() {return "Оборотный маятник";}
    ~Model6(){}
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
    Qt3DCore::QTransform *tr1;
    QList<Plot *> plots;
public:
    Model7();
    void Init();
    void Compute(double);
    void Update(double);
    void CreatePlot(int);
    Qt3DCore::QEntity *GetEntity(){return ent; }
    QVBoxLayout *GetSet(){return set; }
    QVBoxLayout *GetInf() {return inf; }
    QString GetName() {return "Маятник Галилея";}
    ~Model7(){}
};













#endif // MODELS_H
