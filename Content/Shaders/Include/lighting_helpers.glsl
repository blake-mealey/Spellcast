float lighting_get_attenuation(float power, float distance) {
	return 1.0 / (1.0 * (1.0/power) * (distance*distance));
}

float lighting_get_attenuation_safe(float power, float distance) {
	if (power == 0) return 1;
	return 1.0 / (1.0 * (1.0/power) * (distance*distance));
}