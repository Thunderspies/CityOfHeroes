#########################################################
##	template

FxInfo


Flags InheritAnimScale

LifeSpan 7

Condition

	Event
		EName 	Mallet1
		Type	local 
		At	Chest
		bhvr	:EMPBubbleScale.bhvr
		Geom	ElectricalMeleeBubble
		
		LifeSpan	1
		
	End

	Event
		EName 	Mallet2
		Type	start 
		At	chest
		Part	:HitEmber.part
		Part	:HitStar.part
		Part	:HitRing.part
		Part	:HitRing2.part
		Part	:HitGlows2.part
	
		#Sound	PunchHit2 100.0 30.0 .8
	End

End

Condition
	On 	Time
	Time 	2

	Event
		EName 	Mallet2
		Type	start 
		At	chest
	
		Part	:EMPElectricityArch1.part
		Part	:EMPElectricityArch2.part
		LifeSpan	7
	End

End


Condition
	On 	Time
	Time 	7

	Event
		EName	Mallet1
		Type	Destroy
		
	End
	
End

End		