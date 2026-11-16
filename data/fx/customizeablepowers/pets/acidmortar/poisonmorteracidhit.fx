#############################################################
## PoisonMorterAcidHit.fx
#############################################################

FxInfo
LifeSpan	20

#########################################################

Input
	Inpname	Hips
End

#########################################################


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
		At	Chest

		Part	:AcidHit.part
		Part	:AcidHitDark.part
		Part	:AcidHitBubbles.part
		Part	:AcidHitBubblesdark.part
		Part	:AcidHitBubble.part

		Part	:AcidHit2.part
		Part	:AcidHitYellow.part


	End


End

End