#include "data.h"
#include "malloc.h"
#include "astar.h"

PointROM tabPoint[NB_POINTS] = {
//id  x   y    liste des fils
{1,2800,400,{1009,2,8-1,-1,-1,-1,-1}},
{2,2800,600,{1008,1010,1,3,9-1,-1,-1}},
{3,2800,800,{1009,1011,2,4,10-1,-1,-1}},
{4,2800,1000,{1010,1012,3,5,11-1,-1,-1}},
{5,2800,1200,{1011,1013,4,6,12-1,-1,-1}},
{6,2800,1400,{1012,1014,5,7,13-1,-1,-1}},
{7,2800,1600,{1013,6,14-1,-1,-1,-1,-1}},
{8,2600,400,{1015,1017,1002,9,16,1-1,-1}},
{9,2600,600,{1016,1018,1001,1003,8,10,17,2}},
{10,2600,800,{1017,1019,1002,1004,9,11,18,3}},
{11,2600,1000,{1018,1020,1003,1005,10,12,19,4}},
{12,2600,1200,{1019,1021,1004,1006,11,13,20,5}},
{13,2600,1400,{1020,1022,1005,1007,12,14,21,6}},
{14,2600,1600,{1021,1023,1006,13,22,7-1,-1}},
{15,2400,200,{1025,1008,16,24-1,-1,-1,-1}},
{16,2400,400,{1024,1026,1009,15,17,25,8-1}},
{17,2400,600,{1025,1027,1008,1010,16,18,26,9}},
{18,2400,800,{1026,1028,1009,1011,17,19,27,10}},
{19,2400,1000,{1027,1029,1010,1012,18,20,28,11}},
{20,2400,1200,{1028,1030,1011,1013,19,21,29,12}},
{21,2400,1400,{1029,1031,1012,1014,20,22,30,13}},
{22,2400,1600,{1030,1032,1013,21,23,31,14-1}},
{23,2400,1800,{1031,1014,22,32-1,-1,-1,-1}},
{24,2200,200,{1034,1016,25,33,15-1,-1,-1}},
{25,2200,400,{1033,1035,1015,1017,24,26,34,16}},
{26,2200,600,{1034,1036,1016,1018,25,27,35,17}},
{27,2200,800,{1035,1037,1017,1019,26,28,36,18}},
{28,2200,1000,{1036,1038,1018,1020,27,29,37,19}},
{29,2200,1200,{1037,1039,1019,1021,28,30,38,20}},
{30,2200,1400,{1038,1020,1022,29,31,39,21-1}},
{31,2200,1600,{1039,1021,1023,30,32,22-1,-1}},
{32,2200,1800,{1022,31,23-1,-1,-1,-1,-1}},
{33,2000,200,{1041,1025,34,40,24-1,-1,-1}},
{34,2000,400,{1040,1042,1024,1026,33,35,41,25}},
{35,2000,600,{1041,1043,1025,1027,34,36,42,26}},
{36,2000,800,{1042,1044,1026,1028,35,37,43,27}},
{37,2000,1000,{1043,1045,1027,1029,36,38,44,28}},
{38,2000,1200,{1044,1046,1028,1030,37,39,45,29}},
{39,2000,1400,{1045,1029,1031,38,46,30-1,-1}},
{40,1800,200,{1048,1034,41,47,33-1,-1,-1}},
{41,1800,400,{1047,1049,1033,1035,40,42,48,34}},
{42,1800,600,{1048,1050,1034,1036,41,43,49,35}},
{43,1800,800,{1049,1051,1035,1037,42,44,50,36}},
{44,1800,1000,{1050,1052,1036,1038,43,45,51,37}},
{45,1800,1200,{1051,1037,1039,44,46,52,38-1}},
{46,1800,1400,{1052,1038,45,39-1,-1,-1,-1}},
{47,1600,200,{1054,1041,48,53,40-1,-1,-1}},
{48,1600,400,{1053,1055,1040,1042,47,49,54,41}},
{49,1600,600,{1054,1056,1041,1043,48,50,55,42}},
{50,1600,800,{1055,1057,1042,1044,49,51,56,43}},
{51,1600,1000,{1056,1058,1043,1045,50,52,57,44}},
{52,1600,1200,{1057,1044,1046,51,58,45-1,-1}},
{53,1400,200,{1060,1048,54,59,47-1,-1,-1}},
{54,1400,400,{1059,1061,1047,1049,53,55,60,48}},
{55,1400,600,{1060,1062,1048,1050,54,56,61,49}},
{56,1400,800,{1061,1063,1049,1051,55,57,62,50}},
{57,1400,1000,{1062,1064,1050,1052,56,58,63,51}},
{58,1400,1200,{1063,1065,1051,57,64,52-1,-1}},
{59,1200,200,{1067,1054,60,66,53-1,-1,-1}},
{60,1200,400,{1066,1068,1053,1055,59,61,67,54}},
{61,1200,600,{1067,1069,1054,1056,60,62,68,55}},
{62,1200,800,{1068,1070,1055,1057,61,63,69,56}},
{63,1200,1000,{1069,1071,1056,1058,62,64,70,57}},
{64,1200,1200,{1070,1072,1057,63,65,71,58-1}},
{65,1200,1400,{1071,1058,64,72-1,-1,-1,-1}},
{66,1000,200,{1074,1060,67,73,59-1,-1,-1}},
{67,1000,400,{1073,1075,1059,1061,66,68,74,60}},
{68,1000,600,{1074,1076,1060,1062,67,69,75,61}},
{69,1000,800,{1075,1077,1061,1063,68,70,76,62}},
{70,1000,1000,{1076,1078,1062,1064,69,71,77,63}},
{71,1000,1200,{1077,1079,1063,1065,70,72,78,64}},
{72,1000,1400,{1078,1080,1064,71,79,65-1,-1}},
{73,800,200,{1083,1067,74,82,66-1,-1,-1}},
{74,800,400,{1082,1084,1066,1068,73,75,83,67}},
{75,800,600,{1083,1085,1067,1069,74,76,84,68}},
{76,800,800,{1084,1086,1068,1070,75,77,85,69}},
{77,800,1000,{1085,1087,1069,1071,76,78,86,70}},
{78,800,1200,{1086,1088,1070,1072,77,79,87,71}},
{79,800,1400,{1087,1089,1071,78,80,88,72-1}},
{80,800,1600,{1088,1090,1072,79,81,89-1,-1}},
{81,800,1800,{1089,80,90-1,-1,-1,-1,-1}},
{82,600,200,{1091,1074,83,73-1,-1,-1,-1}},
{83,600,400,{1092,1073,1075,82,84,91,74-1}},
{84,600,600,{1091,1093,1074,1076,83,85,92,75}},
{85,600,800,{1092,1094,1075,1077,84,86,93,76}},
{86,600,1000,{1093,1095,1076,1078,85,87,94,77}},
{87,600,1200,{1094,1096,1077,1079,86,88,95,78}},
{88,600,1400,{1095,1097,1078,1080,87,89,96,79}},
{89,600,1600,{1096,1079,1081,88,90,97,80-1}},
{90,600,1800,{1097,1080,89,81-1,-1,-1,-1}},
{91,400,400,{1099,1082,1084,92,98,83-1,-1}},
{92,400,600,{1098,1100,1083,1085,91,93,99,84}},
{93,400,800,{1099,1101,1084,1086,92,94,100,85}},
{94,400,1000,{1100,1102,1085,1087,93,95,101,86}},
{95,400,1200,{1101,1103,1086,1088,94,96,102,87}},
{96,400,1400,{1102,1104,1087,1089,95,97,103,88}},
{97,400,1600,{1103,1088,1090,96,104,89-1,-1}},
{98,200,400,{1092,99,91-1,-1,-1,-1,-1}},
{99,200,600,{1091,1093,98,100,92-1,-1,-1}},
{100,200,800,{1092,1094,99,101,93-1,-1,-1}},
{101,200,1000,{1093,1095,100,102,94-1,-1,-1}},
{102,200,1200,{1094,1096,101,103,95-1,-1,-1}},
{103,200,1400,{1095,1097,102,104,96-1,-1,-1}},
{104,200,1600,{1096,103,97-1,-1,-1,-1,-1}}
};

/*
{1,2800,200,{1019,2,18-1,-1,-1,-1,-1}},
{2,2800,300,{1018,1020,1,3,19-1,-1,-1}},
{3,2800,400,{1019,1021,2,4,20-1,-1,-1}},
{4,2800,500,{1020,1022,3,5,21-1,-1,-1}},
{5,2800,600,{1021,1023,4,6,22-1,-1,-1}},
{6,2800,700,{1022,1024,5,7,23-1,-1,-1}},
{7,2800,800,{1023,1025,6,8,24-1,-1,-1}},
{8,2800,900,{1024,1026,7,9,25-1,-1,-1}},
{9,2800,1000,{1025,1027,8,10,26-1,-1,-1}},
{10,2800,1100,{1026,1028,9,11,27-1,-1,-1}},
{11,2800,1200,{1027,1029,10,12,28-1,-1,-1}},
{12,2800,1300,{1028,1030,11,13,29-1,-1,-1}},
{13,2800,1400,{1029,1031,12,14,30-1,-1,-1}},
{14,2800,1500,{1030,1032,13,15,31-1,-1,-1}},
{15,2800,1600,{1031,1033,14,16,32-1,-1,-1}},
{16,2800,1700,{1032,1034,15,17,33-1,-1,-1}},
{17,2800,1800,{1033,16,34-1,-1,-1,-1,-1}},
{18,2700,200,{1036,1002,19,35,1-1,-1,-1}},
{19,2700,300,{1035,1037,1001,1003,18,20,36,2}},
{20,2700,400,{1036,1038,1002,1004,19,21,37,3}},
{21,2700,500,{1037,1039,1003,1005,20,22,38,4}},
{22,2700,600,{1038,1040,1004,1006,21,23,39,5}},
{23,2700,700,{1039,1041,1005,1007,22,24,40,6}},
{24,2700,800,{1040,1042,1006,1008,23,25,41,7}},
{25,2700,900,{1041,1043,1007,1009,24,26,42,8}},
{26,2700,1000,{1042,1044,1008,1010,25,27,43,9}},
{27,2700,1100,{1043,1045,1009,1011,26,28,44,10}},
{28,2700,1200,{1044,1046,1010,1012,27,29,45,11}},
{29,2700,1300,{1045,1047,1011,1013,28,30,46,12}},
{30,2700,1400,{1046,1048,1012,1014,29,31,47,13}},
{31,2700,1500,{1047,1049,1013,1015,30,32,48,14}},
{32,2700,1600,{1048,1050,1014,1016,31,33,49,15}},
{33,2700,1700,{1049,1051,1015,1017,32,34,50,16}},
{34,2700,1800,{1050,1016,33,51,17-1,-1,-1}},
{35,2600,200,{1053,1019,36,52,18-1,-1,-1}},
{36,2600,300,{1052,1054,1018,1020,35,37,53,19}},
{37,2600,400,{1053,1055,1019,1021,36,38,54,20}},
{38,2600,500,{1054,1056,1020,1022,37,39,55,21}},
{39,2600,600,{1055,1057,1021,1023,38,40,56,22}},
{40,2600,700,{1056,1058,1022,1024,39,41,57,23}},
{41,2600,800,{1057,1059,1023,1025,40,42,58,24}},
{42,2600,900,{1058,1060,1024,1026,41,43,59,25}},
{43,2600,1000,{1059,1061,1025,1027,42,44,60,26}},
{44,2600,1100,{1060,1062,1026,1028,43,45,61,27}},
{45,2600,1200,{1061,1063,1027,1029,44,46,62,28}},
{46,2600,1300,{1062,1064,1028,1030,45,47,63,29}},
{47,2600,1400,{1063,1065,1029,1031,46,48,64,30}},
{48,2600,1500,{1064,1066,1030,1032,47,49,65,31}},
{49,2600,1600,{1065,1067,1031,1033,48,50,66,32}},
{50,2600,1700,{1066,1068,1032,1034,49,51,67,33}},
{51,2600,1800,{1067,1033,50,68,34-1,-1,-1}},
{52,2500,200,{1070,1036,53,69,35-1,-1,-1}},
{53,2500,300,{1069,1071,1035,1037,52,54,70,36}},
{54,2500,400,{1070,1072,1036,1038,53,55,71,37}},
{55,2500,500,{1071,1073,1037,1039,54,56,72,38}},
{56,2500,600,{1072,1074,1038,1040,55,57,73,39}},
{57,2500,700,{1073,1075,1039,1041,56,58,74,40}},
{58,2500,800,{1074,1076,1040,1042,57,59,75,41}},
{59,2500,900,{1075,1077,1041,1043,58,60,76,42}},
{60,2500,1000,{1076,1078,1042,1044,59,61,77,43}},
{61,2500,1100,{1077,1079,1043,1045,60,62,78,44}},
{62,2500,1200,{1078,1080,1044,1046,61,63,79,45}},
{63,2500,1300,{1079,1081,1045,1047,62,64,80,46}},
{64,2500,1400,{1080,1082,1046,1048,63,65,81,47}},
{65,2500,1500,{1081,1083,1047,1049,64,66,82,48}},
{66,2500,1600,{1082,1084,1048,1050,65,67,83,49}},
{67,2500,1700,{1083,1085,1049,1051,66,68,84,50}},
{68,2500,1800,{1084,1050,67,85,51-1,-1,-1}},
{69,2400,200,{1087,1053,70,86,52-1,-1,-1}},
{70,2400,300,{1086,1088,1052,1054,69,71,87,53}},
{71,2400,400,{1087,1089,1053,1055,70,72,88,54}},
{72,2400,500,{1088,1090,1054,1056,71,73,89,55}},
{73,2400,600,{1089,1091,1055,1057,72,74,90,56}},
{74,2400,700,{1090,1092,1056,1058,73,75,91,57}},
{75,2400,800,{1091,1093,1057,1059,74,76,92,58}},
{76,2400,900,{1092,1094,1058,1060,75,77,93,59}},
{77,2400,1000,{1093,1095,1059,1061,76,78,94,60}},
{78,2400,1100,{1094,1096,1060,1062,77,79,95,61}},
{79,2400,1200,{1095,1097,1061,1063,78,80,96,62}},
{80,2400,1300,{1096,1098,1062,1064,79,81,97,63}},
{81,2400,1400,{1097,1099,1063,1065,80,82,98,64}},
{82,2400,1500,{1098,1100,1064,1066,81,83,99,65}},
{83,2400,1600,{1099,1101,1065,1067,82,84,100,66}},
{84,2400,1700,{1100,1102,1066,1068,83,85,101,67}},
{85,2400,1800,{1101,1067,84,102,68-1,-1,-1}},
{86,2300,200,{1104,1070,87,103,69-1,-1,-1}},
{87,2300,300,{1103,1105,1069,1071,86,88,104,70}},
{88,2300,400,{1104,1106,1070,1072,87,89,105,71}},
{89,2300,500,{1105,1107,1071,1073,88,90,106,72}},
{90,2300,600,{1106,1108,1072,1074,89,91,107,73}},
{91,2300,700,{1107,1109,1073,1075,90,92,108,74}},
{92,2300,800,{1108,1110,1074,1076,91,93,109,75}},
{93,2300,900,{1109,1111,1075,1077,92,94,110,76}},
{94,2300,1000,{1110,1112,1076,1078,93,95,111,77}},
{95,2300,1100,{1111,1113,1077,1079,94,96,112,78}},
{96,2300,1200,{1112,1114,1078,1080,95,97,113,79}},
{97,2300,1300,{1113,1115,1079,1081,96,98,114,80}},
{98,2300,1400,{1114,1116,1080,1082,97,99,115,81}},
{99,2300,1500,{1115,1117,1081,1083,98,100,116,82}},
{100,2300,1600,{1116,1118,1082,1084,99,101,117,83}},
{101,2300,1700,{1117,1119,1083,1085,100,102,118,84}},
{102,2300,1800,{1118,1084,101,119,85-1,-1,-1}},
{103,2200,200,{1121,1087,104,120,86-1,-1,-1}},
{104,2200,300,{1120,1122,1086,1088,103,105,121,87}},
{105,2200,400,{1121,1123,1087,1089,104,106,122,88}},
{106,2200,500,{1122,1124,1088,1090,105,107,123,89}},
{107,2200,600,{1123,1125,1089,1091,106,108,124,90}},
{108,2200,700,{1124,1126,1090,1092,107,109,125,91}},
{109,2200,800,{1125,1127,1091,1093,108,110,126,92}},
{110,2200,900,{1126,1128,1092,1094,109,111,127,93}},
{111,2200,1000,{1127,1129,1093,1095,110,112,128,94}},
{112,2200,1100,{1128,1130,1094,1096,111,113,129,95}},
{113,2200,1200,{1129,1131,1095,1097,112,114,130,96}},
{114,2200,1300,{1130,1132,1096,1098,113,115,131,97}},
{115,2200,1400,{1131,1133,1097,1099,114,116,132,98}},
{116,2200,1500,{1132,1134,1098,1100,115,117,133,99}},
{117,2200,1600,{1133,1135,1099,1101,116,118,134,100}},
{118,2200,1700,{1134,1100,1102,117,119,135,101-1}},
{119,2200,1800,{1135,1101,118,102-1,-1,-1,-1}},
{120,2100,200,{1137,1104,121,136,103-1,-1,-1}},
{121,2100,300,{1136,1138,1103,1105,120,122,137,104}},
{122,2100,400,{1137,1139,1104,1106,121,123,138,105}},
{123,2100,500,{1138,1140,1105,1107,122,124,139,106}},
{124,2100,600,{1139,1141,1106,1108,123,125,140,107}},
{125,2100,700,{1140,1142,1107,1109,124,126,141,108}},
{126,2100,800,{1141,1143,1108,1110,125,127,142,109}},
{127,2100,900,{1142,1144,1109,1111,126,128,143,110}},
{128,2100,1000,{1143,1145,1110,1112,127,129,144,111}},
{129,2100,1100,{1144,1146,1111,1113,128,130,145,112}},
{130,2100,1200,{1145,1147,1112,1114,129,131,146,113}},
{131,2100,1300,{1146,1148,1113,1115,130,132,147,114}},
{132,2100,1400,{1147,1149,1114,1116,131,133,148,115}},
{133,2100,1500,{1148,1115,1117,132,134,149,116-1}},
{134,2100,1600,{1149,1116,1118,133,135,117-1,-1}},
{135,2100,1700,{1117,1119,134,118-1,-1,-1,-1}},
{136,2000,200,{1151,1121,137,150,120-1,-1,-1}},
{137,2000,300,{1150,1152,1120,1122,136,138,151,121}},
{138,2000,400,{1151,1153,1121,1123,137,139,152,122}},
{139,2000,500,{1152,1154,1122,1124,138,140,153,123}},
{140,2000,600,{1153,1155,1123,1125,139,141,154,124}},
{141,2000,700,{1154,1156,1124,1126,140,142,155,125}},
{142,2000,800,{1155,1157,1125,1127,141,143,156,126}},
{143,2000,900,{1156,1158,1126,1128,142,144,157,127}},
{144,2000,1000,{1157,1159,1127,1129,143,145,158,128}},
{145,2000,1100,{1158,1160,1128,1130,144,146,159,129}},
{146,2000,1200,{1159,1161,1129,1131,145,147,160,130}},
{147,2000,1300,{1160,1162,1130,1132,146,148,161,131}},
{148,2000,1400,{1161,1131,1133,147,149,162,132-1}},
{149,2000,1500,{1162,1132,1134,148,133-1,-1,-1}},
{150,1900,200,{1164,1137,151,163,136-1,-1,-1}},
{151,1900,300,{1163,1165,1136,1138,150,152,164,137}},
{152,1900,400,{1164,1166,1137,1139,151,153,165,138}},
{153,1900,500,{1165,1167,1138,1140,152,154,166,139}},
{154,1900,600,{1166,1168,1139,1141,153,155,167,140}},
{155,1900,700,{1167,1169,1140,1142,154,156,168,141}},
{156,1900,800,{1168,1170,1141,1143,155,157,169,142}},
{157,1900,900,{1169,1171,1142,1144,156,158,170,143}},
{158,1900,1000,{1170,1172,1143,1145,157,159,171,144}},
{159,1900,1100,{1171,1173,1144,1146,158,160,172,145}},
{160,1900,1200,{1172,1174,1145,1147,159,161,173,146}},
{161,1900,1300,{1173,1175,1146,1148,160,162,174,147}},
{162,1900,1400,{1174,1147,1149,161,175,148-1,-1}},
{163,1800,200,{1177,1151,164,176,150-1,-1,-1}},
{164,1800,300,{1176,1178,1150,1152,163,165,177,151}},
{165,1800,400,{1177,1179,1151,1153,164,166,178,152}},
{166,1800,500,{1178,1180,1152,1154,165,167,179,153}},
{167,1800,600,{1179,1181,1153,1155,166,168,180,154}},
{168,1800,700,{1180,1182,1154,1156,167,169,181,155}},
{169,1800,800,{1181,1183,1155,1157,168,170,182,156}},
{170,1800,900,{1182,1184,1156,1158,169,171,183,157}},
{171,1800,1000,{1183,1185,1157,1159,170,172,184,158}},
{172,1800,1100,{1184,1186,1158,1160,171,173,185,159}},
{173,1800,1200,{1185,1187,1159,1161,172,174,186,160}},
{174,1800,1300,{1186,1160,1162,173,175,187,161-1}},
{175,1800,1400,{1187,1161,174,162-1,-1,-1,-1}},
{176,1700,200,{1189,1164,177,188,163-1,-1,-1}},
{177,1700,300,{1188,1190,1163,1165,176,178,189,164}},
{178,1700,400,{1189,1191,1164,1166,177,179,190,165}},
{179,1700,500,{1190,1192,1165,1167,178,180,191,166}},
{180,1700,600,{1191,1193,1166,1168,179,181,192,167}},
{181,1700,700,{1192,1194,1167,1169,180,182,193,168}},
{182,1700,800,{1193,1195,1168,1170,181,183,194,169}},
{183,1700,900,{1194,1196,1169,1171,182,184,195,170}},
{184,1700,1000,{1195,1197,1170,1172,183,185,196,171}},
{185,1700,1100,{1196,1198,1171,1173,184,186,197,172}},
{186,1700,1200,{1197,1199,1172,1174,185,187,198,173}},
{187,1700,1300,{1198,1173,1175,186,199,174-1,-1}},
{188,1600,200,{1201,1177,189,200,176-1,-1,-1}},
{189,1600,300,{1200,1202,1176,1178,188,190,201,177}},
{190,1600,400,{1201,1203,1177,1179,189,191,202,178}},
{191,1600,500,{1202,1204,1178,1180,190,192,203,179}},
{192,1600,600,{1203,1205,1179,1181,191,193,204,180}},
{193,1600,700,{1204,1206,1180,1182,192,194,205,181}},
{194,1600,800,{1205,1207,1181,1183,193,195,206,182}},
{195,1600,900,{1206,1208,1182,1184,194,196,207,183}},
{196,1600,1000,{1207,1209,1183,1185,195,197,208,184}},
{197,1600,1100,{1208,1210,1184,1186,196,198,209,185}},
{198,1600,1200,{1209,1211,1185,1187,197,199,210,186}},
{199,1600,1300,{1210,1186,198,211,187-1,-1,-1}},
{200,1500,200,{1213,1189,201,212,188-1,-1,-1}},
{201,1500,300,{1212,1214,1188,1190,200,202,213,189}},
{202,1500,400,{1213,1215,1189,1191,201,203,214,190}},
{203,1500,500,{1214,1216,1190,1192,202,204,215,191}},
{204,1500,600,{1215,1217,1191,1193,203,205,216,192}},
{205,1500,700,{1216,1218,1192,1194,204,206,217,193}},
{206,1500,800,{1217,1219,1193,1195,205,207,218,194}},
{207,1500,900,{1218,1220,1194,1196,206,208,219,195}},
{208,1500,1000,{1219,1221,1195,1197,207,209,220,196}},
{209,1500,1100,{1220,1222,1196,1198,208,210,221,197}},
{210,1500,1200,{1221,1223,1197,1199,209,211,222,198}},
{211,1500,1300,{1222,1198,210,223,199-1,-1,-1}},
{212,1400,200,{1225,1201,213,224,200-1,-1,-1}},
{213,1400,300,{1224,1226,1200,1202,212,214,225,201}},
{214,1400,400,{1225,1227,1201,1203,213,215,226,202}},
{215,1400,500,{1226,1228,1202,1204,214,216,227,203}},
{216,1400,600,{1227,1229,1203,1205,215,217,228,204}},
{217,1400,700,{1228,1230,1204,1206,216,218,229,205}},
{218,1400,800,{1229,1231,1205,1207,217,219,230,206}},
{219,1400,900,{1230,1232,1206,1208,218,220,231,207}},
{220,1400,1000,{1231,1233,1207,1209,219,221,232,208}},
{221,1400,1100,{1232,1234,1208,1210,220,222,233,209}},
{222,1400,1200,{1233,1235,1209,1211,221,223,234,210}},
{223,1400,1300,{1234,1210,222,235,211-1,-1,-1}},
{224,1300,200,{1237,1213,225,236,212-1,-1,-1}},
{225,1300,300,{1236,1238,1212,1214,224,226,237,213}},
{226,1300,400,{1237,1239,1213,1215,225,227,238,214}},
{227,1300,500,{1238,1240,1214,1216,226,228,239,215}},
{228,1300,600,{1239,1241,1215,1217,227,229,240,216}},
{229,1300,700,{1240,1242,1216,1218,228,230,241,217}},
{230,1300,800,{1241,1243,1217,1219,229,231,242,218}},
{231,1300,900,{1242,1244,1218,1220,230,232,243,219}},
{232,1300,1000,{1243,1245,1219,1221,231,233,244,220}},
{233,1300,1100,{1244,1246,1220,1222,232,234,245,221}},
{234,1300,1200,{1245,1247,1221,1223,233,235,246,222}},
{235,1300,1300,{1246,1248,1222,234,247,223-1,-1}},
{236,1200,200,{1250,1225,237,249,224-1,-1,-1}},
{237,1200,300,{1249,1251,1224,1226,236,238,250,225}},
{238,1200,400,{1250,1252,1225,1227,237,239,251,226}},
{239,1200,500,{1251,1253,1226,1228,238,240,252,227}},
{240,1200,600,{1252,1254,1227,1229,239,241,253,228}},
{241,1200,700,{1253,1255,1228,1230,240,242,254,229}},
{242,1200,800,{1254,1256,1229,1231,241,243,255,230}},
{243,1200,900,{1255,1257,1230,1232,242,244,256,231}},
{244,1200,1000,{1256,1258,1231,1233,243,245,257,232}},
{245,1200,1100,{1257,1259,1232,1234,244,246,258,233}},
{246,1200,1200,{1258,1260,1233,1235,245,247,259,234}},
{247,1200,1300,{1259,1261,1234,246,248,260,235-1}},
{248,1200,1400,{1260,1235,247,261-1,-1,-1,-1}},
{249,1100,200,{1263,1237,250,262,236-1,-1,-1}},
{250,1100,300,{1262,1264,1236,1238,249,251,263,237}},
{251,1100,400,{1263,1265,1237,1239,250,252,264,238}},
{252,1100,500,{1264,1266,1238,1240,251,253,265,239}},
{253,1100,600,{1265,1267,1239,1241,252,254,266,240}},
{254,1100,700,{1266,1268,1240,1242,253,255,267,241}},
{255,1100,800,{1267,1269,1241,1243,254,256,268,242}},
{256,1100,900,{1268,1270,1242,1244,255,257,269,243}},
{257,1100,1000,{1269,1271,1243,1245,256,258,270,244}},
{258,1100,1100,{1270,1272,1244,1246,257,259,271,245}},
{259,1100,1200,{1271,1273,1245,1247,258,260,272,246}},
{260,1100,1300,{1272,1274,1246,1248,259,261,273,247}},
{261,1100,1400,{1273,1275,1247,260,274,248-1,-1}},
{262,1000,200,{1277,1250,263,276,249-1,-1,-1}},
{263,1000,300,{1276,1278,1249,1251,262,264,277,250}},
{264,1000,400,{1277,1279,1250,1252,263,265,278,251}},
{265,1000,500,{1278,1280,1251,1253,264,266,279,252}},
{266,1000,600,{1279,1281,1252,1254,265,267,280,253}},
{267,1000,700,{1280,1282,1253,1255,266,268,281,254}},
{268,1000,800,{1281,1283,1254,1256,267,269,282,255}},
{269,1000,900,{1282,1284,1255,1257,268,270,283,256}},
{270,1000,1000,{1283,1285,1256,1258,269,271,284,257}},
{271,1000,1100,{1284,1286,1257,1259,270,272,285,258}},
{272,1000,1200,{1285,1287,1258,1260,271,273,286,259}},
{273,1000,1300,{1286,1288,1259,1261,272,274,287,260}},
{274,1000,1400,{1287,1289,1260,273,275,288,261-1}},
{275,1000,1500,{1288,1290,1261,274,289-1,-1,-1}},
{276,900,200,{1293,1263,277,292,262-1,-1,-1}},
{277,900,300,{1292,1294,1262,1264,276,278,293,263}},
{278,900,400,{1293,1295,1263,1265,277,279,294,264}},
{279,900,500,{1294,1296,1264,1266,278,280,295,265}},
{280,900,600,{1295,1297,1265,1267,279,281,296,266}},
{281,900,700,{1296,1298,1266,1268,280,282,297,267}},
{282,900,800,{1297,1299,1267,1269,281,283,298,268}},
{283,900,900,{1298,1300,1268,1270,282,284,299,269}},
{284,900,1000,{1299,1301,1269,1271,283,285,300,270}},
{285,900,1100,{1300,1302,1270,1272,284,286,301,271}},
{286,900,1200,{1301,1303,1271,1273,285,287,302,272}},
{287,900,1300,{1302,1304,1272,1274,286,288,303,273}},
{288,900,1400,{1303,1305,1273,1275,287,289,304,274}},
{289,900,1500,{1304,1306,1274,288,290,305,275-1}},
{290,900,1600,{1305,1307,1275,289,291,306-1,-1}},
{291,900,1700,{1306,1308,290,307-1,-1,-1,-1}},
{292,800,200,{1310,1277,293,309,276-1,-1,-1}},
{293,800,300,{1309,1311,1276,1278,292,294,310,277}},
{294,800,400,{1310,1312,1277,1279,293,295,311,278}},
{295,800,500,{1311,1313,1278,1280,294,296,312,279}},
{296,800,600,{1312,1314,1279,1281,295,297,313,280}},
{297,800,700,{1313,1315,1280,1282,296,298,314,281}},
{298,800,800,{1314,1316,1281,1283,297,299,315,282}},
{299,800,900,{1315,1317,1282,1284,298,300,316,283}},
{300,800,1000,{1316,1318,1283,1285,299,301,317,284}},
{301,800,1100,{1317,1319,1284,1286,300,302,318,285}},
{302,800,1200,{1318,1320,1285,1287,301,303,319,286}},
{303,800,1300,{1319,1321,1286,1288,302,304,320,287}},
{304,800,1400,{1320,1322,1287,1289,303,305,321,288}},
{305,800,1500,{1321,1323,1288,1290,304,306,322,289}},
{306,800,1600,{1322,1324,1289,1291,305,307,323,290}},
{307,800,1700,{1323,1325,1290,306,308,324,291-1}},
{308,800,1800,{1324,1291,307,325-1,-1,-1,-1}},
{309,700,200,{1327,1293,310,326,292-1,-1,-1}},
{310,700,300,{1326,1328,1292,1294,309,311,327,293}},
{311,700,400,{1327,1329,1293,1295,310,312,328,294}},
{312,700,500,{1328,1330,1294,1296,311,313,329,295}},
{313,700,600,{1329,1331,1295,1297,312,314,330,296}},
{314,700,700,{1330,1332,1296,1298,313,315,331,297}},
{315,700,800,{1331,1333,1297,1299,314,316,332,298}},
{316,700,900,{1332,1334,1298,1300,315,317,333,299}},
{317,700,1000,{1333,1335,1299,1301,316,318,334,300}},
{318,700,1100,{1334,1336,1300,1302,317,319,335,301}},
{319,700,1200,{1335,1337,1301,1303,318,320,336,302}},
{320,700,1300,{1336,1338,1302,1304,319,321,337,303}},
{321,700,1400,{1337,1339,1303,1305,320,322,338,304}},
{322,700,1500,{1338,1340,1304,1306,321,323,339,305}},
{323,700,1600,{1339,1341,1305,1307,322,324,340,306}},
{324,700,1700,{1340,1342,1306,1308,323,325,341,307}},
{325,700,1800,{1341,1307,324,342,308-1,-1,-1}},
{326,600,200,{1344,1310,327,343,309-1,-1,-1}},
{327,600,300,{1343,1345,1309,1311,326,328,344,310}},
{328,600,400,{1344,1346,1310,1312,327,329,345,311}},
{329,600,500,{1345,1347,1311,1313,328,330,346,312}},
{330,600,600,{1346,1348,1312,1314,329,331,347,313}},
{331,600,700,{1347,1349,1313,1315,330,332,348,314}},
{332,600,800,{1348,1350,1314,1316,331,333,349,315}},
{333,600,900,{1349,1351,1315,1317,332,334,350,316}},
{334,600,1000,{1350,1352,1316,1318,333,335,351,317}},
{335,600,1100,{1351,1353,1317,1319,334,336,352,318}},
{336,600,1200,{1352,1354,1318,1320,335,337,353,319}},
{337,600,1300,{1353,1355,1319,1321,336,338,354,320}},
{338,600,1400,{1354,1356,1320,1322,337,339,355,321}},
{339,600,1500,{1355,1357,1321,1323,338,340,356,322}},
{340,600,1600,{1356,1358,1322,1324,339,341,357,323}},
{341,600,1700,{1357,1359,1323,1325,340,342,358,324}},
{342,600,1800,{1358,1324,341,359,325-1,-1,-1}},
{343,500,200,{1361,1327,344,360,326-1,-1,-1}},
{344,500,300,{1360,1362,1326,1328,343,345,361,327}},
{345,500,400,{1361,1363,1327,1329,344,346,362,328}},
{346,500,500,{1362,1364,1328,1330,345,347,363,329}},
{347,500,600,{1363,1365,1329,1331,346,348,364,330}},
{348,500,700,{1364,1366,1330,1332,347,349,365,331}},
{349,500,800,{1365,1367,1331,1333,348,350,366,332}},
{350,500,900,{1366,1368,1332,1334,349,351,367,333}},
{351,500,1000,{1367,1369,1333,1335,350,352,368,334}},
{352,500,1100,{1368,1370,1334,1336,351,353,369,335}},
{353,500,1200,{1369,1371,1335,1337,352,354,370,336}},
{354,500,1300,{1370,1372,1336,1338,353,355,371,337}},
{355,500,1400,{1371,1373,1337,1339,354,356,372,338}},
{356,500,1500,{1372,1374,1338,1340,355,357,373,339}},
{357,500,1600,{1373,1375,1339,1341,356,358,374,340}},
{358,500,1700,{1374,1376,1340,1342,357,359,375,341}},
{359,500,1800,{1375,1341,358,376,342-1,-1,-1}},
{360,400,200,{1378,1344,361,377,343-1,-1,-1}},
{361,400,300,{1377,1379,1343,1345,360,362,378,344}},
{362,400,400,{1378,1380,1344,1346,361,363,379,345}},
{363,400,500,{1379,1381,1345,1347,362,364,380,346}},
{364,400,600,{1380,1382,1346,1348,363,365,381,347}},
{365,400,700,{1381,1383,1347,1349,364,366,382,348}},
{366,400,800,{1382,1384,1348,1350,365,367,383,349}},
{367,400,900,{1383,1385,1349,1351,366,368,384,350}},
{368,400,1000,{1384,1386,1350,1352,367,369,385,351}},
{369,400,1100,{1385,1387,1351,1353,368,370,386,352}},
{370,400,1200,{1386,1388,1352,1354,369,371,387,353}},
{371,400,1300,{1387,1389,1353,1355,370,372,388,354}},
{372,400,1400,{1388,1390,1354,1356,371,373,389,355}},
{373,400,1500,{1389,1391,1355,1357,372,374,390,356}},
{374,400,1600,{1390,1392,1356,1358,373,375,391,357}},
{375,400,1700,{1391,1393,1357,1359,374,376,392,358}},
{376,400,1800,{1392,1358,375,393,359-1,-1,-1}},
{377,300,200,{1395,1361,378,394,360-1,-1,-1}},
{378,300,300,{1394,1396,1360,1362,377,379,395,361}},
{379,300,400,{1395,1397,1361,1363,378,380,396,362}},
{380,300,500,{1396,1398,1362,1364,379,381,397,363}},
{381,300,600,{1397,1399,1363,1365,380,382,398,364}},
{382,300,700,{1398,1400,1364,1366,381,383,399,365}},
{383,300,800,{1399,1401,1365,1367,382,384,400,366}},
{384,300,900,{1400,1402,1366,1368,383,385,401,367}},
{385,300,1000,{1401,1403,1367,1369,384,386,402,368}},
{386,300,1100,{1402,1404,1368,1370,385,387,403,369}},
{387,300,1200,{1403,1405,1369,1371,386,388,404,370}},
{388,300,1300,{1404,1406,1370,1372,387,389,405,371}},
{389,300,1400,{1405,1407,1371,1373,388,390,406,372}},
{390,300,1500,{1406,1408,1372,1374,389,391,407,373}},
{391,300,1600,{1407,1409,1373,1375,390,392,408,374}},
{392,300,1700,{1408,1410,1374,1376,391,393,409,375}},
{393,300,1800,{1409,1375,392,410,376-1,-1,-1}},
{394,200,200,{1378,395,377-1,-1,-1,-1,-1}},
{395,200,300,{1377,1379,394,396,378-1,-1,-1}},
{396,200,400,{1378,1380,395,397,379-1,-1,-1}},
{397,200,500,{1379,1381,396,398,380-1,-1,-1}},
{398,200,600,{1380,1382,397,399,381-1,-1,-1}},
{399,200,700,{1381,1383,398,400,382-1,-1,-1}},
{400,200,800,{1382,1384,399,401,383-1,-1,-1}},
{401,200,900,{1383,1385,400,402,384-1,-1,-1}},
{402,200,1000,{1384,1386,401,403,385-1,-1,-1}},
{403,200,1100,{1385,1387,402,404,386-1,-1,-1}},
{404,200,1200,{1386,1388,403,405,387-1,-1,-1}},
{405,200,1300,{1387,1389,404,406,388-1,-1,-1}},
{406,200,1400,{1388,1390,405,407,389-1,-1,-1}},
{407,200,1500,{1389,1391,406,408,390-1,-1,-1}},
{408,200,1600,{1390,1392,407,409,391-1,-1,-1}},
{409,200,1700,{1391,1393,408,410,392-1,-1,-1}},
{410,200,1800,{1392,409,393-1,-1,-1,-1,-1}}
};*/

char listEnable[NB_POINTS] = {3};

PointROM * loadPointList(void)
{
    return tabPoint;
}


