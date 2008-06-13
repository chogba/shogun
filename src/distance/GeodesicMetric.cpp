/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Written (W) 2006-2008 Christian Gehl
 * Copyright (C) 2006-2008 Fraunhofer Institute FIRST
 */

#include "lib/config.h"
#include "lib/common.h"
#include "lib/io.h"
#include "distance/GeodesicMetric.h"
#include "features/Features.h"
#include "features/RealFeatures.h"

CGeodesicMetric::CGeodesicMetric()
: CSimpleDistance<DREAL>()
{
}

CGeodesicMetric::CGeodesicMetric(CRealFeatures* l, CRealFeatures* r)
: CSimpleDistance<DREAL>()
{
	init(l, r);
}

CGeodesicMetric::~CGeodesicMetric()
{
	cleanup();
}

bool CGeodesicMetric::init(CFeatures* l, CFeatures* r)
{
	bool result=CSimpleDistance<DREAL>::init(l,r);

	return result;
}

void CGeodesicMetric::cleanup()
{
}

bool CGeodesicMetric::load_init(FILE* src)
{
	return false;
}

bool CGeodesicMetric::save_init(FILE* dest)
{
	return false;
}

DREAL CGeodesicMetric::compute(INT idx_a, INT idx_b)
{
	INT alen, blen;
	bool afree, bfree;

	double* avec=((CRealFeatures*) lhs)->get_feature_vector(idx_a, alen, afree);
	double* bvec=((CRealFeatures*) rhs)->get_feature_vector(idx_b, blen, bfree);

	ASSERT(alen==blen);

	DREAL s=0;
	DREAL d=0;
	DREAL nx=0;
	DREAL ny=0;
	{
		for (INT i=0; i<alen; i++)
		{
			d+=avec[i]*bvec[i];
			nx+=avec[i]*avec[i];
			ny+=bvec[i]*bvec[i];
			s+=avec[i]+bvec[i];
		}
	}

	((CRealFeatures*) lhs)->free_feature_vector(avec, idx_a, afree);
	((CRealFeatures*) rhs)->free_feature_vector(bvec, idx_b, bfree);


	// trap division by zero
	if(s==0 || nx==0 || ny==0)
		return 0;

	d/=CMath::sqrt(nx*ny);

	// can only happen due to numerical problems
	if (CMath::abs(d)>1.0)
		d=CMath::sign(d);

	return acos(d);
}
