#########################################################
##	FireRing
##
FxInfo
	

Input  
	InpName	Origin
End

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Ring
		Type	Local 
		At	Origin
		Geom	FX_RuneGlows
		bhvr	behaviors/Cauldron_spin.bhvr
		Part1	World/Lairs/COT/CauldronRing.part
		Part2	World/Lairs/COT/CauldronRingMotionDown.part
		Part3	World/Lairs/COT/CauldronShimmerSparklies.part
	End

	Event
		Type	Local 
		At	Ring
		AltPiv 1
		bhvr	behaviors/Cauldron_spinFast.bhvr
		part4	World/Lairs/COT/CauldronTrailDown.part
		
	End


	Event
		Type	Local 
		At	Ring
		AltPiv 3
		bhvr	behaviors/Cauldron_spinFast.bhvr
		part4	World/Lairs/COT/CauldronTrailDown.part
		
	End

	

	Event
		Type	Local 
		At	Ring
		AltPiv 5
		bhvr	behaviors/Cauldron_spinFast.bhvr
		part4	World/Lairs/COT/CauldronTrailDown.part
		
	End

	

	Event
		Type	Local 
		At	Ring
		AltPiv 7
		bhvr	behaviors/Cauldron_spinFast.bhvr
		part4	World/Lairs/COT/CauldronTrailDown.part
		
	End


End

End			