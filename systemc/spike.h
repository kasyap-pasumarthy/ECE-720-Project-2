// See LICENSE for license details.

#include <tlm.h>

class spike 
  : public sc_core::sc_module
  , virtual public tlm::tlm_bw_transport_if<>
{
public:


  SC_HAS_PROCESS(spike);
  spike(sc_core::sc_module_name name, int argc_arg, char **argv_arg, 
        bool xact_dump=true);

  tlm::tlm_initiator_socket<64> master;

  void xact();
  
  void main();

  private:

  int argc;
  char **argv;
  bool xact_dump_f;
  int return_code;

/// Not Implemented for this example but required by the initiator socket
  void invalidate_direct_mem_ptr
    (sc_dt::uint64 start_range, sc_dt::uint64 end_range);
  tlm::tlm_sync_enum nb_transport_bw (tlm::tlm_generic_payload  &gp,
    tlm::tlm_phase &phase, sc_core::sc_time &delay);

};

