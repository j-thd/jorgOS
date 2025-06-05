/* Small testing library that can run on the host or on the device.
When running on the device, functions to print characters must be provided.
*/

void JTEST_run_test(void);
int run_test_on_target(void);
int run_test_on_host(void);