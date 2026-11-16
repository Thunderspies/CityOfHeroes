#########################################################
##	RadiationAura
FxInfo

LifeSpan	200

Input  
	InpName	Hips
End

########################################################


Condition
	On	Time
	Time	20

	Event	
		Ename	LightRaysSpillage
		Type	Posit
		At	Hips
		part1	:RadiationBlastHitRings.part
		part2	:RadiationBlastHitDots.part
		part3	:RadiationCloudHit.part		
		
	End
		
		
End


Condition
	On	Time
	Time	80

	Event
		
		Type	Posit
		At	Hips
		part1	:RadiationBlastCircle2.part
		
	End

End


Condition
	On	Time
	Time	125

	Event
		EName	BlastRing
		Type	Posit
		At	Hips

		part1	:RadiationBlastCircle.part

	End

End

Condition
	On 	Time
	Time 	135
	
	Event
		Type	Destroy
		EName	LightRaysSpillage
	End

End

End				