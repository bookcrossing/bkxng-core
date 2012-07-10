This is the Document for date range rule.
1,
Here are some examples of date range:
{global}[2012-02-10 08:30:00, 2012-02-13 20:30:00]
{year}(02-10 08:30:00, 02-13 20:30:00]
{month}(07 08:30:00, 13 20:30:00)
{week}(1 08:30:00, 5 20:30:00)
{day}[08:30:00, 20:30:00)
{hour}[10:00, 35:00]
{minute}[00, 35]
{global}[now, +1 month]
2,
There are two parts in a date range:
(1),cycle time,the fist part, available values: global, year, month, week, day, hour, minute. 
(2)start time and end time, here we only support date format"Y-m-d H:i:s",here is the relation: 
cycle,   date format 
{global}   "Y-m-d H:i:s" 
{year}     "m-d H:i:s"
{month}    "d H:i:s"
{week}     "w H:i:s"
{day}      "H:i:s"
{hour}     "i:s"
{minute}   "s"
3,
When sycle is "global", it support more date formats which could be converted through strtotime(),such as "now", "+1 month","+1 day","+1 year" etc.
4,
"[" represent that great or equal(">=").
"(" represent that great(">"). 
"]" represent that less or equal("<="). 
")" represent that less("<"). 

5,  "," is a seperator in the second part, It is used to seperate start date and end date. When sycle is "global", at here we support more date format, Please make sure that you do not include symbol ","  in start date or in end date.

6, It is a little tricky, we also support:
{global}[2015-02-10 08:30:00, 2012-02-13 20:30:00]
{year}(11-10 08:30:00, 02-13 20:30:00]
{month}(22 08:30:00, 06 20:30:00)
{week}(6 08:30:00, 0 20:30:00)
{day}[22:30:00, 08:30:00)
{hour}[50:00, 10:00]
{minute}[55, 15]

Do you know their meanings? Yes ,we support this function.