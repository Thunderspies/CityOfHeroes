#########################################################
##	forceBubble
########################################################
FxInfo

LifeSpan	20

########################################################

Condition
	On 	Time
	Time 	10

	Event
		Type	local
		At	Chest
		sound	acid_explo1 80 70 .7
	
		

	End


End


Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	start
		At	root
		
		Part	:AOEAcidHit.part
		Part	:AOEAcidHit.part
		Part	:AOEAcidHit.part
		Part	:AOEAcidHitDark.part
		Part	:AOEAcidHitBubbles.part
		Part	:AOEAcidHitBubblesdark.part
		Part	:AOEAcidHitBubble.part
		
		Part	:AOEAcidHit2.part
		Part	:AOEAcidHitYellow.part
		

	End


End

End