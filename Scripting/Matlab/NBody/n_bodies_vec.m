function   n_bodies
    n=50000
    clear pi;
    SOLAR_MASS=4*pi*pi;
    DAYS_PER_YEAR=365.24;
a


    solar_bodies(1).x    = [ 0., 0., 0.];
    solar_bodies(1).v    = [ 0., 0., 0.];
    solar_bodies(1).mass = SOLAR_MASS;

    solar_bodies(2).x    = [ 4.84143144246472090e+00, -1.16032004402742839e+00, -1.03622044471123109e-01];
    solar_bodies(2).v    = [ 1.66007664274403694e-03 * DAYS_PER_YEAR,7.69901118419740425e-03 * DAYS_PER_YEAR, -6.90460016972063023e-05 * DAYS_PER_YEAR];
    solar_bodies(2).mass =  9.54791938424326609e-04 * SOLAR_MASS;

    solar_bodies(3).x    = [ 8.34336671824457987e+00, 4.12479856412430479e+00, -4.03523417114321381e-01];
    solar_bodies(3).v    = [ -2.76742510726862411e-03 * DAYS_PER_YEAR, 4.99852801234917238e-03 * DAYS_PER_YEAR, 2.30417297573763929e-05 * DAYS_PER_YEAR];
    solar_bodies(3).mass = 2.85885980666130812e-04 * SOLAR_MASS;

    solar_bodies(4).x    = [1.28943695621391310e+01, -1.51111514016986312e+01, -2.23307578892655734e-01];
    solar_bodies(4).v    = [2.96460137564761618e-03 * DAYS_PER_YEAR, 2.37847173959480950e-03 * DAYS_PER_YEAR, -2.96589568540237556e-05 * DAYS_PER_YEAR];
    solar_bodies(4).mass = 4.36624404335156298e-05 * SOLAR_MASS;

    solar_bodies(5).x    =  [ 1.53796971148509165e+01, -2.59193146099879641e+01, 1.79258772950371181e-01 ];
    solar_bodies(5).v    =  [ 2.68067772490389322e-03 * DAYS_PER_YEAR, 1.62824170038242295e-03 * DAYS_PER_YEAR, -9.51592254519715870e-05 * DAYS_PER_YEAR ];
    solar_bodies(5).mass = 5.15138902046611451e-05 * SOLAR_MASS;


    BODIES_SIZE = length(solar_bodies);
    offset_momentum(solar_bodies, BODIES_SIZE, SOLAR_MASS);
    E = bodies_energy(solar_bodies, BODIES_SIZE)
    
    for ii=1:n
        bodies_advance(solar_bodies, BODIES_SIZE, 0.01);
    end
    
    E = bodies_energy(solar_bodies, BODIES_SIZE)
end

function offset_momentum( bodies, nbodies, SOLAR_MASS )
    for ii = 1:5
      bodies(1).v = bodies(1).v  - bodies(ii).v ...
                                * bodies(ii).mass / SOLAR_MASS;
    end

end

function E = bodies_energy(bodies, nbodies) 
   E = 0.0;

   for ii = 1:nbodies
      E = E + bodies(ii).mass * ( bodies(ii).v(1) * bodies(ii).v(1) + ...
            bodies(ii).v(2) * bodies(ii).v(2) + bodies(ii).v(3) * ...
            bodies(ii).v(3) ) / 2.;
      for jj = ii + 1:nbodies 
         dx = bodies(ii).x - bodies(jj).x;
         distance = sqrt(dx(1) * dx(1) + dx(2) * dx(2) + dx(3) * dx(3));
         E = E - (bodies(ii).mass * bodies(jj).mass) / distance;
      end
   end
end

function bodies_advance(bodies, nbodies, dt)
  
  %dSquared = 0.;
  %distance = 0.;
  %mag = 0.;

  for ii=1:nbodies
    for jj=ii+1:nbodies

      d =  bodies(ii).x - bodies(jj).x;
      
      dSquared = d(1) * d(1) + d(2) * d(2) + d(3) * d(3);
      distance = sqrt(dSquared);
      mag = dt / (dSquared * distance);

      bodies(ii).v = bodies(ii).v - d * bodies(jj).mass * mag;

      bodies(jj).v = bodies(jj).v + d * bodies(ii).mass * mag;
    end
  end
  
  for ii = 1:nbodies
    bodies(ii).x = bodies(ii).x + dt * bodies(ii).v;
  end
end
