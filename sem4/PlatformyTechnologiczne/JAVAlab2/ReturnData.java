public class ReturnData {
    int job_id;
    int precision;
    double pi_calculated;
    double error;
    int n_iterations;

    public ReturnData(int job_id, int precision, double pi_calculated, double error, int n_iterations){
        this.error = error;
        this.job_id = job_id;
        this.n_iterations = n_iterations;
        this.pi_calculated = pi_calculated;
        this.precision = precision;
    }

    public String toString(){
        return "ID: " + this.job_id + " precision: " + this.precision + " calculated pi: " + this.pi_calculated +" error : " + this.error + " number of iterations: " + this.n_iterations;
    }
}
