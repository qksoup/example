import sys
import datetime
import pytz

# 'Asia/Shanghai', 'US/Eastern', 'Europe/London', 'Europe/Paris'
def utc_offset(tzname, yyyymmdd):
    dt = datetime.datetime.strptime(yyyymmdd, '%Y%m%d')     
    dt += datetime.timedelta(hours=8) # avoid DST change date 2 AM issue
    tz = pytz.timezone(tzname)
    offset = tz.utcoffset(dt)
    return round(offset.total_seconds() / 60)

#for tz in pytz.all_timezones:
#    print(tz)

yyyymmdd = sys.argv[1] if len(sys.argv) > 1 else '20180102'
print(yyyymmdd, utc_offset('US/Eastern', yyyymmdd))
