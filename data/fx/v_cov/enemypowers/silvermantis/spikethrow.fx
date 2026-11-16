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
	Time 	20



	Event
		
		Type	Local
		At	Origin
		Sound thorntoss3 70 70 .79
	End
End


Condition
	On	Time
	Time	42
	

	Event
		EName	Prime
		Type	start
		At	WepR
		Geom	FX_SilverSpike02
		Bhvr	:ProjectileSilverSpike.bhvr
		Magnet	Target
		LookAt	Target
		Part	:SpikeTrailSmoke.part
		Part	:SpikeTrailSmoke.part
		Part	:SpikeTrailSmoke.part
		Part	:SpikeTrailSmoke2.part
		Part	:SpikeTrailSmoke2.part			
		Part	:silverSparkTrail.part	
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