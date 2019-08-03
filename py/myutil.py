import sys, os
import datetime
import pytz
import numpy as np
from scipy import stats
import statsmodels.api as sm
import statsmodels.tsa.stattools as ts

holidays = set()
def load_holidays(fpath):
    global holidays
    file = open(fpath)
    for line in file:
        date = line.strip()
        if len(date) > 0:
            holidays.add(date)
    file.close()
    
def prev_bday(yyyymmdd):
    format = '%Y%m%d'
    dt = datetime.datetime.strptime(yyyymmdd, format)
    while True:
        dt -= datetime.timedelta(days=1)
        yyyymmdd = datetime.datetime.strftime(dt, format)
        if yyyymmdd in holidays:
            continue
        if dt.weekday() > 4:
            continue
        return yyyymmdd

def next_bday(yyyymmdd):
    format = '%Y%m%d'
    dt = datetime.datetime.strptime(yyyymmdd, format)
    while True:
        dt += datetime.timedelta(days=1)
        yyyymmdd = datetime.datetime.strftime(dt, format)
        if yyyymmdd in holidays:
            continue
        if dt.weekday() > 4:
            continue
        return yyyymmdd

def third_wday(yyyymm, weekday=2): # weekday is 0-6 (0 = Monday)
    dt = datetime.datetime.strptime(yyyymm + '01', '%Y%m%d')
    dt += datetime.timedelta(days=14)
    while (dt.weekday() != weekday):
        dt += datetime.timedelta(days=1)
    return dt.strftime('%Y%m%d')

# 'Asia/Shanghai', 'US/Eastern', 'Europe/London', 'Europe/Paris'
def utc_offset(tzname, yyyymmdd):
    format = '%Y%m%d'
    dt = datetime.datetime.strptime(yyyymmdd, format)
    dt += datetime.timedelta(hours=8) # avoid DST change date 2 AM issue
    tz = pytz.timezone(tzname)
    offset = tz.utcoffset(dt)
    return round(offset.total_seconds() / 60)

def beta(y, x):
    mask = ~np.isnan(y) & ~np.isnan(x)
    y = y[mask]
    x = x[mask]
    slope, intercept, r_value, p_value, std_err = stats.linregress(x, y)
    return intercept, slope, p_value

def adf_test(data):
    cadf = ts.adfuller(data)    
    p_value = cadf[1]
    
    data_lag = data.shift(1)
    data_lag.iloc[0] = data_lag.iloc[1]
    data_ret = data - data_lag
    data_ret.iloc[0] = data_ret.iloc[1]
    data_lag2 = sm.add_constant(data_lag)
    model = sm.OLS(data_ret, data_lag2)
    res = model.fit()
    halflife = round(-np.log(2) / res.params[1], 0)
    return p_value, halflife
