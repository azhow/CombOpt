# Sets and parameters

set PLANES;

param E{i in PLANES}; # minimal landing time
param L{i in PLANES}; # maximal landing time 
param T{i in PLANES}; # ideal landing time
param g{i in PLANES}; # cost per earliness
param h{i in PLANES}; # cost per lateness

param S{i in PLANES, j in PLANES}; # wait time between i and j landings

# Variable definitions
var x{i in PLANES} >= 0; # time in which i PLANE has landed

# Auxiliar variables because mathprog cant have an expression as an argument in max function
var earliness{i in PLANES} >= 0;  
var lateness{i in PLANES} >= 0; 

var delta{i in PLANES, j in PLANES} binary; # =1 if plane i lands before plane j
					    # =0 otherwise

# Objective function
minimize cost: sum{i in PLANES} ((g[i] * earliness[i]) + (h[i] * lateness[i]));

# Constrains

# funcion in mathprog can't have an expression as argument:
s.t. T_MINUS_X {i in PLANES}: earliness[i] >= T[i] - x[i];
s.t. X_MINUS_T {i in PLANES}: lateness[i] >= x[i] - T[i];

# all planes must land (???)
s.t. ALL_PLANES {i in PLANES, j in PLANES: j > i}: delta[i,j] + delta[j,i] = 1;

# time windows must be obeyed (???)
s.t. TIME_WINDOWS {i in PLANES, j in PLANES: j != i}: x[j] >= x[i] + (S[i,j] * delta[i,j]) - ((L[i] - E[j]) * delta[j,i]);

# wait time between landings must be obeyed (???)
s.t. LANDING_WAIT {i in PLANES}: E[i] <= x[i] <= L[i];

end;
