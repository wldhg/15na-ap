// Reference code by:
//   (c) 2008-2011 Daniel Halperin <dhalperi@cs.washington.edu>
// Widh Jio
// math.csi.cpp

#include "csi.hpp"

using std::array;
using std::complex;
using std::vector;
using std::tuple;
using std::get;
using std::forward_as_tuple;

tuple<CSISingleComplex&, array<double, 3>&> getSingleRawCSI(csi::BBPacket *packet)
{
  // Initialize return value
  CSISingleComplex comp;
  for (unsigned char i = 0; i < packet->Ntx; i++)
  {
    vector<array<complex<double>, 30>> tx;
    for (unsigned char j = 0; j < packet->Nrx; j++)
    {
      array<complex<double>, 30> rx;
      tx.push_back(rx);
    }
    comp.push_back(tx);
  }

  // Extract csi complex
  unsigned char *payload = (unsigned char *)packet->payload[0];
  unsigned int index = 0, remainder;
  char temp;
  for (unsigned char i = 0; i < 30; i++)
  {
    index += 3;
    remainder = index % 8;
    for (unsigned char j = 0; j < packet->Nrx * packet->Ntx; j++)
    {
      // Real
      temp = (payload[index / 8] >> remainder) | (payload[index / 8 + 1] << (8 - remainder));
      comp[j / 3][j % 3][i].real = (double)temp;
      // Imaginary
      temp = (payload[index / 8 + 1] >> remainder) | (payload[index / 8 + 2] << (8 - remainder));
      comp[j / 3][j % 3][i].imag = (double)temp;
      // Next
      index += 16;
    }
  }

  // Calculate permutation array
  array<double, 3> perm;
  perm[0] = (packet->antenna_sel & 0x3) + 1;
  perm[1] = ((packet->antenna_sel >> 2) & 0x3) + 1;
  perm[2] = ((packet->antenna_sel >> 4) & 0x3) + 1;

  return forward_as_tuple(comp, perm);
}

CSIVector& csi::getCSIVector(PacketVector &store)
{
  // For now, only calculate amplitude
  CSIVector csis;
  for (unsigned short i = 0; i < SYAA_WINDOW; i++)
  {
    // Get single complex CSI
    auto rawSingle = getSingleRawCSI(store[i]);
    CSISingleComplex& comp = get<0>(rawSingle);
    array<double, 3>& perm = get<1>(rawSingle);
    // Permutate CSI
    CSISingleComplex pomp;
    for (unsigned short j = 0; j < store[i]->Ntx; j++) {
      array<array<complex<double>, 30>, 3> tmpTx;
      for (unsigned short k = 0; k < store[i]->Nrx; k++) {
        tmpTx[perm[k]] = comp[j][k];
      }
      vector<array<complex<double>, 30>> tx;
      for (unsigned short k = 0; k < store[i]->Nrx; k++) {
        tx.push_back(tmpTx[k]);
      }
      pomp.push_back(tx);
    }
    // Scale CSI
    // Calculate Amplitude
    CSISingleAmp amp;
    // Reshape & Merge
  }
  return csis;
}
