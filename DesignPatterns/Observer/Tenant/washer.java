class Washer {
          private List<Tenant> tenants;
          public void addTenant(Tenant t) {
                      tenants.add(t);
            }
         public void start() {
                for (Tenant t : tenants) {
                       t.onStart();
                  }
       }
    public void finish() {                                                                                                                     
          for (Tenant t : tenants) {
                    t.onFinish();
          }
      }
 }
