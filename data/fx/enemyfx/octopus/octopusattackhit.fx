#########################################################
##	cot_waterfall_med.fx
##
FxInfo

LifeSpan	200

###########################################################


Condition
	
	Event
		Type	start
		At	Chest
		
		Part	:SMLHitBurst.part
		Part	:SMLHitBurst2.part
		#Part	:BubbleHitBurst.part
		#Part	:HitSparksHuge.part
		Sound OctopusInkHit 100 100 .88
		Sound Mushroombombhit 100 100 .94
		Sound OctopusInkHit 100 100 .9
		LifeSpan	3

	End

End


End			