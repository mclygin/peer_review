
#define CHECK(Value, min, max) if (dt.Value < min) { throw domain_error(#Value + " is too small"s);}; \
                               if (dt.Value > max) { throw domain_error(#Value + " is too big"s);}    \


void CheckDateTimeValidity(const DateTime& dt) {
    CHECK(year, 1, 9999);
    CHECK(month, 1, 12);

    const bool is_leap_year = (dt.year % 4 == 0) && !(dt.year % 100 == 0 && dt.year % 400 != 0);
    const array month_lengths = { 31, 28 + is_leap_year, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    CHECK(day, 1, month_lengths[dt.month - 1]);
    CHECK(hour, 0, 23);
    CHECK(minute, 0, 59);
    CHECK(second, 0, 59);
}