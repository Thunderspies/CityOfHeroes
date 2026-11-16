#########################################################
##	cot_waterfall_med.fx
##
FxInfo

LifeSpan	200

Input  
	InpName	Origin
End


Input  
	InpName	Target
End

Input  
	InpName	WepR
End


###########################################################
##################################
##################################

Condition
	On 	Time
	Time 	13



	Event
		
		Type	Local
		At	Origin
		Sound Darttoss 70 70 .7
	End
End


Condition
	On	Time
	Time	15
	

	Event
		EName	Prime
		Type	start
		At	WepR
		Geom	FX_SilverSpike02
		Bhvr	:ProjectileSilverDart.bhvr
		Magnet	Target
		LookAt	Target
		Part	:SpikeTrailSmoke.part
		Part	:SpikeTrailSmoke.part
		Part	:SpikeTrailSmoke2.part		
		Part	:silverSparkTrail2.part
	End

End

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy
		

	End

End


End			