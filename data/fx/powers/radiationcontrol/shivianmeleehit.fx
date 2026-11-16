#########################################################
##	RadiationAura
FxInfo

LifeSpan	7

Input  
	InpName	Hips
End

########################################################


Condition
	On	Time
	Time	0

	Event	
		Ename	LightRaysSpillage
		Type	Posit
		At	Hips
		part1	:ShivianHitRings.part
		part2	:ShivianHitDots.part
		part3	:RadiationCloudHit.part	
		Sound PunchHit2 80 80 .5
		
		LifeSpan	15
	End
		
		
End


Condition
	On	Time
	Time	4

	Event
		
		Type	Posit
		At	Hips
		part1	:ShivianCircle2.part
		
		LifeSpan	5
	End

End


Condition
	On	Time
	Time	0

	Event
		EName	BlastRing
		Type	Posit
		At	Hips

		part1	:ShivianCircle.part

		LifeSpan	5
	End

End

Condition
	On 	Time
	Time 	13
	
	Event
		Type	Destroy
		EName	LightRaysSpillage
	End

End

End				