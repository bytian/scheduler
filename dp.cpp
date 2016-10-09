#include "dp.h"
#include "scheduler.h"
#include "object.h"
#include "simulator.h"
#include "transaction.h"

#include <vector>
#include <set>
#include <algorithm>
#include <iostream>

const double DP::ALPHA = 0;

DP::DP() : Scheduler() {}

DP::DP(Simulator* sim) : sim(sim) {}

void DP::init()
{
    exclTrans.resize(sim->getTotalObj());
    inclTrans.resize(sim->getTotalObj());
    sizeO.resize(sim->getTotalObj(), 0);
    sizeT.resize(sim->getTotalTrans(), 0);
}

bool DP::acquire(int tid, int oid, bool excl)
{
    int status = sim->getObj(oid).getStatus();

    if (status == Object::FREE)
    {
        updateT(tid, sizeO[oid]);
        std::set<int> trans;
        trans.insert(tid);
        sim->getObj(oid).addOwner(trans, excl);
        return true;
    }

    if (excl)
    {
        exclTrans[oid].push_back(tid);
        updateO(oid, 1 + sizeT[tid]);
    }
    else
    {
        inclTrans[oid].push_back(tid);
        updateO(oid, 1 + sizeT[tid]);
    }

    return false;
}

void DP::release(int tid, int oid)
{
    sim->getObj(oid).releaseBy(tid);
    updateT(tid, -sizeO[oid]);

    if (sim->getObj(oid).getStatus() == Object::FREE)
    {
        if (!exclTrans[oid].empty() || !inclTrans[oid].empty())
        {
            sim->addToAssign(oid);
        }
    }
}

const std::set<int> DP::assign(int oid)
{
    std::set<int> assigned;
    int l = run_dp(oid);

    if (l == -1)
    {
        int trans = exclTrans[oid][0];
        updateO(oid, -sizeT[trans] - 1);
        sim->getTrans(trans).grantLock();
        updateT(trans, sizeO[oid]);

        assigned.insert(trans);
        exclTrans[oid].erase(exclTrans[oid].begin());

        sim->getObj(oid).addOwner(assigned, true);
    }
    else
    {
        int inclSize = 0;
        for (int i = 0; i < l; ++i)
        {
            inclSize += 1 + sizeT[inclTrans[oid][i]];
        }
        
        updateO(oid, -inclSize);
        for (int i = 0; i < l; ++i)
        {
            sim->getTrans(inclTrans[oid][i]).grantLock();
            updateT(inclTrans[oid][i], sizeO[oid]);
            assigned.insert(inclTrans[oid][i]);
        }

        inclTrans[oid].erase(inclTrans[oid].begin(), inclTrans[oid].begin() + l);
        
        sim->getObj(oid).addOwner(assigned, false);
    }

    return assigned;
}

int DP::getTime() { return sim->getTime(); }

void DP::updateO(int oid, int delta)
{
    sizeO[oid] += delta;
    for (auto itr = sim->getObj(oid).getOwner().begin(); itr != sim->getObj(oid).getOwner().end(); ++itr)
    {
        updateT(*itr, delta);
    }
}

void DP::updateT(int tid, int delta)
{
    sizeT[tid] += delta;
    int oid = sim->getTrans(tid).getBlockBy();
    if (oid > 0)
    {
        updateO(oid, delta);
    }
}

int DP::run_dp(int oid)
{
    sort(exclTrans[oid].begin(), exclTrans[oid].end(), Cmp(this));
    sort(inclTrans[oid].begin(), inclTrans[oid].end(), Cmp(this));

    int n = inclTrans[oid].size();
    int m = exclTrans[oid].size();
    std::vector<std::vector<std::vector<double > > > d(n + 2, std::vector<std::vector<double> >(m + 1, std::vector<double>(m + n + 1, -1.)));
    std::vector<std::vector<std::vector<double> > > step(n + 1, std::vector<std::vector<double> >(m + 1, std::vector<double>(m + n + 1, -1)));
    std::vector<int> sumIn(n + 1);
    std::vector<int> sumEx(m + 1);
    sumIn[n] = 0;
    sumEx[m] = 0;

    for (int i = n - 1; i >= 0; --i)
    {
        sumIn[i] = sumIn[i + 1] + sizeT[inclTrans[oid][i]];
    }
    for (int i = m - 1; i >= 0; --i)
    {
        sumEx[i] = sumEx[i + 1] + sizeT[exclTrans[oid][i]];
    }

    d[0][0][0] = 0.;

    for (int j = 0; j < m; ++j)
    {
        for (int i = 0; i < n; ++i)
        {
            for (int t = j; t <= i + j; ++t)
            {
                if (d[i][j][t] < 0.) continue;
                    
                double x = d[i][j][t] + sumEx[j + 1] + sumIn[i] + m - j - 1 + n - i + .5 * ALPHA * (sumEx[j] + sumIn[i]) + ALPHA * (sumEx[j + 1] + sumIn[i]) * t;

                if (d[i][j + 1][t + 1] < 0 || x < d[i][j + 1][t + 1])
                {
                    d[i][j + 1][t + 1] = x;
                    step[i][j + 1][t + 1] = -1;
                }

                for (int l = i + 1; l <= n; ++l)
                {
                    double x = d[i][j][t] + (sumIn[i] - sumIn[l]) + (sumEx[j] + sumIn[l] + m - j + n - l) * f(l - i) + .5 * ALPHA * (sumEx[j] + sumIn[i]) * f(l - i) * f(l - i) + ALPHA * (sumEx[j] + sumIn[l]) * t * f(l - i);

                    int k = t + ceil(f(l - i));

                    if (d[l][j][k] < 0 || x < d[l][j][k])
                    {
                        d[l][j][k] = x;
                        step[l][k][j] = i;
                    }
                }
            }
        }
    }

    double minD = std::numeric_limits<double>::infinity();
    double minT = -1;

    for (int t = m; t <= m + n; ++t)
    {
        if (d[n][m][t] < minD)
        {
            minD = d[n][m][t];
            minT = t;
        }
    }

    int i = n, j = m, t = minT;
    int next;
    while (true)
    {
        int ii = i, jj = j;
        if (step[i][j][t] == -1)
        {
            --jj;
            --t;
            if (ii == 0 && jj == 0)
            {
                next = -1;
                break;
            }
        }
        else
        {
            t -= ceil(f(i - step[i][j][t]));
            ii = step[i][j][t];
            if (ii == 0 && jj == 0)
            {
                next = ii;
                break;
            }
        }
        
        i = ii;
        j = jj;
    }

    return next;
}
