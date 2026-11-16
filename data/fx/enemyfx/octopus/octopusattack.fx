#########################################################
##	cot_waterfall_med.fx
##
FxInfo

LifeSpan	200

###########################################################


Condition
	On	Time
	Time	22
	

	Event
		EName	Prime
		Type	start
		At	chest
		Geom	Fx_Spike01
		Bhvr	:Inkprojectile.bhvr
		Magnet	Target
		LookAt	Target
		Part1	:QuillTrail.part
		Part2	:QuillTrailSpecks.part
		Part3	:QuillBubblesTrail.part
		Sound Octopusattack1 270 270 .8
		Sound Octopusattack2 270 270 .8
		Sound Octopusattack3 270 270 .8
	End

End



Condition
	On 	PrimeHit

	Event
		Type	start
		At	target
		
		Part	:HitBurst.part
		Part	:HitBurst2.part
		Part	:BubbleHitBurst.part
		Part	:HitSparksHuge.part
		
		LifeSpan	3

	End

	Event
		EName 	Prime
		Type	Destroy
		

	End

End


End			