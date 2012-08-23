/*


############################################################################
##                Test Tools Source Code License Notice                   ##
############################################################################
                 Internet Streaming Media Alliance (ISMA)

          TEST TOOLS SOURCE CODE RELATED TO THE ISMA SPECIFICATION

ISMA Member Apple ('Submitting Member'), submitting the 
Contributed Code in this Test Tools source code has agreed to the
following terms and conditions by including this notice. 'Contributed Code'
means the code originally written by, or code modifications made by, the 
Submitting Member included in this Test Tools source code.  This submission
by Apple was registered as Contribution IN009.

COMPANYNAME1, COMPANYNAME2, COMPANYNAME3, COMPANYNAME4, and COMPANYNAME5
('Previous Submitting Members'), which previously submitted contributed 
code to this Test Tools source code, have agreed to the following terms 
and conditions in their respective submissions.

--------- ISMA Members downloading and/or using this Test Tool ------------
 PLEASE READ BEFORE COPYING, INSTALLING, OR USING.  By loading or using 
 the Test Tools, you agree to the terms and conditions herein.  If you do 
 not wish to so agree, do not load, install, or use this Test Tool.  
---------------------------------------------------------------------------

1.   Terms and Conditions:

1.1  Submitting Member hereby grants to ISMA an IRREVOCABLE, world-wide,
     royalty-free, non-exclusive license:
     (a) to intellectual property rights (Copyright and Patent) in the
         Contributed Code in this Test Tools submission Licensable 
         by the Submitting Member.

        'Licensable' means having the right to grant, to the maximum
         extent possible, whether at the time of the initial grant or
         subsequently acquired, any and all of the rights conveyed herein.

     (b) to use, reproduce and DISTRIBUTE, the submitted Test Tools
         as ISMA deems appropriate.

1.2  Submitting Member hereby grants to all ISMA Members a world-wide,
     royalty-free, non-exclusive license:
     (a) to intellectual property rights (Copyright and Patent) in the
         Contributed Code in this Test Tools submission Licensable 
         by the Submitting Member.
     (b) to use, reproduce and modify the Test Tools for the non-Commercial
         Use of self-testing the ISMA Member's own products in reference
         to the ISMA specifications.

        'Commercial Use' means distribution or otherwise making the
         sumbitted source code available to a non-ISMA Member.

     (c) to make derivative works of the Test Tools that improve or extend
         the utility of the Test Tools so long as the ISMA Member that
         creates such derivative work submits it back to ISMA, subject to
         the same terms and conditions described herein.
     (d) which does not include the right to sublicense or assign, or the
         right to incorporate the Test Tools in the ISMA Member's own products.
     (e) for as long as the Member remains an ISMA Member.

1.3  Submitting Member, Previous Submitting Members and ISMA, MAKE NO WARRANTY 
     OF NONINFRINGEMENT OF THE INTELLECTUAL PROPERTY RIGHTS OF THIRD PARTIES.  
     ISMA Member agrees that ISMA, Submitting Member, and the Previous Submitting
     Members shall NOT be liable or held responsible if use of the Test Tools 
     is found to infringe the intellectual property rights of third parties.

1.4  Submitting Member represents that it holds a good faith belief that the
     Test Tools source code submitted may be of assistance to ISMA Members
     in self-testing their products with reference to the ISMA Specification.
     HOWEVER, SUBMITTING MEMBER, PREVIOUS SUBMITTING MEMBERS, AND ISMA MAKE
     NO WARRANTIES, EXPRESS OR IMPLIED AND, IN PARTICULAR, NO WARRANTY OF 
     MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE. ISMA MEMBERS 
     UTILIZING THE TEST TOOLS DO SO AT THEIR OWN RISK.  SUBMITTING MEMBER, 
     PREVIOUS SUBMITTING MEMBERS, AND ISMA DISCLAIM ALL RESPONSIBILITY 
     FOR DAMAGES OR LIABILITY OF ANY KIND TO MEMBERS.

1.5  If the Submitting Member has knowledge that a license under a third 
     party's intellectual property rights may be required to exercise the
     rights granted by Submitting Member under Sections 1.1(b) or 1.2(b,c,d,e),
     the Submitting Member agrees to include a text file with the Test Tools 
     submitted titled 'LEGAL-3PARTY' which describes the claim and the party 
     making the claim in sufficient detail that a user will know whom to contact.

1.6  Submitting Member and Previous Submitting Members assume no 
     obligation to support or to update the submitted Test Tools 
     beyond the requirements of 1.5

1.7  ISMA assumes no obligation to support or to update any Test Tools and
     reserves the right to remove from the ISMA web site at any time any 
     Test Tools it finds to be infringing or otherwise problematical.
*/

#include "ValidateMP4.h"
#include <cmath>

#define ABS(a) (((a) < 0) ? -(a) : (a));

extern ValidateGlobals vg;

	// for use with ostypetostr_r() and int64todstr_r() for example;
    // when you're using one of these routines more than once in the same print statement
	char   tempStr1[32];
	char   tempStr2[32];
	char   tempStr3[32];
	char   tempStr4[32];
	char   tempStr5[32];
	char   tempStr6[32];
	char   tempStr7[32];
	char   tempStr8[32];
	char   tempStr9[32];
	char   tempStr10[32];


//==========================================================================================

int FindAtomOffsets( atomOffsetEntry *aoe, UInt64 minOffset, UInt64 maxOffset, 
			long *atomCountOut, atomOffsetEntry **atomOffsetsOut )
{
	int err = noErr;
	long cnt = 0;
	atomOffsetEntry *atomOffsets = nil;
	long max = 20;
	startAtomType startAtom;
	UInt64 largeSize;
	uuidType uuid;
	UInt64 curOffset = minOffset;
	long minAtomSize;
	
	BAILIFNULL( atomOffsets = (atomOffsetEntry *)calloc( max, sizeof(atomOffsetEntry)), allocFailedErr );
	
	while (curOffset< maxOffset) {
		memset(&atomOffsets[cnt], 0, sizeof(atomOffsetEntry));	// clear out entry
		atomOffsets[cnt].offset = curOffset;
		BAILIFERR( GetFileDataN32( aoe, &startAtom.size, curOffset, &curOffset ) );
		BAILIFERR( GetFileDataN32( aoe, &startAtom.type, curOffset, &curOffset ) );
		minAtomSize = sizeof(startAtom);
		atomOffsets[cnt].size = startAtom.size;
		atomOffsets[cnt].type = startAtom.type;
		if (startAtom.size == 1) {
			BAILIFERR( GetFileDataN64( aoe, &largeSize, curOffset, &curOffset ) );
			atomOffsets[cnt].size = largeSize;
			minAtomSize += sizeof(largeSize);
			
		}
		if (startAtom.type == 'uuid') {
			BAILIFERR( GetFileData( aoe, &uuid, curOffset, sizeof(uuid), &curOffset ) );
			//atomOffsets[cnt].uuid = uuid;
			memcpy(&atomOffsets[cnt].uuid, &uuid, sizeof(uuid));
			minAtomSize += sizeof(uuid);
		}
		
		atomOffsets[cnt].atomStartSize = minAtomSize;
		atomOffsets[cnt].maxOffset = atomOffsets[cnt].offset + atomOffsets[cnt].size;
		
		if (atomOffsets[cnt].size == 0) {
			// we go to the end
			atomOffsets[cnt].size = maxOffset - atomOffsets[cnt].offset;
			break;
		}
		
		BAILIF( (atomOffsets[cnt].size < (UInt64)minAtomSize), badAtomSize );
		
		curOffset = atomOffsets[cnt].offset + atomOffsets[cnt].size;
		cnt++;
		if (cnt >= max) {
			max += 20;
			atomOffsets = (atomOffsetEntry *)realloc(atomOffsets, max * sizeof(atomOffsetEntry));
		}
	}

bail:
	if (err) {
		cnt = 0;
		if (atomOffsets) 
			free(atomOffsets);
		atomOffsets = nil;
	}
	*atomCountOut = cnt;
	*atomOffsetsOut = atomOffsets;
	return err;
}

TrackInfoRec * check_track( UInt32 theID )
{
	MovieInfoRec	*mir = vg.mir;
	UInt32 i;
	
	if (theID==0) {
		errprint("Track ID %d in track reference atoms cannot be zero\n",theID);
		return 0;
	}
	
	for (i=0; i<(UInt32)mir->numTIRs; ++i) {
			if ((mir->tirList[i].trackID) == theID) return &(mir->tirList[i]);
	}		
	errprint("Track ID %d in track reference atoms references a non-existent track\n",theID);
    
    return 0;
}

UInt32 getTrakIndexByID(UInt32 track_ID)
{
    UInt32 i = 0;
    
    for(i = 0 ; i < (UInt32)vg.mir->numTIRs ; i++)
        if(vg.mir->tirList[i].trackID == track_ID)
            return i;

    errprint("getTrakIndexByID: Track ID %d is not a known track!\n",track_ID);

    return vg.mir->numTIRs;
}

OSErr ValidateFragmentInfo(MovieInfoRec *mir)
{
    UInt32 i;

    for(i = 0 ; i < (UInt32)mir->numTIRs ; i++)
    {
     mir->tirList[i].cumulatedTackFragmentDecodeTime = 0;
    }
    
    for(i = 0 ; i < mir->numFragments ; i++)
    {
        
        for(int k = 0 ; k < (UInt32)mir->numTIRs ; k++)
        {
            mir->moofInfo[i].tfdt[k] = mir->tirList[k].cumulatedTackFragmentDecodeTime;
        }
        
        if(mir->moofInfo[i].numTrackFragments > 0)
        {
                UInt32 j;
                    
                for(j = 0 ; j < mir->moofInfo[i].numTrackFragments ; j++)
                {
                    UInt32 index = getTrakIndexByID(mir->moofInfo[i].trafInfo[j].track_ID);

                    if(index >= (UInt32)mir->numTIRs)
                        return badAtomErr;
                    
                    if(mir->moofInfo[i].trafInfo[j].tfdtFound)
                    {
                        if(mir->moofInfo[i].trafInfo[j].baseMediaDecodeTime != mir->tirList[index].cumulatedTackFragmentDecodeTime)
                        {
                            if(i == 0 && vg.brandDASH)
                            {
                                warnprint("tfdt base media decode time %Lf not equal to accumulated decode time %Lf for track %d for the first fragment of the movie. This software does not handle incomplete presentations. Applying correction.\n",(long double)mir->moofInfo[i].trafInfo[j].baseMediaDecodeTime/(long double)mir->tirList[index].mediaTimeScale,(long double)mir->tirList[index].cumulatedTackFragmentDecodeTime/(long double)mir->tirList[index].mediaTimeScale,mir->moofInfo[i].trafInfo[j].track_ID);                                
                                mir->tirList[index].cumulatedTackFragmentDecodeTime = mir->moofInfo[i].trafInfo[j].baseMediaDecodeTime;
                            }
                            else
                                errprint("tfdt base media decode time %Lf not equal to accumulated decode time %Lf for track %d for sequence_number %d (fragment absolute count %d)\n",(long double)mir->moofInfo[i].trafInfo[j].baseMediaDecodeTime/(long double)mir->tirList[index].mediaTimeScale,(long double)mir->tirList[index].cumulatedTackFragmentDecodeTime/(long double)mir->tirList[index].mediaTimeScale,mir->moofInfo[i].trafInfo[j].track_ID,mir->moofInfo[i].sequence_number,i+1);
                        }

                    }

                        
                    if((mir->moofInfo[i].trafInfo[j].presentationEndTimeInTrackFragment + mir->tirList[index].cumulatedTackFragmentDecodeTime) > mir->tirList[index].lastPresentationTime)
                        mir->tirList[index].lastPresentationTime = mir->moofInfo[i].trafInfo[j].presentationEndTimeInTrackFragment + mir->tirList[index].cumulatedTackFragmentDecodeTime;
                        
                    mir->tirList[index].cumulatedTackFragmentDecodeTime += mir->moofInfo[i].trafInfo[j].cummulatedSampleDuration;

                }
                
        }
    }
	return noErr;
}

UInt32 getMoofIndexByOffset(MoofInfoRec *moofInfo, UInt32 numFragments, UInt64 offset)
{
    UInt32 i;

    for(i = 0 ; i < numFragments ; i++)
        if(moofInfo[i].offset == offset)
            return i;

    return numFragments;
}


SidxInfoRec *getSidxByOffset(SidxInfoRec *sidxInfo, UInt32 numSidx, UInt64 offset)
{
    UInt32 i;

    for(i = 0 ; i < numSidx ; i++)
        if(sidxInfo[i].offset == offset)
            return &sidxInfo[i];

    return (SidxInfoRec *)NULL;
}

bool checkSegmentBoundry(UInt64 offsetLow, UInt64 offsetHigh)
{
    UInt64 currentBoundry = 0;

    for(int i = 0 ; i < vg.segmentInfoSize ; i++)
    {
        currentBoundry += vg.segmentSizes[i];
        
        if(offsetLow < currentBoundry && offsetHigh >= currentBoundry)
            return true;
    }

    return false;
        
}

void verifyLeafDurations(MovieInfoRec *mir)
{
    for(int i = 0 ; i < mir->numTIRs ; i++)
    {
        TrackInfoRec *tir = &(mir->tirList[i]);

        for(UInt32 j = 0 ; j < tir->numLeafs ; j++)
        {
            long double diff = ABS((tir->leafInfo[j].presentationEndTime - tir->leafInfo[j].earliestPresentationTime) - tir->leafInfo[j].sidxReportedDuration);
            
            if(diff > (long double)1.0/(long double)tir->mediaTimeScale)
                errprint("Referenced track duration %Lf of track %d does not match to subsegment_duration %Lf for leaf with EPT %Lf\n",(tir->leafInfo[j].presentationEndTime - tir->leafInfo[j].earliestPresentationTime),tir->trackID,tir->leafInfo[j].sidxReportedDuration,tir->leafInfo[j].earliestPresentationTime);
        }
    }
}

void verifyAlignment(MovieInfoRec *mir)
{
    if(vg.checkSegAlignment == false && vg.checkSubSegAlignment == false)
        return;
    
    if(vg.numControlTracks != (unsigned int)mir->numTIRs)
    {
        errprint("Number of tracks logged %d in alignment control file not equal to the number of indexed tracks %d for this representation\n",vg.numControlTracks,mir->numTIRs);
        return;
    }
    
    for(int i = 0 ; i < mir->numTIRs ; i++)
    {
        TrackInfoRec *tir = &(mir->tirList[i]);
        
        if(vg.numControlLeafs[i] != tir->numLeafs)
        {
            errprint("Number of leafs %d in alignment control file for track %d not equal to the number of leafs %d for this representation\n",vg.numControlLeafs[i],tir->trackID,tir->numLeafs);
            continue;
        }
            
        for(UInt32 j = 0 ; j < (tir->numLeafs-1) ; j++)
        {
            if(vg.checkSubSegAlignment || (vg.checkSegAlignment && vg.controlLeafInfo[i][j+1].firstInSegment > 0))
                if(vg.controlLeafInfo[i][j+1].earliestPresentationTime <= tir->leafInfo[j].lastPresentationTime)
                {
                    if(vg.controlLeafInfo[i][j+1].firstInSegment > 0)
                        errprint("Overlapping segment: EPT of control leaf %Lf for leaf number %d is <= the latest presentation time %Lf corresponding leaf\n",vg.controlLeafInfo[i][j+1].earliestPresentationTime,j,tir->leafInfo[j].lastPresentationTime);
                    else
                        errprint("Overlapping subsegment: EPT of control leaf %Lf for leaf number %d is <= the latest presentation time %Lf corresponding leaf\n",vg.controlLeafInfo[i][j+1].earliestPresentationTime,j,tir->leafInfo[j].lastPresentationTime);
                }
        }
        
    }
}

void logLeafInfo(MovieInfoRec *mir)
{
    FILE *leafInfoFile = fopen("leafinfo.txt","wt");
    if(leafInfoFile == NULL)
    {
        printf("Error opening leafinfo.txt, logging will not be done!\n");
        return;
    }

    fprintf(leafInfoFile,"%ld\n",mir->numTIRs);
    
    for(int i = 0 ; i < mir->numTIRs ; i++)
    {
        TrackInfoRec *tir = &(mir->tirList[i]);

        fprintf(leafInfoFile,"%u\n",(unsigned int)tir->numLeafs);
        
        for(UInt32 j = 0 ; j < tir->numLeafs ; j++)
            fprintf(leafInfoFile,"%d %Lf %Lf\n",tir->leafInfo[j].firstInSegment,tir->leafInfo[j].earliestPresentationTime,tir->leafInfo[j].lastPresentationTime);
            
    }

    fclose(leafInfoFile);
}

OSErr ValidateIndexingInfo(MovieInfoRec *mir)
{
    UInt32 i;
    UInt64 absoluteOffset;
    UInt64 referenceEPT;
    UInt64 lastLeafEPT = 0;
    int leafsProcessed = 0;

    for(i = 0 ; i < (UInt32)mir->numTIRs ; i++)
        mir->tirList[i].leafInfo = (LeafInfo *)malloc(mir->tirList[i].numLeafs * sizeof(LeafInfo));
    
    for(i = 0 ; i < mir->numSidx ; i++)
    {
        UInt32 j;

        absoluteOffset = mir->sidxInfo[i].first_offset + mir->sidxInfo[i].offset + mir->sidxInfo[i].size;
        referenceEPT = mir->sidxInfo[i].earliest_presentation_time;
        
        UInt32 trackIndex = getTrakIndexByID(mir->sidxInfo[i].reference_ID);
        
        if(trackIndex >= (UInt32)mir->numTIRs)
            return badAtomErr;
        
        for(j = 0 ; j < mir->sidxInfo[i].reference_count ; j++)
        {
            SidxInfoRec *sidx;
            MoofInfoRec *moof;            
            
            if(mir->sidxInfo[i].references[j].reference_type == 1)
            {
                sidx = getSidxByOffset(mir->sidxInfo,mir->numSidx,absoluteOffset);
                if(sidx == NULL)
                    errprint("Referenced sidx not found for sidx number %d at reference count %d: Offset %lld\n",i,j,absoluteOffset);

                if(mir->sidxInfo[i].reference_ID != sidx->reference_ID)
                    errprint("Referenced sidx reference_ID %d does not match to reference_ID %d for sidx number %d at reference count %d ; Section 8.16.3.3 of ISO/IEC 14496-12 4th edition: if this Segment Index box is referenced from a \"parent\" Segment Index box, the value of reference_ID shall be the same as the value of reference_ID of the \"parent\" Segment Index box\n",sidx->reference_ID,mir->sidxInfo[i].reference_ID,i,j);

                if((double)referenceEPT/(double)mir->sidxInfo[i].timescale != (double)sidx->earliest_presentation_time/(double)sidx->timescale)
                    errprint("Referenced sidx earliest_presentation_time %lf does not match to reference EPT %lf for sidx number %d at reference count %d\n",(double)sidx->earliest_presentation_time/(double)sidx->timescale,(double)referenceEPT/(double)mir->sidxInfo[i].timescale,i,j);
                
                if((double)mir->sidxInfo[i].references[j].subsegment_duration != sidx->cumulatedDuration)
                    errprint("Referenced sidx duration %lf does not match to subsegment_duration %lf for sidx number %d at reference count %d\n",sidx->cumulatedDuration,(double)mir->sidxInfo[i].references[j].subsegment_duration,i,j);

				if(mir->sidxInfo[i].references[j].starts_with_SAP > 0)
					for(int k = 0 ; k < sidx->reference_count ; k++)
						if(sidx->references[k].starts_with_SAP == 0)
							errprint("Referenced sidx subsegment %d has a starts_with_SAP 0, while the starts_with_SAP of this reference (index %d of sidx %d) is set, violating Section 8.16.3.3 of ISO/IEC 14496-12 4th edition:\n",
															k,j,i);

				if(mir->sidxInfo[i].references[j].SAP_type > 0)
					for(int k = 0 ; k < sidx->reference_count ; k++)
						if((sidx->references[k].SAP_type == 0) || (sidx->references[k].SAP_type > mir->sidxInfo[i].references[j].SAP_type))
							errprint("Referenced sidx subsegment %d has a SAP_type %d while the SAP_type of this reference (index %d of sidx %d) has a SAP_type %d, violating Section 8.16.3.3 of ISO/IEC 14496-12 4th edition:\n",
															k,sidx->references[k].SAP_type,j,i,mir->sidxInfo[i].references[j].SAP_type);
            }
            else
            {
                UInt32 moofIndex = getMoofIndexByOffset(mir->moofInfo,mir->numFragments,absoluteOffset);
                
                if(moofIndex >= mir->numFragments)
                {
                    errprint("Referenced moof not found for sidx number %d at reference count %d: Offset %lld\n",i,j,absoluteOffset);
                    continue;
                }
                
                moof = &mir->moofInfo[moofIndex];

                TrackInfoRec *tir = &(mir->tirList[trackIndex]);

                if(moof->compositionInfoMissingPerTrack[trackIndex])
                {
                    warnprint("Composition info of the referred moof %d for sidx %d is missing.\n",moofIndex,i);
                    continue;
                }
                
                long double leafEPT = (long double)(moof->earliestMoofCompositionTimePerTrack[trackIndex] + moof->tfdt[trackIndex])/(long double)tir->mediaTimeScale;

                if((leafsProcessed > 0) && (leafEPT <= lastLeafEPT))
                {
                    warnprint("A referenced leaf has an EPT %Lf less than a previous (in decode order) leaf EPT %Lf, this is not handled yet! The following operation may be unreliable\n",leafEPT,lastLeafEPT);
                    //lastLeafEPT = leafEPT;
                    //continue;
                }

                if(leafsProcessed > 0 && mir->moofInfo[moofIndex - 1].compositionInfoMissingPerTrack[trackIndex])
                {
                    warnprint("Composition info of the moof %d for sidx %d is missing. The following operation may be unreliable\n",moofIndex-1,i);
                    //continue;
                }
                
                tir->leafInfo[leafsProcessed].earliestPresentationTime = leafEPT;

                if(leafsProcessed > 0)
                {
                    tir->leafInfo[leafsProcessed - 1].lastPresentationTime = (mir->moofInfo[moofIndex - 1].latestMoofCompositionTimePerTrack[trackIndex] + mir->moofInfo[moofIndex - 1].tfdt[trackIndex])/(long double)tir->mediaTimeScale;
                    tir->leafInfo[leafsProcessed - 1].presentationEndTime = (mir->moofInfo[moofIndex - 1].moofPresentationEndTimePerTrack[trackIndex] + mir->moofInfo[moofIndex - 1].tfdt[trackIndex])/(long double)tir->mediaTimeScale;
                }

                if(leafsProcessed == (tir->numLeafs - 1))
                {
                    tir->leafInfo[leafsProcessed].lastPresentationTime = (mir->moofInfo[mir->numFragments - 1].latestMoofCompositionTimePerTrack[trackIndex] + mir->moofInfo[mir->numFragments - 1].tfdt[trackIndex])/(long double)tir->mediaTimeScale;
                    tir->leafInfo[leafsProcessed].presentationEndTime= (mir->moofInfo[mir->numFragments - 1].moofPresentationEndTimePerTrack[trackIndex] + mir->moofInfo[mir->numFragments - 1].tfdt[trackIndex])/(long double)tir->mediaTimeScale;
                }
                
                tir->leafInfo[leafsProcessed].firstInSegment = leafsProcessed > 0 ? checkSegmentBoundry(mir->moofInfo[moofIndex - 1].offset, absoluteOffset) : true;

                tir->leafInfo[leafsProcessed].sidxReportedDuration = (long double)(mir->sidxInfo[i].references[j].subsegment_duration)/(long double)(mir->sidxInfo[i].timescale);
                    
                if((long double)referenceEPT/(long double)mir->sidxInfo[i].timescale != leafEPT)
                    errprint("Referenced moof earliest_presentation_time %Lf does not match to reference EPT %Lf for sidx number %d at reference count %d\n",leafEPT,(long double)referenceEPT/(long double)mir->sidxInfo[i].timescale,i,j);

				if(mir->sidxInfo[i].references[j].SAP_type > 2)
			    {
					warnprint("Sidx %d, index %d: SAP_type > 2 checks not yet implemented! The following operation may be unreliable\n",i,j);
					//continue;
			    }

                if(mir->sidxInfo[i].references[j].SAP_type > 0 || mir->sidxInfo[i].references[j].starts_with_SAP > 0)
                {
                    
                    double SAP_time = (double)(mir->sidxInfo[i].references[j].SAP_delta_time + referenceEPT)/(double)mir->sidxInfo[i].timescale;

                    UInt64 cummulatedDuration = 0;
                    double sampleCompositionTime;
                    bool SAPFound = false;
                    
                    for(UInt32 k = 0; k < moof->numTrackFragments ; k++)
                        if(moof->trafInfo[k].track_ID == mir->sidxInfo[i].reference_ID && moof->trafInfo[k].numTrun > 0)//Assuming 'trun' cannot be empty, 14496-12 version 4 does not indicate such a possiblity.
                        {
                            for(UInt32 l = 0 ; l < moof->trafInfo[k].numTrun ; l++)
                            {
                                for(UInt32 m = 0 ; m < moof->trafInfo[k].trunInfo[l].sample_count ; m++)
                                {
                                    UInt64 sample_composition_time_offset = moof->trafInfo[k].trunInfo[l].version != 0 ? (Int32)moof->trafInfo[k].trunInfo[l].sample_composition_time_offset[m] : (UInt32)moof->trafInfo[k].trunInfo[l].sample_composition_time_offset[m];

                                    sampleCompositionTime = (double)(sample_composition_time_offset + moof->tfdt[trackIndex] + cummulatedDuration)/tir->mediaTimeScale;

                                    bool sample_is_non_sync_sample = (moof->trafInfo[k].trunInfo[l].sample_flags[m] & 0x10000 >> 16) != 0;
                                    
                                    if(sampleCompositionTime == SAP_time)
                                    {
                                        if(sample_is_non_sync_sample)
                                        {
                                            errprint("SAP_type %d specified but the corresponding sample is sample_is_non_sync_sample, for sidx number %d at reference count %d\n",i,j);
                                        }
                                        
                                        SAPFound = true;
                                        //printf("SAP found with composition time %lf \n",sampleCompositionTime);
                                    }
                                    
                                    if(sampleCompositionTime < SAP_time && !sample_is_non_sync_sample)
                                        errprint("SAP found with composition time %lf lesser than the declared SAP time %lf (SAP_delta_time %lf), for sidx number %d at reference count %d; first SAP shall be signaled as per Section 8.16.3.3 of ISO/IEC 14496-12 4th edition\n",sampleCompositionTime,SAP_time,(double)(mir->sidxInfo[i].references[j].SAP_delta_time)/(double)mir->sidxInfo[i].timescale,i,j);
                                    
                                    cummulatedDuration += moof->trafInfo[k].trunInfo[l].sample_duration[m];
                                    
                                    if(SAPFound == true)
                                        break;
                                    
                                    if(mir->sidxInfo[i].references[j].starts_with_SAP > 0)
                                        errprint("starts_with_SAP declared but the first sample is not a SAP, for sidx number %d at reference count %d (checking sample %d of trun %d, traf %d, moof %d)\n",i,j,m+1,l+1,k+1,moofIndex+1);
                                }
                                
                                if(SAPFound == true)
                                    break;
                            }
                                                        
                            if(SAPFound == true)
                                break;
                        }

                    if(SAPFound != true)
                        errprint("SAP not found for sidx number %d at reference count %d\n",i,j);
    				
                }

                lastLeafEPT = leafEPT;

                leafsProcessed ++;

            }
            
            absoluteOffset+=mir->sidxInfo[i].references[j].referenced_size;
            referenceEPT+=mir->sidxInfo[i].references[j].subsegment_duration;
                
        }
        
    }

    verifyLeafDurations(mir);
    verifyAlignment(mir);
    logLeafInfo(mir);
    
	return noErr;
    
}


//==========================================================================================

OSErr ValidateFileAtoms( atomOffsetEntry *aoe, void *refcon )
{
#pragma unused(refcon)
	OSErr err = noErr;
	long cnt;
	atomOffsetEntry *list;
	long i;
	OSErr atomerr = noErr;
	atomOffsetEntry *entry;
	UInt64 minOffset, maxOffset;
	
	minOffset = aoe->offset + aoe->atomStartSize;
	maxOffset = aoe->offset + aoe->size - aoe->atomStartSize;
	
	BAILIFERR( FindAtomOffsets( aoe, minOffset, maxOffset, &cnt, &list ) );
	
	// Process 'ftyp' atom

	atomerr = ValidateAtomOfType( 'ftyp', kTypeAtomFlagMustHaveOne | kTypeAtomFlagCanHaveAtMostOne | kTypeAtomFlagMustBeFirst, 
		Validate_ftyp_Atom, cnt, list, nil );
	if (!err) err = atomerr;
	
	// Process 'moov' atoms
	vg.mir = NULL; 
	atomerr = ValidateAtomOfType( 'moov', kTypeAtomFlagMustHaveOne | kTypeAtomFlagCanHaveAtMostOne, 
		Validate_moov_Atom, cnt, list, nil );
	if (!err) err = atomerr;
	
	// Process 'meta' atoms
	atomerr = ValidateAtomOfType( 'meta', kTypeAtomFlagCanHaveAtMostOne, 
		Validate_meta_Atom, cnt, list, nil );
	if (!err) err = atomerr;
    
	// Count the total fragments and sidx's (if present), and allocate the required memory for that
	vg.mir->numFragments = 0;
	vg.mir->numSidx= 0;
    
	if(vg.mir->fragmented)
	{
    	for (i = 0; i < cnt; i++)
    	{
    		if (list[i].type == 'moof')
                vg.mir->numFragments++;
            
    		if (list[i].type == 'sidx')
                vg.mir->numSidx++;
    	}

        vg.mir->moofInfo = (MoofInfoRec *)malloc(vg.mir->numFragments*sizeof(MoofInfoRec));
        vg.mir->processedFragments = 0;

    	for (i = 0; i < (long)vg.mir->numFragments ; i++)
    	{
            vg.mir->moofInfo[i].compositionInfoMissingPerTrack = (Boolean*)malloc(vg.mir->numTIRs*sizeof(Boolean));
            vg.mir->moofInfo[i].cummulatedMoofSampleDurationPerTrack = (UInt64*)malloc(vg.mir->numTIRs*sizeof(UInt64));
            vg.mir->moofInfo[i].earliestMoofCompositionTimePerTrack = (UInt64*)malloc(vg.mir->numTIRs*sizeof(UInt64));
            vg.mir->moofInfo[i].latestMoofCompositionTimePerTrack = (UInt64*)malloc(vg.mir->numTIRs*sizeof(UInt64));
            vg.mir->moofInfo[i].moofPresentationEndTimePerTrack = (UInt64*)malloc(vg.mir->numTIRs*sizeof(UInt64));
            vg.mir->moofInfo[i].tfdt = (UInt64*)malloc(vg.mir->numTIRs*sizeof(UInt64));
    	}

        vg.mir->sidxInfo = (SidxInfoRec *)malloc(vg.mir->numSidx*sizeof(SidxInfoRec));
        vg.mir->processedSdixs = 0;
	}
    else
    {
        vg.mir->moofInfo = NULL;
        vg.mir->sidxInfo = NULL;
    }
    			
	//
	for (i = 0; i < cnt; i++) {
		entry = &list[i];

		switch (entry->type) {
			case 'mdat':

                if(vg.initializationSegment && entry->offset <= vg.segmentSizes[0])
                    warnprint("mdat found in initialization segment: Section 6.3.4.2. of ISO/IEC 23009-1:2012(E): The Initialization Segment shall not contain any media data with an assigned presentation time.\n");

			case 'skip':
			case 'free':
				break;

            case 'styp':
                atomerr = ValidateAtomOfType( 'styp', 0, 
                    Validate_styp_Atom, cnt, list, nil );
                if (!err) err = atomerr;
                break;
			
			case 'uuid':
					atomerr = ValidateAtomOfType( 'uuid', 0, 
						Validate_uuid_Atom, cnt, list, nil );
					if (!err) err = atomerr;
					break;
                    
            case 'moof':
                    if(!vg.mir->fragmented)
                        errprint("'moof' boxes are not to be expected without an 'mvex' in 'moov'\n");

                    if(vg.initializationSegment && entry->offset <= vg.segmentSizes[0])
                        errprint("moof found in initialization segment: Section 6.3.3. of ISO/IEC 23009-1:2012(E): It shall not contain any \"moof\" boxes.\n");
                    
                    atomerr = ValidateAtomOfType( 'moof', 0, 
                        Validate_moof_Atom, cnt, list, vg.mir);
                    if (!err) err = atomerr;

                    break;

            case 'sidx':
                    if(!vg.mir->fragmented)
                        errprint("'sidx' boxes are not to be expected in a non-fragmented movie\n");
                    
                    atomerr = ValidateAtomOfType( 'sidx', 0, 
                        Validate_sidx_Atom, cnt, list, vg.mir);
                    if (!err) err = atomerr;
            break;
            
			default:
				if (!(entry->aoeflags & kAtomValidated)) 
					warnprint("WARNING: unknown file atom '%s'\n",ostypetostr(entry->type));
				break;
		}
		
		if (!err) err = atomerr;
	}

	if(vg.mir->fragmented)
        ValidateFragmentInfo(vg.mir);

    if(vg.mir->numSidx > 0)
        ValidateIndexingInfo(vg.mir);
	
	aoe->aoeflags |= kAtomValidated;
bail:
	if ( vg.mir != NULL) {
		dispose_mir(vg.mir);
	}

	return err;
}

//==========================================================================================

OSErr Validate_dinf_Atom( atomOffsetEntry *aoe, void *refcon )
{
#pragma unused(refcon)
	OSErr err = noErr;
	long cnt;
	atomOffsetEntry *list;
	long i;
	OSErr atomerr = noErr;
	atomOffsetEntry *entry;
	UInt64 minOffset, maxOffset;
	
	atomprintnotab(">\n"); 
	
	minOffset = aoe->offset + aoe->atomStartSize;
	maxOffset = aoe->offset + aoe->size - aoe->atomStartSize;
	
	BAILIFERR( FindAtomOffsets( aoe, minOffset, maxOffset, &cnt, &list ) );
	
	// Process 'dref' atoms
	atomerr = ValidateAtomOfType( 'dref', kTypeAtomFlagMustHaveOne | kTypeAtomFlagCanHaveAtMostOne, 
		Validate_dref_Atom, cnt, list, nil );
	if (!err) err = atomerr;

	//
	for (i = 0; i < cnt; i++) {
		entry = &list[i];

		if (entry->aoeflags & kAtomValidated) continue;

		switch (entry->type) {
			default:
				warnprint("WARNING: unknown data information atom '%s'\n",ostypetostr(entry->type));
				break;
		}
		
		if (!err) err = atomerr;
	}
	
	aoe->aoeflags |= kAtomValidated;
bail:
	return err;
}
//==========================================================================================
OSErr Validate_edts_Atom( atomOffsetEntry *aoe, void *refcon )
{
#pragma unused(refcon)
	OSErr err = noErr;
	long cnt;
	atomOffsetEntry *list;
	long i;
	OSErr atomerr = noErr;
	atomOffsetEntry *entry;
	UInt64 minOffset, maxOffset;
	
	atomprintnotab(">\n"); 
	
	minOffset = aoe->offset + aoe->atomStartSize;
	maxOffset = aoe->offset + aoe->size - aoe->atomStartSize;
	
	BAILIFERR( FindAtomOffsets( aoe, minOffset, maxOffset, &cnt, &list ) );
	
	// Process 'elst' atoms
	atomerr = ValidateAtomOfType( 'elst', kTypeAtomFlagCanHaveAtMostOne, 
		Validate_elst_Atom, cnt, list, nil );
	if (!err) err = atomerr;

	//
	for (i = 0; i < cnt; i++) {
		entry = &list[i];

		if (entry->aoeflags & kAtomValidated) continue;

		switch (entry->type) {
			default:
				warnprint("WARNING: unknown edit list atom '%s'\n",ostypetostr(entry->type));
				break;
		}
		
		if (!err) err = atomerr;
	}
	
	aoe->aoeflags |= kAtomValidated;
bail:
	return err;
}
//==========================================================================================

OSErr Validate_minf_Atom( atomOffsetEntry *aoe, void *refcon )
{
#pragma unused(refcon)
	OSErr err = noErr;
	long cnt;
	atomOffsetEntry *list;
	long i;
	OSErr atomerr = noErr;
	atomOffsetEntry *entry;
	UInt64 minOffset, maxOffset;
	TrackInfoRec *tir = (TrackInfoRec *)refcon;
	
	atomprintnotab(">\n"); 
	
	minOffset = aoe->offset + aoe->atomStartSize;
	maxOffset = aoe->offset + aoe->size - aoe->atomStartSize;
	
	BAILIFERR( FindAtomOffsets( aoe, minOffset, maxOffset, &cnt, &list ) );
	
	// deal with the different header atoms
	switch (tir->mediaType) {
		case 'vide':
			// Process 'vmhd' atoms
			atomerr = ValidateAtomOfType( 'vmhd', kTypeAtomFlagMustHaveOne | kTypeAtomFlagCanHaveAtMostOne, 
				Validate_vmhd_Atom, cnt, list, nil );
			if (!err) err = atomerr;
			break;
		
		case 'soun':
			// Process 'smhd' atoms
			atomerr = ValidateAtomOfType( 'smhd', kTypeAtomFlagMustHaveOne | kTypeAtomFlagCanHaveAtMostOne, 
				Validate_smhd_Atom, cnt, list, nil );
			if (!err) err = atomerr;
			break;
		
		case 'hint':
			// Process 'hmhd' atoms
			atomerr = ValidateAtomOfType( 'hmhd',kTypeAtomFlagMustHaveOne | kTypeAtomFlagCanHaveAtMostOne, 
				Validate_hmhd_Atom, cnt, list, nil );
			if (!err) err = atomerr;
			break;
		
		case 'odsm':
		case 'sdsm':
			// Process 'nmhd' atoms
			atomerr = ValidateAtomOfType( 'nmhd', kTypeAtomFlagMustHaveOne | kTypeAtomFlagCanHaveAtMostOne, 
				Validate_nmhd_Atom, cnt, list, nil );
			if (!err) err = atomerr;
			break;
		default:
			warnprint("WARNING: unknown media type '%s'\n",ostypetostr(tir->mediaType));
	}


	// Process 'dinf' atoms
	atomerr = ValidateAtomOfType( 'dinf', kTypeAtomFlagMustHaveOne | kTypeAtomFlagCanHaveAtMostOne, 
		Validate_dinf_Atom, cnt, list, nil );
	if (!err) err = atomerr;

	// Process 'stbl' atoms
	atomerr = ValidateAtomOfType( 'stbl', kTypeAtomFlagMustHaveOne | kTypeAtomFlagCanHaveAtMostOne, 
		Validate_stbl_Atom, cnt, list, tir );
	if (!err) err = atomerr;

	//
	for (i = 0; i < cnt; i++) {
		entry = &list[i];

		if (entry->aoeflags & kAtomValidated) continue;

		switch (entry->type) {
			case 'odhd':
			case 'crhd':
			case 'sdhd':
			case 'm7hd':
			case 'ochd':
			case 'iphd':
			case 'mjhd':
				errprint("'%s' media type is reserved but not currently used\n", ostypetostr(entry->type));				
				atomprint("<%s *****>\n",ostypetostr(entry->type));
				atomprint("</%s>\n",ostypetostr(entry->type));
				break;
				
			default:
				warnprint("WARNING: unknown/unexpected atom '%s'\n",ostypetostr(entry->type));
				atomprint("<%s *****>\n",ostypetostr(entry->type));
				atomprint("</%s>\n",ostypetostr(entry->type));
				break;
		}
		
		if (!err) err = atomerr;
	}
	
	aoe->aoeflags |= kAtomValidated;
bail:
	return err;
}

//==========================================================================================

OSErr Validate_mdia_Atom( atomOffsetEntry *aoe, void *refcon )
{
#pragma unused(refcon)
	OSErr err = noErr;
	long cnt;
	atomOffsetEntry *list;
	long i;
	OSErr atomerr = noErr;
	atomOffsetEntry *entry;
	UInt64 minOffset, maxOffset;
	TrackInfoRec *tir = (TrackInfoRec *)refcon;
	
	atomprintnotab(">\n"); 
	
	minOffset = aoe->offset + aoe->atomStartSize;
	maxOffset = aoe->offset + aoe->size - aoe->atomStartSize;
	
	BAILIFERR( FindAtomOffsets( aoe, minOffset, maxOffset, &cnt, &list ) );
	
	// Process 'mdhd' atoms
	atomerr = ValidateAtomOfType( 'mdhd', kTypeAtomFlagMustHaveOne | kTypeAtomFlagCanHaveAtMostOne, 
		Validate_mdhd_Atom, cnt, list, tir );
	if (!err) err = atomerr;

	// Process 'hdlr' atoms
	atomerr = ValidateAtomOfType( 'hdlr', kTypeAtomFlagMustHaveOne | kTypeAtomFlagCanHaveAtMostOne, 
		Validate_mdia_hdlr_Atom, cnt, list, tir );
	if (!err) err = atomerr;

	// Process 'minf' atoms
	atomerr = ValidateAtomOfType( 'minf', kTypeAtomFlagMustHaveOne | kTypeAtomFlagCanHaveAtMostOne, 
		Validate_minf_Atom, cnt, list, tir );
	if (!err) err = atomerr;
	
	// Process 'uuid' atoms
	atomerr = ValidateAtomOfType( 'uuid', 0, 
		Validate_uuid_Atom, cnt, list, nil );
	if (!err) err = atomerr;

	//
	for (i = 0; i < cnt; i++) {
		entry = &list[i];

		if (entry->aoeflags & kAtomValidated) continue;

		switch (entry->type) {
			default:
				warnprint("WARNING: unknown media atom '%s'\n",ostypetostr(entry->type));
				break;
		}
		
		if (!err) err = atomerr;
	}
	
	aoe->aoeflags |= kAtomValidated;
bail:
	return err;
}

//==========================================================================================

OSErr Get_trak_Type( atomOffsetEntry *aoe, TrackInfoRec *tir )
{
	OSErr err = noErr;
	long cnt;
	atomOffsetEntry *list;
	long i;
	atomOffsetEntry *entry;
	UInt64 minOffset, maxOffset;

	long entrycnt;
	atomOffsetEntry *entrylist;
	atomOffsetEntry *entryentry;
	long	j;
	
	minOffset = aoe->offset + aoe->atomStartSize;
	maxOffset = aoe->offset + aoe->size - aoe->atomStartSize;
	
	BAILIFERR( FindAtomOffsets( aoe, minOffset, maxOffset, &cnt, &list ) );
	
	for (i = 0; i < cnt; i++) {
		entry = &list[i];
		if (entry->type == 'mdia') {
			minOffset = entry->offset + entry->atomStartSize;
			maxOffset = entry->offset + entry->size - entry->atomStartSize;
			BAILIFERR( FindAtomOffsets( entry, minOffset, maxOffset, &entrycnt, &entrylist ) );
			for (j=0; j<entrycnt; ++j) {
				entryentry = &entrylist[j];
				if (entryentry->type == 'hdlr') {
					Get_mdia_hdlr_mediaType(entryentry, tir);
					break;
				}
			}
			break;
		}
	}

bail:
	return err;
}

//==========================================================================================

OSErr Validate_trak_Atom( atomOffsetEntry *aoe, void *refcon )
{
	OSErr err = noErr;
	long cnt;
	atomOffsetEntry *list;
	long i;
	OSErr atomerr = noErr;
	atomOffsetEntry *entry;
	UInt64 minOffset, maxOffset;
	TrackInfoRec	*tir = (TrackInfoRec*)refcon;
	
	atomprintnotab(">\n"); 
			
	minOffset = aoe->offset + aoe->atomStartSize;
	maxOffset = aoe->offset + aoe->size - aoe->atomStartSize;
	
	BAILIFERR( FindAtomOffsets( aoe, minOffset, maxOffset, &cnt, &list ) );
	
	// Process 'tkhd' atoms
	atomerr = ValidateAtomOfType( 'tkhd', kTypeAtomFlagMustHaveOne | kTypeAtomFlagCanHaveAtMostOne, 
		Validate_tkhd_Atom, cnt, list, tir );
	if (!err) err = atomerr;

	// Process 'tref' atoms
	atomerr = ValidateAtomOfType( 'tref', kTypeAtomFlagCanHaveAtMostOne, 
		Validate_tref_Atom, cnt, list, tir );
	if (!err) err = atomerr;

	// Process 'edts' atoms
	atomerr = ValidateAtomOfType( 'edts', kTypeAtomFlagCanHaveAtMostOne, 
		Validate_edts_Atom, cnt, list, tir );
	if (!err) err = atomerr;

	// Process 'mdia' atoms
	atomerr = ValidateAtomOfType( 'mdia', kTypeAtomFlagMustHaveOne | kTypeAtomFlagCanHaveAtMostOne, 
		Validate_mdia_Atom, cnt, list, tir );
	if (!err) err = atomerr;

	// Process 'udta' atoms
	atomerr = ValidateAtomOfType( 'udta', 0, 
		Validate_udta_Atom, cnt, list, tir );
	if (!err) err = atomerr;

	// Process 'uuid' atoms
	atomerr = ValidateAtomOfType( 'uuid', 0, 
		Validate_uuid_Atom, cnt, list, tir );
	if (!err) err = atomerr;

	// Process 'meta' atoms
	atomerr = ValidateAtomOfType( 'meta', 0, 
		Validate_meta_Atom, cnt, list, tir );
	if (!err) err = atomerr;

	//
	for (i = 0; i < cnt; i++) {
		entry = &list[i];

		if (entry->aoeflags & kAtomValidated) continue;

		switch (entry->type) {
			default:
				warnprint("WARNING: unknown trak atom '%s'\n",ostypetostr(entry->type));
				break;
		}
		
		if (!err) err = atomerr;
	}


	// Extra checks
	switch (tir->mediaType) {
		case 'vide':
			if (tir->trackVolume) {
				errprint("Video track has non-zero trackVolume\n");
				err = badAtomSize;
			}
			if ((tir->trackWidth==0) || (tir->trackHeight==0)) {
				errprint("Video track has zero trackWidth and/or trackHeight\n");
				err = badAtomSize;
			}
			if (vg.checklevel >= checklevel_samples && !vg.brandDASH) {
				UInt64 sampleOffset;
				UInt32 sampleSize;
				UInt32 sampleDescriptionIndex;
				Ptr dataP = nil;
				BitBuffer bb;
				
				sampleprint("<vide_SAMPLE_DATA>\n"); vg.tabcnt++;
					for (i = 1; i <= (long)tir->sampleSizeEntryCnt; i++) {
						if ((vg.samplenumber==0) || (vg.samplenumber==i)) {
							err = GetSampleOffsetSize( tir, i, &sampleOffset, &sampleSize, &sampleDescriptionIndex );
							sampleprint("<sample num=\"%d\" offset=\"%s\" size=\"%d\" />\n",i,int64toxstr(sampleOffset),sampleSize); vg.tabcnt++;
							BAILIFNIL( dataP = (Ptr)malloc(sampleSize), allocFailedErr );
							err = GetFileData( vg.fileaoe, dataP, sampleOffset, sampleSize, nil );
							
							BitBuffer_Init(&bb, (UInt8 *)((void *)dataP), sampleSize);

							Validate_vide_sample_Bitstream( &bb, tir );
							free( dataP );
							--vg.tabcnt; sampleprint("</sample>\n");
						}
					}
				--vg.tabcnt; sampleprint("</vide_SAMPLE_DATA>\n");
			}
			break;

		case 'soun':
			if (tir->trackWidth || tir->trackHeight) {
				errprint("Sound track has non-zero trackWidth and/or trackHeight\n");
				err = badAtomSize;
			}
			if (vg.checklevel >= checklevel_samples && !vg.brandDASH) {
				UInt64 sampleOffset;
				UInt32 sampleSize;
				UInt32 sampleDescriptionIndex;
				Ptr dataP = nil;
				BitBuffer bb;
				
				sampleprint("<audi_SAMPLE_DATA>\n"); vg.tabcnt++;
					for (i = 1; i <= (long)tir->sampleSizeEntryCnt; i++) {
						if ((vg.samplenumber==0) || (vg.samplenumber==i)) {
							err = GetSampleOffsetSize( tir, i, &sampleOffset, &sampleSize, &sampleDescriptionIndex );
							sampleprint("<sample num=\"%d\" offset=\"%s\" size=\"%d\" />\n",i,int64toxstr(sampleOffset),sampleSize); vg.tabcnt++;
							BAILIFNIL( dataP = (Ptr)malloc(sampleSize), allocFailedErr );
							err = GetFileData( vg.fileaoe, dataP, sampleOffset, sampleSize, nil );
							
							BitBuffer_Init(&bb, (UInt8 *)dataP, sampleSize);

							Validate_soun_sample_Bitstream( &bb, tir );
							free( dataP );
							--vg.tabcnt; sampleprint("</sample>\n");
						}
					}
				--vg.tabcnt; sampleprint("</audi_SAMPLE_DATA>\n");
			}
			break;
			
		case 'odsm':
			if (tir->trackVolume || tir->trackWidth || tir->trackHeight) {
				errprint("ObjectDescriptor track has non-zero trackVolume, trackWidth, or trackHeight\n");
				err = badAtomSize;
			}
			if (vg.checklevel >= checklevel_samples && !vg.brandDASH) {
				UInt64 sampleOffset;
				UInt32 sampleSize;
				UInt32 sampleDescriptionIndex;
				Ptr dataP = nil;
				BitBuffer bb;
				
				sampleprint("<odsm_SAMPLE_DATA>\n"); vg.tabcnt++;
				for (i = 1; i <= (long)tir->sampleSizeEntryCnt; i++) {
					if ((vg.samplenumber==0) || (vg.samplenumber==i)) {
						err = GetSampleOffsetSize( tir, i, &sampleOffset, &sampleSize, &sampleDescriptionIndex );
						sampleprint("<sample num=\"%d\" offset=\"%s\" size=\"%d\" />\n",1,int64toxstr(sampleOffset),sampleSize); vg.tabcnt++;
							BAILIFNIL( dataP = (Ptr)malloc(sampleSize), allocFailedErr );
							err = GetFileData( vg.fileaoe, dataP, sampleOffset, sampleSize, nil );
							
							BitBuffer_Init(&bb, (UInt8 *)dataP, sampleSize);

							Validate_odsm_sample_Bitstream( &bb, tir );
							free( dataP );
						--vg.tabcnt; sampleprint("</sample>\n");
					}
				}
				--vg.tabcnt; sampleprint("</odsm_SAMPLE_DATA>\n");
			}
			break;

		case 'sdsm':
			if (tir->trackVolume || tir->trackWidth || tir->trackHeight) {
				errprint("SceneDescriptor track has non-zero trackVolume, trackWidth, or trackHeight\n");
				err = badAtomSize;
			}
			if (vg.checklevel >= checklevel_samples && !vg.brandDASH) {
				UInt64 sampleOffset;
				UInt32 sampleSize;
				UInt32 sampleDescriptionIndex;
				Ptr dataP = nil;
				BitBuffer bb;
				sampleprint("<sdsm_SAMPLE_DATA>\n"); vg.tabcnt++;
				for (i = 1; i <= (long)tir->sampleSizeEntryCnt; i++) {
					if ((vg.samplenumber==0) || (vg.samplenumber==i)) {
						err = GetSampleOffsetSize( tir, i, &sampleOffset, &sampleSize, &sampleDescriptionIndex );
						sampleprint("<sample num=\"%d\" offset=\"%s\" size=\"%d\" />\n",1,int64toxstr(sampleOffset),sampleSize); vg.tabcnt++;
							BAILIFNIL( dataP = (Ptr)malloc(sampleSize), allocFailedErr );
							err = GetFileData( vg.fileaoe, dataP, sampleOffset, sampleSize, nil );
							
							BitBuffer_Init(&bb, (UInt8 *)dataP, sampleSize);

							Validate_sdsm_sample_Bitstream( &bb, tir);
							free( dataP );
						--vg.tabcnt; sampleprint("</sample>\n");
					}
				}
				--vg.tabcnt; sampleprint("</sdsm_SAMPLE_DATA>\n");
			}
			break;

		case 'hint':
			Validate_Hint_Track(aoe, tir);
			break;

		default:
			if (tir->trackVolume || tir->trackWidth || tir->trackHeight) {
				errprint("Non-visual/audio track has non-zero trackVolume, trackWidth, or trackHeight\n");
				err = badAtomSize;
			}
			break;
	}
	
	aoe->aoeflags |= kAtomValidated;
bail:
	return err;
}
//==========================================================================================

OSErr Validate_stbl_Atom( atomOffsetEntry *aoe, void *refcon )
{
	OSErr err = noErr;
	long cnt;
	atomOffsetEntry *list;
	long i;
	OSErr atomerr = noErr;
	atomOffsetEntry *entry;
	UInt64 minOffset, maxOffset;
	TrackInfoRec *tir = (TrackInfoRec *)refcon;
	
	atomprintnotab(">\n"); 
	
	minOffset = aoe->offset + aoe->atomStartSize;
	maxOffset = aoe->offset + aoe->size - aoe->atomStartSize;
	
	BAILIFERR( FindAtomOffsets( aoe, minOffset, maxOffset, &cnt, &list ) );

    tir->identicalDecCompTimes = true;
    
	for (i = 0; i < cnt; i++) {
		entry = &list[i];
        if (entry->type == 'ctts')
            tir->identicalDecCompTimes = false; //Section 8.6.1.1.
	}
	
	// Process 'stsd' atoms
	atomerr = ValidateAtomOfType( 'stsd', kTypeAtomFlagMustHaveOne | kTypeAtomFlagCanHaveAtMostOne, 
		Validate_stsd_Atom, cnt, list, tir );
	if (!err) err = atomerr;

	// Process 'stts' atoms
	atomerr = ValidateAtomOfType( 'stts', kTypeAtomFlagMustHaveOne | kTypeAtomFlagCanHaveAtMostOne, 
		Validate_stts_Atom, cnt, list, tir );
	if (!err) err = atomerr;

	// Process 'ctts' atoms
	atomerr = ValidateAtomOfType( 'ctts', kTypeAtomFlagCanHaveAtMostOne, 
		Validate_ctts_Atom, cnt, list, tir );
	if (!err) err = atomerr;

	// Process 'stss' atoms
	atomerr = ValidateAtomOfType( 'stss', kTypeAtomFlagCanHaveAtMostOne, 
		Validate_stss_Atom, cnt, list, tir );
	if (!err) err = atomerr;

	// Process 'stsc' atoms
	atomerr = ValidateAtomOfType( 'stsc', kTypeAtomFlagMustHaveOne | kTypeAtomFlagCanHaveAtMostOne, 
		Validate_stsc_Atom, cnt, list, tir );
	if (!err) err = atomerr;

	// Process 'stsz' atoms
	atomerr = ValidateAtomOfType( 'stsz', /* kTypeAtomFlagMustHaveOne | */  kTypeAtomFlagCanHaveAtMostOne, 
		Validate_stsz_Atom, cnt, list, tir );
	if (!err) err = atomerr;

	// Process 'stz2' atoms;  we need to check there is one stsz or one stz2 but not both...
	atomerr = ValidateAtomOfType( 'stz2', /* kTypeAtomFlagMustHaveOne | */  kTypeAtomFlagCanHaveAtMostOne, 
		Validate_stz2_Atom, cnt, list, tir );
	if (!err) err = atomerr;

	// Process 'stco' atoms
	atomerr = ValidateAtomOfType( 'stco', /* kTypeAtomFlagMustHaveOne | */ kTypeAtomFlagCanHaveAtMostOne, 
		Validate_stco_Atom, cnt, list, tir );
	if (!err) err = atomerr;

	// Process 'co64' atoms
	atomerr = ValidateAtomOfType( 'co64', /* kTypeAtomFlagMustHaveOne | */ kTypeAtomFlagCanHaveAtMostOne, 
		Validate_co64_Atom, cnt, list, tir );
	if (!err) err = atomerr;

	// Process 'stsh' atoms	- shadow sync
	atomerr = ValidateAtomOfType( 'stsh', kTypeAtomFlagCanHaveAtMostOne, 
		Validate_stsh_Atom, cnt, list, tir );
	if (!err) err = atomerr;

	// Process 'stdp' atoms	- degradation priority
	atomerr = ValidateAtomOfType( 'stdp', kTypeAtomFlagCanHaveAtMostOne, 
		Validate_stdp_Atom, cnt, list, tir );
	if (!err) err = atomerr;

	// Process 'sdtp' atoms	- sample dependency
	atomerr = ValidateAtomOfType( 'sdtp', kTypeAtomFlagCanHaveAtMostOne, 
		Validate_sdtp_Atom, cnt, list, tir );
	if (!err) err = atomerr;

	// Process 'padb' atoms
	atomerr = ValidateAtomOfType( 'padb', kTypeAtomFlagCanHaveAtMostOne, 
		Validate_padb_Atom, cnt, list, tir );
	if (!err) err = atomerr;

	//
	for (i = 0; i < cnt; i++) {
		entry = &list[i];

		if (entry->aoeflags & kAtomValidated) continue;

		switch (entry->type) {
			default:
				warnprint("WARNING: unknown sample table atom '%s'\n",ostypetostr(entry->type));
				break;
		}
		
		if (!err) err = atomerr;
	}
	
	if (tir->sampleSizeEntryCnt != tir->timeToSampleSampleCnt) {
		errprint("Number of samples described by SampleSize table ('stsz') does NOT match"
				 " number of samples described by TimeToSample table ('stts') \n");
		err = badAtomErr;
	}
	if (!vg.brandDASH && tir->mediaDuration != tir->timeToSampleDuration) {
	
		errprint("Media duration (%s) in MediaHeader does NOT match"
				 " sum of durations described by TimeToSample table (%s) \n", 
				  int64todstr_r( tir->mediaDuration, tempStr1 ),
				  int64todstr_r( tir->timeToSampleDuration, tempStr2 ));
		err = badAtomErr;
	}
	if (tir->sampleToChunk) {

		if(tir->sampleToChunkEntryCnt)
		{
			UInt32 s;		// number of samples
			UInt32 leftover;

			if (tir->sampleToChunk[tir->sampleToChunkEntryCnt].firstChunk 
				> tir->chunkOffsetEntryCnt) {
				errprint("SampleToChunk table describes more chunks than"
						 " the ChunkOffsetTable table\n");
				err = badAtomErr;
			} 
			
			s = tir->sampleSizeEntryCnt - tir->sampleToChunkSampleSubTotal;
			leftover = s % (tir->sampleToChunk[tir->sampleToChunkEntryCnt].samplesPerChunk);
			if (leftover) {
				errprint("SampleToChunk table does not evenly describe"
						 " the number of samples as defined by the SampleToSize table\n");
				err = badAtomErr;
			}
		}
        else if(!vg.brandDASH)
		    warnprint("WARNING: STSC empty; with an empty STSC atom, chunk mapping is not verifiable\n");
	}

	aoe->aoeflags |= kAtomValidated;
bail:
	return err;
}
//==========================================================================================

OSErr Validate_mvex_Atom( atomOffsetEntry *aoe, void *refcon )
{
	OSErr err = noErr;
	long cnt;
	atomOffsetEntry *list;
	long i;
	OSErr atomerr = noErr;
	atomOffsetEntry *entry;
	UInt64 minOffset, maxOffset;
	TrackInfoRec *tir = (TrackInfoRec *)refcon;
	
	atomprintnotab(">\n"); 
	
	minOffset = aoe->offset + aoe->atomStartSize;
	maxOffset = aoe->offset + aoe->size - aoe->atomStartSize;
	
	BAILIFERR( FindAtomOffsets( aoe, minOffset, maxOffset, &cnt, &list ) );

    vg.mir->fragmented = true;
    vg.mir->sequence_number = 0;

    /*Section 8.8.3.1, Quantity:   Exactly one for each track in the Movie Box
      Doesnt say they have to be in order, so we have to manually check it.
      Since bit(4)	reserved=0, setting default_sample_flags is set to a test exception
      Not the cleanest approach though*/
      
    for(i = 0 ; i < vg.mir->numTIRs; i++)
	{
    	tir[i].default_sample_flags = 0xFFFFFFFF;
	}

    //todo: add optional 'leva' boxes
	// Process 'mehd' atoms
	atomerr = ValidateAtomOfType( 'mehd', kTypeAtomFlagCanHaveAtMostOne, 
		Validate_mehd_Atom, cnt, list, vg.mir );
	if (!err) err = atomerr;
    
	// Process 'trex' atoms
	atomerr = ValidateAtomOfType( 'trex', kTypeAtomFlagMustHaveOne, 
		Validate_trex_Atom, cnt, list, tir );
	if (!err) err = atomerr;

    /*Now check if any track information is missing*/
    for(i = 0 ; i < vg.mir->numTIRs ; i++)
	{
    	if(tir[i].default_sample_flags == 0xFFFFFFFF)
            errprint("'mxvex' found but 'trex' box missing for track %d\n",i);
	}

	//
	for (i = 0; i < cnt; i++) {
		entry = &list[i];

		if (entry->aoeflags & kAtomValidated) continue;

		switch (entry->type) {
			default:
				warnprint("WARNING: unknown sample table atom '%s'\n",ostypetostr(entry->type));
				break;
		}
		
		if (!err) err = atomerr;
	}

	aoe->aoeflags |= kAtomValidated;
bail:
	return err;
}


//==========================================================================================

OSErr ValidateAtomOfType( OSType theType, long flags, ValidateAtomTypeProcPtr validateProc, 
		long cnt, atomOffsetEntry *list, void *refcon )
{
	long i;
	OSErr err = noErr;
	char cstr[5] = {0};
	long typeCnt = 0;
	atomOffsetEntry *entry;
	OSErr atomerr;
	atompathType curatompath;
	Boolean curatomprint;
	Boolean cursampleprint;
	
	cstr[0] = (theType >> 24) & 0xff;
	cstr[1] = (theType >> 16) & 0xff;
	cstr[2] = (theType >>  8) & 0xff;
	cstr[3] = (theType >>  0) & 0xff;
	
	for (i = 0; i < cnt; i++) {
		entry = &list[i];
		
		if (entry->aoeflags & kAtomValidated) continue;
		
		if ((entry->type == theType) && ((entry->aoeflags & kAtomSkipThisAtom) == 0)) {
			if ((flags & kTypeAtomFlagCanHaveAtMostOne) && (typeCnt > 1)) {
				errprint("Multiple '%s' atoms not allowed\n", cstr);
			}
			if ((flags & kTypeAtomFlagMustBeFirst) && (i>0)) {
				if (i==1) warnprint("Warning: atom %s before ftyp atom MUST be a signature\n",ostypetostr((&list[0])->type));
				else errprint("Atom %s must be first and is actually at position %d\n",ostypetostr(theType),i+1);
			}			
			typeCnt++;
			addAtomToPath( vg.curatompath, theType, typeCnt, curatompath );
			if (vg.print_atompath) {
				fprintf(stdout,"%s\n", vg.curatompath);
			}
			curatomprint = vg.printatom;
			cursampleprint = vg.printsample;
			if ((vg.atompath[0] == 0) || (strcmp(vg.atompath,vg.curatompath) == 0)) {
				if (vg.print_atom)
					vg.printatom = true;
				if (vg.print_sample)
					vg.printsample = true;
			}
			atomprint("<%s",cstr); vg.tabcnt++;
				atomerr = CallValidateAtomTypeProc(validateProc, entry, 
											entry->refconOverride?((void*) (entry->refconOverride)):refcon);
			--vg.tabcnt; atomprint("</%s>\n",cstr); 
			vg.printatom = curatomprint;
			vg.printsample = cursampleprint;
			restoreAtomPath( vg.curatompath, curatompath );
			if (!err) err = atomerr;
		}
	}

	// 
	if ((flags & kTypeAtomFlagMustHaveOne)  && (typeCnt == 0)) {
		if( theType == IODSAID )
			warnprint( "\nWARNING: no 'iods' atom\n");
		else
			errprint("No '%s' atoms\n",cstr);
	} else if ((flags & kTypeAtomFlagCanHaveAtMostOne) && (typeCnt > 1)) {
		errprint("Multiple '%s' atoms not allowed\n",cstr);
	}

	return err;
}


//==========================================================================================

int mapStringToUInt32(char *src, UInt32 *target);


OSErr Validate_ftyp_Atom( atomOffsetEntry *aoe, void *refcon )
{
#pragma unused(refcon)
	OSErr err = noErr;
	UInt64 offset;
	OSType majorBrand;
	UInt32 version;
	UInt32 compatBrandListSize, numCompatibleBrands;
	char tempstr1[5], tempstr2[5];
	
	offset = aoe->offset + aoe->atomStartSize;
	
	BAILIFERR( GetFileDataN32( aoe, &majorBrand, offset, &offset ) );
	BAILIFERR( GetFileDataN32( aoe, &version, offset, &offset ) );

	atomprintnotab(" majorbrand=\"%.4s\" version=\"%s\", compatible_brands=[\n", ostypetostr_r(majorBrand, tempstr1), 
						int64toxstr((UInt64) version));

	vg.majorBrand = majorBrand;
	if( majorBrand == brandtype_isom ) {
		// the isom can only be a compatible brand
		errprint("The brand 'isom' can only be a compatible, not major, brand\n");
	}
	
	compatBrandListSize = (aoe->size - 8 - aoe->atomStartSize);
	numCompatibleBrands = compatBrandListSize / sizeof(OSType);
	
	if (0 != (compatBrandListSize % sizeof(OSType))) {
		errprint("FileType compatible brands array has leftover %d bytes\n", compatBrandListSize % sizeof(OSType));
	}
	if (numCompatibleBrands <= 0) {
		// must have at least one compatible brand, it must be the major brand
		errprint("There must be at least one compatible brand\n");
	}
	else {
		int ix;
		OSType currentBrand;
		Boolean majorBrandFoundAmongCompatibleBrands = false;
        vg.brandDASH = false;
        OSType dash;
        
        mapStringToUInt32((char *)"dash",&dash);
		
		for (ix=0; ix < (int)numCompatibleBrands; ix++) {
			BAILIFERR( GetFileDataN32( aoe, &currentBrand, offset, &offset ) );
			if (ix<(numCompatibleBrands-1)) atomprint("\"%s\",\n", ostypetostr_r(currentBrand, tempstr1));
			      else atomprint("\"%s\"\n",  ostypetostr_r(currentBrand, tempstr1));
			
			if (majorBrand == currentBrand) {
				majorBrandFoundAmongCompatibleBrands = true;
			}
			
			if (currentBrand == dash)
            {
				vg.brandDASH = true;
			}
			
		}

		if (!majorBrandFoundAmongCompatibleBrands) {
				
				errprint("major brand ('%.4s') not also found in list of compatible brands\n", 
						     ostypetostr_r(majorBrand,tempstr2));
			}

			
 	}
 	
 	atomprint("]>\n"); 
	
	aoe->aoeflags |= kAtomValidated;

bail:
	return noErr;
}

OSErr Validate_styp_Atom( atomOffsetEntry *aoe, void *refcon )
{
#pragma unused(refcon)
	OSErr err = noErr;
	UInt64 offset;
	OSType majorBrand;
	UInt32 version;
	UInt32 compatBrandListSize, numCompatibleBrands;
	char tempstr1[5], tempstr2[5];
    
	
	offset = aoe->offset + aoe->atomStartSize;
	
	BAILIFERR( GetFileDataN32( aoe, &majorBrand, offset, &offset ) );
	BAILIFERR( GetFileDataN32( aoe, &version, offset, &offset ) );

	atomprintnotab(" majorbrand=\"%.4s\" version=\"%s\", compatible_brands=[\n", ostypetostr_r(majorBrand, tempstr1), 
						int64toxstr((UInt64) version));

	vg.majorBrand = majorBrand;
	if( majorBrand == brandtype_isom ) {
		// the isom can only be a compatible brand
		errprint("The brand 'isom' can only be a compatible, not major, brand\n");
	}
	
	compatBrandListSize = (aoe->size - 8 - aoe->atomStartSize);
	numCompatibleBrands = compatBrandListSize / sizeof(OSType);
	
	if (0 != (compatBrandListSize % sizeof(OSType))) {
		errprint("FileType compatible brands array has leftover %d bytes\n", compatBrandListSize % sizeof(OSType));
	}
	if (numCompatibleBrands <= 0) {
		// must have at least one compatible brand, it must be the major brand
		errprint("There must be at least one compatible brand\n");
	}
	else {
		int ix;
		OSType currentBrand;
		Boolean majorBrandFoundAmongCompatibleBrands = false;
        bool msdhFound = false;
        OSType msdh;
        
        mapStringToUInt32((char *)"msdh",&msdh);
        		
		for (ix=0; ix < (int)numCompatibleBrands; ix++) {
			BAILIFERR( GetFileDataN32( aoe, &currentBrand, offset, &offset ) );
			if (ix<(numCompatibleBrands-1)) atomprint("\"%s\",\n", ostypetostr_r(currentBrand, tempstr1));
			      else atomprint("\"%s\"\n",  ostypetostr_r(currentBrand, tempstr1));
			
			if (majorBrand == currentBrand) {
				majorBrandFoundAmongCompatibleBrands = true;
			}
            
			if (currentBrand == msdh) {
				msdhFound = true;
			}
						
		}

		if (!majorBrandFoundAmongCompatibleBrands) {
				
				errprint("major brand ('%.4s') not also found in list of compatible brands\n", 
						     ostypetostr_r(majorBrand,tempstr2));
			}
        
		if (!msdhFound) {
				
				errprint("Brand msdh not found as a compatible brand; violates Section 6.3.4.2. of ISO/IEC 23009-1:2012(E)\n", 
						     ostypetostr_r(majorBrand,tempstr2));
			}

			
 	}
 	
 	atomprint("]>\n"); 
	
	aoe->aoeflags |= kAtomValidated;

bail:
	return noErr;
}


typedef struct track_track {
	UInt32 chunk_num;
	UInt32 chunk_cnt;
} track_track;

OSErr Validate_moov_Atom( atomOffsetEntry *aoe, void *refcon )
{
#pragma unused(refcon)
	OSErr err = noErr;
	long cnt;
	atomOffsetEntry *list;
	long i;
	OSErr atomerr = noErr;
	long trakCnt = 0;
	long thisTrakIndex = 0;
	atomOffsetEntry *entry;
	UInt64 minOffset, maxOffset;
	MovieInfoRec		*mir = NULL;
	
	atomprintnotab(">\n"); 
	
	minOffset = aoe->offset + aoe->atomStartSize;
	maxOffset = aoe->offset + aoe->size - aoe->atomStartSize;
	
	BAILIFERR( FindAtomOffsets( aoe, minOffset, maxOffset, &cnt, &list ) );

    if(vg.initializationSegment && ((aoe->offset + aoe->size) > vg.segmentSizes[0]))
        errprint("Complete moov not found in initialization segment: Section 6.3.3. of ISO/IEC 23009-1:2012(E): 16) The Initialization Segment shall contain an \"ftyp\" box, and a \"moov\" box\n");	

	// find out how many tracks we have so we can allocate our struct
	for (i = 0; i < cnt; i++) {
		entry = &list[i];
		if (entry->type == 'trak') {
			++trakCnt;
		}
	}
	
	if (trakCnt > 0) {
		i = (trakCnt-1) * sizeof(TrackInfoRec);
	} else {
		i = 0;
	}

	BAILIFNIL( vg.mir = (MovieInfoRec	*)calloc(1, sizeof(MovieInfoRec) + i), allocFailedErr );
	mir = vg.mir;
    mir->fragmented = false; //unless 'mvex' is found in 'moov'

	atomerr = ValidateAtomOfType( 'mvhd', kTypeAtomFlagMustHaveOne | kTypeAtomFlagCanHaveAtMostOne, 
		Validate_mvhd_Atom, cnt, list, NULL);
	if (!err) err = atomerr;


	// pre-process 'trak' atoms - get the track types
	// set refconOverride so Validate_trak_Atom gets a tir
	thisTrakIndex = 0;
	for (i = 0; i < cnt; i++) {
		entry = &list[i];
		if (entry->type == 'trak') {
			++(mir->numTIRs);
			atomerr = Get_trak_Type(entry, &(mir->tirList[thisTrakIndex]));
			entry->refconOverride = (long)&(mir->tirList[thisTrakIndex]);
			++thisTrakIndex;
		}
	}
		
	// disable processing hint 'trak' atoms
	//   adding ability to flag a text track to avoid reporting error when its matrix is non-identity
	thisTrakIndex = 0;
	for (i = 0; i < cnt; i++) {
		entry = &list[i];
		if (entry->type == 'trak') {
			if (mir->tirList[thisTrakIndex].mediaType == 'hint') {
				entry->aoeflags |= kAtomSkipThisAtom;
			}
			//    need to pass info that this is a text track to ValidateAtomOfType 'trak' below (refcon arg doesn't seem to work)
	
// ����
			++thisTrakIndex;
		}
	}


	// Process non-hint 'trak' atoms
	atomerr = ValidateAtomOfType( 'trak', 0, Validate_trak_Atom, cnt, list, nil );
	if (!err) err = atomerr;


	// enable processing hint 'trak' atoms
	thisTrakIndex = 0;
	for (i = 0; i < cnt; i++) {
		entry = &list[i];
		if (entry->type == 'trak') {
			if (mir->tirList[thisTrakIndex].mediaType == 'hint') {
				entry->aoeflags &= ~kAtomSkipThisAtom;
			}

// ����
			++thisTrakIndex;
		}
	}


	// Process hint 'trak' atoms
	atomerr = ValidateAtomOfType( 'trak', 0, Validate_trak_Atom, cnt, list, nil );
	if (!err) err = atomerr;
	
	// Process 'mvex' atoms
	atomerr = ValidateAtomOfType( 'mvex', kTypeAtomFlagCanHaveAtMostOne, 
		Validate_mvex_Atom, cnt, list, mir->tirList );
	if (!err) err = atomerr;

	// Process 'iods' atoms
	atomerr = ValidateAtomOfType( 'iods', kTypeAtomFlagMustHaveOne | kTypeAtomFlagCanHaveAtMostOne, 
		Validate_iods_Atom, cnt, list, nil );
	if (!err) err = atomerr;

	// Process 'udta' atoms
	atomerr = ValidateAtomOfType( 'udta', 0, 
		Validate_udta_Atom, cnt, list, nil );
	if (!err) err = atomerr;

	// Process 'uuid' atoms
	atomerr = ValidateAtomOfType( 'uuid', 0, 
		Validate_uuid_Atom, cnt, list, nil );
	if (!err) err = atomerr;

	// Process 'meta' atoms
	atomerr = ValidateAtomOfType( 'meta', 0, 
		Validate_meta_Atom, cnt, list, nil );
	if (!err) err = atomerr;

	//
	for (i = 0; i < cnt; i++){
		entry = &list[i];

		if (entry->aoeflags & kAtomValidated) continue;

		switch (entry->type) {
			case 'mdat':
			case 'skip':
			case 'free':
				break;
				
			case 'wide':	// this guy is QuickTime specific
			// �� if !qt, mpeg may be unfamiliar
				break;
				
			default:
				warnprint("WARNING: unknown movie atom '%s'\n",ostypetostr(entry->type));
				break;
		}
		
		if (!err) err = atomerr;
	}
	
	for (i=0; i<mir->numTIRs; ++i) {
			TrackInfoRec *tir;
			UInt8 all_single;\
			UInt32 j;
			
			tir = &(mir->tirList[i]);
			all_single = 1;

            tir->numLeafs = 0;
            tir->leafInfo = NULL;
			
			if (tir->chunkOffsetEntryCnt > 1) {
				for (j=1; j<=tir->sampleToChunkEntryCnt; j++) {
					if (tir->sampleToChunk[j].samplesPerChunk > 1) 
						{ all_single = 0; break; }
				}
				if (all_single == 1) warnprint("Warning: track %d has %d chunks all containing 1 sample only\n",
												i,tir->chunkOffsetEntryCnt );
			}
		}
		
	// Check for overlapped sample chunks [dws]
	//  this re-write relies on the fact that most tracks are in offset order and most files behave;
	//  we pick the track with the lowest unprocessed chunk offset;
	//  if that is beyond the highest chunk end we have seen, we append it;  otherwise (the rare case)
	//   we insert it into the sorted list.  this gives us a rapid check and an output sorted list without
	//   an n-squared overlap check and without a post-sort
	if(!vg.brandDASH)
	{
		UInt32 totalChunks = 0;
		TrackInfoRec *tir;
		UInt64 highwatermark;
		UInt32 trk_cnt, topslot = 0;
		track_track *trk;
		
		chunkOverlapRec *corp;
		
		UInt8 done = 0;
		
		trk_cnt = mir->numTIRs;
		
		BAILIFNULL( trk = (track_track *)calloc(trk_cnt,sizeof(track_track)), allocFailedErr );

		for (i=0; i<(long)trk_cnt; ++i) {
			// find the chunk counts for each track and setup structures
			tir = &(mir->tirList[i]);
			totalChunks += tir->chunkOffsetEntryCnt;
			
			trk[i].chunk_cnt = tir->chunkOffsetEntryCnt;
			trk[i].chunk_num = 1;	// the next chunk to work on for each track
			
		}
		BAILIFNULL( corp = (chunkOverlapRec *)calloc(totalChunks,sizeof(chunkOverlapRec)), allocFailedErr );
		
		highwatermark = 0;		// the highest chunk end seen

		do { // until we have processed all chunks of all tracks
			UInt32 lowest;
			UInt64 low_offset = 0;
			UInt64 chunkOffset, chunkStop;
			UInt32 chunkSize;
			UInt32 slot;
	
			// find the next lowest chunk start
			lowest = -1;		// next chunk not identified
			for (i=0; i<(long)trk_cnt; i++) {
				UInt64 offset;
				tir = &(mir->tirList[i]);
				if (trk[i].chunk_num <= trk[i].chunk_cnt) {		// track has chunks to process
					offset = tir->chunkOffset[ trk[i].chunk_num ].chunkOffset;
					if ((lowest == (UInt32)-1)  || ((lowest != (UInt32)-1) && (offset<low_offset)))
					{
						low_offset = offset;
						lowest = i;
					}
				}
			}
			if (lowest == (UInt32)-1) 
				errprint("aargh: program error!!!\n");
						
			tir = &(mir->tirList[lowest]);
			BAILIFERR( GetChunkOffsetSize(tir, trk[lowest].chunk_num, &chunkOffset, &chunkSize, nil) );
			if (chunkSize == 0) {
				errprint("Tracks with zero length chunks\n");
				err = badPublicMovieAtom;
				goto bail;
			}
			chunkStop = chunkOffset + chunkSize -1;
			
			if (chunkOffset != low_offset) errprint("Aargh! program error\n");
			
			if (chunkOffset >= (UInt64)vg.inMaxOffset) 
			{
				errprint("Chunk offset %s is at or beyond file size  0x%lx\n", int64toxstr(chunkOffset), vg.inMaxOffset);
			} else if (chunkStop > (UInt64)vg.inMaxOffset) 
			{
				errprint("Chunk end %s is beyond file size  0x%lx\n", int64toxstr(chunkStop), vg.inMaxOffset);
			}
			
			if (chunkOffset >= highwatermark)
			{	// easy, it starts after all other chunks end
				slot = topslot;
			} 
			else 
			{
				// have to insert the chunk into the list somewhere; it might overlap
				UInt32 k, priorslot, nextslot;
				
				// find the first chunk we already have that is starts after the candidate starts (if any)
				slot = topslot;
				for (k=0; k<topslot; k++) {
					// this could be done with binary chop, but it happens rarely
					if (corp[k].chunkStart > chunkOffset) { 
						slot = k; 
						break; 
					}
				}
				
				// Note we only warn if hint track chunks share data with other chunks, of if
				//  two tracks of the same type share data
				
				// do we overlap the prior slots (if any)?
				//   we might overlap slots before that, but if so, they must also overlap the slot
				//   prior to us, and we would have already reported that error
				if (slot > 0) {
					priorslot = slot-1;
					if ((chunkOffset >= corp[priorslot].chunkStart) && (chunkOffset <= corp[priorslot].chunkStop)) {
						if ((tir->mediaType == corp[priorslot].mediaType) || 
						    (tir->mediaType == 'hint') || 
							(corp[priorslot].mediaType == 'hint')) 
						warnprint("Warning: chunk %d of track ID %d at %s overlaps chunk from track ID %d at %s\n",
							trk[lowest].chunk_num, tir->trackID, int64todstr_r( chunkOffset, tempStr1 ), 
							corp[priorslot].trackID, int64todstr_r( corp[priorslot].chunkStart, tempStr2 ));
						else errprint("Error: chunk %d of track ID %d at %s overlaps chunk from track ID %d at %s\n",
							trk[lowest].chunk_num, tir->trackID, int64todstr_r( chunkOffset, tempStr1 ), 
							corp[priorslot].trackID, int64todstr_r( corp[priorslot].chunkStart, tempStr2 ));
					}
				}

				// do we overlap the next slots (if any)?
				//   again, we might overlap slots after that, but if so, we also overlap the next slot
				//   and one report is enough
				if (slot < topslot) {
					if ((chunkStop >= corp[slot].chunkStart) && (chunkStop <= corp[slot].chunkStop)) {
						if ((tir->mediaType == corp[slot].mediaType) || 
						    (tir->mediaType == 'hint') || 
							(corp[slot].mediaType == 'hint')) 
						warnprint("Warning: chunk %d of track ID %d at %s overlaps chunk from track ID %d at %s\n",
							trk[lowest].chunk_num, tir->trackID, int64todstr_r( chunkOffset, tempStr1 ), 
							corp[slot].trackID, int64todstr_r( corp[slot].chunkStart, tempStr2 ));
						else errprint("Error: chunk %d of track ID %d at %s overlaps chunk from track ID %d at %s\n",
							trk[lowest].chunk_num, tir->trackID, int64todstr_r( chunkOffset, tempStr1 ), 
							corp[slot].trackID, int64todstr_r( corp[slot].chunkStart, tempStr2 ));
					}
				}
					
				// now shuffle the array up prior to inserting this record into the sorted list
				for (nextslot = topslot; nextslot>slot; nextslot-- ) 
					corp[nextslot] = corp[ nextslot-1 ];
			}
			corp[ slot ].chunkStart = chunkOffset;
			corp[ slot ].chunkStop  = chunkStop;
			corp[ slot ].trackID 	= tir->trackID;
			corp[ slot ].mediaType  = tir->mediaType;  
			
				
			if (chunkStop > highwatermark) highwatermark = chunkStop;
			topslot++;

			trk[lowest].chunk_num += 1;		// done that chunk
			
			// see whether we have eaten all chunks for all tracks			
			done = 1;
			for (i=0; i<(long)trk_cnt; i++) {
				if (trk[i].chunk_num <= trk[i].chunk_cnt) { done = 0; break; }
			}
		} while (done != 1);
		// until we have processed all chunks of all tracks
	}
			
	aoe->aoeflags |= kAtomValidated;
bail:
//	if (mir != NULL) {
//		dispose_mir(mir);
//	}
	return err;
}

//==========================================================================================

OSErr Validate_moof_Atom( atomOffsetEntry *aoe, void *refcon )
{
	OSErr err = noErr;
	long cnt;
	atomOffsetEntry *list;
	long i;
	OSErr atomerr = noErr;
	atomOffsetEntry *entry;
	UInt64 minOffset, maxOffset;
    MovieInfoRec *mir = (MovieInfoRec *)refcon;
    
    MoofInfoRec *moofInfo = &mir->moofInfo[mir->processedFragments];
	
	atomprintnotab(">\n"); 
	
	minOffset = aoe->offset + aoe->atomStartSize;
	maxOffset = aoe->offset + aoe->size - aoe->atomStartSize;

    moofInfo->offset = aoe->offset;
	
	BAILIFERR( FindAtomOffsets( aoe, minOffset, maxOffset, &cnt, &list ) );

    atomerr = ValidateAtomOfType( 'mfhd', kTypeAtomFlagMustHaveOne | kTypeAtomFlagCanHaveAtMostOne, 
        Validate_mfhd_Atom, cnt, list, moofInfo );
    if (!err) err = atomerr;
    
    moofInfo->numTrackFragments = 0;
    moofInfo->processedTrackFragments = 0;

    for(i = 0 ;  i < mir->numTIRs ; i++)
    {
        moofInfo->cummulatedMoofSampleDurationPerTrack[i] = 0;
        moofInfo->earliestMoofCompositionTimePerTrack[i] = 0xFFFFFFFFFFFFFFFF;
        moofInfo->latestMoofCompositionTimePerTrack[i] = 0;
        moofInfo->moofPresentationEndTimePerTrack[i] = 0;
        moofInfo->compositionInfoMissingPerTrack[i] = false;
    }
    
	for (i = 0; i < cnt; i++)
		if (list[i].type == 'traf')
            moofInfo->numTrackFragments++;

    if(moofInfo->numTrackFragments > 0)
        moofInfo->trafInfo = (TrafInfoRec *)malloc(moofInfo->numTrackFragments*sizeof(TrafInfoRec));
    else
        moofInfo->trafInfo = NULL;
        
    atomerr = ValidateAtomOfType( 'traf', 0, 
        Validate_traf_Atom, cnt, list, moofInfo );
    if (!err) err = atomerr;
    
	//
	for (i = 0; i < cnt; i++) {
		entry = &list[i];

		if (entry->aoeflags & kAtomValidated) continue;

		switch (entry->type) {
                
			default:
				warnprint("WARNING: unknown moof atom '%s'\n",ostypetostr(entry->type));
				break;
		}
		
		if (!err) err = atomerr;
	}
    
    mir->processedFragments++;

	aoe->aoeflags |= kAtomValidated;
bail:
	return err;
}

//==========================================================================================

OSErr Validate_traf_Atom( atomOffsetEntry *aoe, void *refcon )
{
	OSErr err = noErr;
	long cnt;
	atomOffsetEntry *list;
	long i;
	OSErr atomerr = noErr;
	atomOffsetEntry *entry;
	UInt64 minOffset, maxOffset;
    MoofInfoRec *moofInfo = (MoofInfoRec *)refcon;
    
    TrafInfoRec *trafInfo = &moofInfo->trafInfo[moofInfo->processedTrackFragments];

    moofInfo->processedTrackFragments++;
    trafInfo->cummulatedSampleDuration = 0;
    trafInfo->compositionInfoMissing = false;
	
	atomprintnotab(">\n"); 
	
	minOffset = aoe->offset + aoe->atomStartSize;
	maxOffset = aoe->offset + aoe->size - aoe->atomStartSize;
	
	BAILIFERR( FindAtomOffsets( aoe, minOffset, maxOffset, &cnt, &list ) );

    atomerr = ValidateAtomOfType( 'tfhd', kTypeAtomFlagMustHaveOne | kTypeAtomFlagCanHaveAtMostOne, 
        Validate_tfhd_Atom, cnt, list, trafInfo );
    if (!err) err = atomerr;
    
    trafInfo->numTrun = 0;
    trafInfo->processedTrun = 0;
    trafInfo->tfdtFound = false;
    trafInfo->earliestCompositionTimeInTrackFragment = 0xFFFFFFFFFFFFFFFF;
    trafInfo->presentationEndTimeInTrackFragment = 0;
    trafInfo->latestPresentationTimeInTrackFragment = 0;
    
	for (i = 0; i < cnt; i++)
		if (list[i].type == 'trun')
            trafInfo->numTrun++;

    if(trafInfo->duration_is_empty && trafInfo->numTrun > 0)
        errprint("If the duration-is-empty flag is set in the tf_flags, there are no track runs.");

    if(trafInfo->numTrun > 0)
        trafInfo->trunInfo = (TrunInfoRec *)malloc(trafInfo->numTrun*sizeof(TrunInfoRec));
    else
        trafInfo->trunInfo = NULL;
    
    atomerr = ValidateAtomOfType( 'trun', 0, 
        Validate_trun_Atom, cnt, list, trafInfo );
    if (!err) err = atomerr;
    
    atomerr = ValidateAtomOfType( 'tfdt', kTypeAtomFlagCanHaveAtMostOne, 
        Validate_tfdt_Atom, cnt, list, trafInfo );
    if (!err) err = atomerr;
    
	//
	for (i = 0; i < cnt; i++) {
		entry = &list[i];

		if (entry->aoeflags & kAtomValidated) continue;

		switch (entry->type) {            
			default:
				warnprint("WARNING: unknown traf atom '%s'\n",ostypetostr(entry->type));
				break;
		}
		
		if (!err) err = atomerr;
	}


    //Accumulate durations now for later checking
	for (i = 0; i < (long)trafInfo->numTrun; i++)
	{
        trafInfo->cummulatedSampleDuration+=trafInfo->trunInfo[i].cummulatedSampleDuration;

        //Needed for DASH-specific processing of EPT
        TrackInfoRec *tir = check_track(trafInfo->track_ID);

        if(!tir->identicalDecCompTimes)
            trafInfo->compositionInfoMissing = trafInfo->compositionInfoMissing || (trafInfo->trunInfo[i].sample_count > 0 && trafInfo->trunInfo[i].sample_composition_time_offsets_present != true);
        else
        {
            trafInfo->compositionInfoMissing = false;
            for(UInt32 j = 0 ;  j < trafInfo->trunInfo[i].sample_count ; j++)
                if(trafInfo->trunInfo[i].sample_composition_time_offset[j] != 0)
                    errprint("CTTS is missing, indicating composition time = decode times, as per Section 8.16.1.1 of ISO/IEC 14496-12 4th edition, while non-zero composition offsets found in track run.\n");
        }
	}

    if(check_track(trafInfo->track_ID) == NULL)
        return badAtomErr;
    
    UInt32 index;

	index = getTrakIndexByID(trafInfo->track_ID);

    moofInfo->cummulatedMoofSampleDurationPerTrack[index] += trafInfo->cummulatedSampleDuration;
    moofInfo->compositionInfoMissingPerTrack[index] = moofInfo->compositionInfoMissingPerTrack[index] || trafInfo->compositionInfoMissing;

    if(!trafInfo->compositionInfoMissing && (trafInfo->earliestCompositionTimeInTrackFragment < moofInfo->earliestMoofCompositionTimePerTrack[index]))
        moofInfo->earliestMoofCompositionTimePerTrack[index] = trafInfo->earliestCompositionTimeInTrackFragment;

    if(!trafInfo->compositionInfoMissing && (trafInfo->latestPresentationTimeInTrackFragment > moofInfo->latestMoofCompositionTimePerTrack[index]))
        moofInfo->latestMoofCompositionTimePerTrack[index] = trafInfo->latestPresentationTimeInTrackFragment;

    if(!trafInfo->compositionInfoMissing && (trafInfo->presentationEndTimeInTrackFragment > moofInfo->moofPresentationEndTimePerTrack[index]))
        moofInfo->moofPresentationEndTimePerTrack[index] = trafInfo->presentationEndTimeInTrackFragment;

	aoe->aoeflags |= kAtomValidated;
bail:
	return err;
}

void dispose_mir( MovieInfoRec *mir )
{
    
    if(mir->moofInfo)
    {
        UInt32 i;

        for(i = 0 ; i < mir->numFragments ; i++)
        {

            //printf("Fragment number %d / %d\n",i,mir->numFragments);

            if(mir->moofInfo[i].trafInfo != NULL)
            {
                    UInt32 j;
                    
                    for(j = 0 ; j < mir->moofInfo[i].numTrackFragments ; j++)
                    {
                        //printf("Track Fragment number %d / %d, ptr %x\n",j,mir->moofInfo[i].numTrackFragments,&(mir->moofInfo[i].trafInfo[j]));
                        
                        if(mir->moofInfo[i].trafInfo[j].trunInfo != NULL)
                        {
                            UInt32 k;

                            for(k = 0 ; k < mir->moofInfo[i].trafInfo[j].numTrun ; k++)
                            {
                                if(mir->moofInfo[i].trafInfo[j].trunInfo[k].sample_duration != NULL)
                                    free(mir->moofInfo[i].trafInfo[j].trunInfo[k].sample_duration);
                                
                                if(mir->moofInfo[i].trafInfo[j].trunInfo[k].sample_size != NULL)
                                    free(mir->moofInfo[i].trafInfo[j].trunInfo[k].sample_size);
                                
                                if(mir->moofInfo[i].trafInfo[j].trunInfo[k].sample_flags != NULL)
                                    free(mir->moofInfo[i].trafInfo[j].trunInfo[k].sample_flags);
                                
                                if(mir->moofInfo[i].trafInfo[j].trunInfo[k].sample_composition_time_offset != NULL)
                                    free(mir->moofInfo[i].trafInfo[j].trunInfo[k].sample_composition_time_offset);
                            }
                            
                            free(mir->moofInfo[i].trafInfo[j].trunInfo);
                        }
                    }
                        
                free(mir->moofInfo[i].trafInfo);
                free(mir->moofInfo[i].compositionInfoMissingPerTrack);
                free(mir->moofInfo[i].cummulatedMoofSampleDurationPerTrack);
                free(mir->moofInfo[i].earliestMoofCompositionTimePerTrack);
                free(mir->moofInfo[i].latestMoofCompositionTimePerTrack);
                free(mir->moofInfo[i].moofPresentationEndTimePerTrack);
                free(mir->moofInfo[i].tfdt);
            }
        }
        
        free(mir->moofInfo);
    }

    for(int i = 0 ; i < mir->numTIRs ; i++)
        if(mir->tirList[i].leafInfo)
            free(mir->tirList[i].leafInfo);

    
    if(mir->sidxInfo)
    {
        UInt32 i;

        for(i = 0 ; i < mir->numSidx ; i++)
        {
            free(mir->sidxInfo[i].references);
        }

        free(mir->sidxInfo);
    }

	// for each track, get rid of the stuff in it
	free( mir );
}

//==========================================================================================

//==========================================================================================

OSErr Validate_tref_Atom( atomOffsetEntry *aoe, void *refcon )
{
	OSErr err = noErr;
	long cnt;
	atomOffsetEntry *list;
	long i;
	OSErr atomerr = noErr;
	atomOffsetEntry *entry;
	UInt64 minOffset, maxOffset;
	
	atomprintnotab(">\n"); 
	
	minOffset = aoe->offset + aoe->atomStartSize;
	maxOffset = aoe->offset + aoe->size - aoe->atomStartSize;
	
	BAILIFERR( FindAtomOffsets( aoe, minOffset, maxOffset, &cnt, &list ) );
	
	// Process 'tref_hint' atoms
	atomerr = ValidateAtomOfType( 'hint', kTypeAtomFlagCanHaveAtMostOne, 
		Validate_tref_hint_Atom, cnt, list, refcon );
	if (!err) err = atomerr;

	// Process 'tref_dpnd' atoms
	atomerr = ValidateAtomOfType( 'dpnd', kTypeAtomFlagCanHaveAtMostOne, 
		Validate_tref_dpnd_Atom, cnt, list, refcon );
	if (!err) err = atomerr;

	// Process 'tref_ipir' atoms
	atomerr = ValidateAtomOfType( 'ipir', kTypeAtomFlagCanHaveAtMostOne, 
		Validate_tref_ipir_Atom, cnt, list, refcon );
	if (!err) err = atomerr;

	// Process 'tref_mpod' atoms
	atomerr = ValidateAtomOfType( 'mpod', kTypeAtomFlagCanHaveAtMostOne, 
		Validate_tref_mpod_Atom, cnt, list, refcon );
	if (!err) err = atomerr;

	// Process 'tref_sync' atoms
	atomerr = ValidateAtomOfType( 'sync', kTypeAtomFlagCanHaveAtMostOne, 
		Validate_tref_sync_Atom, cnt, list, refcon );
	if (!err) err = atomerr;

	//
	for (i = 0; i < cnt; i++) {
		entry = &list[i];

		if (entry->aoeflags & kAtomValidated) continue;

		switch (entry->type) {
			default:
				warnprint("WARNING: unknown track reference atom '%s'\n",ostypetostr(entry->type));
				break;
		}
		
		if (!err) err = atomerr;
	}
	
	aoe->aoeflags |= kAtomValidated;
bail:
	return err;
}

//==========================================================================================

OSErr Validate_udta_Atom( atomOffsetEntry *aoe, void *refcon )
{
#pragma unused(refcon)
	OSErr err = noErr;
	long cnt;
	atomOffsetEntry *list;
	long i;
	OSErr atomerr = noErr;
	atomOffsetEntry *entry;
	UInt64 minOffset, maxOffset;
	
	atomprintnotab(">\n"); 
	
	minOffset = aoe->offset + aoe->atomStartSize;
	maxOffset = aoe->offset + aoe->size - aoe->atomStartSize;
	
	BAILIFERR( FindAtomOffsets( aoe, minOffset, maxOffset, &cnt, &list ) );
	
	// Process 'cprt' atoms
	atomerr = ValidateAtomOfType( 'cprt', 0,		// can have multiple copyright atoms 
		Validate_cprt_Atom, cnt, list, nil );
	if (!err) err = atomerr;

	// Process 'loci' atoms
	atomerr = ValidateAtomOfType( 'loci', 0,		// can have multiple copyright atoms 
								 Validate_loci_Atom, cnt, list, nil );
	if (!err) err = atomerr;


    // Process 'hnti' atoms
	atomerr = ValidateAtomOfType( 'hnti', kTypeAtomFlagCanHaveAtMostOne,
		Validate_moovhnti_Atom, cnt, list, nil );
	if (!err) err = atomerr;

	//
	for (i = 0; i < cnt; i++) {
		entry = &list[i];

		if (entry->aoeflags & kAtomValidated) continue;

		switch (entry->type) {
			default:
				warnprint("WARNING: unknown/unexpected atom '%s'\n",ostypetostr(entry->type));
				break;
		}
		
		if (!err) err = atomerr;
	}
	
	aoe->aoeflags |= kAtomValidated;
bail:
	return err;
}


//==========================================================================================

//==========================================================================================
//==========================================================================================

static OSErr Validate_rtp_Atom( atomOffsetEntry *aoe, void *refcon )
{
	OSErr 			err = noErr;
    UInt32			dataSize;
    char			*current;
	OSType			subType;
	Ptr				rtpDataP = NULL;
	Ptr				sdpDataP = NULL;
	UInt64			temp64;
    
    atomprintnotab(">\n"); 

	BAILIFNIL( rtpDataP = (Ptr)malloc((UInt32)aoe->size), allocFailedErr );

    dataSize = aoe->size - aoe->atomStartSize;
	BAILIFERR( GetFileData(aoe, rtpDataP, aoe->offset + aoe->atomStartSize, dataSize, &temp64) );
	
	current = rtpDataP;
	subType = EndianU32_BtoN(*((UInt32*)current));
	current += sizeof(UInt32);

    if (subType == 'sdp ') {
		// we found the sdp data
		// make a copy and null terminate it
		dataSize -= 4; // subtract the subtype field from the length 
		BAILIFNIL( sdpDataP = (Ptr)malloc(dataSize+1), allocFailedErr );
		memcpy(sdpDataP, current, dataSize);
		sdpDataP[dataSize] = '\0';
		
		BAILIFERR( Validate_Movie_SDP(sdpDataP) );
	} else {
		errprint("no sdp in movie user data\n");
		err = outOfDataErr;
		goto bail;
	}
    
bail:
	return err;
}

OSErr Validate_moovhnti_Atom( atomOffsetEntry *aoe, void *refcon )
{
	OSErr err = noErr;
	long cnt;
	atomOffsetEntry *list;
	long i;
	OSErr atomerr = noErr;
	atomOffsetEntry *entry;
	UInt64 minOffset, maxOffset;
	
	atomprintnotab(">\n"); 
			
	minOffset = aoe->offset + aoe->atomStartSize;
	maxOffset = aoe->offset + aoe->size - aoe->atomStartSize;
	
	BAILIFERR( FindAtomOffsets( aoe, minOffset, maxOffset, &cnt, &list ) );
	
	// Process 'rtp ' atoms
	atomerr = ValidateAtomOfType( 'rtp ', kTypeAtomFlagCanHaveAtMostOne, 
		Validate_rtp_Atom, cnt, list, NULL );
	if (!err) err = atomerr;
    
    for (i = 0; i < cnt; i++) {
		entry = &list[i];

		if (entry->aoeflags & kAtomValidated) continue;

		switch (entry->type) {
			default:
			// �� should warn
				break;
		}
		
		if (!err) err = atomerr;
	}
	
	aoe->aoeflags |= kAtomValidated;
bail:
	return err;
}

//==========================================================================================

OSErr Validate_sinf_Atom( atomOffsetEntry *aoe, void *refcon, UInt32 flags )
{
#pragma unused(refcon)
	OSErr err = noErr;
	long cnt;
	atomOffsetEntry *list;
	long i;
	OSErr atomerr = noErr;
	atomOffsetEntry *entry;
	UInt64 minOffset, maxOffset;
	
	atomprintnotab(">\n"); 
	
	minOffset = aoe->offset + aoe->atomStartSize;
	maxOffset = aoe->offset + aoe->size - aoe->atomStartSize;
	
	BAILIFERR( FindAtomOffsets( aoe, minOffset, maxOffset, &cnt, &list ) );
	
	// Process 'frma' atoms
	atomerr = ValidateAtomOfType( 'frma', flags | kTypeAtomFlagCanHaveAtMostOne, 
		Validate_frma_Atom, cnt, list, nil );
	if (!err) err = atomerr;

	// Process 'schm' atoms
	atomerr = ValidateAtomOfType( 'schm', kTypeAtomFlagCanHaveAtMostOne, 
		Validate_schm_Atom, cnt, list, nil );
	if (!err) err = atomerr;

	// Process 'schi' atoms
	atomerr = ValidateAtomOfType( 'schi', kTypeAtomFlagCanHaveAtMostOne, 
		Validate_schi_Atom, cnt, list, nil );
	if (!err) err = atomerr;

	for (i = 0; i < cnt; i++) {
		entry = &list[i];

		if (entry->aoeflags & kAtomValidated) continue;

		switch (entry->type) {				
			default:
				warnprint("WARNING: unknown security information atom '%s'\n",ostypetostr(entry->type));
				break;
		}
		
		if (!err) err = atomerr;
	}
	
	aoe->aoeflags |= kAtomValidated;
bail:
	return err;
}

//==========================================================================================

OSErr Validate_meta_Atom( atomOffsetEntry *aoe, void *refcon )
{
#pragma unused(refcon)
	OSErr err = noErr;
	long cnt;
	atomOffsetEntry *list;
	long i;
	OSErr atomerr = noErr;
	atomOffsetEntry *entry;
	UInt64 offset, minOffset, maxOffset;
	
	UInt32 version;
	UInt32 flags;

	atomprintnotab(">\n"); 
	
	// Get version/flags
	BAILIFERR( GetFullAtomVersionFlags( aoe, &version, &flags, &offset ) );
	atomprintnotab("\tversion=\"%d\" flags=\"%d\"\n", version, flags);
	FieldMustBe( version, 0, "version must be %d not %d" );
	FieldMustBe( flags, 0, "flags must be %d not %d" );

	minOffset = offset;
	maxOffset = aoe->offset + aoe->size;
	
	BAILIFERR( FindAtomOffsets( aoe, minOffset, maxOffset, &cnt, &list ) );
	
	// Process 'hdlr' atoms
	atomerr = ValidateAtomOfType( 'hdlr', kTypeAtomFlagMustHaveOne | kTypeAtomFlagCanHaveAtMostOne, 
		Validate_hdlr_Atom, cnt, list, nil );
	if (!err) err = atomerr;

	// Process 'pitm' atoms
	atomerr = ValidateAtomOfType( 'pitm', kTypeAtomFlagCanHaveAtMostOne, 
		Validate_pitm_Atom, cnt, list, nil );
	if (!err) err = atomerr;

	// Process 'dinf' atoms
	atomerr = ValidateAtomOfType( 'dinf', kTypeAtomFlagCanHaveAtMostOne, 
		Validate_dinf_Atom, cnt, list, nil );
	if (!err) err = atomerr;

	// Process 'iloc' atoms
	atomerr = ValidateAtomOfType( 'iloc', kTypeAtomFlagCanHaveAtMostOne, 
		Validate_iloc_Atom, cnt, list, nil );
	if (!err) err = atomerr;

	// Process 'ipro' atoms
	atomerr = ValidateAtomOfType( 'ipro', kTypeAtomFlagCanHaveAtMostOne, 
		Validate_ipro_Atom, cnt, list, nil );
	if (!err) err = atomerr;

	// Process 'iinf' atoms
	atomerr = ValidateAtomOfType( 'iinf', kTypeAtomFlagCanHaveAtMostOne, 
		Validate_iinf_Atom, cnt, list, nil );
	if (!err) err = atomerr;

	// Process 'xml ' atoms
	atomerr = ValidateAtomOfType( 'xml ', kTypeAtomFlagCanHaveAtMostOne, 
		Validate_xml_Atom, cnt, list, nil );
	if (!err) err = atomerr;

	// Process 'bxml' atoms
	atomerr = ValidateAtomOfType( 'bxml', kTypeAtomFlagCanHaveAtMostOne, 
		Validate_xml_Atom, cnt, list, nil );
	if (!err) err = atomerr;

	for (i = 0; i < cnt; i++) {
		entry = &list[i];

		if (entry->aoeflags & kAtomValidated) continue;

		switch (entry->type) {				
			default:
				warnprint("WARNING: unknown meta atom '%s'\n",ostypetostr(entry->type));
				break;
		}
		
		if (!err) err = atomerr;
	}
	
	aoe->aoeflags |= kAtomValidated;
bail:
	return err;
}

