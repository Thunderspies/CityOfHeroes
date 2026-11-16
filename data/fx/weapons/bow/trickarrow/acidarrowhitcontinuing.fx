#############################################################
## AcidArrowHitContinuing.fx
#############################################################

FxInfo

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	arrow
		Type	Local
		At	chest
		Part	:AcidHit.part
		Part	:AcidHitDark.part
		Part	:AcidBubblesHit.part
		Part	:AcidBubblesdarkHit.part
		Part	:AcidBubbleHit.part
		Sound Acid1_loop 90 90 .25

	End

End


End

