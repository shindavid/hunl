
constexpr size_t power_up_helper(size_t t, int p) {
  return t ? power_up_helper(t/2, p+1) : (1<<p);
}

constexpr size_t power_up(size_t t) {
  return power_up_helper(t, 0);
}

