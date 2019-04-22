// Reference code by:
//   (c) 2008-2011 Daniel Halperin <dhalperi@cs.washington.edu>
// Widh Jio
// math.csi.cpp

#include <cmath>

#include "csi.hpp"

using std::array;
using std::complex;
using std::forward_as_tuple;
using std::get;
using std::tuple;
using std::vector;

tuple<CSISingleComplex &, double *> getSingleRawCSI(csi::BBPacket *packet)
{
  // Initialize return value
  CSISingleComplex *_comp = new CSISingleComplex();
  CSISingleComplex &comp = *_comp;
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
  unsigned char *payload = (unsigned char *)packet->payload;
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
      comp[j / 3][j % 3][i].real((double)temp);
      // Imaginary
      temp = (payload[index / 8 + 1] >> remainder) | (payload[index / 8 + 2] << (8 - remainder));
      comp[j / 3][j % 3][i].imag((double)temp);
      // Next
      index += 16;
    }
  }

  // Calculate permutation array
  double *perm = new double[3];
  perm[0] = (packet->antenna_sel & 0x3) + 1;
  perm[1] = ((packet->antenna_sel >> 2) & 0x3) + 1;
  perm[2] = ((packet->antenna_sel >> 4) & 0x3) + 1;

  return forward_as_tuple(comp, perm);
}

double getTotalRSS(double rssiA, double rssiB, double rssiC, uint8_t agc)
{
  double magic = 0;
  if (rssiA != 0)
  {
    magic += pow(10., rssiA / 10.);
  }
  if (rssiB != 0)
  {
    magic += pow(10., rssiB / 10.);
  }
  if (rssiC != 0)
  {
    magic += pow(10., rssiC / 10.);
  }
  if (magic == 0)
  {
    magic += 0b1;
  }
  return 10. * log10(magic) - 44 - double(agc);
}

tuple<long double, CSISingleAmp &> getCSIScaleAmp(CSISingleComplex &comp)
{
  long double total = 0;
  CSISingleAmp *amp = new CSISingleAmp();
  for (CSISingleComplex::iterator cit = comp.begin(); cit != comp.end(); ++cit)
  {
    CSITxDouble *tx = new CSITxDouble();
    for (CSITxComplex::iterator tit = (*cit).begin(); tit != (*cit).end(); ++tit)
    {
      CSITxRxDouble *rx = new CSITxRxDouble();
      for (unsigned short i = 0; i < 30; i++)
      {
        double addition = (*tit)[i].real() * (*tit)[i].real() + (*tit)[i].imag() * (*tit)[i].imag();
        total += addition;
        (*rx)[i] = sqrt(addition);
      }
      tx->push_back(*rx);
    }
    amp->push_back(*tx);
  }
  return forward_as_tuple(total, *amp);
}

CSISingleAmp &scaleAmp(CSISingleAmp &amp, double scale)
{
  for (CSISingleAmp::iterator cit = amp.begin(); cit != amp.end(); ++cit)
  {
    for (CSITxDouble::iterator tit = (*cit).begin(); tit != (*cit).end(); ++tit)
    {
      for (unsigned short i = 0; i < 30; i++)
      {
        // Calculate Amplitude
        (*tit)[i] = 10. * log10((*tit)[i] * scale + 1);
      }
    }
  }
  return amp;
}

CSIVector &csi::getCSIVector(PacketVector &store)
{
  CSIVector *csis = new CSIVector();
  for (unsigned short i = 0; i < SYAA_WINDOW; i++)
  {
    // Get single complex CSI
    auto rawSingle = getSingleRawCSI(store[i]);
    CSISingleComplex &comp = get<0>(rawSingle);
    double *perm = get<1>(rawSingle);
    // Permutate CSI
    CSISingleComplex pomp;
    for (unsigned short j = 0; j < store[i]->Ntx; j++)
    {
      array<array<complex<double>, 30>, 3> tmpTx;
      for (unsigned short k = 0; k < store[i]->Nrx; k++)
      {
        tmpTx[perm[k]] = comp[j][k];
      }
      vector<array<complex<double>, 30>> tx;
      for (unsigned short k = 0; k < store[i]->Nrx; k++)
      {
        tx.push_back(tmpTx[k]);
      }
      pomp.push_back(tx);
    }
    // Calculate scale factor
    double rssiPower = pow(10., getTotalRSS((double)store[i]->rssiA, (double)store[i]->rssiB, (double)store[i]->rssiC, store[i]->agc) / 10.);
    tuple<long double, CSISingleAmp &> csiScaleAmp = getCSIScaleAmp(comp);
    double scale = rssiPower / get<0>(csiScaleAmp) / 30.;
    // Get noise power
    double thermalNoisePower = pow(10., ((double)(store[i]->noise == -127 ? -92 : store[i]->noise)) / 10.);
    // Scale CSI
    double finalScaleFactor = sqrt(((double)(store[i]->Nrx * store[i]->Ntx)) + scale / thermalNoisePower);
    if (store[i]->Ntx == 2)
    {
      finalScaleFactor *= 1.41421356;
    }
    else if (store[i]->Ntx == 3)
    {
      finalScaleFactor *= 1.67880402;
    }
    // Get Amplitude
    CSISingleAmp &amp = scaleAmp(get<1>(csiScaleAmp), finalScaleFactor);
    // Reshape & Merge
    for (CSISingleAmp::iterator ait = amp.begin(); ait != amp.end(); ait++)
    {
      for (CSITxDouble::iterator tit = (*ait).begin(); tit != (*ait).end(); tit++)
      {
        for (unsigned short i = 0; i < 30; i++)
        {
          csis->push_back((*tit)[i]);
        }
      }
    }
  }
  return *csis;
}
