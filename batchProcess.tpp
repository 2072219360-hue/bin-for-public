template <>
int batchProcess::getJson(int index, std::string json_key) {
	std::string caseI = "case_" + std::to_string(index);
	return jsonData[caseI][json_key].asInt();
}
template <>
bool batchProcess::getJson(int index, std::string json_key) {
	std::string caseI = "case_" + std::to_string(index);
	return jsonData[caseI][json_key].asBool();
}
template <>
float batchProcess::getJson(int index, std::string json_key) {
	std::string caseI = "case_" + std::to_string(index);
	return jsonData[caseI][json_key].asFloat();
}