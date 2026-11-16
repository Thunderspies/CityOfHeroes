#############################################################
## SporeSleepBombContinuing.fx
#############################################################

FxInfo

LifeSpan		60

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition

	Event
		Type	local
		At	head
		Part	:ChockingSporeDust.part
		Part	:ChockingSporeDust2.part
		Part	:ChockingSporeGlows.part

		LifeSpan		60

	End

End


End